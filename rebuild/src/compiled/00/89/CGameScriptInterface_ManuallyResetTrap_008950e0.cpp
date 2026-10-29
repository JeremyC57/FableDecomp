#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> CTrapResetInterfaceEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    CTrapResetInterfaceEntry* Begin;
    CTrapResetInterfaceEntry* End;
    CTrapResetInterfaceEntry* LowerBound(const ETCInterfaceType& key);
};
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
        CTrapResetInterfaceEntry* end = map->End;
        if (entry == end || entry->first > 0x7d)
            entry = end;
        Iface_Do(((void*)entry->second));
        return true;
    }
    return false;
}
