**Cathook is a Linux cheat for TF2 designed to be configurable and compete with the best.**

# Disclaimer
Please read through this README if you're having any issues. Your question is likely already answered in it.
I know that the style sucks. There is a lot of design errors and cancerous patterns. Most of the code was intended to be *temporary*.

# You need g++\-6 to compile/use cathook

### Full install script for ubuntu (installs g++\-6 and cathook)
```
sudo apt-get update && \
sudo apt-get install build-essential software-properties-common -y && \
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y && \
sudo apt-get update && \
sudo apt-get install gcc-snapshot -y && \
sudo apt-get update && \
sudo apt-get install gcc-6 g++-6 g++-6-multilib -y && \
git clone --recursive https://github.com/nullifiedcat/cathook && \
cd cathook && \
make -j4
```

### Updating cathook
Navigate into cathook directory (where src, makefile and other files are) and run:
```
git pull origin master && \
git submodule update --remote --recursive && \
make clean && \
make -j4
```

# Injection
`sudo ./attach` to attach to tf2 process (can take argument number 0-N - # of tf2 instance to attach to (for bots))

`sudo ./attach-backtrace` to attach and print backtrace if tf2 crashes. Some users reported that this method makes you get less FPS ingame.

# Followbots

To run followbots, you need to download and install `cathook-ipc-server`.

### Installing script
```
git clone --recursive https://github.com/nullifiedcat/cathook-ipc-server && \
cd cathook-ipc-server && \
make -j4
```
### Updating script is the same as updating cathook

### Running followbot server
`./bin/cathook-ipc-server` or `./bin/cathook-ipc-server &>/dev/null &` to run it in background

# Still have questions? Want to chat with other cathook users?
Visit the [Official Discord Server](https://discord.gg/RywBUSc)!
