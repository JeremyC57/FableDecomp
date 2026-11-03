#!/bin/bash
# rebuild the full header mirror: repo include + chunk734 overlay
S=${S}
W=$S/w22/chunk734
rm -rf $W/mirror; cp -r /home/user/FableDecomp/rebuild/include $W/mirror; cp -r $W/hdr/. $W/mirror/
