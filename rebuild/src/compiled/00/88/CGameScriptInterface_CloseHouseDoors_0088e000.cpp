// CGameScriptInterface::CloseHouseDoors(const CScriptThing& thing) const -- locks every door of the building the script thing refers to
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088E000, length 48 bytes.
// Callees: CScriptThing virtual slot 75 (+0x12c, IsValid) and slot 11 (+0x2c, GetThing), CThingBuilding::LockAllDoors(bool lock,
// bool) @ 0x0082DB00 (true, false) on the thing when bit 6 of its flag byte at +0x6c is set (it is a building).
// Oracle name: ?CloseHouseDoors@CGameScriptInterface@@UBEXABVCScriptThing@@@Z (`this` unused; written on the PDB-header wrapper).
// Leaf: CloseHouseDoors
#include "engine/CGameScriptInterface.h"

class CThingBuilding { public:
    unsigned char _pad[0x6c];
    unsigned char m_typeFlags;                      // +0x6c (bit 6 = building)
    void LockAllDoors(bool lock, bool silent);      // 0x0082DB00
};

class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

struct CGameScriptInterface_Methods : CGameScriptInterface {
    void CloseHouseDoors(const CScriptThing& thing) const;
};

void CGameScriptInterface_Methods::CloseHouseDoors(const CScriptThing& thing) const
{
    if (thing.IsAlive()) {
        CThingBuilding* building = ((CThingBuilding*)thing.GetPThing());
        if (building->m_typeFlags & 0x40)
            building->LockAllDoors(true, false);
    }
}