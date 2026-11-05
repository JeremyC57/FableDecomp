// CAIStateGroup_StealStealableItem::Update() -- claim a free action point on the stealable item, walk to it (or near the item), face it, pick it up into the inventory (dropping whatever was carried), then carry it to the escape point and destroy it if the escape point says so
// Leaf: ?Update@CAIStateGroup_StealStealableItem@@UAEXXZ
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z; 004365b0: ?LowerBound@CInterfaceMap@@QBEPBUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Grade: RELOCATION_MATCH -- retry wave 2 (sec4): the +0x91 dead test through CThing::IsDead() const (Ego_r.pdb
//   ?IsDead@CThing@@QBE_NXZ, a bool accessor) instead of a direct bit test; the bool accessor keeps retail's register
//   choice / test-and-branch shape.
class CTCBase;
#include "engine/C3DVector.h"
class CRightHandedSet { public:
    C3DVector Forward;
    C3DVector Up;
};
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingCreatureBase : public CThing { public:
};
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();   // slot 0 (retail scalar deleting dtor; was placeholder v00); 0x00A01C10
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    CIntelligentPointerThing(CThing* item);     // 0x004A96F0
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& o);   // 0x004A9760
};
template <class T> class CIntelligentPointer;
template <> class CIntelligentPointer<const CThing> : public CBaseIntelligentPointer { public:   // Ego_r: PEscapePoint is CIntelligentPointer<CThing const>
};
typedef CIntelligentPointer<const CThing> CIntelligentPointerConstThing;
bool __fastcall IS_THING_ALIVE(const CIntelligentPointerThing& p);        // 0x004CC340
template <class T> bool __fastcall IS_THING_ALIVE(const T& p);              // 0x0069CE20 = IS_THING_ALIVE<CIntelligentPointer<const CThing> > (Ego_r / FableWin)
struct CTCUserActionPointBase {
    bool GetFreeActionPoint(CThingCreatureBase* creature, C3DVector* pos, CRightHandedSet* set, long* index);   // 0x007FEDE0
};
struct CTCStealable {                           // interface 0x47
    unsigned char _pad[0x10];
    void* POwnerAction;                         // +0x10
};
class CTCCarrying { public:                            // interface 0x46
    CThing* GetThingInPrimarySlot() const;            // 0x006A1440
    void RemoveThingFromPrimarySlot(bool drop); // 0x006A1E00
    bool IsCarryingThing(CThing* thing);        // 0x006A1060
};
struct CTCEscapeFlags {                         // interface 0xAA
    unsigned char _pad[0x6b];
    bool DestroyCarried;                        // +0x6B
};
class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();             // Ego_r.pdb ??1CCreatureActionBase@@UAE@XZ (vptr at +0)
    long GetActionID() const;                   // 0x00692620 (Ego_r.pdb ?GetActionID@CCreatureActionBase@@QBEJXZ)
};
class CCreatureAction_PickUpGenericBox : public CCreatureActionBase { public:   // vtable 0x01274BD4 (RTTI), sizeof 0xb0
    unsigned char _data[0xb0 - 4];              // PThingToPickUp CIntelligentPointer<CThing> at +0xa8
    CCreatureAction_PickUpGenericBox(CThingCreatureBase& owner, CThing& thing);   // 0x00843700 (Ego_r.pdb ??0CCreatureAction_PickUpGenericBox@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z; "ST_PICK_UP_GENERIC_BOX")
    ~CCreatureAction_PickUpGenericBox();        // 0x007120D0 (destroys PThingToPickUp +0xa8, then ~CCreatureActionBase)
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    void Terminate();                           // 0x008FCF40
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float speed);   // 0x0097D580
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool b);        // 0x0097DB70
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long a, bool b, bool c, bool d, bool e, long f);   // 0x0097EC00
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float prox, bool* reached, long a, bool b, bool c, bool d, long e, bool f, float g);   // 0x0097E890
};
struct CAIStateGroup_StealStealableItem : CAIStateGroupBase {
    CIntelligentPointerThing PStealableItem;            // +0x14
    CIntelligentPointerConstThing PEscapePoint;         // +0x1c
    CIntelligentPointerThing PPreviouslyCarriedItem;    // +0x24
    long ActionPointIndex;                      // +0x2c
    bool GotActionPoint;                        // +0x30
    bool ReachedDestination;                    // +0x31
    long PickUpActionID;                        // +0x34
    bool NeedsCleanup;                          // +0x38
    virtual void Update();
};

void CAIStateGroup_StealStealableItem::Update()
{
    if (!PStealableItem.GetPItem() || PStealableItem.GetPItem()->IsDead()) {
        Terminate();
        return;
    }
    CTCUserActionPointBase* actionPoint;
    PStealableItem.GetPItem()->GetTC(TCI_SINGLE_USER_ACTION_POINTS, &actionPoint);
    C3DVector pos;
    CRightHandedSet set;
    if (!actionPoint->GetFreeActionPoint(GetCreature(), &pos, &set, &ActionPointIndex)) {
        Terminate();
        return;
    }
    GotActionPoint = true;
    C3DVector facing = set.Up;
    CTCStealable* stealable;
    PStealableItem.GetPItem()->GetTC(TCI_CARRYABLE, &stealable);
    bool arrived;
    long result;
    do {
        if (!PStealableItem.GetPItem() || PStealableItem.GetPItem()->IsDead()) {
            Terminate();
            return;
        }
        if (stealable->POwnerAction != 0) {
            Terminate();
            return;
        }
        bool reached;
        result = AIAction_MoveToPos(2, pos, &reached, 5, true, true, false, true, 0);
    } while (result == 4);
    if (result == 3) {
        do {
            if (!PStealableItem.GetPItem() || PStealableItem.GetPItem()->IsDead()) {
                Terminate();
                return;
            }
            if (stealable->POwnerAction != 0) {
                Terminate();
                return;
            }
            CThing* item = PStealableItem.GetPItem();
            bool reached;
            result = AIAction_MoveToPosProximity(2, ((CThing*)item)->GetPos(), 1.0f, &reached, 5, true, false, true, 0, true, 0.0f);
        } while (result == 4);
        if (IS_THING_ALIVE(PStealableItem)) {
            CThingCreatureBase* creature = GetCreature();
            CThing* item = PStealableItem.GetPItem();
            facing = ((CThing*)item)->GetPos() - ((CThing*)creature)->GetPos();
        }
    }
    if (!IS_THING_ALIVE(PStealableItem)) {
        Terminate();
        return;
    }
    if (stealable->POwnerAction != 0) {
        Terminate();
        return;
    }
    if (result != 2) {
        Terminate();
        return;
    }
    result = AIAction_LookAtPos(1, ((CThing*)(GetCreature()))->GetPos() + facing, 0x21, 0.0027777f);
    if (!IS_THING_ALIVE(PStealableItem)) {
        Terminate();
        return;
    }
    if (stealable->POwnerAction != 0) {
        Terminate();
        return;
    }
    if (result != 2) {
        Terminate();
        return;
    }
    CCreatureAction_PickUpGenericBox action(*GetCreature(), *PStealableItem.GetPItem());
    PickUpActionID = action.GetActionID();
    CTCCarrying* carrying;
    GetCreature()->GetTC(TCI_CARRYING, &carrying);
    {
        CIntelligentPointerThing previous(carrying->GetThingInPrimarySlot());
        PPreviouslyCarriedItem = previous;
    }
    carrying->RemoveThingFromPrimarySlot(false);
    if (IS_THING_ALIVE(PPreviouslyCarriedItem) && PPreviouslyCarriedItem.GetPItem()->HasTC(TCI_WEAPON))
        PPreviouslyCarriedItem.GetPItem()->Kill(true);
    long done = AIAction_PerformActionAndWait(&action, false);
    if (IS_THING_ALIVE(PEscapePoint) && IS_THING_ALIVE(PStealableItem) && done == 2
        && carrying->IsCarryingThing(PStealableItem.GetPItem())) {
        C3DVector escapePos = ((CThing*)(PEscapePoint.GetPItem()))->GetPos();
        if (AIAction_MoveToPosProximity(1, escapePos, 1.0f, &arrived, 5, true, false, true, 0, true, 0.0f) == 2) {
            ReachedDestination = true;
            const const CTCEscapeFlags* flags;
            static_cast<const CThing*>(GetCreature())->PeekTC(TCI_AI_SCRATCHPAD, &flags);
            if (IS_THING_ALIVE(PStealableItem) && flags->DestroyCarried)
                PStealableItem.GetPItem()->Kill(false);
        }
    }
    Terminate();
}