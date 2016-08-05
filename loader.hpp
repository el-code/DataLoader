#ifndef LOADER_HPP
#define LOADER_HPP

#include <QtNetwork>
#include "parser.hpp"

namespace loader{

class Loader : public QObject
{
    Q_OBJECT
public:
    Loader();
    void print_status();
    void load(const QUrl& folder_url);

private:
    QStringList get_files() const;
    void download_files();

private slots:
    void OnDownloadFolder();
    void OnDownloadFile(QNetworkReply*);

private:
    QNetworkAccessManager m_network_manager;
    QNetworkReply* m_folder_reply;
    parser::Parser m_parser;
    QQueue<QUrl> m_urls;
};

}

#endif // LOADER_HPP

