// Plan phase 5 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's pointer members Begin/End are the header CArray base's begin() / end() (STLport vector start +0 / finish +4).
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef std::pair<ETCInterfaceType, CTCBase*> CTrapResetInterfaceEntry;
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTrapResetInterfaceMap;
struct CTrapResetTargetView {
    unsigned char _pad_0x00[0x2c];
    unsigned int TypeFlags;
    unsigned char _pad_0x30[0x14];
    CTrapResetInterfaceMap Interfaces;
};
struct CGameScriptInterfaceTrapResetVTable {
    unsigned char _pad_0x00[0x2c];
    CTrapResetTargetView* (__fastcall *GetTarget)(CGameScriptInterface*);
};
extern "C" void __fastcall Iface_Do(void* self);

bool __stdcall CGameScriptInterface_ManuallyResetTrap(CGameScriptInterface* self)
{
    CTrapResetTargetView* target =
        ((CGameScriptInterfaceTrapResetVTable*)self->__vftable)->GetTarget(self);
    if (target && (target->TypeFlags & 0x20000000)) {
        CTrapResetInterfaceMap* map = &target->Interfaces;
        ETCInterfaceType key = (ETCInterfaceType)(0x7d);
        CTrapResetInterfaceEntry* entry = map->LowerBound(key);
        CTrapResetInterfaceEntry* end = map->end();
        if (entry == end || entry->first > 0x7d)
            entry = end;
        Iface_Do(((void*)entry->second));
        return true;
    }
    return false;
}
