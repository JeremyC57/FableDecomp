# agentNF10 header overlay

Full copies of the current repo headers (or new headers) under `hdr/engine/`. Every change to an existing header is an `#ifdef`
addition only (`diff` shows no deleted line in any of the 13 changed files); `hdrcheck2.py hdr` reports `default=same` for every
changed header and `same` for every pre-existing opt-in. Grade with `FABLE_HDR_OVERLAY=<this dir>`.

| Header | Opt-in (or NEW) | What it adds | Evidence |
|---|---|---|---|
| CCreatureDef.h | FABLE_RETAIL_CCREATUREDEF_RESPAWNWAIT (nested in SCREENINVENTORIES) | CreatureGroup +0xAC, BattleCrySound +0xB0, OpinionSourceDef +0xB4, RespawnWaitInDaysMin/Max +0xB8/+0xBC, RespawnWaitInFramesMin/Max +0xC0/+0xC4 (replaces the pad, same size) | Ego_r.pdb CCreatureDef (struct_layouts_egor.tsv, 0x10 higher there as the screen inventories); retail reads +0xB8/+0xBC at 0x0072F23E.. |
| CGameTimeManager.h | NEW, FABLE_RETAIL_CGAMETIMEMANAGER | World..DayCount (+0x00..+0x14), inline GetDayCount | Ego_r.pdb CGameTimeManager layout; ?GetDayCount@CGameTimeManager@@QBEJXZ (Ego_r + FableWin), inline at 0x0072F1E4 and in landed 0x00775B40 / 0x0065B854 |
| CTCObjectAugmentations.h | FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_MEMBERS | virtual OnInitialActivate, private AttachAugmentationParticles / SetupAugmentationSounds, PDef +0x0C, LoadedFromSaveGame +0x10, Augmentations vector +0x14 | Ego_r.pdb / FableWin.pdb spellings; retail accesses in 0x00768010 / 0x00768120 |
| CTCObjectAugmentations.h | FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_DAMAGEMULTIPLIER | GetDamageMultiplier() const decl (0x00766140) | Ego_r.pdb / FableWin.pdb ?GetDamageMultiplier@CTCObjectAugmentations@@QBEMXZ; landed out of line |
| CThing.h | FABLE_RETAIL_CTHING_CUTSCENEPAUSE | SetCutscenePauseBehaviour(ECutsceneBehaviour) decl (0x004C78A0) + ECutsceneBehaviour.h include | Ego_r.pdb / FableWin.pdb ?SetCutscenePauseBehaviour@CThing@@QAEXW4ECutsceneBehaviour@@@Z; push 2 at 0x00730692 |
| CTCDoor.h | FABLE_RETAIL_CTCDOOR_CTOR (nested) | CTCDoor(CThing&) decl | Ego_r.pdb / FableWin.pdb ??0CTCDoor@@QAE@AAVCThing@@@Z |
| CTCCameraCollision.h | FABLE_RETAIL_CTCCAMERACOLLISION_ONCREATE | virtual OnCreate; member typed CDefPointer<const CCameraCollisionDef> (same 4 bytes) | Ego_r.pdb / FableWin.pdb ?OnCreate@CTCCameraCollision@@UAEXXZ, Ego_r member type; GetSubDef 0x007E3290 |
| CTCActionUseBase.h | FABLE_RETAIL_CTCACTIONUSEBASE_ONCREATE | virtual OnCreate decl | Ego_r.pdb / FableWin.pdb ?OnCreate@CTCActionUseBase@@UAEXXZ; GetSubDef 0x007EAE40 |
| CHeroSpecialMovementDef.h | FABLE_RETAIL_CHEROSPECIALMOVEMENTDEF | : CSubDefClassBase; TotalTimeToSprintSeconds +0x28.. | RTTI .?AVCHeroSpecialMovementDef@@ (rtti_bases.tsv); Ego_r.pdb members; fmul [eax+28h] 0x007E0970 |
| CTCHeroSpecialMovement.h | NEW | class : CTCBase, members +0x0C..+0x18, OnCreate | RTTI .?AVCTCHeroSpecialMovement@@; Ego_r.pdb layout (0x1C); vtable 0x0126FE3C slot 4 |
| CTCDegradableBase.h | NEW | class : CTCBase, protected virtual OnCreate, members +0x0C..+0x18 | RTTI .?AVCTCDegradableBase@@; Ego_r.pdb layout (0x1C); vtable 0x0126A6A4 slot 4; ?OnCreate@CTCDegradableBase@@MAEXXZ |
| CTCCombat.h | FABLE_RETAIL_CTCCOMBAT_ABILITYLIST (nested) | RemoveMeleeFlourishAbility decl (public), private UpdateUsableMeleeCombatAbilities (0x00736590) | Ego_r.pdb / FableWin.pdb ?..@CTCCombat@@QAEXJ@Z / AAEXXZ |
| CHairCardDef.h / CTattooDef.h | FABLE_RETAIL_CHAIRCARDDEF_TYPENAME / FABLE_RETAIL_CTATTOODEF_TYPENAME (nested) | inline StatGetTypeName (class-name literal) for the shared GetSubDef body | FableWin.pdb ?StatGetTypeName@CHairCardDef@@ / @CTattooDef@@SI?AVCCharString@@XZ; inline at 0x005DA180 / 0x005B1EFC |
| CGiftDef.h | NEW (guard FABLE_ENGINE_CGIFTDEF_SHARED_H; the generated flat model stays in engine/_quarantine/CGiftDef.h) | : CSubDefClassBase, EGiftType Type +0x28, IsWeddingRing +0x2C, StatGetTypeName | RTTI .?AVCGiftDef@@; Ego_r.pdb CGiftDef (0x30); mov edx,[ecx+28h] 0x00894C3C |
| CPerceivedThingDef.h | FABLE_RETAIL_CPERCEIVEDTHINGDEF | : CSubDefClassBase, members +0x28..+0x4C | RTTI .?AVCPerceivedThingDef@@; Ego_r.pdb members; reads 0x00744F14..0x00744F4F |
| CTCPerceiveThing.h | FABLE_RETAIL_CTCPERCEIVETHING_RADII (nested in AWARENESS) | SetSound/Smell/Sight/ExtendedSight/GiveUpChaseRadius + ResetPerceptionVariables decls | Ego_r.pdb / FableWin.pdb QAEXM@Z / QAEXXZ; landed 0x00744470..0x00744DB0 |
| CTCInventoryWeapons.h | FABLE_RETAIL_CTCINVENTORYWEAPONS_ITEMDESCRIPTION | virtual GetItemDescription (UBE), GetItemDescriptionNoDamageNorWeight / Weight / Damage (QBE) | Ego_r.pdb spellings; slot 78 of vtable 0x01253FF4 |

Unproven parts kept out of the default path: EGiftType / EPerceivedThingType enumerators (declared without names); StatGetTypeName
literals by the class-name convention (bytes are not read by these rows' grade beyond the shared template body).
