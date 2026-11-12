"""rebind.py payload.json oracle.tsv workdir: re-derive rebuild/oracles/reloc_bindings.tsv rows for rechecked landed rows.

After a naming landing respells callers, the binding table still lists the old spellings under their first caller. For every
row of a --recheck-landed payload this reads the row's object from the vland work directory (<workdir>/**/<addr>/<addr>.obj),
takes its (symbol, retail target) pairs with reloc_identity.check(), removes the table rows whose first caller is that row,
and adds each pair whose symbol no other row already binds (first caller = this row). Then run
coordinator/bindings_multi_check.py: it must stay at its baseline. Prints removed/added counts; always exits 0.
EH funclet rows (oracle shorter than their parent object) are skipped: their parent row carries the bindings.
"""
import csv
import glob
import json
import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..', '..'))
import reloc_identity as RI  # noqa: E402
import verify_and_land as V  # noqa: E402

pay, orc, work = sys.argv[1:4]
root = os.getcwd()
objdump = os.environ.get('FABLE_OBJDUMP', 'objdump')
d = json.load(open(pay))['result']
rows = [r for v in d.values() for r in (v if isinstance(v, list) else [v]) if isinstance(r, dict) and r.get('address')]
oracle = {r['address']: bytes.fromhex(r['bytes']) for r in csv.DictReader(open(orc), delimiter='\t')}
p = 'rebuild/oracles/reloc_bindings.tsv'
lines = open(p, encoding='utf-8').read().rstrip('\n').split('\n')
hdr, tab = lines[0], [l.split('\t') for l in lines[1:]]
done, skipped, pairs_by = set(), [], {}
for r in rows:
    a = r['address'].lower()
    if a not in oracle:
        skipped.append((a, 'no oracle'))
        continue
    if r.get('funclet_offset') is not None:
        skipped.append((a, 'funclet'))
        continue
    objs = sorted(glob.glob(os.path.join(work, '**', a, a + '.obj'), recursive=True), key=os.path.getmtime)
    if not objs:
        skipped.append((a, 'no object'))
        continue
    leaf = r.get('leaf') or ''
    built = None
    try:
        res = V.obj_text(objs[-1], leaf)
        built = res[0]
    except Exception:
        pass
    hard, warn, pairs = RI.check(root, objdump, objs[-1], leaf, a, oracle[a], built=built)
    if not pairs and any(w.startswith('RELOC_UNCHECKED') for w in warn):
        skipped.append((a, 'unchecked'))
        continue
    pairs_by[a] = sorted({(sym, t) for _, _, sym, t in pairs})
    done.add(a)
before = len(tab)
tab = [t for t in tab if t[2] not in done]
removed = before - len(tab)
have = {t[0] for t in tab}
added = 0
for a in sorted(pairs_by):
    for sym, t in pairs_by[a]:
        if sym in have:
            continue
        tab.append([sym, f'{t:08x}', a])
        have.add(sym)
        added += 1
open(p, 'w', encoding='utf-8').write('\n'.join([hdr] + ['\t'.join(t) for t in tab]) + '\n')
print(f'removed {removed} added {added} rows {len(done)}')
for a, why in skipped:
    print('skipped', a, why)
