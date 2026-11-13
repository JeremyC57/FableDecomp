# Chunk 2040 header overlay (FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2040/hdr)

Both files are verbatim copies of the repo headers (rebuild/include/engine/) plus ONE new opt-in block each; the default text is
unchanged, so no landed includer changes bytes. The row includes `engine/CWorld.h` first so the overlay copy wins over a relative
include from another repo header.

| file | opt-in | what it adds | evidence (two sources) |
|---|---|---|---|
| engine/CTCInventoryAbilities.h | `FABLE_RETAIL_CTCINVENTORYABILITIES_ONSERIALISE` (nested in the retail branch) | `virtual void OnSerialise(CPersistContext& context, bool inGameSave);` public override (no layout / vtable change) | Ego_r.pdb + FableWin.pdb `?OnSerialise@CTCInventoryAbilities@@UAEXAAVCPersistContext@@_N@Z` (UAE = public virtual); retail vtable 0x01256AD4 slot 1 = 0x005F6E40; base `?OnSerialise@CTCInventoryBase@NInventory@@MAEXAAVCPersistContext@@_N@Z` 0x005BFD5E is already declared in engine/NInventory_CTCInventoryBase.h |
| engine/CWorld.h | `FABLE_RETAIL_CWORLD_ABOUTTOLOADHEROSTATE` | `bool IsSerialisingAboutToLoadHeroState() const { return SerialisingAboutToLoadHeroState; }` next to the existing IsSerialisingHeroState accessor | FableWin.pdb `?IsSerialisingAboutToLoadHeroState@CWorld@@QBE_NXZ` (0x01D3BD00; not in Ego_r.pdb = inline in release); member SerialisingAboutToLoadHeroState +0x101 (Ego_r layout already in the header, static-asserted); retail reads `[world+0x101]` after PeekWorld 0x00686A80 at 0x005F70AB / 0x005F70F4 / 0x005F7282 / 0x005F72EE / 0x005F73CB |

Existing repo opt-ins the row uses (no change): FABLE_RETAIL_CPERSISTCONTEXT_ISSAVING_OUTOFLINE, FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND,
FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ (both with the row's `// Inline-budget:` line), FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
(for the same-TU GetMiscThingsDef body), FABLE_RETAIL_CTCINVENTORYABILITIES_{ASSIGN,BUTTONS,INITIALISE,EXPRESSIONS},
FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER, FABLE_RETAIL_CGAMEPLAYERINTERFACE_SPECIALMOVEBUTTONS.
