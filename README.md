# Matt's System Control
This is a set of programs designed to be ran as a daemon and from the command like on a linux system to provide control over actions which would normally require root user privilege, such as changing brightness of a laptop's display. It does this through in a safe manner by sanitising all user input.

## Dependencies
The following dependencies are required for this program:
```
systemd
make
```

## Build Instructions
To build this program do the following:

```console
$ git clone https://www.github.com/CG-Matt/msctl.git
$ cd msctl
$ make
```

The compiled binaries can be found in `build/bin`. This will allow you to test the program before installing it.

## Installation instructions
To use this program the daemon service must be installed, this can be done as follows:

```console
# make install
```

If at any point you would like to uninstall the application simply run:

```console
# make uninstall
```

## Using the program
Once the program is installed and started, you can use `msctl` to send commands to the daemon service.

### Commands
Currently only one command has been implemented, it changes the brightness of your devices screen.

To turn up your screen's brightness use: `msctl --brightness --up`.    
To turn down your screen's brightness use: `msctl --brightness --down`.

## Additional configuration
Additional configuration options are available through defines in the `src/config.h` file. These must be set before the program is compiled and installed. If you have already installed the program and would like to make updates to its configuration, please first uninstall the program and then install it again after making changes to the configuration.