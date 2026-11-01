# Hand chunk 660 (CTCCameraCollision .. GGame initializer): 10 of 10 byte-exact

NA005 of the never-attempted pool (landed-twin rows). All ten are RELOCATION_MATCH with behaviour PASS under g++ and the
lane's VC7.1 cl.exe; vland (read-only) 10/10 RELOCATION_MATCH + PASS. Every catalog label was wrong (10 of 10); each row's
identity was settled from its caller chain / vtable slot / TU neighbours and the PDB name sets before authoring, then the
twin's shape was reused with the row's own classes. Eight rows matched on the first probe, two needed a second probe only to
clear vland SHARED_HEADER_AVAILABLE warnings (no byte change).

## Proven
- 0x007E3370 ??1CTCCameraCollision@@UAE@XZ -- sole caller 0x007E3350 = slot 0 (??_G) of vtable 0x01270394 (RTTI CTCCameraCollision : CTCBase); FableWin ??1CTCCameraCollision@@UAE@XZ, Ego_r has none (implicit); member CameraCollisionDef (CDefPointer<const CCameraCollisionDef>) +0xC (Ego_r layout).
- 0x0087B660 ??1CProjectileWeapon@NCameraModes@@UAE@XZ -- sole caller 0x0087B640 = slot 0 of vtable 0x01277804 (RTTI NCameraModes::CProjectileWeapon : NCameraModes::CBase), the vtable the landed ctor 0x00879620 stores; FableWin ??1, Ego_r none (implicit); frees Attractors (vector<C3DVector>) +0xF0 (Ego_r +0x114 - 0x24), tail-jumps ~CBase 0x0056E0E0.
- 0x007D7950 ??1CCardDef@@UAE@XZ -- in the CTCTavernGameCardPairs TU after GetRandomCard 0x007D75A0, which builds a local CCardDef (vtable 0x01243A44 = RTTI CCardDef); followed by the unreferenced implicit operator= 0x007D79C0 and default ctor 0x007D7A00 (stores 0x01243A44): FableWin lists exactly ??1CCardDef@@UAE@XZ / ??4CCardDef@@QAEAAV0@ABV0@@Z / ??0CCardDef@@QAE@XZ; Ego_r no ??1. RTTI CCardDef : CParentDefClassBase; its ??_G 0x004EC0D4 calls ~CParentDefClassBase 0x00430300, whose body this inlines.
- 0x0072D3E0 ??1CCreatureAction_SheatheItemToInventory_ForDivineWrath@@UAE@XZ -- sole caller CTCSpecialAbilities::SetupDivineWrath 0x0072CD00 (local built by ctor 0x0069F090, vtable 0x0125CA7C); FableWin ??1..@@UAE@XZ; RTTI : CCreatureAction_SheatheItemToInventory (PFollowUpAction CCountedPointer<CCreatureActionBase>, Ego_r +0xB4 -> retail +0xA8).
- 0x0072D430 ??1CCreatureAction_SheatheItemToInventory_ForUnholyPower@@UAE@XZ -- sole caller SetupUnholyPower 0x0072CE20 (ctor 0x0069F0C0, vtable 0x0125CB9C); FableWin ??1..@@UAE@XZ.
- 0x006DF920 ??1CCreatureAction_SheatheTrophy@@UAE@XZ -- CTCTrophy TU: between CCreatureAction_SheatheTrophy's ctor 0x006DF900 (base ctor 0x0069FFD0 with (creature, 0, 0x3EA), vtable 0x0125FB14 = RTTI CCreatureAction_SheatheTrophy) and its ??_G 0x006DF9B0 (inlines the same sequence); ShowToSheatheTrophy::FinishAction 0x006DD980 destroys a local SheatheTrophy; FableWin ??0CCreatureAction_SheatheTrophy@@QAE@AAVCThingCreatureBase@@@Z + ??1CCreatureAction_SheatheTrophy@@UAE@XZ.
- 0x009771A0 ??1CCreatureAction_JackScream@@UAE@XZ -- right after JackScream's ctor 0x00977100 (vtable 0x01297CB4, zeroes +0x114/+0x118) and IsToEvadeAllHits 0x00977190; ??_G 0x00977440 (slot 0 of 0x01297CB4) inlines the identical release before ~CCombatActionBase 0x0062E900; temporary built and destroyed at 0x00975758; FableWin ??1, Ego_r none; PModeCountedPointer (CCountedPointer<NCreatureMode::CModeJackOfBladesScream>) Ego_r +0x12C -> +0x114.
- 0x0090C780 ?SetGuardFollowingPlayer@CTCVillage@@QAEXPAVCThingAICreature@@@Z -- sole caller CAIStateGroup_MinionGuardFollowPlayer::Update 0x0090C540 calls it on CTCVillageMember::GetPVillage() 0x007EE540's result with the creature; member +0xBD = Ego_r GuardFollowingPlayer +0x149 less 17 STLport lists x8 and one set x4; Ego_r + FableWin both name SetGuardFollowingPlayer (beside GetGuardFollowingPlayer); temporary vtable 0x0123E7E0 = CIntelligentPointer<CThingAICreature>.
- 0x01219F00 `dynamic initializer for 'GGame'` (leaf _$E1; object ?GGame@@3VCGame@@A 0x013B83D0) -- .CRT$XCU slot 0x01374218; CInitBaseClass ctor 0x0099A310 + CGame vtable 0x0122E59C; atexit thunk 0x01228930 -> ~CInitBaseClass 0x0099A410; Ego_r + FableWin name it, retail XCU order matches Ego_r's (RelaunchSaveGameName, GGame, GDebugString).
- 0x0121AB3F `anonymous namespace'::`dynamic initializer for 'NEWLINE'` (leaf _$E1; object NEWLINE 0x013B8BA8, const CWideString from L"\n") -- .CRT$XCU slot 0x0137438C; CWideString(const wchar_t*) 0x0099B6B0; all users in the CTCInventoryWeapons TU; Ego_r + FableWin list that TU's anonymous-namespace initializers in the order of retail's run (BANK_HANDLE "GBANK_MAIN" 0x013B8BA4, NEWLINE L"\n" 0x013B8BA8, WEAPON_DESCRIPTION_STRING "Weapon_Descriptions" 0x013B8BAC ...).
Descriptive (not retail names): the five `TUHarness_*` functions (0x007D7950, 0x0072D3E0, 0x0072D430, 0x006DF920, 0x009771A0 sources) only destroy a local so the implicit ??1 is emitted. Unproven details: NEWLINE's const-ness and the retail data symbol (TU-local; the donor hash is ?A0x0a34196c, the retail hash unknown).

## CRT / initializer region (0x0121AB3F, 0x01219F00)
Both are compiler-emitted dynamic initializers (each is referenced only from its .CRT$XCU table slot, constructs an engine
object with an engine constructor and registers a `_$E2` atexit thunk), not vendor library objects: authored as C++ (a
namespace-scope object definition), leaf `_$E1`; no toolchain_library / lib_verify proof applies. vland's RELOC_NAME warnings on
`_$E2` (0x01228930 "AtexitDtor_CInitBaseClass_at_013b83d0", 0x01228FD9 "AtexitDtor_CCharString_at_013b8ba8") are the expected
TU-local thunk names; the second label is wrong in kind (the object is a CWideString).

## What matched (10)
- CTCCameraCollision (0x007E3370): the landed ~CTCFishingRod model (engine/CDefPointer.h, ctor defined to emit ??_G/??1).
- NCameraModes::CProjectileWeapon (0x0087B660): STLport 4.6 vector<C3DVector> (~vector _Destroy no-op, ~_Vector_base null test + free) over a local NCameraModes::CBase; ctor defined (the retail TU defines it).
- CCardDef (0x007D7950): engine/CDefClassBase.h (retail) + a local implicit CParentDefClassBase (InstantiationName, CVectorMap SubDefInfoMap over the STLport vector) + CCardDef (PDB Transfer/Copy/GetSizeofClass overrides); harness destroys a local.
- CCreatureAction sheathe family (0x0072D3E0, 0x0072D430, 0x006DF920) and CCreatureAction_JackScream (0x009771A0): engine/CCountedPointer.h member in the right class level over a local retail-size CCreatureActionBase (0xA8) / CCombatActionBase (0x114); harness or inline ctor.
- CTCVillage::SetGuardFollowingPlayer (0x0090C780): `GuardFollowingPlayer = guard;` through the converting CIntelligentPointer<CThingAICreature> ctor and operator= (plain inline, no __forceinline needed).
- GGame / NEWLINE initializers: plain namespace-scope definitions; CGame's inline empty ctor.

## Parked for Fable 5.1 (0)

## Skipped
None.

## Label corrections
- 0x007E3370 "C3DAnimationBlendState::~C3DAnimationBlendState" -> CTCCameraCollision::~CTCCameraCollision.
- 0x0087B660 "CTCInventoryItem::OnDie" -> NCameraModes::CProjectileWeapon::~CProjectileWeapon.
- 0x007D7950 "CTCInventoryItem::OnDie" -> CCardDef::~CCardDef.
- 0x0072D3E0 / 0x0072D430 / 0x006DF920 / 0x009771A0 "_Dest_val<...CCountedPointer<C3DSkeletalMorphTarget>...>" -> ~CCreatureAction_SheatheItemToInventory_ForDivineWrath / ~..._ForUnholyPower / ~CCreatureAction_SheatheTrophy / ~CCreatureAction_JackScream.
- 0x0090C780 "CVillageTaskCarryCrate::AssignCreatureToTask" -> CTCVillage::SetGuardFollowingPlayer.
- 0x0121AB3F / 0x01219F00 "_gapscan::sub_*" -> the NEWLINE / GGame dynamic initializers.
- Twins (landed rows, respell at landing; NAME_FIXES.tsv): 0x0048C950 -> ~CCreatureAction_SheatheItemToInventory, 0x00430300 -> ~CParentDefClassBase, 0x005DAE40 -> ~CTCWeapon, 0x007D9EF0 -> ~CTCTavernGameSpotTheAddition; 0x0087B640 -> NCameraModes::CProjectileWeapon scalar deleting dtor (already labelled, stand-in spelling); 0x01228930 / 0x01228FD9 -> `dynamic atexit destructor for 'GGame'` / `anonymous namespace'::`dynamic atexit destructor for 'NEWLINE'`.

## Header debt
- engine/CTCBase.h, engine/CCreatureActionBase.h, engine/CCreatureAction_JackScream.h, engine/CCreatureAction_SheatheItemToInventory_For*.h, engine/CTCVillage.h flatten the vptr / base chain: local polymorphic models at retail offsets.
- engine/CParentDefClassBase.h declares ~CParentDefClassBase out of line (0x00430300); retail also expands it inline into derived implicit dtors (~CCardDef 0x007D7950), so a row that needs the inline expansion models the class locally. An inline-body variant of the header (implicit dtor) should be tested against 0x00430300 and its callers.
- No engine/CGame.h / CInitBaseClass.h (fable_game.h's CGame is flat, non-polymorphic); no NCameraModes::CBase header (engine/CBase.h is an unrelated namesake).
