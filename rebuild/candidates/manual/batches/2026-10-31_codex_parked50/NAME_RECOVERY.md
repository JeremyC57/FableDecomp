# Naming and type recovery

This pass treats supplied manifest/catalog labels as hints. Evidence was taken from original Ego_r/FableWin name/type records, native callers/fields/vtables, literals, and original STLport 4.6.2 source. The detailed claim-by-claim evidence is retained in each row's NAMING_EVIDENCE.md, NAMING_TYPES.json, PRIOR_NAME_EVIDENCE.json or NEW_STRATEGY.md and the corresponding evidence/*.json files. The current archive does not claim a complete exact-symbol audit for parked rows.

## Verified rows

- **0044c6c2:** CGameDefinitionManager's original base/allocator hierarchy and four typed const definition handles were recovered using donor order and GLOBAL, ENGINE, ENVIRONMENT and HERO_STATS literal call sites. Their native offsets are D0/D4/D8/DC. Exact private member spellings were not available and remain qualified fallbacks; the template types are not guesses.
- **006772d0:** CThingBaseDef, CDefString, CSmallVector and packed component-entry layout were recovered from original type records. CDefString's default TablePos is -1. Native parent field57 differs from donor61 because of container layout. The actual resource constructor at009fbec0 and CThingBaseDef vtable at0125b664 were checked.
- **006f50b0:** the function is the original std::_Rb_tree erase instantiation containing pair<const unsigned __int64,CCombatantInfo>; CCombatantInfo is a struct and CombatRings is private. The exact STLport types/access and three helper symbols are in ROW.json. Generic CCombatantMap aliases were rejected.
- **007cbf60:** CTCMultiArrow destructor uses the real CDefPointer/CCountedPointer model and genuine fastcall IS_THING_ALIVE/_Destroy declarations. The full original class model and typed vector access produce the accepted leaf.
- **007f9010:** CTCAnimatingObject::PlayAnimation(long,long,bool), float current/last animation frame fields, actual update enum and std::list representation were restored. This was not an integer-frame model.
- **00845b10:** CCreatureAction_TavernGameMasterInteraction uses CTCTavernGameMaster, not the invented CTCGameState. Original protected TavernGame pointer is at12; CTCTavernGame's IsBeingPlayed is at32, and EnterGame is virtual slot29. The target intelligent pointer and inherited creature pointer use the original types/layout. Calls resolve through the nonconst LowerBound at0040f020.
- **0085fb30:** NCreatureMode::IsAbleToBeAttacked and IsKnockedDown/IsUnderground names and const-reference signatures are original. Action virtual prefix through slot47, ThingTypeFlags, and nested CTCCreatureModeManager::CModeBase were recovered. Const LowerBound is004365b0; GetCurrentMode is0073a6e0.
- **008bf1c0:** CCreatureAction_UnblockableMidAttack::Initialise, true physics SetFacingAngleXY slot66, and GFGetAngleXYTo at00661540. The old AngleBetweenPositions label was wrong; original names and native fpatan behavior agree.
- **008c6160:** CCreatureAction_RockTrollFallToDeath::Initialise uses the actual class tag/type/accessor, action prefix and creature pointer at8.
- **009395f0:** CAIStateGroup_SitRoundCampfire::GetPNearCampfire is private const. CIsNearCampfireWithSpace/CThingCompare_Nearest class keys and search template signature were checked. Original CAIBrain base/reference fields matter to the caller.
- **00c0acc0:** no original identity was found after the recorded search. CUnprovenFunction_00c0acc0 is an allowed, explicitly unproven fallback, not a settled recovered name. The native caller and body establish its fastcall float/mask ABI; no external callees/globals are declared in this leaf.

## Other important corrections recorded during parked work

- 00933db0 calls **CCreatureAction_KickCreature** at00933f10; it is not the similarly named KidAnnoy action constructor.
- 007da520 is **CTCTavernGameCardBlackjack::GetRequiredScore**, and005ea000 is the original CTCTavernGame::GetDef out-pointer method. Golf/game-def aliases were misleading.
- 00af12a0's final base destructor is **CDataStream::~CDataStream** at00993b10; restoring the actual hierarchy exposes an extra required intermediate-vtable store that the old short model omitted.
- **VIRTUAL_SCREEN_WIDTH** at013961e8 and **VIRTUAL_SCREEN_HEIGHT** at013961f0 are mutable double data symbols, distinct from PIXEL_WIDTH/HEIGHT at013ca800/013ca808. See DOUBLE_GLOBALS_EGOR.json and native InitVirtualPixels evidence. A prior fallback remains only in historical/closer candidate snapshots, not the selected009e1dd0 source.
- CMemoryAllocatorBase's **CurrentAlignment** at013d28d4 and **CurrentAlignmentShift** at013d28d8 are protected static unsigned longs; originals are recorded with their mangled names in NAME_FIXES.tsv.
- MAIN_FOCAL_POSITION_NAME at013bac48 and TARGETING_CURSOR_POSITION_NAME at013bac50 are original const CCharString globals.
- GDefStringTable at013ca828, PMainGameComponent at013b86a0 and ConstantFPS at01375550 were recovered from original data/type evidence and native references.
- DEFAULT_RHSET at01380104 is supported as an original raw/local data identity. A decorated public symbol was not recovered; do not treat a convenience external reference as proof of an original exported symbol.
- 00c462e0's supplied GetRenderZOffset identity has the wrong apparent ABI. Its four-corner interpolation identity remains unproven and is explicitly represented with CUnproven names.

NAME_FIXES.tsv records 30 address/name findings and the incorrect spellings seen. One entry (00c0acc0) is clearly marked a permitted unproven fallback. These are review metadata, not edits to helper implementations or to the repository's binding table. Original snapshots and wrong spellings remain in historical evidence so the maintainer can trace each change.
