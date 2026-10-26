// CTestQuest::~CTestQuest() -- the compiler-generated (implicit) destructor, kept out of line: five CCharString destructors, then the one at +0x00 as a tail jump
// Leaf: ??1CTestQuest@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004A7750, 51 bytes (+ int3 padding), thiscall, no return value; no callers or references.
// Identity (probable): CTestQuest::~CTestQuest(); evidence: PROBABLE -- no callers and no references (unreferenced COMDATs survive in retail); the body destroys CCharStrings at +0x18/+0x14/+0x10/+0x0C/+0x04 and tail-jumps to ~CCharString for +0x00, exactly Ego_r.pdb CTestQuest (0x1C: six CCharStrings and the long ColourIndex at +0x08), the only Ego_r class with that shape; FableWin.pdb has ??1CTestQuest@@QAE@XZ (non-virtual, as here). It directly follows 0x004A7710, the matching implicit default constructor (CCharString::CCharString 0x0099E4B0 at +0x00/+0x04/+0x0C/+0x10/+0x14/+0x18; FableWin.pdb ??0CTestQuest@@QAE@XZ), so the pair was emitted together. The byte-identical 0x004AA750 cannot also be ~CTestQuest (one retail address = one name) and is landed descriptively
// Label: the manifest's "_Dest_val<std::allocator<CEditLevelMerger::CConflict>,CEditLevelMerger::CConflict>" is propagated across byte-identical bodies (family J072: five implicit destructors of
//   five classes, identical up to member offsets and callees; retail has no identical-code folding, so each address is one function).
// Callees: CCharString::~CCharString 0x0099EAE0 (six times, the last as a tail jmp).
// Layout: Ego_r.pdb CTestQuest members at their PDB offsets (no containers: retail = Ego_r).
// Shape: no vtable store -- VC7.1 resets the vptr only in user-written destructors (agent952's finding, 0x006DF8C0); the class-typed
//   members are destroyed in reverse declaration order, then the first member's destructor (~CCharString +0x00) (a tail `jmp`). KillOne's explicit destructor call is a modelling
//   device: it makes VC7.1 emit the implicit destructor, which it keeps out of line.
class CCharString {                                      // 4 bytes: the shared string data pointer
public:
    CCharString();                                       // 0x0099E4B0
    ~CCharString();                                      // 0x0099EAE0
    void* PStringData;
};
class CTestQuest {                                       // Ego_r.pdb 0x1C bytes, non-polymorphic
public:
    CTestQuest();                                        // implicit in this model; retail 0x004A7710 (see Identity)
    CCharString QuestName;                               // +0x00
    CCharString HSPName;                                 // +0x04
    long        ColourIndex;                             // +0x08
    CCharString DisplayName;                             // +0x0C
    CCharString IniFile;                                 // +0x10
    CCharString OutiFile;                                // +0x14
    CCharString QuestCardDefName;                        // +0x18
};
void KillOne(CTestQuest* p) { p->~CTestQuest(); }