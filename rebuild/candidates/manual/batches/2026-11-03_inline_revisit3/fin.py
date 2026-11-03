#!/usr/bin/env python3
"""fin.py src out addr 'first:last' 'replacement text' [...] : finalise a match: line 2 -> Inline-split note, replace header line
ranges (1-based, inclusive) with the given text (lines split on \\n, each prefixed '// ' unless already a comment)."""
import sys
src,out,addr=sys.argv[1:4]; L=open(src).read().split('\n')
assert L[1].startswith('// Revisit: inline-depth'), L[1][:60]
reps=[]
for i in range(4,len(sys.argv),2):
    a,b=map(int,sys.argv[i].split(':')); reps.append((a,b,sys.argv[i+1]))
for a,b,txt in sorted(reps,reverse=True):
    new=[l if l.startswith('//') else '// '+l for l in txt.split('\n')]
    L[a-1:b]=new
L[1]="// Inline-split: exits wrapped individually; retail's construct is unproven"
t='\n'.join(L)
assert 'inline_depth' not in t.replace("Inline-split",""), 'leftover inline_depth'
open(out,'w').write(t)
