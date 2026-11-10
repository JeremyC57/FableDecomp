// CCombatAbilityBlockDefBase::Transfer(CPersistContext& context) -- persist the block def's blocking data: BlockAngle, the two block flags, the base combat-ability data, then the valid block weapon set through a temporary vector (saving: built from the set; loading: read back and the set rebuilt from it)
// Grade: DIFFER(203v226 with /d2inlS20) -- inline split only: retail expands IsSaving, the vector range ctor, set::operator= and the range insert_unique loop but calls _Rb_tree(const less&, const allocator&) 0x00451ADD and ~_Rb_tree 0x00451B07; compile-only sweep S9..S50 never gives that split (S20: tree ctor called but the range insert called and the dtor expanded; S35/S40: range insert expanded and dtor called but the tree ctor expanded). /Os + y off is right (`pop ecx` cdecl cleanup, `lea eax` temps); the zero register (xor ebx) appears from S20
// Flags: /d2inlS20
// Leaf: ?Transfer@CCombatAbilityBlockDefBase@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00465C54, 226 bytes, virtual thiscall `ret 4`; size-optimised TU with an ebp frame.
// Identity (proven): Ego_r.pdb ?Transfer@CCombatAbilityBlockDefBase@@UAEXAAVCPersistContext@@@Z is vtable slot 18 of CCombatAbilityBlockDefBase
//   (0x01232E04 -> 0x00465C54, rtti-vtables.tsv; the four derived block defs' vtables 0x01232E4C.. hold it too). Members per Ego_r.pdb:
//   CombatAbility (CCombatAbilityBlockingData) +0x28: BlockAngle +0x40, BlockCanBeBroken +0x44, ForceRecoilWhenBlocking +0x45,
//   ValidBlockWeaponTypes (set<EWeaponClass>) +0x48; the CCombatAbilityData base is transferred whole.
// Callees: CPersistContext::Transfer<float>(float&) 0x00431061, Transfer<bool>(bool&) 0x0043314A (x2), Transfer<CCombatAbilityData> 0x00452B4D
//   (CheckCRC, then the data's virtual TransferBinaryIn/Out: the landed row there is a propagated CScrollingComponent label),
//   CPersistContext::TransferVector<EWeaponClass>(vector&) 0x00463F3C, vector<EWeaponClass>::_M_range_initialize 0x00465E44,
//   _Rb_tree<EWeaponClass,...>::_Rb_tree(const less&, const allocator&) 0x00451ADD, insert_unique(const EWeaponClass&) 0x00465D36,
//   operator= 0x00461BF0, ~_Rb_tree 0x00451B07, free 0x00BFEA14 (the vector's inline destructor).
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
        std::set<EWeaponClass> weaponSet;
        weaponSet.insert(weaponTypes.begin(), weaponTypes.end());
        CombatAbility.ValidBlockWeaponTypes = weaponSet;
    }
}
