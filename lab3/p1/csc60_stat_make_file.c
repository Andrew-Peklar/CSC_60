/*************************************************************************\
 *                  Copyright (C) Michael Kerrisk, 2014.                   *
 *                                                                         *
 * This program is free software. You may use, modify, and redistribute it *
 * under the terms of the GNU General Public License as published by the   *
 * Free Software Foundation, either version 3 or (at your option) any      *
 * later version. This program is distributed without any warranty.  See   *
 * the file COPYING.gpl-v3 for details.                                    *
* \*************************************************************************/

/* Author: Andrew Peklar
 * Class:  CSc 60
 * Assing: Lab 2
 * Date:   9/25/2015
*/

#define _BSD_SOURCE     /* Get major() and minor() from <sys/types.h> */
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <grp.h>        /* group */
#include <pwd.h>        /* user */
#include "file_perms.h"
#include "tlpi_hdr.h"

static void displayStatInfo(const struct stat *sb, char *fname) {
    // hold strftime
    char date[40];

    printf("  File: `%s'\n", fname);
    printf("  Size: %lld bytes\tBlocks: %lld\t IO Block: %ld ",
        (long long) sb->st_size,
        (long long) sb->st_blocks,
        (long) sb->st_blksize);

    switch (sb->st_mode & S_IFMT) {
    case S_IFREG:  printf("regular file\n");            break;
    case S_IFDIR:  printf("directory\n");               break;
    case S_IFCHR:  printf("character device\n");        break;
    case S_IFBLK:  printf("block device\n");            break;
    case S_IFLNK:  printf("symbolic (soft) link\n");    break;
    case S_IFIFO:  printf("FIFO or pipe\n");            break;
    case S_IFSOCK: printf("socket\n");                  break;
    default:       printf("unknown file type?\n");      break;
    }

    printf("Device: %xh/%ldd\tInode: %ld\tLinks: %ld\n",
        (unsigned int) sb->st_dev,
        (long int) sb->st_dev,
        (long int) sb->st_ino,
        (long int) sb->st_nlink);

    // %04lo pads 4 digits as zero.
    printf("Access: (%04lo/%s) ",
        (unsigned long) (sb->st_mode & 0002777),
        filePermStr(sb->st_mode, 1));
    printf("Uid: ( %ld/%s )\t",
        (long) sb->st_uid,
        getpwuid(sb->st_uid)->pw_name);
    printf("Gid: ( %ld/%s )\n",
        (long) sb->st_gid,
        getgrgid(sb->st_gid)->gr_name);

    // Access time
    strftime(date, sizeof(date), "%F %T %z", localtime(&sb->st_atime));
    printf("Access: %s\n", date);

    // Modify time
    strftime(date, sizeof(date), "%F %T %z", localtime(&sb->st_mtime));
    printf("Modify: %s\n", date);

    // Changed time
    strftime(date, sizeof(date), "%F %T %z", localtime(&sb->st_ctime));
    printf("Change: %s\n", date);
}

int main(int argc, char *argv[]) {
    struct stat sb;
    Boolean statLink;           /* True if "-l" specified (i.e., use lstat) */
    int fname;                  /* Location of filename argument in argv[] */

    statLink = (argc > 1) && strcmp(argv[1], "-l") == 0;
                                /* Simple parsing for "-l" */
    fname = statLink ? 2 : 1;

    if (argc < 2) cmdLineErr("No file given.\n", -1);

    if (fname >= argc || (argc > 1 && strcmp(argv[1], "--help") == 0))
        usageErr("%s [-l] file\n"
                "        -l = use lstat() instead of stat()\n", argv[0]);

    if (statLink) {
        if (lstat(argv[fname], &sb) == -1)
            errExit("lstat");
    } else {
        if (stat(argv[fname], &sb) == -1)
            errExit("stat");
    }

    displayStatInfo(&sb, argv[fname]);

    exit(EXIT_SUCCESS);
}