#include "DataServer.hpp"

dataserver::dataserver(){
    this->client_max_body_size = -1;
    this->server_name = "";
    this->host = "";
    this->root = "";
};

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



void    dataserver::clear_all()
{
    for(;host.size();)
        this->host.pop_back();
    this->host.erase(host.begin(),host.end());
    this->host.clear();
    this->server_name.erase(server_name.begin(), server_name.end());
    this->server_name.clear();
    this->client_max_body_size = -1;
    this->error_pages.erase(error_pages.begin(), error_pages.end());
    this->error_pages.clear();
    this->root.erase(root.begin(), root.end());
    this->root.clear();
    for (size_t i = 0; i < Location.size();i++)
        Location.clear();
}

void    dataserver::addLocation(std::map<std::string, location> &sv_loc)
{
    std::map<std::string, location>::iterator it = sv_loc.begin();
    this->Location.insert(std::pair<std::string, location>(it->first, it->second));
}

void    dataserver::clearLocation(location &sv_loc)
{
    sv_loc.clearAll();
}

dataserver::~dataserver()
{
    host.clear();
    error_pages.clear();
}