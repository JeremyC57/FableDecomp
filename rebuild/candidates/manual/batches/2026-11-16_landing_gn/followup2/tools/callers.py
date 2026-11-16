# callers.py <target>...: every retail E8/E9 rel32 into a target, mapped to the landed row containing the call site.
import sys,struct,re,bisect
EXE=open('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/dl/game/Fable The Lost Chapters/Fable.exe','rb').read()
pe=struct.unpack_from('<I',EXE,0x3C)[0]; ns=struct.unpack_from('<H',EXE,pe+6)[0]; osz=struct.unpack_from('<H',EXE,pe+20)[0]
base=struct.unpack_from('<I',EXE,pe+24+28)[0]; sh=pe+24+osz
targets={int(t,16) for t in sys.argv[1:]}
ps=open('rebuild/build_candidates.ps1',encoding='utf-8').read()
landed=set(a.lower() for a in re.findall(r"Address = '([0-9a-fA-F]{8})'",ps))
ln={}
for l in open('rebuild/oracles/auto-re-candidates.tsv',encoding='utf-8'):
    r=l.split('\t')
    if len(r)>2 and r[0] in landed and r[2].isdigit(): ln[int(r[0],16)]=(int(r[2]),r[1])
starts=sorted(ln)
for i in range(ns):
    o=sh+i*40; name=EXE[o:o+8].rstrip(b'\0'); vs,va,rs,rp=struct.unpack_from('<IIII',EXE,o+8); ch=struct.unpack_from('<I',EXE,o+36)[0]
    if not ch&0x20: continue
    d=EXE[rp:rp+rs]
    for m in re.finditer(rb'[\xe8\xe9]',d):
        p=m.start()
        if p+5>len(d): break
        site=base+va+p; tgt=(site+5+struct.unpack_from('<i',d,p+1)[0])&0xffffffff
        if tgt in targets:
            k=bisect.bisect_right(starts,site)-1
            row=None
            if k>=0 and starts[k]<=site<starts[k]+ln[starts[k]][0]: row=starts[k]
            print('%08x\t%08x\t%s\t%s'%(tgt,site,'%08x'%row if row else '-',ln[row][1] if row else ''))
