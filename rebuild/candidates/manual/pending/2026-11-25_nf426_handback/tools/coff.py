import struct,sys
def sections(path):
    d=open(path,'rb').read()
    mach,ns,ts,symp,nsym,opt,ch=struct.unpack_from('<HHIIIHH',d,0)
    strt=symp+nsym*18
    def nm(raw):
        if raw[:4]==b'\0\0\0\0':
            o=struct.unpack_from('<I',raw,4)[0]; e=d.index(b'\0',strt+o); return d[strt+o:e].decode()
        return raw.rstrip(b'\0').decode()
    secs=[]
    for i in range(ns):
        o=20+opt+40*i
        n=d[o:o+8]; sz,ptr=struct.unpack_from('<II',d,o+16); nrel,=struct.unpack_from('<H',d,o+32); prel,=struct.unpack_from('<I',d,o+24); chs,=struct.unpack_from('<I',d,o+36)
        if n.startswith(b'/'): n=d[strt+int(n[1:].rstrip(b'\0')):].split(b'\0')[0]
        secs.append(dict(name=n.rstrip(b'\0').decode(),size=sz,raw=d[ptr:ptr+sz],sym=None,sel=None,ch=chs))
    i=0
    while i<nsym:
        o=symp+i*18; raw=d[o:o+8]; val,sec,typ,cls,naux=struct.unpack_from('<IhHBB',d,o+8)
        if cls==3 and val==0 and naux: # section symbol aux
            ao=o+18; sel=d[ao+14]
            if 0<sec<=ns: secs[sec-1]['sel']=sel
        elif cls in(2,3) and 0<sec<=ns and secs[sec-1]['sym'] is None and val==0 and not nm(raw).startswith('.') and not nm(raw).startswith('$'):
            secs[sec-1]['sym']=nm(raw)
        i+=1+naux
    return secs
if __name__=='__main__':
    for s in sections(sys.argv[1]):
        if s['name'].startswith('.text'): print(s['size'],s['sel'],s['sym'],s['raw'].hex() if len(sys.argv)>2 else '')
