#! /usr/bin/make -f

LDFLAGS=-lpcap
CFLAGS=-Wall -w -I/usr/include/

all: pcap-merge

%: %.c
	gcc -o $@ $(CFLAGS) $< -lpcap
