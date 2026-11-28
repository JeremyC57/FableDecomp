# 0x00D05A60 / 0x00D06130 (CAssassin2 / CAssassin3 ::Main): facts for the ruling (AUTH30, 2026-11-27, round 2)

## 1. Is Main inline, and is it in the class?
- No PDB on disk has an IPI stream: Ego_d.pdb, Ego_r.4k.pdb and FableWin.4k.pdb all report "Has IDs: false" (llvm-pdbutil summary); the
  1 KB-block Ego_r.pdb / FableWin.pdb do not open ("Too many directory blocks"). So there are no LF_UDT_SRC_LINE / LF_UDT_MOD_SRC_LINE
  records, and the class's declaration line and extent cannot be read from type records. (types.txt / *.types.txt hold TPI only: 0 hits.)
- FableWin.4k.pdb module 11 (Debug\register_scripts.obj), read by piping llvm-pdbutil, nothing written:
  - S_FRAMEPROC flags "marked inline" on CAssassin2/3 ::Alloc, ::ctor, ::GetParentScript, ::Init, ::Main (and the parent script's
    Alloc / ctor / RegisterMain / Init / Main); NOT on register_scripts.cpp's own LoadDataFromMarker, ConsolePlayCutscene, RegisterScripts.
  - Line blocks, file q_banditcampbossbattle.hpp: CAssassin2 Alloc, ctor and GetParentScript all at line 987 (three functions on one line:
    a macro), Init 990-994, Main 997-1037; CAssassin3 1043 / 1046-1050 / 1053-1092. 5 lines separate CAssassin2::Main's last line from
    CAssassin3's boilerplate line.
- Ego_d (pass 29 package egod_lines.txt): Main file id 4944 lines 997-1037 / 1053-1092, 24 entries each (agrees with FableWin).
- Reading: Main (and every member) is INLINE in retail's source (two sources: FableWin's marked-inline flag; both PDBs' line records in the
  .hpp). In-class placement is what the line layout shows (boilerplate macro, Init, Main back to back; 5 lines to the next class's macro),
  but without UDT source-line records it is not proven by a type record. Note: I tried the pass 30 raw PDB line scanners
  (rebuild/candidates/manual/pending/2026-11-27_codex_pass30/tools/egod_proc.py, egod_lines.py) to read Ego_d's own frame flags; the session's
  permission check refused it, and I did not work around that.
- Shared header: rebuild/include/engine/CQ_BanditCampBossBattleScript.h declares CAssassin2 / CAssassin3 (overlay opt-ins
  FABLE_RETAIL_BANDIT_CAMP_ASSASSINS_PASS29 + TYPED_CTORS_PASS29). New add-only overlay opt-in FABLE_RETAIL_BANDIT_CAMP_ASSASSIN_INCLASS_MAIN_PASS30
  defines Main in each class body (the original `virtual void Main();` kept in #else) and pulls in the body's headers.
  - 00d05a60 g5 (row = g4 minus its out-of-class Main, + the opt-in): RELOCATION_MATCH 1400; pick-any identical to g4 (3 EQ / pk_all 5 EQ);
    test PASS with g5.obj; purity genuine; NAME_GATE PASS (NAME_GATE_g5.txt).
  - 00d06130 g2 (same, and the row's `#pragma inline_depth(2)` before Main dropped): RELOCATION_MATCH 1400 (/O2 /Oy); pick-any 2 EQ / pk_all 4 EQ;
    test PASS; purity genuine; NAME_GATE PASS. Compile note: the in-class body instantiates ~CCountedPointer<MovieBase>, so the row's
    out-of-line specialisation declaration moves above the includes (with fable_compiler.h + CCountedPointer.h first).
- How it would land: header opt-in as above; the row .cpp = the defines, the counted-pointer specialisations, the includes, the inline ctor
  / CIsThingAlive inlines, and a TRIGGER. An inline Main is emitted only if the TU odr-uses it, i.e. emits ??_7CAssassinN (the slot
  references Main), i.e. constructs the entity. Retail's trigger is register_scripts.cpp: RegisterScripts takes every entity's Alloc
  address, the in-class Alloc does `new CAssassinN(...)`, so register_scripts.obj emits the vftable and all inline members as COMDATs.
  A one-function row has no such reference, so the in-class route still needs (a) or (b) below.

## 2. Main out of line with the same-TU neighbours' flag
- Landed same-TU Main rows CAssassin1::Main 0x00D04AE0 and CBCGameMaster::Main 0x00D067A0 carry /d2inlS15 (build_candidates.ps1); the
  CAssassin2/3 Alloc / GetParentScript / Init / ??_G rows and the other BanditCampBossBattle Mains are base flags.
- 00d05a60 g6 (out-of-line Main, no helper, /d2inlS15): DIFFER(1365v1400), identical length to g1 without the flag.
- 00d06130 g3 (out-of-line Main, no helper, /O2 /Oy /d2inlS15): DIFFER(1448v1400), and pickany NEQ ??1CScriptGameResourceObjectMovieBase 25.

## 3. The 46-byte movie ??_G (0x006E7BD0, retail 87)
- Codex's third control is landed 0x00E70BC0 (CV_DemonDoorsScript::CDF_DoDamage::Main, base flags). Compiled here (head/00e70bc0.obj):
  RELOCATION_MATCH, ??_GCScriptGameResourceObjectMovieBase 87 EQ. Its state: no CCountedPointer<MovieBase> specialisation,
  FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR, its Main calls ??1CScriptGameResourceObjectMovieBase 0x006E7B80 (66 B EQ, the
  counted release expanded inside it).
- 00d05a60 g7 (g4 minus the row's MovieBase ReleaseData / ~CCountedPointer specialisations): RELOCATION_MATCH 1400, ??_G still 46.
  g8 (g7 + the PAUSEENTITIES_FORCEINLINE_CTOR opt-in): RELOCATION_MATCH 1400, ??_G still 46.
- Pattern across the four TUs: the ??_G is 87 where the TU expands ~CCountedPointer<MovieBase> inside ~CScriptGameResourceObjectMovieBase
  (0x00E70BC0) and 46 where it does not (HEAD 0x00EA2720 and 0x00E50FD0 emit ??1MovieBase as a 25-byte call to the counted dtor; ours
  expands ~MovieBase into Main with a call to the counted dtor 0x006E7AB0, which is what retail's leaf does). Codex's twinresdepth29_10
  got the 87 copy only by making all five cleanup paths in Main call a MovieBase dtor, against the retail leaf. I have no grade that gives
  both; the 46-byte copy equals the two HEAD copies (bytes, relocations, selection).

## 5. Alternatives for the trigger (no strong copy of a landed function in any of them)
- (a) Constructing helper (graded: g4 / final, and with the in-class route g5 / g2): an anonymous-namespace function that only does
  `new CAssassinN(...)`; scaffolding, not retail; RELOCATION_MATCH; the only strong symbol is the helper (no retail address).
- (b) Option S with the landed Alloc row (0x00D05910 / 0x00D05FE0): one unit holding Alloc (its owner) and Main, so Alloc's own
  `new CAssassinN(...)` emits the vftable and the inline Main with no scaffolding. Not graded: the landed Alloc rows model
  CQ_BanditCampBossBattleScript with a LOCAL class (pointer ParentClass, `virtual ~CAssassin2();`), so the unit needs that row ported onto
  the shared header first, and its own 145-byte bytes regraded in the unit.
- (c) In-class header route (graded above): fixes where `inline` lives, but still needs (a) or (b) as its trigger.
- (d) Retail's actual emitter is register_scripts.obj (RegisterScripts 0x00CD52D0 takes every Alloc address): the object-build-mode answer.
- Existing precedent in the landed tree (not changed by me): landed inline-Main rows 0x00D90290, 0x00E16E00 and 0x00CFCE90 define their
  entity's Alloc STRONGLY while those Allocs are landed as their own rows (0x00D901B0, 0x00E16D10, 0x00CFCDC0): existing LNK2005 pairs.

## Round 3: the coordinator's rulings R1 / R2 (2026-11-27)
R1 (a) line evidence: UNAVAILABLE. No PDB on disk carries LF_UDT_SRC_LINE / LF_UDT_MOD_SRC_LINE (Ego_d, Ego_r.4k, FableWin.4k: "Has IDs:
   false"; Ego_r.pdb / FableWin.pdb do not open), so the class head line cannot be found.
R1 (b) COMDAT evidence: VOID. FableWin.4k.pdb section contributions: CAssassin2::Main 0002:32526736 (2404 B) and Alloc are LNK_COMDAT, and
   so are the register_scripts.cpp controls LoadDataFromMarker 0002:31740480, ConsolePlayCutscene 0002:31741456 and
   CScriptInfoManager::RegisterScripts 0002:31741664 (44,738 B). In all of FableWin, 209,304 of 209,316 code contributions are COMDAT (/Gy debug build).
   Ego_d: llvm-pdbutil reads only 29 modules / 250 contributions of its DBI; among those, 63 of 64 code contributions are COMDAT
   (the other is the linker module). Ego_d's register_scripts contribution cannot be read with the tools I may run.
=> Per R1 the pair joins the object-mode class and stays escalated.
Same tests on landed 0x00E16E00 / 0x00E17320 (CWB_Guard1 / CWB_Guard2::Main): the same two gaps apply. Regraded in scratch, repo not edited:
   landed source RELOCATION_MATCH; without `inline` on Main (n1) DIFFER(1120v1064) for both (pick-any 12 EQ / pk_all 15 EQ). Files:
   00e16e00/, 00e17320/ (head.cpp, n1.cpp, LOG.md).
R2 (Option S with the landed Alloc row as owner): its premise is the inline Main, which R1 declines. The S-unit body set in address order is
   what g2 already graded: Alloc (owner 0x00D05910, 145 B, EQ to retail in c0's pk_all; 0x00D05FE0 likewise for CAssassin3), the
   constructor, GetParentScript, then Main out of line: DIFFER(1365v1400). With `inline` Main the same unit is c0: RELOCATION_MATCH 1400 with
   Alloc EQ at its address, so an S unit would land the pair only if inline were granted. Not built as files: the landed Alloc / GetParentScript
   / Init rows model CQ_BanditCampBossBattleScript locally (pointer ParentClass), so the owner needs a port onto the shared header first.
