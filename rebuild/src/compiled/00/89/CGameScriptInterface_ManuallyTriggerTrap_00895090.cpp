// Plan phase 5 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's pointer members Begin/End are the header CArray base's begin() / end() (STLport vector start +0 / finish +4).
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

class CTCTrapBase {
public:
    void ManualTrigger();   // 0x00788388
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef std::pair<ETCInterfaceType, CTCBase*> CTrapInterfaceEntry;
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTrapInterfaceMap;
struct CTrapTargetView {
    unsigned char _pad_0x00[0x2c];
    unsigned int TypeFlags;
    unsigned char _pad_0x30[0x14];
    CTrapInterfaceMap Interfaces;
};
struct CGameScriptInterfaceTrapVTable {
    unsigned char _pad_0x00[0x2c];
    CTrapTargetView* (__fastcall *GetTarget)(CGameScriptInterface*);
};

bool __stdcall CGameScriptInterface_ManuallyTriggerTrap(CGameScriptInterface* self)
{
    CTrapTargetView* target =
        ((CGameScriptInterfaceTrapVTable*)self->__vftable)->GetTarget(self);
    if (target && (target->TypeFlags & 0x20000000)) {
        CTrapInterfaceMap* map = &target->Interfaces;
        ETCInterfaceType key = (ETCInterfaceType)(0x7d);
        CTrapInterfaceEntry* entry = map->LowerBound(key);
        CTrapInterfaceEntry* end = map->end();
        if (entry == end || entry->first > 0x7d)
            entry = end;
        ((CTCTrapBase*)(((void*)entry->second)))->ManualTrigger();
        return true;
    }
    return false;
}
