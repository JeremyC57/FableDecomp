// CCombatAbilityBlockDefBase::Transfer(CPersistContext& context) -- persist the block def's blocking data: BlockAngle, the two block
// flags, the base combat-ability data, then the valid block weapon set through a temporary vector (saving: built from the set;
// loading: read back and the set rebuilt from it and assigned)
// Leaf: ?Transfer@CCombatAbilityBlockDefBase@@UAEXAAVCPersistContext@@@Z
// Flags: /d2inlS39
// Inline-budget: /d2inlS39 -- the size-mode inliner threshold at which VC7.1 expands what retail expands here (IsSaving, the
//   vector range constructor, the set range constructor with its insert_unique loop, set::operator=); S38 and below call some of
//   them (189 bytes). The tree constructor / destructor stay out of line through FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00465C54, 226 bytes, virtual thiscall `ret 4`; size-optimised def TU with an ebp frame
//   (`#pragma optimize("s", on)` + `("y", off)`: `leave`, `pop ecx`, every address through eax).
// Identity (proven): Ego_r.pdb ?Transfer@CCombatAbilityBlockDefBase@@UAEXAAVCPersistContext@@@Z is vtable slot 18 of CCombatAbilityBlockDefBase
//   (0x01232E04 -> 0x00465C54, rtti-vtables.tsv; the four derived block defs' vtables 0x01232E4C.. hold it too). Members per Ego_r.pdb:
//   CombatAbility (CCombatAbilityBlockingData) +0x28: BlockAngle +0x40, BlockCanBeBroken +0x44, ForceRecoilWhenBlocking +0x45,
//   ValidBlockWeaponTypes (set<EWeaponClass>) +0x48; the CCombatAbilityData base is transferred whole.
// Callees: CPersistContext::Transfer<float>(float&) 0x00431061, Transfer<bool>(bool&) 0x0043314A (x2), Transfer<CCombatAbilityData> 0x00452B4D
//   (CheckCRC, then the data's virtual TransferBinaryIn/Out), CPersistContext::TransferVector<EWeaponClass>(vector&) 0x00463F3C,
//   vector<EWeaponClass>::_M_range_initialize 0x00465E44, _Rb_tree<EWeaponClass,...>::_Rb_tree(const less&, const allocator&) 0x00451ADD,
//   insert_unique(const EWeaponClass&) 0x00465D36, _Rb_tree::operator= 0x00461BF0, ~_Rb_tree 0x00451B07, free 0x00BFEA14 (the
//   vector's inline destructor; both arms share it).
// Shape: two vectors, one per arm (same frame slot, one shared destructor tail); the loaded set is a temporary built with the range
//   constructor and assigned. The tree constructor and destructor are STLport's visible bodies kept out of line, so VC7.1 knows the
//   empty less/allocator temporaries escape nowhere and homes them -- and the cached vector end -- in the dead `context` slot
//   ([ebp+0Bh] / [ebp+8]); with declaration-only callees they get their own frame words (226v226, frame 0x28).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CCOMBATABILITYBLOCKINGDATA
#define FABLE_RETAIL_CCOMBATABILITYBLOCKDEFBASE_TRANSFER
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_SET_RANGE
#define FABLE_RETAIL_STLP_VECTOR_RANGE_CTOR
#define FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CCombatAbilityBlockDefBase.h"
#include "engine/CPersistContext.h"

void CCombatAbilityBlockDefBase::Transfer(CPersistContext& context)
{
    context.Transfer(CombatAbility.BlockAngle);
    context.Transfer(CombatAbility.BlockCanBeBroken);
    context.Transfer(CombatAbility.ForceRecoilWhenBlocking);
    context.Transfer(static_cast<CCombatAbilityData&>(CombatAbility));
    if (context.IsSaving()) {
        std::vector<EWeaponClass> weaponTypes(CombatAbility.ValidBlockWeaponTypes.begin(), CombatAbility.ValidBlockWeaponTypes.end());
        context.TransferVector(weaponTypes);
    } else {
        std::vector<EWeaponClass> weaponTypes;
        context.TransferVector(weaponTypes);
        CombatAbility.ValidBlockWeaponTypes = std::set<EWeaponClass>(weaponTypes.begin(), weaponTypes.end());
    }
}