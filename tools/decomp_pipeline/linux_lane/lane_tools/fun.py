import struct,subprocess,os,sys
S=os.environ['S']
exe=open(f"{S}/dl/game/Fable The Lost Chapters/Fable.exe",'rb').read()
pe=struct.unpack_from('<I',exe,0x3c)[0]; nsec=struct.unpack_from('<H',exe,pe+6)[0]; opt=struct.unpack_from('<H',exe,pe+20)[0]
secs=[]
for i in range(nsec):
    o=pe+24+opt+40*i; vs,va,rs,ro=struct.unpack_from('<IIII',exe,o+8); secs.append((va,vs,rs,ro))
def rd(va,n):
    for sva,vs,rs,ro in secs:
        if sva<=va-0x400000<sva+max(vs,rs): off=va-0x400000-sva+ro; return exe[off:off+n]
    return b'\0'*n
for fa in sys.argv[1:]:
    fa=int(fa,16); body=rd(fa,8)
    assert body[0]==0x6a and body[2]==0x68
    h=struct.unpack_from('<I',body,3)[0]
    th=rd(h,10); fi=struct.unpack_from('<I',th,1)[0]
    magic,maxst,pum=struct.unpack('<III',rd(fi,12))
    ents=[struct.unpack('<iI',rd(pum+8*i,8)) for i in range(maxst)]
    print(hex(fa),'handler',hex(h),'states',maxst,[(t,hex(a)) for t,a in ents])
    for t,a in ents:
        if a: print('   ',hex(a),'|'.join(l.split(None,2)[2] for l in subprocess.run(['python3',f'{S}/w17/xdis.py',hex(a),'0x24'],capture_output=True,text=True).stdout.splitlines()[:6]))
