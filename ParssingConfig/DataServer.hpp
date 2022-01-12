#ifndef DATASERVER_HPP
#define DATASERVER_HPP
#include "parssingfile.hpp"

class dataserver
{
    friend class FileParss;
    private:
        int client_max_body_size;
        int listen;
        std::vector<int> index_listens;
        std::string host;
        std::string server_name;
        std::map<int, std::string> error_pages;
        std::string root;

    public:
        dataserver();
        ~dataserver();

        std::vector<int> getListens();
        void    addListen(int);
        int     getListen();
        void    setListen(int);
        void    setHost(std::string &);
        std::string   getHost();
        void    setServer_name(std::string &);
        std::string   getServer_name();
        void    setClient_max_body_size(int);
        int     getClient_max_body_size();
        void    setError_page(int &, std::string &);
        std::map<int, std::string>  getError_page();
        void    setRoot(std::string &);
        std::string getRoot();

};

#endif