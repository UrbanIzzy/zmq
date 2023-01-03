# preconditins
================
cmake 
json (nlohmann)
cppzmq
libzmq


# cmake:
=========
download cmake from gnu,org add path to environment varibles

# json
=======
download json lib from github 

# cppzmq
=========
download cppzmq from github

# libzmq compilation
=====================
1. git clone git://github.com/zeromq/libzmq.git
2. open VS_2019 Developer command prompt
3. create build folder (e.g mkdir build)
4. enter the folder and run cmake ..
5. a solution called zeromq is generated open it on visual studio
6. add processing defination for draft verfsion: ZMQ_BUILD_API_DRAFT
7. compile the zmqlib project
8. output files are in build/lib

# configuring application while using libzmq
1. create new project
2. under C/C++ -> General - > Additional include directories add location for libzmq and libzmq/include
3. under C/C++ -> Preprocessor - > Preprocessor definitions add ZMQ_STATIC and ZMQ_BUILD_DRAFT_API
4. under Linker -> Input - > Additional dependnecies add libzmq-v142-mt-s-4_3_5.lib, ws2_32.lib, IPHLPAPI.lib
