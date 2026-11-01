#include "rebuild_abi.h"
// CTCCarrying::SetAllCarriedThingsAsBypassFilter(void* key) -- mark the carried things as bypassing the filters under `key`:
//   BypassFilters = true, Key = key, then ResetAllCarriedThingsAsBypassFilter() re-applies it to every carried thing
// Leaf: ?SetAllCarriedThingsAsBypassFilter@CTCCarrying@@QAEXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A2520; Ego_r.pdb ?SetAllCarriedThingsAsBypassFilter@CTCCarrying@@QAEXPAX@Z (the catalog
//   label); the callee is ?ResetAllCarriedThingsAsBypassFilter@CTCCarrying@@QAEXXZ 0x006A23E0 (chunk c678 queue row).
// Layout: Ego_r CTCCarrying BypassFilters (bool) and Key (void*), retail +0x1c / +0x20 (CarrySlots at +0xC is the 16-byte retail
//   CVectorMap<long, CIntelligentPointer<CThing> >).
// Naming wave 18: landed as the FableCarry_006a2520::SetAll(unsigned long) / helper() stand-ins; respelled on the PDB class and
//   members (name-only: the bytes are unchanged; the test's spy defines ResetAllCarriedThingsAsBypassFilter).
class CTCCarrying { public:
    unsigned char _pad[0x1c];
    bool  BypassFilters;                             // +0x1c
    void* Key;                                       // +0x20
    void ResetAllCarriedThingsAsBypassFilter();      // 0x006A23E0
    void SetAllCarriedThingsAsBypassFilter(void* key);
};
void CTCCarrying::SetAllCarriedThingsAsBypassFilter(void* key)
{
    BypassFilters = true;
    Key = key;
    ResetAllCarriedThingsAsBypassFilter();
}
