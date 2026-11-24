# mkbatch357.py <batch dir> <rowdir> <pkdir> <payH json> <addr>=<verdict>=<reason> ...: a land_scripted run dir for NF361:
#   rows/<a>.cpp (from <rowdir>), orig/rows + orig/tests (landed HEAD text, asserted equal to the repo), rows.txt, head/payload.json
#   (landed HEAD text), hdr/engine/{CDefinitionManager.h,CDefClassBase_GetSubDef.h} + orig/hdr_<name> + HEADERS.tsv, TABLE_CHANGES.tsv
#   (header only: no table change), evidence/pk_gate/{H,N}_<a>.* (both checkers), evidence/objsec/<a>.txt, evidence/leafrel/<a>.txt,
#   evidence/owner_cmp.tsv, evidence/pragscan_c.txt, pk_gate.txt (pkcmp346 H v N) + pk_gate_control_reversed.txt + reachflip.txt,
#   RESULTS.tsv (address verdict old new reason). Retail bytes (the vland oracle) stay in work/.
import sys, os, shutil, subprocess, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); R = '/home/user/FableDecomp'; T = A + '/tools'
B, rowdir, pkdir, payH = sys.argv[1:5]; specs = [x.split('=', 2) for x in sys.argv[5:]]
for d in ('rows', 'orig/rows', 'orig/tests', 'head', 'hdr/engine', 'evidence/pk_gate', 'evidence/objsec', 'evidence/leafrel'): os.makedirs(B + '/' + d, exist_ok=True)
for h in ('CDefinitionManager', 'CDefClassBase_GetSubDef'):
    shutil.copy('%s/hdr/engine/%s.h' % (A, h), '%s/hdr/engine/%s.h' % (B, h))
    shutil.copy('%s/orig/hdr_%s/%s.h' % (A, h, h), '%s/orig/hdr_%s.h' % (B, h))
    assert open('%s/orig/hdr_%s.h' % (B, h)).read() == open('%s/rebuild/include/engine/%s.h' % (R, h)).read(), 'stale orig header ' + h
cl = ('equiv_check over the reverse include closure (closure.py: 1,210 rows for CDefinitionManager.h, 199 for CDefClassBase_GetSubDef.h, a subset), '
      'HEAD tree v new tree: 0 differing rows; planted controls flag exactly the true includer sets (141 GETSUBDEF_BODY definer files in the '
      'CDefinitionManager.h plant; 195 files incl. 4 via engine/CTCObjectAugmentations.h in the CDefClassBase_GetSubDef.h plant; NF361 evidence/hdr/)')
dev = ('size-mode `template<> __declspec(noinline) inline` GetSubDef<%s> between optimize("y", off) / ("s", on) and optimize("", on) = owner %s, '
       'which sets the same two pragmas at the catalog default /O2 /Oy /W3 (EXTRA_RULES 2026-11-20 size-pragma scaffolding; NF349 / NF352 per-type form)')
H = ['class\tverdict\tmacro\tnotes',
     'CDefinitionManager\tOK\tFABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CAPPEARANCEDEF_SIZE_BODY\tengine/CDefinitionManager.h: add-only nested opt-in inside FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY, after the CCreatureDef block (no existing line changed): ' + dev % ('CAppearanceDef', '0x0042B0A2') + '; ' + cl,
     'CDefinitionManager\tOK\tFABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CWEAPONDEF_SIZE_BODY\tengine/CDefinitionManager.h: same place and form: ' + dev % ('CWeaponDef', '0x0042B1A6') + '; ' + cl,
     'CDefClassBase_GetSubDef\tOK\tFABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CWEAPONDEF_SIZE_BODY\tengine/CDefClassBase_GetSubDef.h: add-only block before the closing include guard (no existing line changed), the twin for a TU that takes this header\'s template (one macro per type selects the block of whichever header the TU uses; ruling Q3): ' + dev % ('CWeaponDef', '0x0042B1A6') + '; ' + cl,
     'CDefClassBase_GetSubDef\tOK\tFABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CAPPEARANCEDEF_SIZE_BODY\tengine/CDefClassBase_GetSubDef.h: same: ' + dev % ('CAppearanceDef', '0x0042B0A2') + '; ' + cl,
     'CDefClassBase_GetSubDef\tOK\tFABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CENTITYSOUNDDEF_SIZE_BODY\tengine/CDefClassBase_GetSubDef.h: same, keyed on the EXISTING CDefinitionManager.h macro (no landed user of that macro includes this header; ruling Q3): ' + dev % ('CEntitySoundDef', '0x0065F604') + '; ' + cl]
open(B + '/HEADERS.tsv', 'w').write('\n'.join(H) + '\n')
open(B + '/TABLE_CHANGES.tsv', 'w').write('action\tsymbol\ttarget\twitness\tnote\n')
res = ['address\tverdict\told\tnew\treason']; addrs = [a for a, _, _ in specs]
prag = {}
for f in ('pragscan_c_b1.txt', 'pragscan_c_b2.txt'):
    for l in open(A + '/evidence/' + f):
        if l.strip(): prag[l.split()[0]] = l.rstrip('\n')
oc = {l.split('\t')[0]: l for l in open(A + '/work/owner_cmp.tsv')}
for a, verdict, reason in specs:
    shutil.copy('%s/%s.cpp' % (rowdir, a), B + '/rows/%s.cpp' % a)
    open(B + '/orig/rows/%s.cpp' % a, 'w', encoding='utf-8').write(nf.src_of(a).read_text(encoding='utf-8'))
    open(B + '/orig/tests/%s_test.cpp' % a, 'w', encoding='utf-8').write(nf.test_of(a).read_text(encoding='utf-8'))
    for k in ('H', 'N'):
        for e in ('pkall.tsv', 'pvr.txt'): shutil.copy('%s/%s_%s.%s' % (pkdir, k, a, e), B + '/evidence/pk_gate/%s_%s.%s' % (k, a, e))
    shutil.copy(A + '/evidence/objsec/%s.txt' % a, B + '/evidence/objsec/%s.txt' % a)
    shutil.copy(A + '/evidence/leafrel/%s.txt' % a, B + '/evidence/leafrel/%s.txt' % a)
    leaf = nf.leaf_of(nf.src_of(a).read_text(encoding='utf-8'), a)
    res.append('\t'.join([a, verdict, leaf, leaf, reason]))
open(B + '/evidence/pragscan_c.txt', 'w').write(''.join(prag.get(a, a + ' -') + '\n' for a in addrs))
open(B + '/evidence/owner_cmp.tsv', 'w').write('addr\ttype\towner\tnew_obj\tcmpcomdat_v_owner_obj\tcomdat_flags_new\n' + ''.join(oc[a] for a in addrs))
pk = lambda env: subprocess.run(['python3', T + '/pkcmp346.py', pkdir] + addrs, capture_output=True, text=True, env=dict(os.environ, **env)).stdout
open(B + '/pk_gate.txt', 'w').write(pk({})); open(B + '/pk_gate_control_reversed.txt', 'w').write(pk({'PKCMP_REVERSE': '1'}))
# reach flips on exactly this batch's pairs
tmp = A + '/work/rf_' + os.path.basename(B.rstrip('/')); os.makedirs(tmp, exist_ok=True)
for a in addrs:
    for k in ('H', 'N'): shutil.copy('%s/%s_%s.pkall.tsv' % (pkdir, k, a), '%s/%s_%s.pkall.tsv' % (tmp, k, a))
open(B + '/evidence/reachflip.txt', 'w').write(subprocess.run(['python3', T + '/reachflip_coordinator.py', tmp], capture_output=True, text=True).stdout)
open(B + '/RESULTS.tsv', 'w').write('\n'.join(res) + '\n')
open(B + '/rows.txt', 'w').write(''.join(a + '\n' for a in addrs))
p = json.load(open(payH)); p['result']['authored'] = [x for x in p['result']['authored'] if x['address'] in addrs]
assert len(p['result']['authored']) == len(addrs)
json.dump(p, open(B + '/head/payload.json', 'w'), indent=1)
print('MKBATCH_DONE', B, len(specs))
