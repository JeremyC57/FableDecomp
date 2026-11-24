# NF294 notes (2026-11-23): T1 "other", class / namespace / form splits, hand-back 1 (entries 1-60)

Base: the repo working tree as of today. I never wrote to it and ran no git. Numbering is my ENTRIES numbering (data lines, header excluded).

## Summary (counts from STATUS.tsv / APPLIED.tsv)
- **60 of 655 entries reached**: entries 1-60, in order (all of them "class/ns" kind). Entries 61-655 are `open`.
- **5 rows changed** (rows.txt). All are RELOCATION_MATCH at HEAD and after the change, objsec ALL_EQ (`.drectve` and tables included),
  and vland `--recheck-landed` gives 5/5 RELOCATION_MATCH, test PASS (recheck_final.log).
  - **done 2** (+1 done-already, table only):
    - entry 2 0x00997780 CAFile::MakePath: binder 0x00997800, through one new add-only header opt-in;
    - entry 20 0x00D81750 CBRIAR_ROSE_IMPOSTER::Alloc: binder 0x00D80E40.
  - **done-partial 3**:
    - entry 11 0x00717950 CActionTalkBase ctor: binder 0x00717BC0;
    - entry 23 0x009CC190 CBankFile::EntryExists: binder 0x00A5EEA0;
    - entry 25 0x009CCB20 CBankFile::WritePadding: binder 0x009CE8D0.
  - **Table only, 2**, from stale witnesses read off HEAD objects (evidence/objbind_stale.tsv):
    - entry 50 0x006ED9C0 (done-already): witness 0x00733580; the 16 other landed exe callers were source-checked (all real-name CCombatManager / header);
    - entry 34 0x009A0460 (done-partial): witness 0x0099F3B0.
- **skipped-port 49, skipped-held 4.** Their only stand-in binders are off-limits: entry 15's 0x00A269C0 is in T3_PORT_QUEUE.tsv, and
  entries 19 / 52 / 53's 0x009E5C90 / 0x006E93C0 / 0x006E9680 are NF292 ENTRIES targets. See below.
- No rejects. No vtable appeared or changed. No decision is held back on a row.

## Method (order E2: classification pass first, then the in-place set)
1. **Classification read.** tools/cls.py runs per entry over the full reloc_bindings lines (not the truncated ENTRIES column). For each
   stand-in line it prints the witness's source, an OFFLIMITS mark, the lines naming the leaf, the stand-in scope's struct / class /
   namespace definition and the engine includes. It also checks whether `engine/<class>.h` exists and which `#ifdef` holds the leaf.
   - tools/tri4.py found the stale candidates 0x0099F3B0 (entry 34) and 0x00733580 (entry 50, ANYMISSING). Both HEAD objects were compiled and read.
2. **Off-limits.** work/offlimits.txt (1,677 addresses) holds NF292's and NF293's ENTRIES, every lock held by others, RESERVED, T3_PORT_QUEUE
   and the held rows. Every binder I opened was checked against it. Binders that sit in NF292 / NF293 ENTRIES as *witnesses* were not edited.
   - 0x0094FEB0 (entry 5) is one of these. Its line 110 also declares Terminate, which is NF292's entry 33.
3. **In-place set.** Only rows matching one of three shapes were edited, each with zero line delta:
   - an existing opt-in in a header the row already includes;
   - a real-name local class that takes the declaration;
   - a pure rename where every changed symbol is PDB-exact.
   orig/ holds the HEAD copies. tools/fix294.py holds every edit.

## Changed rows (proof: evidence/proof_r1.txt, proof_r1b.txt, proof_r5.txt; links evidence/link_r1..r5.txt)
- **0x00997800 ReplacePathExtension** (entry 2)
  - Change: the free `CWideString __fastcall MakePath(...)` declaration is dropped. The row defines the new add-only
    `FABLE_RETAIL_CAFILE_MAKEPATH` and calls `CAFile::MakePath`.
  - Mangling: YI (free fastcall) -> SI (static public fastcall), so the bytes are the same.
  - First try: defining the existing `FABLE_RETAIL_CAFILE` + `_CREATEDIRECTORY`. Its .text was equal, but `.drectve` gained
    `/DEFAULTLIB:"libcp"`, because the branch includes engine/CArray.h. That try was rejected, which led to the opt-in.
  - Proof: objsec ALL_EQ (proof_r1b).
  - Link: HEAD leaves `?MakePath@@YI...` unresolved; the new set resolves against the owner 0x00997780.
  - Other exe callers already bind CAFile::MakePath: 0x004FAFF0, 0x004FB0A0, 0x00999620 and 0x00999AB0. 0x009BD4EB is not landed.
- **0x00D80E40 CQ_HeroSoulsBriarScript::Main** (entry 20)
  - Change: the nested allocator struct `CBriarRoseImposter` is renamed `CBRIAR_ROSE_IMPOSTER`, on line 76 and the `&...::Alloc` on line 89.
  - Names: Ego_r and FableWin both have `?Alloc@CBRIAR_ROSE_IMPOSTER@CQ_HeroSoulsBriarScript@NScript@@...`, and the stand-in name is in neither.
    The second source is the self row 0x00D81750, whose Leaf defines it; the script binding string "BRIAR_ROSE_IMPOSTER" is on the same line.
  - The struct has no other symbol.
  - Proof: objsec ALL_EQ (26 sections; table equal).
  - Link: HEAD leaves the stand-in unresolved; the new set resolves it.
  - The remaining unresolved CBRIAR_ROSE_IMPOSTER symbols in the link come from the owner object and are the same before and after.
- **0x00A5EEA0 CStreamingFontDataBank::AdjustUpdateEntries** (entry 23)
  - Change: the row defines the existing `FABLE_RETAIL_CBANKFILE_ENTRYQUERY` and drops `EntryExists` from its `Methods : CBankFile` stand-in.
  - Proof: objsec ALL_EQ.
  - Link: `?EntryExists@CBankFile@@QBE_NK@Z` stays unresolved against the owner 0x009CC190, whose object defines only
    `?EntryExists@CBankFile_Retail@@QBE_NK@Z`. The existing PDB binder 0x005A29F4 is in the same state: owner self port, see NAME_FIXES.
- **0x009CE8D0 CBankFile::BeginUpdateEntries** (entry 25)
  - Change: in its real-name local `class CBankFile`, `void WriteZeros(unsigned long)` becomes
    `private: void WritePadding(unsigned long); public:`. The line-102 call is respelled to match.
  - Names: Ego_r + FableWin `?WritePadding@CBankFile@@AAEXK@Z`, private. The self row 0x009CCB20's Leaf comment cites the same spelling,
    and the row already uses the `private: ... WriteFileInformation` pattern.
  - Proof: objsec ALL_EQ (table equal).
  - Link: WritePadding is unresolved against the owner, which defines only `CBankFile_Retail::WritePadding`, the same as for the existing
    PDB binder 0x009CE370 (NAME_FIXES).
- **0x00717BC0 CActionOverheadTalkToThing ctor** (entry 11)
  - Change: in its `NTCScriptedControl::CActionTalkBase` model the base ctor becomes `protected: CActionTalkBase(CThing&, bool, bool, bool); public:`.
    The mem-init passes `*target`.
  - Names: Ego_r + FableWin `??0CActionTalkBase@NTCScriptedControl@@IAE@AAVCThing@@_N11@Z`.
  - This is a pointer -> reference change with the same pushed value: an R1 kind (pointer <-> reference + access).
  - Proof: objsec ALL_EQ (table equal).
  - Link: HEAD leaves the QAE pointer spelling unresolved; the new set resolves against the owner 0x00717950.
- **Name census.** Each new callee spelling is defined out of line only by its own self row. grep shows only calls and comments elsewhere.
- **Tests:** unchanged. All five are standalone models (D2) and pass the precheck.
- **Changed-row includes:** no landed source or test `#include`s a changed row.

## Table-only entries
- **Entry 50 (0x006ED9C0).** 0x00733580's HEAD object binds `?DeregisterCombatant@CCombatManager@@QAEXAAVCThingCreatureBase@@@Z`, never the
  CCombatManagerCalls stand-in, so the stale line is deleted. The PDB line already exists (witness 0x007355F0). No other stand-in line exists, and the 16 other landed exe callers spell the PDB form (source grep), so the entry is done-already.
- **Entry 34 (0x009A0460).** 0x0099F3B0's HEAD object binds `?Append@?$CBasicString@D@@QAEXDK@Z`, never ResizeMap@CThingMap: `replace`.
  - That spelling is in FableWin only (Ego_r has no such line). The second source is the self row's own Leaf and the retail call 0x0099F3C4 -> 0x009A0460.
  - 0x0099F180's CBasicStringChar stand-in stays, as a port.

## Ports worth a later unit (STATUS notes)
- **Flat add-only opt-ins** (generated flat branch of engine/CAIStateGroupBase.h; check the do-not-edit marker first):
  - entry 5, 0x0093C4E0 IsStopping;
  - entry 6, 0x0094A6D0 AIAction_MoveToPos (pointer -> reference too).
- **Entry 5, 0x0094FEB0:** a one-line move onto its local `class CAIStateGroupBase`. I left it because NF292's entry 33 (Terminate) edits the same line.
- **Entry 9, 0x00634470:** re-port onto its landed siblings' shape (FABLE_RETAIL_PLAYER_INPUTPROCESSES + the per-process headers).
- **Entry 13:** 0x008E9190 / 0x00752640 onto the shared engine/NAnimationEntryBodge_CAnimEntryLite.h.
- **Self ports (T3):**
  - entry 21: the binder's own class CBS_Teacher is global;
  - entry 51: CCombatWheel::ResetRings;
  - entry 55: the self row 0x005303F0's global CComponent.
- **`virtual` with a table effect on a local model (T3 form):** entry 27 (0x00AF15B0) and entry 49 (0x00A98AF0).
- **Identity doubts:**
  - entry 16: 0x00A26290 "OP::h1", whose catalog name is CThingFilter_IsInBuilding::operator();
  - entry 40: a CChunkedFileChunk ctor calls the CCardPositionsDef dtor;
  - entry 47: a __stdcall helper stands for a thiscall member.

## Decisions / questions
1. **New opt-in `FABLE_RETAIL_CAFILE_MAKEPATH`.** It duplicates a declaration that already exists under CREATEDIRECTORY (nested in
   FABLE_RETAIL_CAFILE), guarded so the two never meet. The alternative, making the row define FABLE_RETAIL_CAFILE, changes `.drectve`.
   Is the duplicate-under-guard form acceptable (GOTCHAS "guard the second declaration")?
2. **Entry 5 / 0x0094FEB0 overlaps NF292's entry 33 (same source line).** Should NF292 fold IsStopping into its edit, or should I take the
   row after NF292 lands?
3. **Entry 34's PDB spelling is in FableWin only.** Is the self row's Leaf plus the retail call enough as the second source for a table `replace`?

## Rule notes
- **Locks, prechecks and timeouts.**
  - Every landed test was prechecked before its lock (work/pre_tests.log, pre_tests2.log, pre_towner.log).
  - Every HEAD and new source was prechecked after its lock and before its grade call (pre_srcH / srcN / srcN2 / srcOwner).
  - Every grade and link ran in the background with `timeout 900` on its own launch line (gr.sh, lk.sh, rf.sh). equiv_check ran under
    `timeout 3000`; it is not a grade.
- **Brief locks on NF292 witnesses:** 0x00733580 and 0x0099F3B0 are witnesses in NF292's ENTRIES. I locked each for one HEAD-only
  compile (no edit) and then released it, so NF292 may have seen HELD for a few minutes.
- **orig/:** holds only the 5 changed rows and orig/hdr/engine/CAFile.h. The two control copies were removed with clean.py.
- **Staleness:** at hand-back, every orig/ file still equals its repo file (cmp).
- **Repo writes:** `find /home/user/FableDecomp -newer BRIEF.md -type f` (excluding .git) found nothing.
- **Locks still held:** the 5 changed rows (00997800, 00d80e40, 00a5eea0, 009ce8d0, 00717bc0). Controls and owners are released
  (00733580, 0099f3b0, 00997780, 00d81750, 009cc190, 009ccb20, 00717950).
- **No forbidden operations.** No sleep, no foreground waits, no rm (clean.py), no git, nothing written under the repo (find check above), no `__asm` / `naked` / `_emit`.
  No retail or donor bytes are in hand-back files (oracles stay in work/).
- **Tool fixes** (tools/flags.sh, tools/srcof.sh). Both read `grep -A14` after the catalog Address line, which can run into the NEXT entry:
  - flags.sh gave 0x00997800 the `/EHsc` of 0x00CAF500, and its first grade went DIFFER(207v122) at HEAD too;
  - both now read only the row's own block. Earlier agents' grades of rows without CompilerFlags may have used a neighbour's flags.
- **Reading list.**
  - Read in full: the prefix, the standing brief, all of GOTCHAS.md, and NF289 / NF290 / NF291 NOTES + LESSONS.
  - Read the heads and formats only: NF291's STATUS / TABLE_CHANGES / RENAMES / NAME_FIXES / HDR_README.
  - EXTRA_RULES.md: only the naming sections (2026-10-28 names, 2026-11-05 stand-in names, quarantine, anonymous-namespace hashes).
  - Not read: ASM_RULES and COORDINATION (no asm or branch work here), and the nf28x landing READMEs.
- **tools/:**
  - NF291's set, plus cls.py (classification read), fix294.py (all edits), mkstatus294.py (STATUS writer with the per-entry readings),
    and the fixed flags.sh / srcof.sh.
  - Inherited scripts I did not run: e1fix / e2hdr / r1fix* / r4fix / u1fix / u2fix / t3notes_1_60 / mkstatus.py.
