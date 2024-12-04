#include <unistd.h>
#include <argon2.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "protocol.h"
#define PORT 9991

int main(int argc, char**argv) {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = htons(PORT);
	connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));


	if (generate_global_format() != 0) {
		printf("Error generating global format\n");
		return 1;
	}
	if (send_format_to_server(sockfd) != 0) {
		printf("Error sending global_format to server\n");
		return 1;
	}

	struct redir_table_t* redir = malloc_redir_table();
	if (redir == NULL) {
		perror("malloc_redir_table");
		return 1;
	}

	if (generate_format_from_server(sockfd, redir) != 0) {
		printf("Error generating redir table\n");
		return 1;
	}

	
	free_redir_table(redir);
	close(sockfd);
	return 0;
}
