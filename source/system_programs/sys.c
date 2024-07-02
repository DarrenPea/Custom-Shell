#include "system_program.h"

void print_os_info() {
    struct utsname buffer;
    if (uname(&buffer) != 0) {
        perror("uname");
        exit(EXIT_FAILURE);
    }
    printf("OS Name: %s\n", buffer.sysname);
    printf("Kernel Version: %s\n", buffer.release);
    printf("Hostname: %s\n", buffer.nodename);
}

void print_uptime() {
    struct sysinfo s_info;
    int error = sysinfo(&s_info);
    if (error != 0) {
        perror("sysinfo");
        exit(EXIT_FAILURE);
    }
    long uptime = s_info.uptime;
    int days = uptime / (60 * 60 * 24);
    int hours = (uptime % (60 * 60 * 24)) / (60 * 60);
    int minutes = (uptime % (60 * 60)) / 60;
    int seconds = uptime % 60;

    printf("Uptime: %d days, %d hours, %d minutes, %d seconds\n", days, hours, minutes, seconds);
}

void print_user_info() {
    struct passwd *pw;
    uid_t uid;

    uid = geteuid();
    pw = getpwuid(uid);
    if (pw) {
        printf("User: %s\n", pw->pw_name);
    } else {
        perror("getpwuid");
        exit(EXIT_FAILURE);
    }
}

void print_cpu_info() {
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, fp)) != -1) {
        if (strstr(line, "model name") != NULL) {
            printf("CPU: %s", strchr(line, ':') + 2);
            break;
        }
    }

    free(line);
    fclose(fp);
}


void print_memory_info() {
    struct sysinfo s_info;
    int error = sysinfo(&s_info);
    if (error != 0) {
        perror("sysinfo");
        exit(EXIT_FAILURE);
    }
    printf("Total RAM: %lu MB\n", s_info.totalram / (1024 * 1024));
    printf("Free RAM: %lu MB\n", s_info.freeram / (1024 * 1024));
}

int execute() {
    printf(COLOR_CYAN "System Information:\n" COLOR_RESET);
    print_os_info();

    printf(COLOR_CYAN "\nUptime:\n" COLOR_RESET);
    print_uptime();

    printf(COLOR_CYAN "\nUser Information:\n" COLOR_RESET);
    print_user_info();

    printf(COLOR_CYAN "\nCPU Information:\n" COLOR_RESET);
    print_cpu_info();

    printf(COLOR_CYAN "\nMemory Information:\n" COLOR_RESET);
    print_memory_info();

    return 0;
}

int main() {
    execute();
}
