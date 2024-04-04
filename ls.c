/**
 * A simple rewrite of ls for GNU/Linux
 *
 * Copyright (c) 2024 trev-git
 * This program is published under the MIT license.
 * Refer to the LICENSE file for more information.
 */

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <getopt.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define UNDERLINE "\033[4m"
#define DEFAULT_TEXT "\033[0m"

static int all_flag = 0;
static int long_flag = 0;

void parse_flags(int argc, char **argv);
void usage();
void version();
void long_file_print(char *filename);
void file_type(mode_t mode);
void file_perm(mode_t perm);
void file_date(time_t date);

int main(int argc, char **argv)
{
    parse_flags(argc, argv);

    DIR *current_dir;
    struct dirent *dir;
    current_dir = opendir(".");
    if (current_dir == NULL) exit(EXIT_FAILURE);

    if (long_flag)
    {
        printf(
            UNDERLINE "Permissions" DEFAULT_TEXT " "
            UNDERLINE "User" DEFAULT_TEXT " "
            UNDERLINE "Group" DEFAULT_TEXT " "
            UNDERLINE "Size" DEFAULT_TEXT " "
            UNDERLINE "Date Modified" DEFAULT_TEXT " "
            UNDERLINE "Name" DEFAULT_TEXT " "
            "\n"
        );
    }

    while((dir = readdir(current_dir)) != NULL)
    {
        if (!all_flag && dir->d_name[0] == '.')
        {
            continue;
        }
        if (!long_flag)
        {
            printf("%s  ", dir->d_name);
        }
        else
        {
            long_file_print(dir->d_name);
            printf("\n");
        }
    }

    closedir(current_dir);
}

void parse_flags(int argc, char **argv)
{
    int opt;
    while((opt = getopt(argc, argv, "vhal")) != -1)
    {
        switch (opt)
        {
        case 'v':
            version();
            exit(EXIT_SUCCESS);
        case 'h':
            usage();
            exit(EXIT_SUCCESS);
        case 'a':
            all_flag = 1;
            break;
        case 'l':
            long_flag = 1;
            break;
        }
    }
}

void usage()
{
    printf(
        "Usage:\n"
        "  ls [options] [directory]\n\n"
        "Options:\n"
        "  -l - long format\n"
        "  -a - output all files\n"
        "  -h - this help menu\n"
        "  -v - version\n"
    );
}

void version()
{
    printf(
        "ls\n"
        "Written by Timofey Khokhlov\n"
    );
}

void long_file_print(char *filename)
{
    struct stat *stats = malloc(sizeof(struct stat));
    lstat(filename, stats);
    file_type(stats->st_mode);
    file_perm(stats->st_mode);
    printf("%s ", getpwuid(stats->st_uid)->pw_name);
    printf("%s ", getgrgid(stats->st_gid)->gr_name);
    printf("%5lu ", stats->st_size);
    file_date(stats->st_mtim.tv_sec);
    printf("%s", filename);
    free(stats);
}

void file_type(mode_t mode)
{
    if (S_ISDIR(mode)) printf("d");
    else if (S_ISCHR(mode)) printf("c");
    else if (S_ISLNK(mode)) printf("l");
    else if (S_ISBLK(mode)) printf("b");
    else if (S_ISFIFO(mode)) printf("p");
    else printf("-");
}

void file_perm(mode_t perm)
{
    printf((perm & S_IRUSR) ? "r" : "-");
    printf((perm & S_IWUSR) ? "w" : "-");
    printf((perm & S_IXUSR) ? "x" : "-");
    printf((perm & S_IRGRP) ? "r" : "-");
    printf((perm & S_IWGRP) ? "w" : "-");
    printf((perm & S_IXGRP) ? "x" : "-");
    printf((perm & S_IROTH) ? "r" : "-");
    printf((perm & S_IWOTH) ? "w" : "-");
    printf((perm & S_IXOTH) ? "x" : "-");
    printf("  ");
}

void file_date(time_t date)
{
    char buf[50];
    struct tm *time = localtime(&date);
    strftime(buf, 50, "%b %d %H:%M", time);
    printf("%s  ", buf);
}
