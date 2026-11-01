# minimal COFF reader: external symbols of an .obj -> (defined set, undefined set)
import struct
def syms(path):
    b=open(path,'rb').read()
    if len(b)<20: return set(),set()
    mach,nsec,_,sp,ns,opt,_=struct.unpack_from('<HHIIIHH',b,0)
    if mach==0 and nsec==0xFFFF:   # bigobj
        raise ValueError('bigobj')
    st=sp+18*ns
    d=set();u=set();i=0
    while i<ns:
        o=sp+18*i
        nm=b[o:o+8]; val,sec,typ,cls,na=struct.unpack_from('<IhHBB',b,o+8)
        if cls==2:
            if nm[:4]==b'\0\0\0\0':
                off=struct.unpack_from('<I',nm,4)[0]; e=b.index(b'\0',st+off); n=b[st+off:e]
            else: n=nm.rstrip(b'\0')
            n=n.decode('latin-1')
            (d if sec>0 else u).add(n) if not (sec==0 and val>0) else u.add(n)
        i+=1+na
    return d,u
