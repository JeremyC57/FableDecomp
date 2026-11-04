# Naming pass wave 35a (2026-11-04): 923 landed slot functions respelled to the names proven by the Anniversary/registration waves

Input: the 1,161 landed rows of family A (small virtual slots) whose catalog names changed in `naming/2026-11-04_anniversary/`
(high wave, waves 2+3) but whose sources still defined placeholder or wrong symbols. One agent built an overlay in scratch
(`scripts/`, reusing the wave 33 generators); the coordinator applied it (`scripts/apply_w35.py`) and re-graded every row.

## Result
- **923 respelled**, each now defining its exact PDB symbol (objdump-checked by the agent):
  GetParentScript 589 (203 via the shared header, 386 via the landed Alloc row's TU, wave 33 precedent), CEntityScriptBinding<T>::
  GetEntityScriptName 118, OnPredicateFail 99, OnPersist 90, scalar deleting destructors 19, def GetSizeofClass 5 / Copy 3.
- **Re-grade from the working tree** (`logs/log0..3.txt`, `vland.py --recheck-landed`, four splits): 923 / 923 MATCH or
  RELOCATION_MATCH + PASS; no RELOC_NAME / leaf / conflict warning; 20 SHARED_HEADER_AVAILABLE carried over from the Alloc rows.
- Files renamed (`RENAMES.tsv`); `build_candidates.ps1` Source / TestSource / Module updated; test behaviour unchanged.
- **Callers:** none (every respelled address is referenced only by its vtable slot; `CALLERS.tsv`).
- **Bindings:** +3 (`BINDINGS.tsv`: ??1CQ_HangingTreeEvilScript 0x00D6ED60, ??1CQ_SecretPassageScript 0x00DF10D0, ??4CScriptDef
  0x00F2A250); `bindings_multi_check.py` 0.
- **Deferred 238** (`DEFERRED.tsv`, one reason each): 118 CActiveEntityScript<T> destructors (DECISIONS_W29_RESOLVED D1 hold);
  39 OnPredicateFail bodies writing members no class model names; 30 whose source row (ctor / Alloc) is itself still misspelled
  (`NAME_FIXES.tsv`; regenerate after wave 35b); 29 with no landed Alloc row; 11 destructors whose model lacks member destructors;
  6 def rows; 5 others.

## Coordinator decisions (agent NOTES.txt D1-D6)
- D1 accepted: the landed Alloc row's TU declares the nested class where the shared header lacks it (wave 33 precedent); a later
  header pass can move these classes into `engine/<Script>.h`.
- D2 accepted: GetEntityScriptName as a minimal TU instantiating the one inline member (byte-identical).
- D3 accepted: members added to 10 level-script class models; offsets from the byte-matching source, names = the persisted keys,
  which equal the PDB member names.
- D6 held: CHeroSoulsCreatureDef::Copy 0x00F27180 matches only with /Ob1 (not an established flag).
