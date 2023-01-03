import sys
import time
import zmq

sys_v = sys.version
zmq_v = zmq.__version__
version = zmq.zmq_version()
draft = zmq.DRAFT_API
print(sys_v)
print(zmq_v)
print(version)
print(draft)

context = zmq.Context()
socket = context.socket(zmq.DISH)
#socket = 1000

socket.bind("udp://*:5556")
socket.join('test')

while True:
    #  Wait for next request from client
    try:
        message = socket.recv(copy=False)
        print("Received request: %s" % message)
    except zmq.Again:
        print('missed a message')
        continue

    #  Do some 'work'
    time.sleep(1)

#    Send reply to client
#    socket.send(b"World")
    socket.send_string("World", flags=0, copy=False)
#    socket.send(b"World", flags=0, copy=False, group='test')

socket.close()
context.term()
