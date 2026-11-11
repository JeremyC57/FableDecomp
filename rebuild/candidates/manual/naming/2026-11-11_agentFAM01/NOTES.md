# agentFAM01: family `_global::CCountedPointer_*_<addr>` (795 rows)

Result: **64 rows renamed** (`rows/`), final batch `work/final.out`: 14 MATCH + 50 RELOCATION_MATCH, 64/64 VC7.1 tests PASS with the
catalog PassPattern. 12 g++ failures are PREFAILs (the unchanged landed tests use `__fastcall` / old stand-in symbols; checked: each
fails g++ on its own). 3 new portable tests (0x00403D20, 0x00413590, 0x00444050: the old ones linked the old extern "C" symbol).
At most 6 grades on a row (`work/gradelog.txt`). Nothing landed, no git, nothing written under the repo, BUSY.txt untouched.
**731 rows left** (`work/left.txt`), reasons below.

## The settled pattern (proved by VC7.1 probes, `work/exp/e2..e4.cpp`)
- Retail source: `~CCountedPointer() { ReleaseData(); }`, ReleaseData zeroes Data before Info = engine/CCountedPointer.h opt-in
  `FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA`. VC7.1 emits a COMDAT for every referenced inline, so every T in a speed TU emits BOTH
  ReleaseData<T> and ~CCountedPointer<T> as identical 53-byte bodies; retail links /OPT:NOREF, so both survive even unreferenced.
- Size (/O1) TU: ReleaseData is the 44-byte body, the destructor a 5-byte `jmp ReleaseData` thunk; scalar deleting dtors and scope
  ends call ReleaseData directly (probe e4). Every 44-byte body is ReleaseData<T> (19 rows: all have live-pointer or thunk callers).
- Speed (/O2) TU: ShareData / Reset inline ReleaseData; scalar deleting dtors and non-inlined scope ends call `??1` (probe e3).
- So a 53-byte body's member is decided by its CALLERS: called from ShareData / Reset / a size-TU (unpadded, `pop ecx` / `and [],0`)
  function -> ReleaseData; called from padded /O2 code -> ~CCountedPointer (`work/classify.py`). 0x0048AFB0 (size-TU member dtor)
  and its landed twin 0x0048C5C0 (/O2 scalar deleting dtors) are the same T, ReleaseData and ~CCountedPointer: the rule in one pair.
- The brief's claim "every Release row is ReleaseData" holds for the 44-byte rows and the live-pointer callers only; 7 referenced
  53-byte rows are destructors (0x0062D9A0, 0x006E46F0, 0x0073D630, 0x009A4400, 0x009A9D60, 0x009C9FC0, 0x00AE94A0).

## Renamed, per shape / T (RENAMES.tsv section 1; evidence column per row)
- ReleaseData, 44-byte /O1 (14): CPlayerGui, NUISystem::CMesh, CStartupDefinitionManager, NHitLocations::CPhysicalPrimitiveBase,
  CGameDefinitionManager, NUISystem::CMenuEntry, vector<NInventory::CItem>, vector<CCharString>, CDrawQuestInfo::CQuestInfoElement
  Bar / Timer / Counter / CounterList / Tick / MiniGameBar.
- ReleaseData, 53-byte (4): CTexture, CDrawQuestInfo::CQuestInfoElement, CCreatureActionBase, CScriptGameResourceObjectScriptedThingBase.
- ~CCountedPointer, 53-byte (7): CCombatActionBase::CStrikeEvent, CLipSyncSampleData, NCreatureMode::CModeHoverBase::CHoverAnim,
  CBankFileAsyncData, NBankFileManager::CRetailBank, CBankFileAsyncEntry, NParticleEngine::CParticleEmitter.
- Converting copy ctor `??$?0` (12): CQuestInfoElement <- the six element types; CVillageTaskBase <- the six village task types.
- Copy ctor (2): CBankFileAsyncEntry, NCameraModes::CBase.
- operator= (12): CBankFileAsyncData, CThreadedFile, CBankFileAsyncEntry, NParticleEngine::CParticleEmitter,
  NAnimationEntryBodge::CAnimEntryLite, CScriptGameResourceObjectScriptedThingBase, CShaderResource, CTexture, NUISystem::CComponent,
  vector<CItem>, vector<CCharString>; member-template operator=<C3DMesh2> on CCountedPointer<const C3DMesh2>.
- SetStaticCast<NUISystem::CComponent> (3): on CMesh, CText, CList.
- Reset (4): CCodeSectionManager, NPlayerGui::CDrawHealth, NPlayerGui::CDrawCombatMultiplier, CGameDefinitionManager.
- (T*) ctor (6): CTexture, CCreatureActionBase, CHoverAnim, CEntityScriptBase, CActiveEntityScriptBase, CAProgressDisplay.
All rows are explicit instantiations over engine/CCountedPointer.h (no local CCountedPointer model); T is forward-declared, or comes
from its shared header where the instantiation needs it complete or nested (CDrawQuestInfo, CVillageTask*, NUISystem_CMesh/CText/CList,
CCombatActionBase). Reset / (T*) rows bind the retail DeleteData<T> addresses (0x00413300, 0x00443500, 0x00443540, 0x0044EAFD,
0x0041BD80, 0x00668610, 0x008EBDD0, 0x00CDEE00, 0x00CE1100, 0x00403600) as `?DeleteData@?$CCountedPointer@...@@SIXPAX@Z`.

## Left (731), and why
- **692 unreferenced 53-byte bodies** (every one of the family's 714 /O2 release bodies but 22): no call, jmp or absolute reference in
  retail (`work/xrefs.py`, `work/absrefs.py`). Each is either ReleaseData<T> or ~CCountedPointer<T> (identical bytes; every speed-TU
  T emits both) and nothing ties it to a T beyond its TU position. Not provable from two sources; left on CUnprovenPointee. Next idea:
  per-TU COMDAT sets (FableWin donor TU order vs retail neighbours) can bound T sets but not settle ??1 vs ReleaseData.
- **16 referenced release rows, T unproven** (callers outside the catalog or stand-in owners): 0x004D78ED 0x004D799A 0x00535EBB
  0x005BA0CC 0x005BA0F8 (44-byte: ReleaseData, T open) and 0x0041BC90 0x004C24C0 0x007A3200 0x008AB320 0x00A319A0 0x00A6EA10
  0x00A6EAA0 0x00A8D1C0 0x00A8D200 0x00A9A350 (53-byte, /O2 callers: destructors) and 0x00AAC870 (size caller: ReleaseData).
  The five 0x00A8xxxx/0x00A3/0x00A6 ones are one function's scope ends at 0x00A8C3D6..0x00A8C403 (a chunked-file loader; likely
  CChunkedFileChunk family), not followed up.
- **Copy ctors** 0x005BA124 0x00704430 0x008AA9B0 (callers not in the catalog) and the list-node copies 0x00654DFD 0x0065D806
  0x0065D822 0x0065D87F 0x0065D8D0 0x0065D8F6 (called through `_Construct` helpers 0x0065E612..; the list -> T chain not traced;
  CTCVillage holds six list<CCountedPointer<CVillageTaskX>>).
- **operator=** 0x0069C380 (only caller is the stand-in CMapInfo_00697c60::Clear at +0xD4; Ego_r CMapInfo has PMap at +0x10 only),
  0x00764400 (only a stand-in vector::insert binder for NShape::CShape), 0x00B8EBC0 (no named caller).
- **(T*) ctors** 0x00BA7CF0 (CEngineSimulationBase only from one stand-in binder), 0x0094B3A0 (no named caller).
- **9 `_Dtor` rows are not CCountedPointer members**: 0x004485E0 0x00825270 0x00828160 0x00A801A0 0x00AE9B00 (STLport heap
  `_Push_heap`/`_Adjust_heap` over pair<int, CCountedPointer<CNavConnectorNode>> in invented `FableHeap_<addr>` namespaces: a caller
  reshape), 0x00A6F390 0x00A70130 (pair<CCharString, C3DMeshVolume> member destruction), 0x0041C150, 0x009FE550: outside this pattern.

## Caller respellings (section 2): none applied
Binders of renamed addresses are listed in NAME_FIXES.tsv. Destructor -> ReleaseData renames (0x0043E1C0, 0x00654B0A..C63,
0x0044F3B2, ...) need the caller TU to define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA(_DECL_ONLY) so its inlined destructor calls
ReleaseData; not attempted (each is a whole-row regrade).

## NAME_FIXES beyond the family
- Landed 44-byte "~CCountedPointer" rows that are ReleaseData (live callers + jmp-thunk destructors): 0x00429160 CAction,
  0x0042918C CMeshDataBank, 0x0042925C CText, 0x00429288 CList, 0x0042F974 CIEngine, 0x0042F9A0 CASoundBank, 0x005C04BF CPerItemData;
  53-byte 0x004057A0 CFontBank (ShareData / Reset callers).
- ShareData rows all carry the propagated label `CCountedPointer<NUISystem::CAction>::ShareData`; their real T from the ReleaseData
  they call: 0x0042913A CTexture, 0x0042943A CMesh, 0x00429460 CText, 0x004291B8 CMeshDataBank, 0x004190E2 CFontBank,
  0x00461B5C CPhysicalPrimitiveBase, 0x0048524C CMenuEntry, 0x005C053D / 0x005C058F the two vectors.
- Destructor thunks (5-byte `jmp ReleaseData`): 0x00429B72 0x00429BF9 0x00429D8E 0x00429D54 0x00655367..0x00655380.
- 0x00558770 binds SetStaticCast on CList to 0x0042A2CA (that is the CText one; CList's is 0x0042A2DB).

## Descriptive names
None introduced; every spelling is in the Ego_r / FableWin name sets.

## Header debt
- NPlayerGui_CDrawQuestInfo.h needs FABLE_RETAIL_CQUESTMANAGER / DEF_CLASS_BASE / CENGINEPERSISTENTPRIMITIVEBASE /
  NPLAYERGUI_CDRAWBASE_HANDLES to pass its size asserts even for a forward use; CCombatActionBase.h needs BASE_OBJECT +
  CINTELLIGENTPOINTER + CCREATUREACTIONBASE; NUISystem_C{Mesh,Text,List}.h need DEF_CLASS_BASE + CENGINEPERSISTENTPRIMITIVEBASE +
  CENGINEPRIMITIVEHANDLE + CIVCOUNTEDPOINTER.
- engine/CCountedPointer.h's header comment still says the dtor is its own body and ReleaseData is Info-first / never out of line; the
  retail evidence (this pass, NF4/NF5) is the RELEASEDATA opt-in model. Making it the default would touch ~950 landed includers.
