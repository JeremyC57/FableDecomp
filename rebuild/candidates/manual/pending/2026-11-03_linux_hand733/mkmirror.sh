#!/bin/bash
# full header mirror: rebuild/include + hdr overlay on top
D=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/chunk733
rm -rf $D/mirror; cp -r /home/user/FableDecomp/rebuild/include $D/mirror; cp -r $D/hdr/. $D/mirror/
