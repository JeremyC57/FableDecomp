# mkbatch362.py <batch dir> <payload tag> <addr>=<reason> ...: a land_scripted run dir for NF362 (no header change, no table change):
# rows/<a>.cpp + rows.txt, orig/rows + orig/tests (landed HEAD text, asserted current), head/payload.json (HEADPAY payload, filtered) +
# head/HEAD_FROM_REPO.txt (bare addresses only: land_scripted reads it as an address list), RESULTS.tsv (address verdict old new reason;
# old = new = the leaf symbol the HEAD object defines), TABLE_CHANGES.tsv (header only), evidence copies, logs/ (vland HEAD / new).
# Retail bytes (the vland oracle) stay in work/.
import sys, os, shutil, json, glob, subprocess, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); B = sys.argv[1]; tag = sys.argv[2]; specs = [x.split('=', 1) for x in sys.argv[3:]]
for d in ('rows', 'orig/rows', 'orig/tests', 'head', 'logs', 'evidence/pk_gate', 'evidence/objsec'): os.makedirs(B + '/' + d, exist_ok=True)
p = json.load(open('%s/work/pay_%s_H_payload.json' % (A, tag))); keep = {a for a, _ in specs}
p['result']['authored'] = [x for x in p['result']['authored'] if x['address'] in keep]
json.dump(p, open(B + '/head/payload.json', 'w'), indent=1)
heads = {x['address']: x['source_cpp'] for x in p['result']['authored']}
res = ['address\tverdict\told\tnew\treason']
for a, reason in specs:
    o = open(A + '/orig/rows/%s.cpp' % a, encoding='utf-8').read(); assert o == nf.src_of(a).read_text(encoding='utf-8') == heads[a], 'stale orig ' + a
    t = open(A + '/orig/tests/%s_test.cpp' % a, encoding='utf-8').read(); assert t == nf.test_of(a).read_text(encoding='utf-8'), 'stale test ' + a
    for x, y in (('rows/%s.cpp', 'rows/%s.cpp'), ('orig/rows/%s.cpp', 'orig/rows/%s.cpp'), ('orig/tests/%s_test.cpp', 'orig/tests/%s_test.cpp')):
        shutil.copy(A + '/' + x % a, B + '/' + y % a)
    for k in ('H', 'N'):
        for e in ('pkall.tsv', 'pvr.txt'): shutil.copy('%s/evidence/pk_gate/%s_%s.%s' % (A, k, a, e), B + '/evidence/pk_gate/')
    shutil.copy('%s/evidence/objsec/%s.txt' % (A, a), B + '/evidence/objsec/')
    lf = nf.leaf_of(open('%s/rows/%s.cpp' % (A, a), encoding='utf-8').read(), a)
    if not lf.startswith('?'):
        c = [x[4:] for x in nf.ext_syms('%s/work/obj/H_%s.obj' % (A, a)) if x.startswith('DEF ') and ('?' + lf + '@') in x]
        lf = c[0] if len(c) == 1 else lf
    res.append('\t'.join([a, 'ACCEPT', lf, lf, reason]))
rows = [a for a, _ in specs]
open(B + '/RESULTS.tsv', 'w').write('\n'.join(res) + '\n')
open(B + '/rows.txt', 'w').write(''.join(a + '\n' for a in rows))
open(B + '/head/HEAD_FROM_REPO.txt', 'w').write(''.join(a + '\n' for a in rows))
open(B + '/TABLE_CHANGES.tsv', 'w').write('action\tsymbol\ttarget\twitness\tnote\n')
pkc = A + '/tools/pkcmp346.py'
open(B + '/evidence/pk_gate.txt', 'w').write(subprocess.run(['python3', pkc, B + '/evidence/pk_gate'] + rows, capture_output=True, text=True).stdout)
open(B + '/evidence/pk_gate_control_reversed.txt', 'w').write(subprocess.run(['python3', pkc, B + '/evidence/pk_gate'] + rows, capture_output=True, text=True, env=dict(os.environ, PKCMP_REVERSE='1')).stdout)
rf = subprocess.run(['python3', '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/tools/reachflip.py', B + '/evidence/pk_gate'], capture_output=True, text=True).stdout
open(B + '/evidence/reachflip.txt', 'w').write(rf)
cl = subprocess.run(['python3', A + '/tools/classify362.py'] + rows, capture_output=True, text=True).stdout
open(B + '/evidence/classify.txt', 'w').write(cl)
for k, n in (('H', 'recheck_head.log'), ('N', 'recheck_new.log')): shutil.copy('%s/work/vland_%s_%s.log' % (A, tag, k), B + '/logs/' + n)
print('MKBATCH_DONE', B, len(specs), 'pk_gate:', open(B + '/evidence/pk_gate.txt').read().strip().splitlines()[-1], '| reversed:', open(B + '/evidence/pk_gate_control_reversed.txt').read().strip().splitlines()[-1], '|', rf.strip().splitlines()[-1], '| classify OK', cl.count('verdict=OK'), '/', len(rows))
