# Lite TCP

This is a simple tcp server using epoll to enable several client connections.

## Build

```bash
# Get into dev container
# you could specify the target platform 
./dev_amd64
# or
./dev_arm64
# or
./dev_armv7

# then to build the binaries for target platform
# (in container)
# to build
./build.sh
# to build and test
./build_test.sh
# to build and pack
./build_pack.sh
# to build test and pack
./build_test_pack.sh
# to do code static analysis
./build_with_static_analysis.sh
```

## To clang-format your cpp file automatically

Make pre-commit as a git hook, so every time you type `git commit` invokes clang-format. This is done in `dev_core` file.

## Code principal

We need some principals about writing code, [please follow the rules](Develop.md).

## Run example

One host runs tcp server:  

```bash
./mytcp_server
```

Take tcp client to another host, and connect to server (or 127.0.0.1 9999 if on the same host):  

```bash
./mytcp_client <server_ip> <server_port>
```

Then you will see they send each other message, you could reshape the example code for your specific scenario.  
Report a bug or leave a message.
