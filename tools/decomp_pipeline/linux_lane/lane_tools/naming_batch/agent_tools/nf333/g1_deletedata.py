# g1_deletedata.py: NF333 group 1 -- CCountedPointer DeleteData stand-in binders -> NF282 hybrid form
# (FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY + engine/CCountedPointer.h), as landed precedent 0x00AD65A0.
# Saves the HEAD copy to orig/<addr>.cpp and writes rows/<addr>.cpp.
import sys, re, shutil
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF333/tools'); import nf
G = {  # binder: (deleter addr, ns, class, pdb spelling)
 '00436cc0': ('00436ba0', None, 'CDisplayViewManager', '?DeleteData@?$CCountedPointer@VCDisplayViewManager@@@@SIXPAX@Z', 'both PDB name sets'),
 '004ae650': ('004ae510', None, 'CMusicManager', '?DeleteData@?$CCountedPointer@VCMusicManager@@@@SIXPAX@Z', 'both PDB name sets'),
 '006750d0': ('00674e80', None, 'CExpressionSet', '?DeleteData@?$CCountedPointer@VCExpressionSet@@@@SIXPAX@Z', 'FableWin.pdb name set'),
 '006d6260': ('006d6210', None, 'CBrainOutput', '?DeleteData@?$CCountedPointer@VCBrainOutput@@@@SIXPAX@Z', 'both PDB name sets'),
 '00a4ab60': ('00a4a910', None, 'CSpeechFrame', '?DeleteData@?$CCountedPointer@VCSpeechFrame@@@@SIXPAX@Z', 'FableWin.pdb name set'),
 '00a8d9f0': ('00a8d7c0', None, 'CScopedMemoryHandle', '?DeleteData@?$CCountedPointer@VCScopedMemoryHandle@@@@SIXPAX@Z', 'both PDB name sets'),
 '00aecac0': ('00aeca50', 'NParticleEngine', 'CParticleEmitterManager', '?DeleteData@?$CCountedPointer@VCParticleEmitterManager@NParticleEngine@@@@SIXPAX@Z', 'both PDB name sets'),
}
A = nf.A
for a in sys.argv[1:]:
    d, ns, cl, pdb, src = G[a]
    p = nf.src_of(a); t = p.read_text(encoding='utf-8')
    shutil.copy(p, A / 'orig' / (a + '.cpp'))
    q = ('%s::%s' % (ns, cl)) if ns else cl
    decl = 'void __fastcall Destroy_%s(void* object);   // 0x%s: the per-type deleter stored in the block\n' % (d, d.upper())
    assert t.count(decl) == 1, (a, 'decl')
    fwd = ('namespace %s { class %s; }' % (ns, cl)) if ns else ('class %s;' % cl)
    new = ('#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY\n#include "engine/CCountedPointer.h"\n%s\n' % fwd)
    t = t.replace(decl, new)
    use = 'm_pfnDestroy(Destroy_%s)' % d; assert t.count(use) == 1, (a, 'use')
    t = t.replace(use, 'm_pfnDestroy(&CCountedPointer<%s>::DeleteData)' % q)
    L = t.split('\n'); assert L[0].startswith('// CCountedPointer_%s::Reset' % d), (a, L[0])
    assert '// Leaf:' not in t
    L.insert(1, '// Leaf: ?Reset@CCountedPointer_%s@@QAEXPAX@Z' % d)
    k = L.index('#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY')
    L.insert(k, '// Naming (NF333, 2026-11-24): the count block\'s deleter 0x%s now binds its PDB spelling %s (%s;\n'
                    '//   retail stores 0x%s as the count block\'s DeleteFunc here), declared by the shared engine/CCountedPointer.h under FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY (only its\n'
                    '//   address is stored here, T is only declared); was the free stand-in ?Destroy_%s@@YIXPAX@Z. Object otherwise unchanged (NF282 hybrid form, precedent 0x00AD65A0).' % (d.upper(), pdb, src, d.upper(), d))
    (A / 'rows' / (a + '.cpp')).write_text('\n'.join(L), encoding='utf-8')
    print('OK', a)
