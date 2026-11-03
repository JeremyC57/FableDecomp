#!/usr/bin/env python3
# xform.py tree.cpp out.cpp mode   -- tidy version of the research transform.py (no whitespace residue; out-of-class dtor -> inline)
import sys,re
src=open(sys.argv[1]).read(); out=sys.argv[2]; mode=sys.argv[3]
lines=src.split('\n'); res=[]; depth0=False; region=[]
for ln in lines:
    st=ln.strip()
    if st.startswith('#pragma inline_depth'):
        depth0=('(0)' in st); continue
    if st.startswith('// Revisit: inline-depth'): continue
    if not st.startswith('//'):
        # out-of-class definition at column 0
        ln=re.sub(r'^__forceinline\s+([A-Za-z_][A-Za-z_0-9:<>]*::~)',r'inline \1',ln)
        ln=re.sub(r'__forceinline\s+((?:[A-Za-z_:<>]+::)?~)',r'\1',ln)
    res.append(ln); region.append(depth0)
text='\n'.join(res)
if mode in ('dw','dwall'):
    offs=[]; o=0
    for ln,r in zip(res,region): offs.append((o,o+len(ln),r)); o+=len(ln)+1
    def in_region(pos):
        for a,b,r in offs:
            if a<=pos<=b: return r
        return False
    pat=re.compile(r'if \(((?:[^()]|\([^()]*\))*)\)\s*\n?\s*return;')
    pieces=[]; last=0
    m=list(re.finditer(r'^[A-Za-z_][^\n;]*::[^\n;]*\)\s*(?:const\s*)?\n\{',text,re.M))
    start=m[-1].start() if m else 0
    for mm in pat.finditer(text):
        if mm.start()<start or (mode=='dw' and in_region(mm.start())): continue
        pieces.append(text[last:mm.start()]); pieces.append('do { if ('+mm.group(1)+') return; } while (0);'); last=mm.end()
    pieces.append(text[last:]); text=''.join(pieces)
open(out,'w').write(text)
