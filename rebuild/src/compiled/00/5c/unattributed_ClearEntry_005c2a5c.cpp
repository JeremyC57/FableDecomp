// CSoundPairTable_005c2a5c::ClearEntry(long index) -- stores a default (all-zero) {id, CSoundPair} entry at `index` of the owned array
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C2A5C, length 48 bytes.
// Callees: CArray<std::pair<unsigned long, CSoundPair>>::SetAt @ 0x005C2A13 (manifest "push_back":
// (index, const entry&) -- it overwrites in range and grows otherwise, `ret 8`), and the
// CCountedPointer destructor @ 0x005C04BF on the entry's +8 (releases the block, nulls both words).
// The manifest's "CAC_HighScorePost::CAC_HighScorePost" label is a constructor name on a body
// that returns nothing (`leave ; ret 4`, no `mov eax,esi`), so it is rejected; the class is
// named by what the body does. Frame-pointer body with `push dword ptr [ebp+8]`, `leave` and no
// spills, but with the entry's constructors inlined: a size-optimised TU
// (optimize("s",on); the "gsy" form of GOTCHAS 114 disables inlining and spills
// `this`). The 0x14-byte entry is zeroed through one register by its inline constructors
// (pair -> CSoundPair -> CCountedPointer), passed by reference after the index, and only the
// counted pointer sub-object has a destructor to run.
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: this row's object is byte-identical without it (size mode keeps the
//   leaf's frame; T3 nf416 audit).
#pragma optimize("s", on)

// NF158 (q5208): the counted pointer is the shared engine/CCountedPointer.h CCountedPointer<NInventory::CPerItemData>; its inline
//   destructor is the out-of-line COMDAT 0x005C04BF here (Ego_r.pdb / FableWin.pdb ??1?$CCountedPointer@VCPerItemData@NInventory@@@@QAE@XZ,
//   the self row's leaf; the 0x14-byte entry has NInventory::CItem's shape, PPerItemData at +8; this row is slot 1 of the
//   CPersistContext_TransferableVector<NInventory::CItem> vtable 0x01253CA4 per the landed 0x005C0E41 / 0x005C20D8); was the local
//   stand-in N_005c04bf::CCountedPointer.
#include "engine/CCountedPointer.h"
namespace NInventory { class CPerItemData; }

class CSoundPair { public:                         // 0x10 bytes
    long            m_sound;                // +0x00
    CCountedPointer<NInventory::CPerItemData> m_pSample;   // +0x04
    long            m_volume;               // +0x0c
    CSoundPair() : m_sound(0), m_pSample(), m_volume(0) { }
};

struct SSoundPairEntry {                    // std::pair<unsigned long, CSoundPair>, 0x14 bytes
    unsigned long first;
    CSoundPair    second;
    SSoundPairEntry() : first(0), second() { }
};

struct CSoundPairArray {
    SSoundPairEntry* first;
    SSoundPairEntry* last;
    void SetAt(long index, const SSoundPairEntry& entry);   // 0x005C2A13
};

struct CSoundPairTable_005c2a5c {
    void*           __vftable;              // +0x00
    CSoundPairArray* m_pEntries;            // +0x04
    void ClearEntry(long index);
};

void CSoundPairTable_005c2a5c::ClearEntry(long index)
{
    SSoundPairEntry entry;
    m_pEntries->SetAt(index, entry);
}