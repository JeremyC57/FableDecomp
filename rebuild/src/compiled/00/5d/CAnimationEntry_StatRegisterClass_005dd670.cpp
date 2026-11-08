// CAnimationEntry::StatRegisterClass() -- registers the 13 animation-component prototypes: each one by its token (map), by its type and as its type's default (two sorted vector maps), then sorts both maps once
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DD670, 5317 bytes (+ int3 padding); static __fastcall, plain `ret`; Ego_r.pdb / FableWin.pdb
// ?StatRegisterClass@CAnimationEntry@@SIXXZ; one caller (0x0044E907, the game's class-registration pass, next to
// CPhysicalPrimitiveInit::StatRegisterClass).
// Leaf: ?StatRegisterClass@CAnimationEntry@@SIXXZ
// Inline-budget: __forceinline CVectorMap::Insert(const K&, const V&) in engine/CVectorMap.h (opt-in _INSERT_KEYVALUE, `#if defined(_MSC_VER)`, plain inline elsewhere) -- all 26 Insert sites expanded in retail, Ego_r.pdb has no out-of-line Insert; plain inline: called out of line (3919 bytes); no /d2inlT value expanded Insert while keeping push_back split 17/26
// Shape: one statement block per component (FableWin.pdb has no helper between StatRegisterClass and the components' StatGetTokenName /
// StatGetType, so the blocks are written out, here through one local macro): `new T` (IVRefCount 1, vtable, member ctor) adopted by a
// CIVCountedPointer local; SetDefaultValue (slot 8) through it -- after that virtual call VC7.1 treats the pointer as non-null, so every
// later copy is an unconditional `inc [esi+4]`; insert(make_pair(token, component)) into the token map (the pair converted to the
// map's value_type, insert_unique 0x005E2580 out of line, the four temporaries released in reverse); then the two CVectorMap::Insert
// calls (forced inline, header): ByType keyed by the virtual GetType (slot 2), Defaults by the static StatGetType constant. Their
// push_back is expanded at the first 17 sites and called (0x005E3110) from the second Insert of block 9 on: VC7.1's caller budget,
// reproduced without any flag. Last, Update() on both maps: std::sort 0x005E3AD0 (__fastcall) when Dirty, then Dirty cleared.
// Out-of-line callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / dtor 0x0099EAE0 /
// default ctor 0x0099E4B0 (StrikeResponseAnim's member), _Rb_tree::insert_unique 0x005E2580, vector::_M_insert_overflow 0x005E29D0,
// vector::push_back 0x005E3110, std::sort 0x005E3AD0.
// Models: engine/CAnimationEntry.h, engine/CAnimComponentBase.h, the 13 engine/CAnimComponent*.h (FABLE_RETAIL_ANIMCOMPONENT),
// engine/CIVCountedPointer.h (_FROM_RAW, _RELEASE_INNER), engine/CVectorMap.h (_INSERT, _INSERT_KEYVALUE), stlport/fable_map.h.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_FROM_RAW
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_INNER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_ANIMCOMPONENT
#include "engine/CAnimationEntry.h"
#include "engine/CAnimComponentFlags.h"
#include "engine/CAnimComponentTransitionInTime.h"
#include "engine/CAnimComponentDelayTime.h"
#include "engine/CAnimComponentCombatHandedness.h"
#include "engine/CAnimComponentCombatComboChain.h"
#include "engine/CAnimComponentCombatRecoil.h"
#include "engine/CAnimComponentCombatMisc.h"
#include "engine/CAnimComponentCombatFlourishTargetOffset.h"
#include "engine/CAnimComponentFollowingOnAnim.h"
#include "engine/CAnimComponentStrikeSpeedMultiplier.h"
#include "engine/CAnimComponentSetCreatureMode.h"
#include "engine/CAnimComponentFollowingOnAnimFilter.h"
#include "engine/CAnimComponentStrikeResponseAnim.h"

// One registration: the prototype is adopted by `component` (IVRefCount 1 from the constructor) and shared by the three registries.
#define REGISTER_ANIM_COMPONENT(T) \
    { \
        CIVCountedPointer<CAnimComponentBase> component(new T); \
        component->SetDefaultValue(); \
        RegisteredComponentsByToken.insert(std::make_pair(T::StatGetTokenName(), component)); \
        RegisteredComponentsByType.Insert(component->GetType(), component); \
        RegisteredComponentDefaults.Insert(T::StatGetType(), component); \
    }

void __fastcall CAnimationEntry::StatRegisterClass()
{
    REGISTER_ANIM_COMPONENT(CAnimComponentFlags)
    REGISTER_ANIM_COMPONENT(CAnimComponentTransitionInTime)
    REGISTER_ANIM_COMPONENT(CAnimComponentDelayTime)
    REGISTER_ANIM_COMPONENT(CAnimComponentCombatHandedness)
    REGISTER_ANIM_COMPONENT(CAnimComponentCombatComboChain)
    REGISTER_ANIM_COMPONENT(CAnimComponentCombatRecoil)
    REGISTER_ANIM_COMPONENT(CAnimComponentCombatMisc)
    REGISTER_ANIM_COMPONENT(CAnimComponentCombatFlourishTargetOffset)
    REGISTER_ANIM_COMPONENT(CAnimComponentFollowingOnAnim)
    REGISTER_ANIM_COMPONENT(CAnimComponentStrikeSpeedMultiplier)
    REGISTER_ANIM_COMPONENT(CAnimComponentSetCreatureMode)
    REGISTER_ANIM_COMPONENT(CAnimComponentFollowingOnAnimFilter)
    REGISTER_ANIM_COMPONENT(CAnimComponentStrikeResponseAnim)
    RegisteredComponentsByType.Update();
    RegisteredComponentDefaults.Update();
}