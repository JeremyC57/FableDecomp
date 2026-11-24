# NF291 notes (2026-11-23): T1 standin_alias, continuation of NF287 / NF289 (the 110 entries NF289 did not reach)

## Summary (counts from STATUS.tsv / APPLIED.tsv)
- **All 110 ENTRIES reached.**
  - **done: 1 entry, 1 row changed** (rows.txt): 0x007022E0 IsVignette, through binder 0x00896A30 and one new add-only header opt-in.
  - **done-partial (table only): 1.** 0x0062FE60: a stale stand-in line is replaced by what the witness really binds.
  - **skipped-held: 1.** 0x007F5560: its only witness, 0x007F53A0, is in T3_PORT_QUEUE.tsv, so I did not compile it. Its TABLE_CHANGES line is CONDITIONAL.
  - **skipped-port: 107.** Every binder was opened: tri3 reads, the full table lines (tools/tbl.py, work/tbl.txt) and a source read of each stand-in. The reason for each is in its STATUS note.
  - No rejects. No vtable appeared or changed.
- **Recheck** (recheck_final.log, one vland `--recheck-landed` run with hdr/ as the overlay): 0x00896A30 RELOCATION_MATCH PASS, test PASS.
  - The SHARED_HEADER_AVAILABLE warnings are pre-existing (old local models).
  - The RELOC_NAME warning for IsQuestCardActive is pre-existing; it is NF289's entry 24, a T3 port.
  - The RELOC_NAME warning for `?IsVignette@CTCQuestCard@@QBE_NXZ -> 007022e0 labelled 'CTCShotDefinitionBase::CanBeHitByMeleeStrike'` comes from the self row's wrong catalog name (NAME_FIXES line).
- **One header opt-in:** hdr/engine/CTCQuestCard.h (hdr/README.md = HDR_README.md).
- No landed source or test `#include`s the changed row.

## Order (E2)
1. **Classification pass over all 110 entries.**
   - I took the full table lines from reloc_bindings.tsv (tools/tbl.py). The ENTRIES `spellings` column is cut at about 160 characters, so tri3 shows a long PDB spelling as an `S` stand-in (entries 41, 42, 49, 50, 73, 98 and 105).
   - The STATUS notes therefore name the real live binder per line.
2. **Stale-witness pass** (NF289's tri4 / objbind results, rechecked against the full table lines):
   - Entry 27's witness 0x00433670 was compiled at HEAD.
   - Entry 66's witness 0x007F53A0 was not compiled: it is T3-queued.
   - NF289's other "PDB already" binders (0x00431493, 0x004A3740, 0x008A8570, 0x0086E7C0) are the PDB lines' own witnesses, so there are no table changes there.
3. **In-place pass:** entry 1 only (below). No other binder has the stand-in on a class that derives a real header class whose header already declares the member, or can take an add-only declaration:
   - entry 2: a generated _quarantine header, and the parameter enum exists only in the engine header;
   - entry 34: the CWorldMap flat branch is GENERATED do-not-edit;
   - entry 50: both stand-in binders define same-TU stand-in bodies over local filter / vector types (NF284 D3).

## Done row (proof: evidence/proof_r1.txt, link_r1.txt, equiv_r1.txt, xsites_007022e0_0062fe60.tsv, pdbq.txt)
- **0x00896A30 CGameScriptInterface::SetQuestCardObjective**
  - Before: `struct QuestCardMethods:CTCQuestCard{bool IsVignette()const;}` derived the flat engine/CTCQuestCard.h class. The PDB member was declared only in that header's retail branch (`FABLE_RETAIL_CTCQUESTCARD_ISVIGNETTE`).
  - Header: new add-only opt-in `FABLE_RETAIL_CTCQUESTCARD_FLAT_ISVIGNETTE`, a declaration only, in the flat branch. Every assert in the header is already `_AT`.
  - Row:
    - the `#define` sits on line 3, ahead of every include, merged with the old "Retail00896A30,294 bytes." comment;
    - the stand-in struct is now empty;
    - the line count is unchanged (47).
  - Proof:
    - objsec is ALL_EQ under the one rename, `.drectve` included; 5 equal .text sections and no tables.
    - The exe call site is 0x00896AC4 -> 0x007022E0.
    - `?IsVignette@CTCQuestCard@@QBE_NXZ` is in both PDB name sets, and the table already binds it at 0x007022E0 (witness 0x004B1110).
  - equiv_check over the header's 1,407 includer rows: 0 differ. The planted-token control flags 10, which are the flat-branch users.
  - Link (with the self row's HEAD object):
    - The spelling is unresolved both before (the stand-in) and after (the PDB spelling), with 0 duplicates.
    - The self row 0x007022E0 defines only `?CanBeHitByMeleeStrike@CTCShotDefinitionBase@@UAEEXZ`, a generated descriptive virtual. That is an owner self port (T3) and has a NAME_FIXES line.
  - Census: no landed source defines CTCQuestCard::IsVignette out of line.
  - The other six exe callers (0x004B0F10, 0x004B1110, 0x004B1270, 0x004B1340, 0x005FF190, 0x008968C0) declare `bool IsVignette() const` on their own CTCQuestCard models. 0x005FF190 uses the header's retail-branch opt-in; the other five use local models.
  - The table holds no third spelling for 0x007022E0. Their objects were not compiled here, so the split gate's exe scan should confirm.
  - Test unchanged: it is a standalone copy with its own QuestCardMethods stub (NF284 D2). The precheck passes.

## Table-only entries
- **0x0062FE60 (entry 27): replace.**
  - The witness 0x00433670's HEAD object (RELOCATION_MATCH) references `??0CUIDef@@QAE@XZ`, a global `struct CUIDef` local model, and never `??0CClass_0062fe60@@QAE@XZ`. codegrep finds 0 code hits for CClass_0062fe60.
  - The respell would wrap the model in `namespace NUISystem`. That changes the row's own name `?CUIDef_Alloc@@YAPAUCUIDef@@XZ`, itself a stand-in for the PDB `?Alloc@CUIDef@NUISystem@@SIPAVCDefClassBase@@XZ`, so it is a binder self port (T3, NF282 D2).
- **0x007F5560 (entry 66): CONDITIONAL replace, from the source only.**
  - The witness 0x007F53A0's HEAD source declares a non-template `CThingBaseDef::GetSubDef(CDefPointer<const CSoundAtmospheresDef>*) const`, so it binds `?GetSubDef@CThingBaseDef@@QBE_NPAV?$CDefPointer@$$CBVCSoundAtmospheresDef@@@@@Z`. codegrep finds 0 hits for the CSubobject stand-in.
  - Not compiled: the row is T3-queued (NF288 entry 43).

## Identity doubts noted in STATUS (for the T3 unit)
- Entries 5, 18, 29, 33 and 61: the stand-in's shape or receiver contradicts the PDB name.
- Entries 58 and 59: the filter / event types differ from the PDB instance.
- Entries 74 and 80-84: the FableHeap family, whose element types are wrong per row.
- Entries 75 and 77: possibly folded dtor bodies.
- Entries 51 and 53-56: generated `Apply(void*, void*)` rows standing for a private template.

## Decisions / questions
1. **0x00896A30 is named in T3_PORT_QUEUE.tsv**, but only as the binder in NF289's entry 24 (address 0x004B0C80, its QuestManagerMethods members).
   - I changed only its IsVignette line, and left the QuestManagerMethods members alone.
   - Is editing a binder that a T3 line merely mentions acceptable, or should this wait for that port?
2. **Entry 66 is CONDITIONAL.** Please compile 0x007F53A0's HEAD object at landing, as for 0x00EE2DB0 under ruling Q1, before applying the replace.
   - I derived the replacement symbol by hand from the source declaration; take the exact spelling from that object.
3. **Entry 27: replace or delete?** I chose `replace` so the table keeps the split that 0x00433670 really binds. NF289 suggested `delete`. Which does the coordinator want?
4. **The do-not-edit marker is applied unevenly.**
   - CPlayerManager.h's flat branch carries the same "GENERATED ... do not edit by hand" line (line 65) as CTCBuyableHouse.h (line 137) and CWorldMap.h (line 492), yet NF289's flat opt-in there landed.
   - I followed the prefix ruling and left entry 34 (CWorldMap flat) as T3.
   - If a CPlayerManager-style add-only declaration is acceptable in a marked flat branch, entry 34 (binder 0x006E11F0, IsLoadingAnyMap) is a cheap next fix.

## Rule notes
- **Locks, prechecks and timeouts.**
  - Tests were prechecked before their locks were claimed (work/pre_tests.log: clang PASS for 0x00896A30, 0x00433670 and 0x007022E0).
  - HEAD sources were prechecked after the locks and before the grade call (pre_srcH.log), and the new row likewise (pre_srcN.log).
  - Grades and links ran in the background, each with `timeout 900` on its own launch line. equiv_check ran under `timeout 3000`; it is not a grade.
- **No forbidden operations.** No sleep, no foreground waits, no rm (clean.py), no git, nothing written under the repo, no `__asm` / `naked` / `_emit`.
- **Locks still held:** 0x00896A30, the changed row. 0x00433670 and 0x007022E0 are released.
- **orig/:** orig/00896a30.cpp (HEAD row) and orig/hdr/engine/CTCQuestCard.h (header base).
- **Reading list.**
  - Read in full: the prefix, the standing brief, GOTCHAS.md (all of it), NF289's hand-back (NOTES, LESSONS, HDR_README, TABLE_CHANGES, RENAMES, NAME_FIXES, recheck log, objbind evidence), COORDINATION.md and the nf289 landing README.
  - Searched for my addresses rather than read line by line: NF289's STATUS / APPLIED.
  - Not read in full, for context budget: ASM_RULES (no asm work here), EXTRA_RULES.md, and the nf281-nf288 landing READMEs.
- **tools/:**
  - NF289's helpers, plus my own:
    - tbl.py: full table lines per entry;
    - mkstatus.py: the STATUS writer, holding the per-entry readings;
    - r1fix_nf291.py: the header and row edit;
    - eqrun.sh: the equiv_check launcher.
  - tools/e2hdr.py came with the folder. It edits CAIStateGroupBase.h for a row (0x0094A6D0) that is not in my ENTRIES, and I did not run it.
