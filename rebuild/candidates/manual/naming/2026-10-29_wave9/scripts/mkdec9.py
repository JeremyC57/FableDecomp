# DECISIONS_W9.tsv: one row per address of the wave-9 inputs (queue 566-575 + misfiled chunk 520, wave-8 open rows, wave-8 renames)
# old emitters = analysis.json 'wrong' (baseline index), new = idx/new.jsonl (ov_final) for edited rows.
# mkdec9.py  -> DECISIONS_W9.tsv, prints status counts
import sys, os, json, csv, collections, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import inputs, targets9
W = inputs.W
A = json.load(open(W + '/analysis.json'))
Q = inputs.queue(); D8 = inputs.w8()
edited = set(l.strip() for l in open(W + '/runs/final.rows.txt') if l.strip())
NEW = {}
for l in open(W + '/idx/new.jsonl'):
    r = json.loads(l)
    if 'err' not in r: NEW[r['a']] = set(r['u'])   # references only: a spelling the row now defines itself (inline COMDAT) binds nothing
DEF = {}
for l in open(W + '/idx/new.jsonl'):
    r = json.loads(l)
    if 'err' not in r: DEF[r['a']] = set(r['d'])
BT, BS = inputs.bindings()
KIND = {}
for l in open(W + '/classes.tsv'):
    q = l.rstrip('\n').split('\t'); KIND[(q[0], q[4])] = q[2]
FAMROWS = collections.defaultdict(set)
for fam, lst in (('acc9', 'runs/acc9.final.tasks.tsv'), ('cita9', 'runs/cita9.rows.txt'), ('xfl9', 'runs/xfl9.rows.txt'),
                 ('gror9', 'runs/gror9.rows.txt'), ('aia9', 'runs/aia9f.rows.txt'), ('rtc9', 'runs/rtc9f.rows.txt'),
                 ('gws9', 'runs/gws9.rows.txt'), ('nga9', 'runs/nga9.rows.txt'), ('h9_opeq', 'runs/opeq.rows.txt'), ('h9_ren', 'runs/ren.rows.txt')):
    for l in open(W + '/' + lst):
        x = l.strip().split('\t')[0]
        if re.match(r'^[0-9a-f]{8}$', x): FAMROWS[x].add(fam)
for x in ('0097db70', '004c9840', '00839080'): FAMROWS[x].add('h9_self')
# exact deferral reasons (DEFERRED.tsv has the plans)
ROWWHY = {
    '00404720': 'row grades FAIL at baseline (behaviour test), an edit cannot be rechecked to PASS',
    '00a44a40': 'scalar deleting dtor row: the access respelling turns it DIFFER (the dtor call changes); hand',
    '004c9840': 'defining row: with the enum parameter its inline find(const long&) copies the enum into a temporary (DIFFER 216 v 213)',
    '0080bab0': 'compile error with the respelled class (SRC_FAIL); hand', '0080bd30': 'compile error with the respelled class (SRC_FAIL); hand',
}
MEGA = [(r'GetPItem', 'mega family GetPItem / CIntelligentPointer<T> wrapper (DEFERRED: plan P1)'),
        (r'^\?\?1CBaseIntelligentPointer@@', 'mega family ~CIntelligentPointer<CThing> / ~CBaseIntelligentPointer stand-ins (DEFERRED: plan P2)'),
        (r'SetPItem', 'mega family SetPItem(CThing*) stand-ins (DEFERRED: plan P3)'),
        (r'LowerBound', 'mega family CInterfaceMap::LowerBound 0x40F020 (DEFERRED: plan P4)'),
        (r'^\?\?1CCharString@@QAE@XZ', 'mega family CCharString dtor stand-ins (DEFERRED: plan P5)')]
KWHY = {'virtual': 'virtual-ness differs: a layout (vtable) change in the row, not a respelling', 'access+virtual': 'virtual-ness differs (layout change)',
        'key': 'class-key change (struct/class) of a type the row does not define', 'access+key': 'class key + access change: hand',
        'access': 'access/const respelling not reached (class not defined in the row, or the edit failed its check)',
        'const': 'const respelling not reached (class not defined in the row, or the edit failed its check)',
        'undemangled': 'spelling llvm-undname cannot demangle: hand', 'other': 'name / scope / signature change: a hand edit of the row'}
def conf(a, v):
    if a in targets9.FINAL: return 'proven', 'Ego_r.pdb+FableWin.pdb'
    if a in D8: return D8[a]['confidence'], D8[a]['sources']
    p = v.get('pdb', '-') if v else '-'
    return ({'Ego_r.pdb+FableWin.pdb': 'proven', 'FableWin.pdb': 'proven-1pdb', 'Ego_r.pdb': 'proven-1pdb'}.get(p, 'probable'), p)
RENAMES = {'00438570': '?PeekGuiDef@CPlayerGui@@SIABVCPlayerGuiDef@@XZ', '00405f00': '?GFIntToWideString@@YI?AVCWideString@@J@Z',
           '0051ed80': '?GetPThingWithUID@CThingManager@@QAEPAVCThing@@_K@Z', '0049d810': '?GetWorldSeed@CWorld@@QBEPAKXZ',
           '008fcf80': '?PeekPlayerManager@CAIStateGroupBase@@IBEABVCPlayerManager@@XZ'}
addrs = sorted(set(A) | set(RENAMES))
out = [('address', 'final_symbol', 'confidence', 'sources', 'chunks', 'identity', 'w9', 'wrong_spellings', 'bound_wrong_symbols', 'evidence')]
cnt = collections.Counter()
for a in addrs:
    v = A.get(a, {})
    fin = targets9.FINAL.get(a) or v.get('final') or RENAMES[a]
    c, srcs = conf(a, v)
    chunks = ','.join(sorted(set(x[0] for x in Q.get(a, [])))) or (D8[a]['chunks'] if a in D8 else 'w8')
    ident = D8[a]['identity'] if a in D8 else ('change' if v.get('wrong') else 'same')
    wrong = v.get('wrong', {})
    fixed = collections.defaultdict(list); left = collections.defaultdict(list)
    for s, rows in wrong.items():
        for r in rows:
            if r in edited and r in NEW and s not in NEW[r]: fixed[s].append(r)
            else: left[s].append(r)
    nf = sum(map(len, fixed.values())); nl = sum(map(len, left.values()))
    fams = sorted(set(f for rs in fixed.values() for r in rs for f in FAMROWS.get(r, ())))
    emit_new = sorted(r for r in edited if fin in NEW.get(r, ()) or fin in DEF.get(r, ()))
    why = collections.Counter()
    for s, rs in left.items():
        for r in rs:
            if r in ROWWHY: why[ROWWHY[r]] += 1; continue
            m = [t for p, t in MEGA if re.search(p, fin)]
            if m: why[m[0]] += 1; continue
            if a in targets9.CONF and a not in targets9.FINAL: why['decided, not applied: ' + targets9.CONF[a][:90]] += 1; continue
            if c == 'probable': why['probable: no second source found, stays open'] += 1; continue
            if c.startswith('pdb-spelling'): why['identity check: second source not found, stays open'] += 1; continue
            why[KWHY.get(KIND.get((a, s), 'other'), KWHY['other'])] += 1
    proven = c.startswith('proven')
    if a in RENAMES and a not in A:
        st = 'applied: h9_ren (defining row respelled; RENAMES.tsv APPLIED)'
    elif a in targets9.CONF and nf == 0 and a not in targets9.FINAL:
        st = 'decided, not applied: ' + targets9.CONF[a]
    elif not wrong or nf + nl == 0:
        st = 'no change needed (no indexed row emits a wrong spelling)'
    elif not proven:
        st = 'open: %s; %d emitter(s) left: %s' % ('probable' if c == 'probable' else 'identity check', nl,
              '; '.join('%d %s' % (n, t) for t, n in why.most_common()) or '-')
        if nf: st += ' | %d emitter(s) respelled by %s to the PDB-exact form of the name the row already called (access/const, or a hoisted CCharString); the identity question is unchanged' % (nf, '+'.join(fams) or '-')
    elif nl == 0:
        st = 'applied: %d wrong-spelling emitter(s) respelled (%s)' % (nf, '+'.join(fams) or '-')
    elif nf:
        st = 'partly: %d of %d respelled (%s); left: %s' % (nf, nf + nl, '+'.join(fams) or '-', '; '.join('%d %s' % (n, t) for t, n in why.most_common()))
    else:
        st = 'open: %d emitter(s) left: %s' % (nl, '; '.join('%d %s' % (n, t) for t, n in why.most_common()))
    if a in RENAMES and a in A: st += ' | defining row respelled by h9_ren (RENAMES.tsv APPLIED)'
    cnt[st.split(':')[0]] += 1
    bw = '|'.join(sorted(s for s in BT.get(a, ()) if s != fin))
    ev = (targets9.CONF.get(a) or (Q[a][0][2] if a in Q else (D8[a]['evidence'] if a in D8 else '')))[:400].replace('\t', ' ')
    ev = (ev + ' | wave-9: final emitted by %d edited row(s) after the change' % len(emit_new)) if emit_new else ev
    ws = ' ; '.join(sorted(left)) if left else ''
    out.append((a, fin, c, srcs, chunks, ident, st, ws, bw, ev))
open(W + '/DECISIONS_W9.tsv', 'w').write(''.join('\t'.join(x) + '\n' for x in out))
print(len(out) - 1, cnt)
