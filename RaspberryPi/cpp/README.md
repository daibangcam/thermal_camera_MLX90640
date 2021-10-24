1.install SDL2 library
./install.sh 

2.set I2C baudrate on /boot/config.txt file
sudo nano /boot/config.txt

add the following in the last line 
dtparam=i2c1_baudrate=1000000

and the reboot raspberrypi

3.build source
make

4.run
./main
