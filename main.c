/*
   ________      ___      ________       ___  __        ________  _______       _________    ________      ___  ___     
  |\   ___ \    |\  \    |\   ____\     |\  \|\  \     |\  _____\|\  ___ \     |\___   ___\ |\   ____\    |\  \|\  \    
  \ \  \_|\ \   \ \  \   \ \  \___|_    \ \  \/  /|_   \ \  \__/ \ \   __/|    \|___ \  \_| \ \  \___|    \ \  \\\  \   
   \ \  \ \\ \   \ \  \   \ \_____  \    \ \   ___  \   \ \   __\ \ \  \_|/__       \ \  \   \ \  \        \ \   __  \  
    \ \  \_\\ \   \ \  \   \|____|\  \    \ \  \\ \  \   \ \  \_|  \ \  \_|\ \       \ \  \   \ \  \____    \ \  \ \  \ 
     \ \_______\   \ \__\    ____\_\  \    \ \__\\ \__\   \ \__\    \ \_______\       \ \__\   \ \_______\   \ \__\ \__\
      \|_______|    \|__|   |\_________\    \|__| \|__|    \|__|     \|_______|        \|__|    \|_______|    \|__|\|__|
                                                                                
  by Blucher
  discord - https://discordapp.com/users/657872729126469642/
  github - https://github.com/Blucherx3
*/
#include "diskfetch.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sqlite3.h>

int main(int argSize, char* argv[])
{
    sqlite3 *disks;
    char *bd_msg = malloc(sizeof(char) * 256);
    int Ecode = NOTHING;
    bd_open(&disks, &Ecode);
    if(Ecode == DB_NOT_FOUND){
        printf("Pezda");
        sqlite3_close(disks);
        goto pre_complete;
    }
    
    if(argSize < 2){
        Ecode = UNVALIDABLE_CLI_ARGUMENT;
        goto lets_chek_ecode;
    }

    if((access(argv[1], F_OK) != 0) & (argv[1][0] != '-')){
        Ecode = DISK_NOT_FOUND;
        goto lets_chek_ecode;
    }

    struct disk_info_page disk1;
    switch (argv[1][0]) {
    case '/': switch (argv[1][5]) {
        case 's':
            {
                disk1 = get_sata_info_page(argv[1], &Ecode);
                break;
            }
        case 'n':
            {
                disk1 = get_nvme_info(argv[1], argSize, &Ecode);
                break;
            }
        default:
            perror("\n"RED"Diskfetch: i don`t know what is a disk"RESET"\n");
            return -1;
        }
        break;

    case '-': switch (argv[1][1]) {
        case 'h':
            puts("\ndiskfetch - it's like neofetch, but for disk\nsyntax:\n\t\tdiskfetch <flags> <path to your disk>\n\nflags:\n\t-h\t- outputs this information\n\t-v\t- outputs version\n");
            goto pre_complete;
        case 'v':
            puts("diskfetch-1.0.1");
            goto pre_complete;
        case 'p':
            bd_put_disk(disks, bd_msg, Ecode);
            goto lets_chek_ecode;
        default:
            perror("Error: i don`t know whot is a flag\n");
            return -1;
        }
        break;

    default:
        perror(RED"Error: underfind argument"RESET);
        return -1;
    }
    struct disk_db_info db_info = bd_get_disk_info(disks, disk1.vender, bd_msg, Ecode);

 lets_chek_ecode:
    switch (Ecode) {
    case NOTHING:
        break;
    case UNVALIDABLE_CLI_ARGUMENT:
        perror(RED"\nERROR: invalid command line argument\n"RESET);
        return -1;
    case DISK_NOT_FOUND:
        perror(RED"\nERROR: disk not found\n"RESET);
        return -1;
    case FILE_SISTEM_EROR: 
        perror(RED"\nError: get info about disk from file sistem fail\n"RESET);
        return -1;
    case GET_SMART_NVME_ERROR:
        perror("\nDiskfetch can found yuor disk but:\n"RED"Error: get info about nvme fail\n"RESET);
        return -1;
    case GET_SMART_ATA_ERROR:
        perror("\nDiskfetch can found yuor disk but:\n"RED"Error: get info about sata/ata device fail\n"RESET);
        return -1;
    case DB_CUSTOM_MSG:
        perror(bd_msg);
        return -1;
    default:
        perror(YELLOW"\nUnuknown error\n"RESET);
        return -1;
    }
    
    
    int cont;
    char **ascii = get_ascii_art(&db_info, &cont);
    print_disk_info(disk1, ascii, cont, db_info);
    bd_info_free(&db_info);
    free(ascii);
    sqlite3_close(disks);
    
 pre_complete:
    free(bd_msg);
    return 0;
}
