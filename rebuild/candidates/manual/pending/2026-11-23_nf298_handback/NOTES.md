# NF298 notes (2026-11-23): T1 "other" class / namespace / form, continuation of NF294 / NF295, hand-back 1

Base: the repo working tree as of today. I never wrote to it and ran no git.

Numbering: my "entry n" is the n-th data line of ENTRIES.tsv (file line n+1).

## Summary (counts from STATUS.tsv / APPLIED.tsv)
- **70 entries reached:** entries 1-60 in order, plus 10 pulled ahead by a stale-witness scan over all 535 entries (ruling E2; the order
  is recorded in each STATUS note): 64, 69, 92, 93, 122, 241, 242, 245, 246, 426.
- **Status counts:** done 9, done-already 14, done-conditional 1 (entry 93), done-partial 3, done-partial (conditional) 1 (entry 64),
  skipped-port 21, skipped-held 21. No rejects. No vtable change. No header change.
- **8 rows changed** (rows.txt). All are zero line delta and in place. Each is RELOCATION_MATCH at HEAD and after the change.
  - objsec HEAD v new is ALL_EQ for all 8 under an explicit rename map built from ENTRIES (tools/proofx.py). The map covers exactly
    each object's DEF/UND symbol diff, so both function COMDAT sets are the same. Every non-debug section is equal, `.drectve` and the
    emitted vtables included (evidence/proof_b1.txt).
  - The vland `--recheck-landed` run on the new sources gives 8/8 RELOCATION_MATCH, test PASS (recheck_final.log). Its warnings
    (SHARED_HEADER_AVAILABLE, one RELOC_NAME on a callee label in 0x00DBF2D0) are pre-existing.
- **Table:** 37 changes (TABLE_CHANGES.tsv): 26 deletes, 2 `delete-conditional` (entries 64 and 93: apply only if the gate scan finds no
  binder of that spelling), 9 adds.

## Families
- **NScript nested entity-script classes (entries 2, 3, 20-26: done).** Each quest Main declares its entity-script allocators as nested
  structs inside the (already NScript, already class-keyed) script class. Nine of them dropped the PDB underscore or used another name:
  - CHSBMagicBarrier -> CHSB_MagicBarrier (0x00D80E40);
  - CHSScreamer -> CHS_Screamer (0x00D835D0);
  - CMCMinion / CMCMinionGuard / CMCSpawnPoint -> CMC_Minion / CMC_MinionGuard / CMC_SpawnPoint (0x00D9E5E0);
  - CWhisperMarker -> CMK_OFI_GWLL_WHIS2 and CWhisperFarmRaidIntro -> CM_WhisperFarmRaidIntro (0x00DCC770, through its ALLOC macro);
  - CBodyGuardRespawn -> CM_BodyGuardRespawn (0x00E5D4F0);
  - CRestartMarker -> CM_RestartMarker (0x00DBF2D0).

  The rename touches the struct declaration and the `&X::Alloc` use, in code only (comments untouched).
  - **Two sources per name:** each PDB spelling is in both name sets (evidence/pdbq_nscript.txt). The retail binder passes the binding
    string literal that is the PDB class name without its C (e.g. "MK_OFI_GWLL_WHIS2", "M_RestartMarker"). The self row defines
    the PDB spelling (its Leaf; HEAD object compiled).
  - **Census:** each Alloc address has exactly one imm32 code reference in Fable.exe, inside its Main (evidence/absrefs_nf298.tsv;
    0x00DC0090 also has one data word at 0x0144E6FC, outside any row). There are no E8/E9 callers. The old identifiers appear in no
    other landed source except one test: 0x00E5D4F0's standalone test still models `CBodyGuardRespawn` with a stub Alloc (NF284 D2:
    stays). No landed source or test includes any of the 8 changed rows by path (grep of the 8 basenames over rebuild/src and rebuild/tests: 0).
  - **New-name census (the PDB spellings the binders now reference).** Besides the 9 self rows, 17 landed family rows already define the same
    Alloc under the PDB name as a strong same-TU copy of the owner's body (`return CCountedPointer<CEntityScriptBase>(new X(parent, data, me));`,
    the same text as the owner): 0x00D83F00, 0x00D81C30, 0x00D81C00, 0x00D84470, 0x00D9F150, 0x00D9FFA0, 0x00D9F250, 0x00D9EE00, 0x00D9EDF0,
    0x00D9EB20, 0x00DCF6C0, 0x00DCF5F0, 0x00DCF6B0, 0x00DCF5E0, 0x00DC0150, 0x00E5E0A0, 0x00E5E090 (GetParentScript / scalar-dtor / Init rows
    that carry the Alloc row's class model). They exist at HEAD; my change adds no definition, only references to a name they already define
    with the owner's body. Recorded as debt (decision 4). The other files the identifier grep lists (0x00EF5CB0, 0x00E292C0, 0x00EF34A0 + its
    test, 0x00EFC500, 0x00D9F1B0, 0x00D9F070) use the class names but define no Alloc. No landed row defines a `CHeightMap::Clear` body other
    than its owner 0x008F6050.
  - **Link (evidence/link_A.txt; HEAD binders + owners v new binders + owners):** HEAD leaves the 9 stand-in Allocs unresolved; the new set
    resolves all 9 against the owners. 0 duplicates. The `CBodyGuard` Alloc is unresolved on both sides (its owner was not in the set;
    not touched).
  - Not done: entry 111 (0x00D890A0, CSummonedCreature -> CSUMMONED_CREATURE): binder 0x00D87CB0 is a witness in NF296/NF297 ENTRIES
    (not reached in order anyway).
- **Stale witnesses (entries 40-46, 245, 246, and 64, 69, 92, 93, 122, 241, 242, 426).**
  - The scan: the stand-in line and the PDB line share a witness. I compiled each free witness's HEAD object and read it with objbind.py.
    Every one binds only the PDB spelling: 0x00EA8830 x9, 0x00AEABE0, 0x00AEE8C0, 0x00D6ED10, 0x00D73610, 0x00B218B0, 0x00E8B9C0 x2,
    0x00ADF610 (evidence/objbind_00ea8830.txt, objbind_st2.txt).
  - Callers (xsites / absrefs): the witness is the sole caller for 69, 92, 122, 241, 242 and the 9 MurderTwist targets: plain delete.
  - For the targets with more callers I compiled every free one (evidence/objbind_st3.txt):
    - **426:** 0x00AE1BC0 binds the PDB spelling, and 0x00AE1030 binds the other stand-in (its own line). Every caller is read, so the
      delete is plain. done-partial (the other stand-in stays).
    - **93:** 0x00D73AE0 binds the PDB spelling. 0x00D73970 / 0x00D73C70 fail their own clang precheck (`deleted function '~CHTG_..'
      cannot override`, own lines), so they were not compiled and their locks were released. `delete-conditional`.
    - **64:** 0x00AEB8C0 binds the other (J_N) stand-in. 0x00AEB6D0 / 0x00AEB790 are NF296/NF297 witnesses, not compiled.
      `delete-conditional`; the J_N stand-in stays (port).
  - Not compiled: witness 0x00DD1AC0 (entry 96) fails its own clang precheck (`deleted function '~CM_WhisperFarmRaidIntro' cannot override
    a non-deleted function`, own line, not toolchain). Its lock was released uncompiled. The three anonymous-namespace candidates (369, 372,
    378) were left for the anon-ns pass.
- **Singles.**
  - **Entry 4 (0x008F6050 CHeightMap::Clear):** 0x008F63C0's local model now has `private: void Clear();` (PDB AAE, both name sets). Its
    other binder 0x0081E7E0 (CMap::Clear) stays: it uses the stand-in struct `CGameMapGrid`, and the shared engine/CHeightMap.h exists (private
    Clear, `friend class CMap` only under opt-ins), so the fix is a port. The other callers 0x00820CC0 / 0x00821A80 already bind AAE.
  - **Entry 59 (0x00AEDCA0 GetNoParticles):** 0x00AEAC90 declares it const (PDB QBE, both name sets).
    - Its landed test fails clang only inside VC7.1 iostream (`<cwchar>`, ostream, istream), so a VC7.1 `cl /Zs` (background, timeout
      900, after its lock) passed instead, per ruling D1 (evidence/zs_t00aeac90.log).
    - QBE is unresolved in the new link and QAE was unresolved at HEAD, so nothing HEAD resolved is lost. The owner self row defines
      the global stand-in (NAME_FIXES).
    - Left: 0x00B0C830. Its global CParticleEmitter is the template argument of the row's CIntelligentPointer, so this is a port.

## Ports and held entries (STATUS notes give the reason per entry)
- **Held (21):** the only binder is a witness in NF296 / NF297 ENTRIES, or is off-limits. Entries 7 / 8 are a likely stale pair:
  0x00851860 already calls `NUISystem::CIME::Get()` and the `CIMECalls` token is absent, but that binder is an NF296/NF297 witness.
- **Port shapes (21):**
  - a member bound on a derived local class where the PDB member sits on the base (1, 10, 15);
  - free `extern "C"` / `@name@N` stand-ins (5, 16);
  - a stand-in element type of an STL-instance row, or a class used as a template argument, where renaming changes COMDAT names
    (11, 14, 32, 59/60's 0x00B0C830 / 0x00B0C8A0);
  - nesting a polymorphic base of the binder's own class (37, 39);
  - a parameter-type, enum or virtual difference (17, 38, 48, 49, 51);
  - the binder's own self leaf on the stand-in class (29, 50, 52, 54, 56: NF282 D2, T3).

## Decisions / questions
1. **Header debt (kept out of NAME_FIXES, which takes addresses only).** engine/CQ_MinionCampScript.h's default branch forward-declares `CMCMinionGuard` / `CMCSpawnPoint` /
   `CMCMinion` without the PDB underscore. These lines emit no symbol and binder 0x00D9E5E0 does not include the header, so I left it.
   Should the header's names be corrected under an opt-in later, or leave it as debt?
2. **Class key of the nested allocator structs.** The Main binders keep `struct` for the nested allocator classes, as the other allocators
   in the same rows do. The key does not appear in any emitted symbol (static member of a nested class; the parent is `AAV23@`). I did
   not change it, to keep the change minimal. Should a later pass make them `class` to match the PDB key?
3. **The two conditional deletes (entries 64, 93).** Their unread callers could not be compiled: two are NF296/NF297 witnesses, and two fail
   their own clang precheck. Apply each delete only if the landing's gate scan finds no binder of the spelling, or hold the line?
4. **17 same-TU strong Alloc copies** in the entity-script family rows (list above). Each repeats the owner's body under the PDB name, so a
   full link already gives LNK2005 against the owner at HEAD. They predate this batch, and my respell only adds references. Queue them for
   the nf86 / nf88 pick-any scaffolding or a declaration-only class model (T3)?

## Rule notes
- **Locks.** Every compiled row was locked before compiling, and every landed test was prechecked before its lock (pre_tests1/2).
  - Compiled rows: the 8 changed rows; the 11 owners (the 9 Allocs, 0x00AEDCA0, 0x008F6050); the stale witness 0x00EA8830; and the
    7 stale witnesses in set2.
  - Each source was prechecked after its lock and before its grade (pre_srcH / srcH2 / srcN).
  - 0x00DD1AC0, 0x00D73970 and 0x00D73C70 failed their own source precheck and were released uncompiled.
  - Also compiled: the callers 0x00AEB8C0, 0x00D73AE0 and 0x00AE1BC0 (set3: tests prechecked before the locks, sources after).
  - 0x00AEAC90: its landed test failed clang (VC7.1 iostream only) before I claimed the lock. I claimed the lock, then ran the VC7.1
    `cl /Zs` on the test in the background. It passed before any grade of that row.
  - All locks are released except the 8 changed rows.
- **Background runs.** Every grade, link, `cl /Zs` and vland run went to the background with `timeout 900` on its own launch line
  (gr.sh, lk.sh, zsrun.sh, rf.sh), and was checked with `kill -0` in a later call.
- **Not done:** no sleep, no `rm` (clean.py only), no git, nothing written under the repo, no `__asm` / `naked` / `_emit`.
- **Retail bytes:** none in hand-back files (checked by a hex-run grep). The vland oracle stays in work/pay1.
- **orig/** holds the HEAD copy of every changed row, all still equal to the repo at hand-back. No header was changed, so hdr/ is empty.
- **Tests:** unchanged. No landed source or test includes a changed row by path.
- **Reading list.**
  - Read in full: the prefix, the standing brief, all of GOTCHAS.md, EXTRA_RULES.md, ASM_RULES.md, COORDINATION.md, NF295's NOTES /
    LESSONS / HDR_README / APPLIED / RENAMES / TABLE_CHANGES / NAME_FIXES / REJECTED, and the landing READMEs of nf281_282, nf283, nf284,
    nf285 and nf295.
  - NF295's STATUS.tsv: read cut to 300 characters per line, except entries 3, 4, 54 and 62, which I read in full.
  - The standing brief's `pending/2026-11-22_nf28x_handback/` does not exist (per-agent nf28N_handback folders do; not read).
- **tools/:** NF295's set (paths set to this folder), plus:
  - proofx.py: objsec under an explicit rename map from ENTRIES, checking that the map covers the symbol diff. It replaces proof.sh's
    sorted pairing, which mispairs multi-rename rows such as 0x00DCC770.
  - mkstatus298.py + verdicts298.py: the STATUS generator and its verdict table.
