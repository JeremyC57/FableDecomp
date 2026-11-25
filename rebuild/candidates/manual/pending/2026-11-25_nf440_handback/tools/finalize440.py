#!/usr/bin/env python3
"""finalize440.py <batch>: write the lander's files for batches/<batch> from its measured evidence:
RESULTS, CHECKS, SELF_SYMBOLS, RENAMES, CATALOG_CHANGES, BEST_GUESS, TABLE_CHANGES (header only), HEADERS (header only), UNITS, rows.txt.
Verdict: T1 (name bound here by a landed caller) ACCEPT when every gate passes; T2 / T3 best-guess names HOLD 'R-NF440-1 only' when every
other gate passes (descriptive_check fails PDB_NAME_ELSEWHERE on every FableWin.pdb name), else HOLD with the failing gate."""
import sys, os, re, json, subprocess, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
from lib440 import *
b = sys.argv[1]; B = W + '/batches/' + b
P = [l.rstrip('\n').split('\t') for l in open(B + '/PLAN.tsv') if l.strip()]
A = {f[0]: f for f in tsv(W + '/evidence/assign.tsv')}
cat, keys = catalog()
def lines(fn, a):
    p = B + '/evidence/' + fn
    return [l.rstrip('\n') for l in open(p)] if os.path.exists(p) and not a else [l.rstrip('\n') for l in open(p) if l.startswith(a)] if os.path.exists(p) else []
def vl(fn):
    d = {}
    p = B + '/evidence/' + fn
    if os.path.exists(p):
        for l in open(p):
            m = re.match(r'^([0-9a-f]{8})\s+(\S+)\s+(PASS|FAIL|\S+)\s', l)
            if m: d[m.group(1)] = m.group(2) + ' ' + m.group(3)
    return d
vh, vn = vl('vland_head.log'), vl('vland_new.log')
warn_n = sum(1 for l in open(B + '/evidence/vland_new.log') if 'WARN' in l) if os.path.exists(B + '/evidence/vland_new.log') else -1
warn_h = sum(1 for l in open(B + '/evidence/vland_head.log') if 'WARN' in l) if os.path.exists(B + '/evidence/vland_head.log') else -1
lit = sum(1 for l in open(B + '/evidence/vland_new.log') if 'LITERAL_DIFF' in l) if os.path.exists(B + '/evidence/vland_new.log') else -1
link = open(B + '/evidence/link.txt').read() if os.path.exists(B + '/evidence/link.txt') else ''
dups = re.findall(r'duplicates (\d+)', link); unr = re.findall(r'unresolved matching watch: (\d+)', link)
uh = set(re.findall(r'UNRESOLVED x\d+ (\S+)', link.split('## new set')[0])) if link else set()
un = set(re.findall(r'UNRESOLVED x\d+ (\S+)', link.split('## new set')[1])) if '## new set' in link else set()
link_ok = len(dups) == 2 and int(dups[1]) <= int(dups[0]) and not (un - uh)
link_s = 'dups %s->%s, unresolved(CCountedPointer) %s->%s, new %d' % (dups[0] if dups else '?', dups[1] if len(dups) > 1 else '?', unr[0] if unr else '?', unr[1] if len(unr) > 1 else '?', len(un - uh))
defs = collections.defaultdict(list)
for l in open(B + '/evidence/definers.tsv').read().splitlines()[1:]:
    n, d, v = l.split('\t'); defs[n].append(d + ':' + v)
bld = {}
for p in P:
    a = p[0]; bld[a] = {k: nf.field(a, k) or '' for k in ('Module', 'Source', 'TestSource')}
res = ['address\tverdict\told\tnew\treason']
chk = ['address\ttier\tmember\thn(HEAD/new grade, leaf bytes, test linked with row)\tobjsec\tvland_head/new\tpk_new(neq/reach)\tdefiners(other objects defining the name: body v row)\tlink\tname_gate']
ss = ['address\tsymbol\tstatus\tleaf_given\tcurrent_line']; ren = ['address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence\told_source\tnew_source\tnew_test_source']
cc = ['address\tfield\told\tnew\tnote']; bg = ['address\tsymbol\tbasis\tnote']; units = ['address\tunit\tkind']
C = collections.Counter(); hold_other = []
for p in P:
    a, mem, T, old, new = p[:5]; f = A.get(a) or [a, '', '', 'R1', '', mem, new, 'Reset: callers bind the PDB name']; tier = f[3]
    hn = [l for l in lines('hn.txt', a)]; hn = hn[0] if hn else ''
    obs = (lines('objsec.txt', a) or [''])[0].split('\t')
    pkn = [l.split('\t') for l in lines('pk_N.tsv', a) if '\t#' not in l]
    neq = sum(1 for x in pkn if len(x) > 1 and x[1].startswith('NEQ')); reach = sum(1 for x in pkn if len(x) > 8 and x[-2] == 'y' and x[1].startswith('NEQ'))
    ng = (lines('name_gate.txt', a) or [''])[0]
    gates = {
        'hn': 'H=RELOCATION_MATCH' in hn and 'N=RELOCATION_MATCH' in hn and 'leafbytes SAME' in hn and 'test PASS' in hn,
        'objsec': len(obs) > 2 and obs[2].startswith('ALL_EQ'),
        'vland': vh.get(a, '').startswith(('RELOCATION_MATCH', 'MATCH')) and vn.get(a, '') in ('RELOCATION_MATCH PASS', 'MATCH PASS') and vh.get(a, '').split()[0] == vn.get(a, '').split()[0],
        'pk': neq == 0, 'link': link_ok, 'name_gate': 'NAME_GATE PASS' in ng,
    }
    bad = [k for k, v in gates.items() if not v]
    flat = T.replace('const ', 'Const').replace('::', '')
    memn = {'D': '~CCountedPointer', 'RD': 'ReleaseData', 'SH': 'ShareData', 'Reset': 'Reset'}[mem]; ftok = {'D': 'CCountedPointer', 'RD': 'ReleaseData', 'SH': 'ShareData', 'Reset': 'Reset'}[mem]
    mod = 'CCountedPointer<%s>' % ((T[6:] + ' const') if T.startswith('const ') else T)   # the catalog's own const spelling
    oldsrc = bld[a]['Source']; d2 = oldsrc.split('/')[0] + '/' + oldsrc.split('/')[1]
    newsrc = '%s/CCountedPointer%s_%s_%s.cpp' % (d2, flat, ftok, a); newtst = newsrc[:-4] + '_test.cpp'
    oldcat = cat.get(a, ('', 0))[0]
    if tier == 'R1':
        why = ('self respell (NF440, from NF428 hold_r1): the CCountedPointer<%s>::Reset stand-in (self %s) takes the PDB name its retail callers '
               'already bind (one source; NF428 found no RTTI second source; owner best-guess rule 2026-11-25). Code exactly as NF428 measured it (comment only changed). Leaf bytes '
               'unchanged (RELOCATION_MATCH both sides). One source (basis one-source:caller-binding): no new claim, the table already binds this name here; registry line in evidence/BEST_GUESS_acc_side.tsv (R-NF440-1 (c)).') % (T, old)
        verdict = 'ACCEPT' if not bad else 'HOLD'
    elif tier == 'T1':
        wit = sorted(x for x in bindings()[0].get(new, ()) )
        why = ('self respell (NF440): the binding table already binds the PDB name %s at 0x%s (landed caller), and the self was the stand-in %s; '
               'leaf bytes unchanged (RELOCATION_MATCH both sides). One source (basis one-source:caller-binding): no new claim, the table already binds this name here; its registry line is in evidence/BEST_GUESS_acc_side.tsv (the gate rejects it, R-NF440-1 (c)).') % (new, a.upper(), old)
        verdict = 'ACCEPT' if not bad else 'HOLD'
    else:
        why = ('best-guess respell (NF440, owner rule 2026-11-25): %s. Basis %s. Leaf bytes unchanged. HELD for ruling R-NF440-1 only: '
               'descriptive_check rejects every FableWin.pdb name as PDB_NAME_ELSEWHERE, and naming_status would grade this guessed self agree_pdb.') % (
               f[7], 'one-source:' + ('tu-order' if tier == 'T2' else 't-predictor'))
        verdict = 'HOLD'
        bg.append('\t'.join([a, new, 'one-source:' + ('tu-order' if tier == 'T2' else 't-predictor'), (f[7])[:300]]))
    if bad:
        why = 'gate(s) failed: %s. ' % ','.join(bad) + why; hold_other.append(a)
    C[(tier, verdict, 'gates_ok' if not bad else 'gates_bad')] += 1
    res.append('\t'.join([a, verdict, old, new, why]))
    dd = []
    for n in (new, '?ReleaseData@?$CCountedPointer@%s@@QAEXXZ' % re.search(r'\?\$CCountedPointer@(.+?@@)@', new).group(1)) if mem == 'SH' else (new,):
        dd += defs.get(n, [])
    chk.append('\t'.join([a, tier, mem, hn.split(' ', 1)[1] if hn else '?', obs[2] if len(obs) > 2 else '?', '%s/%s' % (vh.get(a, '?'), vn.get(a, '?')),
                          'copies=%d neq=%d reach_neq=%d' % (len(pkn), neq, reach), ';'.join(dd) or '-', link_s, ng.split(' ', 1)[1] if ' ' in ng else ng]))
    ss.append('\t'.join([a, new, 'RELOCATION_MATCH', new, '']))
    if oldcat != '%s::%s' % (mod, memn):
        ren.append('\t'.join([a, mod, memn, new, oldcat, old, 'T1 binding' if tier == 'T1' else 'best guess (%s)' % tier, oldsrc, newsrc, newtst]))
    if bld[a]['Module'] != mod: cc.append('\t'.join([a, 'Module', bld[a]['Module'], mod, 'build_candidates.ps1 Module follows the new T']))
    units.append('%s\t%s\t-' % (a, a))
for nm, L in (('RESULTS.tsv', res), ('CHECKS.tsv', chk), ('SELF_SYMBOLS.tsv', ss), ('RENAMES.tsv', ren), ('CATALOG_CHANGES.tsv', cc),
              ('BEST_GUESS.tsv', bg), ('UNITS.tsv', units), ('TABLE_CHANGES.tsv', ['action\tsymbol\ttarget\twitness\tnote']),
              ('HEADERS.tsv', ['header\tchange\tnote'])):
    open(B + '/' + nm, 'w').write('\n'.join(L) + '\n')
open(B + '/rows.txt', 'w').write(''.join(p[0] + '\n' for p in P))
# batch-level gates: descriptive_check (repo tool, verbatim), apply_table --dry, reserved_check
env = dict(os.environ, RESERVED_REFS='')
out = []
for cmd in (['python3', 'tools/decomp_pipeline/descriptive_check.py', 'rebuild/oracles/descriptive_names.tsv', '--extra', B + '/BEST_GUESS.tsv'],
            ['python3', 'tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/apply_table.py', B + '/TABLE_CHANGES.tsv', '--dry'],
            ['python3', 'tools/decomp_pipeline/linux_lane/lane_tools/coordinator/reserved_check.py', B + '/rows', B + '/rows.txt']):
    r = subprocess.run(cmd, cwd=R, capture_output=True, text=True, env=env, timeout=600)
    out.append('$ ' + ' '.join(cmd[1:]) + '\n' + r.stdout + r.stderr)
open(B + '/evidence/batch_gates.txt', 'w').write('\n'.join(out))
print(b, dict(C), 'hold_other', hold_other)
for o in out: print(o.strip().splitlines()[0][:100], '|', o.strip().splitlines()[-1][:160])
