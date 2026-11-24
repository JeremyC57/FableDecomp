# absrefs.py <addr>...: every 4-byte absolute little-endian reference to addr anywhere in Fable.exe sections (file offset -> VA)
import sys,struct,os
EXE=open(os.environ['FABLE_EXE'],'rb').read()
pe=struct.unpack_from('<I',EXE,0x3C)[0]; ns=struct.unpack_from('<H',EXE,pe+6)[0]; osz=struct.unpack_from('<H',EXE,pe+20)[0]
base=struct.unpack_from('<I',EXE,pe+24+28)[0]; sh=pe+24+osz
for a in sys.argv[1:]:
    pat=struct.pack('<I',int(a,16)); n=0
    for i in range(ns):
        o=sh+i*40; vs,va,rs,rp=struct.unpack_from('<IIII',EXE,o+8); d=EXE[rp:rp+rs]; j=d.find(pat)
        while j>=0: print('%s\tabsref\t%08x'%(a,base+va+j)); n+=1; j=d.find(pat,j+1)
    print('%s\ttotal\t%d'%(a,n))
