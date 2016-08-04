#include <QDebug>
#include <iostream>
#include "loader.hpp"

namespace loader
{

Loader::Loader()
{
    connect(&m_network_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(OnDownloadFolder(QNetworkReply*)));
}

void Loader::load(const QUrl& folder_url)
{
    m_folder_reply = m_network_manager.get(QNetworkRequest(folder_url));
    qDebug() << "Loading " << folder_url.path(QUrl::ComponentFormattingOption::DecodeReserved) << "\n";
}

void Loader::OnDownloadFolder(QNetworkReply* )
{
    std::cout << "Downloaded" << std::endl;
    m_parser.parse(*m_folder_reply);
}

void Loader::print_status()
{
    qDebug() << m_network_manager.configuration().name() << "\n";
}

}


