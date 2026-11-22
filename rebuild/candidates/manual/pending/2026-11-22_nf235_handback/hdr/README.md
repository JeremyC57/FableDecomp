# NF235 header overlay (2026-11-22): add-only, member-specific opt-ins (entries q12656 / q12657)

Base copies: `orig/engine/{CFrontendGameComponent,CUserProfileManager,CMainGameComponentInit,CGameJoystickManager,CCountedPointer,CMeshBankManager}.h`
(the repo tree with nf234 landed). Other overlays of these headers (`$S/w22/agent*/hdr/engine/<h>`, AUTH24 / C2T folders not read): CFrontendGameComponent.h
in NF226 (landed) and NF234 (= repo); CUserProfileManager.h in NF55 / NF66 (landed long ago); none of the others. `grep -rlw` of the new macros over
rebuild/ and every other agent hdr/ (work/macros.txt): no hit. Every new text is inside a new `#ifdef` / `#if defined` term (the old text is the
`#else` or untouched), except three edits to existing lines: CMeshBankManager.h's `#ifndef FABLE_RETAIL_CMESHBANKMANAGER_VIRTUAL_DTOR` becomes
`#if !defined(..VIRTUAL_DTOR) && !defined(..CTOR_DTOR)` (same line), CCountedPointer.h's size-block `#if defined(...)` line gains two terms, and seven
FABLE_STATIC_ASSERTs are pinned to their _AT form (old line numbers). The default path preprocesses identically (equiv_check); no layout change.

- `FABLE_RETAIL_CFRONTENDGAMECOMPONENT_SETQUIT` (CFrontendGameComponent.h): `virtual void SetQuit();`, the public override of the protected
  CGameComponent::SetQuit (all three PDBs ?SetQuit@CFrontendGameComponent@@UAEXXZ; RTTI vtable 0x01238768 slot 4 = 0x00494370). No new slot.
- `FABLE_RETAIL_CFRONTENDGAMECOMPONENT_CTOR_DTOR` (CFrontendGameComponent.h): a fourth layout branch (one per TU, not with CLEARSTRING /
  INITGRAPHICS / SHUTDOWN): the whole Ego_r.pdb layout at the retail offsets (header comment lists them), `virtual ~CFrontendGameComponent();`
  and the private members the three rows call (Input, Output, InitGraphics, Shutdown, SetMode(EGameMode), GetWorldNames, LoadProfile; all
  three PDBs, AAE). Declares `enum EGameMode { };` (global, as the PDB mangling W4EGameMode@@ and the landed 0x00495FE0) and
  `NGameComponentHelpers::ECursorMovement` unless FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_CTOR_DTOR (which declares the same enum) is defined.
  Same size as the default path (0x1E60).
- `FABLE_RETAIL_CUSERPROFILEMANAGER_DEFAULTPROFILENAME` (CUserProfileManager.h): declarations of ?GetDefaultProfileName@..@@QBE?AVCWideString@@XZ
  (0x004065D0) and ?SetProfileName@..@@QAEXABVCWideString@@_N@Z (0x0040D400), Ego_r + FableWin, the spellings the table already binds.
- `FABLE_RETAIL_CMAINGAMECOMPONENTINIT_INLINE_CTOR_DTOR` (CMainGameComponentInit.h): the default ctor and dtor inline (empty bodies), as Run
  0x00496CF0 expands them; same symbols as the out-of-line owners 0x004131A0 / 0x004131D0, whose pick-any copies EQ (work/pk_00496cf0_new.tsv).
- `FABLE_RETAIL_CGAMEJOYSTICKMANAGER_MAINPLAYERJOYSTICK` (CGameJoystickManager.h): ?Get@CGameJoystickManager@@SIAAV1@XZ (0x0049BB90) and
  ?SetMainPlayerJoystickDeviceNumber@CGameJoystickManager@@QAEXJ@Z (0x0049BB30), Ego_r + FableWin. Its four FABLE_STATIC_ASSERTs are pinned.
- `FABLE_RETAIL_CMESHBANKMANAGER_CTOR_DTOR` (CMeshBankManager.h): the retail class (bases CBaseClassNonCopyable + CDeviceResetCallback, RTTI
  COL 0x01356AB4 / 0x01356A64), ctor / virtual dtor / OnPreDeviceReset / OnPostDeviceReset (Ego_r + FableWin), the nested
  CMeshDataBankInfo (BankHandle CCharString +0, MeshDataBank CCountedPointer<CMeshDataBank> +4) and `std::list<CMeshDataBankInfo> MeshDataBanks`
  +0x08, an alternative class definition inside the existing `#pragma pack` block (the default class is the `#else`). The `__vftable` offset
  assert is skipped for this branch (`#if !defined(..VIRTUAL_DTOR) && !defined(..CTOR_DTOR)`, same line); its three asserts are pinned.
- `FABLE_RETAIL_CCOUNTEDPOINTER_CMESHDATABANK_RELEASEDATA_SIZE_BODY` / `..._CMESHDATABANK_RESET_SIZE_BODY` (CCountedPointer.h, the NF225 /
  NF234 size-body block; the block's `#if defined(...)` line gains the two terms): ReleaseData<CMeshDataBank> = 0x0042918C (Data-first body) and
  Reset<CMeshDataBank> = 0x0042FC86 (default body), explicit specializations under optimize("s", on). The /O2 dtor 0x00A0A150 still expands them.

Users: rows/00495620.cpp, 00496070.cpp, 00496cf0.cpp (CTOR_DTOR + SETQUIT; 0x00496070 also DEFAULTPROFILENAME; 0x00496CF0 also INLINE_CTOR_DTOR +
MAINPLAYERJOYSTICK), 00494370.cpp (SETQUIT), 00494350.cpp / 00494360.cpp (default path), 00a0a110.cpp / 00a0a150.cpp / 00a09d80.cpp
(CMESHBANKMANAGER_CTOR_DTOR; 0x00A0A150 also the two CMeshDataBank size bodies). Each row includes the overlaid headers itself, before any
header that includes them (NF213 lesson).

Proof (work/):
- pin_asserts.py rebuild/include hdr (six headers): pinned 4 (CGameJoystickManager.h) + 3 (CMeshBankManager.h, an earlier run), 0 elsewhere
  (work/pin.log); the pinned lines keep their old line numbers (`FABLE_STATIC_ASSERT_AT(<old line>, ...)`).
- assert_names_check.py rebuild/include hdr: mismatches 0 (work/names.log).
- equiv_check.py rebuild/include v work/t/include (= rebuild/include with these six headers overlaid; symlinks) over all landed rows: 0 of 57,124 landed
  rows differ (sources + tests; work/equiv.log, work/equiv.tsv empty).
- Landed includers (work/hdr_includers.txt): CFrontendGameComponent.h 8, CUserProfileManager.h 55, CMainGameComponentInit.h 3, CGameJoystickManager.h 5,
  CCountedPointer.h 8,475, CMeshBankManager.h 6 sources; none defines a new macro, so the opt-ins change none of them.
