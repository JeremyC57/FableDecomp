#!/usr/bin/env python3
"""finalize.py: matches.tsv -> out/<addr>.cpp with the line-2 marker dropped (or the unproven-wrapping note) and stale
pragma/forceinline comment text rewritten."""
import re,os,sys
I=os.path.dirname(os.path.abspath(__file__))
NOTE="// Inline-split: exits wrapped individually; retail's construct is unproven"
SUB={
 '00715110':[("in-body `#pragma inline_depth(0)` around the reload `Actions.Add(CCountedPointer(new ...))` keeps that temporary's ~CCountedPointer out of line (0x0048C5C0) while the aim/fire temporaries expand it",
              "the local CCountedPointer dtor is retail's 0x0048C5C0 body (it zeroes Data/Info); with that cost VC7.1's per-site rule calls it for the reload temporary (two branches deep) and expands it for the aim/fire temporaries")],
 '00d79d50':[("(implicit handle dtor expanded only at the first return, inline_depth(0) elsewhere).","(plain inline; the movie ctor is the out-of-line COMDAT 0x006E7B60 in this TU, called at both movie sites).")],
 '00d7a360':[("(implicit handle dtor expanded only at the first return, inline_depth(0) elsewhere).","(plain inline; the movie ctor is the out-of-line COMDAT 0x006E7B60 in this TU, called at both movie sites).")],
 '00ee7a20':[("(implicit handle dtor expanded only at the first return, inline_depth(0) elsewhere).","(plain inline; the movie ctor is the out-of-line COMDAT 0x006E7B60 in this TU, called at every movie site).")],
 '00ce0920':[("~CScriptThing __forceinline; ","plain-inline ~CScriptThing; "),(" with depth 0 just before the `;` (temp dtor out of line, counted operator= still direct)"," (temp dtor out of line by the per-site rule, counted operator= still direct); the pre-loop and loop-top terminate checks are do-while(0) statements")],
 '00ce0b00':[("~CScriptThing __forceinline; ","plain-inline ~CScriptThing; "),(" with depth 0 just before the `;` (temp dtor out of line, counted operator= still direct)"," (temp dtor out of line by the per-site rule, counted operator= still direct); the pre-loop and loop-top terminate checks are do-while(0) statements")],
 '00ce0ce0':[("~CScriptThing __forceinline; ","plain-inline ~CScriptThing; "),(" with depth 0 just before the `;` (temp dtor out of line, counted operator= still direct)"," (temp dtor out of line by the per-site rule, counted operator= still direct); the pre-loop and loop-top terminate checks are do-while(0) statements")],
 '00cf02a0':[("the three terminate exits inside the renown block: those three `return`s sit in `#pragma inline_depth(0)` regions.",
              "the three terminate exits inside the renown block (plain inline array dtor; the first and third renown-block checks are\n//   do-while(0) statements, VC7.1's per-site frequency rule does the rest).")],
 '00dde230':[("all of it under\n//   `#pragma inline_depth(0)` so its exits call ~CScriptThing out of line.","its terminate checks are\n//   do-while(0) statements (the loop-top check is not), and the per-site rule calls ~CScriptThing out of line there.")],
 '00e02e50':[("the timer dtor is __forceinline so the early exits (up to\n//   the target pick) destroy it inline, while the exits in the 18/12/8 stages and the closing brace, under inline_depth(0),\n//   call the out-of-line copy 0x00CD4470.",
              "the timer dtor is plain inline; the early terminate checks (up to\n//   the target pick) are do-while(0) statements and destroy it inline, while the exits in the 18/12/8 stages and the closing\n//   brace call the out-of-line copy 0x00CD4470 by VC7.1's per-site rule.")],
 '00e6b750':[("in-body `#pragma inline_depth(0)` around the region-hit terminate return gives the out-of-line ~CScriptThing","the loop-top, post-wait and region-hit terminate checks as do-while(0) statements; the wait-loop check stays a plain if, and its exit calls ~CScriptThing out of line by the per-site rule")],
}
for ln in open(I+'/matches.tsv'):
    a,f,n=ln.rstrip('\n').split('\t')
    s=open(f"{I}/rows/{a}/{f}").read()
    L=s.split('\n'); assert L[1].startswith('// Revisit: inline-depth'),a
    if 'do-while' in n: L[1]=NOTE
    else: del L[1]
    s='\n'.join(L)
    for o,r in SUB.get(a,[]):
        assert o in s,(a,o[:40]); s=s.replace(o,r)
    s=re.sub(r'\.PImp\n\s*;\n',r'.PImp;\n',s)
    bad=[l for l in s.split('\n') if 'inline_depth' in l or ('__forceinline' in l and '~' in l)]
    if bad: print(a,'LEFT:',bad[:3])
    open(f"{I}/out/{a}.cpp",'w').write(s)
print('ok')
