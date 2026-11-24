# mkbatch363_b2.py: NF363 Part 2 land_scripted run dir batches/b2 (NF359 option A, the Alloc form, for 0x00455EF0 / 0x0044C1BB /
# 0x0045C5DA, one unit). Asserts the HEAD payload / orig copies equal the landed files and the new payload equals rows/ + tests/.
import sys, os, shutil, json, glob
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); B = A + '/batches/b2'; ROWS = ['00455ef0', '0044c1bb', '0045c5da']
for d in ('rows', 'tests', 'orig/rows', 'orig/tests', 'head', 'logs', 'evidence/pk_gate', 'evidence/pk_gate_planted', 'evidence/link', 'evidence/tests'): os.makedirs(B + '/' + d, exist_ok=True)
p = json.load(open(A + '/work/pay_b2_H_payload.json'))
assert [x['address'] for x in p['result']['authored']] == ROWS
for x in p['result']['authored']:
    assert x['source_cpp'] == nf.src_of(x['address']).read_text(encoding='utf-8') and x['test_cpp'] == nf.test_of(x['address']).read_text(encoding='utf-8')
json.dump(p, open(B + '/head/payload.json', 'w'), indent=1)
pn = {x['address']: x for x in json.load(open(A + '/work/pay_b2_N_payload.json'))['result']['authored']}
for a in ROWS:
    assert pn[a]['source_cpp'] == open(A + '/rows/%s.cpp' % a, encoding='utf-8').read(), 'stale new source ' + a
    assert pn[a]['test_cpp'] == open(A + '/rows/%s_test.cpp' % a, encoding='utf-8').read(), 'stale new test ' + a
    shutil.copy(A + '/rows/%s.cpp' % a, B + '/rows/'); shutil.copy(A + '/rows/%s_test.cpp' % a, B + '/tests/')
    assert open(A + '/orig/rows/%s.cpp' % a, encoding='utf-8').read() == nf.src_of(a).read_text(encoding='utf-8')
    assert open(A + '/orig/tests/%s_test.cpp' % a, encoding='utf-8').read() == nf.test_of(a).read_text(encoding='utf-8')
    shutil.copy(A + '/orig/rows/%s.cpp' % a, B + '/orig/rows/'); shutil.copy(A + '/orig/tests/%s_test.cpp' % a, B + '/orig/tests/')
    for f in glob.glob(A + '/evidence/pk_gate2/*_%s.*' % a): shutil.copy(f, B + '/evidence/pk_gate/')
    for f in glob.glob(A + '/evidence/pk_gate2_planted/*_%s.*' % a): shutil.copy(f, B + '/evidence/pk_gate_planted/')
    shutil.copy(A + '/evidence/droptest_%s.tsv' % a, B + '/evidence/')
shutil.copy(A + '/evidence/pk_gate2.txt', B + '/pk_gate.txt')
shutil.copy(A + '/evidence/pk_gate2_control_reversed.txt', B + '/pk_gate_control_reversed.txt')
shutil.copy(A + '/evidence/pk_gate2_control_planted.txt', B + '/pk_gate_control_planted.txt')
for f in ('reachflip2.txt', 'setdiff_v_0045c5f6.txt', 'setdiff_head_v_new.txt', 'retcmp_part2.txt', 'vtcmp_part2.txt', 'symscan_part2.txt',
          'cmpcomdat_unit.txt', 'definers_unit.txt', 'definers_census.txt', 'maplink_new_749810_first.txt'): shutil.copy(A + '/evidence/' + f, B + '/evidence/')
for f in glob.glob(A + '/evidence/link_unit*.txt'): shutil.copy(f, B + '/evidence/link/')
for f in ('rt_r1.log', 'rt_m1.log'): shutil.copy(A + '/work/' + f, B + '/evidence/tests/')
for f in glob.glob(A + '/work/tst/mut_*_test.cpp'): shutil.copy(f, B + '/evidence/tests/')
shutil.copy(A + '/work/vland_b2_H.log', B + '/logs/recheck_head.log'); shutil.copy(A + '/work/vland_b2_N.log', B + '/logs/recheck_new.log')
old = {'00455ef0': '??0CThingNoiseDef@@QAE@XZ', '0044c1bb': '??0CTCNoiseDef@@QAE@XZ', '0045c5da': '?CThingNoiseDef_ScalarDeletingDestructor@@YIPAXPAXHI@Z'}
common = ('NF359 option A (Alloc form, 0x004304E0 precedent; advisor ruling, owner delegated): the landed 0x0045C5F6 code lines with this row\'s Leaf '
          'and comment block; object section-for-section identical to a fresh HEAD 0x0045C5F6 object (16 sections SAME per object, 48 over three; DEF / UND equal); '
          'vland --recheck-landed RELOCATION_MATCH PASS HEAD and new, the new test on the shared headers PASS (its mutant FAILs); every function '
          'COMDAT EQ to retail by retcmp (bytes + relocation targets, 8 of 8), both vftables 25 slots slot for slot (slot 0 the ??_E alias of ??_G) '
          'and ??_7CDefPointeeBase 10 slots; both checkers GATE PASS (only EQ copies ADDED), reversed PASS, planted FAIL as it must, reach flips 0; '
          'drop test clean (only UND stand-ins and, for 0x0045C5DA, the stand-in self are dropped; no landed object references them); unit link: '
          '3 LNK2005 on ?Alloc@CThingNoiseDef, byte- and relocation-identical, sel 1 (the nf184 pattern); unresolved set only shrinks')
spec = {'00455ef0': '; selection 1 -> 2 (the strong stand-in ctor becomes the pick-any COMDAT); table delete ??0CMemberBase_0044c0c0 (witness this row)',
        '0044c1bb': '; selection 1 -> 2 (the strong stand-in ctor becomes the pick-any COMDAT)',
        '0045c5da': '; self respelled ?CThingNoiseDef_ScalarDeletingDestructor@@YIPAXPAXHI@Z -> ??_GCThingNoiseDef@@UAEPAXI@Z (vland WARN STANDIN_LEAF is the '
                    'standin_leaf.py underscore gap: it decodes `scalar deleting\' but not the catalog\'s `scalar_deleting_destructor\')'}
L = ['address\tverdict\told\tnew\treason']
for a in ROWS:
    new = nf.leaf_of(open(A + '/rows/%s.cpp' % a, encoding='utf-8').read(), a)
    L.append('\t'.join([a, 'ACCEPT', old[a], new, common + spec[a]]))
open(B + '/RESULTS.tsv', 'w').write('\n'.join(L) + '\n')
open(B + '/rows.txt', 'w').write(''.join(a + '\n' for a in ROWS))
open(B + '/UNITS.tsv', 'w').write('address\tunit\n' + ''.join('%s\tnf363_noisedef\n' % a for a in ROWS))
open(B + '/TABLE_CHANGES.tsv', 'w').write('action\tsymbol\ttarget\twitness\tnote\n'
    'delete\t??0CMemberBase_0044c0c0@@QAE@XZ\t0044c0c0\t00455ef0\tNF363 drop test: the new 0x00455EF0 object no longer names it (its ctor call binds ??0CSubDefClassBase@@QAE@XZ, '
    'line witnessed by 0044c57d); the only landed source and only witness was 0x00455EF0 (source grep: 1 file); scratch census: UND only in 0x00455EF0 objects\n')
open(B + '/RENAMES.tsv', 'w').write('kind\trow\told\tnew\tbatch\n'
    'self\t0045c5da\t?CThingNoiseDef_ScalarDeletingDestructor@@YIPAXPAXHI@Z\t??_GCThingNoiseDef@@UAEPAXI@Z\tb2 (self symbol; Source / Module / catalog name unchanged; rebuild/oracles/self_symbols.tsv lines 1210 and 27219 follow at the next self_symbols.py pass)\n'
    'callee\t00455ef0\t??0CMemberBase_0044c0c0@@QAE@XZ\t??0CSubDefClassBase@@QAE@XZ\tb2 (0x0044C0C0; the PDB name other rows bind)\n'
    'callee\t0044c1bb\t??0CShoveHaPennyRegionDef@@QAE@XZ\t??0CSubDefClassBase@@QAE@XZ\tb2 (0x0044C0C0; 126 other sources keep the stand-in, NAME_FIXES_QUEUE land1120nf190)\n')
open(B + '/NAME_FIXES.tsv', 'w').write('address\tcorrect_symbol\tevidence\twrong_spellings_seen\n')
print('MKBATCH_DONE', B)
