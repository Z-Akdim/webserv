#ifndef PARSSINGFILE_HPP
#define PARSSINGFILE_HPP

#include "library.hpp"
#include "DataServer.hpp"
// #include "location.hpp"

#define SERVER "server"
#define OPEN_BRACKET "["
#define CLOSE_BRACKET "]"
#define OPEN_BRACE "{"
#define CLOSE_BRACE "}"
#define LISTEN "listen"
#define HOST "host"
#define SERVER_NAME "server_name"
#define CLIENT_MAX_BODY_SIZE "client_max_body_size"
#define ERROR_PAGE "error_page"
#define ROOT "root"
#define LOCATION "location"
#define AUTOINDEX "autoindex"
#define INDEX "index"
#define ALLOW_METHODS "allow_methods"
#define FASTCGI_PASS "fastcgi_pass"
#define COMMENT1 ';'
#define COMMENT2 '#'
#define PATH_CONFIG_FILE "config/"
#define EXTENTION_FILE "conf"
#define INVALIDE_FILE_NAME "Error: File Name : Invalide :("

class dataserver;
class location;

class ParssFile
{
    private:
        std::string extention;
        std::string file_name;
        std::vector<std::string> content_file;
        std::vector<int> index_server;
        std::vector<dataserver> servers;
    public:
        ParssFile(int ac, char **arg);
        ~ParssFile();
        void    check_argument(int ac, char** av);
        void    check_bracket_brace_file();
        void    fill_file_content();
        void    remove_spaces(std::string &);
        void    delete_cmments(std::string &, char);
        void    find_OpenClose_EachServer();
        void    take_port(std::string &, dataserver&);
        void    take_host(std::string &, dataserver&);
        void    take_server_name(std::string &, dataserver&);
        void    take_C_M_B_S(std::string &, dataserver&);
        void    take_Error_Page(std::string &, dataserver&);
        void    take_Root(std::string &, dataserver&);
        /**********************************************/
        void    take_L_autoindex(std::string &, location &);
        void    take_L_index(std::string &, location &);
        void    take_L_fastcgi_pass(std::string& , location &);
        void    take_L_Allow_Methods(std::string &, location &);


        // void    _print(std::vector<std::string> line);
        // void    _print(std::string type);
};


int lenght_int(int nbr);

#endif