#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "diskfetch.h"
#include <string.h>

/*
disks
-------------------------------
|vendor| name | color | ascii |
|------|------|-------|-------|
| INT  | TEXT |  INT  | TEXT  |


 */


// open base
void bd_open(sqlite3 *disk_bd, int ecode){
    if (sqlite3_open("diskfetch.db", &disk_bd) != SQLITE_OK) {
        ecode = DB_NOT_FOUND;
    }
    return;
}


void bd_put_disk(sqlite3 *disk_db, struct disk_db_info *info, char *err_msg, int ecode){
    char *request = malloc(sizeof(char) * 256);
    int rc1 = snprintf(request, 256, "INSERT INTO disks (vendor, name, color, ascii) VALUES (%d, %s, %d, %s);",
                       info->vendor,
                       info->name,
                       info->color,
                       info->ascii_path);
    ecode = rc1 ? DB_REQUEST_CONSTRUCT_FAIL : 0;
    int rc2 = sqlite3_exec(disk_db, request, 0, 0, &err_msg);
    free(request);
    ecode = rc2 ? DB_CUSTOM_MSG : 0;
    return;
}

void bd_get_disk_info(sqlite3 *disk_db, int vendor_code, struct disk_db_info *info, char *err_msg, int ecode){
    char *request = malloc(sizeof(char) * 256);
    snprintf(request, 256, "SELECT name, color, ascii FROM disks WHERE vendor = %d;", vendor_code);
    sqlite3_stmt *stm;
    int rc1 = sqlite3_prepare_v2(disk_db, request, -1, &stm, 0);
    ecode = rc1 ? DB_PREPARE_FAIL : 0;
    if (sqlite3_step(stm) == SQLITE_ROW) {
        const unsigned char *name = sqlite3_column_text(stm, 0);
        const int color = sqlite3_column_int(stm, 1);
        const unsigned char *ascii = sqlite3_column_text(stm, 2);
        info->name = (char *)name;
        info->color = color;
        info->ascii_path = (char *)ascii;
        info->vendor = vendor_code;
    } else {
        ecode = DB_INFO_NOT_FOUND;
        return;
    }
    return;
}

