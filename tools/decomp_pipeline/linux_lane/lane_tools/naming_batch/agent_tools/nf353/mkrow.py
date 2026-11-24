# mkrow.py <maps.tsv> <addr...>: write rows/<a>.cpp = the landed source with the scaffolding block inserted right before the first
# `#include` other than "fable_compiler.h" (so the define precedes every header, stlport/fable_map.h included), and copy the landed
# source / test to orig/rows, orig/tests. <maps.tsv>: addr<TAB>extra-map text (the other map<> operator[] copies the opt-in also
# keeps declaration-only, measured from the row's HEAD object; '-' for none). Agent NF353.
import sys, os, re, shutil
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A)
extra = {l.split('\t')[0]: l.rstrip('\n').split('\t')[1] for l in open(sys.argv[1]) if '\t' in l}
for a in sys.argv[2:]:
    sp = nf.src_of(a); t = sp.read_text(encoding='utf-8')
    assert 'FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY' not in t, a
    L = t.split('\n')
    k = next(i for i, l in enumerate(L) if re.match(r'\s*#\s*include\s', l) and 'fable_compiler.h' not in l)
    ex = extra.get(a, '-')
    blk = ['// Fidelity: scaffolding, not retail (NF353 family port, T3 nf340 0x00CD3D2E): FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY keeps map::operator[]',
           '//   declaration-only, so this TU no longer emits its map<CCharString,CScriptGameResourceObjectScriptedThingBase>::operator[] copy',
           '//   (NEQ to retail 0x00CD3D2E: retail\'s is size-compiled, 118 bytes); the leaf is unchanged and calls the external symbol, which the',
           '//   landed owner row 0x00CD3D2E defines under the same four-argument STLport spelling (NF350 respell).']
    if ex != '-': blk.append('//   It also keeps %s declaration-only.' % ex)
    blk.append('#define FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY')
    open(A + '/rows/%s.cpp' % a, 'w', encoding='utf-8', newline='').write('\n'.join(L[:k] + blk + L[k:]))
    shutil.copy(sp, A + '/orig/rows/%s.cpp' % a)
    tp = nf.test_of(a)
    if tp and tp.exists(): shutil.copy(tp, A + '/orig/tests/%s_test.cpp' % a)
    print(a, 'inserted before line', k + 1, L[k][:80])
