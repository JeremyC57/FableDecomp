#!/usr/bin/env python3
"""t4.py in.cpp out.cpp spec
Pragma-free rewrite with per-CLASS wrapping of `if (COND) return;` sites of the row function (the last X::Y(...) definition).
Site classes: H = first statement of a loop body (prev token `{` of for/while/do), W = right after a NewScriptFrame() call,
S = any other. spec: letters to wrap, e.g. 'HWS', 'S', '' ; optional ':2' suffix per letter for double wrap (e.g. 'S2W').
Extra flags in spec: 'm<bits>' explicit per-site mask (overrides classes), 'c' convert `for (;;) {` + head check to while(!term).
Prints the site table (idx class cond) to stderr."""
import sys,re
src=open(sys.argv[1]).read(); out=sys.argv[2]; spec=sys.argv[3] if len(sys.argv)>3 else ''
lines=[l for l in src.split('\n') if not l.strip().startswith('#pragma inline_depth')]
text='\n'.join(lines)
text=re.sub(r'__forceinline(\s+[A-Za-z_:<>]+::~)',r'inline\1',text)
text=re.sub(r'__forceinline(\s+~)',r'\1',text)
text=re.sub(r'virtual __forceinline(\s+~)',r'virtual\1',text)
text=re.sub(r'__forceinline virtual(\s+~)',r'virtual\1',text)
m=list(re.finditer(r'^[A-Za-z_][^\n;]*::[^\n;]*\)\s*(?:const\s*)?\n\{',text,re.M))
start=m[-1].start() if m else 0
C=r'((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)'
pat=re.compile(r'\bif\s*\('+C+r'\)\s*(?:\{\s*return(\s+[^;{}]+)?;\s*\}|return(\s+[^;{}]+)?;)')
sites=[mm for mm in pat.finditer(text) if mm.start()>=start]
def cls(mm):
    pre=text[start:mm.start()].rstrip()
    if pre.endswith('{'):
        ln=pre.split('\n')[-1]
        if re.search(r'\b(for|while|do)\b',ln): return 'H'
    last=pre.split('\n')[-1]
    if 'NewScriptFrame' in last: return 'W'
    return 'S'
mask=None
mm_=re.search(r'm([012]+)',spec)
if mm_: mask=mm_.group(1); spec=spec.replace(mm_.group(0),'')
conv='c' in spec; spec=spec.replace('c','')
rconv='r' in spec; spec=spec.replace('r','')
rany='R' in spec; spec=spec.replace('R','')
lvl={}
for k in re.finditer(r'([HWS])(2?)',spec): lvl[k.group(1)]=2 if k.group(2) else 1
pieces=[]; last=0
for k,mm in enumerate(sites):
    c=cls(mm); sys.stderr.write(f"{k}\t{c}\t{mm.group(1)[:50]}\n")
    w=(int(mask[k]) if k<len(mask) else 0) if mask is not None else lvl.get(c,0)
    if not w: continue
    rv=(mm.group(2) or mm.group(3) or '')
    body='if ('+mm.group(1)+') return'+rv+';'
    for _ in range(w): body='do { '+body+' } while (0);'
    pieces.append(text[last:mm.start()]); pieces.append(body); last=mm.end()
pieces.append(text[last:]); text=''.join(pieces)
if conv:
    text=re.sub(r'for \(;;\) \{\s*\n(\s*)(?:if \(IsActiveThreadTerminating\(\)\)\s*\n?\s*return;|(?:do \{ )+if \(IsActiveThreadTerminating\(\)\) return;(?: \} while \(0\);)+)',
          r'while (!IsActiveThreadTerminating()) {',text)
if rconv or rany:
    # while (!IsActiveThreadTerminating()) { BODY } -> for (;;) { do { if (IsActiveThreadTerminating()) return; } while (0); BODY }
    # only when what follows the loop (to the function's closing brace) is empty or starts with a terminate-return check (r);
    # R: also when followed by anything (NOT semantics-preserving: diagnostic only)
    pos=0
    while True:
        k=text.find('while (!IsActiveThreadTerminating()) {',pos)
        if k<0: break
        d=0; j=k+len('while (!IsActiveThreadTerminating()) ')
        while True:
            if text[j]=='{': d+=1
            elif text[j]=='}':
                d-=1
                if d==0: break
            j+=1
        rest=text[j+1:]
        nxt=rest.lstrip()
        ok = nxt.startswith('}') or re.match(r'(do \{ )?if \(IsActiveThreadTerminating\(\)\)\s*\{?\s*return;',nxt)
        if ok or rany:
            ind=text[:k].split('\n')[-1]
            text=text[:k]+'for (;;) {\n'+ind+'    do { if (IsActiveThreadTerminating()) return; } while (0);'+text[k+len('while (!IsActiveThreadTerminating()) {'):]
            sys.stderr.write(f"r-conv at {k} ok={bool(ok)}\n")
        pos=k+5
open(out,'w').write(text)
