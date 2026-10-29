#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

class CTCTrapBase {
public:
    void ManualTrigger();   // 0x00788388
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> CTrapInterfaceEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    CTrapInterfaceEntry* Begin;
    CTrapInterfaceEntry* End;
    CTrapInterfaceEntry* LowerBound(const ETCInterfaceType& key);
};
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
        CTrapInterfaceEntry* end = map->End;
        if (entry == end || entry->first > 0x7d)
            entry = end;
        ((CTCTrapBase*)(((void*)entry->second)))->ManualTrigger();
        return true;
    }
    return false;
}
