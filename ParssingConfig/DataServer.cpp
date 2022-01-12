#include "DataServer.hpp"

dataserver::dataserver(){};

std::vector<int>dataserver::getListens()
{
    return this->index_listens;
}

void    dataserver::addListen(int listen)
{
    this->index_listens.push_back(listen);
}

void    dataserver::setListen(int listen)
{
    this->listen = listen;
}

int     dataserver::getListen()
{
    return this->listen;
}

std::string dataserver::getHost()
{
    return this->host;
}

void    dataserver::setHost(std::string &str)
{
    this->host = str;
}

void    dataserver::setServer_name(std::string &str)
{
    this->server_name = str;
}

std::string dataserver::getServer_name()
{
    return this->server_name;
}

int     dataserver::getClient_max_body_size()
{
    return this->client_max_body_size;
}

void    dataserver::setClient_max_body_size(int cmbs)
{
    this->client_max_body_size = cmbs;
}

std::map<int, std::string>  dataserver::getError_page()
{
    return this->error_pages;
}

void    dataserver::setError_page( int &nbr_error, std::string &error_bath)
{
    this->error_pages.insert(std::pair<std::string, int> (error_bath, nbr_error));
}

void    dataserver::setRoot(std::string &ptr)
{
    this->root = ptr;
}

std::string dataserver::getRoot()
{
    return this->root;
}
dataserver::~dataserver(){};