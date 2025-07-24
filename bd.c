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
| INT  | TEXT |  TEXT | TEXT  |


 */


// open base
void bd_open(sqlite3 **disk_bd, int *ecode){
    if (sqlite3_open("assets/diskfetch.db", disk_bd) != SQLITE_OK) {
        *ecode = DB_NOT_FOUND;
    }
    return;
}


void bd_put_disk(sqlite3 *disk_db, char *err_msg, int ecode){
    struct disk_db_info info;

    printf("Enter vendor code:");
    scanf("%d", &(info.vendor));

    char *ptr_name = malloc(sizeof(char)*32);
    printf("Enter vendor name:");
    scanf("%s", info.name);

    char *color_name = malloc(sizeof(char)*32);
    printf("Enter color name:");
    scanf("%31s", color_name);
    switch (color_name[0]) {
    case 'r':{
        info.color = RED;
        break;
    }
    case 'g':{
        info.color = GREEN;
        break;
    }
    case 'c':{
        info.color = CYAN;
        break;
    }
    case 'y':{
        info.color = YELLOW;
        break;
    }
    case 'b':{
        info.color = BLUE;
        break;
    }
    case 'w':{
        info.color = WHITE;
        break;
    }
    case 'm':{
        info.color = MAGENTA;
        break;
    }
    default:{
        info.color = YELLOW;
    }
    }
    free(color_name);

    printf("Enter path to ascii logo:");
    scanf("%s", info.ascii_path);

    printf("Do you want add\n\tvendor: %d\n\tname: %s\n\tcolor: %s\n\tascii: %s\n[y/n]:",
           info.vendor,
           info.name,
           info.color,
           info.ascii_path);
    char acsept;
    scanf("%c", &acsept);
    if(acsept != 'y'){
        if(acsept == 'n'){
            printf("Ok\nAbroat\n");
        }else{
            printf("What?\nAbroat\n");
        }
        return;
    }
    char *request = malloc(sizeof(char) * 256);
    int rc1 = snprintf(request, 256, "INSERT INTO disks (vendor, name, color, ascii) VALUES (%d, %s, %s, %s);",
                       info.vendor,
                       info.name,
                       info.color,
                       info.ascii_path);
    ecode = rc1 ? DB_REQUEST_CONSTRUCT_FAIL : 0;
    int rc2 = sqlite3_exec(disk_db, request, 0, 0, &err_msg);
    free(request);
    ecode = rc2 ? DB_CUSTOM_MSG : 0;
    printf("Done\n");
    return;
}

struct disk_db_info bd_get_disk_info(sqlite3 *disk_db, int vendor_code, char *err_msg, int ecode)
{
    struct disk_db_info resault;
    char *request = malloc(sizeof(char) * 256);
    snprintf(request, 256, "SELECT name, color, ascii FROM disks WHERE vendor = %d;", vendor_code);
    sqlite3_stmt *stm;
    int rc1 = sqlite3_prepare_v2(disk_db, request, -1, &stm, NULL);
    ecode = rc1 ? DB_PREPARE_FAIL : 0;
    if (sqlite3_step(stm) == SQLITE_ROW) {
        const unsigned char *name = sqlite3_column_text(stm, 0);
        const unsigned char *color = sqlite3_column_text(stm, 1);
        const unsigned char *ascii = sqlite3_column_text(stm, 2);
        resault.name = (char *)name;
        resault.color = (char *)color;
        resault.ascii_path = (char *)ascii;
        resault.vendor = vendor_code;
        sqlite3_finalize(stm);
    } else {
        ecode = NOTHING;
        resault.name =  "unuknow";
        resault.ascii_path = "assets/ascii_default.txt";
        resault.color = YELLOW;
        resault.vendor = 0;
        free(request);
    }
    
    return resault;
}

