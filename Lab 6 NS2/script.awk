BEGIN{
    TCPSent = 0;
    TCPRecieved = 0;
    TCPLost = 0;
    UDPSent = 0;
    UDPRecieved = 0;
    UDPLost = 0;
    totalSent = 0;
    totalRecieved = 0;
    totalLost = 0;
    lastPacSent = 0;
    lastPacRecv = 0;

    curr_pktid = -1;
    curr_delay = 0;
    start_time = 0;
    end_time = 0;
    total_delay = 0;
    total_packets = 0;
    avg_delay = 0.0;

    recv=0;
    gotime = 0.1;
    packet_size = 50;
    time_interval=0.1;
    total_throughput=0;
    avg_throughput=0.0

  
    seqno = -1;
    receivedPackets = 0;
    droppedPackets = 0;
}
{
    packetType = $7
    event = $1
    pktid = $6
    time = $2
    level = $4
    node_id = $3
    
    # Throughput
  
    if ( event == "r" )   
    {
        total_throughput += (packet_size * 8.0)/1000;
        recv++;
    }
    
    # Delivery delay timings
    if(!(pktid in packet_array))
    {
        time_array[pktid,"start"] = time;
        time_array[pktid,"end"] = time;
        packet_array[pktid];
        total_packets++;
    }
    else
    {
        time_array[pktid,"end"] = time;
    }
    # Packet delivery ratio
    if(packetType == "tcp")
    {
        if((event == "s" || event == "f")&& level == "AGT")
        {
            TCPSent++;
        }
        else if(event == "r" && level == "AGT")
        {
            TCPRecieved++;
        }
        else if(event == "D")
        {
            TCPLost++;
        }
    }
    else if(packetType == "cbr" )
    {
        if((event == "s" || event == "f")&& level == "AGT")
        {
            UDPSent++;
        }
        else if(event == "r" && level == "AGT")
        {
            UDPRecieved++;
        }
        else if(event == "D")
        {
            UDPLost++;
        }
    }
        
    print (time, total_throughput/time) > "./thoro.dat";
    print (time, UDPLost+TCPLost) > "./packetLost.dat";
    if(TCPSent!=0)
    print (time, (TCPRecieved/TCPSent)*100) > "./pdr.dat";
    if(UDPSent!=0)
    print (time,(UDPRecieved/UDPSent)*100) > "./pdr.dat";
    
    

}
END{
# Packet delivery ratio
    totalSent = TCPSent + UDPSent;
    totalLost = TCPLost + UDPLost;
    printf("\n");
    printf("TCP packets sent : %d\n",TCPSent);
    printf("TCP packets recieved : %d\n",TCPRecieved);
    printf("TCP packets dropped: %d\n", TCPLost);
if(TCPSent>0)
printf("PDR  %d\n", (TCPRecieved/TCPSent)*100);

    printf("UDP packets sent : %d\n",UDPSent);
    printf("UDP packets recieved : %d\n",UDPRecieved);
    printf("UDP packets dropped: %d\n", UDPLost);
if(UDPSent>0)
printf("PDR  %f\n", (UDPRecieved/UDPSent)*100);

    printf("Total Sent: %d\n",totalSent);
    printf("Total Dropped: %d\n\n",totalLost);


# Average delay
    for (i = 1;i <= total_packets;i++)
    {
     
        curr_delay = time_array[i,"end"] - time_array[i,"start"];
        total_delay += curr_delay;
        print (i, curr_delay) > "./delay.dat";
    }
    # printf("Total: %d\n\n",total_packets);
    # printf("Total Delay: %f\n\n",total_delay);
    avg_delay = total_delay / total_packets;
    printf("Average delay: %f\n\n",avg_delay);

# Average Throughput
    avg_throughput = total_throughput/8.5;
    printf("Average throughput: %f\n",avg_throughput);

}