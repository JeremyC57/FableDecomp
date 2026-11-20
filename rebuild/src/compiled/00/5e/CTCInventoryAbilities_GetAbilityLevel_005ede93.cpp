// CTCInventoryAbilities::GetAbilityLevel(EHeroAbility ability) const -- the hero's current level in one ability
// Leaf: ?GetAbilityLevel@CTCInventoryAbilities@@QBEJW4EHeroAbility@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EDE93, out of line (`mov eax,[ecx+160h] ; mov ecx,[esp+4] ; mov eax,[eax+ecx*8] ; ret 4`).
// Identity: Ego_r.pdb 0x0057CAFE + FableWin.pdb 0x01EBA6D4 ?GetAbilityLevel@CTCInventoryAbilities@@QBEJW4EHeroAbility@@@Z (const, EHeroAbility);
//   all 27 landed REFS rows (CTCInventoryExperience, CTCHeroExperience, the CTCWill*/CTC*Spell ctors, 0x008EDFF0 ...) bind this spelling.
// Layout: engine/CTCInventoryAbilities.h retail branch: Abilities +0x160 is vector<pair<long, CDefPointer<const CHeroAbilityDef> > >
//   (8-byte elements, level in .first), as the landed SetAbilityLevelTo 0x005EE01A writes it.
// NF113 (PRIORITY q6234): was a local CTCInventoryAbilities model with ?GetAbilityLevel@CTCInventoryAbilities@@QAEHH@Z (int index,
//   non-const); now the shared header's const EHeroAbility member.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryAbilities.h"

long CTCInventoryAbilities::GetAbilityLevel(EHeroAbility ability) const
{
    return Abilities[ability].first;
}
