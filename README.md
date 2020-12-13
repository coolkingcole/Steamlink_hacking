# Steamlink_hacking

## Prior work
It was already known that placing a flash drive in a steamlink with a specific file structure is how the Steamlink can be updated (https://support.steampowered.com/kb_article.php?ref=3966-EFLV-7805) or ssh can be enabled with a non-empty file ```/steamlink/config/system/enable_ssh.txt``` (https://github.com/garbear/kodi-steamlink). 

## My work
If the steamlink is booted with a flashdrive with the ```enable_ssh.txt``` file on it in the right place you can get roo ssh access with the password steamlink123. This made working with the steamlink very easy. 

I was able to find another special directory structure that can be placed on a flashdrive that allows any bash script to be ran. Found here()()()(). A file placed on a flashdrive at ```/steamlink/factory_test/``` will be ran at boot, this makes it very easy to drop files onto the system and maintaine persistance if needed.

## POC
###Goal
Establish persistance and execute code on Windows hosts connected to the steamlink with UAC bypass

### How it works
The file I place here for my POC is `run.sh` this is a very simple script that copies a compiled binary placed in `/steamlink/config/keyboard_emu` and a script `/steamlink/config/typer_back_door.sh` into the home directory of a user named steam on the steamlink. Then the run.sh bash script appends `/home/steam/typer_back_door.sh &` to the end of one of the init.d startup scripts so that `typer_back_door.sh` will be ran in the background each boot.
