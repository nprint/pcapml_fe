from scapy.all import *
import dpkt

def dpkt_readEther(buf):
    packet = dpkt.ethernet.Ethernet(buf)
    return packet

def scapy_readEther(buf):
    packet = Ether(_pkt=buf)
    return packet