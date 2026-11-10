# agentSND header overlay (w22, 2026-11-10)

Every grade in this chunk ran with `FABLE_HDR_OVERLAY=$S/w22/agentSND/hdr` exported (gr.sh / cc.sh export it; export it before
gall.sh). The overlay directories are searched before `rebuild/include`. Files copied from the repo were taken on 2026-11-10 and
differ from it only as listed; every change other than the CASoundManager / CASoundEmitter slot names sits under its own `#ifdef`.

| file | kind | change |
|---|---|---|
| `engine/CASoundManager.h` | rewritten | All 54 slots named per `SLOTS.tsv` (real names from the Fable Anniversary 360 PDB + TLC evidence; slot 9 `Get2DEmitter` and 51 `ExtractMetaData` descriptive), with their parameter types; includes `engine/CASoundEmitter.h`; nested `CASoundManager::CAtmosMap` (RTTI). Kept: `FABLE_RETAIL_CASOUNDMANAGER_BANKREGISTRATION` (InitSound 0x00417A58 still compiles), size 0x4C, `PSoundManager`. `FABLE_RETAIL_CASOUNDMANAGER_SLOT9` is now a no-op. New opt-in `FABLE_RETAIL_CASOUNDMANAGER_ENABLE_NAMES`: EnableMusic / EnableSound / EnableAtmos (0x009915E0 / 0x00991600 / 0x00991620); the default keeps the old SetEnabled* spellings until the coordinator respells their callers (NAME_FIXES.tsv). |
| `engine/CASoundEmitter.h` | new | The abstract emitter (RTTI `CASoundEmitter : CBaseClass`, vtable 0x0129E684, 25 slots), class of the object at CSoundManagerLH+0x44 (a `CSoundEmitterLH`). Slot names: Anniversary `CSoundEmitterUN` publics aligned by the TLC `CThingSoundEmitter` wrappers; 18 `Is3D` and 24 `GetActiveSampleInfo` descriptive. |
| `engine/CSoundInit.h` | new | Ego_r.pdb CSoundInit (0x20 bytes: Volume .. OverrideID; GainFaderBits +0x14), ctor 0x0041C930. |
| `engine/CASoundBank.h` | opt-in | `FABLE_RETAIL_CASOUNDBANK_SOUNDINDEX`: `long GetSoundIndexFromSymbol(const CCharString&) const` 0x00A01920 (FableWin.pdb + Anniversary). |
| `engine/CTCInventoryAbilities.h` | opt-in | `FABLE_RETAIL_CTCINVENTORYABILITIES_PRIORITISED`: GetPrioritisedAbilityDefs 0x005F4B30 / GetPrioritisedExpressionDefs 0x005EED8C (QBE, vector by reference) / SetExpressionToButton(long, long) 0x005F66D9. |
| `engine/CTCInventoryTrade.h` | opt-ins | `FABLE_RETAIL_CTCINVENTORYTRADE_OPENINVENTORY` (the slot-30 override 0x005A94BD), `FABLE_RETAIL_CTCINVENTORYTRADE_ACTUALINVENTORYPC` (private ConstructActualInventoryPC 0x005ACEDA). |
| `engine/NInventory_CTCInventoryBase.h` | opt-in | `FABLE_RETAIL_CTCINVENTORYBASE_PDEF`: PDef typed `CDefPointer<const CInventoryDef>` (+0x68) and the protected inline `GetPDef() const` (Ego_r.pdb / FableWin.pdb IBE, by value). |
| `engine/CInventoryDef.h` | new (partial) | Ego_r.pdb CInventoryDef members up to MeshAreaBRPos at retail offsets (+0x3C .. +0xAC); no sizeof. |
| `engine/C2DBoxF.h` | nested opt-in | `FABLE_RETAIL_C2DBOXF_CORNERS_FORCEINLINE`: the corners constructor `FABLE_FORCEINLINE` (retail expands it in the trade size TU at 0x005A95E1). |
