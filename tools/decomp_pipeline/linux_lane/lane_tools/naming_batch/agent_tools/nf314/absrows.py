# absrows.py <addr>...: every 4-byte absolute reference to addr in Fable.exe, with the catalog row whose extent holds the site
# (L = landed, c = catalog only, '-' = none: data such as a vtable). For delete checks of deleter / vtable-slot targets. NF314.
import sys,struct,re,bisect,os
EXE=open(os.environ['FABLE_EXE'],'rb').read()
pe=struct.unpack_from('<I',EXE,0x3C)[0]; ns=struct.unpack_from('<H',EXE,pe+6)[0]; osz=struct.unpack_from('<H',EXE,pe+20)[0]
base=struct.unpack_from('<I',EXE,pe+24+28)[0]; sh=pe+24+osz
ps=open('/home/user/FableDecomp/rebuild/build_candidates.ps1',encoding='utf-8').read()
landed=set(m.group(1).lower() for m in re.finditer(r"Address = '([0-9a-fA-F]{8})'",ps))
ln={}
for l in open('/home/user/FableDecomp/rebuild/oracles/auto-re-candidates.tsv',encoding='utf-8'):
    r=l.split('\t')
    if len(r)>2 and r[2].isdigit(): ln[int(r[0],16)]=(int(r[2]),r[1])
st=sorted(ln)
for a in sys.argv[1:]:
    pat=struct.pack('<I',int(a,16))
    for i in range(ns):
        o=sh+i*40; vs,va,rs,rp=struct.unpack_from('<IIII',EXE,o+8); d=EXE[rp:rp+rs]; j=d.find(pat)
        while j>=0:
            site=base+va+j; k=bisect.bisect_right(st,site)-1; row='-'
            if k>=0 and st[k]<=site<st[k]+ln[st[k]][0]: row='%08x%s %s'%(st[k],'L' if '%08x'%st[k] in landed else 'c',ln[st[k]][1][:60])
            print('%s\t%08x\t%s'%(a,site,row)); j=d.find(pat,j+1)
