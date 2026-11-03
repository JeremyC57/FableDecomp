#!/bin/bash
# mkmirror.sh: full header mirror = repo rebuild/include + this chunk's hdr/ overlay on top
D=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/chunk728b
rm -rf $D/mirror; cp -r /home/user/FableDecomp/rebuild/include $D/mirror; cp -r $D/hdr/. $D/mirror/
rm -rf $D/mirror/.git* 2>/dev/null; echo mirror ok
