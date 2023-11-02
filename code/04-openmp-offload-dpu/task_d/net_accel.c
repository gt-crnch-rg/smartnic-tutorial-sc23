#include <err.h>
#include <errno.h>

#include <omp.h> 
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define _PORT_ 3310
#define _ITERATIONS_ (10)

int tcp_init(char MODE, char *server_ip)
{
	int fd, ret;
	int enable_reuse = 1;
	socklen_t socklen_;
	struct sockaddr_in sockaddr_;

	server_ip[strlen(server_ip)-1] = '\0';

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) {
		err(-EAGAIN, "failed to crate socket.");
		return -EAGAIN;
	}
	ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, \
					 &enable_reuse, sizeof(enable_reuse));
	if (ret == -1) {
		err(-EAGAIN, "failed to set socket to use.");
		return -EAGAIN;
	}


	sockaddr_.sin_family = AF_INET;
	sockaddr_.sin_port = htons(_PORT_);

	if (MODE == 'c') {
		sockaddr_.sin_addr.s_addr = inet_addr(server_ip);
		ret = connect(fd, (struct sockaddr *)&sockaddr_, sizeof(sockaddr_));
		if (ret == -1) {
			err(-EAGAIN, "failed to connect socket.");
			return -EAGAIN;
		}
	} else {

		sockaddr_.sin_addr.s_addr= INADDR_ANY;

		ret = bind(fd, (struct sockaddr *) &sockaddr_, sizeof(sockaddr_));
		if (ret == -1) {
			err(-EAGAIN, "failed to bind socket.");
			return -EAGAIN;
		}

		ret = listen(fd, 1);
		if (ret == -1) {
			err(-EAGAIN, "failed to listen socket.");
			return -EAGAIN;
		}

		socklen_ = sizeof(sockaddr_);
		fd = accept(fd, (struct sockaddr *)&sockaddr_, &socklen_);
		if (fd == -1) {
			err(-EAGAIN, "failed to listen socket.");
			return -EAGAIN;
		}
	}

	return fd;
}

void ping_pong(int fd, char mode)
{
	int i, m;
	m = 0;

	for (i = 0; i < _ITERATIONS_; ++i) {
		if (mode == 'c') {
			printf("to   server > %02d\n", m);
			send(fd, &m, sizeof(m), 0);
			recv(fd, &m, sizeof(m), 0);
			++m;
		} else {
			recv(fd, &m, sizeof(m), 0);
			++m;
			printf("to   client > %02d\n", m);
			send(fd, &m, sizeof(m), 0);
		}
	}

}

int main(int argc, char *argv[])
{
	int fd;
	char mode;
	char *ip;
	
	if (argc < 2) {
		mode = 's';
	} else if (argc > 2 && argv[1][0] == 'c') {
		mode = 'c';
		ip   = argv[2];
	} else {
		err(-EINVAL, "invalid argument. Leave empty for server. c for client");
		return -EINVAL;
	}

#pragma omp target map(to:ip[0:strlen(ip)]) nowait
{
	fd = tcp_init(mode, ip);

	if (fd < 0) {
		err(-EAGAIN, "init failed.");
		goto err_out;
	}

	puts("connection established.");

	ping_pong(fd, mode);

	close(fd);
err_out:
}

#pragma omp taskwait

	return 0;
}
