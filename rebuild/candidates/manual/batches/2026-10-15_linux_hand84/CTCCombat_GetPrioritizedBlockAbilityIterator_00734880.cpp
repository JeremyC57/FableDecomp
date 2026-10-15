// CTCCombat::GetPrioritizedBlockAbilityIterator(CBlockAbilityIterator* out, long weaponClass) -- looks the block ability up with the prioritised finder; on success copies the found iterator to *out and returns true, else false
// Leaf: GetPrioritizedBlockAbilityIterator
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00734880, length 49 bytes (+ int3 padding).
// Callees: the prioritised block-ability finder @ 0x00734340 (a __thiscall member on this TC taking the
// iterator out pointer and the weapon class; manifest "CTCCombat_FindBlockAbilityPrioritized").
// Naming: the manifest gives this body and 0x00733750 the same label and prototype
// (GetBlockAbilityIterator(_Tree_const_iterator*, EWeaponClass)); both are called from SetStruck
// (0x00734C60). This is the twin over the prioritised finder, named after it.
// Layout = engine/CTCCombat.h (no member is touched here).
// Shape: as 0x00733750 -- a one-word iterator local in the `push ecx` frame slot, zero-initialised (the
// store scheduled after the pushes); `if (Find(&it, type)) { *out = it; return true; } return false;`.
#include "engine/CTCCombat.h"

struct CCombatAbilityEntry;
typedef CCombatAbilityEntry* CBlockAbilityIterator;     // _Tree_const_iterator: the node pointer

struct CTCCombat_Methods : CTCCombat {
    bool FindBlockAbilityPrioritized(CBlockAbilityIterator* it, long weaponClass);   // 0x00734340
    bool GetPrioritizedBlockAbilityIterator(CBlockAbilityIterator* out, long weaponClass);
};

bool CTCCombat_Methods::GetPrioritizedBlockAbilityIterator(CBlockAbilityIterator* out, long weaponClass)
{
    CBlockAbilityIterator it = 0;
    if (FindBlockAbilityPrioritized(&it, weaponClass)) {
        *out = it;
        return true;
    }
    return false;
}
