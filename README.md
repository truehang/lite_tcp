# Lite TCP
This is a simple tcp server using epoll to enable several client connections.
## Build
```bash
 ./build.sh
```  
## To clang-format your cpp file automatically
Make pre_commit_script as a git hook, so every time you type `git commit` invokes clang-format.
```bash
mkdir -p .git/hooks/
cp pre_commit_script .git/hooks/pre-commit
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


