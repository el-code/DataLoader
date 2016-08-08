#include <QDebug>
#include <QFile>
#include <iostream>
#include "loader.hpp"

namespace loader
{

Loader::Loader()
{
    connect(&m_network_manager, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(OnError(QNetworkReply::NetworkError)));
}

void Loader::load(const QUrl& folder_url)
{
    m_folder_reply = m_network_manager.get(QNetworkRequest(folder_url));
    connect(m_folder_reply, SIGNAL(finished()), this, SLOT(OnDownloadFolder()));
    qDebug() << "Loading " << folder_url.path(QUrl::ComponentFormattingOption::DecodeReserved) << "\n";
}

void Loader::OnDownloadFolder()
{
    std::cout << "Downloaded" << std::endl;
    QStringList files = m_parser.parse(*m_folder_reply);
    const char* moex_ftp_url = "http://ftp.moex.com";
    for(auto elem : files)
    {
        m_urls.push_back(QUrl(moex_ftp_url + elem));
    }
    connect(&m_network_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(OnDownloadFile(QNetworkReply*)));
    download_files();
}

void Loader::download_files()
{
    if(!m_urls.isEmpty())
    {
        m_curr_download =  m_network_manager.get((QNetworkRequest(m_urls.front())));
        connect(m_curr_download, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(OnDownloadProgress(qint64,qint64)));
        m_urls.pop_front();
    }
}

void Loader::OnDownloadFile(QNetworkReply* reply)
{
    QString fname(reply->url().fileName());
    QFile file(fname);
    file.open(QFile::ReadWrite);
    file.write(reply->readAll());
    file.close();

    download_files();
}

void Loader::OnDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    int percent = bytesReceived * 100 / bytesTotal;
    QString fname = m_curr_download->url().fileName();

    std::cout << "\r downloading... " << fname.toStdString() << " -> " << bytesReceived << " / " << bytesTotal << " (" << percent << "%)";
}

void Loader::OnError(QNetworkReply::NetworkError code)
{
    std::cout << "Cause errror while downloading. Error code:" << code << std::endl;
}

void Loader::print_status()
{
    qDebug() << m_network_manager.configuration().name() << "\n";
}

}


