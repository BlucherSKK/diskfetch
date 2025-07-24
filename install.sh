#!/bin/sh

FILES="main.c nvme-interface.c health-progres-bar.c output.c ata-interface.c bd.c"
LIBS="nvme atasmart sqlite3"


gcc -o3 $FILES -o diskfetch $(echo "$LIBS" | sed 's/ / -l/g' | sed 's/^/-l/') 


mkdir /usr/local/bin/diskfetch 
mv diskfetch /usr/local/bin/diskfetch/ 

cp -r assets /usr/local/bin/diskfetch/

touch /usr/bin/diskfetch

cat > /usr/bin/diskfetch << EOF
#!/bin/bash

cd /usr/local/bin/diskfetch

sudo /usr/local/bin/diskfetch/diskfetch "\$@"
EOF

chmod +x /usr/bin/diskfetch

