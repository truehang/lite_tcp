From ubuntu:jammy
MAINTAINER Hogan Chou <hoganchous@gmail.com>
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && \
apt-get install -y sudo libevent-dev libssl-dev \
build-essential vim git cmake file clang-format clang-tidy
RUN useradd -m -u 1000 --shell /bin/bash user && adduser user sudo \
	&& echo "user:user" | chpasswd
USER user
WORKDIR /home/user

