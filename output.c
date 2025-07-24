#include "diskfetch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <fstab.h>
#include <linux/fs.h>
#include <stdio.h>
#include <unistd.h>


int print_disk_info(struct disk_info_page disk_info, char** ascii, int len_ascii, struct disk_db_info info)
{   
    for(int i = 0; i < len_ascii; i++){
        if(i < 9){
            printf("%s %s", ascii[i], get_info_string(i, "abcdbefgh", disk_info, info));
        }else{
            printf("%s\n", ascii[i]);
        }
    }
    return 0;
}

int non_enter(char *s)
{
    for(int i = 0; i < strlen(s); i++){
        if(s[i] == '\n'){
            s[i] = ' ';
            break;
        }
    }
    return 0;
}

char** get_ascii_art(struct disk_db_info* info, int* len_aski)
{
    char *path = info->ascii_path;
    char *color = info->color; 

    FILE* ff = fopen(path, "r");

    char *buffer = malloc(sizeof(char)*128*128);
    fread(buffer, 1, 128*128, ff);
    int len_str = 0;
    int qua_str = 0;
    int tmp_len_str = 0;
    for(int i = 0; i <= 128*128; i++){
        if(buffer[i] == '\n'){
            if(len_str > tmp_len_str){
                len_str = tmp_len_str;
            }
            tmp_len_str = 0;
            qua_str++;
        } else if(buffer[i] == '\0'){
            break;
        } else {
            tmp_len_str++;
        }
    }
    char **ascii_art = malloc(sizeof(char *)*qua_str);
    char *ascii_string = malloc(sizeof(char)*qua_str*(len_str+32));
    char *tmp_str_ptr = ascii_string;
    int num_str = 0;
    for(int i = 0; i <= qua_str*len_str; i++){
        if(buffer[i] == '\n'){
            snprintf(&ascii_string[(len_str+32)*num_str], len_str+32, "%s%s%s", color, tmp_str_ptr, RESET);
            ascii_art[num_str] = &ascii_string[(len_str+32)*num_str];
            num_str++;
            tmp_str_ptr = &ascii_string[(len_str+32)*num_str];
        } else if (buffer[i] == '\0'){
            break;
        } else {
            ascii_string[i%len_str] = buffer[i];
        }
    
    }

    fclose(ff);
    free(buffer);

    *len_aski = qua_str;
    return ascii_art;
}

int free_ascii_art(char** art, int lines) 
{
    for (int i = 0; i < lines; i++) {
        free(art[i]);
    }
    free(art);
    return 0;
}

char* get_info_string(int index,const char mask[], struct disk_info_page disk, struct disk_db_info info)
{
    /*
        a - first string with vendor name
        b - -_-_-_-_-_-_-_-_-_-_-_-_
        c - string "HEALTH"
        d - health progres bar
        e - string with size
        f - string with temperature
        g - wtring with power cycles counter
        h - power hours
    */
    if(index > strlen(mask)){
        perror(RED"Error:"RESET" index or mask is unvalidable");                               
    }
    char* resault = malloc(sizeof(char)*128);
    switch (mask[index]) {
        case 'a':
            snprintf(resault, 128, "-----%s-----\n", info.name);
            break;
        case 'b':
            snprintf(resault, 128, HEALT_COLOR_75"-_-_-_-_-_-_-_-_-_-_-_-_\n"RESET);
            break;
        case 'c':
            snprintf(resault, 128, "HEALTH\n");
            break;
        case 'd':
            snprintf(resault, 128, "%s\n", get_health_bar(100 - disk.health));
            break;
        case 'e':
            snprintf(resault, 128, "SIZE: %.2f gb\n", (float)disk.size/(1024 * 1024 * 1024));
            break;
        case 'f':
            snprintf(resault, 128, "TEMPERATURE: %d °C\n", disk.temp);
            break;
        case 'g':
            snprintf(resault, 128, "POWER CYCLES: %llu\n", disk.power_cycles);
            break;
        case 'h':
            snprintf(resault, 128, "HOURS: %llu h\n", disk.hours);
            break;
        default:
            snprintf(resault, 128, " ");
    }

    return resault;

}
