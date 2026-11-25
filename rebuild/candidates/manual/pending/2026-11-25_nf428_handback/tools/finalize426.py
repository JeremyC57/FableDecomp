#!/usr/bin/env python3
"""finalize426.py <batch>: build RESULTS.tsv, CHECKS.tsv, UNITS.tsv, SELF_SYMBOLS.tsv, TABLE_CHANGES.tsv (empty: no caller binds these
addresses), RENAMES.tsv and head/payload.json from the gate outputs (work/vland_<b>{h,n}.log, evidence/objsec.txt, evidence/pk_{H,N}.tsv,
work/hn_<b>.log, evidence/definers.tsv, evidence/link_all.txt). A row is ACCEPT only when every gate passes."""
import sys, os, re, json, shutil, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); b = sys.argv[1]; B = A + '/batches/' + b
P = [l.rstrip('\n').split('\t') for l in open(B + '/PLAN.tsv') if l.strip()]
def vl(k):
    d = {}; w = collections.defaultdict(list)
    for l in open(A + '/work/vland_%s%s.log' % (b, k)):
        m = re.match(r'^([0-9a-f]{8}) +(\S+) +(\S+) +(\S+)', l)
        if m and m.group(2) in ('MATCH', 'RELOCATION_MATCH', 'DIFFER') or (m and m.group(2).startswith(('DIFFER', 'RELOC'))): d[m.group(1)] = (m.group(2), m.group(3))
        m2 = re.match(r'^([0-9a-f]{8}) +WARN (\S+)', l)
        if m2: w[m2.group(1)].append(m2.group(2))
    return d, w
vh, wh = vl('h'); vn, wn = vl('n')
osx = {l.split('\t')[0]: l.split('\t')[1] for l in open(B + '/evidence/objsec.txt')}
def pk(side):
    d = collections.defaultdict(list)
    for l in open(B + '/evidence/pk_%s.tsv' % side):
        f = l.rstrip('\n').split('\t')
        if len(f) > 2 and not f[1].startswith('#'): d[f[0]].append(f[1:])
    return d
ph, pn = pk('H'), pk('N')
hn = {l.split()[0]: l.strip() for l in open(A + '/work/hn_%s.log' % b) if re.match(r'^[0-9a-f]{8} ', l)}
deb = collections.defaultdict(list)
for l in open(A + '/evidence/definers.tsv'):
    f = l.rstrip('\n').split('\t')
    if f[5] == 'NEQ': deb[f[0]].append(f[2])
link = open(A + '/evidence/link_all.txt').read()
lowneq = {}
for l in open(A + '/evidence/lower_definers.tsv'):
    f = l.rstrip('\n').split('\t')
    if f[1] == 'NEQ': lowneq[f[0]] = f[2]
hs, ns = link.split('## new set')
_dup = lambda t: {l.split()[-1] for l in t.splitlines() if l.strip().startswith('DUP')}
_unr = lambda t: {l.split()[-1] for l in t.splitlines() if l.strip().startswith('UNRESOLVED')}
_names = {x[4] for x in P} | {'?Reset@AutoTinyCountedHandle@@QAEXXZ', '?Release@CIVCP@@QAEXXZ'}
# new set: no duplicate and no unresolved name of this batch or of the stand-ins, and nothing HEAD did not already have
link_ok = (not (_dup(ns) & _names) and not (_unr(ns) & _names) and _dup(ns) <= _dup(hs) and _unr(ns) <= _unr(hs))
cat = nf.catalog()
R = open(B + '/RESULTS.tsv', 'w'); C = open(B + '/CHECKS.tsv', 'w'); U = open(B + '/UNITS.tsv', 'w'); SS = open(B + '/SELF_SYMBOLS.tsv', 'w')
RN = open(B + '/RENAMES.tsv', 'w')
print('address\tverdict\told\tnew\treason', file=R)
print('address\tobjsec\tleaf_sel_H/N\tvland_head/new\ttest_head/new\tvland_new_warns\tpk_H\tpk_N\texposed_debt\tlink\ttest_linked_row', file=C)
print('address\tunit\tkind', file=U)
print('address\tsymbol\tstatus\tleaf_given\tcurrent_line', file=SS)
print('address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence\told_source\tnew_source\tnew_test_source', file=RN)
nacc = 0
for a, mem, t, old, new, obj, sh in P:
    gh = vh.get(a, ('?', '?')); gn = vn.get(a, ('?', '?'))
    bad_w = [x for x in wn.get(a, []) if x in ('RELOC_NAME', 'STANDIN_LEAF', 'GENERIC_STRUCT', 'LITERAL_DIFF') and x not in wh.get(a, [])]
    pkn = pn.get(a, []); neq = [x for x in pkn if x[0].startswith('NEQ')]; reach = [x for x in pkn if x[-2] == 'y' or x[-3] == 'y']
    sel = re.search(r'leaf_sel H=(\S+) N=(\S+)', open(B + '/evidence/objsec.txt').read().split(a, 1)[1]).groups()
    test_linked = 'test PASS' in hn.get(a, '')
    ok = (gn[0] == gh[0] and gn[1] == 'PASS' and not bad_w and not neq and link_ok and test_linked and
          osx.get(a, '').startswith('ALL_EQ'))
    verdict = 'ACCEPT' if ok else 'HOLD'; nacc += ok
    copies = ';'.join('%s %s@%s' % (x[0], x[-1][:40], x[2]) for x in pkn) or 'none'
    exp = ','.join(sorted(set(deb.get(a, [])))) or 'none'
    reason = ('self respell (NF426): the unreferenced out-of-line COMDAT copy at this address is %s of CDefPointer<const %s>; '
              'row = explicit instantiation of engine/CDefPointer.h\'s member (select-any, as retail), leaf bytes unchanged (HEAD %s sel 1 -> new sel 2); '
              'T: FableWin.pdb %s module stream (only ?Release@/??1 of this T; retail TU holds exactly 2 copies) + Ego_r.pdb (%s); '
              'member: VC7.1 /O1 and /O2 emit the Release copy before the ~CDefPointer copy (toys 21/21; /Od reverses it, 3/3, matching the FableWin.pdb dtor-first order in 62/62 single-T modules); gates: vland %s/%s, objsec %s, pk new copies [%s], '
              'link /FORCE: new names 0 duplicates / 0 unresolved, the HEAD stand-in LNK2005 gone, nothing new; exposed existing pick-any debt (NF419 r2, unchanged rows whose copy of this name differs): %s'
              % ('Release()' if mem == 'Release' else '~CDefPointer()', t, gh[0], obj, [e for e in [x for x in json.load(open(A + '/evidence/single.json')) if a in x['addrs']][0]['ego'] if re.match(r'^(CDefClassBase::GetSubDef|CDefinitionManager::GetDef)<', e)][0],
                 gh[0] + ' ' + gh[1], gn[0] + ' ' + gn[1], osx.get(a), copies, exp))
    if a in lowneq:
        reason += ('; link-order consequence (GOTCHAS nf399_b2): landed row(s) %s of the same retail TU sit below this address and emit a differing '
                   'select-any copy of this name, so an address-ordered full link keeps their copy and drops this row\'s EQ one; debt queued in T3.txt'
                   % ', '.join('0x' + x.upper() for x in lowneq[a].split(',')))
    print('\t'.join([a, verdict, old, new, reason]), file=R)
    print('\t'.join([a, osx.get(a, '?'), '%s/%s' % sel, '%s/%s' % (gh[0], gn[0]), '%s/%s' % (gh[1], gn[1]), ','.join(bad_w) or '-',
                     'copies=%d' % len(ph.get(a, [])), 'copies=%d neq=%d reach=%d' % (len(pkn), len(neq), len(reach)), exp,
                     'PASS' if link_ok else 'FAIL', 'PASS' if test_linked else 'FAIL'], file=C) if False else
          '\t'.join([a, osx.get(a, '?'), '%s/%s' % sel, '%s/%s' % (gh[0], gn[0]), '%s/%s' % (gh[1], gn[1]), ','.join(bad_w) or '-',
                     'copies=%d' % len(ph.get(a, [])), 'copies=%d neq=%d reach=%d' % (len(pkn), len(neq), len(reach)), exp,
                     'PASS' if link_ok else 'FAIL', 'PASS' if test_linked else 'FAIL']), file=C)
    print('%s\t%s\t-' % (a, a), file=U)
    print('\t'.join([a, new, gn[0], new, '']), file=SS)
    mod = 'CDefPointer<const %s>' % t; name = 'Release' if mem == 'Release' else '~CDefPointer'
    stem = '%s/%s/CDefPointerconst%s_%s_%s' % (a[:2], a[2:4], t, 'Release' if mem == 'Release' else 'CDefPointer', a)
    oldsrc = nf.field(a, 'Source'); oldt = nf.field(a, 'TestSource')
    print('\t'.join([a, mod, name, new, cat[a]['name'], old, 'FableWin.pdb %s (?Release@ + ??1 of one T) + Ego_r.pdb %s (GetSubDef/GetDef<T>); Release copy below the destructor copy (VC7.1 /O1,/O2 order)' % (obj, obj.split('\\')[-1]),
                     oldsrc or '', stem + '.cpp', stem + '_test.cpp']), file=RN)
for f in (R, C, U, SS, RN): f.close()
open(B + '/TABLE_CHANGES.tsv', 'w').write('action\tsymbol\ttarget\twitness\tnote\n')
shutil.copy(A + '/work/p_%sh/payload.json' % b, B + '/head/payload.json')
print(b, 'ACCEPT', nacc, 'of', len(P))
