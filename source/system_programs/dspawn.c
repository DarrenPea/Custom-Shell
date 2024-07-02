#include "system_program.h"

char output_file_path[PATH_MAX];

static int daemon_work()
{

    int num = 0;
    FILE *fptr;
    char *cwd;
    char buffer[1024];

    // write PID of daemon in the beginning
    fptr = fopen(output_file_path, "a");
    if (fptr == NULL)
    {
        return EXIT_FAILURE;
    }

    fprintf(fptr, "Daemon process running with PID: %d, PPID: %d, opening logfile with FD %d\n", getpid(), getppid(), fileno(fptr));

    // then write cwd
    cwd = getcwd(buffer, sizeof(buffer));
    if (cwd == NULL)
    {
        perror("getcwd() error");
        return 1;
    }

    fprintf(fptr, "Current working directory: %s\n", cwd);
    fclose(fptr);

    while (1)
    {

        // use appropriate location if you are using MacOS or Linux
        fptr = fopen(output_file_path, "a");

        if (fptr == NULL)
        {
            return EXIT_FAILURE;
        }

        fprintf(fptr, "PID %d Daemon writing line %d to the file.  \n", getpid(), num);
        num++;

        fclose(fptr);

        sleep(10);

        if (num == 10) // we just let this process terminate after 10 counts
            break;
    }

    return EXIT_SUCCESS;
}

int main() {
	// Setup path
    if (getcwd(output_file_path, sizeof(output_file_path)) == NULL)
    {
        perror("getcwd() error, exiting now.");
        return 1;
    }
    strcat(output_file_path, "/dspawn.log"); 

	pid_t pid;

	pid = fork();

	if (pid < 0) {
		printf("Fork failed.");
		exit(1);
	}
	// parent process
	else if (pid > 0) {
		exit(EXIT_SUCCESS);
	}
	// child process
	else if (pid == 0) {
		if (setsid() < 0) {
			exit(EXIT_FAILURE);
		}

		signal(SIGCHLD, SIG_IGN);
		signal(SIGHUP, SIG_IGN);

		pid = fork();

		if (pid < 0) {
			printf("Second Fork failed");
			exit(1);
		}
		// child process
		else if (pid == 0) {
			
			umask(0);

			chdir("/");

			int x, fd0, fd1, fd2;
			for (x = sysconf(_SC_OPEN_MAX); x>=0; x--) {
				close (x);
			}

			/*
			* Attach file descriptors 0, 1, and 2 to /dev/null. */
			fd0 = open("/dev/null", O_RDWR);
			fd1 = dup(0);
			fd2 = dup(0);

			daemon_work();
		}
		// parent process
		else if (pid > 0) {
			exit(EXIT_SUCCESS);
		}
	}
}