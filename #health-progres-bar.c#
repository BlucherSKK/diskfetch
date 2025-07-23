#include "diskfetch.h"
#include <stdio.h>
#include <stdlib.h>

char* get_health_bar(int health)
{   
    char* resault = malloc(sizeof(char*)*128);
    /*
        [####100%####] - 1
        [####85%###-]  - 2
        [####75%##--]  - 3
        [####60%#---]  - 4
        [####50%----]  - 5
        [###-45%----]  - 6
        [##--30%----]  - 7
        [#---15%----]  - 8
        [----0%----]   - 9
    */
    if(health > 100){
        perror("Error: health - unvalidable argument");
    } else if(health > 85){
        snprintf(resault, 128,
            "["HEALT_COLOR_100"####"RESET"%d%%"HEALT_COLOR_100"####"RESET"]", health);
    } else if(health > 75){
        snprintf(resault, 128,
            "["HEALT_COLOR_85"####"RESET"%d%%"HEALT_COLOR_85"###"RESET"-]", health);
    } else if(health > 60){
        snprintf(resault, 128,
            "["HEALT_COLOR_75"####"RESET"%d%%"HEALT_COLOR_75"##"RESET"--]", health);
    } else if(health > 50){
        snprintf(resault, 128,
            "["HEALT_COLOR_60"####"RESET"%d%%"HEALT_COLOR_60"#"RESET"---]", health);
    } else if(health > 45){
        snprintf(resault, 128,
            "["HEALT_COLOR_50"####"RESET"%d%%----]", health);
    } else if(health > 30){
        snprintf(resault, 128,
            "["HEALT_COLOR_45"###"RESET"-%d%%----]", health);
    } else if(health > 15){
        snprintf(resault, 128,
            "["HEALT_COLOR_30"##"RESET"--%d%%----]", health);
    } else if(health > 0){
        snprintf(resault, 128,
            "["HEALT_COLOR_15"#"RESET"---%d%%----]", health);
    } else {
        snprintf(resault, 128,
            "["HEALT_COLOR_0"DEAD"RESET"]");
    }
    return resault;
    
}