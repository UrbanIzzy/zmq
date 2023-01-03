#include <iostream>
#include <zmq.h>
//#include "zmq.hpp"

#include <sys\timeb.h>
#include "nlohmann/json.hpp"
#include "zmqServer.h"

using namespace std;
using namespace nlohmann;

#define USE_UDP_SOCKET      1

int main()
{
    int rc;
    struct timespec start, end;
    cout << "*************************************" << endl;
    cout << "\t\tSERVER" << endl;
    cout << "*************************************" << endl;
//    cout << "Using cppzmq version: " << CPPZMQ_VERSION_MAJOR << "." << CPPZMQ_VERSION_MINOR << "." << CPPZMQ_VERSION_PATCH << endl;
    cout << "Using zeromq version: " << ZMQ_VERSION_MAJOR << "." << ZMQ_VERSION_MINOR << "." << ZMQ_VERSION_PATCH << endl;
    cout << "Using nlohmann json version: " << NLOHMANN_JSON_VERSION_MAJOR << "." << NLOHMANN_JSON_VERSION_MINOR << "." << NLOHMANN_JSON_VERSION_PATCH << endl;
    cout << "*************************************" << endl;
    
    /* create DISH socket*/
    void* ctx = zmq_ctx_new();

#ifdef USE_UDP_SOCKET
    void* dish = zmq_socket(ctx, ZMQ_DISH);
    rc = zmq_bind(dish, "udp://*:8081");
    cout << "using udp socket" << endl;
#else
    void* dish = zmq_socket(ctx, ZMQ_REP);
    rc = zmq_bind(dish, "tcp://*:8081");
    cout << "using tcp socket" << endl;
#endif

    cout << "socket @" << dish << endl;
    cout << "zmq_bind: " << rc << endl;
    zmq_sleep(1);

#ifdef USE_UDP_SOCKET
    /* joing message's group */
    rc = zmq_join(dish, "JSON");
    cout << "zmq_join: " << rc << endl;
#endif

    zmq_msg_t msg;
    size_t len = 8542;

    /* recieving message */
    rc = zmq_msg_init(&msg);
    zmq_sleep(1);
    cout << "zmq_msg_init: " << rc << endl;
    int recv_rc = zmq_msg_recv(&msg, dish, 0);

    timespec_get(&start, TIME_UTC);
//    clock_gettime(CLOCK_REALTIME, &start);
    cout << "zmq_msg_recv: expected: " << len << "\tactual: " << recv_rc <<"\t time: " << 1000000000 * start.tv_sec + start.tv_nsec << endl;
 
    if (len != recv_rc)
    {
#ifndef USE_UDP_SOCKET
        zmq_msg_t msg;
        len = sizeof("error...");
        rc = zmq_msg_init_size(&msg, len);
        cout << "zmq_msg_init_size: " << rc << endl;
        memcpy(zmq_msg_data(&msg), "error...", len);
        cout << "message is: " << (char*)&msg << endl;
        size_t bytes;
        bytes = zmq_msg_send(&msg, dish, 0);
#endif
        cout << "ERR: error while receiving message.." << endl;
    }
    else 
    {
        cout << "OK: Message receive complete.." << endl;
    }

    /* release resouces */
    zmq_msg_close(&msg);
    zmq_close(dish);
    zmq_ctx_destroy(ctx);
    return 0;
}
