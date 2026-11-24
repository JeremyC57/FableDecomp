# mkbatch352.py <batch dir> <rowdir> <pkdir> <objsuffix> <addr>=<verdict>=<reason> ...: a land_scripted run dir for NF352:
#   rows/<a>.cpp (from <rowdir>), orig/rows + orig/tests (landed HEAD text), rows.txt, head/payload.json (landed HEAD text),
#   hdr/engine/CCountedPointer.h + orig/hdr_CCountedPointer.h + HEADERS.tsv, evidence/pk_gate/{H,N}_<a>.* (both checkers),
#   evidence/objsec/<a>.txt, evidence/leafrel/<a>.txt, evidence/droptest_<a>.tsv, evidence/pragscan_c.txt,
#   pk_gate.txt (pkcmp346 H v N) + pk_gate_control_reversed.txt (sides swapped), RESULTS.tsv (address verdict old new reason).
# Retail bytes (the vland oracle) stay in work/.
import sys, os, shutil, subprocess, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); R = '/home/user/FableDecomp'
B, rowdir, pkdir, suf = sys.argv[1:5]; specs = [x.split('=', 2) for x in sys.argv[5:]]
for d in ('rows', 'orig/rows', 'orig/tests', 'head', 'hdr/engine', 'evidence/pk_gate', 'evidence/objsec', 'evidence/leafrel'): os.makedirs(B + '/' + d, exist_ok=True)
shutil.copy(A + '/hdr/engine/CCountedPointer.h', B + '/hdr/engine/CCountedPointer.h')
shutil.copy(A + '/orig/hdr_CCountedPointer/CCountedPointer.h', B + '/orig/hdr_CCountedPointer.h')
assert open(B + '/orig/hdr_CCountedPointer.h').read() == open(R + '/rebuild/include/engine/CCountedPointer.h').read(), 'stale orig header'
note = ('engine/CCountedPointer.h: add-only opt-in (one new block before the closing include guard; no existing line changed): size-mode '
        '`template<> __declspec(noinline) inline` %s = owner %s, which sets optimize("s", on) at the catalog default /O2 /Oy /W3 '
        '(EXTRA_RULES 2026-11-20 size-pragma scaffolding, 2026-11-21 condition 2; NF349 per-type form); equiv_check over the 13,751-row reverse '
        'include closure: 0 differing; planted control flags every includer (evidence/hdr/ in the NF352 hand-back)')
open(B + '/HEADERS.tsv', 'w').write('class\tverdict\tmacro\tnotes\n'
    'CCountedPointer\tOK\tFABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY\t' + note % ('Reset<NUISystem::CComponent>', '0x00429C6F') + '\n'
    'CCountedPointer\tOK\tFABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RELEASEDATA_SIZE_BODY\t' + note % ('ReleaseData<NUISystem::CComponent>', '0x004291DE')
    + '; used only by the batches/r1 ruling texts (Q1); with no user it emits nothing\n')
res = ['address\tverdict\told\tnew\treason']; gate = []; rev = []
prag = {l.split()[0]: l.rstrip('\n') for l in open(A + '/evidence/pragscan_final.txt') if l.strip()}
for a, verdict, reason in specs:
    shutil.copy('%s/%s.cpp' % (rowdir, a), B + '/rows/%s.cpp' % a)
    shutil.copy(A + '/orig/rows/%s.cpp' % a, B + '/orig/rows/%s.cpp' % a)
    assert open(B + '/orig/rows/%s.cpp' % a).read() == nf.src_of(a).read_text(encoding='utf-8'), 'stale orig ' + a
    shutil.copy(A + '/orig/tests/%s_test.cpp' % a, B + '/orig/tests/%s_test.cpp' % a)
    for k in ('H', 'N'):
        for e in ('pkall.tsv', 'pvr.txt'): shutil.copy('%s/%s_%s.%s' % (pkdir, k, a, e), B + '/evidence/pk_gate/%s_%s.%s' % (k, a, e))
    shutil.copy(A + '/evidence/objsec/%s%s.txt' % (a, suf), B + '/evidence/objsec/%s.txt' % a)
    shutil.copy(A + '/evidence/leafrel/%s%s.txt' % (a, suf), B + '/evidence/leafrel/%s.txt' % a)
    shutil.copy(A + '/evidence/droptest_%s%s.tsv' % (a, suf), B + '/evidence/droptest_%s.tsv' % a)
    gate.append(open('%s/gate_%s.txt' % (pkdir, a)).read()); rev.append(open('%s/gate_%s_reversed.txt' % (pkdir, a)).read())
    leaf = nf.leaf_of(nf.src_of(a).read_text(encoding='utf-8'), a)
    res.append('\t'.join([a, verdict, leaf, leaf, reason]))
open(B + '/evidence/pragscan_c.txt', 'w').write(''.join((prag.get(a + suf) or prag.get(a, a + ' -')) + '\n' for a, _, _ in specs))
open(B + '/pk_gate.txt', 'w').write('\n'.join(gate)); open(B + '/pk_gate_control_reversed.txt', 'w').write('\n'.join(rev))
open(B + '/RESULTS.tsv', 'w').write('\n'.join(res) + '\n')
open(B + '/rows.txt', 'w').write(''.join(a + '\n' for a, _, _ in specs))
p = json.load(open(A + '/work/pay_%s_H_payload.json' % os.path.basename(B.rstrip('/'))))
json.dump(p, open(B + '/head/payload.json', 'w'), indent=1)
print('MKBATCH_DONE', B, len(specs))
