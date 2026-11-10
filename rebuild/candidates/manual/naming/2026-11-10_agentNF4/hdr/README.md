# agentNF4 header overlays

Full copies of `rebuild/include/engine/<X>.h`. Each one adds an `#ifdef` opt-in only. When the macro is not
defined, the preprocessed default path is token-identical to the repo header. The only exception is the
`__LINE__`-derived FABLE_STATIC_ASSERT typedef names. The rows grade with `FABLE_HDR_OVERLAY=$D/hdr`.

| Header | Opt-in macro | Adds | Evidence | Rows |
|---|---|---|---|---|
| CSpecialAbilitiesFireballSpellDef.h | FABLE_RETAIL_CSPECIALABILITIESFIREBALLSPELLDEF_MEMBERS | Alloc, out-of-line ctor 0x00457B35, Transfer/Copy/GetSizeofClass overrides. Dtor stays implicit. | vtable 0x012345A4 slots; Ego_r + FableWin member names | 00457b35 |
| CTrollBattleDef.h | FABLE_RETAIL_CTROLLBATTLEDEF_MEMBERS | Alloc 0x004E4833 + overrides | vtable 0x01242F0C; Ego_r | 004e4833 |
| CEnvironmentDef.h | FABLE_RETAIL_CENVIRONMENTDEF_PARENT | Class over CParentDefClassBase; Ego_r members at Ego_r-4; sizeof 0x134 | RTTI base list names CParentDefClassBase; ctor calls 0x00430370 | 004558bf |
| CEnvironmentThemeDaySetDef.h | FABLE_RETAIL_CENVIRONMENTTHEMEDAYSETDEF | Retail layout (CArray TimeTheme +0x3C ... sizeof 0x6C). The generated layout moves under #else. | Ego_r layout; ctor/??_G bytes | 00455c43, 00455c64 |
| CCreatureAction_PickupGameObject.h | FABLE_RETAIL_CCREATUREACTION_PICKUPGAMEOBJECT | Retail members over CCreatureActionBase | Ego_r layout; ctor + ??_G sequence | 007eb2b0 |
| CCreatureAction_BriarRoseConcentrateInto.h | FABLE_RETAIL_CCREATUREACTION_BRIARROSECONCENTRATEINTO | Imposter bool +0x114, PEnemy CIntelligentPointer +0x115 | Ego_r layout; Clone + dtor bytes | 008e8c20 |
| CCreatureAction_ActivateQuest.h | FABLE_RETAIL_CCREATUREACTION_ACTIVATEQUEST | ScriptName, LoadResources, PObjectToUse +0xAD; implicit copy ctor | Ego_r layout; ??_G bytes | 0084b500 |

Header debt that was noticed but not fixed here:
- The default path of `CEnvironmentDef.h` derives from CDefClassBase. RTTI says the base is CParentDefClassBase.
- `CCardPositionsDef.h` declares `virtual ~CCardPositionsDef()`. Def dtors are implicit in retail, so a ??_G built from this header will not inline the dtor.
