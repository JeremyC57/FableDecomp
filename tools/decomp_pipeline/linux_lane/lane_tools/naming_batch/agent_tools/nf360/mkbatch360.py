# mkbatch360.py <batch dir> <addr> <verdict> <old self> <reason> [variant files...]: a one-row land_scripted run dir for NF360 (in-place self
# respells; no header change, no test change): rows/<a>.cpp, rows.txt, orig/rows + orig/tests (landed HEAD text, asserted current),
# head/payload.json (HEADPAY payload filtered to the row; no prose HEAD_FROM_REPO), RESULTS.tsv, TABLE_CHANGES / RENAMES / NAME_FIXES (lines for
# the row), pk gate texts (+ reversed / planted controls), evidence copies, logs/ (vland HEAD / new; filtered to the row), variants/.
# Retail bytes (the vland oracle) stay in work/.
import sys, os, shutil, json, glob, subprocess
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A)
B, a, verdict, old, reason = sys.argv[1:6]; var = sys.argv[6:]
for d in ('rows', 'orig/rows', 'orig/tests', 'head', 'logs', 'evidence/pk_gate', 'evidence/objsec', 'evidence/link', 'variants'): os.makedirs(B + '/' + d, exist_ok=True)
p = json.load(open(A + '/work/pay_b1_H_payload.json')); p['result']['authored'] = [x for x in p['result']['authored'] if x['address'] == a]
assert len(p['result']['authored']) == 1 and p['result']['authored'][0]['source_cpp'] == nf.src_of(a).read_text(encoding='utf-8')
json.dump(p, open(B + '/head/payload.json', 'w'), indent=1)
pn = json.load(open(A + '/work/pay_b1_N_payload.json')); e = [x for x in pn['result']['authored'] if x['address'] == a][0]
assert e['source_cpp'] == open(A + '/rows/%s.cpp' % a).read(), 'new payload is stale'
shutil.copy('%s/rows/%s.cpp' % (A, a), B + '/rows/%s.cpp' % a)
o = open(A + '/orig/rows/%s.cpp' % a).read(); assert o == nf.src_of(a).read_text(encoding='utf-8'), 'stale orig'
shutil.copy(A + '/orig/rows/%s.cpp' % a, B + '/orig/rows/')
t = open(A + '/orig/tests/%s_test.cpp' % a).read(); assert t == nf.test_of(a).read_text(encoding='utf-8'), 'stale test'
shutil.copy(A + '/orig/tests/%s_test.cpp' % a, B + '/orig/tests/')
for k in ('H', 'N'):
    for x in ('pkall.tsv', 'pvr.txt'): shutil.copy('%s/evidence/pk_gate/%s_%s.%s' % (A, k, a, x), B + '/evidence/pk_gate/')
for f in glob.glob(A + '/evidence/objsec/%s*.txt' % a): shutil.copy(f, B + '/evidence/objsec/')
shutil.copy(A + '/evidence/droptest_%s.tsv' % a, B + '/evidence/')
tag = {'0042af3c': 'af3c', '00428b10': 'dd'}[a]
for f in glob.glob(A + '/evidence/link_%s_*.txt' % tag): shutil.copy(f, B + '/evidence/link/')
for f in ('symscan_summary.txt', 'reachflip.txt', 'copycensus_%s.txt' % {'0042af3c': 'getdef', '00428b10': 'deletedata'}[a],
          'cmpcomdat_%s.txt' % {'0042af3c': 'getdef', '00428b10': 'deletedata'}[a]): shutil.copy(A + '/evidence/' + f, B + '/evidence/')
for f in glob.glob(A + '/evidence/pkall_ctl_*.tsv'): shutil.copy(f, B + '/evidence/')
pkc = os.path.dirname(os.path.abspath(__file__)) + '/pkcmp346.py'
open(B + '/pk_gate.txt', 'w').write(subprocess.run(['python3', pkc, B + '/evidence/pk_gate', a], capture_output=True, text=True).stdout)
open(B + '/pk_gate_control_reversed.txt', 'w').write(subprocess.run(['python3', pkc, B + '/evidence/pk_gate', a], capture_output=True, text=True, env=dict(os.environ, PKCMP_REVERSE='1')).stdout)
shutil.copy(A + '/evidence/pk_gate_control_planted.txt', B + '/pk_gate_control_planted.txt')
for f in ('RENAMES.tsv', 'NAME_FIXES.tsv', 'TABLE_CHANGES.tsv'):
    L = open(A + '/' + f).read().splitlines(); body = [l for l in L[1:] if a in l.split('\t')[:2]]
    open(B + '/' + f, 'w').write('\n'.join([L[0]] + body) + '\n')
for k, n in (('H', 'recheck_head.log'), ('N', 'recheck_new.log')):
    shutil.copy(A + '/work/vland_b1_%s.log' % k, B + '/logs/' + n)
for v in var: shutil.copy(v, B + '/variants/')
new = nf.leaf_of(open('%s/rows/%s.cpp' % (A, a)).read(), a)
open(B + '/RESULTS.tsv', 'w').write('address\tverdict\told\tnew\treason\n' + '\t'.join([a, verdict, old, new, reason]) + '\n')
open(B + '/rows.txt', 'w').write(a + '\n')
print('MKBATCH_DONE', B, a, verdict)
