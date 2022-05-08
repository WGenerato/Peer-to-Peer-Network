# Peer-to-Peer-Network
The purpose of this project is to implement a centralized peer network using both UDP and TCP connection protocols. 
UDP is a connectionless protocol, which means that it does not require any initial communication between the server and the client. Either entity can send information to the other without establishing any connection prior to transmission. 
TCP is a connection oriented protocol, which means that a connection must be set up between the server and the client before any information can be exchanged. 
The end goal is to develop an index server, which holds a registry of content and which peer device that content can be obtained from. The index server acts as a guide for the peers, while the content downloads will be done between the appropriate peers. 
There will be three peers, where each of them has one specific piece of content stored in its local directory. The peers will interact with the index server by fulfilling one of the predefined functions. The functions are content registration, content downloading, content deregistration, viewing the available content and quitting the program. 
