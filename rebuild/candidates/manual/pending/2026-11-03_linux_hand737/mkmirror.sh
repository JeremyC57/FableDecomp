#!/bin/bash
# full header mirror: frozen repo rebuild/include (HEAD 4ff4f9bd, MIRROR_HEAD.txt) + hdr overlay on top
D=${S}/w22/chunk737
[ -d "$D" ] || exit 1
rm -rf $D/mirror; cp -r $D/base_include $D/mirror; cp -r $D/hdr/. $D/mirror/
