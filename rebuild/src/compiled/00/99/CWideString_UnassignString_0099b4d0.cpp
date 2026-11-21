// CWideString::UnassignString() -- drop this string's reference to its data block: when the owner count falls to zero, delete the block (its STLport text, then the block), then clear the pointer
// Leaf: ?UnassignString@CWideString@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099B4D0, 62 bytes, thiscall (`push edi ; mov edi,ecx ...`), `ret`.
// Identity (proven): Ego_r.pdb 0x00B026F0 / FableWin.pdb 0x02F66B70 ?UnassignString@CWideString@@IAEXXZ (protected; both PDBs);
//   fable_string.h FABLE_RETAIL_CWIDESTRING declares it protected and inline. The landed callers bind it under five spellings
//   (UnassignString QAE / AAE / IAE, Release AAE, UnassignString_0099b4d0 IAE: NAME_FIXES); the catalog's CCharString::UnassignString
//   label was wrong (CCharString's is 0x0099E9B0).
// Callees: free 0x00BFEA14 (the STLport wide text block, the inline ~basic_string of CWideStringData::Data), operator delete
//   0x00BFE9BC (the 0x10-byte CWideStringData).
// Shape: the header's FABLE_RETAIL_CWIDESTRING_SPRINTF_ORIGINAL body (the data pointer snapshotted before the owner count drops;
//   `delete PStringData` re-reads the member and tests it), as the landed CWideString::Sprintf 0x0099BA70 TU uses it.
// Label correction: the July row was a free `__fastcall CWideString_UnassignString(CWideString*)` on a local CWideString model.
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_SPRINTF_ORIGINAL
#include <wchar.h>
#include "fable_string.h"

// Fidelity: scaffolding, not retail -- the header defines UnassignString inline, so this TU emits its COMDAT through a reference
// (the nf86 / nf88 address helper; a protected member is named through an anonymous derived class).
namespace {
struct UnassignStringReference : CWideString {
    static void (CWideString::*Get())();
};
void (CWideString::*UnassignStringReference::Get())() { return &UnassignStringReference::UnassignString; }
}
