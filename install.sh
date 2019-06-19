sudo apt-get update


curl https://openframeworks.cc/versions/v0.10.1/of_v0.10.1_linuxarmv6l_release.tar.gz --insecure -O
mkdir openFrameworks
tar -xvzf of_v0.10.1_linuxarmv6l_release.tar.gz -C openFrameworks
cd /home/pi/openFrameworks/of_v0.10.1_linuxarmv6l_release/scripts/linux/debian
sudo ./install_dependencies.sh
make Release -C /home/pi/openFrameworks/of_v0.10.1_linuxarmv6l_release/libs/openFrameworksCompiled/project
cd /home/pi/openFrameworks/of_v0.10.1_linuxarmv6l_release/addons
git clone https://github.com/256dpi/ofxMQTT
git clone https://github.com/bakercp/ofxIpVideoGrabber
cd /home/pi/openFrameworks/of_v0.10.1_linuxarmv6l_release/apps
git clone https://github.com/martial/humanstories
make

cd ~
git clone https://github.com/goodtft/LCD-show.git
sudo chmod -R 755 LCD-show
cd LCD-show/
sudo ./LCD35-show
