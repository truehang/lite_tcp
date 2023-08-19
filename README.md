# Lite TCP
This is a simple tcp server using epoll to enable several client connections.
## Build
```bash
mkdir -p build && cd build && cmake ..  
make  
```  

## Run example
One host runs tcp server:  
```bash
./mytcp_server
```  
Take tcp client to another host, and connect to server:  
```bash
./mytcp_client <server_ip> <server_port>
```  
Then you will see they send each other message, you could reshape the example code for your specific scenario.  
Report a bug or leave a message.


