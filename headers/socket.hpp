#ifndef _MSOCKET_HPP_
#define _MSOCKET_HPP_

#include "headers.hpp"

#include "DataServer.hpp"

#define BACKLOG 2048
class dataserver;

namespace SERVER
{
	class ASOCKET
	{
	public:
		std::vector<int> _ports;
		dataserver _data_server;
		short _port;
		std::string _host;
		std::vector<int> _masterSockFDs;
		int _masterSockFD;
		struct sockaddr_in _Adrress;
		socklen_t _addrLen;
		fd_set _masterRFDs;
		fd_set _masterWFDS;

		fd_set _readFDs;
		fd_set _writeFDs;

		int _maxSockFD;

	public:
		ASOCKET();
		void SetupSocket(int port);
		void CreatSocket();
		void BindSocket();
		void ListenSocket();
	};

}
#endif