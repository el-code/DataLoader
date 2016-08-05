#include <QDebug>
#include <QFile>
#include <iostream>
#include "loader.hpp"

namespace loader
{

Loader::Loader()
{
    connect(&m_network_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(OnDownloadFile(QNetworkReply*)));
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
    for(auto elem : files)
    {
        m_urls.push_back(QUrl("http://ftp.moex.com" + elem));
    }
}

void Loader::download_files()
{
    if(!m_urls.isEmpty())
    {
        m_network_manager.get((QNetworkRequest(m_urls.front())));
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

void Loader::print_status()
{
    qDebug() << m_network_manager.configuration().name() << "\n";
}

}


