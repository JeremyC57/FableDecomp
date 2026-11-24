#!/usr/bin/env python3
"""syms336.py <obj..>: per object, DEF / UND external symbols (nf.ext_syms) -- names only, no bytes"""
import sys
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF336/tools'); import nf
for o in sys.argv[1:]:
    print('#####', o.split('/')[-1])
    for s in sorted(nf.ext_syms(o)): print('  ', s)
