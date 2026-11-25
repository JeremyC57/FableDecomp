// CAIStateGroup_SeekEnemy::IsStillRunnable() -- validate the remembered enemy against target selection and perception; retail 0x0093b8b0, 319 bytes. Callees: CVectorMap::LowerBound 0x004365b0, CBaseIntelligentPointer::GetPItem 0x00a01b50, CTCTargetingAI slot 29 thunk 0x004ddae5, IsWillingToAttackTarget 0x008648a0, CTCPerceiveThing::GetPerceivedThing 0x007446a0, CTCPerceiveThing::IsAwareOfThing 0x00643b3b, CanCreatureSeeCreature 0x008645e0. Frame globals 0x01375550 and 0x012925b8.
// Naming (NF173, 2026-11-20): q4718/q5091: 0x007446A0 is Ego_r.pdb / FableWin.pdb ?GetPerceivedThing@CTCPerceiveThing@@QBEPBVCThing@@XZ; declared on the row's existing local CTCPerceiveThing (in place), no longer on the CTCPerceiveThingAccess stand-in.
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_SeekEnemy.h"
#include "engine/CThingAICreature.h"
#define CTCPerceiveThing CTCPerceiveThingLayout
#include "engine/CTCPerceiveThing.h"
#undef CTCPerceiveThing

class CThing;
class CTCPerceiveThing : public CTCPerceiveThingLayout
{
public:
    bool IsAwareOfThing() const;   // 0x00643B3B
    const CThing* GetPerceivedThing() const;   // 0x007446A0 (Ego_r.pdb / FableWin.pdb ?GetPerceivedThing@CTCPerceiveThing@@QBEPBVCThing@@XZ)
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

// Naming (NF437, 2026-11-25): the LowerBound split -- the row-local map stand-in CInterfaceMap (?LowerBound@CInterfaceMap@@QAEPAUEntry@1@PBH@Z) replaced by the shared
//   CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<..> > typedef of engine/CVectorMap.h that the landed AI rows use (0x00929F10): CThing::Components at +0x44 (0x10 bytes),
//   so the call binds the established ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@...@@QBE spelling at 0x004365B0.
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

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
};

struct CThingNativeState : CThing {
    FABLE_FORCEINLINE bool IsDead() const {
        return (*(const fable_u8*)((const char*)this + 0x91) & 1u) != 0;
    }
};

extern bool FABLE_FASTCALL IsWillingToAttackTarget(CThingAICreature*, CThing*); // 0x008648a0
// 0x008645E0 Ego_r.pdb + FableWin.pdb ?IsAwareOfThing@NCreatureAI@@YI_NPBVCThingAICreature@@PBVCThing@@_N@Z (name-fix 2026-11-12)
namespace NCreatureAI { bool FABLE_FASTCALL IsAwareOfThing(const CThingAICreature* creature, const CThing* thing, bool flag); }
// NF166 (round 10, q11679 / q11691 / q11725): 0x01375550 is ?ConstantFPS@CMainGameComponent@@1JA (protected static long,
//   engine/CMainGameComponent.h), read through the inline public accessor GetConstantFPS() (Ego_r + FableWin
//   ?GetConstantFPS@CMainGameComponent@@SIJXZ) as retail code outside the class does; was the row-local stand-in g_FrameForTargetPerception.
#include "engine/CMainGameComponent.h"
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
    ETCInterfaceType type;
    if (creature->ComponentBits()[1] & 0x00100000u) {
        const CInterfaceMap* map = creature->ComponentMap();
        const CInterfaceMap::value_type* it;
        type = (ETCInterfaceType)0x34;
        it = map->LowerBound(type);
        const CInterfaceMap::value_type* last = map->end();
        if (it == last || it->first > 0x34)
            it = last;
        perception = (CTCPerceiveThingAccess*)it->second;
    }

    CTCTargetingAI* targetSource;
    if (creature->ComponentBits()[0] & 0x100u) {
        const CInterfaceMap* map = creature->ComponentMap();
        const CInterfaceMap::value_type* it;
        type = (ETCInterfaceType)8;
        it = map->LowerBound(type);
        const CInterfaceMap::value_type* last = map->end();
        if (it == last || it->first > 8)
            it = last;
        targetSource = (CTCTargetingAI*)it->second;
    }

    if (((CThing*)PEnemy.GetPItem()) && !((CThingNativeState*)PEnemy.GetPItem())->IsDead()) {
        CThing* target = GetTargetFromSlot29(targetSource);
        if (((CThing*)PEnemy.GetPItem()) == target) {
            CAIBrain* willingBrain = GetBrain();
            CThingAICreature& attacker = *willingBrain->Creature;
            if (IsWillingToAttackTarget(&attacker, ((CThing*)PEnemy.GetPItem()))) {
                const CThing* perceived = perception->GetPerceivedThing();
                if (((CThing*)PEnemy.GetPItem()) == perceived && perception->IsAwareOfThing()) {
                    fable_i32 frames = perception->FramesSinceDirectlyPerceivedThing;
                    if (frames != -1 && (float)frames < (float)CMainGameComponent::GetConstantFPS() * g_TargetPerceptionFrameScale)
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