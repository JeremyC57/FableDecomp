# Chunk 576 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **A wait loop that retail does NOT rotate is a do-while (0x00DD8DE0).** Retail tests `Me.MsgIsUsedBy("SCRIPT_NAME_HERO")`
  once at the loop top, keeps the result raw in bl (`mov bl,al`), moves the success arm out of line and gives the
  termination exit its own `pop ebx ... ret`. Source: `do { if (used) { ...; return; } NewScriptFrame(); } while
  (!IsActiveThreadTerminating());`. `while (!used)` (neg/sbb, test duplicated, 309v259) and `for (;;) { if (used) break; ... }`
  (298v259) are both rotated by VC7.1.
- **Threading one null test through several inline CCharString compares needs the callee's body in the TU (0x00F088E0).**
  Five `name == "..."` tests on a by-value CCharString compile in retail to two chains (null arm: intrinsic strcmp against "";
  non-null arm: PData cached in esi across five Compare calls). With Compare 0x00411570 only declared, VC7.1 reloads and
  re-tests the argument after each call (355v325); defining `CBasicString<char>::Compare` in the TU under
  `#pragma auto_inline(off)` (the landed 0x004FDCB0 trick) matches at once.
- **IF-macro arm order: test the flag the way retail lays it out (0x00DA24F0).** Retail's fall-through arm is the
  `!ChantTimerPaused` one; `if (ChantTimerPaused) {A} else {B}` comes out arm-swapped (272v276), `if (!ChantTimerPaused) {B}
  else {A}` matches. Every arm of the script IF macro opens with `if (IsActiveThreadTerminating()) return;`.
- **CScriptDef offsets: retail = Ego_r - 0xa8 before FHPatterns, - 0xb0 after FHTimeLimit (0x00D86A30).** The two Dinkumware
  vectors (0x10 -> STLport 0xC) add 4 each; HS_GMShortDistance / HS_GMLongDistance / HS_GMGuardDistance are retail
  +0xc90 / +0xc94 / +0xc98.
- **CEntityScriptBinding / CSpawnedFunc new-expressions and Transfer/TransferVector persist rows match on the first probe from
  the landed chunk 573-575 models** (0x00E54450, 0x00EEEC00, 0x00E37520, 0x00E54880); vector<bool> members of
  CV_BookCollectingScript are 12 bytes in retail (BookDonated +0xa0, BookOwned +0xac, size 0xB8), so a plain three-pointer
  STLport vector model gives the exact TransferVector<bool> symbol.

Review at landing:
- 0x00DD8DE0 took 4 grades (the loop form); no row needed more.
- **0x00F088E0 emits an extra function body** (`?Compare@?$CBasicString@D@@KIJPBD0@Z`, the landed 0x00411570 loop) in its TU
  under auto_inline(off), as the landed 0x004FDCB0 does; the leaf is EntityCountDecrease (largest function too, but the
  `// Leaf:` line pins it). The landed 0x00411570 itself is a free `long __fastcall strcmp_signed(const char*, const char*)`
  (`?strcmp_signed@@YIJPBD0@Z`); it is the Ego_r `?Compare@?$CBasicString@D@@KIJPBD0@Z` that the inline CCharString::operator==
  calls (the landed 0x00DFC320 and this row bind it so; reloc_bindings has five other spellings; NAME_FIXES.tsv).
- **Landed CTestTheInventoryScript rows are global structs with invented spellings:** Main 0x00EEEA90 (non-virtual, global),
  TestForLampOff 0x00EEED20 (global), and both bind the spawn vtable 0x012F59AC as `CSpawnedFunc0<CTestTheInventoryScript>`;
  Ego_r has `??0?$CSpawnedFunc@VCTestTheInventoryScript@NScript@@@@...` (NAME_FIXES.tsv). This chunk's TestForLampOn binds the
  NScript spellings, so expect a cross-row RELOC_NAME/CONFLICT until those two are respelled.
- **Landed entity Mains as global classes:** CPrisonKeyDoor::Main 0x00DD9180 and CWaitingGuardInWoods::Main 0x00D86680 are
  `?Main@<entity>@<quest>@NScript@@EAEXXZ` per their vtable slot and Ego_r (NAME_FIXES.tsv); 0x00D86680 also names the
  CScriptDef longs descriptively (they are HS_GMLongDistance +0xc94 / HS_GMGuardDistance +0xc98).
- 0x00CFF9A0 PlayCutscene is bound without NScript (NAME_FIXES.tsv); 0x00CB8930 still carries the catalog label
  CTCVillage::OnInitialActivate (the vland RELOC_NAME warning on 0x00E54450).
- The landed PauseChantTimer 0x00DA2490 models CQ_MinionCampScript flat and binds 0x0143E8F8 as `g_pThingManager_0143e8f8`
  (`CThingManager_Calls`); this chunk's RunChantTimer uses chunk 573's `g_pGameScriptInterface` spelling for the same global.
  0x0143E8F8 still needs one agreed descriptive name.
- Local models next to flattened headers (CBaseObject, CSpawnedFuncBase): `// Layout:` lines + NOTES "Header debt".
