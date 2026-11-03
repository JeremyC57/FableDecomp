#!/usr/bin/env python3
# Second spellings per retail target: every symbol a claimed pass-12 row binds (leaf + relocations, from AUDIT.tsv) that
# differs from a spelling landed rows already bind to the same address. Classified against the PDB name sets and the
# PRIORITY queue. usage (repo root): python3 -I spell2.py <codex12> <out.tsv>
import sys, csv, collections, re
from pathlib import Path
C = Path(sys.argv[1])
pdb = set()
for f in ('ghidra_out/fablewin_pdb_names.tsv', 'ghidra_out/egor_pdb_names.tsv'):
    for l in open(f, errors='replace'):
        p = l.rstrip('\n').split('\t')
        if len(p) > 1: pdb.add(p[1])
bind = collections.defaultdict(set)
for r in csv.DictReader(open('rebuild/oracles/reloc_bindings.tsv'), delimiter='\t'): bind[int(r['target'], 16)].add(r['symbol'])
q = collections.defaultdict(list)
for l in open('rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv', errors='replace'):
    p = l.rstrip('\n').split('\t')
    if len(p) < 4: continue
    for tok in re.findall(r'(?:0x)?([0-9A-Fa-f]{8})', p[1]):
        q[int(tok, 16)].append((p[0], p[2], ' done]' in p[3]))
lane = lambda s: s.replace('_W', 'G')
rows = collections.defaultdict(set); kinds = {}
for r in csv.DictReader(open(C/'AUDIT.tsv'), delimiter='\t'):
    if r['kind'] not in ('leaf', 'reloc') or not r['target']: continue
    t = int(r['target'], 16); rows[(t, r['symbol'])].add(r['row']); kinds[(t, r['symbol'])] = r['kind']
out = open(sys.argv[2], 'w')
out.write('target\tcodex_symbol\tkind\tin_pdb\tqueue_status\tqueue_entries\trows\tlanded_spellings\tproposed_action\n')
n = collections.Counter()
for (t, s), rs in sorted(rows.items()):
    others = sorted(bind.get(t, set()) - {s})
    if not others: continue
    ip = 'yes' if s in pdb else 'no'
    qe = q.get(t, [])
    open_q = [e for e in qe if not e[2]]
    if any(lane(e[1]) == lane(s) or e[1].startswith(s+' ') for e in qe): qs = 'queued_correct_' + ('open' if any((lane(e[1]) == lane(s) or e[1].startswith(s+' ')) and not e[2] for e in qe) else 'done')
    elif qe: qs = 'queue_disagrees'
    else: qs = 'not_queued'
    if qs.startswith('queued_correct'): act = 'already queued: respell the landed binders at landing'
    elif qs == 'queue_disagrees': act = 'REVIEW: queue names another spelling for this address'
    elif ip == 'yes': act = 'PRIORITY cx12: PDB spelling; respell the landed binders at landing'
    elif s in bind.get(t, set()): act = '-'
    else: act = 'REVIEW cx12: no PDB entry (vendor/data/descriptive); settle before landing'
    n[(ip, qs)] += 1
    qtxt = '; '.join(f'{e[0]}:{e[1][:90]}{" done" if e[2] else ""}' for e in qe[:3])
    out.write(f'{t:08x}\t{s}\t{kinds[(t, s)]}\t{ip}\t{qs}\t{qtxt}\t{" ".join(sorted(rs))}\t{"; ".join(others)}\t{act}\n')
for k, v in sorted(n.items()): print(k, v)
