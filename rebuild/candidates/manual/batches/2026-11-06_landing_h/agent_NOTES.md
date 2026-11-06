# chunk 807 notes (six gap-scan Mains)

All six names are proven from two sources: the entity/script vtable slot (with landed neighbours) plus the Ego_r.pdb and FableWin.pdb
name sets. No descriptive names in the row names. Descriptive only: the quest_status enumerator names in CQ_UndeadRisingScript.h.

## 0x00E0B160 NScript::CQ_UndeadRisingScript::Main -- RELOCATION_MATCH (a2)
- Shape: generated level-script Main; three `{ }` cutscene blocks; explicit CCharString("") spawn names; a row-local inline
  CScriptThing::IsAlive body (null PImp test + slot-75 virtual) is what retail expands at the two effect re-creation tests (a1 called
  the 0x004AB130 COMDAT: 51 lines).
- Header: new engine/CQ_UndeadRisingScript.h (FABLE_RETAIL_NSCRIPT).
- Callee names: WatchForTermination 0x00E0C350 (landed under CV_LostTraderScript::WatchForQuestFailureState: wrong, NAME_FIXES),
  LampChecker 0x00E0C4C0, the five entity Allocs; map dtor 0x00CDBFB0 has a placeholder catalog label (NAME_FIXES).

## 0x00DBCD60 NScript::CQ_NewOakValeIntroScript::CNOVI_Victim::Main -- RELOCATION_MATCH (a1)
- Shape: the landed AffairMan model (TRetVoid, for(;!false;NSF), OUTOFLINE movie + STB members, declared-only cast-map ctor/dtor,
  inline CIsThingConscious ctor/dtor). Matched first try.
- Header: CQ_NewOakValeIntroScript.h opt-in FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_VICTIM_LIVEFATHER.
- strchk: retail's string list starts with the vtable 0x0127094C (a dir32 the tool counts as a string); aligned after that.

## 0x00DB86B0 ...::CNOVI_LiveFather::Main -- parked DIFFER(4325v4325), 111 lines
- Same size; only the hint-loop return pads merge differently (retail: Xbox + PC returns share one pad, final T-check its own).
  10 spellings tried (STATUS.tsv). Lane gap candidate: return-pad tail merging.

## 0x00ECF7F0 NScript::CV_SickChildScript::CWomanToAttract::Main -- parked DIFFER(4835v4885), 204 lines
- Key spelling: same-TU noinline CBasicString<char>::Compare body (LESSONS). FORCEINLINE_TEXT_EQ + TEXT_EQ_INLINE opt-ins.
- Residue: retail expands the SetIsPushableByHero by-value CScriptThing copy but calls the predicate copy; any inline/implicit copy
  expands both (gotchas 2131/2248). ~CTimer expanded only at the natural loop exit in retail.

## 0x00DC93F0 ...::CSecretPassageSkeleton::Main -- parked DIFFER(4536v4507), 320 lines (SDIFF 9)
- Generator probe/gen_dc93f0.py (best: a6 stdtor cpdecl xstr). IsDistanceBetweenThingsUnder(hero, marker, 2.0f) order from retail.
- Residue: the CSpawnedFunc1 Par1 copy is called while the by-value temporary is expanded (same per-site class); end-pad movie dtor.

## 0x00EE57B0 NScript::CV_TourGuideScript::CTourGuideGuide::Main -- parked DIFFER(4356v4246), 287 lines
- Header: CV_TourGuideScript.h opt-in FABLE_RETAIL_CV_TOURGUIDESCRIPT_GUIDE.
- Nested talk block (retail T-check between the distance and MsgIsTalkedToBy tests) removed every body hunk.
- Residue: exit pads (retail expands DeregisterTimer at the natural exit and one shared pad, calls 0x00CD4470 at the common pad;
  plain inline expands it at 6 pads), and retail's inline CTimer ctor keeping both indices in ebx/ebp. a4/a9 (ctor + dtor
  declared-only) are 256 lines but lose the body nesting; a10 is the better base.

## One symbol, one address
- 0x00411570: reloc_bindings.tsv binds 11 spellings to it (?Compare@@YIJPBD0@Z, CompareStrings, Compare_00411570, ...; the landed row is strcmp_signed);
  the PDB spelling ?Compare@?$CBasicString@D@@KIJPBD0@Z is already one of them. Only the parked 0x00ECF7F0 uses it (NAME_FIXES).
- 0x00DAEDE0 / 0x00DAC2C0 and the 14 other NOVI Allocs are bound by 0x00DABAC0 as C<Name> instead of CNOVI_<Name> (NAME_FIXES).
  No row of mine binds them, but the new header declares the CNOVI_ spellings.
- 0x00CDBFB0 / 0x00E0C350: bound here with the PDB spelling; the catalog/landed labels differ (NAME_FIXES).

## Header debt
- None beyond the opt-ins. CV_SickChildScript.h is unchanged.
