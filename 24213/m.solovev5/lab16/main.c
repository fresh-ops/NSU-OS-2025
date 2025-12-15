#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

int main() {
	if (!isatty(STDIN_FILENO)) {
		perror("The stdin must be a terminal");
		exit(1);
	}

	struct termios saved_attrs;

	if (tcgetattr(STDIN_FILENO, &saved_attrs) == -1) {
		perror("Cannot get stdin attributes");
		exit(1);
	}

	struct termios new_attrs = saved_attrs;
	new_attrs.c_lflag &= ~ICANON;
	new_attrs.c_cc[VMIN] = 1;
	new_attrs.c_cc[VTIME] = 0;

	if (tcsetattr(STDIN_FILENO, TCSANOW, &new_attrs) == -1) {
		perror("Cannot change stdin attributes");
		exit(1);
	}

	printf("What character would you prefer? >>> ");
	if (fflush(stdout)) {
		perror("Failed flushing the stdout");
		if (tcsetattr(STDIN_FILENO, TCSANOW, &saved_attrs) == -1) {
			perror("Cannot restore stdin attributes");
		}

		exit(1);
	}

	int char_code = getchar();

	if (char_code == EOF) {
		fprintf(stderr, "Error reading a character\n");
		if (tcsetattr(STDIN_FILENO, TCSANOW, &saved_attrs) == -1) {
			perror("Cannot restore stdin attributes");
		}

		exit(1);
	}

	char input = char_code;
	printf("\nThe %c is a good choice\n", input);

	if (tcsetattr(STDIN_FILENO, TCSANOW, &saved_attrs) == -1) {
		perror("Cannot restore stdin attributes");
		exit(1);
	}
	exit(0);
}
