// NCreatureMode::CModeBriarRoseConcentrate::FrameUpdate() -- base update; unless she was hit, count the concentration timer down and, at zero, start the drain-life "concentrate out of" action on a live target; when hit (or the action is refused) remove creature mode 0x3f
// Leaf: ?FrameUpdate@CModeBriarRoseConcentrate
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E8760, 203 bytes (+ int3 padding), `ret`; vtable 0x01268548 slot 3.
// Callees: CModeBase::FrameUpdate @ 0x0073AC50 (qualified), CModeBriarRoseConcentrate::FindHitEvent @ 0x008E79A0 (landed),
//   CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times), CCreatureAction_BriarRoseConcentrateOutof ctor
//   (CThingCreatureBase*, CThingCreatureBase*) @ 0x008C2CB0 (landed; retail sizeof 0x11c), CThingCreatureBase::SetCurrentAction
//   @ 0x006644F0, ~CCombatActionBase @ 0x0062E900, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x31 = bit
//   17 of the dword at +0x24 = the creature-mode manager), CTCCreatureModeManager::RemoveMode @ 0x0073ABA0.
// Name: FableWin.pdb `?FrameUpdate@CModeBriarRoseConcentrate@NCreatureMode@@UAEXXZ` (slot 3).
// Layout (retail; Ego_r.pdb CModeBriarRoseConcentrate less 4 after the 12-byte retail vector): CModeStandBase 0x68, Filter +0x68,
//   Timer +0x74, PTarget +0x78 (CIntelligentPointer<CThingCreatureBase>, 8 bytes), Imposters +0x80, Imposter +0x8c.
// Shape: as the landed sibling CModeBriarRoseTeleportInvulnerable::FrameUpdate @ 0x008E8830; here the action temporary is
//   built in a plain statement (no `&&` around it, so no conditional-destruction flag); the action class declares no dtor,
//   so its generated dtor is just the ~CCombatActionBase call; the SetCurrentAction result rides in bl across it. All the
//   fall-through paths join at the RemoveMode block (out-parameter GetInterface, component in eax then ecx).
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CCreatureActionBase;

class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

class CTCCreatureModeManager { public:            // interface 0x31
    void RemoveMode(unsigned long mode);   // 0x0073ABA0
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
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};

class CCreatureActionBase { public:               // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa4];
};
class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char _combat[0x6c];
    virtual ~CCombatActionBase();                  // 0x0062E900
};
class CCreatureAction_CastWillDrainLifeSpell : public CCombatActionBase { public:   // retail sizeof 0x11c
    unsigned char _spell[0x08];
};
class CCreatureAction_BriarRoseConcentrateOutof : public CCreatureAction_CastWillDrainLifeSpell { public:
    CCreatureAction_BriarRoseConcentrateOutof(CThingCreatureBase* owner, CThingCreatureBase* target);   // 0x008C2CB0
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThingCreatureBase* ParentThing;       // +0x04
    unsigned char       _body[0x14];       // WeaponBasedFilter, FramesBeforeRemovingSelf, Active
    void FrameUpdate();                    // 0x0073AC50
};
struct CModeStandBase : CModeBase {        // retail sizeof 0x68
    unsigned char _stand[0x4c];
};

namespace NCreatureMode {
class CModeBriarRoseConcentrate : public CModeStandBase { public:   // vtable 0x01268548
    unsigned char           Filter[0x0c];  // +0x68 CAnimFilterOfGroup
    long                    Timer;         // +0x74
    CIntelligentPointer<CThingCreatureBase> PTarget;       // +0x78 CIntelligentPointer<CThingCreatureBase>
    unsigned char           Imposters[0x0c];   // +0x80 vector<CIntelligentPointer<CThing> > (12-byte retail vector)
    bool                    Imposter;      // +0x8c
    bool FindHitEvent();                   // 0x008E79A0
    void FrameUpdate();
};
}

void NCreatureMode::CModeBriarRoseConcentrate::FrameUpdate()
{
    CModeBase::FrameUpdate();
    if (!FindHitEvent()) {
        if (Imposter)
            return;
        if (--Timer > 0)
            return;
        if (PTarget.GetPItem() != 0 && !PTarget.GetPItem()->IsDead()) {
            CThingCreatureBase* owner = ParentThing;
            bool started = owner->SetCurrentAction(CCreatureAction_BriarRoseConcentrateOutof(owner, PTarget.GetPItem()));
            if (started)
                return;
        }
    }
    CTCCreatureModeManager* modes;
    if (ParentThing->GetTC(TCI_ENTITY_MODE_MANAGER, &modes))
        modes->RemoveMode(0x3f);
}