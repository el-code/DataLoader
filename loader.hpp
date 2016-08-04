#ifndef LOADER_HPP
#define LOADER_HPP

#include <QtNetwork>

namespace loader{

class Parser;

class Loader : public QObject
{
    Q_OBJECT
public:
    Loader();
    void print_status();
    void load(const QUrl& folder_url);

private:
    QStringList get_files() const;

private slots:
    void OnDownloadFolder(QNetworkReply*);

private:
    QNetworkAccessManager m_network_manager;
    QNetworkReply* m_folder_reply;
    Parser m_parser;
};

}

#endif // LOADER_HPP

