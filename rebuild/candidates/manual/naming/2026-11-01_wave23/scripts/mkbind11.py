# BINDINGS.tsv for wave 11 (from mkbind10.py): mkbind11.py <final pairs.jsonl> <new index jsonl> <rows.txt>
# old = symbols each edited row's object named before (baseline index idx/full.jsonl, the CURRENT repo compiled),
# new = its object after (index of ov_final) and the graded relocation pairs of the final recheck.
# RETIRE (old symbol, its bound target): no edited row names it now, no unedited indexed row names it, and no unindexed row's
#   source mentions its identifiers; KEEP otherwise. RETARGET-HASH / ADD-HASH: anonymous-namespace hashes follow the file text.
# Stale RETIRE: another bound spelling of a target this wave settled (FINAL below) that no indexed row names and no unindexed
#   source mentions. ADD: a (symbol, target) pair of the final recheck, not yet bound, that the row names only after its wave-11
#   edit (the settled spelling: PDB = in Ego_r/FableWin, else the chosen libpk / descriptive spelling). CONFLICT: bound elsewhere.
# Also writes runs/reloc_bindings.after.tsv (the repo table with the RETIREs removed and the ADDs appended).
import sys, os, json, collections, re, csv
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import inputs, und
from rows import rows, ROOT
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
pp, newidx, rf = sys.argv[1:4]
edited = set(l.strip() for l in open(rf) if l.strip())
R = rows()
OLD, OLDD = {}, {}
for l in open(os.path.join(W, 'idx/full.jsonl')):
    r = json.loads(l)
    if 'err' not in r: OLD[r['a']] = set(r.get('d', [])) | set(r.get('u', [])); OLDD[r['a']] = set(r.get('d', []))
NEW, NEWD = {}, {}
for l in open(newidx):
    r = json.loads(l)
    if 'err' not in r: NEW[r['a']] = set(r.get('d', [])) | set(r.get('u', [])); NEWD[r['a']] = set(r.get('d', []))
PAIRS = {}
for l in open(pp):
    r = json.loads(l); PAIRS[r['a']] = set(map(tuple, r['pairs']))
BT, BS = inputs.bindings(); NM = inputs.names(); PDB = NM['Ego_r.pdb'] | NM['FableWin.pdb']
FINAL = {}
for a in PAIRS:
    fresh = (NEW.get(a, set()) - OLD[a]) if a in OLD else set()
    for s, t in PAIRS[a]:
        if s in fresh and '?A0x' not in s: FINAL.setdefault(t, set()).add(s)
other = collections.Counter()
for a, s in OLD.items():
    if a in edited: continue
    for x in s: other[x] += 1
unindexed = [a for a in R if a not in OLD and a not in edited]
_txt = {}
def mentioned(sym):
    qn = und.qname(und.get([sym])[sym])
    comps = [c.lstrip('~') for c in re.sub(r'<.*>', '', qn).split('::') if re.match(r'^~?\w+$', c)]
    if not comps: return []
    hits = []
    for a in unindexed:
        if a not in _txt:
            try: _txt[a] = open(os.path.join(ROOT, 'rebuild/src/compiled', R[a]['Source']), errors='replace').read()
            except OSError: _txt[a] = ''
        t = _txt[a]
        if all(re.search(r'\b' + re.escape(c) + r'\b', t) for c in comps): hits.append(a)
    return hits
stillnew = collections.Counter(x for a in edited for x in NEW.get(a, ()))
removed = collections.defaultdict(set)
for a in sorted(edited):
    if a not in OLD or a not in NEW: continue
    for s in OLD[a] - NEW[a]:
        if s in BS: removed[s].add(a)
out = [('action', 'symbol', 'target', 'by_row', 'note')]
cnt = collections.Counter()
for s, rs in sorted(removed.items(), key=lambda kv: (BS[kv[0]][0], kv[0])):
    t, by = BS[s]
    if '?A0x' in s: act, note = 'RETARGET-HASH', 'anonymous-namespace hash of an edited file (follows the text); was named by %s' % ','.join(sorted(rs))
    elif stillnew[s]: act, note = 'KEEP', 'still named by %d edited row(s)' % stillnew[s]
    elif other[s]: act, note = 'KEEP', 'still named by %d unedited row(s) (index)' % other[s]
    else:
        m = mentioned(s)
        if m: act, note = 'KEEP', 'identifier appears in %d unindexed row(s) (%s): conservative' % (len(m), ','.join(m[:4]))
        else:
            fin = sorted(FINAL.get(t, ()))
            act, note = 'RETIRE', 'no row names it after wave 11 (was: %s; bound by %s)%s' % (','.join(sorted(rs)), by, ('; the target is now ' + ' / '.join(fin)) if fin else '')
    cnt[act] += 1; out.append((act, s, t, by, note))
done = set(r[1] for r in out[1:])
for tgt, fins in sorted(FINAL.items()):
    for s in sorted(BT.get(tgt, ())):
        if s in fins or s in done or stillnew[s] or other[s]: continue
        if mentioned(s): continue
        t_, by = BS[s]
        if t_ != tgt: continue
        cnt['RETIRE'] += 1; done.add(s)
        out.append(('RETIRE', s, tgt, by, 'stale: no indexed row names it and no unindexed row source mentions it (bound by %s; the target is now %s)' % (by, ' / '.join(sorted(fins)))))
seen = set()
for a in sorted(PAIRS):
    fresh = (NEW.get(a, set()) - OLD[a]) if a in OLD else None
    for s, t in sorted(PAIRS[a]):
        if fresh is not None and s not in fresh: continue
        if (s, t) in seen: continue
        seen.add((s, t))
        b = BS.get(s)
        if b and b[0] == t: continue
        if b: act, note = 'CONFLICT', 'bound to %s by %s; wave-11 row %s resolves it to %s' % (b[0], b[1], a, t)
        elif '?A0x' in s: act, note = 'ADD-HASH', 'anonymous-namespace hash of the edited file %s' % a
        elif s in PDB: act, note = 'ADD', 'exact Ego_r/FableWin spelling, not yet bound; first named by %s' % a
        else: act, note = 'ADD', 'wave-11 spelling (no PDB name: libpk / GameCODA / descriptive owner or compiler-derived instantiation), not yet bound; first named by %s' % a
        cnt[act] += 1; out.append((act, s, t, a, note))
open(os.path.join(W, 'BINDINGS.tsv'), 'w').write(''.join('\t'.join(r) + '\n' for r in out))
# apply to a copy of the repo table
ret = set((r[1], r[2]) for r in out[1:] if r[0] in ('RETIRE', 'RETARGET-HASH'))
add = [(r[1], r[2], r[3]) for r in out[1:] if r[0] in ('ADD', 'ADD-HASH')]
lines = open(ROOT + '/rebuild/oracles/reloc_bindings.tsv').read().splitlines()
keep = [lines[0]] + [l for l in lines[1:] if tuple(l.split('\t')[:2]) not in ret]
open(os.path.join(W, 'runs/reloc_bindings.after.tsv'), 'w').write('\n'.join(keep + ['\t'.join(x) for x in add]) + '\n')
print(cnt, 'removed lines', len(lines) - len(keep), 'added', len(add))
for r in out[1:]:
    if r[0] == 'CONFLICT': print(r)
# new defined symbols per edited row (leaf respellings only expected)
for a in sorted(edited):
    if a in OLDD and a in NEWD:
        nd = NEWD[a] - OLDD[a]
        nd = [s for s in nd if not s.startswith('??_C@') and not s.startswith('__real@') and '?A0x' not in s]
        if nd: print('NEWDEF', a, ' '.join(sorted(nd))[:400])
