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
    Locl locl = df_get_locl();
    char *bd_msg = malloc(sizeof(char) * 256);
    int Ecode = NOTHING;
    bd_open(&disks, &Ecode);
    if(Ecode == DB_NOT_FOUND){goto lets_chek_ecode;}
    
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
            Ecode = UNK_DISK_TYPE;
            goto lets_chek_ecode;
        }
        break;

    case '-': switch (argv[1][1]) {
        case 'h':
            puts(df_get_lc_msg(locl, Help));
            goto pre_complete;
        case 'v':
            puts(df_get_lc_msg(locl, Version));
            goto pre_complete;
        case 'p':
            bd_put_disk(disks, bd_msg, Ecode);
            goto lets_chek_ecode;
        default:
            Ecode = UNK_FLAG;
            goto lets_chek_ecode;
        }
        break;

    default:
        Ecode = UNVALIDABLE_CLI_ARGUMENT;
        goto lets_chek_ecode;
    }
    struct disk_db_info db_info = bd_get_disk_info(disks, disk1.vender, bd_msg, Ecode);
    if(Ecode != NOTHING){return puts(df_get_lc_err(locl, Ecode));}

 lets_chek_ecode:
    free(bd_msg);
    sqlite3_close(disks);
    if(Ecode != NOTHING){return puts(df_get_lc_err(locl, Ecode));}
    
    int cont;
    char **ascii = get_ascii_art(&db_info, &cont);
    print_disk_info(disk1, ascii, cont, db_info);
    bd_info_free(&db_info);
    free(ascii);
 
 pre_complete:
    return 0;
}
