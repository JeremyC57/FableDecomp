# mkbatch356.py <batch dir> <addr>=<verdict>=<reason> ...: a land_scripted run dir for NF356 (owner ports onto landed size-body opt-ins;
# no header change): rows/<a>.cpp, rows.txt, orig/rows + orig/tests (landed HEAD text, asserted current), head/payload.json (HEADPAY
# payload) + head/HEAD_FROM_REPO.txt (sha1 of the repo file and of the payload text), RESULTS.tsv (address verdict old_leaf new_leaf reason;
# the old leaf is the symbol the HEAD object defines), evidence copies (objsec, leafrel, pk_gate, droptest, link, pragscan), the gate
# texts, logs/ (vland HEAD / new). Retail bytes (the vland oracle) stay in work/.
import sys, os, shutil, json, hashlib, glob
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); R = '/home/user/FableDecomp'
B = sys.argv[1]; specs = [x.split('=', 2) for x in sys.argv[2:]]
OLD = {'0042af3c': '?GetDef_0042af3c@CDefinitionManager@@QAE_NJAAV?$CDefPointer@$$CBVCDefClassBase@@@@@Z'}
for d in ('rows', 'orig/rows', 'orig/tests', 'head', 'logs', 'evidence/pk_gate', 'evidence/objsec', 'evidence/leafrel', 'evidence/link'): os.makedirs(B + '/' + d, exist_ok=True)
res = ['address\tverdict\told\tnew\treason']; hf = []
p = json.load(open(A + '/work/pay_b1_H_payload.json')); keep = {a for a, _, _ in specs}   # one HEAD payload for all six rows, filtered to this batch
p['result']['authored'] = [x for x in p['result']['authored'] if x['address'] in keep]
json.dump(p, open(B + '/head/payload.json', 'w'), indent=1)
srcs = {x['address']: x['source_cpp'] for x in p['result']['authored']}
for a, verdict, reason in specs:
    shutil.copy('%s/rows/%s.cpp' % (A, a), B + '/rows/%s.cpp' % a)
    o = open(A + '/orig/rows/%s.cpp' % a).read(); assert o == nf.src_of(a).read_text(encoding='utf-8'), 'stale orig ' + a
    shutil.copy(A + '/orig/rows/%s.cpp' % a, B + '/orig/rows/%s.cpp' % a)
    t = open(A + '/orig/tests/%s_test.cpp' % a).read(); assert t == nf.test_of(a).read_text(encoding='utf-8'), 'stale test ' + a
    shutil.copy(A + '/orig/tests/%s_test.cpp' % a, B + '/orig/tests/%s_test.cpp' % a)
    h1 = hashlib.sha1(o.encode('utf-8')).hexdigest(); h2 = hashlib.sha1(srcs[a].encode('utf-8')).hexdigest()
    hf.append('%s\trebuild/src/compiled/%s\t%s\t%s\t%s' % (a, nf.field(a, 'Source'), h1, h2, 'SAME' if h1 == h2 else 'DIFF'))
    for k in ('H', 'N'):
        for e in ('pkall.tsv', 'pvr.txt'): shutil.copy('%s/evidence/pk_gate/%s_%s.%s' % (A, k, a, e), B + '/evidence/pk_gate/')
    for f in glob.glob(A + '/evidence/objsec/%s*.txt' % a): shutil.copy(f, B + '/evidence/objsec/')
    shutil.copy(A + '/evidence/leafrel/%s.txt' % a, B + '/evidence/leafrel/')
    shutil.copy(A + '/evidence/droptest_%s.tsv' % a, B + '/evidence/')
    new = nf.leaf_of(open('%s/rows/%s.cpp' % (A, a)).read(), a)
    res.append('\t'.join([a, verdict, OLD.get(a, new), new, reason]))
for f in glob.glob(A + '/evidence/link_*.txt'): shutil.copy(f, B + '/evidence/link/')
for f in ('pragscan.txt', 'pragscan_control_planted.txt', 'drop_refscan.txt', 'reachflip.txt', 'probes.tsv'): shutil.copy(A + '/evidence/' + f, B + '/evidence/')
import subprocess
rows = [a for a, _, _ in specs]; pkc = os.path.dirname(os.path.abspath(__file__)) + '/pkcmp346.py'
open(B + '/pk_gate.txt', 'w').write(subprocess.run(['python3', pkc, B + '/evidence/pk_gate'] + rows, capture_output=True, text=True).stdout)
open(B + '/pk_gate_control_reversed.txt', 'w').write(subprocess.run(['python3', pkc, B + '/evidence/pk_gate'] + rows, capture_output=True, text=True, env=dict(os.environ, PKCMP_REVERSE='1')).stdout)
shutil.copy(A + '/evidence/pk_gate_control_planted.txt', B + '/pk_gate_control_planted.txt')
for f in ('RENAMES.tsv', 'NAME_FIXES.tsv', 'TABLE_CHANGES.tsv'):
    L = open(A + '/' + f).read().splitlines(); hdr, body = L[0], [l for l in L[1:] if any(r in l.split('\t')[:2] for r in rows)]
    open(B + '/' + f, 'w').write('\n'.join([hdr] + body) + '\n')
for k, n in (('H', 'recheck_head.log'), ('N', 'recheck_new.log')): shutil.copy(A + '/work/vland_b1_%s.log' % k, B + '/logs/' + n)
open(B + '/head/HEAD_FROM_REPO.txt', 'w').write('HEAD payload (head/payload.json) built from the repo sources by nf.py pay with HEADPAY=1 (NF356, 2026-11-24). '
    'Per row: repo file, sha1 now, sha1 of the payload source_cpp:\n' + '\n'.join(hf) + '\n')
open(B + '/RESULTS.tsv', 'w').write('\n'.join(res) + '\n')
open(B + '/rows.txt', 'w').write(''.join(a + '\n' for a, _, _ in specs))
print('MKBATCH_DONE', B, len(specs))
