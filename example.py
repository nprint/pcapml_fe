import argparse

import pcapml_fe
from pcapml_fe_helpers import *

def main():
    '''
    Reads a pcapng file labeled and sorted with pcapml, presenting traffic samples to 
    the user for features to be extracted from. To test the method on a new dataset
    the only needed change is to load in a different dataset
    '''
    parser = argparse.ArgumentParser()
    parser.add_argument('pcapml_dataset')
    args = parser.parse_args()
    
    for traffic_sample in pcapml_fe.sampler(args.pcapml_dataset):
        extract_info(traffic_sample)

def extract_info(traffic_sample):
    '''
    Each sample contains the sampleID, metadata and a list of packets 
    with their associated timestamps
    '''
    print("Sample ID:", traffic_sample.sid)
    print("Sample metadata:", traffic_sample.metadata)
 
    '''
    iterating over the traffic sample (packets and timestamps)
    '''
    for pkt in traffic_sample.packets:
        # Print packet timestamp and raw bytes
        print(pkt.ts, pkt.raw_bytes)

        dpacket = dpkt_readEther(pkt.raw_bytes)
        dpacket.pprint()

        spacket = scapy_readEther(pkt.raw_bytes)
        print(spacket.summary())
        pass

if __name__ == '__main__':
    main()
