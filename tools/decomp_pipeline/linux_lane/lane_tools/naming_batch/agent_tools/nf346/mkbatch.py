# mkbatch.py <batch dir> <verdict> <addr...>: a land_scripted run dir: rows/<a>.cpp (new), orig/rows/<a>.cpp + orig/tests/<a>_test.cpp
# (landed HEAD text), rows.txt, head/payload.json (HEADPAY: the landed source/test text, untouched), evidence/pk_gate/{H,N}_<a>.*,
# evidence/objsec/<a>.txt, pk_gate.txt (pkcmp346, HEAD v new) + pk_gate_control_reversed.txt (sides swapped), RESULTS.tsv
# (address verdict old new reason; old/new = the graded symbol V.obj_text picks in the HEAD / new object with the row's Leaf).
# The oracle file nf.py pay writes (retail bytes) stays in work/.
import sys, os, re, shutil, subprocess, contextlib, io, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline'); import verify_and_land as V
B, verdict, addrs = sys.argv[1], sys.argv[2], sys.argv[3:]
A = str(nf.A)
for d in ('rows', 'orig/rows', 'orig/tests', 'head', 'evidence/pk_gate', 'evidence/objsec'): os.makedirs(B + '/' + d, exist_ok=True)
ev = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(A + '/work/eval.tsv')}
res = ['address\tverdict\told\tnew\treason']
for a in addrs:
    shutil.copy(A + '/rows/%s.cpp' % a, B + '/rows/%s.cpp' % a)
    shutil.copy(A + '/orig/rows/%s.cpp' % a, B + '/orig/rows/%s.cpp' % a)
    if os.path.exists(A + '/orig/tests/%s_test.cpp' % a): shutil.copy(A + '/orig/tests/%s_test.cpp' % a, B + '/orig/tests/%s_test.cpp' % a)
    for k in ('H', 'N'):
        for e in ('pkall.tsv', 'pvr.txt'): shutil.copy(A + '/evidence/pk/%s_%s.%s' % (k, a, e), B + '/evidence/pk_gate/%s_%s.%s' % (k, a, e))
    shutil.copy(A + '/evidence/objsec/%s.txt' % a, B + '/evidence/objsec/%s.txt' % a)
    leaf = nf.leaf_of(nf.src_of(a).read_text(encoding='utf-8'), a)
    with contextlib.redirect_stderr(io.StringIO()):
        old = V.obj_text(A + '/work/obj/H_%s.obj' % a, leaf)[2]; new = V.obj_text(A + '/work/obj/%s.obj' % a, leaf)[2]
    pc = subprocess.run(['python3', A + '/tools/pkcmp346.py', A + '/evidence/pk', a], capture_output=True, text=True).stdout
    fixed = sorted(set('%s %s' % (re.match(r'\?\?\$GetDef@(.*?)@@@', l.split('\t')[5]).group(1).lstrip('V'), '0x' + l.split('\t')[3].upper())
                       for l in pc.splitlines() if '\tFIXED\t' in l and '??$GetDef@' in l))
    e = ev[a]
    why = ('NF346: adds #define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD next to the row\'s GETDEF_GLOBAL_BODY (existing CDefPointer.h opt-in, the retail '
           're-reading SetDynamicCast; owner 0x0066C340, as landed 0x0066B370 / batches/2026-11-24_landing_nf341_r2; no header change): GetDef copies NEQ -> EQ (both checkers): %s; '
           'own symbol unchanged, scratch grade %s before and after; pk gate PASS (0 turned / newly NEQ); objsec: leaf identical, only GetDef<T> copies turning EQ, '
           'their NOADDR SetDynamicCast helpers, and dropped unreferenced NOADDR Assign helpers differ (%s)') % ('; '.join('GetDef<%s> %s' % tuple(f.split()) for f in fixed), e[2], e[7])
    if verdict != 'ACCEPT': why = '%s: %s | left: %s' % (verdict, why, e[8][:300])
    res.append('%s\t%s\t%s\t%s\t%s' % (a, verdict, old, new, why))
open(B + '/RESULTS.tsv', 'w').write('\n'.join(res) + '\n')
open(B + '/rows.txt', 'w').write(''.join(a + '\n' for a in addrs))
w = A + '/work/pay_' + os.path.basename(B.rstrip('/'))
subprocess.run(['python3', A + '/tools/nf.py', 'pay', w + '_H'] + addrs, env=dict(os.environ, HEADPAY='1'), check=True)
subprocess.run(['python3', A + '/tools/nf.py', 'pay', w + '_N'] + addrs, check=True)
shutil.copy(w + '_H_payload.json', B + '/head/payload.json')
for x in json.load(open(B + '/head/payload.json'))['result']['authored']:
    assert x['source_cpp'] == nf.src_of(x['address']).read_text(encoding='utf-8'), x['address']
r = subprocess.run(['python3', A + '/tools/pkcmp346.py', B + '/evidence/pk_gate'] + addrs, capture_output=True, text=True).stdout
open(B + '/pk_gate.txt', 'w').write('# pkcmp346.py HEAD v new, both checkers (pk_all.py --tsv --row-length=<catalog> and pickany_vs_retail.py)\n' + r)
r2 = subprocess.run(['python3', A + '/tools/pkcmp346.py', B + '/evidence/pk_gate'] + addrs, capture_output=True, text=True, env=dict(os.environ, PKCMP_REVERSE='1')).stdout
open(B + '/pk_gate_control_reversed.txt', 'w').write('# planted control: the same comparison with the sides swapped (new as HEAD); every fix must read TURNED_NEQ and the gate FAIL\n' + r2)
print(B, len(addrs), r.strip().splitlines()[-1], '| control:', r2.strip().splitlines()[-1])
