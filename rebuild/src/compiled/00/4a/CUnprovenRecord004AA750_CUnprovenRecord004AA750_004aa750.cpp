// CUnprovenRecord_004AA750::~CUnprovenRecord_004AA750() -- the compiler-generated (implicit) destructor, kept out of line: five CCharString destructors, then the one at +0x00 as a tail jump
// Leaf: ??1CUnprovenRecord_004AA750@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AA750, 51 bytes (+ int3 padding), thiscall, no return value; no callers or references.
// Identity (descriptive): CUnprovenRecord_004AA750::~CUnprovenRecord_004AA750() (class unproven); evidence: DESCRIPTIVE -- no callers and no references; byte-identical to 0x004A7750 (landed as the probable ~CTestQuest beside its default constructor 0x004A7710). Retail has no identical-code folding, so this is a different function -- the destructor of a second 0x1C record with CCharStrings at +0x00/+0x04/+0x0C/+0x10/+0x14/+0x18 and a destructor-less dword at +0x08, whose class no evidence names (its neighbours 0x004AA700 / 0x004AA7B0 are unrelated CCountedPointer helpers). The type is the owner-rule CUnproven placeholder
// Label: the manifest's "_Dest_val<std::allocator<CEditLevelMerger::CConflict>,CEditLevelMerger::CConflict>" is propagated across byte-identical bodies (family J072: five implicit destructors of
//   five classes, identical up to member offsets and callees; retail has no identical-code folding, so each address is one function).
// Callees: CCharString::~CCharString 0x0099EAE0 (six times, the last as a tail jmp).
// Layout: descriptive member names; the destroyed offsets are the retail ones.
// Shape: no vtable store -- VC7.1 resets the vptr only in user-written destructors (agent952's finding, 0x006DF8C0); the class-typed
//   members are destroyed in reverse declaration order, then the first member's destructor (~CCharString +0x00) (a tail `jmp`). KillOne's explicit destructor call is a modelling
//   device: it makes VC7.1 emit the implicit destructor, which it keeps out of line.
class CCharString {                                      // 4 bytes: the shared string data pointer
public:
    CCharString();                                       // 0x0099E4B0
    ~CCharString();                                      // 0x0099EAE0
    void* PStringData;
};
class CUnprovenRecord_004AA750 {                         // DESCRIPTIVE: 0x1C-byte record, class unproven (see Identity)
public:
    CCharString String00;                                // +0x00
    CCharString String04;                                // +0x04
    int         Field08;                                 // +0x08 (no destructor: type unknown)
    CCharString String0C;                                // +0x0C
    CCharString String10;                                // +0x10
    CCharString String14;                                // +0x14
    CCharString String18;                                // +0x18
};
void KillOne(CUnprovenRecord_004AA750* p) { p->~CUnprovenRecord_004AA750(); }