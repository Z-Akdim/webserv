#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include "headers.hpp"

#include "socket.hpp"
#include "Client.hpp"
#include "time.h"
#include "parssingfile.hpp"
#include "request.hpp"
#include "response.hpp"
// #include "DataServer.hpp"

#define BUFFER_SIZE 1024
#define CHUNKED 2
#define running 1

class dataserver;
class Request;
namespace SERVER
{
	class Client;
	class ASOCKET;

	class ASERVER
	{
	private:
		fd_set _masterRFDs;
		fd_set _masterWFDS;
		int _maxSockFD;
		std::vector<int> _masterSockFDs;
		std::vector<int> _ports;
		short _port;
		struct sockaddr_in _Adrress;
		socklen_t _addrLen;

		fd_set _readFDs;
		int _activity;
		std::vector<Client *> _clients;
		std::map<int, std::string> _clientList;
		std::map<int, int> _accptMaster;
		dataserver _data_server;
		ASOCKET _socket;
		/////////////////////////
		Request _requset;

		std::vector<dataserver> data_servers;

	public:
		void launch(std::vector<dataserver> servers);
		void waitClients();
		void newClient(int &sockFD);
		dataserver getDataServer();
		Request getRequest();
		~ASERVER(void);
	};
	
}

#endif