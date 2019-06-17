sudo apt-get update
sudo apt-get upgrade



curl -sL http://deb.nodesource.com/setup_8.x | sudo bash -
sudo apt-get install nodejs
curl https://openframeworks.cc/versions/v0.10.1/of_v0.10.1_linuxarmv6l_release.tar.gz --insecure -O
mkdir openFrameworks
tar -xvzf of_v0.10.1_linuxarmv6l_release.tar.gz -C openFrameworks
cd /home/pi/openFrameworks/of_v0.10.1_linuxarmv6l_release/scripts/linux/debian
sudo ./install_dependencies.sh
make Release -C /home/pi/openFrameworks/of_v0.10.1_linuxarmv6l_release/libs/openFrameworksCompiled/project
cd /home/pi/openFrameworks/of_v0.10.1_linuxarmv6l_release/addons
git clone https://github.com/256dpi/ofxMQTT
git clone https://github.com/bakercp/ofxIpVideoGrabber
git clone https://github.com/patriciogonzalezvivo/ofxPiTFT
cd /home/pi/openFrameworks/of_v0.10.1_linuxarmv6l_release/apps
git clone https://github.com/martial/humanstories
make

cd ~
wget https://raw.githubusercontent.com/adafruit/Raspberry-Pi-Installer-Scripts/master/adafruit-pitft.sh
chmod +x adafruit-pitft.sh
sudo ./adafruit-pitft.sh
