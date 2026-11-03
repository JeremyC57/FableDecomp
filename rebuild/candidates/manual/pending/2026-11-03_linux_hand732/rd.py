import struct,os,sys
p=os.environ['S']+'/dl/game/Fable The Lost Chapters/Fable.exe'
b=open(p,'rb').read()
pe=struct.unpack_from('<I',b,0x3c)[0]; n=struct.unpack_from('<H',b,pe+6)[0]; opt=struct.unpack_from('<H',b,pe+20)[0]
secs=[]
for i in range(n):
    o=pe+24+opt+i*40; secs.append((struct.unpack_from('<I',b,o+12)[0],struct.unpack_from('<I',b,o+8)[0],struct.unpack_from('<I',b,o+20)[0],struct.unpack_from('<I',b,o+16)[0]))
def off(va):
    va-=0x400000
    for s,sz,raw,rsz in secs:
        if s<=va<s+rsz: return raw+va-s
for a in sys.argv[1:]:
    o=off(int(a,16))
    if o is None: print(a,'bss'); continue
    d=b[o:o+8]
    print(a, d.hex(), 'f=%r'%struct.unpack('<f',d[:4])[0], 'd=%r'%struct.unpack('<d',d)[0], 'str=%r'%b[o:o+40].split(b'\0')[0])
