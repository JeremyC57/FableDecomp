// CTCHeroStats::PeekMarriageInfo(unsigned __int64, CHeroMarriageInfo&) const -- copies out the marriage record for this wife, if there is one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057AC89, 59 bytes.
//
// Shape: HasMarriageInState's (0x0057AC29, landed) list walk -- `jmp` into the bottom test, a 64-bit WifeUID compare
// against the by-value key's two argument words -- with the hit block placed after the `xor al,al` exit: the 24-byte
// POD record is copied with `push 6 ; pop ecx ; rep movsd` (size TU) and the block jumps back to the shared `ret 0Ch`.
// Layout: Marriages +0x1D8 in retail (PDB +0x1F0); CHeroMarriageInfo from the PDB (0x18 bytes: WifeUID, MaritalHomeUID,
// State, GayMarriage), the node's value at +0x08.
// Leaf: ?PeekMarriageInfo@CTCHeroStats@@QBE_N_KAAVCHeroMarriageInfo@@@Z
// Identity: Ego_r.pdb 0x0056E06C + FableWin.pdb ?PeekMarriageInfo@CTCHeroStats@@QBE_N_KAAVCHeroMarriageInfo@@@Z (the record by
//   reference); engine/CTCHeroStats.h FABLE_RETAIL_CTCHEROSTATS_PEEKMARRIAGEINFO declares it, and the landed callers 0x00682810 /
//   0x006D1E30 bind it.
// NF113 (PRIORITY q4715): was the stand-in CTCHeroStats_Retail::PeekMarriageInfo(unsigned __int64, CHeroMarriageInfo*) over local
//   CHeroMarriageInfo / node / layout models; now the shared header's member on its typed retail layout (FABLE_RETAIL_CTCHEROSTATS_SERIALISE:
//   std::list<CHeroMarriageInfo> Marriages +0x1D8) walking the STLport list.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_PEEKMARRIAGEINFO
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CHeroMarriageInfo.h"
#include "engine/CTCHeroStats.h"

bool CTCHeroStats::PeekMarriageInfo(unsigned __int64 uid, CHeroMarriageInfo& info) const
{
    for (std::list<CHeroMarriageInfo>::const_iterator it = Marriages.begin(); it != Marriages.end(); ++it) {
        if ((*it).WifeUID == uid) {
            info = *it;
            return true;
        }
    }
    return false;
}
