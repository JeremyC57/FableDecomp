#!/bin/bash
# full header mirror: frozen repo rebuild/include (HEAD 40c4d6d6, MIRROR_HEAD.txt) + hdr overlay on top
D=${S}/w22/chunk738
[ -d "$D" ] || exit 1
rm -rf $D/mirror; cp -r $D/base_include $D/mirror; cp -r $D/hdr/. $D/mirror/
