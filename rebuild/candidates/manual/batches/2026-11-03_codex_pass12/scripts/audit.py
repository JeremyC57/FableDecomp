#!/usr/bin/env python3
# Naming audit of graded rows: leaf + every relocation symbol vs PDB name sets, established spellings, queue wrong spellings,
# landed bindings. usage (repo root, lane env): python3 audit.py <codex12_dir> <handoff_root>
import sys, os, csv, json, re, glob, collections
sys.path.insert(0, 'tools/decomp_pipeline')
import verify_and_land as V, reloc_identity as R
from pathlib import Path
C, X = Path(sys.argv[1]), Path(sys.argv[2])
ROOT = Path('.').resolve()
pdb = set()
for f in ('ghidra_out/fablewin_pdb_names.tsv', 'ghidra_out/egor_pdb_names.tsv'):
    for l in open(f, errors='replace'):
        p = l.rstrip('\n').split('\t')
        if len(p) > 1: pdb.add(p[1])
est = {}
for r in csv.DictReader(open('rebuild/candidates/manual/naming/ESTABLISHED_SPELLINGS.tsv'), delimiter='\t'):
    est[int(r['address'], 16)] = r['symbol']
wrong = collections.defaultdict(list); qcorrect = {}
TOK = re.compile(r'(\?[^\s;|()]+|@[A-Za-z_]\w*@\d+|_[A-Za-z_]\w*(?:@\d+)?)')
for l in open('rebuild/candidates/manual/naming/NAME_FIXES_QUEUE.tsv', errors='replace'):
    p = l.rstrip('\n').split('\t')
    if len(p) < 5: continue
    try: a = int(p[1], 16)
    except ValueError: continue
    done = ' done]' in p[3]
    qcorrect.setdefault(a, []).append((p[2], p[0], done))
    for t in TOK.findall(p[4]):
        if t.startswith('?') or t.startswith('@'): wrong[t].append((a, p[0], done))
bind_by_t = collections.defaultdict(set); bind_by_s = collections.defaultdict(set)
for r in csv.DictReader(open('rebuild/oracles/reloc_bindings.tsv'), delimiter='\t'):
    t = int(r['target'], 16); bind_by_t[t].add(r['symbol']); bind_by_s[r['symbol']].add((t, r['address']))
orc = {r['address']: r for r in csv.DictReader(open(C/'oracle.tsv'), delimiter='\t')}
out = open(C/'AUDIT.tsv', 'w'); out.write('row\tkind\tsymbol\ttarget\tverdict\tnote\n')
sec = open(C/'second_spellings.tsv', 'w'); sec.write('row\ttarget\tsymbol\tlanded_spellings\n')
for r in csv.DictReader(open(C/'rows.tsv'), delimiter='\t'):
    a = r['address']; sel = json.loads((X/'rows'/a/'SELECTED_GRADE.json').read_text()); leaf = sel['leaf']
    obj = next(iter(glob.glob(str(C/'work'/a/'**'/(a+'.obj')), recursive=True)))
    retail = bytes.fromhex(orc[a]['bytes'])
    built = V.obj_text(obj, leaf)[0]
    hard, warn, pairs = R.check(ROOT, V.OBJDUMP, obj, leaf, a, retail, built=built)
    items = [('leaf', leaf, int(a, 16))]
    seen = set()
    for off, kind, sym, t in pairs:
        if (sym, t) in seen: continue
        seen.add((sym, t)); items.append(('reloc', sym, t))
    for kind, sym, t in items:
        flags = []
        v = 'OK' if sym in pdb else 'NOT_IN_PDB_SETS'
        others = sorted(bind_by_t.get(t, set()) - {sym})
        if v == 'OK' and others: v = 'PDB_OTHERS_BOUND'
        if t in est and est[t] != sym: flags.append(f'ESTABLISHED_CLASH: {est[t]}')
        corr = {q[0] for q in qcorrect.get(t, [])}
        lane = lambda x: x.replace('_W', 'G')   # lane /Zc:wchar_t- spells the PDB's _W as G
        hits = [w for w in wrong.get(sym, []) if w[0] == t]
        if hits and sym not in corr and lane(sym) not in {lane(c) for c in corr}:
            flags.append('QUEUE_WRONG_SPELLING: ' + '; '.join(f'{w[1]} 0x{w[0]:08X}{" done" if w[2] else ""}' for w in hits[:3]))
        oth_t = sorted({x for x, _ in bind_by_s.get(sym, set())} - {t})
        if oth_t: flags.append('SYMBOL_BOUND_ELSEWHERE: ' + ', '.join(f'{x:08x}' for x in oth_t))
        if t in qcorrect and not any(lane(q[0]) == lane(sym) for q in qcorrect[t]):
            flags.append('QUEUE_CORRECT_IS: ' + '; '.join(f'{q[0]} ({q[1]}{" done" if q[2] else ""})' for q in qcorrect[t][:2]))
        note = (f'{len(others)} other bound spellings: ' + '; '.join(others[:3])) if others else ''
        if flags: note = (note + ' | ' if note else '') + ' | '.join(flags)
        out.write(f'{a}\t{kind}\t{sym}\t{t:08x}\t{v}\t{note}\n')
        if others: sec.write(f'{a}\t{t:08x}\t{sym}\t{"; ".join(others)}\n')
    for h in hard: out.write(f'{a}\thard\t{h}\t\tHARD\t\n')
print('done')
