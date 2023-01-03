#include <iostream>
#include <fstream>
#include <zmq.h>
#include <time.h>

//#include "zmq.hpp"
#include "nlohmann/json.hpp"

using namespace std;
//using namespace zmq;
using namespace nlohmann;

#define USE_UDP_SOCKET      1

constexpr auto JSON_FILE1 = "C:\\Projects\\zmq\\zmqClient\\zmqClient\\file.json";
constexpr auto JSON_FILE2 = "C:\\Projects\\zmq\\zmqClient\\zmqClient\\file2.json";

int main(int argc, char* argv[])
{
	int	client_id = 2;
	int rc;
	struct timespec start, end;

	if (argc > 1)
	{
		client_id = atoi(argv[1]);
	}

	cout << "*****************************" << endl;
	cout << "\tCLIENT" << " ID: " << client_id << endl;
	cout << "*****************************" << endl;
//	cout << "Using zeromq version: " << CPPZMQ_VERSION_MAJOR << "." << CPPZMQ_VERSION_MINOR << "." << CPPZMQ_VERSION_PATCH << "." << endl;
	cout << "Using zeromq version: " << ZMQ_VERSION_MAJOR << "." << ZMQ_VERSION_MINOR << "." << ZMQ_VERSION_PATCH << endl;
	cout << "Using nlohmann json version: " << NLOHMANN_JSON_VERSION_MAJOR << "." << NLOHMANN_JSON_VERSION_MINOR << "." << NLOHMANN_JSON_VERSION_PATCH << endl;
	cout << "client ID: " << client_id << endl;
	cout << "*****************************" << endl;

	/* create RADIO socket*/
	void *ctx = zmq_ctx_new();
#ifdef USE_UDP_SOCKET
	void *radio = zmq_socket(ctx, ZMQ_RADIO);
	const char* radio_url = "udp://127.0.0.1:8081";
	rc = zmq_connect(radio, radio_url);
	cout << "using udp socket" << endl;
#else
	void* radio = zmq_socket(ctx, ZMQ_REQ);
	const char* radio_url = "tcp://127.0.0.1:8081";
	rc = zmq_connect(radio, radio_url);
	cout << "using tcp socket" << endl;
#endif

	cout << "radio: " << radio << ", connecting to: " << radio_url << endl;
	cout << "zmq_connect: " << rc << endl;

	std::ifstream ifs;
	if (client_id == 1)
	{
		ifs.open(JSON_FILE1);
	}
	else
	{
		ifs.open(JSON_FILE2);
	}

	if (!ifs.is_open())
	{
		cout << "Cannot open json file" << endl;
	}

	json json_file;
	ifs >> json_file;
	string json_req = json_file.dump();
	size_t len = json_req.length();
	cout << "json:" << endl << json_req .c_str() << endl;

	/* prepare message */
	zmq_msg_t msg;
	rc = zmq_msg_init_size(&msg, len);
	cout << "zmq_msg_init_size: " << rc << endl;
	memcpy(zmq_msg_data(&msg), json_req.c_str(), len);
	cout << "message is: " << (char*)&msg << endl;

	/* set message group */
#ifdef USE_UDP_SOCKET
	rc = zmq_msg_set_group(&msg, "JSON");
	cout << "zmq_msg_set_group: " << rc << endl;
#endif
	/* send message */
	size_t bytes;

	int64_t start64, end64;

	timespec_get(&start, TIME_UTC);
	start64 = 1000000000 * start.tv_sec + start.tv_nsec;
	cout << "zmq_msg_send time: " << 1000000000 * start.tv_sec + start.tv_nsec << endl;
	bytes = zmq_msg_send(&msg, radio, 0);
	cout << "zmq_msg_send: expected: " << int(len) << "\tactual bytes sent: " << int(bytes) << endl;

	/* validate message */
	if (len != bytes)
		cout << "ERR: error while sending message.." << endl;

#ifndef USE_UDP_SOCKET

	/* recieving message */
	rc = zmq_msg_init(&msg);
//	cout << "zmq_msg_init: " << rc << endl;
	int recv_rc = zmq_msg_recv(&msg, radio, 0);
	timespec_get(&end, TIME_UTC);
	end64 = 1000000000 * end.tv_sec + end.tv_nsec;
	cout << "total time: " << end64 - start64 << endl;

	
	char str[100];
	memcpy(str, zmq_msg_data(&msg), recv_rc);
	cout << "zmq_msg_recv:\t" << str << endl;
#endif
	/* release resouces */
	zmq_msg_close(&msg);
	zmq_close(radio);

	zmq_ctx_destroy(ctx);
	return 0;
}