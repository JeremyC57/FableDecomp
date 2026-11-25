#!/usr/bin/env python3
"""calls.py <addr...>: retail call/jmp targets of a row (from NF431's dossier) with their table names (reloc_bindings) and self symbols."""
import sys, os, re, collections
S = os.environ['S']; R = '/home/user/FableDecomp/rebuild/oracles/'
rb = collections.defaultdict(set)
for l in open(R + 'reloc_bindings.tsv'):
    p = l.rstrip('\n').split('\t')
    if len(p) >= 2: rb[p[1].lower()].add(p[0])
ss = {}
for l in open(R + 'self_symbols.tsv'):
    p = l.rstrip('\n').split('\t')
    if len(p) >= 2: ss[p[0].lower()] = p[1]
for a in sys.argv[1:]:
    t = open(f'{S}/w22/agentNF431/dos/{a}.txt').read()
    ret = t.split('-- retail', 1)[1].split('-- FableWin', 1)[0]
    print('##', a)
    for m in re.finditer(r'(call|jmp)\s+0x([0-9a-f]+)', ret):
        tg = m.group(2).zfill(8)
        if int(tg, 16) >> 12 == int(a, 16) >> 12: continue
        names = sorted(rb.get(tg, []))
        print(f'  {m.group(1)} {tg} self={ss.get(tg, "-")[:110]} | table={" ; ".join(n[:110] for n in names[:3])}{" (+%d)" % (len(names)-3) if len(names) > 3 else ""}')
