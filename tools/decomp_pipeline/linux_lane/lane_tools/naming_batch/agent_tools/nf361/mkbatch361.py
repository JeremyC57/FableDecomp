# mkbatch361.py <batch dir> <rowdir> <gate tag> <isolation tag|-> <payH json> <vland tag> <addr>=<verdict>=<reason> ...: a land_scripted run
# dir with NO header change: rows/<a>.cpp (from <rowdir>), orig/rows + orig/tests (landed HEAD text, asserted equal to the repo), rows.txt,
# head/payload.json (landed HEAD text), TABLE_CHANGES.tsv (header line only), evidence/pk_gate/{H,N}_<a>.* (both checkers),
# evidence/objsec/<a>.txt, evidence/leafrel/<a>.txt (+ _isolation copies), evidence/owner_cmp.tsv, evidence/pragscan_c.txt,
# pk_gate.txt + pk_gate_control_reversed.txt + evidence/reachflip.txt (work/gate/<tag>/), logs/recheck_{head,new}.log (vland),
# RESULTS.tsv (address verdict old new reason). Retail bytes (the vland oracle) stay in work/.
import sys, os, shutil, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); W = A + '/work'
B, rowdir, gt, iso, payH, vt = sys.argv[1:7]; specs = [x.split('=', 2) for x in sys.argv[7:]]; addrs = [a for a, _, _ in specs]
for d in ('rows', 'orig/rows', 'orig/tests', 'head', 'logs', 'evidence/pk_gate', 'evidence/objsec', 'evidence/leafrel', 'evidence/isolation'): os.makedirs(B + '/' + d, exist_ok=True)
open(B + '/TABLE_CHANGES.tsv', 'w').write('action\tsymbol\ttarget\twitness\tnote\n')
res = ['address\tverdict\told\tnew\treason']
for a, verdict, reason in specs:
    shutil.copy('%s/%s.cpp' % (rowdir, a), B + '/rows/%s.cpp' % a)
    open(B + '/orig/rows/%s.cpp' % a, 'w', encoding='utf-8').write(nf.src_of(a).read_text(encoding='utf-8'))
    open(B + '/orig/tests/%s_test.cpp' % a, 'w', encoding='utf-8').write(nf.test_of(a).read_text(encoding='utf-8'))
    for k in ('H', 'N'):
        for e in ('pkall.tsv', 'pvr.txt'): shutil.copy('%s/gate/%s/%s_%s.%s' % (W, gt, k, a, e), B + '/evidence/pk_gate/%s_%s.%s' % (k, a, e))
    shutil.copy('%s/ev/objsec/%s_%s.txt' % (W, a, gt), B + '/evidence/objsec/%s.txt' % a)
    shutil.copy('%s/ev/leafrel/%s_%s.txt' % (W, a, gt), B + '/evidence/leafrel/%s.txt' % a)
    if iso != '-':
        shutil.copy('%s/ev/objsec/%s_%s.txt' % (W, a, iso), B + '/evidence/isolation/%s_objsec_%s.txt' % (a, iso))
        for e in ('pkall.tsv', 'pvr.txt'): shutil.copy('%s/pk/%s_%s.%s' % (W, iso, a, e), B + '/evidence/isolation/%s_%s.%s' % (iso, a, e))
    leaf = nf.leaf_of(nf.src_of(a).read_text(encoding='utf-8'), a)
    res.append('\t'.join([a, verdict, leaf, leaf, reason]))
for f, dst in (('pk_gate.txt', 'pk_gate.txt'), ('pk_gate_rev.txt', 'pk_gate_control_reversed.txt'), ('reachflip.txt', 'evidence/reachflip.txt'), ('reachflip_S.txt', 'evidence/reachflip_S.txt')):
    shutil.copy('%s/gate/%s/%s' % (W, gt, f), B + '/' + dst)
for k, n in (('H', 'head'), ('N', 'new')): shutil.copy('%s/vland_%s_%s.log' % (W, vt, k), B + '/logs/recheck_%s.log' % n)
for f in ('owner_cmp_%s.tsv' % gt, 'pragscan_%s.txt' % gt, 'lvsize_%s.txt' % vt):
    if os.path.exists(W + '/' + f): shutil.copy(W + '/' + f, B + '/evidence/' + f.replace('_%s.' % gt, '.').replace('_%s.' % vt, '.'))
open(B + '/RESULTS.tsv', 'w').write('\n'.join(res) + '\n')
open(B + '/rows.txt', 'w').write(''.join(a + '\n' for a in addrs))
p = json.load(open(payH)); p['result']['authored'] = [x for x in p['result']['authored'] if x['address'] in addrs]
assert len(p['result']['authored']) == len(addrs)
json.dump(p, open(B + '/head/payload.json', 'w'), indent=1)
print('MKBATCH_DONE', B, len(specs))
