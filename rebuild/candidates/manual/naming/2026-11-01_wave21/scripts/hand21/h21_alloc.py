# h21_alloc.py: naming wave 21, family alloc -- wave 20 DEFERRED: the three __fastcall allocator stores 0x00C16690 / 0x00C166D0 / 0x00C166B0 the
# respelled SetSound*Allocator_00c08xx0 rows call; their own rows carried the propagated CHoverable_ProcessEvent over hover stand-ins. Re-authored
# byte-identical under the spellings wave 20 bound (StoreSound*Allocator_00c166x0(LHA::CAllocator*), descriptive, libpk): store the allocator into
# the library global (0x0139C960 / 0x0139C968 / 0x0139C964) and fall back to the library's default malloc/free allocator object at 0x0143BB34
# (vtable 0x012B2AB8: ctor 0x00C166F0, Allocate 0x00C16700, Free 0x00C16720, dynamic initializer 0x01224B40 -- the CVtable_012b2ab8 /
# CStatic_00c166f0 spellings of those rows become CDefaultAllocator_00c166f0 / g_DefaultAllocator_0143bb34, one class, one name).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
_X = set()
_xp = os.path.join(he.W, 'runs', 'excl_alloc.txt')
if os.path.exists(_xp): _X = set(l.split()[0] for l in open(_xp) if l.strip())
def P(a, txt):
    if a not in _X: he.put(a, 'src', txt)
def R(a, k, pat, rep, n=None):
    if a not in _X: he.resub(a, k, pat, rep, n)
for a, nm, g, what, setter in (('00c16690', 'StoreSoundAllocator_00c16690', '0139c960', 'sound allocator', '0x00C08CF0'), ('00c166d0', 'StoreSoundTempAllocator_00c166d0', '0139c968', 'temporary-sound allocator', '0x00C08D00'), ('00c166b0', 'StoreSoundPhysicalAllocator_00c166b0', '0139c964', 'physical-sound allocator', '0x00C08D10')):
    P(a, '''// %s(LHA::CAllocator* allocator) -- install the %s into the libpk library global 0x%s; a null allocator selects the library's default malloc/free allocator object 0x0143BB34
// Leaf: ?%s@@YIXPAVCAllocator@LHA@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x%s, 21 bytes (`__fastcall`: `test ecx,ecx ; mov [g],ecx ; jne ; mov dword ptr [g],offset default ; ret`).
// Naming (naming wave 21, wave 20 DEFERRED): the spelling the landed %s (SetSound*Allocator, chunk 687 / naming wave 20) binds; descriptive (libpk, no PDB).
//   Was the propagated CHoverable_ProcessEvent over hover stand-ins. The default object is the libpk malloc/free allocator (vtable 0x012B2AB8:
//   ctor 0x00C166F0, Allocate 0x00C16700, Free 0x00C16720; dynamic initializer 0x01224B40), CDefaultAllocator_00c166f0 (descriptive) here.
namespace LHA { class CAllocator; }
class CDefaultAllocator_00c166f0;                               // vtable 0x012B2AB8, derives from LHA::CAllocator
extern LHA::CAllocator* g_p%s_%s;                   // 0x%s
extern CDefaultAllocator_00c166f0 g_DefaultAllocator_0143bb34;   // 0x0143BB34

void __fastcall %s(LHA::CAllocator* allocator);

void __fastcall %s(LHA::CAllocator* allocator)
{
    g_p%s_%s = allocator;
    if (!allocator)
        g_p%s_%s = (LHA::CAllocator*)&g_DefaultAllocator_0143bb34;
}
''' % (nm, what, g.upper(), nm, a.upper(), setter, nm[5:-9], g, g.upper(), nm, nm, nm[5:-9], g, nm[5:-9], g))
for a in ('00c166f0', '00c16700', '00c16720'):
    for k in ('src', 'tst'): R(a, k, r'CVtable_012b2ab8(?![A-Za-z0-9_])', 'CDefaultAllocator_00c166f0')
    R(a, 'src', r'^(// Leaf: [^\n]*\n)', r'\1// Naming (naming wave 21): CVtable_012b2ab8 -> CDefaultAllocator_00c166f0, the libpk default malloc/free allocator object at 0x0143BB34 (dynamic initializer 0x01224B40) the StoreSound*Allocator stores 0x00C16690 / 0x00C166D0 / 0x00C166B0 fall back to (descriptive, libpk).\n', 1)
a = '00c166f0'
if a not in _X:
    p, rel, t = he._rw(a, 'src')
    if '// Naming (naming wave 21)' not in t:
        he._w(p, t.replace('// CDefaultAllocator_00c166f0::CDefaultAllocator_00c166f0() -- root polymorphic class: only the vtable store\n', '// CDefaultAllocator_00c166f0::CDefaultAllocator_00c166f0() -- root polymorphic class: only the vtable store\n// Naming (naming wave 21): CVtable_012b2ab8 -> CDefaultAllocator_00c166f0, the libpk default malloc/free allocator object at 0x0143BB34 (dynamic initializer 0x01224B40; Allocate 0x00C16700 / Free 0x00C16720) the StoreSound*Allocator stores 0x00C16690 / 0x00C166D0 / 0x00C166B0 fall back to (descriptive, libpk).\n', 1)); print('ok', a, 'src')
    else: print('already', a, 'src')
a = '01224b40'
for k in ('src', 'tst'):
    R(a, k, r'CStatic_00c166f0(?![A-Za-z0-9_])', 'CDefaultAllocator_00c166f0')
    R(a, k, r'\bg_CStatic_00c166f0_0143bb34\b', 'g_DefaultAllocator_0143bb34')
# one class key for the allocator object (the stores declare `class`; the data symbol ?g_DefaultAllocator_0143bb34@@3VCDefaultAllocator_00c166f0@@A)
for a in ('00c166f0', '01224b40'):
    if a not in _X: he.resub(a, 'src', r'\bstruct CDefaultAllocator_00c166f0 \{', 'class CDefaultAllocator_00c166f0 { public:', 1)
