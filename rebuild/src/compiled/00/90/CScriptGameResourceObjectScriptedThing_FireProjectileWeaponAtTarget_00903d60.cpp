// CScriptGameResourceObjectScriptedThing::FireProjectileWeaponAtTarget(const CScriptThing& target) -- queue a projectile-weapon attack on a live, targetable target for the live scripted thing
// Retry 10b (chain-split sweep, FIXES B-16): the target thing's item is a named local (`CThing_Retail* item = thing->GetPItem();`) before the dead test, which gives retail's register colouring (PThing in esi, the target in edi).
// Leaf: FireProjectileWeaponAtTarget
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00903D60, length 181 bytes (`ret 4`), vtable 0x0128D86C slot 3.
// PDB: ?FireProjectileWeaponAtTarget@CScriptGameResourceObjectScriptedThing@@UAEXABVCScriptThing@@@Z.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PThing at +0x14), CScriptThing::IsAlive (slot 75)
// and GetPThing (slot 11), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined
// GetInterface, interface 0x1f = CTCScriptedControl), operator new @ 0x00BFEA1A (0x20 bytes),
// NTCScriptedControl::CActionFireProjectileWeapon::CActionFireProjectileWeapon(CThing*) @ 0x00716790 (vtable
// 0x01265960; manifest label "CTCTargeted::CTCTargeted" is propagated), CTCScriptedControl::AddAction @ 0x007137D0.
// Shape: as SummonerLightningOrbAttackTarget (0x00903CA0) plus a target type test (ThingTypeFlags bit 2 at
// +0x6c) between GetPThing and the unchecked GetInterface (key and miss read in the dead `target` slot).
// `this` uses engine/CScriptGameResourceObjectScriptedThing.h (PThing reached through a local view).
#include <stddef.h>
#include "engine/CScriptGameResourceObjectScriptedThing.h"


class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                             // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);          // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CThing_Retail {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];               // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;                   // +0x44
    unsigned char  _pad4c[0x20];
    unsigned long  ThingTypeFlags;                 // +0x6c
    unsigned char  _pad70[0x21];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
struct CIntelligentPointer_Retail {                // CIntelligentPointer<CThing>
    virtual void v00();
    void* m_pItem;
    CThing_Retail* GetPItem();                     // 0x00A01B50 CBaseIntelligentPointer::GetPItem
};

class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"

namespace NTCScriptedControl {
struct CActionBase {                               // vtable 0x01265504
    virtual void v00();
};
}

class CTCScriptedControl { public:                        // interface 0x1f component
    void AddAction(NTCScriptedControl::CActionBase* action);   // 0x007137D0
};

namespace NTCScriptedControl {
struct CActionFireProjectileWeapon : CActionBase {             // vtable 0x01265960, 0x20 bytes
    unsigned char _body[0x1c];
    CActionFireProjectileWeapon(CThing_Retail* target);         // 0x00716790
};
}

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    void FireProjectileWeaponAtTarget(const CScriptThing& target);
};

void CScriptGameResourceObjectScriptedThing_Methods::FireProjectileWeaponAtTarget(const CScriptThing& target)
{
    CIntelligentPointer_Retail* thing = (CIntelligentPointer_Retail*)PThing;
    if (thing->GetPItem()) {
        CThing_Retail* item = thing->GetPItem();
        if (!item->m_dead) {
            if (target.IsAlive()) {
                CThing_Retail* targetThing = ((CThing_Retail*)target.GetPThing());
                if (targetThing->ThingTypeFlags & 4) {
                    CTCScriptedControl* control;
                    thing->GetPItem()->GetInterface(&control, 0x1f);
                    control->AddAction(new NTCScriptedControl::CActionFireProjectileWeapon(targetThing));
                }
            }
        }
    }
}