# mkbatch365.py <batch dir> <gate dest> <vland tag> <addr>=<verdict>=<reason> ...: a land_scripted run dir with NO header change (NF361's
# mkbatch361 layout): rows/<a>.cpp (var/G), orig/rows + orig/tests (landed HEAD text), rows.txt, head/payload.json (landed HEAD text,
# filtered to the batch), TABLE_CHANGES.tsv (header line only), evidence/pk_gate/{H,N}_<a>.*, evidence/objsec, leafrel, footprint (fpcheck365),
# owner_cmp, droptest, pk_gate.txt + pk_gate_control_reversed.txt + evidence/reachflip*.txt (work/gate/<dest>/), logs/recheck_{head,new}.log
# (vland; one log per vland payload, which may cover more rows than the batch), RESULTS.tsv. Retail bytes (vland oracle) stay in work/.
import sys, os, shutil, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); W = A + '/work'
B, gt, vt = sys.argv[1:4]; specs = [x.split('=', 2) for x in sys.argv[4:]]; addrs = [a for a, _, _ in specs]
for d in ('rows', 'orig/rows', 'orig/tests', 'head', 'logs', 'evidence/pk_gate', 'evidence/objsec', 'evidence/leafrel', 'evidence/footprint', 'evidence/owner_cmp', 'evidence/droptest'): os.makedirs(B + '/' + d, exist_ok=True)
open(B + '/TABLE_CHANGES.tsv', 'w').write('action\tsymbol\ttarget\twitness\tnote\n')
res = ['address\tverdict\told\tnew\treason']
for a, verdict, reason in specs:
    shutil.copy('%s/var/G/%s.cpp' % (A, a), B + '/rows/%s.cpp' % a)
    assert open('%s/var/G/%s.cpp' % (A, a), encoding='utf-8').read() == open('%s/rows/%s.cpp' % (A, a), encoding='utf-8').read()
    h = nf.src_of(a).read_text(encoding='utf-8'); assert h == open('%s/var/H/%s.cpp' % (A, a), encoding='utf-8').read(), a
    open(B + '/orig/rows/%s.cpp' % a, 'w', encoding='utf-8').write(h)
    open(B + '/orig/tests/%s_test.cpp' % a, 'w', encoding='utf-8').write(nf.test_of(a).read_text(encoding='utf-8'))
    for k in ('H', 'N'):
        for e in ('pkall.tsv', 'pvr.txt'): shutil.copy('%s/gate/%s/%s_%s.%s' % (W, gt, k, a, e), B + '/evidence/pk_gate/%s_%s.%s' % (k, a, e))
    for src, dst in (('objsec', 'objsec'), ('leafrel', 'leafrel'), ('fp', 'footprint'), ('own', 'owner_cmp')):
        shutil.copy('%s/ev/%s/%s.txt' % (W, src, a), B + '/evidence/%s/%s.txt' % (dst, a))
    shutil.copy('%s/ev/drop/%s.tsv' % (W, a), B + '/evidence/droptest/%s.tsv' % a)
    leaf = nf.leaf_of(h, a)
    res.append('\t'.join([a, verdict, leaf, leaf, reason]))
for f, dst in (('pk_gate.txt', 'pk_gate.txt'), ('pk_gate_rev.txt', 'pk_gate_control_reversed.txt'), ('reachflip.txt', 'evidence/reachflip.txt'), ('reachflip_S.txt', 'evidence/reachflip_S.txt')):
    shutil.copy('%s/gate/%s/%s' % (W, gt, f), B + '/' + dst)
for k, n in (('H', 'head'), ('N', 'new')): shutil.copy('%s/vland_%s_%s.log' % (W, vt, k), B + '/logs/recheck_%s.log' % n)
open(B + '/RESULTS.tsv', 'w').write('\n'.join(res) + '\n')
open(B + '/rows.txt', 'w').write(''.join(a + '\n' for a in addrs))
p = json.load(open('%s/pay_%s_H_payload.json' % (W, vt))); p['result']['authored'] = [x for x in p['result']['authored'] if x['address'] in addrs]
assert len(p['result']['authored']) == len(addrs)
json.dump(p, open(B + '/head/payload.json', 'w'), indent=1)
print('MKBATCH_DONE', B, len(specs))
