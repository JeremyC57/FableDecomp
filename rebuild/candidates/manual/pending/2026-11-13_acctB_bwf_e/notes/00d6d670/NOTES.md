## 0x00D6D670 NScript::CQ_HangingTreeEvilScript::Conclusion -- RELOCATION_MATCH (grades: 7)
- What made it match: whole function written from OUTLINE on the shared headers; CCountedPointer<CScriptThing>::ShareData declared
  out of line (explicit specialization decl, 0x007044D0) under the implicit CScriptThing::operator=; ~CScriptThing given an inline
  empty body in the row (VC7.1's budget then expands it on temporaries and calls 0x004AA840 for the 6 function-scope locals);
  `bool timeUp = false;` declared before the Prisoner assignment; the wait loop as a top-tested do { if (ExecutionerInterrupted) break; ... } while (!timeUp);
  the escape cut-scene locals in an inner scope (MissionSucceeded after their dtors); FABLE_RETAIL_STLP_MAP (no-arg map ctor -> 0x00CDBF70 with only this).
- Headers / flags: overlay engine/CQ_HangingTreeEvilScript.h (FABLE_RETAIL_CQ_HANGINGTREEEVIL_CONCLUSION declares Conclusion());
  defines FABLE_RETAIL_BASE_OBJECT / _CCHARSTRING / _CLEVELSCRIPTBASE / _NSCRIPT / _STLP_MAP / _CSCRIPTTHING_IMPLICIT_ASSIGNMENT; Flags: none.
- Strings: the three Msg* tests use "" (retail 0x0122D70E-style empty literal), not "SCRIPT_NAME_HERO". Made-up local names: unusedHandle, timeUp, guard1H.. (locals only).

- repair (2026-11-12): header now justifies the visible CScriptThing::GetPos / IsAlive bodies (inline in retail, no call to 0x004AA980 / 0x004AB130; copied from the landed rows). Still RELOCATION_MATCH, strict headers too.
