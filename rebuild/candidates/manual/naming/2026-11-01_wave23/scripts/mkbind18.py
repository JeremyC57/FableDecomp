# BINDINGS.tsv for wave 15 (from mkbind14.py): mkbind15.py <final pairs.jsonl> <new index jsonl> <rows.txt>
# wave 13: anonymous-namespace symbols are normalised to the Ego_r/FableWin hash over the whole table (owner policy 2026-11-01:
#   the ?A0x hash is TU-path derived, the tools match ignoring it); RETARGET fixes a binding that names the wrong function.
# old = symbols each edited row's object named before (baseline index idx/full.jsonl, the CURRENT repo compiled),
# new = its object after (index of ov_final) and the graded relocation pairs of the final recheck.
# RETIRE (old symbol, its bound target): no edited row names it now, no unedited indexed row names it, and no unindexed row's
#   source mentions its identifiers; KEEP otherwise. RETARGET-HASH / ADD-HASH: anonymous-namespace hashes follow the file text.
# Stale RETIRE: another bound spelling of a target this wave settled (FINAL below) that no indexed row names and no unindexed
#   source mentions. ADD: a (symbol, target) pair of the final recheck, not yet bound, that the row names only after its wave-13
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
for l in open(os.environ.get('IDX', os.path.join(W, 'idx/full.jsonl'))):
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
norm = lambda x: re.sub(r'\?A0x[0-9a-f]{8}', '?A0x', x)
PH = collections.defaultdict(set)
for x in PDB:
    if '?A0x' in x: PH[norm(x)].add(x)
# CThingCompare_InDirectionOfAndNearestToHeuristicBitch lives in two TUs (Ego_r ?A0xffab493a / ?A0xeaff0e08): the retail
# targets follow the decision + retail adjacency (AdvanceInCover TU / TakeCover TU)
MULTI_TU = {'00978790': '?A0xffab493a', '00979370': '?A0xffab493a', '0097abf0': '?A0xeaff0e08', '0097b2c0': '?A0xeaff0e08'}
def anon_pdb(x, t):
    c = PH.get(norm(x), set())
    if len(c) == 1: return next(iter(c))
    h = MULTI_TU.get(t.lower())
    if h:
        c = [y for y in c if h in y]
        if len(c) == 1: return c[0]
    return None
# a binding that names the wrong retail function (CDefString: the conversion operator is 0x00415D70, GetString 0x00415DD0)
RETARGET = {}
EST = {}
for l in open(os.path.join(W, 'ESTABLISHED_SPELLINGS.tsv'), errors='replace'):
    q = l.rstrip('\n').split('\t')
    if len(q) >= 3: EST[q[1]] = q[2]
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
    if '?A0x' in s: continue   # anonymous-namespace symbols: the table carries the PDB hash (pass ANON below)
    elif stillnew[s]: act, note = 'KEEP', 'still named by %d edited row(s)' % stillnew[s]
    elif other[s]: act, note = 'KEEP', 'still named by %d unedited row(s) (index)' % other[s]
    else:
        m = mentioned(s)
        if m: act, note = 'KEEP', 'identifier appears in %d unindexed row(s) (%s): conservative' % (len(m), ','.join(m[:4]))
        else:
            fin = sorted(FINAL.get(t, ()))
            act, note = 'RETIRE', 'no row names it after wave 18 (was: %s; bound by %s)%s' % (','.join(sorted(rs)), by, ('; the target is now ' + ' / '.join(fin)) if fin else '')
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
seen = set(); ANON_DONE = set()
for a in sorted(PAIRS):
    fresh = (NEW.get(a, set()) - OLD[a]) if a in OLD else None
    for s, t in sorted(PAIRS[a]):
        if fresh is not None and s not in fresh and not ('?A0x' in s and norm(s) in set(map(norm, fresh))) and s not in RETARGET: continue
        if (s, t) in seen: continue
        seen.add((s, t))
        b = BS.get(s)
        if '?A0x' in s:
            p = anon_pdb(s, t)
            if not p:
                # no Ego_r/FableWin spelling of this anonymous-namespace symbol (data symbols are not in the name sets): the
                # compiled spelling, one entry per (hash-normalised symbol, target) -- the tools match ignoring the hash
                k = (norm(s), t)
                if k in ANON_DONE or any(norm(x) == norm(s) for x in BT.get(t, ())): continue
                ANON_DONE.add(k); cnt['ADD'] += 1
                out.append(('ADD', s, t, a, 'anonymous-namespace data symbol, no Ego_r/FableWin data-symbol spelling recorded (the name sets carry only its dynamic initializer); the compiled TU hash -- the tools match ignoring the hash (owner policy 2026-11-01); first named by %s' % a))
                continue
            s = p; b = BS.get(s)
            if (s, t) in seen: continue
            seen.add((s, t))
        if b and b[0] == t: continue
        if b and s in RETARGET:
            cnt['RETIRE'] += 1; out.append(('RETIRE', s, b[0], b[1], RETARGET[s][1]))
            cnt['ADD'] += 1; out.append(('ADD', s, t, a, RETARGET[s][2]))
            continue
        if b: act, note = 'CONFLICT', 'bound to %s by %s; wave-18 row %s resolves it to %s' % (b[0], b[1], a, t)
        elif '?A0x' in s: act, note = 'ADD', 'exact Ego_r/FableWin anonymous-namespace spelling (PDB hash, owner policy 2026-11-01), not yet bound; first named by %s' % a
        elif s in EST: act, note = 'ADD', 'established spelling (ESTABLISHED_SPELLINGS.tsv: %s), not yet bound; first named by %s' % (EST[s], a)
        elif '@CThingSearchTools@@QBE' in s: act, note = 'ADD', 'const member of CThingSearchTools (called through Ego_r CTCBase::PeekThingSearchTools() const, wave 13 stools); filter / container types stay row-local; first named by %s' % a
        elif 'IS_THING_ALIVE' in s: act, note = 'ADD', 'ICF: the retail body 0x006961F0 is the identical-COMDAT fold of the IS_THING_ALIVE<T> instantiations; this is the CIntelligentPointer<CThingCreatureBase> one the CCreatureAction target member names (DECISIONS_W14_RESOLVED 0x0097A630; Ego_r template IS_THING_ALIVE); first named by %s' % a
        elif s in PDB: act, note = 'ADD', 'exact Ego_r/FableWin spelling, not yet bound; first named by %s' % a
        else: act, note = 'ADD', 'wave-18 spelling (no PDB name: libpk / GameCODA / descriptive owner or compiler-derived instantiation), not yet bound; first named by %s' % a
        cnt[act] += 1; out.append((act, s, t, a, note))
# a stale second name of a settled target whose only remaining namer means another address (00b7ad20: 0x01436E1C)
FORCE_RETIRE = {}
# queue rows whose wrong spelling is bound but named by no landed row (c676 / c677): retired outright
MANUAL_RETIRE = {('?nonvirt_dtor@CEngineSubPrimitiveGameMesh@@QAEXXZ', '006661b0'): 'c676: 0x006661B0 is ??1CThingCreatureBase@@UAE@XZ; no row names this stand-in',
                 ('?ClearMessages@CMessageOwner@@QAEXXZ', '00c60410'): 'c677: 0x00C60410 is CEaxContext::ClearEvents (descriptive EAX layer); no row names this stand-in'}
for (x, t), n in MANUAL_RETIRE.items():
    if x in BS and BS[x][0] == t and not other[x] and not stillnew[x] and not any(r[1] == x for r in out[1:]):
        cnt['RETIRE'] += 1; out.append(('RETIRE', x, t, BS[x][1], n))
for (x, t), n in FORCE_RETIRE.items():
    if x in BS and BS[x][0] == t:
        out = [r for r in out if not (r[0] == 'KEEP' and r[1] == x)]
        cnt['RETIRE'] += 1; cnt['KEEP'] -= 1; out.append(('RETIRE', x, t, BS[x][1], n))
for x, (nt, n1, n2) in RETARGET.items():
    if x in BS and BS[x][0] != nt and not any(r[1] == x and r[2] == nt for r in out[1:]):
        by = sorted(a for a in PAIRS if (x, nt) in PAIRS[a])
        if not by: print('RETARGET-UNPROVEN', x); continue
        cnt['RETIRE'] += 1; out.append(('RETIRE', x, BS[x][0], BS[x][1], n1))
        cnt['ADD'] += 1; out.append(('ADD', x, nt, by[0], n2))
boundnow = set((x, BS[x][0]) for x in BS) | set((r[1], r[2]) for r in out[1:] if r[0] == 'ADD')
for x in sorted(BS, key=lambda k: (BS[k][0], k)):
    if '?A0x' not in x: continue
    t, by = BS[x]
    p = anon_pdb(x, t)
    if not p or p == x: continue
    cnt['RETIRE'] += 1; out.append(('RETIRE', x, t, by, 'anonymous-namespace hash normalisation: the hash is TU-path derived; the table carries the Ego_r/FableWin hash %s' % p))
    if (p, t) not in boundnow:
        boundnow.add((p, t)); cnt['ADD'] += 1
        out.append(('ADD', p, t, by, 'anonymous-namespace hash normalisation (owner policy 2026-11-01): exact Ego_r/FableWin spelling of %s' % x))
_ret = set((r[1], r[2]) for r in out[1:] if r[0] == 'RETIRE')
out = [out[0]] + [r for r in out[1:] if not (r[0] == 'KEEP' and (r[1], r[2]) in _ret)]
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
