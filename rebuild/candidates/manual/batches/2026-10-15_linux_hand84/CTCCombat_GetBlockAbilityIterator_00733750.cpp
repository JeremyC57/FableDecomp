// CTCCombat::GetBlockAbilityIterator(CBlockAbilityIterator* out, long weaponClass) -- looks the block ability up with the plain finder; on success copies the found iterator to *out and returns true, else false
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00733750, length 49 bytes.
// Callees: the block-ability finder @ 0x00733620 (a __thiscall member on this TC taking the iterator out
// pointer and the type; manifest "CTCCombat_FindBlockAbility").
// Layout = engine/CTCCombat.h (no member is touched here).
// Shape: a one-word iterator local in the `push ecx` frame slot, zero-initialised (the store is scheduled
// after the pushes); `if (Find(&it, weaponClass)) { *out = it; return true; } return false;` with the found
// iterator re-read from the frame.
#include "engine/CTCCombat.h"

struct CCombatAbilityEntry;
typedef CCombatAbilityEntry* CBlockAbilityIterator;     // _Tree_const_iterator: the node pointer

struct CTCCombat_Methods : CTCCombat {
    bool FindBlockAbility(CBlockAbilityIterator* it, long weaponClass);         // 0x00733620
    bool GetBlockAbilityIterator(CBlockAbilityIterator* out, long weaponClass);
};

bool CTCCombat_Methods::GetBlockAbilityIterator(CBlockAbilityIterator* out, long weaponClass)
{
    CBlockAbilityIterator it = 0;
    if (FindBlockAbility(&it, weaponClass)) {
        *out = it;
        return true;
    }
    return false;
}
