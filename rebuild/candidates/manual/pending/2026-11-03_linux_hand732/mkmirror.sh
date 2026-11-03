#!/bin/bash
# mkmirror.sh: full header mirror = repo rebuild/include + chunk 728 round-2 overlay (pending) + this chunk's hdr/ on top
D=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/chunk732
P8=/home/user/FableDecomp/rebuild/candidates/manual/pending/2026-11-03_linux_hand728/round2/hdr
rm -rf $D/mirror; cp -r /home/user/FableDecomp/rebuild/include $D/mirror; cp -r $P8/. $D/mirror/; cp -r $D/hdr/. $D/mirror/
rm -rf $D/mirror/.git* 2>/dev/null; echo mirror ok
