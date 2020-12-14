#/bin/sh
cp /mnt/config/keyboard_emu /home/steam/
cp /mnt/config/typer_back_door.sh /home/steam/
echo "/home/steam/typer_back_door.sh &" >> /etc/init.d/startup/S15audio.sh