#include "diskfetch.h"
#include <atasmart.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>


struct disk_info_page get_sata_info_page(char* path, int* Ecode)
{
    /*
        0 1 2 3 4 5 6 7
        / d e v / s d a
    */
    struct disk_info_page resault;
    struct SkDisk *disk;
    
    int codes[5];

    sk_disk_open(path, &disk);
    
    int kek = sk_disk_smart_read_data(disk);
    if(kek != 0){
        perror(RED"\nError: disk dont`t has SMART parametrs\n"RESET);
        goto exit;
    }

    uint64_t *size = malloc(sizeof(uint64_t));
    codes[0] = sk_disk_get_size(disk, size);
    resault.size = *(long long unsigned int*)size;
    free(size);

    uint64_t *temperature = malloc(sizeof(uint64_t));
    codes[1] = sk_disk_smart_get_temperature(disk, temperature);
    resault.temp = (((int)*(__u32*)temperature)/1000) - 273;
    free(temperature);

    uint64_t count_bad_sectors;
    codes[2] = sk_disk_smart_get_bad(disk, &count_bad_sectors);
    long long unsigned int sector_size;
    char d_path[4] = {path[5], path[6], path[7], '\0'};
    char path_sector_size[64];
    snprintf(path_sector_size, 64,
        "/sys/block/%s/queue/logical_block_size", d_path);
    FILE* ff = fopen(path_sector_size, "r");
    fscanf(ff, "%llu", &sector_size);
    fclose(ff);
    resault.health = (int)(count_bad_sectors/(resault.size/sector_size)) * 100;

    uint64_t power_c;
    codes[3] = sk_disk_smart_get_power_cycle(disk, &power_c);
    resault.power_cycles = *(__u64*)&power_c;

    uint64_t *power_h = malloc(sizeof(uint64_t));
    codes[4] = sk_disk_smart_get_power_on(disk, power_h);
    resault.hours = *(__u32*)power_h/3600000;
    free(power_h);

    char model_str[64];
    char path_vendr[64];
    snprintf(path_vendr, 64,
        "/sys/block/%s/device/model", d_path);
    FILE* fd = fopen(path_vendr, "r");
    fscanf(fd, "%s", model_str);
    fclose(fd);
    resault.vender = model_to_vender_code(model_str);
    
    sk_disk_free(disk);
    
    

    int sum_codes = 0;
    for(int* i = codes; i <= &codes[4]; i++){
        sum_codes += *i;
    }
    
    *Ecode = sum_codes!=0 ? GET_SMART_ATA_ERROR : 0;

    return resault;
    
    exit:
    Ecode = (int*)GET_SMART_ATA_ERROR;
    return resault;

    
}

int model_to_vender_code(char* model)
{
    switch (model[0]) {
    case 'W':
        switch (model[1]) {
        case 'D': return 0x10DE;
        default: return 0;
        }
    case 'S':
        switch (model[1]) {
        case 'T': return 0x1B4B;
        default: return 0;
        }
    default: return 0;
    }
}
