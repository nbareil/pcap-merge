/*
 * pcap-merge --- stupid simple pcap file merger
 *
 * Copyright (C) 2014  Nicolas Bareil <nico@chdir.org>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */

#include <stdio.h>
#include <stdlib.h>

#include <pcap/pcap.h>

unsigned int n;
pcap_dumper_t *writer;

void process_pcapfile(char *out, char *filename) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *p;

    p = pcap_open_offline(filename, errbuf);
    if (! p) {
        fprintf(stderr, "pcap_open_offline(%s) failed: %s", filename, errbuf);
        return;
    }

    if (n++ == 0) {
        writer = pcap_dump_open(p, out);
        if (! writer) {
            fprintf(stderr, "pcap_dump_open(%s) failed: %s", filename, pcap_geterr(p));
            exit(1);
        }
    }

    while (1) {
        struct pcap_pkthdr *h;
        unsigned char *data;
        int ret;

        ret = pcap_next_ex(p, &h, (const unsigned char **)&data);

        switch (ret) {
            case 1:
                // ok
                pcap_dump((unsigned char *)writer, h, data);
                break;
            case 0:
                // timeout expired
                fprintf(stderr, "timeout");
                break;
            case -1:
                // error while reading packet
                fprintf(stderr, "error while reading packet");
                break;
            case -2:
                // no more packet
                // fprintf(stderr, "no more packet");
                goto close_pcap;
            default:
                // unkown error
                fprintf(stderr, "unkown error %d", ret);
                break;
        }

    }

close_pcap:
    pcap_close(p);
}

int main(int argc, char *argv[]) {
    unsigned int i;

    if (argc < 3) {
        fprintf(stderr, "usage: %s outfile infile1 infile2 infile3 ...\n", argv[0]);
        return 1;
    }

    for (i = 2; i < argc; i++) {
        fprintf(stderr, "reading %s... ", argv[i]);
        process_pcapfile(argv[1], argv[i]);
        fprintf(stderr, "done.\n");
    }

    pcap_dump_close(writer);
    fprintf(stderr, "kthx bye!\n");

    return 0;
}
