sudo apt-get update
sudo apt-get upgrade
sudo apt-get install mongodb-server
sudo service mongod start

curl -sL http://deb.nodesource.com/setup_8.x | sudo bash -
sudo apt-get install nodejs
curl https://openframeworks.cc/versions/v0.10.1/of_v0.10.1_linuxarmv6l_release.tar.gz --insecure -O
mkdir openFrameworks
tar vxfz of_*_vlinuxarmv6l_release.tar.gz -C openFrameworks --strip-components 1
cd /home/pi/openFrameworks/scripts/linux/debian
sudo ./install_dependencies.sh
make Release -C /home/pi/openFrameworks/libs/openFrameworksCompiled/project
cd /home/pi/openFrameworks/addons
git clone https://github.com/256dpi/ofxMQTT
git clone https://github.com/bakercp/ofxIpVideoGrabber
cd /home/pi/openFrameworks/apps
git clone https://github.com/martial/humanstories
cd /home/pi/openFrameworks/apps/humanstories/MoscaServer
npm install mosca --save

