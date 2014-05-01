pcap-merge
==========

pcap-merge is as simple and dummy as its name.

usage:

     $ pcap-merge *pcap | tcpdump -nr -

FAQ
===

Q: mergecap can do it! Is it NIH syndrom?
A: Nope, it just crashes when you have a *lot* of pcap files to merge.

    # mergecap -w myfifo -a $(ls  201*pcap |sort -n)
    mergecap: Can't open 20140119052613.pcap: Too many open files

