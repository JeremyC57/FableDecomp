// CScriptGameResourceObjectScriptedThing::PlayLoopingAnimation(const CCharString& animation, long numLoops, bool useMovement, bool clearActions, bool addAsQueuedAction, bool waitForAnimationToFinish, bool usePhysics, bool animMayNeedCameraPositionUpdated, bool allowLooking) -- queue a looping animation for the live scripted thing at the next action priority, optionally clearing its scripted actions and finishing a creature's current action first
// Retry 10b (chain-split sweep, FIXES B-16): the `GetPItem()` result is a named local `item` before it is dereferenced, which gives retail's register colouring/order (the chained call gives the parked difference).
// Leaf: PlayLoopingAnimation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00904BD0, length 253 bytes (`ret 24h`), vtable 0x0128D86C slot 20.
// PDB: ?PlayLoopingAnimation@CScriptGameResourceObjectScriptedThing@@UAEXABVCCharString@@J_N111111@Z.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PThing at +0x14), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020 (inlined GetInterface, interface 0x1f), CTCScriptedControl::CleanupAllActions @ 0x00713630, CThingCreatureBase::
// FinishCurrentAction @ 0x00662650, operator new @ 0x00BFEA1A (0x28 bytes), NTCScriptedControl::CActionPlayAnimation::
// CActionPlayAnimation(const CCharString&, bool, bool, long, bool, long, bool x5) @ 0x009034F0, CTCScriptedControl::AddAction
// @ 0x007137D0.
// Shape: the landed PlayCombatAnimation 0x00904AF0 shape (unchecked GetInterface, the miss reading the `clearActions` slot,
// the creature test on ThingTypeFlags bit 3); the priority argument is `ActionPriority++` (old value pushed, the increment
// stored inside the new-expression's non-null arm); the new-expression's null result duplicates the AddAction tail.
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

namespace NTCScriptedControl {
struct CActionBase {                               // vtable 0x01265504
};
}

class CCharString;

class CThingCreatureBase { public:
    void FinishCurrentAction();                    // 0x00662650
};

class CTCScriptedControl { public:                        // interface 0x1f component
    void CleanupAllActions();                      // 0x00713630
    void AddAction(NTCScriptedControl::CActionBase* action);   // 0x007137D0
};

namespace NTCScriptedControl {
struct CActionPlayAnimation : CActionBase {        // vtable 0x0128D6F4, 0x28 bytes
    unsigned char _body[0x28];
    CActionPlayAnimation(const CCharString& animation, bool stayOnLastFrame, bool looping, long numLoops,
                         bool useMovement, long priority, bool addAsQueuedAction, bool waitForAnimationToFinish,
                         bool usePhysics, bool animMayNeedCameraPositionUpdated, bool allowLooking);   // 0x009034F0
};
}

struct CScriptGameResourceObjectScriptedThing_Methods : CScriptGameResourceObjectScriptedThing {
    void PlayLoopingAnimation(const CCharString& animation, long numLoops, bool useMovement, bool clearActions, bool addAsQueuedAction,
                              bool waitForAnimationToFinish, bool usePhysics, bool animMayNeedCameraPositionUpdated, bool allowLooking);
};

void CScriptGameResourceObjectScriptedThing_Methods::PlayLoopingAnimation(const CCharString& animation, long numLoops, bool useMovement, bool clearActions, bool addAsQueuedAction,
                                                                         bool waitForAnimationToFinish, bool usePhysics, bool animMayNeedCameraPositionUpdated, bool allowLooking)
{
    CIntelligentPointer<CThing_Retail>* thing = (CIntelligentPointer<CThing_Retail>*)PThing;
    if (thing->GetPItem()) {
        CThing_Retail* item = thing->GetPItem();
        if (!item->m_dead) {
            CTCScriptedControl* control;
            thing->GetPItem()->GetInterface(&control, 0x1f);
            if (clearActions) {
                control->CleanupAllActions();
                if (thing->GetPItem()->ThingTypeFlags & 8)
                    ((CThingCreatureBase*)thing->GetPItem())->FinishCurrentAction();
            }
            control->AddAction(new NTCScriptedControl::CActionPlayAnimation(animation, false, true, numLoops, useMovement, ActionPriority++,
                                                                            addAsQueuedAction, waitForAnimationToFinish, usePhysics,
                                                                            animMayNeedCameraPositionUpdated, allowLooking));
        }
    }
}