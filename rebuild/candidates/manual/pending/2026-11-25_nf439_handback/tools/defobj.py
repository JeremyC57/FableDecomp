#!/usr/bin/env python3
"""defobj.py <addr...>: compile the landed (HEAD) source of each definer row with VC7.1 (nf.compile_one, the row's flags) into
work/g/<addr>_D/<addr>.obj and print the grade + the object's DEF symbols matching CCountedPointer / CThing."""
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline')
for a in sys.argv[1:]:
    st, leaf, fl, obj, wd = nf.compile_one(a, nf.src_of(a), 'D')
    d = sorted(s[4:] for s in nf.ext_syms(obj) if s.startswith('DEF ')) if obj.exists() else ['NOOBJ']
    print(a, st if isinstance(st, str) else getattr(st, 'name', st), obj, len(d), [x for x in d][:6])
