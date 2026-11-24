# mkbatch366.py <batch dir>: the land_scripted run dir for NF366's seven in-place DeleteData self respells (one dir; no header, test or table
# change): rows/<a>.cpp, rows.txt, orig/rows + orig/tests (landed HEAD text, asserted current), head/payload.json (HEAD payload, no prose
# HEAD_FROM_REPO), RESULTS.tsv, TABLE_CHANGES / RENAMES / NAME_FIXES (lines for the rows), pk gate texts + controls, evidence copies,
# logs/ (vland HEAD / new). Retail bytes (the vland oracle) stay in work/.
import sys, os, shutil, json, glob
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); B = sys.argv[1]
ROWS = ['00447660', '00a28370', '00a281e0', '00aa4310', '00b3e080', '00cd42e0', '00f38920']
M = json.load(open(A + '/work/mangle.json'))
for d in ('rows', 'orig/rows', 'orig/tests', 'head', 'logs', 'evidence/pk_gate', 'evidence/objsec', 'evidence/link', 'variants'): os.makedirs(B + '/' + d, exist_ok=True)
p = json.load(open(A + '/work/pay_b1_H_payload.json'))
for x in p['result']['authored']: assert x['source_cpp'] == nf.src_of(x['address']).read_text(encoding='utf-8'), x['address']
json.dump(p, open(B + '/head/payload.json', 'w'), indent=1)
pn = json.load(open(A + '/work/pay_b1_N_payload.json'))
for x in pn['result']['authored']: assert x['source_cpp'] == open(A + '/rows/%s.cpp' % x['address']).read(), 'stale new payload ' + x['address']
dupe = {'00a281e0': '0x00A24520 (C3DMeshLODInfo::BuildMesh, pick-any sel 2, body EQUAL: 11 B, 0 relocations)',
        '00f38920': '0x00F38990 (CSurfaceCompositor::CSurfaceCompositor, pick-any sel 2, body EQUAL: 11 B, 0 relocations)'}
res = ['address\tverdict\told\tnew\treason']
for a in ROWS:
    shutil.copy('%s/rows/%s.cpp' % (A, a), B + '/rows/')
    o = open('%s/orig/rows/%s.cpp' % (A, a)).read(); assert o == nf.src_of(a).read_text(encoding='utf-8'), 'stale orig ' + a
    shutil.copy('%s/orig/rows/%s.cpp' % (A, a), B + '/orig/rows/')
    t = open('%s/orig/tests/%s_test.cpp' % (A, a)).read(); assert t == nf.test_of(a).read_text(encoding='utf-8'), 'stale test ' + a
    shutil.copy('%s/orig/tests/%s_test.cpp' % (A, a), B + '/orig/tests/')
    for k in ('H', 'N'):
        for x in ('pkall.tsv', 'pvr.txt'): shutil.copy('%s/evidence/pk_gate/%s_%s.%s' % (A, k, a, x), B + '/evidence/pk_gate/')
    for f in glob.glob('%s/evidence/objsec/%s*.txt' % (A, a)): shutil.copy(f, B + '/evidence/objsec/')
    shutil.copy('%s/evidence/link_%s.txt' % (A, a), B + '/evidence/link/')
    for f in glob.glob('%s/var/%s_fwd.cpp' % (A, a)): shutil.copy(f, B + '/variants/')
    new = '?DeleteData@?$CCountedPointer@%s@@@@SIXPAX@Z' % M[a]
    inc = '#include "engine/CCountedPointer.h"' in open('%s/rows/%s.cpp' % (A, a)).read() and 'class ' not in open('%s/rows/%s.cpp' % (A, a)).read().split('#include "engine/CCountedPointer.h"')[1].split('struct Obj')[0]
    form = 'T declared by its shared header (include variant objsec OTHER_SAME, .drectve equal; forward-declaration variant in variants/)' if inc else 'T forward-declared (no shared class header)'
    link = ('link owner + binders: 1 new LNK2005 against the EQUAL-body pick-any copy in ' + dupe[a]) if a in dupe else 'link owner + binders: 0 duplicates'
    res.append('%s\tACCEPT\t@DeleteIfSet@4\t%s\tin-place self respell onto the PDB name in the landed 0x00428B10 form (explicit specialization under the landed DELETEDATA_DECL_ONLY opt-in, HEAD body over its local slot-0 model; %s); vland MATCH PASS H and N (the "leaf matches no symbol" warning gone); objsec OTHER_SAME symdelta 0 (sel 1 both sides, .drectve and UND equal; the object\'s one DEF is the Leaf); pk_all + pvr SAME, GATE PASS, 0 reach flips; drop test clean (no table line, no UND binder of @DeleteIfSet@4); %s; the Reset binder binds the stand-in ?Destroy_%s@@YIXPAX@Z, unresolved at HEAD and after (NAME_FIXES)' % (a, new, form, link, a))
open(B + '/RESULTS.tsv', 'w').write('\n'.join(res) + '\n')
open(B + '/rows.txt', 'w').write('\n'.join(ROWS) + '\n')
for f in ('RENAMES.tsv', 'NAME_FIXES.tsv', 'TABLE_CHANGES.tsv'):
    L = open(A + '/' + f).read().splitlines(); body = [l for l in L[1:] if any(a in l.split('\t')[:2] for a in ROWS)]
    open(B + '/' + f, 'w').write('\n'.join([L[0]] + body) + '\n')
shutil.copy(A + '/work/vland_b1_H.log', B + '/logs/recheck_head.log'); shutil.copy(A + '/work/vland_b1_N.log', B + '/logs/recheck_new.log')
for f in ('pk_gate.txt', 'pk_gate_control_reversed.txt', 'pk_gate_control_planted.txt'): shutil.copy(A + '/evidence/' + f, B + '/' + f)
for f in ('reachflip.txt', 'symscan_controls.txt', 'cmpcomdat_c3dmesh2.txt', 'cmpcomdat_crendertarget.txt', 'xsites.txt', 'reset_callers.txt', 'identity.tsv', 'droptest.txt', 'pdb_exact.txt', 'locks_final.txt'):
    if os.path.exists(A + '/evidence/' + f): shutil.copy(A + '/evidence/' + f, B + '/evidence/')
shutil.copy(A + '/evidence/link_all7.txt', B + '/evidence/link/')
print('BATCH_OK', B, len(ROWS))
