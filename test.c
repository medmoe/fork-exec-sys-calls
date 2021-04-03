#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("usage: %s sleep_time repeat_factor\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	int sleep_time = atoi(argv[1]); // sleep time in seconds
	if (sleep_time < 1) {
		fprintf(stderr, "%s: sleep time must be at least 1 sec.\n", argv[0]);
		return EXIT_FAILURE;
	}

	int repeat_factor = atoi(argv[2]);
	if (repeat_factor < 1) {
		fprintf(stderr, "%s: repeat factor must be at least 1 time.\n", argv[0]);
		return EXIT_FAILURE;
	}

	printf("--Ready to sleep %d s for %d time(s)\n", sleep_time, repeat_factor);

	for (int i=0; i<repeat_factor; i++)
		sleep(sleep_time);

	printf("--I have slept %d s for %d time(s)\n", sleep_time, repeat_factor);

	return EXIT_SUCCESS;
}
