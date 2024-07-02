#include "system_program.h"

int main() {
	FILE *fp;
    char path[1035];
    int num_daemons = 0;

    // Execute the command and redirect the output to a temporary file
    system("ps -efj | grep dspawn | grep -Ev 'tty|pts' | wc -l > temp.txt");

    // Open the file to read the output
    fp = fopen("temp.txt", "r");
    if (fp == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    // Read the output from the file
    if (fgets(path, sizeof(path) - 1, fp) != NULL) {
        num_daemons = atoi(path);
    }

    // Close the file
    fclose(fp);
    // Remove the temporary file
    remove("temp.txt");

    // Format and print the output
    if (num_daemons > 0) {
        printf("Live daemons: %d\n", num_daemons);
    } else {
        printf("No daemon is alive right now.\n");
    }

    return EXIT_SUCCESS;
}