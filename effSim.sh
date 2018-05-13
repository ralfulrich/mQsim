#!/bin/bash

MIN=0.001
MAX=0.01
NBINS=20
NSIM=1000

mkdir -p eff_data

for IBIN in `seq 0 ${NBINS}`; do

    QE=`awk "BEGIN {print ${MIN} + ${IBIN} * (${MAX}-${MIN})/(${NBINS}) };"`

    echo ${QE}
    
    ./musim mcp- ${NSIM}  ${QE}
    mv B4.root eff_data/MCP_${QE}.root 

done
