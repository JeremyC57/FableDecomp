#!/usr/bin/env python3
"""tu_flag_check.py <lo_hex> <hi_hex> <flag> [<flag>...]: re-grade every landed catalog row in [lo,hi) under BASE+flags
(read-only; sources from rebuild/src). Prints per row: base status vs flagged status. Shows whether a per-TU flag is
admissible (every landed row of the TU still matches)."""
import sys, os, re, subprocess
from pathlib import Path
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline")
import verify_and_land as V
S=Path(os.environ['S']); A=S/'w22'/'research861'
lo=int(sys.argv[1],16); hi=int(sys.argv[2],16); flags=sys.argv[3:]
t=open('/home/user/FableDecomp/rebuild/build_candidates.ps1',encoding='utf-8',errors='replace').read()
rows=[]
for b in re.split(r'\[pscustomobject\]@\{', t):
    a=re.search(r"Address\s*=\s*'([0-9A-Fa-f]+)'", b); src=re.search(r"Source\s*=\s*'([^']*)'", b); f=re.search(r"CompilerFlags\s*=\s*'([^']*)'", b)
    if a and src and lo<=int(a.group(1),16)<hi: rows.append((int(a.group(1),16), src.group(1), f.group(1) if f else ''))
rows.sort()
import csv, glob
oracle={}
for r in csv.DictReader(open('/home/user/FableDecomp/rebuild/oracles/auto-re-candidates.tsv',encoding='utf-8-sig'),delimiter='\t'):
    oracle[r['address'].lower()]=(r['name'], r['bytes'])
e=V.env()
for addr,src,fl in rows:
    p=Path('/home/user/FableDecomp/rebuild/src/compiled')/src
    if not p.exists(): print(hex(addr),'no source',src); continue
    a8=f'{addr:08x}'
    if a8 not in oracle: print(hex(addr),'no oracle'); continue
    name,hexb=oracle[a8]; retail=bytes.fromhex(hexb)
    while retail and retail[-1] in (0xCC,0x90): retail=retail[:-1]
    txt=p.read_text(encoding='utf-8',errors='replace')
    m=re.search(r'^// Leaf: (\S+)',txt,re.M); leaf=m.group(1) if m else name.rsplit('::',1)[-1]
    extra=[x for x in fl.replace('/O2 /Oy /W3','').split() if x and x!='none']
    work=S/'wine'/'q'/f'tufc_{a8}'; work.mkdir(parents=True,exist_ok=True)
    st0=V.parity_of(V.vc71(txt),a8,leaf,retail,work,e,extra=tuple(extra))[0]
    st1=V.parity_of(V.vc71(txt),a8,leaf,retail,work,e,extra=tuple(extra+flags))[0]
    print(f'{a8} {name[:50]:50s} [{" ".join(extra)}] base={st0:18s} +{" ".join(flags)}={st1}', flush=True)
