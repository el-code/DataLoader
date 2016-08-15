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

        m_server = server_path.substr(0, pos);
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

    void Loader::handle_connect(const boost::system::error_code& err)
    {
        if(!err)
        {
            std::cout << "connected to " << m_server << std::endl;

            std::ostream request_stream(&m_request);
            request_stream << "GET " << m_path << " HTTP/1.1\r\n";
            request_stream << "Host: " << m_server << "\r\n";
            request_stream << "Accept: */*\r\n";
            request_stream << "Connect: keep-alive\r\n";

            std::cout << "request: " << &m_request << std::endl;

            boost::asio::async_write(m_socket, m_request, boost::bind(&Loader::handle_write_request, this, boost::asio::placeholders::error));
        }
        else
        {
            std::cout << "Couldn`t connect to" << m_server << " . Error: " << err.message() << std::endl;
        }
    }

    void Loader::handle_write_request(const boost::system::error_code& err)
    {
        if(!err)
        {
            boost::asio::async_read_until(m_socket, m_reply, "\r\n", boost::bind(&Loader::handle_read_status, this, boost::asio::placeholders::error));
        }
        else
        {
            std::cout << "Cant send request. Message: " << err.message() << std::endl;
        }
    }

    void Loader::handle_read_status(const boost::system::error_code& err)
    {
        if(!err)
        {
            std::istream reply_stream(&m_reply);

            std::string http_version;
            reply_stream >> http_version;
            unsigned short status;
            reply_stream >> status;
            std::string status_message;
            std::getline(reply_stream, status_message);
            if(!reply_stream || http_version.substr(0, 5) != "HTTP/")
            {
                std::cout << "invalid response" << std::endl;
                return;
            }
            if(status != 200)
            {
                std::cout << "Response returned witch sttus code " << status << std::endl;
                return;
            }

            boost::asio::async_read_until(m_socket, m_reply, "\r\n\r\n", boost::bind(&Loader::handle_read_header, this, boost::asio::placeholders::error));
        }
        else
        {
            std::cout << err.message() << std::endl;
        }
    }

    void Loader::handle_read_header(const boost::system::error_code& err)
    {
        if(!err)
        {
            // Process the response headers.
            std::istream response_stream(&m_reply);
            std::string header;
            while (std::getline(response_stream, header) && header != "\r")
              std::cout << header << "\n";
            std::cout << "\n";

            // Write whatever content we already have to output.
            if (m_reply.size() > 0)
              std::cout << &m_reply;

            // Start reading remaining data until EOF.
            boost::asio::async_read(m_socket, m_reply,
                boost::asio::transfer_at_least(1),
                boost::bind(&Loader::handle_read_content, this,
                  boost::asio::placeholders::error));
        }
        else
        {
            std::cout << err.message() << std::endl;
        }
    }

    void Loader::handle_read_content(const boost::system::error_code& err)
    {
        if(!err)
        {
            // Write all of the data that has been read so far.
            std::cout << &m_reply;

            // Continue reading remaining data until EOF.
            boost::asio::async_read(m_socket, m_reply,
                boost::asio::transfer_at_least(1),
                boost::bind(&Loader::handle_read_content, this,
                  boost::asio::placeholders::error));
        }
        else if(err != boost::asio::error::eof)
        {
            std::cout << "Error: " << err.message() << std::endl;
        }
    }
}


