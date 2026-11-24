# mkbatch.py <batch dir> <name> <addr...>: a land_scripted run dir for the NF353 family port (FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY):
# rows/<a>.cpp, orig/rows/<a>.cpp, orig/tests/<a>_test.cpp, rows.txt, head/payload.json (+ HEAD_FROM_REPO.txt), empty TABLE_CHANGES.tsv /
# UNITS.tsv / HEADERS.tsv, hdr/engine/ (empty), evidence/pk_gate/{H,N}_<a>.{pkall.tsv,pvr.txt}, evidence/objdiff/<a>.txt,
# evidence/resolve/<a>.txt, pk_gate.txt (pkcmp346 HEAD v new) + pk_gate_control_reversed.txt, pk_reach.txt (+ reversed),
# evidence/grade_head_v_new.txt + logs/recheck_{head,new}.log (from work/vland_<name>_{H,N}.log), RESULTS.tsv. Agent NF353.
import sys, os, re, shutil, subprocess, json, contextlib, io
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline'); import verify_and_land as V
B, name, addrs = sys.argv[1], sys.argv[2], sys.argv[3:]
A = str(nf.A)
for d in ('rows', 'orig/rows', 'orig/tests', 'head', 'hdr/engine', 'evidence/pk_gate', 'evidence/objdiff', 'evidence/resolve', 'logs'): os.makedirs(B + '/' + d, exist_ok=True)
for f, h in (('TABLE_CHANGES.tsv', 'action\tsymbol\ttarget\twitness\tnote'), ('UNITS.tsv', 'address\tunit'), ('HEADERS.tsv', 'header\tchange\tnote')):
    open(B + '/' + f, 'w').write(h + '\n')
ev = {}
for l in open(A + '/work/rowsummary.tsv'):
    f = l.rstrip('\n').split('\t'); ev[f[0]] = f
for a in addrs:
    shutil.copy(A + '/rows/%s.cpp' % a, B + '/rows/%s.cpp' % a)
    shutil.copy(A + '/orig/rows/%s.cpp' % a, B + '/orig/rows/%s.cpp' % a)
    assert open(B + '/orig/rows/%s.cpp' % a, encoding='utf-8').read() == nf.src_of(a).read_text(encoding='utf-8'), 'stale orig ' + a
    shutil.copy(A + '/orig/tests/%s_test.cpp' % a, B + '/orig/tests/%s_test.cpp' % a)
    for k in ('H', 'N'):
        for e in ('pkall.tsv', 'pvr.txt'): shutil.copy(A + '/evidence/pk/%s_%s.%s' % (k, a, e), B + '/evidence/pk_gate/%s_%s.%s' % (k, a, e))
    shutil.copy(A + '/evidence/objdiff/%s.txt' % a, B + '/evidence/objdiff/%s.txt' % a)
    shutil.copy(A + '/evidence/resolve/%s.txt' % a, B + '/evidence/resolve/%s.txt' % a)
open(B + '/rows.txt', 'w').write(''.join(a + '\n' for a in addrs))
subprocess.run(['python3', A + '/tools/mkpay.py', 'H', A + '/work/pay_%s_H' % name] + addrs, check=True, capture_output=True)
shutil.copy(A + '/work/pay_%s_H_payload.json' % name, B + '/head/payload.json')
for x in json.load(open(B + '/head/payload.json'))['result']['authored']:
    assert x['source_cpp'] == nf.src_of(x['address']).read_text(encoding='utf-8'), x['address']
open(B + '/head/HEAD_FROM_REPO.txt', 'w').write('head/payload.json: each row\'s landed source and test read from the repo (/home/user/FableDecomp) at hand-back time; asserted equal to the repo files.\n')
r = subprocess.run(['python3', A + '/tools/pkcmp346.py', B + '/evidence/pk_gate'] + addrs, capture_output=True, text=True).stdout
open(B + '/pk_gate.txt', 'w').write('# pkcmp346.py HEAD v new, both checkers (repo pk_all.py --tsv --leaf=<full Leaf> --row-length=<catalog>; pickany_vs_retail.py --leaf=<full Leaf>)\n' + r)
r2 = subprocess.run(['python3', A + '/tools/pkcmp346.py', B + '/evidence/pk_gate'] + addrs, capture_output=True, text=True, env=dict(os.environ, PKCMP_REVERSE='1')).stdout
open(B + '/pk_gate_control_reversed.txt', 'w').write('# planted control: the same comparison with the sides swapped (new as HEAD). The dropped copies read as ADDED, and any NEQ the change removed\n# reads NEWLY_NEQ leaf-called, so the gate must FAIL here.\n' + r2)
r3 = subprocess.run(['python3', A + '/tools/pkreach.py', B + '/evidence/pk_gate'] + addrs, capture_output=True, text=True).stdout
r4 = subprocess.run(['python3', A + '/tools/pkreach.py', B + '/evidence/pk_gate'] + addrs, capture_output=True, text=True, env=dict(os.environ, PKREACH_REVERSE='1')).stdout
open(B + '/pk_reach.txt', 'w').write('# pkreach.py HEAD v new (coordinator ruling 2026-11-24: an already-NEQ copy whose leaf reach flips n -> y fails the gate)\n' + r3 +
                                    '# control, sides swapped (the removed leaf-called NEQ ??A copy must read as a flip):\n' + r4)
H, N = A + '/work/vland_%s_H.log' % name, A + '/work/vland_%s_N.log' % name
shutil.copy(H, B + '/logs/recheck_head.log'); shutil.copy(N, B + '/logs/recheck_new.log')
def st(p):
    d = {}
    for l in open(p, errors='replace'):
        m = re.match(r'([0-9a-f]{8})\s+(\S+)\s+(\S+)\s', l)
        if m and m.group(2) != 'WARN': d.setdefault(m.group(1), (m.group(2), m.group(3)))
    return d
hs, ns = st(H), st(N); out = ['# vland --recheck-landed, HEAD payload (head/payload.json) v new payload (rows/): parity behaviour per row']; bad = 0
for a in addrs:
    x, y = hs.get(a, ('NOT_GRADED', '-')), ns.get(a, ('NOT_GRADED', '-'))
    ok = x == y and x[0] in ('MATCH', 'RELOCATION_MATCH') and x[1] == 'PASS'; bad += not ok
    out.append('%s\t%s %s\t%s %s\t%s' % (a, x[0], x[1], y[0], y[1], 'SAME' if ok else 'CHANGED_OR_FAIL'))
out.append('# rows %d, grade unchanged and passing %d, other %d' % (len(addrs), len(addrs) - bad, bad))
open(B + '/evidence/grade_head_v_new.txt', 'w').write('\n'.join(out) + '\n')
res = ['address\tverdict\told\tnew\treason']
for a in addrs:
    leaf = nf.leaf_of(nf.src_of(a).read_text(encoding='utf-8'), a)
    with contextlib.redirect_stderr(io.StringIO()):
        old = V.obj_text(A + '/work/obj/H_%s.obj' % a, leaf)[2]; new = V.obj_text(A + '/work/obj/N_%s.obj' % a, leaf)[2]
    e = ev[a]; g = [l for l in out if l.startswith(a)][0].split('\t')
    res.append('%s\tACCEPT\t%s\t%s\tNF353 family port (T3 nf340 0x00CD3D2E): adds `#define FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY` under a `// Fidelity: scaffolding, not retail` block before the includes; flags %s; '
               'vland --recheck-landed HEAD %s, new %s (%s); leafrel HEAD v new: %s; objdiff: %s; pk gate (both checkers) %s, reach flips %s; '
               'resolve: %s; drop test: evidence/drop_census.tsv in the hand-back (cross-batch, definers outside all NF353 batches)' % (
               a, old, new, nf.field(a, 'CompilerFlags') or '-', g[1], g[2], g[3], e[1], e[2], e[3], e[4], e[5]))
open(B + '/RESULTS.tsv', 'w').write('\n'.join(res) + '\n')
print(B, len(addrs), out[-1], '|', r.strip().splitlines()[-1], '| control:', r2.strip().splitlines()[-1], '|', r3.strip().splitlines()[-1])
