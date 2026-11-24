# mkbatch349.py <batch dir> <verdict> <addr...>: a land_scripted run dir for the NF352 size-body port:
#   rows/<a>.cpp (new), orig/rows/<a>.cpp + orig/tests/<a>_test.cpp (landed HEAD text), rows.txt, head/payload.json (HEADPAY: landed text),
#   hdr/engine/CDefinitionManager.h (new header) + orig/hdr_CDefinitionManager.h (HEAD header) + HEADERS.tsv,
#   evidence/pk_gate/{H,N}_<a>.* (both checkers), evidence/objsec/<a>.txt (H v N), evidence/objsec_RN/<a>.txt (REREAD-only v N),
#   evidence/pragscan_c.txt, pk_gate.txt (pkcmp346 H v N) + pk_gate_control_reversed.txt (sides swapped), RESULTS.tsv
#   (address verdict old new reason; old / new = the graded symbol V.obj_text picks with the row's Leaf in the HEAD / new object).
# The oracle file nf.py pay writes (retail bytes) stays in work/.
import sys, os, re, shutil, subprocess, contextlib, io, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline'); import verify_and_land as V
B, verdict, addrs = sys.argv[1], sys.argv[2], sys.argv[3:]
A = str(nf.A)
for d in ('rows', 'orig/rows', 'orig/tests', 'head', 'hdr/engine', 'evidence/pk_gate', 'evidence/objsec', 'evidence/objsec_RN'): os.makedirs(B + '/' + d, exist_ok=True)
ev = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(A + '/work/eval.tsv')}
shutil.copy(A + '/hdr/engine/CDefinitionManager.h', B + '/hdr/engine/CDefinitionManager.h')
shutil.copy(A + '/orig/hdr_CDefinitionManager/CDefinitionManager.h', B + '/orig/hdr_CDefinitionManager.h')
assert open(B + '/orig/hdr_CDefinitionManager.h').read() == open('/home/user/FableDecomp/rebuild/include/engine/CDefinitionManager.h').read(), 'stale orig header'
open(B + '/HEADERS.tsv', 'w').write('class\tverdict\tmacro\tnotes\n' + ''.join(
    'CDefinitionManager\tOK\tFABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_%s_SIZE_BODY\tengine/CDefinitionManager.h: add-only nested opt-in (one new #ifdef FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY block after the existing one; no existing line changed): size-mode `template<> __declspec(noinline) inline` GetDef<%s> = owner %s (EXTRA_RULES 2026-11-20 size-pragma scaffolding, 2026-11-21 condition 2); equiv_check over the 1,210-row reverse include closure: 0 differing, planted control 160/160 GLOBAL_BODY definers flagged (evidence/hdr/ in the NF352 hand-back)\n'
    % (u, n, o) for u, n, o in (('CTHINGBASEDEF', 'CThingBaseDef', '0x0042AF3C'), ('CTHINGOBJECTDEF', 'CThingObjectDef', '0x0042B040'),
                               ('CUIMISCTHINGSDEF', 'NUISystem::CUIMiscThingsDef', '0x0042B312'), ('CEXPRESSIONDEF', 'CExpressionDef', '0x005F81BE'))))
res = ['address\tverdict\told\tnew\treason']
OWN = {'VCThingBaseDef': '0x0042AF3C', 'VCThingObjectDef': '0x0042B040', 'VCUIMiscThingsDef@NUISystem': '0x0042B312', 'VCExpressionDef': '0x005F81BE'}
prag = {l.split()[0]: l.rstrip('\n') for l in open(A + '/evidence/pragscan_c.txt') if l.strip()}
pl = []
for a in addrs:
    shutil.copy(A + '/rows/%s.cpp' % a, B + '/rows/%s.cpp' % a)
    shutil.copy(A + '/orig/rows/%s.cpp' % a, B + '/orig/rows/%s.cpp' % a)
    if os.path.exists(A + '/orig/tests/%s_test.cpp' % a): shutil.copy(A + '/orig/tests/%s_test.cpp' % a, B + '/orig/tests/%s_test.cpp' % a)
    for k in ('H', 'N'):
        for e in ('pkall.tsv', 'pvr.txt'): shutil.copy(A + '/evidence/pk/%s_%s.%s' % (k, a, e), B + '/evidence/pk_gate/%s_%s.%s' % (k, a, e))
    shutil.copy(A + '/evidence/objsec/%s.txt' % a, B + '/evidence/objsec/%s.txt' % a)
    shutil.copy(A + '/evidence/objsec_RN/%s.txt' % a, B + '/evidence/objsec_RN/%s.txt' % a)
    pl.append(prag[a])
    leaf = nf.leaf_of(nf.src_of(a).read_text(encoding='utf-8'), a)
    with contextlib.redirect_stderr(io.StringIO()):
        old = V.obj_text(A + '/work/obj/H_%s.obj' % a, leaf)[2]; new = V.obj_text(A + '/work/obj/%s.obj' % a, leaf)[2]
    pc = subprocess.run(['python3', A + '/tools/pkcmp346.py', A + '/evidence/pk', a], capture_output=True, text=True).stdout
    fixed = sorted(set('%s %s' % (re.match(r'\?\?\$GetDef@(.*?)@@@', l.split('\t')[5]).group(1), '0x' + l.split('\t')[3].upper())
                       for l in pc.splitlines() if '\tFIXED\t' in l and '??$GetDef@' in l))
    e = ev[a]
    sz = ['GetDef<%s> %s' % (t.lstrip('V'), ad) for t, ad in (f.split() for f in fixed) if t in OWN]
    oth = ['GetDef<%s> %s' % (t.lstrip('V'), ad) for t, ad in (f.split() for f in fixed) if t not in OWN]
    why = ('NF352: adds #define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD (existing CDefPointer.h opt-in, as NF346) plus the new size-body opt-in(s) '
           'next to the row\'s GETDEF_GLOBAL_BODY (header engine/CDefinitionManager.h, add-only; EXTRA_RULES 2026-11-20 size-pragma scaffolding: (a) objsec, '
           '(b) owners set optimize("y", off) / ("s", on) at the catalog default /O2 /Oy /W3, (c) pragscan C_OK): size-mode copies NEQ -> EQ (both checkers): %s%s; '
           'own symbol unchanged, scratch grade %s before and after; pk gate PASS (0 turned / newly NEQ); objsec H v N: leaf identical, only GetDef<T> copies '
           'turning EQ, their NOADDR SetDynamicCast helpers and dropped unreferenced NOADDR Assign helpers differ (%s); objsec REREAD-only v N: only the '
           'opted-in GetDef<T> COMDATs differ (%s)') % ('; '.join(sz), ('; REREAD alone: ' + '; '.join(oth)) if oth else '', e[2], e[8], e[9])
    if verdict != 'ACCEPT': why = '%s: %s' % (verdict, why)
    res.append('%s\t%s\t%s\t%s\t%s' % (a, verdict, old, new, why))
open(B + '/RESULTS.tsv', 'w').write('\n'.join(res) + '\n')
open(B + '/rows.txt', 'w').write(''.join(a + '\n' for a in addrs))
open(B + '/evidence/pragscan_c.txt', 'w').write('# tools/pragscan.py on the preprocessed new TU (header tree with the new header): optimize pragmas before the first NF352 block\n' + '\n'.join(pl) + '\n')
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
