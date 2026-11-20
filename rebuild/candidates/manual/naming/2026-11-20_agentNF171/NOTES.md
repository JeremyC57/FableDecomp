# NF171 notes (2026-11-20, round 10: CThing / player GUI naming)

Handed back early (context). Counts come from APPLIED.tsv / STATUS.tsv / recheck_final.log.

## Census
`work/probe.tsv`: one HEAD compile of every landed REFS row (195 rows) with the symbol each binds at each entry address
(`tools/probe.py`, `tools/summ.py`). Several entry texts listed binders that were already respelled at HEAD (q4768's five, q6211's two);
the census, not the entry text, decided the work.

## Per entry (short)
- q4477, q5024: done-already (55/55 REFS rows bind the Create claim; 0x0085CDE0 is Validate in Leaf, catalog and manifest).
- q4768: 0x00490A22 was the only wrong binder (43/44 right); ported onto NHeroInformationScreens_CManager.h (the 0x00490927 recipe).
- q5023: 0x00637FC0 is not a landed row. Both binders 0x0071A710 / 0x0071A750 were moved together onto the shared headers (0x0071A730 form);
  their own leaves became the PDB CTCMapwho spellings that every referencer already binds. 0x0071A750 also moves 0x00637A10 from the
  DestroyMapwhoNode stand-in to RemoveThing (NAME_FIXES: the 0x00637A10 self row / catalog name is that stand-in).
- q5131: 0x00891FF0 ported (CPlayerGui::GetMiniMap, CMiniMapDisplay::RemoveMarker, PClass).
- q5132: 0x0082A310 ported (Initialise, NF171 opt-in). BLOCKED rows 0x00440E40 / 0x00441670: see below.
- q5207 / q6211: the old-style 0x00838930 row (extern "C" Forward, no Leaf) re-authored under the PDB virtual; all 5 referencers already bound it.
- q11704: self 0x00446B10 defined on the header's own class; 4 binders respelled; 0x0068EB20 deferred (NF168 lock).
- q11767: 0x0088E1C0 ported; the 6 "(none)" REFS rows are byte coincidences (operand bytes 60 52 6A 00), recorded not-a-binder.
- q11921: 0x00687C90 respelled (NF171 opt-in on CGamePlayerInterface.h), test rewritten (it linked the row).
- q11922 / q11924: 0x004C7CA0 leaf made const in place (QBEJXZ), 0x00892DF0 re-authored on the headers (its GetIndex stand-in now binds
  the const spelling). The full header port of 0x004C7CA0 was NOT re-attempted: NF153 spent its 3 forms (DIFFER 31v29, `mov eax,ecx` +
  movsx into edx); the row keeps its pre-existing local CThing / CThingManager / CInputManager models.
- q11935: 0x00903970 respelled (ClearCurrentAction); 0x006929A0 deferred (NF170 lock, binds `@runSub@4`).
- q11936: partial. 0x00903920, 0x00903970 ported, 0x0096FF10 respelled in place (made-up StopCurrentAction). Open, not attempted:
  0x00890BA0, 0x00890C10 (CThing_Retail giant-preamble rows), 0x00890C80 (CThingNative), 0x007A71B0, 0x007AB190 (FinishCurrentAction
  declared on a local CThing model; it is CThingCreatureBase's, which has a header, so a local CThingCreatureBase would be a NEWLOCAL:
  header port needed).
- q11942, q11973, q11975: not started (open).

## q5132 decision for the coordinator (0x00440E40 / 0x00441670)
Both rows are byte-identical 25-byte `~list<T>` bodies (call 0x00440730 clear, then free the sentinel). The raw Fable.exe scan
(tools/ecall.py: every E8/E9 rel32 and every absolute dword in all sections) finds NO caller and NO reference to either, so nothing proves
T. 0x00440730 is retail's _List_base::clear over intelligent-pointer elements, landed as the CMiniMapDisplay::CMarkerInfo instance (agentNF23),
and is plausibly an ICF fold of several T. Options: (a) keep the stand-ins `@CActiveFile_Helper_00440730@4` /
`?CActiveFile_Base_OnReadFinished@@YIXPAUCActiveFile@@@Z` (and their own wrong names CActiveFile_OnReadFinished, catalog
`_global::StdList_Dtor_*`); (b) bind the one folded name per the nf104 per-identity rule. I did not choose: the element type is unproven.

## Casts reported (brief: casts to reach a PDB spelling)
- 0x0088E1C0: `static_cast<CThingPhysical*>(CThing*)` after `GetTypeFlags() & 2`; 0x00903920 / 0x00903970: `static_cast<CThingCreatureBase*>`
  after `GetTypeFlags() & 8`. Both sides are PDB classes from shared headers (nf105: source-faithful), the 0x0085FA00 / 0x00869020 idiom.
- 0x00687D50: `(EGameAction)action, (EInputButtonEventType)buttonEventType` because the row keeps its own stand-in (long, long) signature
  (the PDB self signature is the enum one; its self rename is a 6-referencer family, NAME_FIXES). 0x006881D0 / 0x00582A16 / 0x007E0750 pass
  the held kind as `(EInputButtonEventType)2` (no enumerator name recovered; the landed 0x00747DA0-style rows use local enums instead).
- No stand-in class was cast to a real class.

## Inferred
- 0x00891FF0: a named `const CThing& pthing` local orders the singleton load after the GetPThing vcall (one-expression form DIFFER 53v49).

## Descriptive names
- None new. 0x013B86C4 keeps the landed `?g_miniMapFlag@@3EA` (no PDB data symbol found; only 0x0082A310 binds it).
- The row-local inline helper `CTCInventory::IsActionButtonHeld` (0x00582A16) is a pre-existing descriptive helper (now takes EGameAction).

## Local models / acceptance
- No new local model of a header class (selfcheck.log). Rows ported onto headers dropped local models (0x00490A22: CWideString, pair,
  bitset, CVectorMap specialisation, CHeroInfoScreensManager; 0x0071A710 / 0x0071A750 / 0x0082A310 / 0x00838930 / 0x00446B10 / 0x00891FF0 /
  0x0088E1C0 / 0x00892DF0 / 0x00903920 / 0x00903970: all local models). In-place respells inside pre-existing local models: 0x006881D0,
  0x00582A16, 0x007E0750 (CGamePlayerInterface), 0x00687C90 (CInputProcessGameBase member type), 0x004C7CA0 (CThing const), 0x0096FF10
  (CThingCreatureBase member name).
- objdump defined-symbol diffs (HEAD vs new, tools/defs.sh) per row are in STATUS notes: only own leaves, header inlines / pick-any COMDATs.
- Tests rewritten on the shared headers (they link the row): 0x0071A710, 0x0071A750, 0x00838930, 0x00446B10, 0x00687C90 (PassPatterns kept).
  Unchanged standalone tests (do not link the row): 0x00490A22, 0x00891FF0, 0x0082A310, 0x00687D50, 0x006881D0, 0x00582A16, 0x007E0750,
  0x0088E1C0, 0x004C7CA0, 0x00892DF0, 0x00903920, 0x00903970, 0x0096FF10.

## Self renames (RENAMES s1) and their referencers
0x0071A710 (4 refs, all bind the new leaf), 0x0071A750 (1, binds it), 0x0082A310 (1, binds it), 0x00838930 (5, bind it), 0x00446B10
(13 refs: 8 bound + 4 moved here; 0x0068EB20 keeps the existing QAE_NJJJ spelling, held by NF168: no new spelling is created),
0x004C7CA0 (7: 6 bound + 0x00892DF0 moved here), 0x00892DF0 / 0x00903920 / 0x00903970 (no referencer; vtable slots only).
No EH funclet includes any changed row (grep of rebuild/src/compiled/01/21 for the changed file names: none).

## Flags lines
- None added.

## Rule slips
- Entry order: q11936 was left partial (5 rows not attempted) and q11942 / q11973 / q11975 not started when I handed back for context.
- One Bash call failed on a shell quote (an apostrophe inside a single-quoted NAME_FIXES argument); the lines before it ran, the NAME_FIXES
  line was re-run.
- I claimed 0x00692DB0 by mistake (already right) and released it (owner file `released`).

## grades_used convention
STATUS `grades_used` = q.py grades of the new source through tools/g.sh (work/grades.log), failed compiles included. Not counted: the
census probe compile, the pdiff pair (HEAD + new compile, run once per finished row), bindchk compiles of referencers, and the vland rechecks.
Maximum 2 (rows whose first compile failed on a missing opt-in, or whose first form differed: 0x00446B10, 0x0088E1C0, 0x00891FF0, 0x00892DF0, 0x00903920, 0x00903970). No row over its budget.
