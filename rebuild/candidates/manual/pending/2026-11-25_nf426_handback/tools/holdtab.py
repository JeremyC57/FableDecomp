#!/usr/bin/env python3
"""holdtab.py: one line per family address (the two shared stand-ins): shape, TU (FableWin.pdb module via plain agree_pdb anchors on both
sides) or boundary, retail copies in the TU, debug candidates, verdict + reason, the candidate Ts. -> FAMILY_TABLE.tsv"""
import json, re, collections, sys
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF426'
r = json.load(open(A + '/status/rec.json')); occ = json.load(open(A + '/evidence/occ.json'))
t = json.load(open(A + '/evidence/occ_tu.json')); tu = json.load(open(A + '/evidence/tu_occ.json')); c = json.load(open(A + '/evidence/tu_cand.json'))
acc = {}
for b in ('b1', 'b2', 'b3'):
    for l in open(A + '/batches/%s/PLAN.tsv' % b): f = l.rstrip('\n').split('\t'); acc[f[0]] = (b, f[4])
s = {x['obj']: x for x in json.load(open(A + '/evidence/single.json'))}
FAM = {'?Reset@AutoTinyCountedHandle@@QAEXXZ', '?Release@CIVCP@@QAEXXZ'}
out = open(A + '/FAMILY_TABLE.tsv', 'w'); cnt = collections.Counter()
print('address\tstandin\tshape\ttu\tretail_in_tu\tdebug_cands\tverdict\treason\tnew_or_candidates', file=out)
for a in sorted(occ):
    if r[a]['self_leaf'] not in FAM: continue
    sh = occ[a][0]; v = t[a]
    if a in acc:
        verdict, why, names = 'ACCEPT', 'single-T count-equal TU + Ego_r second source', acc[a][1]
    elif v[5] != 'same':
        verdict, why, names = 'HOLD', 'boundary: nearest plain anchors are in different objects (%s / %s)' % (v[2], v[4]), ''
    else:
        o = v[2]; n1 = len(tu[o]); cd = c.get(o, []); n2 = len(cd)
        Ts = sorted({re.sub(r'^(\?\?1|\?Release@)\?\$', '', nm).split('@@@@')[0] for i, nm, z in cd})
        names = ' '.join(Ts)
        if n1 != n2: verdict, why = 'HOLD', 'count mismatch: retail %d copies v debug %d candidates (kept copies follow link order)' % (n1, n2)
        elif o in s and not s[o]['nego']: verdict, why = 'HOLD', 'single T, one source only (no Ego_r object instantiates it)'
        elif o in s: verdict, why = 'HOLD', 'single T but not generated (lock / template form)'
        else: verdict, why = 'HOLD', 'count-equal multi-T TU (%d Ts): T set proven, per-copy order not' % len(Ts)
    cnt[(verdict, why.split(':')[0].split(' (')[0])] += 1
    print('\t'.join([a, r[a]['self_leaf'], sh, v[2] if v[5] == 'same' else 'boundary', str(len(tu.get(v[2], []))) if v[5] == 'same' else '-',
                     str(len(c.get(v[2], []))) if v[5] == 'same' else '-', verdict, why, names]), file=out)
for k, n in sorted(cnt.items()): print(n, k)
