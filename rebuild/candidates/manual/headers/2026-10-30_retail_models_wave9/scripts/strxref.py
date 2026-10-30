#!/usr/bin/env python3
"""strxref.py <string>...: retail VA of each NUL-terminated string and the code sites that push/mov its address (imm32),
with a capstone window around each site."""
import sys, os, struct
sys.argv, a = [sys.argv[0]], sys.argv[1:]
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import vt
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
md = Cs(CS_ARCH_X86, CS_MODE_32)
text = [s for s in vt.secs if s[0] == ".text"][0]
tb = vt.exe[text[3]:text[3] + text[4]]
win = int(os.environ.get("WIN", "48"), 0)
for s in a:
    for name, sva, sz, ra, rs in vt.secs:
        blob = vt.exe[ra:ra + rs]; i = blob.find(s.encode() + b"\0")
        while i >= 0:
            if i == 0 or blob[i-1] == 0:
                va = sva + i; print("==", s, hex(va), name)
                pat = struct.pack("<I", va); j = tb.find(pat)
                while j >= 0:
                    site = text[1] + j
                    print("  site", hex(site))
                    for ins in md.disasm(tb[j - win: j + win], site - win):
                        if ins.address >= site - win + 8: print("   %08x %s %s" % (ins.address, ins.mnemonic, ins.op_str))
                    j = tb.find(pat, j + 1)
            i = blob.find(s.encode() + b"\0", i + 1)
