import socket
import zmq

context = zmq.Context()

#  Socket to talk to server
socket = context.socket(zmq.RADIO)
socket.connect("udp://127.0.0.1:5556")

#  Do 10 requests, waiting each time for a response
for request in range(10):
    print("Sending request %s …" % request)
    socket.send(b"dasfdsafdas", group='test')
#    socket.send(f'Hello'.encode('ascii'), group='test')

    #  Get the reply.
    try:
        message = socket.recv(copy=False)
        #message = socket.recv_string(0)
        print("Received request: %s" % message)
    except zmq.Again:
        print('missed a message')
        continue
    print("Received reply %s [ %s ]" % (request, message))

socket.close()
context.term()
