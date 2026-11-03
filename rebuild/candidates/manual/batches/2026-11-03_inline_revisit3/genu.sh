#!/bin/bash
# genu.sh : write the uniform per-class variants u_<spec>.cpp for every row
I=${S}/pragma3
for d in $I/rows/*/; do
  for sp in H W S HW HS WS HWS c cH cW cS cHW cHS cWS cHWS S2 W2 HWS2 HW2S2 H2W2S2 W2S HS2 WS2; do
    python3 $I/t4.py $d/landed.cpp $d/u_$sp.cpp "$sp" 2>/dev/null
  done
done
