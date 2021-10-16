# Distance vector routing protocol

set ns [new Simulator]

# File openings
set traceFile [open trace_dvr.tr w]
$ns trace-all $traceFile

# NAM trace file
set namFile [open out_dvr.nam w]
$ns namtrace-all $namFile

$ns rtproto DV

#Create Nodes
set node1 [$ns node]
set node2 [$ns node]
set node3 [$ns node]
set node4 [$ns node]
set node5 [$ns node]
set node6 [$ns node]
set node7 [$ns node]
set node8 [$ns node]
set node9 [$ns node]
set node10 [$ns node]

$node1 color red
$node2 color blue
$node3 color orange
$node4 color yellow
$node5 color green
$node6 color black
$node7 color pink
$node8 color brown
$node9 color magenta
$node10 color violet

# Create node links
$ns duplex-link $node1 $node2 10Mb 8ms DropTail
$ns duplex-link $node2 $node3 10Mb 8ms DropTail
$ns duplex-link $node3 $node4 10Mb 8ms DropTail
$ns duplex-link $node4 $node5 10Mb 8ms DropTail
$ns duplex-link $node5 $node6 10Mb 8ms DropTail
$ns duplex-link $node6 $node7 10Mb 8ms DropTail
$ns duplex-link $node7 $node8 10Mb 8ms DropTail
$ns duplex-link $node8 $node9 10Mb 8ms DropTail
$ns duplex-link $node9 $node10 10Mb 8ms DropTail
$ns duplex-link $node10 $node1 10Mb 8ms DropTail

#UDP Connection Model to node/ [Agent+NULL]
set udp1 [new Agent/UDP]
$ns attach-agent $node1 $udp1
set null1 [new Agent/Null]
$ns attach-agent $node7 $null1
$ns connect $udp1 $null1
$udp1 set fid_ 1

#CBR over UDP node1 to node7
set cbr1 [new Application/Traffic/CBR]
$cbr1 attach-agent $udp1
$cbr1 set packetSize_ 500
$cbr1 set rate_ 1.5Mb


proc finish {} {
    global ns traceFile namFile
    $ns flush-trace
    close $traceFile
    close $namFile
    exec nam out_dvr.nam &
    exit 0
}

# Time slots + Launch

$ns rtmodel-at 2.0 down $node1 $node10
$ns rtmodel-at 5.0 up $node1 $node10
$ns rtmodel-at 7.0 down $node1 $node2
$ns rtmodel-at 7.0 up $node1 $node10

$ns at 0.5 "$cbr1 start"
$ns at 9.0 "finish"

$ns run

