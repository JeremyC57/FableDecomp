# Naming pass wave 20 (2026-11-01): 105 landed rows respelled to proven names, all byte-identical

Inputs: brief `naming/briefs/BRIEF_W20.md`, the wave 19 README / AGENT_README / DEFERRED / scripts, QUEUE_W20_input.tsv (80 rows: 520 1,
c686 25, c687 38, c688 16 -- the c687 / c688 rows were appended by the coordinator as those chunks landed (87ca772c8, bdbb63423); no c689 rows
arrived) and DEFERRED_W19_INPUT.tsv (440 rows).

`scripts/compose20.py ov_final` re-runs every stage, in order, on fresh copies of the current repo (`AGENT_README.md` has the commands).
ov_final has 105 sources plus the tests of 0x009E5120 / 0x009E51C0 / 0x00A6F290 (FastCompare -> operator<), 0x00C61F60 / 0x00C44AB0 / 0x00A3EF00 /
0x00AAC530 / 0x00AAC580 / 0x00C73A00 / 0x00C6A080 / 0x009FC550 (stand-in names mirrored).

**Recheck** against HEAD (commit bdbb63423 = chunk 688; the 142 rows landed after the wave 19 index, the 7 chunk-687 rows and the 5 chunk-688
rows are indexed in `idx/delta*.jsonl` and merged into `idx/full.jsonl`, 53,162 rows) with vland --recheck-landed through scripts/vrun.py
(`logs/recheck_final.log`, `runs/final.pairs.jsonl`): **105 / 105 PASS (9 MATCH, 96 RELOCATION_MATCH).** No byte moved. Nothing failed its
final recheck; the per-family trials (c686a 8/8, s1 22/22, fastcmp 33/36 then 3/3 after the test respells, c687 20/20, c688 19/19) are in logs/.
ov_final was composed from scratch after the trials (every hand stage ran once on fresh copies: ok/already counts in logs/compose_ov_final_*.log).

**BINDINGS.tsv:** 56 RETIRE / 41 ADD / 12 KEEP, no CONFLICT. `runs/reloc_bindings.after.tsv` = the current repo table minus the RETIREs plus
the ADDs; `bindings_multi_check.py` prints 0 (`logs/bindings_multi_check.log`). Index: `idx/full.jsonl` = HEAD, `idx/after.jsonl` = HEAD + ov_final.
Two ADDs are descriptive members that moved with their class: `?GetFramesRemaining@CPKArray@@QAEXK@Z` (0x00C61BE0) and the three
`StoreSound*Allocator_00c166x0` __fastcall stores (DECISIONS_W20 / DEFERRED: their own rows carry the propagated CHoverable_ProcessEvent).

**LABELS.tsv:** 31 labels (bare catalog spelling; mklabels20 FIX corrects the queue spellings the wave proved wrong: 0x007808B0 InitFirePoints,
0x00B21850's three parameters, the compiler leaves of 0x00781670 / 0x0042B467). **RENAMES.tsv:** 46 rows, repo-rooted paths, bare new_name.
Apply both after the content commits.

**QUEUE_W20.tsv** (80 rows): respelled 43, respelled-partial 9, already-correct 14, deferred 14 (0x00C28690 libpk second copy; 0x013CA828
GDefStringTable data symbol; the libpk decision items 0x00C096E0 / 0x00C10A30 + its four setters-and-Play / 0x012B1E24 / 0x00C29FC0; 0x00C73AF0
const char* vs const unsigned short*; 0x00CBB860 / 0x00CBA8E0 own rows not driven). **DEFERRED.tsv:** 456 rows (the queue leftovers +
runs/deferred_extra.tsv: the wave 19 input minus the one entry this wave resolved (0x0129BA3C EPSILON rows) plus runs/deferred20_manual.tsv).
**DECISIONS_W20.tsv:** 7 items for the coordinator (GDefStringTable, vtable 0x012B1E24, CPKVoiceParams, CDebugStream, the 0x00AD3A10 set /
_Rb_tree copy constructor, vector::push_back() at 0x0051B620, CPKArray::GetFramesRemaining).

## Families, in commit order (`runs/<fam>.rows.txt`, cumulative content in `ov_<fam>/`)
- **c686a 8** (scope 1): `?AddFile@CWADBank@@QAEXABVCWideString@@@Z` in UpdateMetaData 0x00502170; the private `?InitFirePoints@CTCFlammable@@AAEXXZ`
  (the PDB name -- InitialiseFirePoints is in neither PDB) in 0x00780C80 / 0x00780D20; `?SetFromVectorAndAngle@CMatrix3x4@@QAEXABVC3DVector@@M@Z` on a
  default-constructed CMatrix3x4 in 0x00A13C60; own rows 0x007817F0 `?GetEffectAtHelperPoint@CFlammableDef@@QBE_NABVCCharString@@AAJ@Z` (long& out; MATCH)
  and 0x00781B90 `?GetAllFireHelpers@CFlammableDef@@QBEXAAV?$vector@VCDefString@@..@@@Z` (class CDefString, unsigned long CRC; MATCH); own rows 0x00C632E0 /
  0x00C63350 `?SetRoomHF@CEaxContext@@QAEJKJ@Z` / SetRoomLF (were CLight::SetIntegerProperty2/3).
- **sort 4** (scope 1, 0x00CBDAD0's callers): the CScriptManager sort chain re-authored on the STLport 4.6 _algo.c text, byte-identical:
  0x00CBDF70 `??$sort@PAU?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@std@@V?$CCompareString@..@@@std@@YIX..@Z` (binds
  `??$__introsort_loop@..` 0x00CBDAD0, was IntrosortLoop_00cbdad0), 0x00CBD420 __final_insertion_sort, 0x00CBB2D0 __unguarded_insertion_sort_aux, and the
  pair copy constructor 0x00CB9C50 `??0?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@std@@QAE@ABU01@@Z` (MATCH).
- **spline 4** (scope 1): the NParticleEngine::CPSCSplineBase model (nested CSplinePosParamData / CSplineRandomOffsetData, CParticleUpdateParameters<unsigned
  long, unsigned long>, CCardinalSpline::GetSplinePosition const 0x00A567A0, the private TransformPosToMaintainShape 0x00B202D0 / GetClosestParameterisedControlPoints
  0x00B21450 / 0x00B214C0): 0x00B21850 `?TransformFromPosition@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@3@AAVC3DVector@@M@Z`
  (three parameters per the PDB -- the queue's one-parameter spelling is wrong), 0x00B218B0 TransformFromIndex, 0x00B21940 GetPointAtPos, 0x00B219A0 GetPointAtIndex.
- **erase 3** (scope 1): 0x00AD14E0 `?_M_erase@?$_Rb_tree@KU?$pair@$$CBKM@std@@..@@AAEXPAU?$_Rb_tree_node@..@@@Z` (map<unsigned long,float> Groups of
  COptimisedVertex) and 0x0043020A (map<CCharString,long>, key destroyed through ~CCharString 0x0099EAE0) on the landed 0x00834FD0 STLport _tree text;
  _M_erase private (AAE) in RemapGroups 0x00AC97D0.
- **misc 11** (scope 1): EPSILON -> DELTA_NOTIONAL_ZERO in 0x00751E80 / 0x00788617 / 0x00BADB40; GetSize() -> `?GetLength@CMemoryDataOutputStream@@QBEKXZ`
  in 0x009D4A50 / 0x009D4F10; 0x0042B467 `?_M_find@..` with the long value (CUnprovenValue gone); 0x00781670 the map<CDefString,long> find (compiler Leaf);
  0x007E6EC0 rebuilt from the landed 0x0056D050 twin as `??$GetPFirstThing@VCThingFilter_HasDefName@@VCThing@@@CThingSearchTools@@QBE_NABVCThingFilter_HasDefName@@PAPAVCThing@@@Z`
  (DECISIONS_W19 item 6 resolved: the callee 0x006A7BE0 is HasDefName::operator(), 0x006A80A0 is HasTC) and PlayGame 0x007E5D20 naming both instantiations;
  0x009E6530 `??MCCharString@@QBE_NABV0@@Z` own row; 0x00AC99F0 `?RemapGroups@COptimisedPrimitive@QuadricOptimiserInternals@@QAEXABV?$map@KK..@@@Z`.
- **fastcmp 36** (scope 1, 0x009E6530): FastCompare -> operator< in 36 callers (famlib driver; 1 skipped: 0x009E6FA0 SymbolString stand-in); the three
  self-contained tests that define the comparator respelled (they LINK_FAILed first).
- **c687 20** (scope 1, chunk 687 NAME_FIXES; libpk descriptive names, no PDB): own rows 0x00C61C60 `?GetCount@CPKArray@@QBEKXZ` / 0x00C61BC0 GetCapacity
  (+ caller 0x00C44B50), 0x00C44A90 `?GetVolume@CAmbientEmitter@@QBEMXZ` (+ 0x00C61F60 / 0x00C44AB0), 0x00C08800 `??0CSoundSystemConfig@@QAE@XZ`,
  0x00C087C0 `?SetChannel@CSoundSystemConfig@@QAEXK_N@Z` over the landed SetMaskBit16 0x00C0AEE0 (+ 0x00A3EF00), 0x00C08CF0 / 0x00C08D00 / 0x00C08D10
  the __stdcall SetSound*Allocator(LHA::CAllocator*) over the __fastcall stores, 0x00C36C70 `?Lookup@CAmbientScene@@QAEPAUCSequenceItem@@K@Z`, 0x00C46DA0
  `?Add@SPosition2D@@QAEXABUCIndexedValue@@@Z`, 0x00AD7550 `??$sort@PAVCGroup@COptimisedVertex@QuadricOptimiserInternals@@P6I_NABV123@0@Z@std@@..` (STLport
  text, __final_insertion_sort inlined), 0x00AD4E20 the CGroup element, 0x00C73A00 ClassifyAudioExtension, 0x00CA6620 HWND, 0x00AAC530 / 0x00AAC580
  `?GetNextEmitterID@CSoundManagerLH@@QAEJXZ`.
- **c688 19** (scope 1, chunk 688 NAME_FIXES): 0x00C69FB0 Blend's non-const first parameter; 0x00C6A080 CStereoDelayLine; own rows 0x0051AF00
  `?DeleteData@?$CCountedPointer@VCObject@CLevelLoader@@@@SIXPAX@Z` (engine/CCountedPointer.h), 0x00513570 the STLport __uninitialized_fill_n over
  CLevelLoader::CObject::CLevelFiles, 0x006C27A0 `?SetAsLoading@CObject@CLevelLoader@@QAEXABV?$vector@..@@ABV?$set@VCCharString..@@J@Z`, 0x0050F980
  `??1CLevelFiles@CObject@CLevelLoader@@QAE@XZ` (engine/CCountedPointer.h) with the nesting in 0x00518B80 / 0x005195B0 / 0x0051B620 / 0x006C2D40 (the vector
  operator= with its allocator parameter), 0x00B6D250 / 0x00B6D0B0 / 0x00BE4380 vector<CTVertexLandscapeBackground>::_M_clear / _M_insert_overflow / push_back
  (STLport text), the five map<EOpinion..>::_M_erase rows 0x004507FB / 0x004DA360 / 0x0067A840 / 0x0067A800 / 0x0067A7C0 (STLport _tree text; the two
  size-optimised ones keep their pragma), 0x009FC550 `??1CResource@@UAE@XZ` on engine/CResource.h (FABLE_RETAIL_DEF_CLASS_BASE; callee
  `?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z` 0x009FC020).

## Not done this wave (DEFERRED.tsv / DECISIONS_W20.tsv)
Scope 2 (wave 19 DEFERRED: PeekTypeList 33, HasTC 11, the cab / cmb / tcb destructor families, 0x00CAF680, the CODA globals, 0x01238C8C) and scope 3
(destructor and mega families, cg10) were not reached: the three chunk queues (c686 / c687 / c688) took the wave. Per-row deferrals added:
0x00AD7400 (three invented CRbTree stand-ins), 0x00431606 (hand-leaf insert_unique), 0x00AD2F60 / 0x00AD39A0 (identity by caller), 0x00AD5B50
(__insertion_sort stand-in now bound by the sort row), the three StoreSound*Allocator own rows, the 0x00AD3A10 set-copy callers, the two ~CMemoryBlock
stand-ins of the 009fc550 destructor family (0x004303F0 / 0x004E108C).
