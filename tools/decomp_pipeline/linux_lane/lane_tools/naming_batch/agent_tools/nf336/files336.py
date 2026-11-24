#!/usr/bin/env python3
"""files336.py <addr..|-f ENTRIES>: catalog block per address (Source, TestSource, CompilerFlags, PassPattern, Module, Name)
bounded to the row's own [pscustomobject]@{ ... } block. Output TSV: addr src test flags pass name"""
import re,sys
R='/home/user/FableDecomp/rebuild/build_candidates.ps1'
args=sys.argv[1:]
if args and args[0]=='-f':
    args=[l.strip() for l in open(args[1]) if l.strip()]
want=set(a.lower().replace('0x','') for a in args)
txt=open(R,encoding='utf-8',errors='replace').read()
blocks=txt.split('[pscustomobject]@{')
for b in blocks[1:]:
    b=b.split('\n    }')[0] if '\n    }' in b else b
    m=re.search(r"Address\s*=\s*'([0-9a-fA-F]{8})'",b)
    if not m: continue
    a=m.group(1).lower()
    if a not in want: continue
    def g(k):
        mm=re.search(r"\n\s*"+k+r"\s*=\s*'([^']*)'",b)
        return mm.group(1) if mm else ''
    print('\t'.join([a,g('Source'),g('TestSource'),g('CompilerFlags'),g('PassPattern'),g('Module')+'::'+g('Name')]))
