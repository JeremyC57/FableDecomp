#!/usr/bin/env python3
"""mkrow.py <W> <specfile> <outdir>: write rows/<a>.cpp and mut/<a>.cpp from a spec file. Spec blocks separated by a line '@@':
  addr|leaf|one-line summary
  D: MACRO ...            (defines, in order; FABLE_RETAIL_NSCRIPT is always first)
  I: engine/X.h ...       (includes after engine/CGameScriptInterface.h)
  E: evidence line        (any number; comment lines)
  M: old => new           (mutation: replace text `old` in the body with `new`, must occur once)
  then the definition text (C++), verbatim."""
import sys, os, re
W, spec, out = sys.argv[1:4]
os.makedirs(out + '/rows', exist_ok=True); os.makedirs(out + '/mut', exist_ok=True)
for blk in open(spec).read().split('\n@@\n'):
    L = [l for l in blk.strip('\n').split('\n')]
    if not L or not L[0].strip(): continue
    a, leaf, summ = L[0].split('|', 2)
    D, I, E, M, body = [], [], [], [], []
    for l in L[1:]:
        if l.startswith('D: '): D += l[3:].split()
        elif l.startswith('I: '): I += l[3:].split()
        elif l.startswith('E: '): E.append(l[3:])
        elif l.startswith('M: '): M.append(l[3:].split(' => '))
        else: body.append(l)
    body = '\n'.join(body).strip('\n') + '\n'
    hdr = [f'// {summ}', f'// Leaf: {leaf}',
           f'// Retail: Fable.exe (TLC, VC7.1) @ 0x{a.upper()}. Identity (NF433, 2026-11-25): the retail RTTI vftable placement of',
           '//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /',
           '//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).']
    hdr += ['// ' + e for e in E]
    pre = ['#define FABLE_RETAIL_NSCRIPT'] + ['#define ' + d for d in D] + ['#include "engine/CGameScriptInterface.h"'] + [f'#include "{i}"' for i in I]
    src = '\n'.join(hdr + pre) + '\n\n' + body
    open(f'{out}/rows/{a}.cpp', 'w').write(src)
    mb = body
    for o, n in M:
        assert mb.count(o) == 1, (a, o); mb = mb.replace(o, n)
    if M: open(f'{out}/mut/{a}.cpp', 'w').write('\n'.join(hdr + pre) + '\n\n' + mb)
    print(a, 'ok' if M else 'ok (no mutant)')
