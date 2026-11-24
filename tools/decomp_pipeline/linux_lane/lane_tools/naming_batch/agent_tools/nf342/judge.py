# judge.py (NF342): join work/lines.tsv (classify.py) with the HEAD census (evidence/census/<addr>.json, census.py read) and the manual
# verdicts (work/manual.tsv: line, verdict STALE|PARTIAL|OPEN|UNPROVEN, proof / what is left) -> hand-back files:
#   STALE_T3.tsv   lines whose fix a later landing already made (compiled proof)
#   PARTIAL.tsv    lines whose fix is half done (what is left)
#   work/verdicts.tsv  every line: verdict + reason (SUMMARY.md counts come from it)
# Verdicts: STALE / PARTIAL / OPEN_CHECKED (compiled, fix not there) / OPEN (pre-filter: obstacle still in the table / Leaf / measured symbol)
#           / UNPROVEN (candidate not compiled or not gradable) / OTHER (not decidable by script) / NO_P (no exact PDB target)
import os, sys, json, re, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import common as C

CEN = {}
for f in os.listdir(C.A + '/evidence/census'):
    if f.endswith('.json'): CEN[f[:8]] = json.load(open(C.A + '/evidence/census/' + f))
MAN = {}
if os.path.exists(C.W + '/manual.tsv'):
    for l in open(C.W + '/manual.tsv'):
        p = l.rstrip('\n').split('\t')
        if len(p) >= 3 and p[0].isdigit(): MAN[int(p[0])] = (p[1], p[2])
Q = {ln: (sha, f) for ln, sha, f in C.queue()}
OPEN = C.tracker()


def graded_ok(r):
    return r.get('obj') and r['grade'].startswith(('MATCH', 'RELOCATION_MATCH')) and r['behaviour'] == 'PASS'


def self_verdict(x, r):
    a, P = x['address'], x['P']
    if not r: return 'UNPROVEN', 'not compiled'
    if not r.get('obj'): return 'UNPROVEN', 'no object (%s %s)' % (r['grade'], r['behaviour'])
    g = r.get('graded', '')
    if P in r['DEF'] and g == P and graded_ok(r):
        left = [s for s in x['standins'] if s and (s in r['DEF'] or s in r['UND'])]
        proof = 'DEF %s; graded = P; %s %s' % (P, r['grade'], r['behaviour'])
        if left: return 'PARTIAL', proof + '; but still references the named stand-in(s) ' + ' | '.join(left)[:300]
        return 'STALE', proof
    if P in r['DEF'] and graded_ok(r): return 'PARTIAL', 'DEF %s but the grade compares %s (%s %s)' % (P, g, r['grade'], r['behaviour'])
    if P in r['DEF']: return 'UNPROVEN', 'DEF P but grade %s %s' % (r['grade'], r['behaviour'])
    return 'OPEN_CHECKED', 'HEAD object does not define P; graded %s (%s %s)' % (g[:150], r['grade'], r['behaviour'])


def binder_verdict(x, binders, target, P):
    fixed, notfixed, unk = [], [], []
    for b in binders:
        r = CEN.get(b)
        if not r or not r.get('obj'): unk.append((b, 'not compiled' if not r else 'no object')); continue
        at = sorted({s for t, s, k in r.get('bind', []) if t == target})
        if at == [P]:
            sel = r.get('DEF_SEL', {}).get(P) if P in r['DEF'] else None
            if P in r['DEF'] and sel == 2: fixed.append((b, 'binds only P at %s through its own pick-any (COMDAT ANY) copy of P (%s %s)' % (target, r['grade'], r['behaviour'])))
            elif P in r['DEF']: notfixed.append((b, 'binds P but DEFINES it (strong same-TU copy%s)' % ('' if sel is None else ', COMDAT selection %s' % sel)))
            else: fixed.append((b, 'binds only P at %s (UND, %s %s)' % (target, r['grade'], r['behaviour'])))
        elif at:
            notfixed.append((b, 'binds ' + ' | '.join(at)[:250] + ' at ' + target))
        else:
            old = [s for s in x['standins'] if s in r['UND'] or s in r['DEF']]
            if old: notfixed.append((b, 'graded function binds nothing at %s; object still references %s' % (target, old[0][:150])))
            elif P in r['UND']: fixed.append((b, 'graded function binds nothing at %s; UND has P, no named stand-in (NF339 test)' % target))
            else: unk.append((b, 'no binding at %s and no P / stand-in reference' % target))
    def fmt(L): return '; '.join('%s: %s' % kv for kv in L)
    # a named address whose graded function binds nothing at the target and references neither P nor a named stand-in is not a binder
    # of this target (the text named it for another reason, e.g. "as 0x00ac0e50"): ignored, but at least one real binder must be proven
    unk = [(b, w) for b, w in unk if not w.startswith('no binding at')]
    nonp = [(s, w) for s, w in C.table().get(target, []) if s != P]
    left_tab = ' | '.join('%s[witness %s]' % (s[:90], w) for s, w in nonp[:4])
    if fixed and not notfixed and not unk:
        if nonp: return 'PARTIAL', 'named binders fixed: ' + fmt(fixed) + ' || left: the table still has non-P spellings at %s: %s' % (target, left_tab)
        return 'STALE', fmt(fixed) + ' || table: every spelling at %s is P' % target
    if fixed and notfixed: return 'PARTIAL', 'fixed ' + fmt(fixed) + ' || left ' + fmt(notfixed) + ((' || unknown ' + fmt(unk)) if unk else '')
    if notfixed: return 'OPEN_CHECKED', fmt(notfixed) + ((' || unknown ' + fmt(unk)) if unk else '')
    if fixed and unk: return 'UNPROVEN', 'fixed ' + fmt(fixed) + ' || unknown ' + fmt(unk)
    if not fixed and not unk: return ('OPEN_CHECKED' if nonp else 'UNPROVEN'), 'no named address binds the target' + ((' ; table still has non-P spellings: ' + left_tab) if nonp else '')
    return 'UNPROVEN', fmt(unk) or 'no binder'


# corroboration of a P that did not come from the line's own text (P_from agreed / tracker): the line must point at it independently
EV303P = {}
for _l in open(C.R + '/rebuild/candidates/manual/pending/2026-11-23_nf303_handback/evidence/census.tsv', encoding='utf-8', errors='replace'):
    _p = _l.rstrip('\n').split('\t')
    if len(_p) > 4: EV303P[_p[1]] = _p[4]
NF339P = {}
for _l in open(C.R + '/rebuild/candidates/manual/pending/2026-11-24_nf339_handback/STALE_T3.tsv', encoding='utf-8', errors='replace'):
    _p = _l.rstrip('\n').split('\t')
    if len(_p) > 2: NF339P.setdefault(_p[0], set()).add(_p[2])


def member_key(sym):
    m = re.match(r'\?\?([0-9A-Z]|_[0-9A-Z])(\?\$)?([A-Za-z_]\w*)', sym)
    if m: return '??' + m.group(1) + m.group(3)            # ctor / dtor / operator of class X
    m = re.match(r'\?([A-Za-z_]\w*)@', sym)
    return m.group(1) if m else None


def corroborate(x, rows_by_addr):
    a, P = x['address'], x['P']
    ln = int(x['line']); f = Q[ln][1]; txt = ' '.join(f[2:])
    if EV303P.get(a) == P: return 'nf303 census evidence names P'
    if P in NF339P.get(a, ()): return 'NF339 STALE_T3 names P for this address'
    for y in rows_by_addr.get(a, []):
        if y['P_from'] == 'text' and y['P'] == P: return 'line %s gives P from its own text' % y['line']
    k = member_key(P)
    for sym in C.MANG.findall(txt):
        if sym != P and member_key(sym) == k: return 'the line quotes %s (same member as P)' % sym[:90]
    return ''


def main():
    L = [l.rstrip('\n').split('\t') for l in open(C.W + '/lines.tsv')]
    h = L[0]; rows = [dict(zip(h, r)) for r in L[1:]]
    out = []
    for x in rows:
        ln = int(x['line']); x['standins'] = [s for s in x['standins'].split(' || ') if s]
        bs = [b for b in x['binders'].split(',') if b]
        kind, pf = x['kind'], x['prefilter']
        if ln in MAN: v, why = MAN[ln]
        elif pf == 'OTHER': v, why = 'OTHER', 'not decidable by script (%s)' % x['unit'][:60]
        elif pf == 'NO_P': v, why = 'NO_P', 'no exact PDB target spelling'
        elif pf == 'OPEN' and kind.startswith('self') and x['address'] in CEN:   # compiled anyway (negative controls): use the object
            v, why = self_verdict(x, CEN.get(x['address'])); why = 'control: ' + why
        elif pf == 'OPEN': v, why = 'OPEN', x['why']
        elif kind.startswith('self'): v, why = self_verdict(x, CEN.get(x['address']))
        elif kind == 'binder':
            v, why = binder_verdict(x, bs, x['address'], x['P'])
            if v in ('STALE', 'PARTIAL'):
                # the target's own row must define P as well (the tracker counts the self Leaf as a spelling)
                a, P = x['address'], x['P']; r = CEN.get(a)
                if a in C.landed():
                    if r and r.get('obj') and P not in r.get('DEF', []):
                        v = 'PARTIAL'; why += ' || left: the target row itself still defines %s, not P' % r.get('graded', '?')[:150]
                    elif not r or not r.get('obj'):
                        lf = C.leaf(a) or ''
                        if lf != P: v = 'PARTIAL' if v == 'PARTIAL' else 'UNPROVEN'; why += ' || target row not compiled (Leaf %s)' % lf[:80]
                    else: why += ' || target row DEF P'
        elif kind == 'unit':
            # self proof + every table spelling at the target is P + the table's P witnesses bind P
            v1, w1 = self_verdict(x, CEN.get(x['address']))
            if bs: v2, w2 = binder_verdict(x, bs, x['address'], x['P'])
            else: v2, w2 = 'STALE', 'no table line at the target (no binder left)'
            if v1 == 'STALE' and v2 == 'STALE': v, why = 'STALE', 'self: ' + w1 + ' || binders: ' + w2
            elif 'UNPROVEN' in (v1, v2): v, why = 'UNPROVEN', 'self: %s %s || binders: %s %s' % (v1, w1, v2, w2)
            elif v1 == 'OPEN_CHECKED': v, why = 'OPEN_CHECKED', 'self (the open spelling per the agent STATUS): %s || binders: %s %s' % (w1, v2, w2)
            elif 'STALE' in (v1, v2) or 'PARTIAL' in (v1, v2): v, why = 'PARTIAL', 'self: %s %s || binders: %s %s' % (v1, w1, v2, w2)
            else: v, why = ('UNPROVEN' if 'UNPROVEN' in (v1, v2) else 'OPEN_CHECKED'), 'self: %s %s || binders: %s %s' % (v1, w1, v2, w2)
        elif kind == 'binder2':
            T_ = x['P_from'].split()[-1]; v, why = binder_verdict(x, [x['address']], T_, x['P'])
        else: v, why = 'UNPROVEN', 'table / other kind: needs a manual verdict'
        out.append((x, v, why))
    byaddr = collections.defaultdict(list)
    for x, v, why in out: byaddr[x['address']].append(x)
    for k, (x, v, why) in enumerate(out):
        if v == 'STALE' and x['P_from'] in ('agreed', 'tracker') and int(x['line']) not in MAN:
            c = corroborate(x, byaddr)
            if c: out[k] = (x, v, why + ' || P (%s) corroborated: %s' % (x['P_from'], c))
            else: out[k] = (x, 'UNPROVEN', why + ' || P came from the %s spelling and nothing in the line names it' % x['P_from'])
    with open(C.W + '/verdicts.tsv', 'w') as f:
        f.write('line\tsha\tsource\taddress\tkind\tprefilter\tverdict\twhy\n')
        for x, v, why in out: f.write('\t'.join([x['line'], x['sha'], x['source'], x['address'], x['kind'], x['prefilter'], v, why]) + '\n')
    hdr = 'line\tsha\tsource\taddress\tunit\treason\tkind\tpdb_symbol\tproof\tlanded_by_candidates\tremaining\n'
    with open(C.A + '/STALE_T3.tsv', 'w') as fs, open(C.A + '/PARTIAL.tsv', 'w') as fp:
        fs.write(hdr); fp.write(hdr.replace('proof', 'done_and_left'))
        for x, v, why in out:
            if v not in ('STALE', 'PARTIAL'): continue
            ln = int(x['line']); sha, f = Q[ln]
            assert sha == x['sha'], 'queue drifted at line %d' % ln
            a = x['address']; lb = ', '.join(C.later_batches(a, x['source'])) or '-'
            if x['kind'] == 'binder': lb2 = sorted({b for bb in x['binders'].split(',') if bb for b in C.later_batches(bb, x['source'])})
            else: lb2 = []
            if lb2: lb += ' | binders: ' + ', '.join(lb2)
            rem = OPEN.get(a, {}).get('status', 'done (not in OPEN.tsv)')
            line = '\t'.join([x['line'], sha, f[0], f[1], f[2], f[3] if len(f) > 3 else '', x['kind'], x['P'], why, lb, 'tracker: ' + rem]) + '\n'
            (fs if v == 'STALE' else fp).write(line)
    c = collections.Counter(v for x, v, why in out); print(c)
    cs = collections.Counter((x['kind'], v) for x, v, why in out); print(cs)


if __name__ == '__main__': main()
