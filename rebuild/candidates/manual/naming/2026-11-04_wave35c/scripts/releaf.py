# releaf.py <addr> <module> <sym> [note]: draft = current source with an exact Leaf line (+ a naming note) -> drafts/<addr>.cpp
import sys, os, re
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from c35 import *
a,mod,sym=sys.argv[1:4]; note=sys.argv[4] if len(sys.argv)>4 else ''
s=src_of(a)
lines=s.split('\n')
li=[i for i,l in enumerate(lines) if re.match(r'^//\s*Leaf:',l)]
nl='// Leaf: '+sym
if li: lines[li[0]]=nl
else:
    k=1 if lines and lines[0].startswith('//') else 0
    lines.insert(k,nl)
if note:
    k=[i for i,l in enumerate(lines) if l.startswith('// Leaf:')][0]
    lines.insert(k+1,'// Naming wave 35c: '+note)
open(W+'/drafts/'+a+'.cpp','w').write('\n'.join(lines))
