#include <netinet/ip.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

const char *hello = "Hello world!\n";

int collatz(int n)
{
	if (n == 1)
		return 1;
	else if (n & 1)
		return collatz(3 * n + 1);
	else
		return collatz(n / 2);
}

void server(void);

int main(int argc, char *argv[])
{
	if (argc == 1) {
		printf(hello);
	} else if (strcmp(argv[1], "-s") == 0) {
		server();
	} else if (strcmp(argv[1], "-c") == 0) {
		*(int *)NULL = 0;
	} else {
		int n = atoi(argv[1]);
		printf("collatz(%d) = %d\n", n, collatz(n));
	}
	return 0;
}

void server(void)
{
	int sock, ssock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	fd_set fdset;
	struct sockaddr_in bindaddr = {.sin_family = AF_INET,
	                               .sin_port = htons(12345),
	                               .sin_addr = INADDR_ANY};

	int flag = 1;
	setsockopt(ssock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof flag);
	bind(ssock, (struct sockaddr *)&bindaddr, sizeof bindaddr);
	listen(ssock, 1);
	sock = accept(ssock, NULL, NULL);

	FD_ZERO(&fdset);
	FD_SET(sock, &fdset);
	while (select(sock + 1, &fdset, NULL, NULL, NULL) != 0) {
		char buf[1024];
		int n, ret;

		ret = read(sock, buf, 1024);
		if (ret == 0) break;
		buf[ret] = 0;
		n = atoi(buf);
		sprintf(buf, "%d\n", collatz(n));
		write(sock, buf, strlen(buf));
	}

	close(sock);
	close(ssock);
}
