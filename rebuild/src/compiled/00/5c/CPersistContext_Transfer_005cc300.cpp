#pragma optimize("s",on)
// CPersistContext::Transfer<CDefIndex>(const char* name, CDefIndex& value) @ 0x005CC300 -- the two-argument named transfer: build a
// zeroed default CDefIndex and forward to the three-argument Transfer<CDefIndex>(name, value, default) at 0x005CC1E8.
// Leaf: ??$Transfer@VCDefIndex@@@CPersistContext@@QAEXPBDAAVCDefIndex@@@Z
// NF188 (2026-11-20, q4510): name proven. FableWin.pdb 0x0199627C ??$Transfer@VCDefIndex@@@CPersistContext@@QAEXPBDAAVCDefIndex@@@Z (the
//   two-argument instance) + its callee 0x005CC1E8 = Ego_r.pdb 0x00455979 / FableWin.pdb 0x019CE186
//   ??$Transfer@VCDefIndex@@@CPersistContext@@QAEXPBDAAVCDefIndex@@ABV1@@Z (the three-argument instance) + the only caller
//   CTCInventoryWeapons::OnSerialise 0x005C3A95 passes ActiveMeleeWeaponDefIndex (+0x154 in Ego_r) / ActiveRangedWeaponDefIndex,
//   both CDefIndex (Ego_r.pdb CTCInventoryWeapons layout; CDefIndex = { long Val }). Transfer<long> is the separate COMDAT 0x004110B0.
//   Was the row-local stand-in CPersistContext::Transfer_long_ / TransferImpl; now an explicit specialization on engine/CPersistContext.h.
#define FABLE_RETAIL_CPERSISTCONTEXT
#include "engine/CPersistContext.h"
#include "engine/CDefIndex.h"

template<> void CPersistContext::Transfer<CDefIndex>(const char* name, CDefIndex& value)
{
    CDefIndex defaultValue(0);
    Transfer(name, value, defaultValue);
}
