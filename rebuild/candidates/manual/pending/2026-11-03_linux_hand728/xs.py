# xs.py <disasm file>: annotate push/mov imm of string addresses with the C string there
import os,sys,struct,re
S=os.environ["S"]
exe=open(f"{S}/dl/game/Fable The Lost Chapters/Fable.exe","rb").read()
pe=struct.unpack_from("<I",exe,0x3c)[0]
nsec=struct.unpack_from("<H",exe,pe+6)[0]; opt=struct.unpack_from("<H",exe,pe+20)[0]
secs=[]
for i in range(nsec):
    o=pe+24+opt+40*i; vs,va,rs,ro=struct.unpack_from("<IIII",exe,o+8); secs.append((va,rs,ro))
def rd(va,n):
    for sva,rs,ro in secs:
        if sva<=va-0x400000<sva+rs: off=va-0x400000-sva+ro; return exe[off:off+n]
    return None
for l in open(sys.argv[1]):
    l=l.rstrip('\n'); ext=''
    for m in re.findall(r'0x(1[0-4][0-9a-f]{5})',l):
        b=rd(int(m,16),80)
        if b:
            s=b.split(b'\0')[0]
            if len(s)>=3 and all(32<=c<127 for c in s): ext+='  ; "%s"'%s.decode()
            else:
                if 'qword' in l: ext+='  ; %r'%struct.unpack('<d',b[:8])
                elif 'dword' in l or 'fld' in l or 'fmul' in l or 'fadd' in l or 'fcomp' in l: ext+='  ; %r'%struct.unpack('<f',b[:4])
    print(l+ext)
