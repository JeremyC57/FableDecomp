# NF47 notes (round 7: game-component singletons e18 / e19 and CBankFile::FindIndexBySymbolCRC e74)

All counts come from STATUS.tsv, APPLIED.tsv, RENAMES.tsv, work/grades.log and recheck_final.log.
The run was interrupted by a container restart after 56 of 57 rows had a status; the resumed run checked the Wine lane first
(work/probe_005eb4b0_resume.log, a probe of the repo copy: RELOCATION_MATCH), finished the last row, wrote the hand-back files
and ran the final recheck.

## Outcome
- WORKLIST.tsv: 57 distinct rows. STATUS.tsv: 54 ported, 1 not-a-binder, 2 blocked, 0 deferred.
- recheck_final.log: one vland run over all 54 changed rows: 54 RELOCATION_MATCH, 54 tests PASS.
- APPLIED.tsv: e19 done (9 binders); e18 blocked (32 respelled; 1 row left, 0x008AF850); e74 blocked (22 respelled; 1 row left,
  0x0065C781). 0 rejected (REJECTED.tsv has its header only).
- Grades: 177 in work/grades.log over the 56 graded/attempted rows (q.py grades plus three vland runs: run 1 over 54 rows, a
  2-row test rerun, the final 54-row run); the most any row used is 5 (0x0082A180).
- RENAMES.tsv: 3 section-1 self renames, 80 section-2 binder respellings over 54 rows (32 to PMainGameComponent, 9 to
  m_pNewFrontendGameComponent, 22 to FindIndexBySymbolCRC, the rest callees the same edits moved onto PDB spellings:
  CCharString::NullChar, GetMainPlayer, GetPPlayerCharacterThing, IsFirstPersonTargeting, GetRegion,
  InitialiseMiniMapFileLoading_Region, NTargeting::GetTargetingPositionOnThing, CConsole::Initialise, InitialiseDefs).
  Section 2 is generated from the relocation diff of each row's original object against its new one (work/diffrel_all.txt;
  jump-table `$L` labels dropped).
- Locks: 57 under nflocks/ are NF47's (the worklist rows). The other locks are NF48's; no NF46 / NF48 row was touched.

## Rows
- **0x005EB4B0 not-a-binder:** the repo copy already binds ?PMainGameComponent@CMainGameComponent@@1PAV1@A (probed twice). Its
  own header says "Naming wave 18" made that fix; the commit cannot be named without git. It still reads the static through a
  pre-existing local CMainGameComponent model (a port candidate, not a binder fault).
- **0x00639BA0** is listed under e18 but already bound 0x013B86A0 exactly; it was ported for e74.
- **0x008AF850 blocked/port (1 grade):** the row is an ExternalRetry namespace with local CCharString / CDefStringTable /
  SymbolMap / CASoundBank models and locally defined callee bodies; the header's GetSampleBank() returns ::CASoundBank&, whose
  GetSoundIndexFromSymbol takes ::CCharString (C2664; attempt work/attempts/008af850_a1.cpp). Port first: fable_string.h
  CCharString + engine/CASoundBank.h (SOUNDINDEX) + CDefStringTable.
- **0x0065C781 blocked/port (1 grade):** the CTextBank stand-in is NGameText::CDataBank (PTextBank +0x14), but 0x009C9070
  GetPTextEntry returns NGameText::CTextEntry, whose Ego_r members SpeechBank +0x1C / SoundName +0x24 are missing from
  engine/NGameText_CDataBank.h (C2248 / C2440; attempt work/attempts/0065c781_a1.cpp). 1002-byte row with 22 local models.
  Port first: a CTextEntry member opt-in in engine/NGameText_CDataBank.h.
- **0x004175E5 (judgment):** the row IS CMainGameComponent::Shutdown and already modelled CMainGameComponent locally; the stand-in
  global was replaced by that model's own protected static PMainGameComponent. Moving Shutdown onto the header needs typed
  scoped_ptr / counted-pointer members (a port task).
- **e74 fallback rows** (0x009C95E0, 0x0054F370, 0x0054F410, 0x00594FDB, 0x0057AD4C, 0x0047EEE4, 0x0048F6BD, 0x00639BA0,
  0x00892850, 0x004B1720, 0x004B1BA0, 0x005BDA1D, 0x008139C0, 0x00891940, 0x00891A00, 0x004023F0): each keeps its PRE-EXISTING
  local NGameText::CDataBank model (engine/NGameText_CDataBank.h exists) and only takes the CBankFile base from engine/CBankFile.h
  so the call binds the real FindIndexBySymbolCRC. No row gained a new local class (checked: no class/struct name in any row
  that its original lacks). Moving those CDataBank models onto the header is a separate port.
- Rows edited after their last q.py grade (comment-only: 0x004023F0, 0x0048DE44, 0x007BEDF0) are covered by recheck_final.log.

## e74 claim check
?FindIndexBySymbolCRC@CBankFile@@QBEKK@Z is Ego_r.pdb 0x00B1A5C0 and FableWin.pdb 0x02F978C0. None of the bound stand-ins
(NGameText::CDataBank::LookupValue, CTagTable::GetTagFromCRC, CSpeechMap::LookupValue, CTextBank::LookupValue,
CBankFileAsync::LookupValue, CDataBank_Retail::LookupText, CVectorMap_006e3ab0::LookupValue) is in either PDB, and CTagTable /
CSpeechMap / CTextBank / CDataBank_Retail are not PDB classes at all. So no caller has an ICF "true callee" of its own. Every
caller reaches 0x009CC410 on the game text bank (NGameText::CDataBank : CBankFile) or a CBankFileAsync (CBankFileAsync : CBankFile).

## Tests
Two tests linked the old stand-ins and failed (LINK_FAIL, work/recheck_run1_tests_linkfail.log). Both were rewritten on the new
spellings with the PassPattern kept: rows/0047db73_test.cpp and rows/00497b30_test.cpp. They define the statics, use opaque storage
for the singletons, and reach the protected / private names (PMainGameComponent, m_pNewFrontendGameComponent, the private
GetPTextBank) through explicit class-template instantiations, where access is not checked. They do not use `#define private
public`, because that would change the mangled ABE / 1 / 0 access codes. Both pass (work/recheck_tests1.log, recheck_final.log).

## Self renames (RENAMES.tsv section 1)
- 0x0042F75E CNewFrontendGameComponent::Init QAE -> UAE (virtual; no E8 caller; reached through the vtable).
- 0x00497B30 CProgressDisplay::GetPTextBank -> ABEPBVCDataBank@NGameText@@ (its only E8 caller 0x0049A0B0 includes
  engine/CProgressDisplay.h and already spells it).
- 0x0082A180 CMiniMapDisplay::InitialiseOnRegionLoad(const CCharString&) (its only E8 caller 0x0064DFDE already spells it).
The landed file names already carry the right class and method, so no file rename is needed.

## Binding checks on the final objects
- work/chk_final.txt: across the 54 rows there are 80 (row, target) lines on 0x013B86A0 / 0x013B871C / 0x009CC410, all EXACT,
  0 WRONG, and every row has at least one line.
- recheck_final.log has no RELOC_NAME warning on the three targets.
- reloc_bindings.tsv: the new GetPTextBank and InitialiseOnRegionLoad(const CCharString&) leaves are bound only to their own
  addresses (0x00497B30, 0x0082A180). Init@CNewFrontendGameComponent has no binding row, because it is reached through the vtable.

## Funclets
No file in rebuild/src/compiled/01/21/*nwind* (1,356 files) includes a changed row's file name or address.

## NAME_FIXES.tsv (proven, not applied: outside this slice)
- 0x006E3AB0: the row is CTCTalk::PlaySpeech(const CCharString&, EDialogueLayer, bool, bool) (both PDBs). The catalog name
  CMemoryDataInputStream_006e3ab0::OpenStringSource is a propagated label, and caller 0x006E3B30 binds a non-PDB spelling.
- 0x007BEDF0: NThingShot::GetCreatureFireAtPos(const CThing&) (both PDBs; caller 0x007BC8FC already binds it); the self leaf is
  still the global stand-in.
- 0x00890670: CGameScriptInterface::IsHeroInProjectileWeaponMode() const, virtual (both PDBs); the self leaf is still an extern C
  stand-in.
- 0x00A39CC0: CWADFile::Open is virtual (UAE) in both PDBs; the self row and caller 0x00A39D80 spell QAE.
- 0x006E3890 (not landed): PlaySpeech(unsigned long, const CCharString&, EDialogueLayer, bool, bool) in both PDBs. Row 0x006E3AB0
  still calls it through its stand-in `Play`; this round only moved that stand-in's first parameter to the index type. Caller
  0x006E3B10 binds a non-PDB spelling.
- Candidate only, not proven: 0x0070C0D0 is still the stand-in GetMeshBank_2. The caller 0x00753930 binds
  ?GetMeshBank@CAnimateThing@@QAEPAVCMeshDataBank@@XZ, but the PDB spelling is const, returning a reference
  (?GetMeshBank@CAnimateThing@@QBEABVCMeshDataBank@@XZ). This needs a body check.

## Judgments for the coordinator (possible rule slips)
- The 16 e74 fallback rows edit a PRE-EXISTING local NGameText::CDataBank model even though engine/NGameText_CDataBank.h exists.
  0x004023F0 reshapes it the most: a polymorphic CBankFile STATICBLOCKS base, an opaque rest, and a declared
  GetDataFormatVersion override.
- 0x004175E5 adds the static PMainGameComponent to its pre-existing local CMainGameComponent model. The landed 0x005EB4B0 is
  the precedent.
- Neither change is a new class, but the coordinator may prefer to hold them until the header port.

## Descriptive names
None added in rows. The test-only helper templates (GrabMainStatic, GrabFrontStatic, GrabGetPTextBank) are fixture code.
