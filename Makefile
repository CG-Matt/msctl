.PHONY: all msctl msctld install uninstall

# Compiler Settings
CC=gcc
CCFLAGS=-Wall -Wextra -Werror
SOURCE_DIR=src
BUILD_DIR=build
BUILD_BIN_DIR=$(BUILD_DIR)/bin

# Installation settings
USR_BIN_DIR=/usr/local/bin
SYSTEM_SERVICE_DIR=/etc/systemd/system

all: msctl msctld

common:
	mkdir -p build/bin

msctl: common
	$(CC) $(CCFLAGS) -o $(BUILD_BIN_DIR)/msctl $(SOURCE_DIR)/msctl/main.c $(SOURCE_DIR)/msctl/args_parser.c $(SOURCE_DIR)/socket_manager/sockmgr.c

msctld: common
	$(CC) $(CCFLAGS) -o $(BUILD_BIN_DIR)/msctld $(SOURCE_DIR)/msctld/main.c $(SOURCE_DIR)/file_manager/filemgr.c $(SOURCE_DIR)/msctld/cmd_handler.c

install: msctl msctld
	cp $(BUILD_BIN_DIR)/msctld $(USR_BIN_DIR)
	cp $(BUILD_BIN_DIR)/msctl  $(USR_BIN_DIR)
	cp extra/msctld.service $(SYSTEM_SERVICE_DIR)
	systemctl enable msctld
	systemctl start msctld

uninstall:
	systemctl stop msctld
	systemctl disable msctld
	rm $(SYSTEM_SERVICE_DIR)/msctld.service
	rm $(USR_BIN_DIR)/msctl
	rm $(USR_BIN_DIR)/msctld