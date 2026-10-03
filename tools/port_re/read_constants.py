import struct,sys,re
d=open(__import__('os').environ['FABLE_EXE'],'rb').read()
o=struct.unpack_from('<I',d,0x3c)[0]; ns=struct.unpack_from('<H',d,o+6)[0]; osz=struct.unpack_from('<H',d,o+20)[0]
secs=[struct.unpack_from('<8sIIII',d,o+24+osz+40*i) for i in range(ns)]
def rd(va,n):
    rva=va-0x400000
    for nm,vs,a,rs,rp in secs:
        if a<=rva<a+max(vs,rs):
            off=rva-a
            return d[rp+off:rp+off+n] if off<rs else b'\0'*n
for a in sys.argv[1:]:
    v=int(a,16); b=rd(v,8)
    print(f"{v:08x}: f32={struct.unpack('<f',b[:4])[0]!r} u32={struct.unpack('<I',b[:4])[0]:#x} f64={struct.unpack('<d',b)[0]!r}")
