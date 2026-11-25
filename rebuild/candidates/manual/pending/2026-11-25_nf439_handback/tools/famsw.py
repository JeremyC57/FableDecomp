#!/usr/bin/env python3
"""famsw.py <batch> <addr...>: the CThingSwitch scalar-deleting-destructor family respell: the free stand-in
?AutoTinySecondMemberDestructor@@YIXPAX@Z (retail call target 0x00CE1000 in every member) -> the qualified call of
??1?$CCountedPointer@VCEntityScriptBase@@@@QAE@XZ under FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY. Writes rows/ and orig/ copies."""
import sys, os, shutil, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
b = sys.argv[1]; B = str(nf.A) + '/batches/' + b
for d in ('rows', 'orig/rows', 'orig/tests', 'tests'): os.makedirs(B + '/' + d, exist_ok=True)
NOTE = ('// Naming (NF423, 2026-11-25): the member release 0x00CE1000 now binds its PDB spelling ??1?$CCountedPointer@VCEntityScriptBase@@@@QAE@XZ\n'
        '//   (FableWin.pdb name set; the self row\'s Leaf; retail calls 0x00CE1000 here), called on the +0x34 member through the shared\n'
        '//   engine/CCountedPointer.h under FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY (declaration only: retail calls the out-of-line COMDAT);\n'
        '//   was the free stand-in ?AutoTinySecondMemberDestructor@@YIXPAX@Z. Object otherwise unchanged.\n')
for a in sys.argv[2:]:
    s = nf.src_of(a); t = nf.test_of(a)
    shutil.copy(s, B + '/orig/rows/%s.cpp' % a)
    if t and t.exists(): shutil.copy(t, B + '/orig/tests/%s_test.cpp' % a)
    src = open(s, encoding='utf-8').read()
    o1 = '#include "engine/CCountedPointer.h"\n'
    if src.count(o1) != 1: print('SHAPE?', a, 'include'); continue
    src = src.replace(o1, NOTE + '#define FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY\n#include "engine/CCountedPointer.h"\nclass CEntityScriptBase;\n')
    o2 = 'extern void __fastcall AutoTinySecondMemberDestructor(void* member);\n'
    if src.count(o2) != 1: print('SHAPE?', a, 'extern'); continue
    src = src.replace(o2, '')
    m = re.findall(r'    AutoTinySecondMemberDestructor\((static_cast<unsigned char\*>\(self\) \+ 52)\);', src)
    if len(m) != 1: print('SHAPE?', a, 'call'); continue
    src = src.replace('    AutoTinySecondMemberDestructor(%s);' % m[0], '    reinterpret_cast<CCountedPointer<CEntityScriptBase>*>(%s)->CCountedPointer<CEntityScriptBase>::~CCountedPointer();' % m[0])
    open(B + '/rows/%s.cpp' % a, 'w', encoding='utf-8').write(src)
    print('OK', a)
