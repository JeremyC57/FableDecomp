#!/usr/bin/env python3
"""gate428.py: count gate per retail TU. D copies (the 53 / 44 byte ~CCountedPointer / ReleaseData shapes) v the FableWin module's
??1 + ?ReleaseData CCountedPointer names; SH copies v its ?ShareData names -> evidence/gate.json"""
import json,collections,re,os
R='/home/user/FableDecomp'; A=os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
t=json.load(open(A+'/evidence/occ_tu.json'))
cand=collections.defaultdict(lambda: {'D':[], 'SH':[]})
for l in open(R+'/ghidra_out/fablewin_obj_functions.tsv',errors='replace'):
    f=l.rstrip('\n').split('\t')
    if len(f)<7: continue
    n=f[6]
    if re.match(r'^(\?\?1|\?ReleaseData@)\?\$CCountedPointer@',n): cand[f[0]]['D'].append((int(f[1]),n))
    elif n.startswith('?ShareData@?$CCountedPointer@'): cand[f[0]]['SH'].append((int(f[1]),n))
ret=collections.defaultdict(lambda: {'D':[], 'SH':[]})
for k,v in t.items():
    if v[7]=='same': ret[v[4]]['D' if v[0].startswith('D') else 'SH'].append(k)
out={}; C=collections.Counter()
for o in set(ret)|set(cand):
    for kind in ('D','SH'):
        a=sorted(ret[o][kind]); b=cand[o][kind] if o in cand else []
        if not a: continue
        Ts=sorted({re.match(r'^.*?\?\$CCountedPointer@(.*?)@@@@',n).group(1) if re.match(r'^.*?\?\$CCountedPointer@(.*?)@@@@',n) else n for i,n in b})
        st='EQ' if len(a)==len(b) else ('LESS' if len(a)<len(b) else 'MORE')
        C[(kind,st,min(len(Ts),3))]+=len(a)
        out[o+'|'+kind]=dict(obj=o,kind=kind,addrs=a,names=sorted(n for i,n in b),Ts=Ts,st=st)
json.dump(out,open(A+'/evidence/gate.json','w'),indent=1)
for k,v in sorted(C.items()): print(k,v)
