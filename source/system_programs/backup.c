#include "system_program.h"

int main() {
    // Get the value of the BACKUP_DIR environment variable
    char* backup_dir = getenv("BACKUP_DIR");
    if (backup_dir == NULL) {
        printf("Error: BACKUP_DIR environment variable not set.\n");
        return 1;
    }

    char* pwd = getenv("PWD");
    char* archive_dir = (char*)malloc(PATH_MAX); // Allocate with PATH_MAX size for safety
    snprintf(archive_dir, PATH_MAX, "%s/archive", pwd);

    if (mkdir(archive_dir, 0777) != 0) {
        if (errno != EEXIST) {
        	perror("mkdir");
        	exit(EXIT_FAILURE);
    	}
    }

	// Get the current date and time formatted as YYYYMMDDHHMMSS
    time_t now;
    struct tm *timestamp;
    char timestamp_str[15 + 1]; // For YYYYMMDDHHMMSS + '\0'
    
    now = time(NULL);
    timestamp = localtime(&now);
    strftime(timestamp_str, sizeof(timestamp_str), "%Y%m%d%H%M%S", timestamp);

    // Create the zip file name with the timestamp
    char zip_filename[256];
    snprintf(zip_filename, sizeof(zip_filename), "%s/backup-%s.zip", archive_dir, timestamp_str);

	char zip_archivename[128];
	snprintf(zip_archivename, sizeof(zip_archivename), "./archive/backup-%s.zip", timestamp_str);

    // Create a command to zip the directory
    char zip_command[512];
	struct stat sb;
	if (stat(backup_dir, &sb) == 0) {
		if (S_ISDIR(sb.st_mode)) {
			snprintf(zip_command, sizeof(zip_command), "zip -r \"%s\" \"%s\"", zip_filename, backup_dir);
		}
		else if (S_ISREG(sb.st_mode)) {
			snprintf(zip_command, sizeof(zip_command), "zip \"%s\" \"%s\"", zip_filename, backup_dir);
		}
	}
	else {
		perror("stat");
        return 1;
	}

    printf("Creating backup of '%s' at '%s'\n", backup_dir, zip_archivename);

    // Execute the zip command
    int result = system(zip_command);
    if (result != 0) {
        printf("Error creating zip file.\n");
        return 1;
    }

	printf("Backup created successfully.\n");
    return 0;
}
