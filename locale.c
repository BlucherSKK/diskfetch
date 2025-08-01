#include <stdio.h>
#include <stdlib.h>
#include "diskfetch.h"




Locl df_get_locl()
{
    char *unp_locl = getenv("LANG");

    if(unp_locl == NULL){return EN;}

    switch(unp_locl[0]){
    case 'r':
        switch(unp_locl[1]){
        case 'u': return RU;
        }
    case 'e':
        switch(unp_locl[1]){
        case 'n': return EN;
        }
    default: return EN;
    }
}

char *df_get_lc_msg(Locl lc, Msg msg)
{
    switch(lc){
    case RU: switch(msg){
        case Help: return "\ndiskfetch - это как neofetch, только для дисков.\nсинтаксис:\n\t\tdiskfetch <флаги> <путь до диска>\n\nфлаги:\n\t-h\t- выводит это сообщение\n\t-v\t- выводит версию\n";
        case Version: return "\nверсия - "VER"\n";
        }
    case EN: switch(msg){
        case Help: return "\ndiskfetch - it's like neofetch, but for disk\nsyntax:\n\t\tdiskfetch <flags> <path to your disk>\n\nflags:\n\t-h\t- outputs this information\n\t-v\t- outputs version\n";
        case Version: return "\nversion - "VER"\n";
        }
    }
}


char *df_get_lc_err(Locl lc, Ecode err)
{
    if(err == NOTHING){return NULL;}
    switch(lc){
    case RU: switch(err){
        case UNK_FLAG: return RED"\nОшибка: "RESET"неизвестный флаг\n";
        case UNVALIDABLE_CLI_ARGUMENT: return RED"\nОшибка: "RESET"неправильный аргумент командной строки\n";
        case DISK_NOT_FOUND: return RED"\nОшибка: "RESET"диск не найден\n";
        case GET_SMART_ATA_ERROR: return RED"\nОшибка: "RESET"диск распознан как SATA девайс, но POSIX.1 не отвечает\n";
        case GET_SMART_NVME_ERROR: return RED"\nОшибка: "RESET"диск разпознан как NVME девайс, но POSIX.1 не отвечает\n";
        case DB_NOT_FOUND: return RED"\nОшибка: "RESET"база данный diskfetch.db не найдена\n";
        case UNK_DISK_TYPE: return RED"\nОшибка: "RESET"тип диска не распознан\n";
        default: return RED"\nНеизвестная ошибка\n"RESET;
        }
    case EN: switch(err){
        case UNK_FLAG: return RED"\nError: "RESET"unuknow flag\n";
        case UNVALIDABLE_CLI_ARGUMENT: return RED"\nError: "RESET"invalid CLI argument\n";
        case DISK_NOT_FOUND: return RED"\nError: "RESET"disk not found\n";
        case GET_SMART_ATA_ERROR: return RED"\nError: "RESET"disk found like SATA device, but POSIX.1 not reply\n";
        case GET_SMART_NVME_ERROR: return RED"\nError: "RESET"disk found like NVME device, but POSIX.1 not reply\n";
        case FILE_SISTEM_EROR: return RED"\nError: "RESET"disk not found in fs\n";
        case DB_NOT_FOUND: return RED"\nError: "RESET"data base diskfetch.db not found\n";
        case UNK_DISK_TYPE: return RED"\nError: "RESET"underfind disk type\n";
        default: return RED"\nUnuknow error\n"RESET;
        }
    }
}
