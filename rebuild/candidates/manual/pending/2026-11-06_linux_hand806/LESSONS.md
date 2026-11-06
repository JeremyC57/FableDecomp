# Chunk 806 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- In the EH-less quest-script TUs, VC7.1 expands an inline dtor at the natural end of a scope (including the fall-out of a loop)
  and calls the COMDAT copy at `return` statements. Retail TeddyGirl 0x00DAF080 shows this:
  - its loop-top thread check and the check after the setup both jump to the epilogue that expands `~CScriptThing Bully`
    (0x00DB0461);
  - so the check is the loop condition (`for(;!IsActiveThreadTerminating();NSF)`), not `if(T) return`.
  - The Oracle 0x00EBCF70 shows the same: the check after StartScriptingEntity and the loop-bottom check share the expanded
    epilogue 0x00EBDBD0.
- Two exits that share one epilogue mean one source check. If the retail code makes a single call to IsActiveThreadTerminating
  where the macros would emit two in a row, drop one: Oracle a5 removed the `@T` after StartScriptingEntity that preceded
  `while(!IsActiveThreadTerminating())`.
- Temporary-destructor flag bits ([esp+x] |= 1, 2, ...) are numbered in source order of the temporaries. Reversed bits mean the
  block is earlier in source (AffairWife 0x00DB2B10, phase B).
- A store written before a call in source is scheduled before that call (AffairWife `WhatsThisIndex=10` before the line
  assignment; Sister `WavedAtHero`/`MoveState` before `WaveTimer=5`).
- `rand()%pGlobalScriptDef->X.size()` indexing `pGlobalScriptDef->Anims[...]`: retail re-reads the def pointer after rand, so the
  index lives in its own local (Sister 0x00EC68A0).
- VC7.1 canonicalises an if/else swap and a trailing `continue`: identical code, so they never fix block placement (Sister a4-a6).
- `"TEXT_"+a+"_"+b+"_TITLE"` with CCharString a, b needs FABLE_RETAIL_CCHARSTRING_CONCAT_STRING (the CCharString+CCharString
  operator 0x0099F570). Without it, VC7.1 reports C2666 (ambiguous operator+) (Oracle).
- /Oa does not explain a load CSE'd across calls in a script Main: it changes far more (Oracle a6: 1240 v 778 diff lines).

Review at landing:
- Every parked row needed more than 4 grades.
- GuardOuter (4 grades) and Forger (4 grades) are below the 10-attempt target because time ran out. Their next ideas are in
  NOTES.md.
- The landed CNOVITeddyGirl_GivenTeddy_00db0600.cpp models CQ_NewOakValeIntroScript / CNOVI_TeddyGirl as global local structs,
  with Leaf "GivenTeddy" and the wrong AddGoodDeed spelling (NAME_FIXES.tsv).
- Two new shared headers: engine/CQ_BanditCampScript.h and engine/CV_OracleScript.h. The landed CGate1GuardInner::Main 0x00D02B80
  and the Oracle same-TU rows keep local models next to them.
