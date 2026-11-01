# h17_w16d.py: naming wave 17, wave 16 DEFERRED per-row caller respells (name-only; the recheck proves the bytes):
#  008edff0: the CWillButtonHelperTimer ctor is ??0CWillButtonHelperTimer@@QAE@W4EHeroAbility@@AAVCThing@@@Z (c672): the callers
#    0x007A09B0 / 0x007C6760 / 0x007C87F0 modelled it as (long, CThing*). EHeroAbility enumerators have no ordinal source, so the
#    ability stays a cast of the landed value (as the wave 16 EPlayerMode decision); the thing is passed by reference.
#  00f37030: CGPUScratchBufferTextureManager::FreeMemoryPage takes a long page index (?FreeMemoryPage@...@QAEXJ@Z); 0x00B6D600 declared
#    void* (PAX).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import _rw, _w, edit
NOTE = '// Naming wave 17: the CWillButtonHelperTimer ctor is c672\'s (EHeroAbility, CThing&) one (??0CWillButtonHelperTimer@@QAE@W4EHeroAbility@@AAVCThing@@@Z);\n//   the ability is a cast of the landed value (no enumerator ordinal source) -- name-only.\n'
for a, par, init_old, init_new in (('007a09b0', 'thing', 'm_tTimer(11, ParentThing)', 'm_tTimer((EHeroAbility)11, *ParentThing)'),
                                   ('007c6760', 'owner', 'Timer(5, ParentThing)', 'Timer((EHeroAbility)5, *ParentThing)'),
                                   ('007c87f0', 'creature', 'Timer(0x10, ParentThing)', 'Timer((EHeroAbility)0x10, *ParentThing)')):
    p, rel, t = _rw(a, 'src')
    if init_new in t: print('already', a); continue
    old = 'CWillButtonHelperTimer(long ability, CThing* %s);' % par
    assert t.count(old) == 1 and t.count(init_old) == 1, a
    t = t.replace(old, 'CWillButtonHelperTimer(EHeroAbility ability, CThing& %s);' % par)
    t = t.replace(init_old, init_new)
    m = re.search(r'(?m)^struct CWillButtonHelperTimer \{', t)
    if not re.search(r'\benum EHeroAbility\b', t): t = t[:m.start()] + 'enum EHeroAbility { };                                   // Ego_r enum (enumerators: no ordinal source)\n' + t[m.start():]
    lines = t.split('\n'); i = 0
    while i < len(lines) and lines[i].startswith('//'): i += 1
    lines.insert(i, NOTE.rstrip('\n'))
    _w(p, '\n'.join(lines)); print('ok', a, 'src')
edit('00b6d600', 'src', '    void FreeMemoryPage(void* page);               // 0x00F37030', '    void FreeMemoryPage(long page);                // 0x00F37030 (?FreeMemoryPage@CGPUScratchBufferTextureManager@@QAEXJ@Z; naming wave 17: was void*)')
edit('00b6d600', 'src', '    void*         m_pMemoryPage;                   // +0x1d8', '    long          m_pMemoryPage;                   // +0x1d8 (the page index FreeMemoryPage takes)')
