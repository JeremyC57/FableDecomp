# classify.py (NF342): classify every T3_PORT_QUEUE line (self / binder / table / other) and run the cheap pre-filter.
#   python3 -I classify.py  -> work/lines.tsv (one row per queue line) + counts on stdout
# Columns: line sha source address unit kind P P_from standins binders prefilter why
# kind:
#   self   = the line's fix is the col-2 row's own spelling moving to P (a PDB name)
#   binder = col 2 is a table target; the fix is the named binders binding P instead of a stand-in
#   table  = a named table line is to go / be respelled
#   other  = obstacle is not a spelling (debt, scaffolding, held by ruling, drectve, clusters, identity research, rejected ...)
# prefilter (candidates only; the compile is the proof):
#   self:   CAND if the row's Leaf == P or the tracker's self_leaf == P; else OPEN
#   binder: CAND if no table line at the target with a non-P spelling is witnessed by a named binder, and no stand-in the line names is
#           still a table spelling at the target; else OPEN
#   table:  CAND if the named table symbol is gone from the target
import sys, re, collections
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF342/tools')
import common as C

PDB = C.pdb(); TAB = C.table(); ENT = C.landed(); OPEN = C.tracker()
import os
SSP = C.R + '/rebuild/oracles/self_symbols.tsv'; SS_MTIME = os.path.getmtime(SSP); SS = {}
for _l in open(SSP, encoding='utf-8', errors='replace').read().splitlines()[1:]:
    _p = _l.split('\t')
    if len(_p) > 2: SS[C.norm(_p[0])] = (_p[1], _p[2])

OTHER_UNITS = ('held', 'rejected', 'debt', 'drectve unit', 'shared-name cluster', 'pick-any', 'local-model debt', 'class-model defect',
               'defines an owner Alloc', 'same-TU strong copy', 'FAMILY UNIT', 'identity research', 'part of the NF284')
OTHER_SRC = {'flatten2', 'nf328', 'nf331', 'pickany_d2inlS15', 'acctB_b24B6', 'acctB_b24C8', 'nf336', 'nf337', 'nf335', 'nf306', 'nf296'}


def pdb_syms(text):
    out = []
    for s in C.MANG.findall(text):
        s2 = s.rstrip('.')
        if s2 in PDB and s2 not in out: out.append(s2)
    return out


def standin_syms(text):
    return [s for s in dict.fromkeys(C.MANG.findall(text)) if s not in PDB]


def addrs(text, a):
    return [x for x in dict.fromkeys(C.norm(h) for h in C.HEX.findall(text)) if x != a]


def classify(src, a, unit, reason):
    txt = unit + ' ' + reason
    P, Pfrom, kind = '', '', ''
    # explicit self forms
    m = re.match(r'([0-9a-f]{8}) (skipped-port|skipped-held|rejected) (\?\S+)', reason)
    m2 = re.search(r'measured (\S*) -> (\S+)', reason)
    if src in OTHER_SRC or any(unit.startswith(u) for u in OTHER_UNITS):
        kind = 'other'
        if unit == 'held' and (re.match(r'[0-9a-f]{8} (skipped-held|NF323 Q|done by)', reason) or re.match(r'\?\S+:', reason)):
            kind = 'self'      # a held self: done still means the self defines and grades P (NF339 D1)
        if unit == 'held' and re.match(r'[0-9a-f]{8} table binder of 0x([0-9A-Fa-f]{8}) still references (\?\S+);', reason):
            kind = 'binder2'   # col 2 is the binder; target and old spelling in the text
    if not kind:
        if m: kind = 'self'
        elif src in ('alias1', 'access1', 'access2', 'access3', 'access4', 'flatten4', 'rename1', 'flatten-switch1', 'flatten-local2', 'flatten1') and m2: kind = 'self'
        elif src == 'binders1': kind = 'binder'
        elif src in ('nf317', 'nf318') and re.match(r'\?\S+:', reason): kind = 'self'
        elif src == 'tracker' and 'SELF PORT' in unit: kind = 'self'
        elif src == 'nf330' and re.search(r'PDB \?', reason): kind = 'self'
        elif src == 'nf333' and unit == 'table': kind = 'table'
        elif a in TAB and re.search(r'\bbinder', txt, re.I): kind = 'binder'
        elif re.search(r'\bself\b', txt) and a in ENT: kind = 'self?'
        elif a in TAB and [h for h in addrs(txt, a) if h in ENT]: kind = 'binder'   # names other landed rows at a table target: binders
        else: kind = 'other'
    # lines whose obstacle is the binders (the self already defined P when the line was written): binder kind, binders unnamed
    # nf326 "outside the CScriptThing member unit" / "binders bind": the hand-back STATUS says the open spelling was the self's or a binder's:
    # done = the self defines + grades P AND every table spelling at the target is P (kind unit)
    if kind == 'self' and src == 'nf326' and re.search(r'binders bind |outside the CScriptThing member uni', reason + ' ' + ST326.get(a, '')):
        kind = 'unit'
    # same-TU strong copies in other rows: scaffolding / debt, not a spelling
    if kind == 'self?' and re.search(r'same-TU', txt): kind = 'other'
    # P
    if m and m.group(3) in PDB: P, Pfrom = m.group(3), 'text'
    elif m2 and m2.group(2) in PDB: P, Pfrom = m2.group(2), 'text'
    elif src in ('nf317', 'nf318') and re.match(r'(\?\S+):', reason) and re.match(r'(\?\S+):', reason).group(1) in PDB:
        P, Pfrom = re.match(r'(\?\S+):', reason).group(1), 'text'
    else:
        ps = pdb_syms(txt)
        o = OPEN.get(a, {})
        if o.get('pdb_spelling') and o['pdb_spelling'] in PDB and (not ps or o['pdb_spelling'] in ps):
            P, Pfrom = o['pdb_spelling'], 'tracker'
        elif len(ps) == 1: P, Pfrom = ps[0], 'text'
        elif o.get('rtti') and o['rtti'] in PDB: P, Pfrom = o['rtti'], 'tracker rtti'
        elif a not in OPEN:
            sp = {s for s, w in TAB.get(a, [])}
            lf = C.leaf(a)
            if lf: sp.add(lf)
            if len(sp) == 1 and next(iter(sp)) in PDB: P, Pfrom = next(iter(sp)), 'agreed'
    if not P and kind.startswith('self'):
        # the line names the OLD self spelling: the PDB name with the same ?name@scope@@ (access / const / return letters differ)
        for sym in C.MANG.findall(txt):
            mm = re.match(r'(\?[^@?]+@[^?]*?@@)', sym)
            if not mm: continue
            c = PREF.get(mm.group(1), [])
            if len(c) == 1: P, Pfrom = c[0], 'pdb same name@scope'; break
    return kind, P, Pfrom


# NF303's census evidence (target -> [(binder, stand-in spelling, kind)]): the binders an nf303 "binder -" line stands for
EV303 = collections.defaultdict(list)
for _l in open(C.R + '/rebuild/candidates/manual/pending/2026-11-23_nf303_handback/evidence/census.tsv', encoding='utf-8', errors='replace'):
    _p = _l.rstrip('\n').split('\t')
    if len(_p) < 6: continue
    for _it in _p[5].split(' '):
        _m = re.match(r'([A-Z_]+):([0-9a-f]{8}):(?:SOT3:|SO:|T3:)?(.*)', _it)
        if _m: EV303[_p[1]].append((_m.group(2), _m.group(3), _m.group(1)))


def ev303_prefilter(a, P):
    e = EV303.get(a, [])
    tl = TAB.get(a, [])
    def present(sym): return [w for t, w in tl if t == sym or (len(sym) > 60 and t.startswith(sym))]
    still = [(b, s) for b, s, k in e if present(s)]
    if not e: return None
    if not still: return 'CAND', 'nf303 census stand-ins all gone from the table at the target', [b for b, s, k in e], [s for b, s, k in e]
    if len(still) < len(e): return 'PARTIAL_CAND', 'nf303 census: %d of %d stand-ins gone' % (len(e) - len(still), len(e)), [b for b, s, k in e], [s for b, s, k in e]
    return 'OPEN', 'nf303 census stand-ins still table lines: ' + ' | '.join('%s[%s]' % (s[:70], b) for b, s in still[:3]), [b for b, s, k in e], [s for b, s, k in e]


# NF326's untruncated reasons (the queue copy is cut at ~220 characters)
ST326 = {}
for _l in open(C.R + '/rebuild/candidates/manual/pending/2026-11-24_nf326_handback/STATUS.tsv', encoding='utf-8', errors='replace'):
    _p = _l.rstrip('\n').split('\t')
    if len(_p) > 3: ST326[C.norm(_p[0])] = _p[3]


PREF = collections.defaultdict(list)
for _s in PDB:
    _m = re.match(r'(\?[^@?]+@[^?]*?@@)', _s)
    if _m: PREF[_m.group(1)].append(_s)


def prefilter(kind, a, P, standins, binders):
    if kind in ('other',): return 'OTHER', ''
    if not P: return 'NO_P', ''
    if kind in ('self', 'self?'):
        lf = C.leaf(a) if a in ENT else None
        if a not in ENT: return 'NO_ROW', ''
        tl = OPEN.get(a, {}).get('self_leaf', '')
        if lf == P: return 'CAND', 'leaf=P'
        if tl == P: return 'CAND', 'tracker self_leaf=P'
        ms = SS.get(a)
        if ms and ms[0] == P: return 'CAND', 'self_symbols=P'
        if lf and lf.startswith('?') and len(lf) > 12: return 'OPEN', 'full Leaf != P: %s' % lf[:120]
        sp = C.src_path(a)
        newer = os.path.getmtime(sp) > SS_MTIME if sp and os.path.exists(sp) else True
        if ms and ms[1] in ('MATCH', 'RELOCATION_MATCH') and ms[0] and not newer:
            return 'OPEN', 'measured (self_symbols, source unchanged since): %s' % ms[0][:120]
        return 'CAND_UNMEASURED', 'Leaf %s; self_symbols %s; source %s' % ((lf or '-')[:40], (ms[0][:60] + ' ' + ms[1]) if ms else '-', 'newer' if newer else 'older')
    if kind == 'unit':
        tl = TAB.get(a, []); nonp = [(s, w) for s, w in tl if s != P]
        if nonp: return 'OPEN', 'target still has non-P spellings: ' + ' | '.join('%s[%s]' % (s[:80], w) for s, w in nonp[:3])
        lf = C.leaf(a)
        if lf and lf != P and lf.startswith('?'): return 'OPEN', 'self Leaf != P: ' + lf[:100]
        return 'CAND', 'table agrees on P; self Leaf %s' % ('= P' if lf == P else (lf or '-')[:40])
    if kind == 'binder':
        tl = TAB.get(a, [])
        bad = [(s, w) for s, w in tl if s != P and w in binders]
        still = [s for s in standins if any(s == t for t, w in tl)]
        if bad: return 'OPEN', 'binder witnesses non-P: ' + ' | '.join('%s[%s]' % (s[:80], w) for s, w in bad[:3])
        if still: return 'OPEN', 'named stand-in still a table spelling: ' + still[0][:100]
        if not binders:
            nonp = [(s, w) for s, w in tl if s != P]
            if nonp: return 'OPEN', 'no named binder; target still has non-P spellings: ' + ' | '.join('%s[%s]' % (s[:80], w) for s, w in nonp[:3])
            return 'CAND', 'no named binder; target agrees on P'
        return 'CAND', 'named binders witness no non-P spelling'
    if kind == 'table':
        tl = TAB.get(a, [])
        still = [s for s in standins if any(s == t for t, w in tl)]
        return ('OPEN', 'table line still there: ' + still[0][:100]) if still else ('CAND', 'named table symbol gone')
    return 'OTHER', ''


# binders the line names only through a stand-in token; taken from the agent's own hand-back STATUS.tsv (column 3)
EXTRA_BINDERS = {('nf285', '00997f80'): ['00999230'],      # pending/2026-11-22_nf285_handback/STATUS.tsv
                 ('nf291', '004fb4a0'): ['006e11f0'],      # pending/2026-11-23_nf291_handback/STATUS.tsv
                 ('nf292', '00a99110'): ['00a99610']}      # pending/2026-11-23_nf292_handback/STATUS.tsv


def main():
    out = open(C.W + '/lines.tsv', 'w')
    out.write('line\tsha\tsource\taddress\tunit\tkind\tP\tP_from\tstandins\tbinders\tprefilter\twhy\ttracker\n')
    cnt = collections.Counter(); bysrc = collections.defaultdict(collections.Counter)
    for ln, sha, f in C.queue():
        src, a, unit, reason = f[0], C.norm(f[1]), f[2], (f[3] if len(f) > 3 else '')
        kind, P, Pfrom = classify(src, a, unit, reason)
        txt = unit + ' ' + reason
        st = [s for s in standin_syms(txt) if s != P]
        bs = [x for x in addrs(txt, a) if x in ENT]
        if kind == 'binder' and src == 'binders1': bs = [a]
        if (src, a) in EXTRA_BINDERS: bs = EXTRA_BINDERS[(src, a)]
        if kind == 'binder2':
            mm = re.match(r'[0-9a-f]{8} table binder of 0x([0-9A-Fa-f]{8}) still references (\?\S+);', reason)
            T_, old = C.norm(mm.group(1)), mm.group(2)
            P = OPEN.get(T_, {}).get('pdb_spelling', '') or (C.leaf(T_) if (C.leaf(T_) or '') in PDB else ''); Pfrom = 'target %s' % T_
            st = [old]; bs = [a]
            still = [w for s_, w in TAB.get(T_, []) if s_ == old]
            pf, why = (('OPEN', 'old spelling still a table line at %s (witness %s)' % (T_, ','.join(still))) if still else ('CAND', 'old spelling gone from target %s' % T_))
        elif src == 'nf303' and kind == 'binder' and EV303.get(a):
            pf, why, bs, st = ev303_prefilter(a, P)
            if not P: pf = 'NO_P'
        else:
            pf, why = prefilter(kind, a, P, st, bs)
        if (kind == 'unit' or (kind == 'binder' and not bs)) and pf in ('CAND', 'PARTIAL_CAND'):
            bs = sorted({w for s_, w in TAB.get(a, []) if s_ == P and w in ENT and w != a})   # the table's P witnesses (compiled as the binders)
        trk = OPEN.get(a, {}).get('status', 'done/absent')
        out.write('\t'.join(map(str, [ln, sha, src, a, unit[:60], kind, P, Pfrom, ' || '.join(st)[:600], ','.join(bs), pf, why, trk])) + '\n')
        cnt[(kind, pf)] += 1; bysrc[src][(kind, pf)] += 1
    out.close()
    for k, v in sorted(cnt.items()): print(k, v)
    print()
    for s, c in sorted(bysrc.items(), key=lambda kv: -sum(kv[1].values())):
        print(s, dict(c))


if __name__ == '__main__': main()
