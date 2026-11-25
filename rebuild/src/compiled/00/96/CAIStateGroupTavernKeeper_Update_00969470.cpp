// CAIStateGroup_TavernKeeper::Update() -- close the keeper's shop stock and reset the 0x23 component, walk back inside the pub if outside, then loop: serve payment requests (old jug point + receive payment), refill empty jug slots on the bar (fetch the jug, walk to it, fill it), or idle at random (do nothing / lean on the bar); terminates when stopped or any step fails
// Leaf: ?Update@CAIStateGroup_TavernKeeper@@MAEXXZ
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00969470, length 698 bytes (aligned `and esp,-8` frame, virtual slot 12).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface: 0x90 shop keeper, 0x23, 0xb9 tavern bar),
//   CTCShopKeeper::GetShopTC @ 0x00798F50 (x2), CTCShop::SetStockState @ 0x00773C00, the 0x23 component's 0x007EE540
//   (manifest IS_THING_ALIVE<...> label is propagated), CThingBuilding::IsPointInside @ 0x0082D000, CThing::GetPosition
//   @ 0x004C73D0 / GetPhysPos @ 0x004C7950, AIAction_MoveToPosProximity @ 0x0097E890, GetPaymentRequest 0x009683A0,
//   GetPaymentPoint 0x00968860, ReceivePayment 0x009692C0, the brain's slot 2, IsStopping 0x008FD010, Terminate
//   0x008FCF40, CTCTavernBar::GetEmptyJugSlot 0x007BA1D0 / GetJug 0x007B9F60 (returns the jug pointer by value),
//   WalkToPoint 0x00968230, FillJug 0x00968FA0, FreeBarPoint 0x00968950, GetPWorldSeed 0x008FCFD0 + GFROR13
//   0x00497890 (inline random step), AIAction_DoNothing 0x0097E200, LeanOnBar 0x009689C0, CBaseIntelligentPointer.
// Layout: engine/CAIStateGroup_TavernKeeper.h over the landed LeanOnBar 0x009689C0 model; the pub's physics body +0x60
//   holds the walk radius at +0x34.
// Shape: C3DVector has a user copy ctor so GetPhysPos builds straight into the by-value argument (`sub esp,0Ch ;
//   push esp`); the pub pointer (IsPointInside) and the pub thing for GetPhysPos are named locals fetched before the
//   argument pushes; the random idle is the landed inline seed step with `switch (r % 2)`.
class CTCBase;
#include "engine/C3DVector.h"
class CRightHandedSet { public: float m[6]; };
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCPhysicsBase;

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CBaseObject*a0);                // 0x00A01B90
    CBaseIntelligentPointer& operator=(const CBaseIntelligentPointer& other) { SetPItem((CBaseObject*)(((CThing*)other.GetPItem()))); return *this; }
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
};

float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
class CThingCreatureBase;
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
class CCreatureAction_PlayIntoLoopOutOfAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x14];
    CCharString LoopAnimName;                   // +0xbc
    CCharString OutOfAnimName;                  // +0xc0
    CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase* creature, const CCharString& into,
        const CCharString& loop, const CCharString& outOf, float loopTime, bool b, long c, long d);   // 0x008427E0
};
class CAIBrain { public:
    virtual void v00(); virtual void v01();
    virtual void WaitForNextUpdate();           // slot 2 (+0x08)
    unsigned char _pad[0x18 - 4];
    CThing* PCreature;                          // +0x18
};
// NF51 (round 7): the AI state-group base comes from the shared engine/CAIStateGroupBase.h (was a local CAIStateGroupBase model
// with long / bool* / CThing* parameters, and an empty local EAIActionReturn): the callee 0x0097E890 is the protected
// ?AIAction_MoveToPosProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@MAA_NW4EMovementType@@_N44PBVCThing@@4M@Z
// (Ego_r.pdb / FableWin.pdb; header _MEMBERS + _AIACTIONS + _MOVETOPOSPROXIMITY) with the Ego_r enums of engine/fable_ai_action_enums.h.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroupBase.h"

long __fastcall GFROR13(unsigned long value);   // 0x00497890
class CTCShop { public:
    void SetStockState(long state);             // 0x00773C00
};
class CTCShopKeeper { public:                          // interface 0x90
    CTCShop* GetShopTC();                       // 0x00798F50
};
class CTCVillage;
class CTCVillageMember { public:
    CTCVillage* GetPVillage() const;   // 0x007EE540 (Ego_r.pdb ?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ)
};
struct CTCScratch23 {                           // interface 0x23
};
struct CTCPhysicsBase {
    unsigned char _pad[0x34];
    float Radius;                               // +0x34
};
struct CThingBuildingModel : CThing {
    bool IsPointInside(const C3DVector& pos);   // 0x0082D000
};
struct CThingPos : CThing {                     // no data of its own: PBody is CThing's +0x60
};
struct CIntelligentPointerObject : CBaseIntelligentPointer {     // CIntelligentPointer<CThingObject>
    virtual void v00();
};
class CTCTavernBar { public:                           // interface 0xb9
    bool GetEmptyJugSlot(CThing& creature, C3DVector& pos, CRightHandedSet& rh, long& actionPoint, long& slot);   // 0x007BA1D0 (Ego_r / FableWin QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@AAJ3@Z)
    CIntelligentPointerObject GetJug(long slot);   // 0x007B9F60
};
struct CAIStateGroup_TavernKeeper : CAIStateGroupBase {
    CThing* GetCreature() const { return PBrain->PCreature; }   // (the row's helper, moved here from its local base model)
    CBaseIntelligentPointer PJug;               // +0x14 CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PBar;               // +0x1c CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PPub;               // +0x24 CIntelligentPointer<CThingBuilding>
    CBaseIntelligentPointer PPayee;             // +0x2c CIntelligentPointer<CThingAICreature>
    long JugActionPointIndex;                   // +0x34
    C3DVector JugPos;                           // +0x38
    CRightHandedSet JugRHSet;                   // +0x44
    long JugSlot;                               // +0x5c
    bool WalkToPoint(const C3DVector& pos, const CRightHandedSet& rh);   // 0x00968230
    bool GetPaymentRequest();                   // 0x009683A0
    bool GetPaymentPoint();                     // 0x00968860
    void FreeBarPoint();                        // 0x00968950
    bool LeanOnBar();                           // 0x009689C0
    bool FillJug();                             // 0x00968FA0
    bool ReceivePayment();                      // 0x009692C0
    protected:
    virtual void Update();
    public:
};

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x004C7950 ?GetPhysPos@CThing@@QBE?AVC3DVector@@XZ (was ?GetPhysPos@CThingPos@@QBE?AVC3DVector@@XZ)
void CAIStateGroup_TavernKeeper::Update()
{
    CTCShopKeeper* shopKeeper;
    if (GetCreature()->GetTC(TCI_SHOP_KEEPER, &shopKeeper) && shopKeeper->GetShopTC())
        shopKeeper->GetShopTC()->SetStockState(0);
    CTCScratch23* scratch;
    GetCreature()->GetTC(TCI_VILLAGE_MEMBER, &scratch);
    ((CTCVillageMember*)scratch)->GetPVillage();
    CThingPos* creature = (CThingPos*)GetCreature();
    CThingBuildingModel* pub = (CThingBuildingModel*)PPub.GetPItem();
    if (!pub->IsPointInside(creature->GetPos())) {
        bool arrived;
        CTCPhysicsBase* body = ((CThingPos*)PPub.GetPItem())->GetPhysicsTC();
        CThingPos* pubThing = (CThingPos*)PPub.GetPItem();
        if (AIAction_MoveToPosProximity(ACTION_REPEAT_UNTIL_FINISHED, reinterpret_cast<const CThing*>(pubThing)->GetPhysPos(), body->Radius, arrived, AI_MOVEMENT_WALKING, true,
                false, true, 0, true, 0.0f) != 2) {
            Terminate();
            return;
        }
    }
    CTCTavernBar* bar;
    ((CThing*)PBar.GetPItem())->GetTC(TCI_TAVERN_BAR, &bar);
    for (;;) {
        if (GetPaymentRequest()) {
            if (!GetPaymentPoint()) {
                PBrain->WaitForNextUpdate();
                if (IsStopping()) {
                    Terminate();
                    return;
                }
                continue;
            }
            if (!ReceivePayment()) {
                if (IsStopping()) {
                    Terminate();
                    return;
                }
                PBrain->WaitForNextUpdate();
                if (IsStopping()) {
                    Terminate();
                    return;
                }
                continue;
            }
        }
        if (bar->GetEmptyJugSlot(*GetCreature(), JugPos, JugRHSet, JugActionPointIndex, JugSlot)) {
            PJug = bar->GetJug(JugSlot);
            if (!WalkToPoint(JugPos, JugRHSet)) {
                FreeBarPoint();
                Terminate();
                return;
            }
            if (!FillJug()) {
                FreeBarPoint();
                Terminate();
                return;
            }
            FreeBarPoint();
            continue;
        }
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        switch (rotated % 2) {
        case 0:
            if (AIAction_DoNothing() == 1) {
                Terminate();
                return;
            }
            break;
        case 1:
            if (!LeanOnBar()) {
                Terminate();
                return;
            }
            break;
        }
    }
}