# BINDINGS.auto.tsv + BINDINGS.tsv for wave 8: mkbind8.py <base_tag> <new_tag> <rows.txt>
# auto: per (old symbol, target) -> new symbol deltas of the baseline vs the final recheck of the edited rows.
# old = symbol a row stopped emitting for a target, new = what it emits now. RETIRE when no row outside the edited set emits old
# (naming/w8/full.jsonl = the w7 index + the 485 rows rescanned in wave 8, 2026-10-29) and no edited row still emits it; KEEP otherwise.
# actions: RETIRE (drop old -> target), KEEP, RETARGET (rebind to another target), ADD (PDB / settled spelling not yet bound),
# ADD-HASH (anonymous-namespace hash, follows the file), NO-BIND (stand-in spelling), CONFLICT? (bound elsewhere: read the note).
import csv, json, os, sys, collections
W = os.path.dirname(os.path.abspath(__file__)); os.chdir(W)
ROOT = '/home/user/FableDecomp'
from targets import FINAL
base_t, new_t, rf = sys.argv[1:4]
edited = set(l.strip() for l in open(rf) if l.strip())
def load(t):
    return {r['a']: set(map(tuple, r['pairs'])) for r in map(json.loads, open(os.path.join(W, 'runs', t + '.pairs.jsonl')))}
B = load(base_t); N = load(new_t)
BND = {}
for r in csv.reader(open(ROOT + '/rebuild/oracles/reloc_bindings.tsv'), delimiter='\t'):
    if len(r) >= 3 and r[0] != 'symbol': BND.setdefault(r[0], (r[1], r[2]))
S = set()
for p in [ROOT + '/ghidra_out/egor_pdb_names.tsv', ROOT + '/ghidra_out/fablewin_pdb_names.tsv']:
    for l in open(p, errors='replace'):
        q = l.rstrip('\n').split('\t')
        if len(q) > 1: S.add(q[1])
other = collections.Counter()
for l in open(os.path.join(W, 'full.jsonl')):
    r = json.loads(l)
    if r['a'] in edited: continue
    for s in set(r.get('u', [])): other[s] += 1
still = collections.Counter(s for a in N for s, t in N[a])
pairs = collections.defaultdict(lambda: collections.defaultdict(set))
for a in sorted(set(B) & set(N) & edited):
    rem = B[a] - N[a]; add = N[a] - B[a]
    for s, t in rem:
        for n in ([s2 for s2, t2 in add if t2 == t] or ['(none)']): pairs[(s, t)][n].add(a)
auto = ['old_symbol\ttarget\tnew_symbol\tnote']
for (old, t), d in sorted(pairs.items(), key=lambda x: (x[0][1], x[0][0])):
    for new, rs in sorted(d.items()):
        o = other[old]; st = still[old]
        if '@?A0x' in old: v = 'RETARGET: anonymous-namespace hash follows the file text; rebind the row-local name'
        elif o or st: v = 'KEEP: still emitted by %d unedited row(s) in full.jsonl%s' % (o, ' and %d edited row(s)' % st if st else '')
        else: v = 'RETIRE-candidate: all %d rechecked emitting row(s) respelled; no unedited emitter in full.jsonl' % len(rs)
        fin = FINAL.get(t)
        nv = 'new = final spelling' if new == fin else ('new is intermediate (final: %s)' % fin if fin else 'new (no final recorded)')
        auto.append('\t'.join([old, t, new, '%s; %s; rows %d: %s' % (v, nv, len(rs), ','.join(sorted(rs)))]))
open('BINDINGS.auto.tsv', 'w').write('\n'.join(auto) + '\n')
MAN = {   # wave-8 scope-1 RELOC_CONFLICT settlements (pass-9 side right): symbol -> (action, target, by_row, note)
    '?GetData@CBankFileAsyncData@@QAEPAXXZ': ('RETARGET', '009d5530', '009c8700',
        'conflict settled: was bound to 0x009D5580 by 00be04c0. Ego_r order IsFinished/GetData/PeekData/CanRelocateData = retail 0x9D54E0/0x9D5530/0x9D5580/0x9D55D0; '
        'retail callers of 0x9D5530: 009c8700 009c8fb0 00a25d20 00a42ea0 00bdc510 00bf3b60 009fd9b0'),
    '?PeekData@CBankFileAsyncData@@QBEPBXXZ': ('RETARGET', '009d5580', '00be04c0',
        'conflict settled: was bound to 0x009D5530 by 009c8700; the only retail caller of 0x9D5580 is 00be04c0 (now emits PeekData)'),
    '??0?$CIntelligentPointer@$$CBVCThing@@@@QAE@PBVCThing@@@Z': ('RETARGET', '0056c860', '0064a2db',
        'conflict settled: was bound to 0x004A96F0 by 0072cba0. 0x0056C860 stores vtable 0x01237F1C (RTTI CIntelligentPointer<const CThing>); '
        '0x004A96F0 stores 0x01238C6C (CIntelligentPointer<CThing>); 0072cba0 and 0071c9f0 now emit ??0?$CIntelligentPointer@VCThing@@@@QAE@PAVCThing@@@Z'),
    '?resize@?$vector@_NV?$allocator@_N@std@@@std@@QAEXIAB_N@Z': ('RETARGET', '00467686', '00cdd0a0',
        'conflict settled: was bound to 0x006860F0 by 006821e0. 0x006860F0 is STLport vector::_M_fill_assign (landed defining row 006860f0); '
        'resize is 0x00467686 (pass-9 row 00cdd0a0); 006821e0 now calls _M_fill_assign through the inline assign'),
    '?_M_fill_assign@?$vector@_NV?$allocator@_N@std@@@std@@QAEXIAB_N@Z': ('ADD', '006860f0', '006821e0',
        'scope 1: the spelling the landed defining row 006860f0 defines (structural: the PDBs carry Dinkumware vector<bool>, retail the generic STLport vector<T>)'),
}
out = [('action', 'symbol', 'target', 'by_row', 'note')]; seen = set()
for sym, (act, tgt, by, note) in MAN.items():
    out.append((act, sym, tgt, by, note)); seen.add((sym, tgt))
for l in auto[1:]:
    old, t, new, note = l.split('\t')
    rows = note.rsplit('rows ', 1)[-1].split(': ', 1)[-1]; first = rows.split(',')[0]
    b = BND.get(old)
    if (old, t) not in seen:
        if note.startswith('RETIRE'): act = 'RETIRE' if b and b[0] == t else 'RETIRE (not bound to this target)'
        elif note.startswith('RETARGET'): act = 'RETARGET-HASH'
        else: act = 'KEEP'
        bn = ('bound to %s by %s' % b) if b else 'not bound'
        out.append((act, old, t, b[1] if b else '-', '%s; was emitted by %s; %s' % (bn, rows, note.split(';')[0])))
        seen.add((old, t))
    if (new, t) in seen or new == '(none)': continue
    seen.add((new, t))
    nb = BND.get(new)
    if nb and nb[0] == t: act, n2 = 'KEEP', 'new spelling already bound by %s' % nb[1]
    elif nb: act, n2 = 'RETARGET', 'new spelling bound to %s by %s; the wave-8 recheck resolves it to %s' % (nb[0], nb[1], t)
    elif '?A0x' in new: act, n2 = 'ADD-HASH', 'anonymous-namespace hash of the edited file (follows the text)'
    elif new in S: act, n2 = 'ADD', 'PDB spelling (Ego_r/FableWin), not yet bound'
    elif new == FINAL.get(t): act, n2 = 'ADD', 'settled final (DECISIONS_W8.tsv), not yet bound'
    else: act, n2 = 'NO-BIND', 'intermediate: stand-in spelling (not a PDB / settled name)'
    out.append((act, new, t, first, '%s; emitted by %s' % (n2, rows)))
open('BINDINGS.tsv', 'w').write('\n'.join('\t'.join(r) for r in out) + '\n')
print(collections.Counter(r[0] for r in out[1:]))
for r in out[1:]:
    if r[0] in ('RETARGET', 'NO-BIND', 'RETIRE (not bound to this target)'): print(r[0], r[1][:100], r[2], r[4][:160])
