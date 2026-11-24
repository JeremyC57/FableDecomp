# xsites.py <addr>...: every E8/E9 rel32 site in Fable.exe code targeting addr, with the containing catalog row (L = landed) or '-'
import sys,struct,re,bisect,os
EXE=open(os.environ.get('FABLE_EXE','/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/dl/game/Fable The Lost Chapters/Fable.exe'),'rb').read()
pe=struct.unpack_from('<I',EXE,0x3C)[0]; ns=struct.unpack_from('<H',EXE,pe+6)[0]; osz=struct.unpack_from('<H',EXE,pe+20)[0]
base=struct.unpack_from('<I',EXE,pe+24+28)[0]; sh=pe+24+osz
ps=open('/home/user/FableDecomp/rebuild/build_candidates.ps1',encoding='utf-8').read()
landed=set(m.group(1).lower() for m in re.finditer(r"Address = '([0-9a-fA-F]{8})'",ps))
ln={}
for l in open('/home/user/FableDecomp/rebuild/oracles/auto-re-candidates.tsv',encoding='utf-8'):
    r=l.split('\t')
    if len(r)>2 and r[2].isdigit(): ln[int(r[0],16)]=(int(r[2]),r[1])
st=sorted(ln); T={int(a,16) for a in sys.argv[1:]}
for i in range(ns):
    o=sh+i*40; vs,va,rs,rp=struct.unpack_from('<IIII',EXE,o+8); ch=struct.unpack_from('<I',EXE,o+36)[0]
    if not ch&0x20: continue
    d=EXE[rp:rp+rs]
    for m in re.finditer(rb'[\xe8\xe9]',d):
        p=m.start()
        if p+5>len(d): break
        site=base+va+p; tgt=(site+5+struct.unpack_from('<i',d,p+1)[0])&0xffffffff
        if tgt in T:
            k=bisect.bisect_right(st,site)-1; row='-'
            if k>=0 and st[k]<=site<st[k]+ln[st[k]][0]: row='%08x%s %s'%(st[k],'L' if '%08x'%st[k] in landed else 'c',ln[st[k]][1][:60])
            print('%08x\t%s\t%08x\t%s'%(tgt,'call' if d[p]==0xe8 else 'jmp',site,row))
