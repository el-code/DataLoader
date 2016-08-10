#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include "loader.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    auto args = a.arguments();
    if(args.size() != 2)
    {
        std::cerr << "usage: DataLoader <url to ftp folder>" << std::endl;
        return 1;
    }
    loader::Loader load;

    return a.exec();
}

