import struct,os,sys
p=os.environ['S']+'/dl/game/Fable The Lost Chapters/Fable.exe'
b=open(p,'rb').read()
pe=struct.unpack_from('<I',b,0x3c)[0]; n=struct.unpack_from('<H',b,pe+6)[0]; opt=struct.unpack_from('<H',b,pe+20)[0]
secs=[]
for i in range(n):
    o=pe+24+opt+i*40; secs.append((struct.unpack_from('<I',b,o+12)[0],struct.unpack_from('<I',b,o+8)[0],struct.unpack_from('<I',b,o+20)[0],struct.unpack_from('<I',b,o+16)[0]))
def va(off):
    for s,sz,raw,rsz in secs:
        if raw<=off<raw+rsz: return 0x400000+s+off-raw
for a in sys.argv[1:]:
    pat=struct.pack('<I',int(a,16)); i=0; out=[]
    while True:
        i=b.find(pat,i)
        if i<0: break
        out.append(hex(va(i))); i+=1
    print(a,len(out),' '.join(out[:30]))
