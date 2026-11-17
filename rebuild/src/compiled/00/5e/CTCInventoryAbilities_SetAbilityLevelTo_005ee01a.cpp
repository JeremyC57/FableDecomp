// CTCInventoryAbilities::SetAbilityLevelTo(EHeroAbility ability, long level) -- store the level into the ability's entry of Abilities
//   (+0x160, the vector<pair<long, CDefPointer<const CHeroAbilityDef> > >: 8-byte elements, `first` is the level)
// Leaf: ?SetAbilityLevelTo@CTCInventoryAbilities@@QAEXW4EHeroAbility@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EE01A, 20 bytes: `mov eax,[ecx+160h] ; mov ecx,[esp+8] ; mov edx,[esp+4] ;
//   mov [eax+edx*8],ecx ; ret 8`.
// NF64 (round 7, q10686): Ego_r.pdb 0x0057CB75 / FableWin.pdb 0x01EBB4D3 ?SetAbilityLevelTo@CTCInventoryAbilities@@QAEXW4EHeroAbility@@J@Z,
//   declared by engine/CTCInventoryAbilities.h FABLE_RETAIL_CTCINVENTORYABILITIES_SETLEVEL ("out of line at 0x005EE01A"); the landed callers
//   0x00576C4F and NF61's 0x005785B3 bind it. Re-authored on the header's retail layout (Abilities at +0x160 after the STLport vectors).
//   Was catalogued CEngineLandscapeRenderer::OnRegistedZSortingPatchRelocation (a propagated label, a local model storing at +0x160).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_SETLEVEL
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryAbilities.h"

void CTCInventoryAbilities::SetAbilityLevelTo(EHeroAbility ability, long level)
{
    Abilities[ability].first = level;
}
