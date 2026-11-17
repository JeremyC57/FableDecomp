// CAIStateGroupBase::AIAction_MoveToPossiblyBlockedObjectAndFace(thing, index, proximity, seconds, filter, route, avoid, speed)
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Grade: RELOCATION_MATCH (1117v1117)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00980770, 1120 bytes, ret 0x24.
// Shape: alive + filter guard, user-action-point interface 0x2d, optional pre-calculated route, approach the thing,
//   then poll GetFreeActionPoint (minion wander while waiting, timeout = fps * seconds frames), require that nobody carries
//   the thing (its CTCCarryable, interface 0x47, has no PCarryingTC), walk to the point and turn to face along its orientation; the point is freed on every failure.
// Notes: each `bool reached` lives in its own scope (retail packs it into the dead `route` argument slot); the
//   final branch is written inverted (`result != 2 || !filter()` first) so the route-failure return cross-jumps to
//   the same tail retail picks; retry loops use `if (r == 4) continue; break;`.
// Name fix (agentNF5 2026-11-10): CAIStateGroupBase::AIAction_LookAtPos 0x0097D580 is protected, EAIActionReturn(EAIActionRepeat, C3DVector, long, float) (?AIAction_LookAtPos@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@JM@Z, Ego_r.pdb / FableWin.pdb).
// Name fix (agentNF5 2026-11-10): CAIStateGroupBase::AIAction_MoveToPos 0x0097EC00 is protected, EAIActionReturn(EAIActionRepeat, C3DVector, bool&, EMovementType, bool, bool, const CThing*, bool, float) (Ego_r.pdb / FableWin.pdb).
enum EMovementType;                           // MS forward enum
enum EAIActionRepeat;                         // MS forward enum
enum EAIActionReturn;                         // MS forward enum
#include "fable_compiler.h"
class CTCBase;
#include "engine/C3DVector.h"
class CRightHandedSet { public:
    C3DVector Up;                               // +0x00
    C3DVector Forward;                          // +0x0c
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0x3d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;    // 0x004C73D0
    FABLE_FORCEINLINE bool TestTC(long type) const { return ((m_presentBits[type >> 5] >> (type & 31)) & 1) != 0; }
    FABLE_FORCEINLINE CTCBase* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(key);
        return (it == map->end() || type < it->first ? map->end() : it)->second;
    }
};
class CThingPhysical : public CThing { public:
    const C3DVector& PeekPos() const;       // 0x006A4D00
};
class CThingCreatureBase : public CThingPhysical { public:
};
class CThingBuilding;
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
template <class T> bool __fastcall IS_THING_ALIVE(const T& p);              // Ego_r / FableWin: T = CIntelligentPointer<CThing> 0x004CC340
struct CTCUserActionPointBase {
public:
    void SetActionPointAsFree(long a0);   // 0x007FEF40 (Ego_r.pdb ?SetActionPointAsFree@CTCUserActionPointBase@@QAEXJ@Z)
public:                 // interface 0x2d
    bool GetFreeActionPoint(CThing& user, C3DVector& position, CRightHandedSet& orientation, long& index);   // 0x007FEDE0 (NF35: Ego_r + FableWin ..QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@AAJ@Z)
};
class CTCCarrying;
class CTCCarryable {                            // interface 0x47 (vtable 0x0123B35C slot 21: `push 47h ; pop eax`)
public:
    unsigned char _pad[0x10];
    CTCCarrying* PCarryingTC;                   // +0x10 (Ego_r.pdb CTCCarryable; the carrier, null when not carried)
};
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
class CWorld { public: static long __fastcall GetFrame(); };              // 0x0049D870
// NF51 (round 7): the AI state-group base comes from the shared engine/CAIStateGroupBase.h (was a local CAIStateGroupBase model with
// long returns / movement, a local global CAIFilterBase and a local CIntelligentPointerThing): the callees 0x0097E890
// ?AIAction_MoveToPosProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@MAA_NW4EMovementType@@_N44PBVCThing@@4M@Z
// and 0x0097F690 ?AIAction_MoveToPreCalcRoute@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCThing@@AA_NW4EMovementType@@ABVCAIFilterBase@1@_N@Z
// are the header's protected members (Ego_r.pdb / FableWin.pdb; _MEMBERS + _AIACTIONS + _MOVETOPOSPROXIMITY + _FILTER, with
// _MEALACTIONS for AIAction_MoveToPos 0x0097EC00 / AIAction_LookAtPos 0x0097D580 and _MINIONWANDER 0x0097F870) with the Ego_r enums of
// engine/fable_ai_action_enums.h; the filter is the header's nested CAIStateGroupBase::CAIFilterBase and the by-value thing the shared
// CIntelligentPointer<CThing> (IS_THING_ALIVE<CIntelligentPointer<CThing> > 0x004CC340); the brain is engine/CAIBrain.h (was a local
// CAIBrain model) and the creature read the header's inline protected GetCreature (_GETCREATURE + FABLE_RETAIL_CAIBRAIN_GETCREATURE). The function itself is the protected member of
// the overlay opt-in FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSSIBLYBLOCKED (Ego_r.pdb).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSPROXIMITY
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_FILTER
#define FABLE_RETAIL_CAISTATEGROUPBASE_MINIONWANDER
#define FABLE_RETAIL_CAISTATEGROUPBASE_MOVETOPOSSIBLYBLOCKED
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#include "engine/fable_ai_action_enums.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"

EAIActionReturn CAIStateGroupBase::AIAction_MoveToPossiblyBlockedObjectAndFace(CIntelligentPointer<CThing> thing, long& index, float proximity,
    float seconds, const CAIFilterBase& filter, const CThing* route, const CThing* avoid, float speed)
{
    if (((CThing*)thing.GetPItem()) == 0 || (((CThing*)thing.GetPItem())->m_flags91 & 1) || !filter())
        return AI_ACTION_FAILED;
    CThing* target = ((CThing*)thing.GetPItem());
    bool hasPoint = target->TestTC(0x2d);
    if (!hasPoint)
        return AI_ACTION_FAILED;
    CTCUserActionPointBase* point = (CTCUserActionPointBase*)target->GetTC(0x2d);
    long result;
    if (route) {
        bool reached;
        result = AIAction_MoveToPreCalcRoute(*route, reached, AI_MOVEMENT_WALKING, filter, true);
        if (result != 2)
            return (EAIActionReturn)result;
    }
    for (;;) {
        if (((CThing*)thing.GetPItem()) == 0 || (((CThing*)thing.GetPItem())->m_flags91 & 1) || !filter())
            return AI_ACTION_FAILED;
        CThing* item = ((CThing*)thing.GetPItem());
        bool reached;
        result = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, item->GetPos(), proximity, reached, AI_MOVEMENT_WALKING, true, false, true, 0, true, 0.0f);
        if (result == 2)
            break;
        if (result == 4)
            continue;
        return (EAIActionReturn)result;
    }
    if (!IS_THING_ALIVE(thing))
        return AI_ACTION_FAILED;
    long fps = CMainGameComponent::GetConstantFPS();
    unsigned long endFrame = CWorld::GetFrame() + (long)(fps * seconds);
    C3DVector pos;
    CRightHandedSet orientation;
    while (!point->GetFreeActionPoint((CThing&)GetCreature(), pos, orientation, index)) {
        if (AIAction_MinionWander(AI_MOVEMENT_WALKING, 0) == 1)
            return AI_ACTION_TERMINATE_PROCESS;
        if (CWorld::GetFrame() > endFrame)
            return AI_ACTION_FAILED;
        if (!IS_THING_ALIVE(thing) || !filter())
            return AI_ACTION_FAILED;
    }
    CThing* carried = ((CThing*)thing.GetPItem());
    bool isCarryable = carried->TestTC(0x47);
    if (!isCarryable || ((CTCCarryable*)carried->GetTC(0x47))->PCarryingTC != 0) {
        ((CTCUserActionPointBase*)point)->SetActionPointAsFree(index);
        return AI_ACTION_FAILED;
    }
    for (;;) {
        if (!IS_THING_ALIVE(thing))
            return AI_ACTION_FAILED;
        if (!filter()) {
            ((CTCUserActionPointBase*)point)->SetActionPointAsFree(index);
            return AI_ACTION_FAILED;
        }
        bool reached;
        result = AIAction_MoveToPos((EAIActionRepeat)(2), pos, reached, (EMovementType)(3), true, true, avoid, true, speed);
        if (result == 4)
            continue;
        break;
    }
    if (!IS_THING_ALIVE(thing))
        return AI_ACTION_FAILED;
    if (result != 2 || !filter()) {
        ((CTCUserActionPointBase*)point)->SetActionPointAsFree(index);
        if (result == 2)
            result = 3;
        return (EAIActionReturn)result;
    }
    C3DVector forward = orientation.Forward;
    C3DVector lookPos = forward + ((CThingCreatureBase*)PBrain->Creature)->PeekPos();
    for (;;) {
        if (!IS_THING_ALIVE(thing))
            return AI_ACTION_FAILED;
        if (!filter()) {
            ((CTCUserActionPointBase*)point)->SetActionPointAsFree(index);
            return AI_ACTION_FAILED;
        }
        result = AIAction_LookAtPos((EAIActionRepeat)(2), lookPos, 0x11, 0.01f);
        if (result == 4)
            continue;
        break;
    }
    if (!IS_THING_ALIVE(thing))
        return AI_ACTION_FAILED;
    if (result == 2 && filter())
        return (EAIActionReturn)result;
    ((CTCUserActionPointBase*)point)->SetActionPointAsFree(index);
    if (result == 2)
        result = 3;
    return (EAIActionReturn)result;
}