// std::pair<long, CDefPointer<const CHeroAbilityDef> >::~pair() -- destroy one (level, hero ability def) pair of the abilities screen: release the counted def pointer
// Leaf: ??1?$pair@JV?$CDefPointer@$$CBVCHeroAbilityDef@@@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F8736, 26 bytes, thiscall, plain `ret`; size-optimised TU (unaligned, `and [esi+4],0`).
// Identity (agentFAM06): FableWin.pdb ??1?$pair@JV?$CDefPointer@$$CBVCHeroAbilityDef@@@@@std@@QAE@XZ (implicit pair destructor; both
//   PDB name sets hold vector<pair<long, CDefPointer<const CHeroAbilityDef> > >), the element type of Ego_r.pdb
//   CTCInventoryAbilities::Abilities. Callers, all in the CTCInventoryAbilities TU: the constructor 0x005F3BCF (the temporary pair
//   of `Abilities(20, pair())`, 0x005F3C5D vector(n, value) then call 0x005F3C65), the 8-byte-stride range destroy 0x005F9A7D,
//   the scalar deleting helper 0x005F8D31 and the jmp at 0x005F93BC. The release reads `second` at +4. The landed binders spell it
//   ??1SAbilityEntry@@QAE@XZ / ?Destroy_pair@@YIXPAUElem@@@Z / ?AutoTinyScalarDestructor_005f8736@@YIXPAX@Z (NAME_FIXES.tsv).
//   Catalog CIVCountedPointer_Release is a placeholder.
// Body: the implicit destructor of the shared STLport 4.6 std::pair (rebuild/include/stlport/fable_containers.h) over the shared
//   CDefPointer<T> (engine/CDefPointer.h: ~CDefPointer() { Release(); }, the release in place). The helper below only makes
//   VC7.1 emit the implicit ??1 COMDAT; it is not a retail function.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/CDefPointer.h"
class CHeroAbilityDef;

typedef std::pair<long, CDefPointer<const CHeroAbilityDef> > THeroAbilityPair;

void EmitHeroAbilityPairDestructor(THeroAbilityPair* p)   // emits the implicit ??1; not a retail function
{
    p->THeroAbilityPair::~THeroAbilityPair();
}
