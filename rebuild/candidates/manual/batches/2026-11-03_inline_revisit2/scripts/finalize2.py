#!/usr/bin/env python3
"""finalize2.py: the 5 lever matches of the second pass -> out/<addr>.cpp (line-2 marker dropped, stale comments rewritten)."""
import os,re
I=os.path.dirname(os.path.abspath(__file__))
R={
'009f7890':('x/v2_od.cpp',[
 ("is built with optimisation off:","is built with optimisation off (`// Flags: /Od`, the TU's own switch; /Od implies /Ob0):"),
 ("stored to a dead temporary. `#pragma optimize(\"\", off)` alone still expands the inline placement new (the pragma\n//   does not reach /Ob); `inline_depth(0)` stands for the TU's /Ob0, which keeps the call. The /Gr calling convention is\n//   spelled `__fastcall`.",
  "stored to a dead temporary. `#pragma optimize(\"\", off)` alone still expands the inline placement new (the pragma\n//   does not reach /Ob); the TU flag /Od keeps STLport's inline placement new a call, as retail's out-of-line 0x00405C10\n//   shows some TU did. The /Gr calling convention is spelled `__fastcall`. (Pragma-only alternative that also matches:\n//   keep `#pragma optimize(\"\", off)` and declare the placement new without a body.)")]),
'00ef6fe0':('x/v10_explicit_inst_ctor.cpp',[
 ("which reproduces the retail call; this level is defined out of line so the TU emits it.",
  "which reproduces the retail call; this level is the class template's own inline copy constructor, emitted as its\n// COMDAT by an explicit instantiation of that member (retail emitted it out of line because a caller did not inline it).")]),
'0066bd00':('x/g2_itfirst.cpp',[
 ("__forceinline dtors, the found-handle `return` in\n//   `#pragma inline_depth(0)`)","plain inline dtors, the walk guarded by\n//   `if (it != names.end())`)"),
 ("the walk keeps the cursor in edi and re-reads end()\n//   after each call.","the walk keeps the cursor in edi and re-reads end()\n//   after each call; the cursor is taken before the empty test (`mov edi,esi` ahead of the `je`), and that guard halves the\n//   frequency of the found-handle exit, so VC7.1's per-site rule calls the vector dtor there and expands it at the end.")]),
'0066bdf0':('x/g2_itfirst.cpp',[
 (" on top of the parked forceinline vector dtors + inline_depth(0) return:"," on top of the plain-inline vector dtors and the `if (it != names.end())` loop guard:"),
 ("the STLport vector destructors are\n//   __forceinline and the found-handle `return` sits in `#pragma inline_depth(0)` (GOTCHAS \"mixed inline/out-of-line\"\n//   recipe), which gives retail's inline fall-through copy and the out-of-line call. No pragma otherwise.",
  "the STLport vector destructors are\n//   plain inline and the walk is guarded by `if (it != names.end())` with the cursor taken first; the guard halves the\n//   found-handle exit's frequency, so VC7.1's per-site rule calls the dtor there and expands the fall-through copy."),
 ("//   tried: plain inline dtors (both exits inline, 267v223);","//   tried: plain inline dtors without the guard (both exits inline, 267v223);")]),
'00bc9b80':('x/v1_noprag_force.cpp',[
 ("FindNode is recursive and visible in this TU, so VC7.1 inlines its first level here","FindNode is recursive, visible in this TU and __forceinline (no inline_recursion), so VC7.1 inlines its first level here"),
 ("a helper member (Children()/Key()) is left as a call by the depth limit.","a helper member (Children()/Key()) would be left as a call.")]),
'00dd6440':('ex/110011111.cpp',[
 ("wait and out of line (0x00CD4470) from that wait on (in-body inline_depth(0)).",
  "wait and out of line (0x00CD4470) from that wait on: plain inline, every terminate check a do-while(0) statement except\n//   the two around the StartScriptingEntity wait, and VC7.1's per-site frequency rule gives the split.")]),
}
NOTE="// Inline-split: exits wrapped individually; retail's construct is unproven"
WRAPPED={'00dd6440'}
for a,(f,subs) in R.items():
    s=open(f"{I}/rows/{a}/{f}").read(); L=s.split('\n'); assert L[1].startswith('// Revisit: inline-depth'),a; L[1:2]=[NOTE] if a in WRAPPED else []; s='\n'.join(L)
    for o,r in subs:
        assert o in s,(a,o[:50]); s=s.replace(o,r)
    bad=[l for l in s.split('\n') if 'inline_depth' in l and not l.startswith('//   tried')]
    if bad: print(a,'LEFT:',bad[:3])
    open(f"{I}/out/{a}.cpp",'w').write(s)
print('ok')
