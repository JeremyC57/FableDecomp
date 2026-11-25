#!/usr/bin/env python3
"""forcelink.py (NF445 check 3): link one object per translation unit, HEAD v new, with link /DLL /NOENTRY /FORCE (VC7.1 linker from
verify_and_land) and report unresolved externals (LNK2001 / LNK2019) and duplicate definitions (LNK2005 / LNK4006), plus the delta."""
import os,re,sys
sys.path.insert(0,'tools/decomp_pipeline'); import verify_and_land as V
from pathlib import Path
B=Path(sys.argv[1]); pk=B/'evidence'/'pk'; out=B/'evidence'
H=['00c6ca20','00c6cac0','00c6d9d0','00c6da30','00c6e630','00c6e690','00c6e6a0','00c6e6d0','00c6e760','00c6e780','00c70120','00c701d0','00c70a30','00c70b00','00c70bb0','00c71400','00c556e0','00c55680','00c55270','00c6e660']
N=['00c6ca20','00c6cac0','00c6d9d0','00c6da30','00c6e630','00c6e760','00c6e780','00c70120','00c701d0','00c70a30','00c70b00','00c70bb0','00c71400','00c556e0','00c55680','00c55270']
e=V.env(); res={}
for tag,rows in (('H',H),('N',N)):
    wd=Path(os.environ['S'])/'wine'/'forcelink'/tag; wd.mkdir(parents=True,exist_ok=True)
    lk=V.run_tool(V.VC/'bin'/'link.exe',['-nologo','-dll','-noentry','-force','-machine:X86',f'-out:{wd/"x.dll"}']+[str(pk/f'{tag}_{a}.obj') for a in rows],e)
    txt=lk.stdout or ''; (out/f'forcelink_{tag}.log').write_text(txt)
    def sym(l):
        m=re.search(r'symbol "[^"]*" \((\S+)\)',l) or re.search(r'symbol (\S+)',l) or re.search(r'" \((\S+)\) already defined',l) or re.search(r': (\S+) already defined',l)
        return m.group(1) if m else l
    und={sym(l) for l in txt.splitlines() if 'LNK2001' in l or 'LNK2019' in l}
    dup={sym(l) for l in txt.splitlines() if 'LNK4006' in l or 'LNK2005' in l}
    res[tag]=(und,dup); print(tag,'objects',len(rows),'unresolved',len(und),'duplicates',len(dup))
(hu,hd),(nu,nd)=res['H'],res['N']
with open(out/'forcelink_delta.txt','w') as f:
    for s in sorted(hu-nu): f.write(f'UND-\t{s}\n')
    for s in sorted(nu-hu): f.write(f'UND+\t{s}\n')
    for s in sorted(hd-nd): f.write(f'DUP-\t{s}\n')
    for s in sorted(nd-hd): f.write(f'DUP+\t{s}\n')
print(open(out/'forcelink_delta.txt').read())
