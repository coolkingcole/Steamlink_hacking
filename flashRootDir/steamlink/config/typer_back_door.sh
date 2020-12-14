# Wait for streaming_client to start
until pids=$(pidof streaming_client)
do   
    sleep 5
done
#run keyboard emulation to prove windows execution.
/home/steam/keyboard_emu