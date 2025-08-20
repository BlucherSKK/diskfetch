#!/bin/bash

RED='\033[0;31m'    # Красный
GREEN='\033[0;32m'  # Зеленый
YELLOW='\033[0;33m' # Желтый
BLUE='\033[0;34m'   # Синий
RESET='\033[0m'        # Сброс цвета

# установка основных переменных для компиляции, пути к файлам и тд.
FILES="main.c nvme-interface.c health-progres-bar.c output.c ata-interface.c bd.c locale.c"
LIBS="nvme atasmart sqlite3"

ASSETS="/usr/share/diskfetch"
BIN="/usr/bin"

# проаерка локали и установка реплик установшика
case $LANG in
    ru*)
        TITLE="Вас приветствует установщик ${GREEN}diskfetch${RESET}"
        ERROR_ROOT="${RED}Установочный скрипт необходимо запускать из под root${RESET}"
        ASK_1="Вы хотите установить(i) или удолить(u)?(default: i)[i/u]:"
        PRE_EXIT="делать больше нечего"
        FOUND="найдено: "
        NOT_FOUND="не найдено: "
        PKG_ISSUE="Нехватае некоторых зависимостей, устоновить их сейчас?(default: y)[y/n]: "
        PKGM_FOUND="Найден пакетник: "
        PKGM_NOT_FOUND="Пакетник ненайден"
        ;;
    *)
        TITLE="Hello, is it ${RESET}diskfetch${RESET} launcher"
        ERROR_ROOT="${RED}You must exec this script with root permission${RESET}"
        ASK_1="You wish install(i) or uninstall(u)?(default: i)[i/u]:"
        PRE_EXIT="nothing happend"
        FOUND="found: "
        NOT_FOUND="not found: "
        PKG_ISSUE="Few package not found, install now?(default: y)[y/n]: "
        PKGM_FOUND="Package manager found: "
        PKGM_NOT_FOUND="Package manager not found"
        ;;
esac

# проверка на исполнение от рута ибо утелита системная и требуется доступ к диркам в корне
if [ "$EUID" != "0" ]; then
    echo -e $ERROR_ROOT
    exit 1
fi



combine_line() {
    local message=$1
    local state=$2
    local total_length=$(( ${#message} + ${#state} )) 
    local spaces=$(( 60 - total_length ))
         
    local RESET='\033[0m'
    case "$2" in
        "warn")
            local MARK='\033[0;33m'
            ;;
        "fail")
            local MARK='\033[0;31m'
            ;;
        "done")
            local MARK='\033[0;32m'
            ;;
        *)
            local MARK=''
            ;;
    esac
    
            
    if [ $spaces -lt 0 ]; then
        echo -e "\033[0;31mOutput error${RESET}"
        return 1
    fi

    local space_string=$(printf '%*s' "$spaces" '' | tr ' ' ' ')
    echo -e "${message}${space_string}${MARK}[${state}]${RESET}"
}

check_dependenci() {
    local name=$1
    local type=$2
    
    case $type in
        "lib")
            local LIB=("/lib" "/usr/lib" "/usr/local/lib")
            ;;
        "util")
            local LIB=("/bin" "/usr/bin" "/usr/local/bin")
            ;;
    esac
    
    for path in $LIB; do
        if [ -e "$path/$name" ]; then
            combine_line "$FOUND$path/$name" "done"
            return 0  # Возвращает true (0) если файл существует
        fi
    done
    combine_line "$NOT_FOUND$name" "warn"
    return 1  # Возвращает false (1) если ни один файл не существует
}

# вывод приветствия (-е для того чтоб цвета работали)
echo -e $TITLE

# вопрос установка или деинсталяция
echo -n $ASK_1
read OP_FLAG
case "$OP_FLAG" in
    "u")
        echo "удоление $ASSETS $BIN/diskfetch"
        rm -rf $ASSETS
        rm -rf "$BIN/diskfetch"
        exit 0
        ;;
    "" | "i")
        OP_FLAG="i"
        ;;
    *)
        echo $PRE_EXIT
        exit 1
        ;;
esac

LIBATASMART_EST='libatasmart'
SUDO_EST='sudo'
LIBNVME_EST='libnvme'
SQL_EST='sqlite3'
if check_dependenci "libnvme.so" "lib"; then
    LIBNVME_EST=''
fi
if check_dependenci "libatasmart.so" "lib"; then
    LIBATASMART_EST=''
fi
if check_dependenci "libsqlite3.so" "lib"; then
    SQL_EST=''
fi
if check_dependenci "sudo" "util"; then
    SUDO_EST=''
fi

if [ -n "$LIBATASMART_EST$LIBNVME_EST$SUDO_EST$SQL_EST" ]; then
    echo -n $PKG_ISSUE
    read PKG_ISSUE_REQ

    case "$PKG_ISSUE_REQ" in
        "n")
            echo $PRE_EXIT
            exit 0
            ;;
        "y" | "")
            if command -v apt-get >/dev/null 2>&1; then
                combine_line "${PKGM_FOUND}apt" "done"
                apt install $LIBATASMART_EST $LIBNVME_EST $SUDO_EST $SQL_EST
            elif command -v dnf >/dev/null 2>&1; then
                combine_line "${PKGM_FOUND}dnf" "done"
                dnf install $LIBATASMART_EST $LIBNVME_EST $SUDO_EST $SQL_EST
            elif command -v yum >/dev/null 2>&1; then
                combine_line "${PKGM_FOUND}yum" "done"
                yum install $LIBATASMART_EST $LIBNVME_EST $SUDO_EST $SQL_EST
            elif command -v pacman >/dev/null 2>&1; then
                combine_line "${PKGM_FOUND}pacman" "done"
                pacman -S $LIBATASMART_EST $LIBNVME_EST $SUDO_EST $SQL_EST
            elif command -v zypper >/dev/null 2>&1; then
                combine_line "${PKGM_FOUND}zypper" "done"
                zypper install $LIBATASMART_EST $LIBNVME_EST $SUDO_EST $SQL_EST
            elif command -v apk >/dev/null 2>&1; then
                combine_line "${PKGM_FOUND}apk" "done"
                apk install $LIBATASMART_EST $LIBNVME_EST $SUDO_EST $SQL_EST
            else
                combine_line "${PKGM_NOT_FOUND}" "fail"
                exit 1;
            fi
            ;;
    esac
fi

gcc -o3 $FILES -o diskfetch $(echo "$LIBS" | sed 's/ / -l/g' | sed 's/^/-l/') 

mkdir $ASSETS
mv diskfetch $ASSETS/

cp -r assets $ASSETS/

touch $BIN/diskfetch

cat > $BIN/diskfetch << EOF
#!/bin/bash

cd $ASSETS

sudo $ASSETS/diskfetch "\$@"
EOF

chmod +x $BIN/diskfetch


