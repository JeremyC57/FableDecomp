# h21_csound.py: naming wave 21, family csound -- DECISIONS_W20_RESOLVED item "vtable 0x012B1E24 class name": CSoundSystem is the one
# class name for the libpk sound system (vtable 0x012B1E24; CreateSoundSystem_00c096e0 returns it, the landed CSoundManagerLH ctor
# 0x00A3EF00 drives it). Every landed row modelling that object as CAudioOwner is respelled name-only (byte-identical):
#   00c094a0 ctor (+ the setup record CSteeringSaveRecord -> CSoundSystemSetup, the 0x30-byte record 0x00A3EF00 builds and 0x00C096E0 passes on),
#   00c09380 dtor, 00c0a6b0 scalar deleting dtor, 00c091f0 Update, 00c08ae0 WaitForSprite, 00c11d00 CSoundEventManager ctor (owner parameter),
#   00c0b200 CAudioParamBlock ctor (owner parameter), 00c0b380 SetSlot / 00c0af20 Update (owner member), 00c12880 / 00c0a9a0 (comments),
#   00c096e0 the factory: was CWorldManager::InitializeSteeringAfterSave over a CSteeringGroup stand-in; now the __stdcall
#   CreateSoundSystem_00c096e0(CSoundSystemSetup*) the landed 0x00A3EF00 binds (chunk 687 NAME_FIXES; `this` was unused).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
_X = set()
_xp = os.path.join(he.W, 'runs', 'excl_csound.txt')
if os.path.exists(_xp): _X = set(l.split()[0] for l in open(_xp) if l.strip())
NOTE = '// Naming (naming wave 21, DECISIONS_W20_RESOLVED): CAudioOwner -> CSoundSystem, the one class name for vtable 0x012B1E24 (the libpk sound system CreateSoundSystem_00c096e0 0x00C096E0 returns and the landed CSoundManagerLH ctor 0x00A3EF00 drives; descriptive, libpk has no PDB).'
def note(a, k='src'):
    p, rel, t = he._rw(a, k)
    if NOTE in t: print('already', a, k); return
    m = re.search(r'^// Leaf: [^\n]*\n', t, re.M)
    i = m.end() if m else (t.index('\n') + 1)
    he._w(p, t[:i] + NOTE + '\n' + t[i:]); print('ok', a, k)
RE = r'CAudioOwner(Base)?(?![A-Za-z0-9_])'   # right boundary only: mangled text (??0CAudioOwner@@, ??_GCAudioOwner@@) has no left word boundary
for a in ('00c094a0', '00c09380', '00c0a6b0', '00c091f0', '00c08ae0', '00c11d00', '00c0b200', '00c0b380', '00c0af20', '00c12880', '00c0a9a0'):
    if a in _X: continue
    he.resub(a, 'src', RE, r'CSoundSystem\1')
    he.resub(a, 'tst', RE, r'CSoundSystem\1')
    note(a)
a = '00c094a0'
if a not in _X:
    he.resub(a, 'src', r'CSteeringSaveRecord(?![A-Za-z0-9_])', 'CSoundSystemSetup')
    he.resub(a, 'tst', r'CSteeringSaveRecord(?![A-Za-z0-9_])', 'CSoundSystemSetup')
    he.edit(a, 'src', '// Layout: library code with no RTTI or PDB type', '// Naming (naming wave 21): the setup record is the 0x30-byte CSoundSystemSetup the landed CSoundManagerLH ctor 0x00A3EF00 builds and CreateSoundSystem_00c096e0 0x00C096E0\n//   passes on (its +0x14 word is the LHA::CFileManager pointer there: a non-zero one builds the adapter 0x00C1B950, else the PKCDefaultFileManager\n//   0x00C1AC40 -- the CSBAgents / CSBDefaultAgents names below are the earlier landing\'s). The field names of this TU are kept (name-only pass).\n// Layout: library code with no RTTI or PDB type')
a = '00c0a6b0'
if a not in _X:
    he.resub(a, 'src', r'\bCreateAudioOwner_00c0a6b0\b', 'NewSoundSystem_00c0a6b0')
    he.resub(a, 'tst', r'\bCreateAudioOwner_00c0a6b0\b', 'NewSoundSystem_00c0a6b0')
he.put('00c096e0', 'src', '''// CreateSoundSystem_00c096e0(CSoundSystemSetup* setup) -- libpk sound-system factory (`__stdcall`, no `this`): resolve the optional context object from the setup's +0x24 / +0x2C words (0x00C1C1A0) and, when there is one, build the 0x84-byte CSoundSystem (vtable 0x012B1E24) from the setup and it; 0 otherwise
// Leaf: ?CreateSoundSystem_00c096e0@@YGPAUCSoundSystem@@PAUCSoundSystemSetup@@@Z
// Flags: /EHsc
// Retail: Fable.exe @ 0x00C096E0, 120 bytes, `ret 4`, EH frame (handler 0x01210C33; state 0 guards the new'd block).
// Naming (naming wave 21, DECISIONS_W20_RESOLVED / chunk 687 NAME_FIXES): was landed as CWorldManager::InitializeSteeringAfterSave over a
//   CSteeringGroup stand-in (propagated; `this` is never used). The landed CSoundManagerLH ctor 0x00A3EF00 pushes its 0x30-byte
//   CSoundSystemSetup and binds exactly this `__stdcall` spelling; the object it returns stores vtable 0x012B1E24 (the CSoundSystem ctor
//   0x00C094A0 / dtor 0x00C09380) and is driven through slot 4 Initialise / slot 11 SetBufferSize there. Descriptive (libpk, no PDB).
// Callees: the __fastcall lookup 0x00C1C1A0 taking the setup's +0x24 and +0x2C words (manifest label "CWorldMap::SaveToFile" is wrong;
//   the callee ignores the second register; its landed spelling LookupSteeringWorldEntry / CSteeringWorldEntry is the earlier landing's,
//   kept here), operator new 0x00BFEA1A (0x84 bytes), CSoundSystem::CSoundSystem(CSoundSystemSetup*, CSteeringWorldEntry*) 0x00C094A0.
// Shape: a missing entry or a failed allocation returns 0; `return new CSoundSystem(...)` keeps the allocation in the EH frame slot for
//   the constructor's unwind.
struct CSteeringWorldEntry;

struct CSoundSystemSetup {                      // 0x30 bytes (the landed 0x00A3EF00 model); only +0x24 / +0x2C are read here
    unsigned char _pad[0x24];
    void*         Long24;                       // +0x24
    unsigned char _pad28[0x4];
    void*         Long2C;                       // +0x2C
};

CSteeringWorldEntry* __fastcall LookupSteeringWorldEntry(void* worldKey, void* entryKey);   // 0x00C1C1A0

struct CSoundSystem {                           // vtable 0x012B1E24, 0x84 bytes
    CSoundSystem(CSoundSystemSetup* setup, CSteeringWorldEntry* entry);   // 0x00C094A0
    unsigned char _body[0x84];
};

CSoundSystem* __stdcall CreateSoundSystem_00c096e0(CSoundSystemSetup* setup);

CSoundSystem* __stdcall CreateSoundSystem_00c096e0(CSoundSystemSetup* setup)
{
    CSteeringWorldEntry* entry = LookupSteeringWorldEntry(setup->Long24, setup->Long2C);
    if (entry)
        return new CSoundSystem(setup, entry);
    return 0;
}
''')
