# mkbatch364.py <batch dir>: the land_scripted run dir for NF364 b1 (19 rows, one header opt-in, no table change):
#   rows/<a>.cpp (+ rows.txt), orig/rows + orig/tests (landed HEAD text, asserted equal to the repo NOW), head/payload.json (HEAD payload) +
#   head/HEAD_FROM_REPO.txt (bare addresses only), hdr/engine/CCountedPointer.h (new) + orig/hdr_CCountedPointer.h (repo, asserted equal NOW) +
#   HEADERS.tsv, TABLE_CHANGES.tsv (header line only), RESULTS.tsv (ACCEPT only where check364 / pkcmp / reachflip / pragscan / vland all pass),
#   evidence/ (pk_gate H/N, objsec, check364, pragscan364, pk_gate.txt, reversed control, reachflip), logs/ (vland HEAD / new).
import sys, os, shutil, json, re
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF364'
sys.path.insert(0, A + '/tools'); import nf
R = '/home/user/FableDecomp'; B = sys.argv[1]
ROWS = '00427216 00582ace 0058315b 0058a0c7 0058e208 005907ab 0059cb90 0059d270 0059ed42 0059f51e 0059fee3 005a8c66 005d64dd 005d6748 005d7081 005a29f4 005a9cbe 005aaf28 005ad571'.split()
for d in ('rows', 'orig/rows', 'orig/tests', 'head', 'hdr/engine', 'evidence/pk_gate', 'evidence/objsec', 'logs'): os.makedirs(B + '/' + d, exist_ok=True)
H = R + '/rebuild/include/engine/CCountedPointer.h'
assert open(A + '/work/inc_old/engine/CCountedPointer.h').read() == open(H).read(), 'repo header moved since the trees were copied'
shutil.copy(A + '/work/inc_new/engine/CCountedPointer.h', B + '/hdr/engine/CCountedPointer.h'); shutil.copy(H, B + '/orig/hdr_CCountedPointer.h')
open(B + '/HEADERS.tsv', 'w').write('class\tverdict\tmacro\tnotes\nCCountedPointer\tOK\tFABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY_YOFF_TU\t'
    'engine/CCountedPointer.h: add-only opt-in (one new block before the closing include guard; no existing line changed; 38 added lines): the y-off / s-on '
    'sibling of FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY (NF362 P1, advisor decision, owner delegated): optimize("y", on) + ("s", on), the same '
    '`template<> __declspec(noinline) inline` Reset<NUISystem::CComponent> body (= owner 0x00429C6F), then RESTORES optimize("y", off) + ("s", on); '
    '#error with the RESET_SIZE_BODY macro; Fidelity: scaffolding, not retail. Whole-catalog equiv_check old v new tree: 0 differing; planted control '
    '(mirror with the 19 new rows, new v new+token): exactly the 19 user rows (src), 0 tests (evidence/equiv/)\n')
open(B + '/TABLE_CHANGES.tsv', 'w').write('action\tsymbol\ttarget\twitness\tnote\n')
gate = {l.split()[0]: l.split()[1] for l in open(A + '/evidence/check364.txt') if re.match(r'[0-9a-f]{8} ', l)}
prag = {l.split()[0]: l.split()[1] for l in open(A + '/evidence/pragscan364.txt') if re.match(r'[0-9a-f]{8} ', l)}
vl = {}
for k in ('H', 'N'):
    for l in open(A + '/work/vland_b1_%s.log' % k):
        m = re.match(r'([0-9a-f]{8})\s+(MATCH|RELOCATION_MATCH|DIFFER\S*|\S+)\s+(PASS|FAIL)', l)
        if m: vl[(k, m.group(1))] = m.group(2) + ' ' + m.group(3)
    shutil.copy(A + '/work/vland_b1_%s.log' % k, B + '/logs/recheck_%s.log' % ('head' if k == 'H' else 'new'))
pk_ok = open(A + '/evidence/pk_gate.txt').read().rstrip().endswith('GATE PASS') and 'GATE FAIL' in open(A + '/evidence/pk_gate_control_reversed.txt').read()
rf_ok = 'pairs 19 NEQ reach flips 0' in open(A + '/evidence/reachflip.txt').read()
# equiv: the whole-catalog run finished ('rows checked' in its log) with 0 differing lines; the planted control flags exactly the 19 rows' src, no test
eqf = A + '/evidence/equiv/equiv_full_old_v_new.tsv'; eqc = A + '/evidence/equiv/equiv_control_new_v_planted_mirror.tsv'
eq_ok = 'rows checked' in open(A + '/work/equiv_full.log').read() and open(eqf).read().strip() == ''
ctl = [l.split('\t') for l in open(eqc).read().splitlines() if l.strip()]
ctl_ok = 'rows checked' in open(A + '/work/equiv_ctl.log').read() and sorted((c[0], c[1], c[2]) for c in ctl) == sorted((a, 'src', 'DIFF') for a in ROWS)
print('equiv full ok', eq_ok, '| planted control ok', ctl_ok, len(ctl))
res = ['address\tverdict\told\tnew\treason']; pay = json.load(open(A + '/work/pay_b1_H_payload.json'))
for a in ROWS:
    src = nf.src_of(a).read_text(encoding='utf-8')
    assert open(A + '/orig/rows/%s.cpp' % a, encoding='utf-8').read() == src, 'stale orig ' + a
    assert open(A + '/orig/tests/%s_test.cpp' % a, encoding='utf-8').read() == nf.test_of(a).read_text(encoding='utf-8'), 'stale test ' + a
    shutil.copy(A + '/rows/%s.cpp' % a, B + '/rows/%s.cpp' % a); shutil.copy(A + '/orig/rows/%s.cpp' % a, B + '/orig/rows/%s.cpp' % a)
    shutil.copy(A + '/orig/tests/%s_test.cpp' % a, B + '/orig/tests/%s_test.cpp' % a)
    for k in ('H', 'N'):
        for e in ('pkall.tsv', 'pvr.txt'): shutil.copy(A + '/evidence/pk_gate/%s_%s.%s' % (k, a, e), B + '/evidence/pk_gate/%s_%s.%s' % (k, a, e))
    shutil.copy(A + '/evidence/objsec/%s.txt' % a, B + '/evidence/objsec/%s.txt' % a)
    vh, vn = vl.get(('H', a), '-'), vl.get(('N', a), '-')
    ok = eq_ok and ctl_ok and gate.get(a) == 'OK' and prag.get(a) == 'OK' and pk_ok and rf_ok and vh == vn and vn.endswith('PASS') and vn.split()[0] in ('MATCH', 'RELOCATION_MATCH')
    leaf = nf.leaf_of(src, a)
    res.append('\t'.join([a, 'ACCEPT' if ok else 'HOLD', leaf, leaf,
        ('Fixes the Reset<NUISystem::CComponent> pick-any copy 0x00429C6F (62 -> 59 B, EQ under both checkers, bytes + relocations equal to the owner object) with the '
         'new add-only opt-in FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY_YOFF_TU (one added #define); pragscan: state before the block == state it restores '
         '(y off, s on), end-of-TU state unchanged; objsec: exactly one COMDAT changes, leaf identical, no symbol delta; 0 turned / new NEQ, 0 reach flips; vland %s HEAD and new. '
         'NEEDS hdr/engine/CCountedPointer.h applied first' % vn) if ok else 'gate failed: equiv=%s control=%s check364=%s pragscan=%s pk=%s reachflip=%s vland %s -> %s' % (eq_ok, ctl_ok, gate.get(a), prag.get(a), pk_ok, rf_ok, vh, vn)]))
open(B + '/RESULTS.tsv', 'w').write('\n'.join(res) + '\n'); open(B + '/rows.txt', 'w').write(''.join(a + '\n' for a in ROWS))
open(B + '/head/HEAD_FROM_REPO.txt', 'w').write(''.join(a + '\n' for a in ROWS))
json.dump(pay, open(B + '/head/payload.json', 'w'), indent=1)
for f in ('check364.txt', 'pragscan364.txt', 'pk_gate.txt', 'pk_gate_control_reversed.txt', 'reachflip.txt'): shutil.copy(A + '/evidence/' + f, B + '/evidence/' + f)
if os.path.isdir(A + '/evidence/equiv'): shutil.copytree(A + '/evidence/equiv', B + '/evidence/equiv', dirs_exist_ok=True)
print('MKBATCH364', B, sum(1 for r in res[1:] if '\tACCEPT\t' in r), 'ACCEPT of', len(ROWS))
