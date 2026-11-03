#!/usr/bin/env python3
"""transform.py landed.cpp out.cpp [mode]
Pragma-free rewrite of a `// Revisit: inline-depth` row:
 - drop every `#pragma inline_depth(...)` line (remember which lines sat in a depth-0 region);
 - `__forceinline` on destructors -> plain inline (in-class `inline` semantics);
 - mode 'dw' (default): every `if (COND) return;` OUTSIDE a former depth-0 region, inside function bodies, becomes
   `do { if (COND) return; } while (0);` (raises that exit's static frequency: an inner-loop exit);
 - mode 'plain': only the first two steps.
 - mode 'dwall': like 'dw' but wraps every `if (COND) return;` of the row function (one macro everywhere)."""
import sys,re
src=open(sys.argv[1]).read(); out=sys.argv[2]; mode=sys.argv[3] if len(sys.argv)>3 else 'dw'
lines=src.split('\n'); res=[]; depth0=False; region=[]
for ln in lines:
    st=ln.strip()
    if st.startswith('#pragma inline_depth'):
        depth0 = ('(0)' in st); continue
    ln=re.sub(r'__forceinline(\s+(?:[A-Za-z_:<>]+::)?~)',r'\1',ln)
    res.append(ln); region.append(depth0)
text='\n'.join(res)
if mode in ('dw','dwall'):
    # mark region by character offsets
    offs=[]; o=0
    for ln,r in zip(res,region): offs.append((o,o+len(ln),r)); o+=len(ln)+1
    def in_region(pos):
        for a,b,r in offs:
            if a<=pos<=b: return r
        return False
    # only after the first line that starts a function body definition with '::' and '(' at column 0 (the row function)
    pat=re.compile(r'if \(((?:[^()]|\([^()]*\))*)\)\s*\n?\s*return;')
    pieces=[]; last=0
    # find row function start: last top-level definition line containing '::' and ending with ')' followed by '{'
    m=list(re.finditer(r'^[A-Za-z_][^\n;]*::[^\n;]*\)\s*(?:const\s*)?\n\{',text,re.M))
    start=m[-1].start() if m else 0
    for mm in pat.finditer(text):
        if mm.start()<start or (mode=='dw' and in_region(mm.start())): continue
        pieces.append(text[last:mm.start()]); pieces.append('do { if ('+mm.group(1)+') return; } while (0);'); last=mm.end()
    pieces.append(text[last:]); text=''.join(pieces)
open(out,'w').write(text)
