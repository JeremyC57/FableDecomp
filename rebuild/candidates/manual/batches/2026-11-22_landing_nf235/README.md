# Landing nf235 (Account A, 2026-11-22): the CFrontendGameComponent and CMeshBankManager vtable families

Agent NF235. Hand-back: `pending/2026-11-22_nf235_handback/`. Entries q12656 and q12657 done; q12658 (CEngine ctor 0x00B260B0) blocked;
q12659 (0x00418DCA) not started.

- **Rows (9):** 0x00495620 ~CFrontendGameComponent, 0x00496070 ctor, 0x00496CF0 Run, slot rows 0x00494370 SetQuit, 0x00494350 /
  0x00494360 On{Pre,Post}DeviceReset; 0x00A0A110 / 0x00A0A150 CMeshBankManager ctor / dtor, 0x00A09D80 OnPostDeviceReset.
  Recheck on the landed tree (after Codex pass 27; `logs/recheck.log`): 3 MATCH + 6 RELOCATION_MATCH, every test PASS.
- **Names:** five self leaves onto their PDB spellings (Run UAE vtable slot 2, SetQuit / On*DeviceReset virtual); vtables
  `??_7CFrontendGameComponent@@6BCBaseClassNonCopyable@@@` / `@@6BCDeviceResetCallback@@@`, `??_7CMeshBankManager@@6BCDeviceResetCallback@@@`
  (all three PDBs + RTTI COLs); Input / Output private AAE; 0x013B7D58 binds `?GOldGameComponentToDelete@@3PAVCGameComponent@@A`.
- **Headers (add-only opt-ins):** CFrontendGameComponent.h `_SETQUIT` / `_CTOR_DTOR`, CUserProfileManager.h `_DEFAULTPROFILENAME`,
  CMainGameComponentInit.h `_INLINE_CTOR_DTOR`, CGameJoystickManager.h `_MAINPLAYERJOYSTICK`, CMeshBankManager.h `_CTOR_DTOR`,
  CCountedPointer.h `_CMESHDATABANK_RELEASEDATA_SIZE_BODY` / `_RESET_SIZE_BODY`. Base headers unchanged since the brief (also after
  pass 27). Proof: the agent's pin_asserts (7) + assert_names_check (0) + equivalence over 57,124 rows (0 differ, `logs/equiv.tsv`);
  re-run here over the 100 pass-27 rows and the 9 rows after pass 27: 0 differ.
- **reloc_bindings:** 9 stand-in lines deleted, 6 added (TABLE_CHANGES.tsv); spelling census `logs/census_*.txt`: no address grows.
- **Pick-any / link (agent):** no new NEQ copy (four ~CCountedPointer copies identical at HEAD; SetMode's copy differs only in the
  L"" literal); 0 LNK2005, +2 LNK4006 (LoadProfile / SetMode inline in 0x00496070 beside owners 0x00494410 / 0x00495FE0).

## Coordinator rulings on NF235's questions
1. LoadProfile / SetMode inline in the ctor (+2 LNK4006, copies equal the owners): accepted, the pattern 0x004963A0 / 0x00496CF0 land.
2. 0x013B7D58: respell the three other binders (0x004189C2, 0x00412F90, 0x0042EC7C) to GOldGameComponentToDelete next round (queued).
3. 0x00B260B0: queued as one unit with the dtor 0x00B261D0 (CEngine.h override opt-in in PDB spelling).
4. 0x004065D0 -> GetDefaultProfileName: queued for the next agent to prove (both names in both PDBs; body L"Default"; both binders).
5. 0x00496CF0 keeps its local CSystemManager model (Win32 types): accepted for now, queued with the header tests.
6. The three changed existing lines (`#ifndef` -> two-term `#if !defined`, a `#if defined` gaining two terms, 7 asserts pinned):
   accepted as add-only, default path preprocesses identically (equivalence 0).
