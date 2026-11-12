# hdr/ overlay of chunk 2036 (agent2036)

Both files are copies of the repo headers with ONE added nested opt-in each; the default text and every existing opt-in block
are unchanged (landed includers keep their bytes).

## engine/CTCInventory.h -- `FABLE_RETAIL_CTCINVENTORY_ITEMDESCRIPTION` (nested in FABLE_RETAIL_CTCINVENTORY)
- Adds `virtual void GetItemDescription(CWideString& text, long itemDefIndex, bool hidePrice) const;   // 0x005841F9`.
- Evidence: (1) Ego_r.pdb + FableWin.pdb `?GetItemDescription@CTCInventory@@UBEXAAVCWideString@@J_N@Z`; (2) CTCInventory vtable
  0x01250DE4 slot 78 holds 0x005841F9 (its only reference), the slot CTCInventoryBase::GetItemDescription (0x005BC6B3) and the
  landed Clothing 0x005B5CF8 / Weapons 0x005C7913 overrides occupy; the landed ConstructItemDescription 0x005819DD calls +0x138
  with false. Needs FABLE_RETAIL_CTCINVENTORYBASE_VTABLE (the base declares slot 78 there).
- Row: 0x005841F9 (RELOCATION_MATCH).

## engine/CTattooDef.h -- `FABLE_RETAIL_CTATTOODEF_TYPEBALANCED` (nested in FABLE_RETAIL_CTATTOODEF)
- Adds the three const members `float GetTypeBalancedAttractiveness / Scariness / Goodstrength(const std::vector<CAppearanceModifierScalingDef>&) const`
  (0x0057FC46 / 0x0057FC71 / 0x0057FC9C, out of line in retail) plus, under the same opt-in, `#include "stlport/fable_containers.h"`
  and a `class CAppearanceModifierScalingDef;` forward declaration. Declarations only.
- Evidence: (1) Ego_r.pdb `?GetTypeBalanced*@CTattooDef@@QBEMABV?$vector@VCAppearanceModifierScalingDef@@V?$allocator@...@std@@@std@@@Z`;
  (2) retail calls them on the CDefPointer<const CTattooDef> that CTCInventoryItem::GetTattooCardDef 0x005D9AA0 returns
  (0x00584579 / 84 / 8F), right after the CAppearanceModifierDef twins 0x0057FB86 / BBB / BF0 of the same signature.
- Row: 0x005841F9.

Header debt noticed (not changed): engine/CGameDefinitionManager.h models the hero-stats def at +0xDC as a raw
`const CHeroStatsDef* PHeroStatsDef` (descriptive name). FableWin.pdb's `?GetPHeroStatsDef@CGameDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCHeroStatsDef@@@@XZ`
returns a CDefPointer by value, which suggests the real member is a `CDefPointer<const CHeroStatsDef>` (same 4 bytes; the
PeekHeroStatsDef body `*PHeroStatsDef` reads the same word). A probe with that member type changed nothing in 0x005841F9's c2
priorities, so the row does not depend on it.
