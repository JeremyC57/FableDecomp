// CAIStateGroup_SeekEnemy::IsStillRunnable() -- validate the remembered enemy against target selection and perception; retail 0x0093b8b0, 319 bytes. Callees: CVectorMap::LowerBound 0x004365b0, CBaseIntelligentPointer::GetPItem 0x00a01b50, CTCTargetingAI slot 29 thunk 0x004ddae5, IsWillingToAttackTarget 0x008648a0, CTCPerceiveThing::GetPerceivedThing 0x007446a0, CTCPerceiveThing::IsAwareOfThing 0x00643b3b, CanCreatureSeeCreature 0x008645e0. Frame globals 0x01375550 and 0x012925b8.
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_SeekEnemy.h"
#include "engine/CThingAICreature.h"
#define CTCPerceiveThing CTCPerceiveThingLayout
#include "engine/CTCPerceiveThing.h"
#undef CTCPerceiveThing

class CTCPerceiveThing : public CTCPerceiveThingLayout
{
public:
    bool IsAwareOfThing() const;   // 0x00643B3B
};
#include "engine/CTCBase.h"
#include "engine/_quarantine/CThing.h"

#if defined(_MSC_VER)
#define FABLE_FORCEINLINE __forceinline
#define FABLE_NATIVE_THISCALL __fastcall
#else
#define FABLE_FORCEINLINE inline
#define FABLE_NATIVE_THISCALL
#endif

struct CInterfaceMap {
    struct Entry { fable_i32 Type; void* Component; };
    Entry* First;
    Entry* Last;
    Entry* LowerBound(const fable_i32* type);                 // 0x004365b0
};

// The generated creature header is real; its native interface set/map remain opaque.
struct CThingAICreatureComponentAccess : CThingAICreature {
    FABLE_FORCEINLINE fable_u32* ComponentBits() {
        return (fable_u32*)((char*)this + 0x20);
    }
    FABLE_FORCEINLINE CInterfaceMap* ComponentMap() {
        return (CInterfaceMap*)((char*)this + 0x44);
    }
};

class CTCTargetingAI : public CTCBase { public:}; // real slot29 GetTarget (+0x74); actual shared component header
typedef CThing* (CTCTargetingAI::*TargetMethod)();
union TargetMethodSlot { void* Code; TargetMethod Method; };
struct TargetVTable { void* OtherSlots[29]; TargetMethodSlot GetTarget; };
FABLE_FORCEINLINE CThing* GetTargetFromSlot29(CTCTargetingAI* targeting) {
    return (targeting->*((TargetVTable*)targeting->__vftable)->GetTarget.Method)();
}
#if defined(_MSC_VER)
typedef char NativeTargetPMFSize[sizeof(TargetMethod)==4 ? 1:-1];
#endif

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CTCPerceiveThingAccess : CTCPerceiveThing {
    CThing* GetPerceivedThing();                               // direct 0x007446a0 -> 0x00a01b50
};

struct CThingNativeState : CThing {
    FABLE_FORCEINLINE bool IsDead() const {
        return (*(const fable_u8*)((const char*)this + 0x91) & 1u) != 0;
    }
};

extern bool FABLE_FASTCALL IsWillingToAttackTarget(CThingAICreature*, CThing*); // 0x008648a0
// 0x008645E0 Ego_r.pdb + FableWin.pdb ?IsAwareOfThing@NCreatureAI@@YI_NPBVCThingAICreature@@PBVCThing@@_N@Z (name-fix 2026-11-12)
namespace NCreatureAI { bool FABLE_FASTCALL IsAwareOfThing(const CThingAICreature* creature, const CThing* thing, bool flag); }
extern fable_i32 g_FrameForTargetPerception;                   // 0x01375550
extern float g_TargetPerceptionFrameScale;                     // 0x012925b8

struct CAIStateGroup_SeekEnemyAccess : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;                              // matches CAIStateGroup_SeekEnemy::PEnemy at +0x14
    FABLE_FORCEINLINE CAIBrain* GetBrain() const { return PBrain; }
    FABLE_FORCEINLINE CThingAICreatureComponentAccess* Creature() const {
        return (CThingAICreatureComponentAccess*)GetBrain()->Creature;
    }
    bool IsStillRunnable();
};

bool CAIStateGroup_SeekEnemyAccess::IsStillRunnable()
{
    CThingAICreatureComponentAccess* creature = Creature();
    CTCPerceiveThingAccess* perception = 0;
    fable_i32 type;
    if (creature->ComponentBits()[1] & 0x00100000u) {
        CInterfaceMap* map = creature->ComponentMap();
        CInterfaceMap::Entry* it;
        type = 0x34;
        it = map->LowerBound(&type);
        CInterfaceMap::Entry* last = map->Last;
        if (it == last || it->Type > 0x34)
            it = last;
        perception = (CTCPerceiveThingAccess*)it->Component;
    }

    CTCTargetingAI* targetSource;
    if (creature->ComponentBits()[0] & 0x100u) {
        CInterfaceMap* map = creature->ComponentMap();
        CInterfaceMap::Entry* it;
        type = 8;
        it = map->LowerBound(&type);
        CInterfaceMap::Entry* last = map->Last;
        if (it == last || it->Type > 8)
            it = last;
        targetSource = (CTCTargetingAI*)it->Component;
    }

    if (((CThing*)PEnemy.GetPItem()) && !((CThingNativeState*)PEnemy.GetPItem())->IsDead()) {
        CThing* target = GetTargetFromSlot29(targetSource);
        if (((CThing*)PEnemy.GetPItem()) == target) {
            CAIBrain* willingBrain = GetBrain();
            CThingAICreature& attacker = *willingBrain->Creature;
            if (IsWillingToAttackTarget(&attacker, ((CThing*)PEnemy.GetPItem()))) {
                CThing* perceived = perception->GetPerceivedThing();
                if (((CThing*)PEnemy.GetPItem()) == perceived && perception->IsAwareOfThing()) {
                    fable_i32 frames = perception->FramesSinceDirectlyPerceivedThing;
                    if (frames != -1 && (float)frames < (float)g_FrameForTargetPerception * g_TargetPerceptionFrameScale)
                        return true;
                } else {
                    CAIBrain* visibleBrain = GetBrain();
                    CThingAICreature& observer = *visibleBrain->Creature;
                    if (NCreatureAI::IsAwareOfThing(&observer, ((CThing*)PEnemy.GetPItem()), true))
                        return true;
                }
            }
        }
    }
    return false;
}

#if defined(_MSC_VER)
typedef char fable_abi_assert_seek_group[(sizeof(CAIStateGroup_SeekEnemy) == 0x1c) ? 1 : -1];
typedef char fable_abi_assert_enemy_offset[(offsetof(CAIStateGroup_SeekEnemy, PEnemy) == offsetof(CAIStateGroup_SeekEnemyAccess, PEnemy)) ? 1 : -1];
typedef char fable_abi_assert_brain_offset[(offsetof(CAIStateGroupBase, PBrain) == 0x04) ? 1 : -1];
typedef char fable_abi_assert_creature_offset[(offsetof(CAIBrain, Creature) == 0x18) ? 1 : -1];
typedef char fable_abi_assert_perception_frames[(offsetof(CTCPerceiveThing, FramesSinceDirectlyPerceivedThing) == 0x1c) ? 1 : -1];
typedef char fable_abi_assert_thing_size[(sizeof(CThing) == 0x98) ? 1 : -1];
#endif