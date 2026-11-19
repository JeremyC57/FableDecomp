#!/usr/bin/env python3
"""codaobj.py <lib> <symbol> : locate a COMDAT function in a COFF .lib, print member, size, relocations (offset -> symbol)"""
import struct, sys
lib, want = sys.argv[1], sys.argv[2]
b = open(lib, 'rb').read()
longnames = None; off = 8
while off < len(b):
    hdr = b[off:off+60]; name = hdr[:16].decode('latin1').strip(); size = int(hdr[48:58]); d = off+60
    if name == '//': longnames = b[d:d+size]
    obj = b[d:d+size]
    if name not in ('/', '//') and len(obj) > 20 and struct.unpack('<H', obj[:2])[0] == 0x14c:
        mach, nsec, ts, symptr, nsym, opt, ch = struct.unpack('<HHIIIHH', obj[:20])
        strtab = symptr+18*nsym
        def sname(e):
            if e[:4] == b'\0\0\0\0':
                o = struct.unpack('<I', e[4:8])[0]; return obj[strtab+o:obj.index(b'\0', strtab+o)].decode('latin1')
            return e[:8].rstrip(b'\0').decode('latin1')
        syms = {}; i = 0
        while i < nsym:
            e = obj[symptr+18*i:symptr+18*i+18]; syms[i] = (sname(e),) + struct.unpack('<Ih', e[8:14]); i += 1+e[17]
        for idx, (n, val, sec) in syms.items():
            if n == want and sec > 0:
                s = obj[20+opt+40*(sec-1):20+opt+40*sec]
                sz, ptr, rptr = struct.unpack('<III', s[16:28]); nrel = struct.unpack('<H', s[32:34])[0]
                mem = name
                if name.startswith('/') and longnames: o = int(name[1:]); mem = longnames[o:longnames.index(b'\0', o)].decode('latin1')
                print('member', mem, 'section', sec, 'size', sz)
                for r in range(nrel):
                    va, si, ty = struct.unpack('<IIH', obj[rptr+10*r:rptr+10*r+10]); print('  reloc +0x%x type %d -> %s' % (va, ty, syms.get(si, ('?',))[0]))
                sys.exit()
    off = d+size+(size & 1)
print('not found')
