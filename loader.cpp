#include <QDebug>
#include <QFile>
#include <iostream>
#include "loader.hpp"

namespace loader
{
    Loader::Loader(boost::asio::io_service& io_service) : m_resolver(io_service), m_socket(io_service)
    {}

    void Loader::load(const std::string &url)
    {
        std::string::size_type pos = url.find('/');
        if(pos == std::string::npos)
        {
            std::cerr << "Invalid url" << std::endl;
            return;
        }

        std::string server_path = url.substr(pos + 2);
        pos = server_path.find('/');
        if(pos == std::string::npos)
        {
            std::cerr << "Invalid url" << std::endl;
            return;
        }

        m_server = server_path.substr(0, pos - 1);
        m_path = server_path.substr(pos);

        tcp::resolver::query query(m_server, "http");

        m_resolver.async_resolve(query, boost::bind(&Loader::handle_resolve, this, boost::asio::placeholders::error, boost::asio::placeholders::iterator));
    }

    void Loader::handle_resolve(const boost::system::error_code &error, boost::asio::ip::basic_resolver<tcp>::iterator endpoint_iterator)
    {
        if(!error)
        {
            // trying to establish connecting
            boost::asio::async_connect(m_socket, endpoint_iterator, boost::bind(&Loader::handle_connect, this, boost::asio::placeholders::error));
        }
        else
        {
            std::cout << "Error while resolving server name. Description: " << error.message() << std::endl;
        }
    }
}


