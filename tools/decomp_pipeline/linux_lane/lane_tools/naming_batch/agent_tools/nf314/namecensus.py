#!/usr/bin/env python3
# namecensus.py (v2): every new symbol of this hand-back (TABLE_CHANGES add / replace targets, RENAMES section 1 own Leafs (col 4,
# address col 1) and RENAMES section 2 new_symbol (col 3, callee col 4)). For each, list landed row sources (rebuild/src/compiled) that
# DEFINE Class::Member (a body; initialiser lists and const allowed) with the defining line, and whether the file is the target's own row.
import re, os
R='/home/user/FableDecomp/rebuild/src/compiled'
new={}
rows=[l.rstrip('\n').split('\t') for l in open('TABLE_CHANGES.tsv')][1:]
for r in rows:
    act,sym,tgt=r[0],r[1],r[2]
    if act=='replace': new.setdefault(sym.split(' -> ')[1],tgt)
    elif act=='add': new.setdefault(sym,tgt)
sec=0
for l in open('RENAMES.tsv'):
    if l.startswith('# Section 1'): sec=1; continue
    if l.startswith('# Section 2'): sec=2; continue
    f=l.rstrip('\n').split('\t')
    if sec==1 and len(f)>=4 and f[3].startswith('?'): new.setdefault(f[3],f[0].replace('0x','').lower())
    if sec==2 and len(f)>=4 and f[2].startswith('?'): new.setdefault(f[2],f[3].replace('0x','').lower())
def parts(m):
    if m.startswith('??0'):
        cls=m[3:].split('@')[0]; return cls,cls
    m=m.lstrip('?')
    seg=m.split('@'); name=seg[0]; cls=seg[1]
    if cls.startswith('?$'): cls=cls[2:]
    return cls,name
files=[]
for d,_,fs in os.walk(R):
    for f in fs:
        if f.endswith('.cpp'): files.append(os.path.join(d,f))
src={f:open(f,errors='replace').read() for f in files}
for sym,tgt in sorted(new.items()):
    cls,name=parts(sym)
    pat=re.compile(r'\b'+re.escape(cls)+r'(?:<[^;{()]*>)?::'+re.escape(name)+r'\s*\([^;{]*\)[^;{]*\{')
    own=[];other=[]
    for f,t in src.items():
        for m in pat.finditer(t):
            line=t[:m.start()].count('\n')+1
            sig=t[m.start():m.end()].replace('\n',' ')[:110]
            (own if f.endswith('_'+tgt+'.cpp') else other).append('%s:%d %s'%(os.path.relpath(f,R),line,sig))
    print('%s\ttarget=%s\town_def=%d\tother_defs=%d'%(sym,tgt,len(own),len(other)))
    for o in other: print('    OTHER '+o)
