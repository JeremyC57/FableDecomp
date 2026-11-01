# Naming pass wave 17 (2026-11-01): 414 landed rows respelled to proven names, all byte-identical

Inputs: brief `naming/briefs/BRIEF_W17.md` (+ W16 / W15), the wave 16 README / DEFERRED / scripts, `ESTABLISHED_SPELLINGS.tsv`, and
QUEUE_W17_input.tsv. The coordinator appended rows while the wave ran, so the input ended at 296 rows: 520 1, c675 30, c676 17, c677 24,
c678 38, c681 31 and cg10 155.

`scripts/compose17.py ov_final` (with `compose17_steps.py`) re-runs every stage, in order, on fresh copies of the current repo. hand17/ holds
the per-family hand stages; fam_q.py / fam_q17.py are the generic caller respells. fam_q17 adds a "derived" access mode for protected
members reached from a class that derives from the member's class at offset 0 (checked against ghidra_out/rtti_bases.tsv).
ov_final has 416 files: 414 sources plus 2 tests (0x00404720, 0x00D25410).

**Recheck** against HEAD with vland --recheck-landed through scripts/vrun.py (`logs/recheck_final.log`, `runs/final.grades.tsv`):
**414/414 PASS (4 MATCH, 410 RELOCATION_MATCH).** No byte moved. The 4 rows whose respell failed the recheck were reverted and listed in
DEFERRED.tsv as `[recheck-fail]`.
- 390 rows come from one full recheck of ov_final.
- The 24 rows added last (9 c681 ??_G, 15 c681 callers) were rechecked in trial overlays. Their composed files are byte-identical to the
  trial files (md5), and the other 392 files did not change (`runs/ov_final.md5.*`).

**BINDINGS.tsv:** 110 RETIRE / 32 ADD / 27 KEEP.
- `runs/reloc_bindings.after.tsv` = the current repo table minus the RETIREs plus the ADDs (35665 lines).
- `bindings_multi_check.py` prints 0 (`logs/bindings_multi_check.log`).
- The index is `idx/head.jsonl`: HEAD at 09:55, plus the 75 rows that chunks c678 / c681 landed during the wave (`idx/delta3`).
  `idx/after.jsonl` = head + ov_final.
- The CUnproven placeholder spellings of 0x006C4F80 / 0x009AA780 / 0x004427D0 are now RETIREd.

**LABELS.tsv:** 46 labels, in bare catalog spelling. **RENAMES.tsv:** 35 rows; the paths are repo-rooted and new_name is bare. Apply both
after the content commits.

## Families, in commit order (`runs/<fam>.rows.txt`, cumulative content in `ov_<fam>/`)
- **gdt 13**: ??_G stand-ins re-authored as the compiler-emitted `??_G<Class>@@UAEPAXI@Z`, using the empty-PDB-ctor emitter.
  - Rows: 0x004942A2 CSubtitleRenderer, 0x006C9C30 CTCHeroOpinionDeedLog, 0x00DBEFA0 CQ_NewOakValeIntroScript, 0x00D782C0 CQ_HangingTreeGoodScript.
  - Plus the 9 c681 rows: CFishCompOrganiser (nested), CQR_EscortTrader, CV_ExposeMayor, CQ_MinionCamp, CLadyGreyWifeManager,
    CQ_RansomVictim, CV_Bordello, CEnginePrimitiveManagerRepeatedStaticMeshes, CEngineSurfaceCompositionManager.
- **qctor 1**: 0x00D25410 is the ctor of CQ_DragonBossFightScript (c676), not of CQ_NewOakValeIntroScript.
  - It is re-authored as `??0CQ_DragonBossFightScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z` (source + test).
  - Vtable binding: 0x012CB70C.
- **stl 21**: real STLport 4.6 instantiations replace the CUnproven / CTreeOwner stand-ins.
  - map<long,COpinionDeed*>: 0x006C4F80 plus 6 siblings, including the map ctor and dtor.
  - The four 0x009AA780 siblings, map<CCharString,NBankFileManager::CContainedBank> (wave 16 DEFERRED).
  - set<unsigned long>: 0x004CF810 _M_erase and 0x00AD2790 const find.
  - 0x00527910: map<EGameAction,pair<C3DVector,long>>. 0x00882BC0: set<CStackEntry*> _M_erase.
  - 0x004427D0: deque<CSubtitleRenderer::CSetTextInfo>, built on the PDB members.
  - allocator is a class in 0x00451583 / 0x00450CCC / 0x00459AF3 / 0x00D34F00.
  - 0x004FE7F0: _Rb_tree_iterator<.., _Nonconst_traits>, which corrects the insert_unique spellings.
- **own 4**:
  - 0x00772110: CTCShop::GetNumShoppers const.
  - 0x0049CF80: the CRightHandedSet copy ctor.
  - 0x00AAB7A0 / 0x00AAB7D0: the CLHFile(PBD / PBG) ctors.
- **data 11**:
  - Anonymous-namespace CTCInventoryItem orientation constants (0x0121AC30 / ACD0 / AD20 / AD50, plus DEFAULT_ORIENTATION in 0x005D9730).
  - 0x005D92F0 calls the private static GetRHSetFromOrientation.
  - g_AudioContext / CAudioContext unified in 0x00C4E1B0, 0x00C3EC30, 0x00C71CF0 and 0x01224BC0.
  - 0x00C72AF0 calls slot +0x50 as alSourcef.
- **c677 6**: NScreenEffect::CScreenEffectPriorityStack.
  - 0x00880A40: Reset is private. 0x00881370: the ctor moves into the namespace. 0x00883E50: _M_erase is private.
  - 0x006B8410 calls Clear.
  - 0x00D6EF20: the CQ_HangingTreeGoodScript ctor. 0x00CFA2F0: ?Alloc@CQ_AmbushTradersScript@NScript@@SI...
- **queue 16**: fam_q / fam_q17 for 4 targets: CountRemainingGroups ABE, FindASimplyDelightfulShop, ControlledMove IAE, GetPWorldSeed IBE.
- **w16d 10** (wave 16 DEFERRED, scope 2):
  - CTCGraphicAppearance moves into namespace NGraphicAppearance (4 callers of 0x004BFAC0).
  - CWillButtonHelperTimer(EHeroAbility, CThing&) callers (3).
  - 0x00B6D600: FreeMemoryPage(long).
  - The 0x00404720 own row and its test: the old test expected the wrong pass pattern.
  - 0x009AE150: TErrorCallback fastcall, and the PDB CPersistContext ctor.
- **cg10 319**: fam_q for 36 of the 155 cg10 targets. The largest is SetPItem 0x00A01B90 with 201 rows.
- **c678 3**: GetPSoundPair (0x007C04A0), the private CBankFile::CloseEntries (0x009D1090), GetDevBankFilePath (0x009CB960).
- **c681 15**: CMemoryDataOutputStream::PeekData const (11 callers) and GetLength const (10 callers); the two sets overlap.

## Queue outcomes (QUEUE_W17.tsv, 296 rows)
- 55 respelled, 42 respelled-partial, 152 deferred, 47 already-correct.
- Each row carries rows_edited, rows_left and detail. For the partial and deferred rows the detail names the blocking reason: notes, the
  fam_q skip categories, or own-row stand-ins.
- 0x00440730 (HOLD) is not in the input and was not touched.

## Deferred (DEFERRED.tsv, 397 rows)
- The queue rows that are deferred or partial.
- 4 recheck failures, reverted: 0x00401F30 DIFFER, and LINK_FAIL on 0x006A2520 / 0x009E1D00 / 0x009E1D50.
- 229 wave 16 rows carried. 5 wave 16 rows were resolved here: 004bfac0, 008edff0, 00f37030, 00404720 + 009ae150, and the 009aa780 siblings.

Not reached:
- The PClass singleton 013b89fc and PMainGameComponent 013b86a0.
- The destructor families (~CBaseClassNonCopyable 0099a300, ~CScriptBase 00cbd510, ...).
- Most of the mega families: GetPItem 00a01b50, ~CCharString 0099eae0, bitset::test, GetSubDef, ConstantFPS.
- The c678 / c681 own-row re-authors (quest-script ctors / Alloc, STLport map/list stand-ins).
