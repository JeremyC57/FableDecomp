// CTCInventoryBase::GetInventoryGuildValidity() -- finds the first building thing carrying thing-component 0x6A and, when one exists, remembers it in pBuildingThing and returns true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDC77, length 61 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on)
// (`and dword ptr [ebp-4],0` zero store).
// Callees: the thing-search-tools accessor @ 0x00686D80 (`mov ecx,[0x013B8A1C] ; jmp`, no arguments), and
// CThingSearchTools::GetPFirstLeafThing<CThingFilter_HasTC, CThingBuilding>(filter, CThing** out) @ 0x005C0D9D (its bool is ignored).
// Retail CTCInventoryBase: no engine header; pBuildingThing at +0xB8 (PDB +0xCC minus five 4-byte-narrower vectors).
// Shape: the out pointer is zeroed first (named local), the HasTC filter is a temporary built in the call (its
// TC store lands after the pushes), and the bool result uses the `mov al,1 / xor al,al` two-exit form.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CThing;
class CThingFilter_HasTC { public:
    long m_tc;
    CThingFilter_HasTC(long tc) : m_tc(tc) {}
};
class CThingSearchTools { public:
    bool GetPFirstLeafThing(const CThingFilter_HasTC& filter, CThing** out);   // 0x005C0D9D
};
CThingSearchTools* GetThingSearchTools();       // 0x00686D80
struct CTCInventoryBase_Retail {
    unsigned char _pad_0x00[0xB8];
    CThing*       pBuildingThing;               // +0xB8
    bool GetInventoryGuildValidity();
};
bool CTCInventoryBase_Retail::GetInventoryGuildValidity()
{
    CThing* building = 0;
    GetThingSearchTools()->GetPFirstLeafThing(CThingFilter_HasTC(0x6A), &building);
    if (building) {
        pBuildingThing = building;
        return true;
    }
    return false;
}