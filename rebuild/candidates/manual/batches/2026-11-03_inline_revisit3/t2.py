#!/usr/bin/env python3
"""t2.py landed.cpp out.cpp mode [mask]
modes: plain | dw (wrap returns outside depth-0) | dwall (wrap all returns in the row function) | mask (bit string per site, 1=wrap)
Return sites: `if (COND) return;`, `if (COND) {return;}` (pragma lines inside dropped). Also prints the site table to stderr:
 idx  in_depth0  cond"""
import sys,re
src=open(sys.argv[1]).read(); out=sys.argv[2]; mode=sys.argv[3]; mask=sys.argv[4] if len(sys.argv)>4 else ''
lines=src.split('\n'); res=[]; region=[]; depth0=False
for ln in lines:
    st=ln.strip()
    if st.startswith('#pragma inline_depth'):
        m=re.search(r'inline_depth\(\s*(\d*)\s*\)',st); depth0=(m.group(1)=='0'); continue
    ln=re.sub(r'__forceinline(\s+(?:[A-Za-z_:<>]+::)?~)',r'\1',ln)
    res.append(ln); region.append(depth0)
text='\n'.join(res)
offs=[]; o=0
for ln,r in zip(res,region): offs.append((o,o+len(ln),r)); o+=len(ln)+1
def in_region(a,b):
    return any(r for x,y,r in offs if not (y<a or x>b))
m=list(re.finditer(r'^[A-Za-z_][^\n;]*::[^\n;]*\)\s*(?:const\s*)?\n\{',text,re.M))
start=m[-1].start() if m else 0
C=r'((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)'
pat=re.compile(r'\bif \('+C+r'\)\s*(?:\{\s*return(\s+[^;{}]+)?;\s*\}|return(\s+[^;{}]+)?;)')
sites=[mm for mm in pat.finditer(text) if mm.start()>=start]
for k,mm in enumerate(sites):
    sys.stderr.write(f"{k}\t{int(in_region(mm.start(),mm.end()))}\t{mm.group(1)[:60]}\n")
pieces=[]; last=0
for k,mm in enumerate(sites):
    if mode=='plain': w=False
    elif mode=='dwall': w=True
    elif mode=='dw': w=not in_region(mm.start(),mm.end())
    elif mode=='mask': w=(k<len(mask) and mask[k]=='1')
    if not w: continue
    pieces.append(text[last:mm.start()]); rv=(mm.group(2) or mm.group(3) or ''); pieces.append('do { if ('+mm.group(1)+') return'+rv+'; } while (0);'); last=mm.end()
pieces.append(text[last:]); text=''.join(pieces)
open(out,'w').write(text)
