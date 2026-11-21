#!/usr/bin/env python3
"""symflags.py <wr.json> [name-substr] : c1's symbol-file records (the fwrite holding decorated names): name -> flag word
(the 2-byte value c2 reads into sym+0x69, located as the word before the 0x82/0x86 record tag pattern is not used: we print
the 12 bytes after each name and the flag word found by the c8/68-style pattern = bytes [10:12] for function records)."""
import json,sys,re
t=json.load(open(sys.argv[1])); pat=sys.argv[2] if len(sys.argv)>2 else ''
for e in t['events']:
    if e[0]!='f': continue
    d=bytes.fromhex(e[4])
    for m in re.finditer(rb'(\?[\x21-\x7e]+?)\x00', d):
        nm=m.group(1).decode('latin1')
        if pat not in nm: continue
        tail=d[m.end():m.end()+16]
        print('%-50s %s'%(nm[:50], tail.hex(' ')))
