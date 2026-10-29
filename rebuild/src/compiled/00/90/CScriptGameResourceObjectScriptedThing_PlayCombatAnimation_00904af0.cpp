// CScriptGameResourceObjectScriptedThing::PlayCombatAnimation(const CCharString& animation, bool b1, bool clearActions, bool b3, bool b4, bool b5, bool b6) -- queue a combat animation for the live scripted thing, optionally clearing its scripted actions and finishing a creature's current action first
// Leaf: PlayCombatAnimation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00904AF0, length 224 bytes (`ret 1Ch`), vtable 0x0128D86C slot 19.
// PDB: ?PlayCombatAnimation@CScriptGameResourceObjectScriptedThing@@UAEXABVCCharString@@_N11111@Z.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PThing at +0x14), CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, interface 0x1f = CTCScriptedControl),
// CTCScriptedControl::CleanupAllActions @ 0x00713630, CThingCreatureBase::FinishCurrentAction @ 0x00662650,
// operator new @ 0x00BFEA1A (0x18 bytes), NTCScriptedControl::CActionPlayCombatAnimation::CActionPlayCombatAnimation
// (const CCharString&, bool, bool, bool, bool, bool) @ 0x00903590 (vtable 0x0128D728), CTCScriptedControl::AddAction @ 0x007137D0.
// Shape: the family guard and the unchecked GetInterface (key in the `push ecx` word, miss read from the
// `clearActions` slot); the third argument is the clear flag and is not forwarded; a creature (ThingTypeFlags
// bit 3 at +0x6c) also finishes its current action, each access re-fetching the item.
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

class CScriptThing {                               // vtable 0x01238C8C
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual CThing_Retail* GetPThing() const;      // slot 11 (+0x2c) 0x004AAA60
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26();
    virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46();
    virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51();
    virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56();
    virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61();
    virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66();
    virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74();
    virtual bool IsAlive() const;                  // slot 75 (+0x12c) 0x004AB130
};

class CTCScriptedControl;

namespace NTCScriptedControl {
struct CActionBase {                               // vtable 0x01265504
    virtual void v00();
};
}


class CCharString;

class CThingCreatureBase { public:
    void FinishCurrentAction();                   // 0x00662650
};

class CTCScriptedControl { public:                       // interface 0x1f component
    void CleanupAllActions();                     // 0x00713630
    void AddAction(NTCScriptedControl::CActionBase* action);   // 0x007137D0
};

namespace NTCScriptedControl {
struct CActionPlayCombatAnimation : CActionBase {              // vtable 0x0128D728, 0x18 bytes
    unsigned char _body[0x14];
    CActionPlayCombatAnimation(const CCharString& animation, bool a, bool b, bool c, bool d, bool e);   // 0x00903590
};
}

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    void PlayCombatAnimation(const CCharString& animation, bool b1, bool clearActions, bool b3, bool b4, bool b5, bool b6);
};

void CScriptGameResourceObjectScriptedThing_Methods::PlayCombatAnimation(const CCharString& animation, bool b1, bool clearActions, bool b3, bool b4, bool b5, bool b6)
{
    CIntelligentPointer<CThing_Retail>* thing = (CIntelligentPointer<CThing_Retail>*)PThing;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            CTCScriptedControl* control;
            thing->GetPItem()->GetInterface(&control, 0x1f);
            if (clearActions) {
                control->CleanupAllActions();
                if (thing->GetPItem()->ThingTypeFlags & 8)
                    ((CThingCreatureBase*)thing->GetPItem())->FinishCurrentAction();
            }
            control->AddAction(new NTCScriptedControl::CActionPlayCombatAnimation(animation, b1, b3, b4, b5, b6));
        }
    }
}