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
#define HEALT_COLOR_0       "\x1B[38;2;255;0;85m"

#define RESET   "\x1B[0m"
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
  int color;
  char *ascii_path;
};

enum Ecodec{
    NOTHING = 0,
    FILE_SISTEM_EROR = 101,
    GET_SMART_NVME_ERROR = 102,
    GET_SMART_ATA_ERROR = 103,
    UNVALIDABLE_CLI_ARGUMENT = 104,
    DB_NOT_FOUND,
    DB_REQUEST_CONSTRUCT_FAIL,
    DB_CUSTOM_MSG,
    DB_PREPARE_FAIL,
    DB_INFO_NOT_FOUND,
    DISK_NOT_FOUND = 404,
};

enum VendorCodes{
    WESTERN_DIGITAL_VCODE = 0x10DE,
    SAMSUNG_VCODE = 0x144D,
    KINGSTON_VCODE = 0x1E0F,
    INTEL_VCODE = 0x8086,
    SEAGATE_VCODE = 0x1B4B,
    SANDISK_VCODE = 0x15B7,
    UNDERFIND_VCODE = -1
};

struct disk_info_page get_nvme_info(char path_j[], int pathSize, int* Ecodes);

char* get_vender_name(int v_code);

char* get_health_bar(int health);

int free_ascii_art(char** art, int lines);

char* get_info_string(int index,const char mask[], struct disk_info_page disk);

char** get_ascii_art(int v_code, int* len_aski);

int print_disk_info(struct disk_info_page disk_info, char** ascii, int len_ascii);

int model_to_vender_code(char* model);

struct disk_info_page get_sata_info_page(char* path, int* Ecode);
