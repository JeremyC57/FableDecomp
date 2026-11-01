# h22_emit.py: naming wave 22, family emit -- DECISIONS_W21_RESOLVED "00c44f80 caller of CAmbientEmitter::ClearEvents" (per-row respell):
# the destructor 0x00C44F80 runs 0x00C44D30 on `this`, and 0x00C44D30 is the landed ?ClearEvents@CAmbientEmitter@@QAEXXZ (naming wave 21), so
# the class this destructor tears down is the CAmbientEmitter: the stand-in CGameFilters_00c44f80 is respelled CAmbientEmitter (CleanupB ->
# ClearEvents, CleanupA -> ReleaseSounds: the landed own row 0x00C44B80 ?ReleaseSounds@CAmbientEmitter@@QAEXXZ, whose header names this
# destructor as its only caller). The same object is the 0x8C-byte `new` of CResourceWorld::AllocateFor 0x00C36180 (ctor 0x00C44EB0) and the
# `delete` of CResourceWorld::FreeAllocation 0x00C36290, which called it CResourceAllocation (descriptive): one class, one name -- respelled
# CAmbientEmitter there too (the class of the landed own rows 0x00C44B80 / 0x00C44BD0 / 0x00C44CE0 / 0x00C44D30). Name-only.
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
a = '00c44f80'
he.edit(a, 'src', '    void CleanupB();                       // 0x00C44D30', '    void ClearEvents();                    // 0x00C44D30 (landed ?ClearEvents@CAmbientEmitter@@QAEXXZ)')
he.edit(a, 'src', '    CleanupB();\n', '    ClearEvents();\n')
he.edit(a, 'src', '    void CleanupA();                       // 0x00C44B80', '    void ReleaseSounds();                  // 0x00C44B80 (landed ?ReleaseSounds@CAmbientEmitter@@QAEXXZ)')
he.edit(a, 'src', '    CleanupA();\n', '    ReleaseSounds();\n')
he.edit(a, 'src', 'struct CGameFilters_00c44f80 {', 'class CAmbientEmitter { public:')
he.edit(a, 'src', 'struct CGameFilters_00c44f80;\n', 'class CAmbientEmitter;\n')
he.resub(a, 'src', r'\bCGameFilters_00c44f80\b', 'CAmbientEmitter')
p, rel, t = he._rw(a, 'src')
N = '// Naming (naming wave 22, DECISIONS_W21_RESOLVED): the class is the CAmbientEmitter whose ClearEvents 0x00C44D30 this destructor runs on `this` (was the stand-in CGameFilters_00c44f80).'
if N in t: print('already', a, 'note')
else:
    i = t.index('\n') + 1; he._w(p, t[:i] + N + '\n' + t[i:]); print('ok', a, 'note')

NR = '// Naming (naming wave 22): the allocation is the CAmbientEmitter (destructor 0x00C44F80 runs the landed CAmbientEmitter::ReleaseSounds 0x00C44B80 / ClearEvents 0x00C44D30 on it); was the descriptive CResourceAllocation.'
for a in ('00c36180', '00c36290'):
    he.edit(a, 'src', 'struct CResourceAllocation {', 'class CAmbientEmitter { public:')
    he.resub(a, 'src', r'\bCResourceAllocation\b', 'CAmbientEmitter')
    he.resub(a, 'tst', r'\bCResourceAllocation\b', 'CAmbientEmitter')
    p, rel, t = he._rw(a, 'src')
    if NR in t: print('already', a, 'note')
    else:
        i = t.index('\n') + 1; he._w(p, t[:i] + NR + '\n' + t[i:]); print('ok', a, 'note')
