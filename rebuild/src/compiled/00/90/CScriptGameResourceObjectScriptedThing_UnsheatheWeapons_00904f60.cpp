// CScriptGameResourceObjectScriptedThing::UnsheatheWeapons() -- queue a do-creature-action wrapping an unsheathe-weapons creature action for the live scripted thing
// Leaf: UnsheatheWeapons
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00904F60, length 199 bytes, vtable 0x0128D86C slot 25.
// PDB: ?UnsheatheWeapons@CScriptGameResourceObjectScriptedThing@@UAEXXZ.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PThing at +0x14), CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, interface 0x1f = CTCScriptedControl),
// operator new @ 0x00BFEA1A (0x1c bytes), CCreatureAction_UnsheatheWeapons::CCreatureAction_UnsheatheWeapons
// (CThingCreatureBase&) @ 0x008B8590 (vtable 0x0127B1B4), NTCScriptedControl::CActionDoCreatureAction::
// CActionDoCreatureAction(const CCreatureActionBase&) @ 0x00713140 (vtable 0x01265838),
// CTCScriptedControl::AddAction @ 0x007137D0, CCombatActionBase::~CCombatActionBase @ 0x0062E900.
// Shape: the family guard and the unchecked GetInterface (component in ebp), the creature fetched before
// operator new; the 0x118-byte creature action is a temporary built inside the new-expression's arguments, so
// it is constructed only when the allocation succeeded and destroyed after AddAction under VC's
// conditional-temporary flag (bl; its frame word is zeroed at entry and doubles as the lookup key). The
// temporary's implicit destructor stores no vtable and calls the CCombatActionBase destructor.
// `this` uses engine/CScriptGameResourceObjectScriptedThing.h (PThing reached through a local view).
#include <stddef.h>
#include "engine/CScriptGameResourceObjectScriptedThing.h"

void* __cdecl operator new(size_t size);           // 0x00BFEA1A

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

class CThing_Retail { public:
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
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
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

class CThingCreatureBase;

class CCreatureActionBase {                        // retail 0xa8 bytes
public:
    virtual ~CCreatureActionBase();
    unsigned char _body04[0xa4];
};

class CCombatActionBase : public CCreatureActionBase {   // retail 0x114 bytes
public:
    virtual ~CCombatActionBase();                  // 0x0062E900
    unsigned char _bodyA8[0x114 - 0xa8];
};

class CCreatureAction_UnsheatheWeapons : public CCombatActionBase {   // vtable 0x0127B1B4, retail 0x118 bytes
public:
    CCreatureAction_UnsheatheWeapons(CThingCreatureBase& creature);   // 0x008B8590
    unsigned long WeaponType;                      // +0x114 EUnsheatheWeaponType
};

namespace NTCScriptedControl {
struct CActionDoCreatureAction : CActionBase {                 // vtable 0x01265838, 0x1c bytes
    unsigned char _body[0x18];
    CActionDoCreatureAction(const CCreatureActionBase& action);   // 0x00713140
};
}

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    void UnsheatheWeapons();
};

void CScriptGameResourceObjectScriptedThing_Methods::UnsheatheWeapons()
{
    CIntelligentPointer<CThing_Retail>* thing = (CIntelligentPointer<CThing_Retail>*)PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            CTCScriptedControl* control;
            thing->GetPItem()->GetInterface(&control, 0x1f);
            CThingCreatureBase* creature = (CThingCreatureBase*)thing->GetPItem();
            control->AddAction(new NTCScriptedControl::CActionDoCreatureAction(CCreatureAction_UnsheatheWeapons(*creature)));
        }
    }
}