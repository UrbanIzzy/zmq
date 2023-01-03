#ifndef _ZMQ_SERVER_H_
#define _ZMQ_SERVER_H_

#include <iostream>

using namespace std;

namespace zmq_sock {

    enum socket_type {
        eUDP = 0,
        eTCP,
        eTOTAL_TYPES
    };


    class server
    {
    public:
        server(void);
        int bind(socket_type sock_type, string ip, int port);
        int send(string);
        int stop(void);

    private:
        void* context;
        void* sock;

        int _port;
    };
}

#endif
