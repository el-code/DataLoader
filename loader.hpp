#ifndef LOADER_HPP
#define LOADER_HPP

#include <QtNetwork>

namespace loader{

class Loader : public QObject
{
    Q_OBJECT
public:
    Loader();
    void print_status();
    void load_main_folder(const QUrl& folder_url);

private slots:
    void OnDownloadFolder(QNetworkReply*);

private:
    QNetworkAccessManager m_network_manager;
    QNetworkReply* m_folder_reply;
};

}

#endif // LOADER_HPP

