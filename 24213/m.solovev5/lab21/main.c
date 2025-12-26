#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#define type_length(t) (sizeof(t) * 3 + 2)

unsigned int cnt = 0;

void sig_handler(int sig) {
	char message[9 + type_length(unsigned int)];
	int bytes;

	switch (sig) {
		case SIGINT:
			cnt += 1;
			write(STDOUT_FILENO, "\a", 1);
			break;
		case SIGQUIT:
			bytes = sprintf(message, "\nBeeps: %u\n", cnt);
			write(STDOUT_FILENO, message, bytes);
			_exit(0);
			break;
	}
}

int main() {
	if (sigset(SIGINT, sig_handler) == SIG_ERR) {
		perror("Failed setting signal handler");
		exit(1);
	}

	if (sigset(SIGQUIT, sig_handler) == SIG_ERR) {
		perror("Failed setting signal handler");
		exit(1);
	}

	for (;;) {
		pause();
	}
}
