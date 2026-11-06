# chunk 813 header overlay

Three headers. One is a repo header with an additive opt-in edit, and two are new headers.

For the repo header, `diff rebuild/include/engine/CQ_NewOakValeIntroScript.h hdr/engine/CQ_NewOakValeIntroScript.h` shows only `>`
lines, so the default path is textually unchanged. I re-checked the diff against the repo copy just before hand-back.

The four landed includers of CQ_NewOakValeIntroScript.h were rechecked read-only with this overlay, using `vland.py
--recheck-landed` on a payload built from their landed sources and tests. All four are RELOCATION_MATCH + PASS (logs/recheck_vland.log;
also listed in SIBLINGS.tsv): 00DB09E0, 00DB2B10, 00DB6C60, 00DBCD60.

The two new headers have no landed includer.

## engine/CQ_NewOakValeIntroScript.h (repo header, opt-in additions)
The opt-in is `FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_BARRELMAN_THERESA`. It adds:
- A forward declaration `class CNOVI_BarrelMan;` inside CQ_NewOakValeIntroScript, next to the existing nested-class
  declarations.
- The nested `CNOVI_BarrelMan` class: 36 bytes from Ego_r.pdb.
  - Members: ParentClass +0x14, PMasterData +0x18, ComplainedAboutStock +0x1c, HeroLetMeDown +0x1d, OverheardYet +0x1e,
    EMyPhase MyPhase +0x20.
  - EMyPhase is declared empty because its enumerator names are not in the PDB name set; callers cast.
  - Vtable 0x012D9014: slot 0 dtor 0x00DB5310, Main 0x00DB5330, Init 0x00DB5260, GetParentScript 0x00DB5250.
  - Alloc 0x00DB51B0.
- The nested `CNOVI_Theresa` class: 32 bytes.
  - Members: ParentClass +0x14, PMasterData +0x18, spoken_already +0x1c, DoneIntro +0x1d.
  - Vtable 0x012D83A4: slot 0 0x00DAC4D0, Main 0x00DB97A0, Init 0x00DAC4F0, GetParentScript 0x00DAC4C0.
  - Alloc 0x00DAC420.
- Evidence:
  - The Ego_r.pdb layouts and the Ego_r.pdb / FableWin.pdb Main names (manglings in the header comment).
  - The vtables stored by the two Allocs.
  - The member offsets the two Mains read.
- Used by the parked rows 0x00DB5330 and 0x00DB97A0. The opt-in name is distinct from the header's existing opt-ins (`_ENTITIES`,
  `_GUARD`, `_VICTIM_LIVEFATHER`).

## engine/CV_BeggarAndChildScript.h (new)
- Sits behind `#ifdef FABLE_RETAIL_NSCRIPT`, with no `#error` branch.
- The owner class `NScript::CV_BeggarAndChildScript : CLevelScriptBase<CQ_SunnyvaleMasterData>`:
  - Virtuals: RegisterMain 0x00E57C60, Main 0x00E57E60, Init 0x00E57C00, OnPersist 0x00E57D20 (all landed).
  - Fields at Ego_r.pdb offset - 0x14 (sizeof 0x64): BeggarHit 0x48 .. BeggarReplyTimer 0x60.
  - 0x00E5B3B0 reads every field it uses at these offsets (all but TaughtSneer / TaughtThanks / TaughtBattleCry).
- The nested `CBeggarBully : CEntityScriptBase` (28 bytes):
  - Members: ParentClass +0x14, PMasterData +0x18.
  - Vtable 0x012E6C1C: dtor 0x00E5B390, Main 0x00E5B3B0, Init 0x00E5B380, GetParentScript 0x00E5B370.
  - Alloc 0x00E5B2D0.
- Used by the parked row 0x00E5B3B0.

## engine/CV_FisticuffsClubScript.h (new)
- Sits behind `#ifdef FABLE_RETAIL_NSCRIPT`.
- Includes the existing engine/NScript_CV_FisticuffsClubScript.h (the owner class, unchanged), which already forward-declares
  the nested entity.
- Defines `CV_FisticuffsClubScript::CFisticuffsMember : CEntityScriptBase` (48 bytes):
  - Members: ParentClass +0x14, PMasterData +0x18, PerformingCheerOrBoo +0x1C, FC_Status +0x20, ShoutTimer +0x24,
    CheerName +0x28, BooName +0x2C.
  - Vtable 0x012EB0F8, stored by the ctor 0x00E83920: dtor 0x00E839D0, Main 0x00E84B80, Init 0x00E83A20, GetParentScript
    0x00E83980, OnPredicateFail 0x00E83990.
- Used by the parked row 0x00E84B80.
