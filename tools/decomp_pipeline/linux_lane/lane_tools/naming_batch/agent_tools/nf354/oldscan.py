#!/usr/bin/env python3
"""oldscan.py <symbols file> <obj...> : per object, which of the listed symbols it DEFINES / REFERENCES (external symbol table, exact
names) -> TSV obj, symbol-index, DEF|UND, symbol (agent NF354 (copied from NF350)). Also prints a summary line per object (none = no listed symbol)."""
import sys
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF354/tools')
import nf
syms = [l for l in open(sys.argv[1]).read().splitlines() if l.strip()]
for o in sys.argv[2:]:
    e = nf.ext_syms(o); hit = []
    for i, s in enumerate(syms):
        for k in ('DEF', 'UND'):
            if k + ' ' + s in e: hit.append('%d:%s' % (i, k))
    print('%s\t%s' % (o.rsplit('/', 1)[-1], ' '.join(hit) or 'none'))
