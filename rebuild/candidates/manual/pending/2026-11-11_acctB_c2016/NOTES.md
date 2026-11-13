# Hand chunk 2016 (CThing .. CTCTavernGameCardPairs): 2 of 3 byte-exact

All grades were taken with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2016/hdr` (hdr/README.md lists every header change).
Read-only verifier (`vland.py payload.json oracle.tsv`, no --land): both src rows RELOCATION_MATCH + behaviour PASS, no RELOC_IDENTITY;
four RELOC_NAME warnings on 0x004CAA00, all label corrections (NAME_FIXES.tsv, plus chunk 2008's ThingTypeToTypeFlags). Tests PASS under
g++ (tall.sh) and VC7.1 (tall.py). Row lengths: all three catalog lengths are right (each row ends at its last `ret` / jmp, int3 after);
no SPLITS.tsv.

## What matched (2)
- **CThing** 0x004CAA00 `?ConstructFromLevelScript@CThing@@UAE_NPBD@Z` (catalog name already right), RELOCATION_MATCH on pass-1 grade 4.
  The text-side mirror of chunk 2008's ConstructFromBinarySave 0x004CA390 (same TU): same prologue up to the section name, then
  `LevelPersistent = false`; the parser over a named CCharString copy of the script, the hero-state UID or SerialiseThingInfo +
  InitFromThingInfo, the map number (Transfer<short> "ThingCurrentMapID" or GetLevelBeingLoaded), the two persistence flags, the
  ScriptName / ScriptData / EndThing loop, the remapped UID, the def / editor / map-who / creatable components, then every live
  component's SerialiseLevelState. Levers: the render / update links cleared Next before Prev; the CBasicString<char> Compare /
  operator==(const char*) bodies visible in the TU (VC7.1 then hoists the key's null test: one `repe cmpsb` ladder, one operator==
  ladder on esi); the TU's out-of-line CThing::PeekComponent defined in the TU (expanded); the final component walk with a separate
  `next` iterator. Retail builds a second CStringParser over the same text that nothing reads (0x004CAC8C..0x004CACB9; kept as
  `unused_parser`).
- **CTCInventoryQuests** 0x0061AD60 `?DrawActiveTestQuests@CTCInventoryQuests@@QAEXABVCEnginePrimitivePrepareInfo@@@Z`, RELOCATION_MATCH on
  pass-1 grade 5. Levers: the 4-colour backdrop vector filled black then `colours[i].Initialise(...)` in place; the title position one
  C2DVector from two GetPDef() copies (Y first); the two "..." strings as named locals; `selected = true` before the HSP-name assignment;
  the counter as the loop's second increment; __fastcall std::__destroy_aux (overlay opt-in); GetTestQuestsToDisplay's body (landed
  0x0061A8A0) visible and not expanded in the TU, so VC7.1 knows the local vector is not captured (a diagnostic probe with a trivial
  body proved the lever first: 1633v1631 -> RELOCATION_MATCH).

## Parked for Fable 5.1 (1)
- 007d6870 CTCTavernGameCardPairs::ProcessInput DIFFER(1778v1778): 540/571 instructions equal with frame offsets masked; frame 0x30 v
  0x38 (the |dy| GFFloatAbs copy and dy share string-temporary slots, retail gives them their own words [esp+10h] / [esp+24h]);
  x87 operand order of the inline GetRight cross product (0x007D6BFC fld [edi+10h]/fmul [edi+8], 0x007D6C15..0x007D6C1D) and of the
  first offset Dot (0x007D6C3D..0x007D6C4D); final Dot product order (retail X, Z, then Y at 0x007D6ED1..0x007D6EDB; ours Z, Y, X for
  every spelling tried); eax/edx swap in the two inline def releases (0x007D6F0B / 0x007D6F53) | tried: 14 graded / scripted full-row
  compiles (pass 1: 12, pass 2: 2) + ~30 reduced-fragment probes + 4 c2 frame-packer traces: Dot variants (GROUPED / ORDERED_XYZ /
  plain; plain matches the first two dots in isolation), GetRight variants (operator^, RIGHT_COMPONENTS, per-component paren / factor
  orders, named right, right-as-this: the fragment shows RIGHT_COMPONENTS fixing 5 of 6 cross products in isolation, not in the full
  row), final-dot spellings (operand order, named vectors, GFDotProduct bodies, compare forms: all invariant), named vs implicit
  PlaySoundW strings (named ones cross-jump WIN/LOSE and shrink the frame), dx/dy at function scope (no frame change), GFFloatAbs by
  reference, a C2DVector delta | next: trace c2's x87 candidate ids for the cross product and the final dot (the commutative-order
  lever of MATCHING_PLAYBOOK), and find what makes retail's string temporaries conflict with the movement-block floats in the frame
  packer (c2 trace: our |dy| copy n6 and dy n17 join the WIN / FLIP string slots); a test is not written yet.

## Skipped
None.

## Names
- Proven (two sources each; LABELS.tsv): the three rows; CThing::CreateSerialiseCreatableTCs 0x004CA230 (Ego_r + FableWin, protected);
  CThingManager::GetLevelBeingLoaded 0x0051E2C0 (Ego_r donor order + body); NInventory::CTCInventoryBase::Draw2DText 0x005BCAFE (static
  __fastcall) and DrawInventoryText 0x005BCE49; CRenderManager2D::Draw2DBoxColours 0x009DDA50 (array parameter); CQuestManager::IsQuestActive
  0x004AF610 (const); CTCInventoryQuests::GetTestQuestsToDisplay 0x0061A8A0 (bound); CTCTavernGameCardBase::ProcessInput 0x008F12D0;
  CWorld::GetActiveTestQuests (FableWin inline); CTestQuestNameCompare (Ego_r).
- Label corrections: NAME_FIXES.tsv (7 lines: CreateSerialiseCreatableTCs, Draw2DText, DrawInventoryText, Draw2DBoxColours (PBV -> QBV),
  GetLevelBeingLoaded / IsSerialisingGameState manifest labels, GetControlMovementFromGameActionEvent). Chunk 2008's 0x0051E580
  ThingTypeToTypeFlags fix also applies here (vland RELOC_NAME warning).
- Descriptive (searched Ego_r / FableWin / Ego_d name sets, fablewin_editor_symbols, Fable.exe strings, reloc_bindings; nothing names
  them): `g_bResetUIDsOnLoad` 0x013B8636 (global bool, read only by 0x004CADC6 and CMap::LoadFromFile 0x00821B73, never written);
  `TEST_QUEST_GREEN` / `TEST_QUEST_YELLOW` / `TEST_QUEST_RED` 0x0137D70C / 0x0137D710 / 0x0137D714 (statically initialised TU colours);
  the function-local static `flash` 0x013B8C84; `UseCardBaseInput` 0x01383C3F (parked row; a TU static bool initialised true, never
  written); the NInventory::ETextLayout enumerators TEXT_LAYOUT_LEFT / CENTRE / RIGHT (the landed DrawInventoryText's spelling);
  `ECardGameType` values not needed. All local variable names.
- PDB-derived but unconfirmed in retail data symbols: NInventory::WHITE / GREY / BLACK (FableWin.pdb dynamic initializers of 148 TUs;
  retail 0x0137D700 / 0x0137D704 / 0x0137D708 by value and order).
- Established spellings reused: g_DefaultSerialisationSectionName 0x013BAB0C, VIRTUAL_SCREEN_WIDTH / HEIGHT, PRenderManager 0x013B8384,
  PFontManager 0x013B838C, DELTA_NOTIONAL_ZERO.

## Header debt
- No repo header yet for CCard, CTCTavernGameCardBase, CTCTavernGameCardPairs (new overlay headers); the landed card-game rows
  (0x007D5F60.., 0x008F0930..) still use local padded models.
- engine/CInventoryDef.h was partial (now FULL opt-in); engine/CRenderManager2D.h, CQuestManager.h, CWorld.h, CTestQuest.h,
  NInventory_CTCInventoryBase.h, CTCInventoryQuests.h, CThing.h, CThingManager.h, stlport/fable_containers.h carry new opt-ins (hdr/README.md).
- The landed GetTestQuestsToDisplay 0x0061A8A0, Draw2DText 0x005BCAFE, DrawInventoryText 0x005BCE49 and CreateTCsFromString 0x004CA230
  rows use local stand-in models / wrong spellings (NAME_FIXES.tsv).
