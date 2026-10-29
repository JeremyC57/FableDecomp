# BINDINGS.tsv for wave 9: mkbind9.py <final pairs.jsonl> <new index jsonl> <rows.txt>
# old = symbols each edited row's object named before (baseline index idx/full.jsonl + idx/miss2.jsonl, compiled from the repo),
# new = its object after (index of ov_final) and the graded relocation pairs of the final recheck.
# RETIRE (old symbol, its bound target): no edited row names it now, no unedited indexed row names it, and no unindexed row's
# source mentions its identifier; KEEP otherwise. RETARGET-HASH / ADD-HASH: anonymous-namespace hashes follow the file text.
# ADD: a (symbol, target) pair of the final recheck not yet bound whose symbol is an exact Ego_r/FableWin spelling or a settled
# wave-9 final (only symbols the row names after, not before, its wave-9 edit); NO-BIND: a new stand-in spelling (not a PDB name). CONFLICT: bound to another target (none expected).
import sys, os, json, collections, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import inputs, und
from rows import rows, ROOT
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
pp, newidx, rf = sys.argv[1:4]
edited = set(l.strip() for l in open(rf) if l.strip())
R = rows()
OLD = {}
for f in ('idx/full.jsonl', 'idx/miss2.jsonl', 'idx/base7.jsonl'):
    for l in open(os.path.join(W, f)):
        r = json.loads(l)
        if 'err' not in r: OLD[r['a']] = set(r['d']) | set(r['u'])
NEW = {}
for l in open(newidx):
    r = json.loads(l)
    if 'err' not in r: NEW[r['a']] = set(r['d']) | set(r['u'])
PAIRS = {}
for l in open(pp):
    r = json.loads(l); PAIRS[r['a']] = set(map(tuple, r['pairs']))
BT, BS = inputs.bindings(); NM = inputs.names(); PDB = NM['Ego_r.pdb'] | NM['FableWin.pdb']
import targets9
other = collections.Counter()
for a, s in OLD.items():
    if a in edited: continue
    for x in s: other[x] += 1
unindexed = [a for a in R if a not in OLD and a not in edited]
def mentioned(sym):
    # every plain identifier of the qualified name (scopes and leaf) must appear in the row's source
    qn = und.qname(und.get([sym])[sym])
    comps = [c.lstrip('~') for c in re.sub(r'<.*>', '', qn).split('::') if re.match(r'^~?\w+$', c)]
    if not comps: return []
    hits = []
    for a in unindexed:
        try: t = open(os.path.join(ROOT, 'rebuild/src/compiled', R[a]['Source']), errors='replace').read()
        except OSError: continue
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
        else: act, note = 'RETIRE', 'no row names it after wave 9 (was: %s; bound by %s)' % (','.join(sorted(rs)), by)
    cnt[act] += 1; out.append((act, s, t, by, note))
# catalog-wide settlements (targets9.FINAL): a bound wrong spelling of the settled target that no row names at all (stale binding)
done = set(r[1] for r in out[1:])
for tgt, fin in sorted(targets9.FINAL.items()):
    for s in sorted(BT.get(tgt, ())):
        if s == fin or s in done or stillnew[s] or other[s]: continue
        if any(s in OLD[a] for a in edited if a in OLD and a not in NEW): continue
        m = mentioned(s)
        if m: continue
        t_, by = BS[s]
        if t_ != tgt: continue
        cnt['RETIRE'] += 1
        out.append(('RETIRE', s, tgt, by, 'stale: no indexed row names it and no unindexed row source mentions it (bound by %s; the row now emits %s)' % (by, fin)))
seen = set()
for a in sorted(PAIRS):
    fresh = (NEW.get(a, set()) - OLD[a]) if a in OLD else None   # symbols this row names only after the wave-9 edit
    for s, t in sorted(PAIRS[a]):
        if fresh is not None and s not in fresh: continue           # named before wave 9 too: not a wave-9 binding question
        if (s, t) in seen: continue
        seen.add((s, t))
        b = BS.get(s)
        if b and b[0] == t: continue
        if b: act, note = 'CONFLICT', 'bound to %s by %s; wave-9 row %s resolves it to %s' % (b[0], b[1], a, t)
        elif '?A0x' in s: act, note = 'ADD-HASH', 'anonymous-namespace hash of the edited file %s' % a
        elif s in PDB or s == targets9.FINAL.get(t): act, note = 'ADD', 'exact Ego_r/FableWin spelling, not yet bound; first named by %s' % a
        else: act, note = 'NO-BIND', 'stand-in spelling named by %s (not a PDB name): left unbound' % a
        cnt[act] += 1; out.append((act, s, t, a, note))
open(os.path.join(W, 'BINDINGS.tsv'), 'w').write(''.join('\t'.join(r) + '\n' for r in out))
print(cnt)
for r in out[1:]:
    if r[0] in ('CONFLICT',): print(r)
