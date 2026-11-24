# mkbatch363_b1.py: NF363 Part 1 land_scripted run dir batches/b1 (0x0042AF3C, in-place self respell, NF360 b1 text plus the R2 Fidelity
# marker and the stale-comment fix). Asserts the HEAD payload / orig copies equal the landed files and the new payload equals rows/.
import sys, os, shutil, json, glob
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); R = '/home/user/FableDecomp'; a = '0042af3c'; B = A + '/batches/b1'
NF360 = R + '/rebuild/candidates/manual/pending/2026-11-24_nf360_handback/batches/b1'
for d in ('rows', 'orig/rows', 'orig/tests', 'head', 'logs', 'evidence/pk_gate', 'evidence/pk_gate_planted', 'evidence/objsec', 'evidence/link', 'evidence/copiers'): os.makedirs(B + '/' + d, exist_ok=True)
p = json.load(open(A + '/work/pay_b1_H_payload.json')); e = p['result']['authored']
assert len(e) == 1 and e[0]['address'] == a and e[0]['source_cpp'] == nf.src_of(a).read_text(encoding='utf-8') and e[0]['test_cpp'] == nf.test_of(a).read_text(encoding='utf-8')
json.dump(p, open(B + '/head/payload.json', 'w'), indent=1)
pn = json.load(open(A + '/work/pay_b1_N_payload.json'))['result']['authored'][0]
assert pn['source_cpp'] == open(A + '/rows/%s.cpp' % a, encoding='utf-8').read(), 'new payload stale'
shutil.copy(A + '/rows/%s.cpp' % a, B + '/rows/')
assert open(A + '/orig/rows/%s.cpp' % a, encoding='utf-8').read() == nf.src_of(a).read_text(encoding='utf-8')
assert open(A + '/orig/tests/%s_test.cpp' % a, encoding='utf-8').read() == nf.test_of(a).read_text(encoding='utf-8')
shutil.copy(A + '/orig/rows/%s.cpp' % a, B + '/orig/rows/'); shutil.copy(A + '/orig/tests/%s_test.cpp' % a, B + '/orig/tests/')
for f in glob.glob(A + '/evidence/pk_gate/*_%s.*' % a): shutil.copy(f, B + '/evidence/pk_gate/')
for f in glob.glob(A + '/evidence/pk_gate_planted/*_%s.*' % a): shutil.copy(f, B + '/evidence/pk_gate_planted/')
shutil.copy(A + '/evidence/pk_gate.txt', B + '/pk_gate.txt')
shutil.copy(A + '/evidence/pk_gate_control_reversed.txt', B + '/pk_gate_control_reversed.txt')
shutil.copy(A + '/evidence/pk_gate_control_planted.txt', B + '/pk_gate_control_planted.txt')
for f in ('reachflip.txt', 'reachflip_control_planted.txt', 'droptest_0042af3c.tsv', 'copiers363.tsv', 'copiers_groups.txt'): shutil.copy(A + '/evidence/' + f, B + '/evidence/')
for f in glob.glob(A + '/evidence/objsec/0042af3c*.txt'): shutil.copy(f, B + '/evidence/objsec/')
for f in glob.glob(A + '/evidence/link_af3c_*.txt'): shutil.copy(f, B + '/evidence/link/')
for f in glob.glob(A + '/evidence/copiers/*.tsv'): shutil.copy(f, B + '/evidence/copiers/')
shutil.copy(A + '/work/vland_b1_H.log', B + '/logs/recheck_head.log'); shutil.copy(A + '/work/vland_b1_N.log', B + '/logs/recheck_new.log')
for f in ('TABLE_CHANGES.tsv', 'RENAMES.tsv', 'NAME_FIXES.tsv'): shutil.copy(NF360 + '/' + f, B + '/' + f)
old = '?GetDef_0042af3c@CDefinitionManager@@QAE_NJAAV?$CDefPointer@$$CBVCDefClassBase@@@@@Z'
new = nf.leaf_of(open(A + '/rows/%s.cpp' % a, encoding='utf-8').read(), a)
reason = ('in-place self respell onto the PDB name, strong explicit specialization (0x0042B040 form; ruling R1), NF360 b1 text plus ruling R2 '
          '`// Fidelity: scaffolding, not retail` on the reinterpret_cast (with its reason) and the stale "def type is opaque" sentence fixed; '
          'regraded from scratch: vland --recheck-landed RELOCATION_MATCH PASS HEAD and new (STANDIN_LEAF / leaf-matches-no-symbol warnings gone); '
          'objsec OTHER_SAME symdelta 0 (leaf bytes / relocs / characteristics equal, sel 1 both, .drectve and UND equal; also OTHER_SAME v NF360 new '
          'object); pk_all + pvr SAME / GATE PASS, reversed PASS, planted FAIL as it must; reach flips 0; drop test: only the stand-in self, 0 table '
          'lines, no landed binder; link owner + 0x004CA010: PDB name unresolved at HEAD, resolved new, 0 duplicates, unresolved sets otherwise equal')
open(B + '/RESULTS.tsv', 'w').write('address\tverdict\told\tnew\treason\n' + '\t'.join([a, 'ACCEPT', old, new, reason]) + '\n')
open(B + '/rows.txt', 'w').write(a + '\n')
print('MKBATCH_DONE', B)
