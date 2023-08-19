# Lite TCP
This is a simple tcp server using epoll to enable several client connections.
## Build
```bash
 ./build.sh
``` 
after build, it generates a debian package, you could also install it.
 
## To clang-format your cpp file automatically
Make pre_commit_script as a git hook, so every time you type `git commit` invokes clang-format.
```bash
mkdir -p .git/hooks/
cp pre_commit_script .git/hooks/pre-commit
```
This is very helpful when you want to contribute to this project or maintain your code.
We need some principals about writing code, [please follow the rules](Develop.md).

## Run example
One host runs tcp server:  
```bash
./mytcp_server
```  
Take tcp client to another host, and connect to server (or 127.0.0.1 9999 if on the same host by fault):  
```bash
./mytcp_client <server_ip> <server_port>
```  
Then you will see they send each other message, you could reshape the example code for your specific scenario.  
Report a bug or leave a message.


