#include <nvme/linux.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include <libnvme.h>
#include <stddef.h>
#include <sqlite3.h>

#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN    "\x1B[36m"
#define WHITE   "\x1B[37m"

#define HEALT_COLOR_100     "\x1B[38;2;0;255;85m"
#define HEALT_COLOR_85      "\x1B[38;2;50;255;0m"
#define HEALT_COLOR_75      "\x1B[38;2;100;255;0m"
#define HEALT_COLOR_60      "\x1B[38;2;150;255;85m"
#define HEALT_COLOR_50      "\x1B[38;2;200;255;85m"
#define HEALT_COLOR_45      "\x1B[38;2;255;255;85m"
#define HEALT_COLOR_30      "\x1B[38;2;255;150;85m"
#define HEALT_COLOR_15      "\x1B[38;2;255;50;85m"
#define HEALT_COLOR_0 "\x1B[38;2;255;0;85m"

#define VER "1.0.3"

#define RESET "\x1B[0m"
/** 
 *   Struct for contain and transfer info about disk
 *   
 * @temp:	        Composite Temperature: Contains a value corresponding
 *			   to a temperature in Celsium that represents the current
 *			   composite temperature of the controller and namespace(s)
 *			   associated with that controller. The manner in which
 *			   this value is computed is implementation specific and
 *			   may not represent the actual temperature of any physical
 *			   point in the NVM subsystem. 
 * @health:	        Percentage Used: Contains a vendor specific estimate
 *			   of the percentage of NVM subsystem life used based on
 *			   the actual usage and the manufacturer's prediction of
 *			   NVM life. A value of 100 indicates that the estimated
 *			   endurance of the NVM in the NVM subsystem has been
 *			   consumed, but may not indicate an NVM subsystem failure.
 *			   The value is allowed to exceed 100.
 * @power_cycles:	Power Cycles: Contains the number of power cycles.
* @size:           Size of disk in bytes.        
**/
struct disk_info_page{
    int                    temp;
    int                    health;
    long long unsigned int power_cycles;
    long long unsigned int size;
    int                    vender;
    long long unsigned int hours;
};

struct disk_db_info{
    int vendor;
    char *name;
    char *color;
    char *ascii_path;
    bool heap;
};

typedef enum{
    RU,
    EN
} Locl;

typedef enum {
  Help,
  Version,

} Msg;

typedef enum{
    NOTHING,
    UNK_FLAG,
    FILE_SISTEM_EROR,
    UNK_DISK_TYPE,
    GET_SMART_NVME_ERROR,
    GET_SMART_ATA_ERROR,
    UNVALIDABLE_CLI_ARGUMENT,
    DB_NOT_FOUND,
    DB_REQUEST_CONSTRUCT_FAIL,
    DB_CUSTOM_MSG,
    DB_PREPARE_FAIL,
    DB_INFO_NOT_FOUND,
    DISK_NOT_FOUND,
} Ecode;


struct disk_info_page get_nvme_info(char path_j[], int pathSize, int* Ecodes);

char *get_health_bar(int health);

char *bd_get_color(char *str);

char *df_get_lc_msg(Locl lc, Msg msg);
Locl df_get_locl();

int bd_info_free(struct disk_db_info *info);

int free_ascii_art(char** art, int lines);

char* get_info_string(int index,const char mask[], struct disk_info_page disk, struct disk_db_info info);

char** get_ascii_art(struct disk_db_info* info, int* len_aski);

int print_disk_info(struct disk_info_page disk_info, char** ascii, int len_ascii, struct disk_db_info info);

int model_to_vender_code(char* model);

void bd_open(sqlite3 **disk_bd, int *ecode);

char *df_get_lc_err(Locl lc, Ecode err);

void bd_put_disk(sqlite3 *disk_db, char *err_msg, int ecode);

struct disk_db_info bd_get_disk_info(sqlite3 *disk_db, int vendor_code, char *err_msg, int ecode);

struct disk_info_page get_sata_info_page(char* path, int* Ecode);
