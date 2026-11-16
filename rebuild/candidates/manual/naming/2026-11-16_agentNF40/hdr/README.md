# NF40 overlay headers (FABLE_HDR_OVERLAY)

Each file is a copy of the current repo header `rebuild/include/engine/<name>` with one additive opt-in block; with the opt-in
undefined the header is byte-identical in effect to the repo copy.

| Overlay | Copies | Adds (opt-in) | Used by rows |
|---|---|---|---|
| engine/CFrontEndDef.h | rebuild/include/engine/CFrontEndDef.h | FABLE_RETAIL_CFRONTENDDEF_TRANSFER: `public: virtual void Transfer(class CPersistContext&);` | 0x004331C0 |
| engine/CSpecialAbilitiesTurncoatSpellDef.h | rebuild/include/engine/CSpecialAbilitiesTurncoatSpellDef.h | FABLE_RETAIL_CSPECIALABILITIESTURNCOATSPELLDEF_TRANSFER (virtual Transfer) | 0x00465662 |
| engine/CSpecialAbilitiesDrainLifeDef.h | rebuild/include/engine/CSpecialAbilitiesDrainLifeDef.h | FABLE_RETAIL_CSPECIALABILITIESDRAINLIFEDEF_TRANSFER (virtual Transfer) | 0x004724C2 |
| engine/CSpecialAbilitiesEnflameDef.h | rebuild/include/engine/CSpecialAbilitiesEnflameDef.h | FABLE_RETAIL_CSPECIALABILITIESENFLAMEDEF_TRANSFER (virtual Transfer) | 0x00465258 |
| engine/CCreatureGenerationFamilyDef.h | rebuild/include/engine/CCreatureGenerationFamilyDef.h | FABLE_RETAIL_CCREATUREGENERATIONFAMILYDEF_TRANSFER (virtual Transfer) | 0x004696C1 |
| engine/CCombatTypeDef.h | rebuild/include/engine/CCombatTypeDef.h | FABLE_RETAIL_CCOMBATTYPEDEF_TRANSFER (virtual Transfer) | 0x0046F7C8 |
| engine/CHeroAbilityDef.h | rebuild/include/engine/CHeroAbilityDef.h | FABLE_RETAIL_CHEROABILITYDEF_TRANSFER: virtual Transfer, `#include "engine/CDefIndex.h"`, and `CDefIndex DummyObject;` in place of `unsigned int DummyObject[1]` (same size/offset, Ego_r member type) | 0x00469AB3 |
| engine/CRegionMinimap.h | rebuild/include/engine/CRegionMinimap.h | FABLE_RETAIL_CREGIONMINIMAP_WORLDCOORDS_OUTOFLINE (only without _POPULATE): declares `void GetWorldCoordinatesFromTGAPosition(C3DVector&, const C2DCoordI&) const;` (Ego_r.pdb member, out-of-line copy 0x0066C810) | 0x0066C810 |
| engine/CStreamingFontBank.h | rebuild/include/engine/CStreamingFontBank.h | FABLE_RETAIL_CSTREAMINGFONTBANK_SETUSEFIXEDFUNCTION: declares `static void FABLE_FASTCALL SetUseFixedFunction(bool);` (Ego_r/FableWin ?SetUseFixedFunction@CStreamingFontBank@@SIX_N@Z, 0x009A0EF0); needs FABLE_RETAIL_CENGINEPRIMITIVEMANAGERTEXT_RENDER for the static member | 0x009A0EF0 |

All nine were in effect for recheck_final.log (83 rows, all MATCH / RELOCATION_MATCH + PASS).
