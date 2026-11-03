#!/bin/bash
# full header mirror: rebuild/include + hdr overlay on top
D=${S}/w22/chunk736
[ -d "$D" ] || exit 1
rm -rf $D/mirror; cp -r /home/user/FableDecomp/rebuild/include $D/mirror; cp -r $D/hdr/. $D/mirror/
