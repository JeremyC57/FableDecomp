# Hand chunk 408 (CAIStateGroupBase .. CAFile): 39 of 40 byte-exact

All 39 are RELOCATION_MATCH by q.py through `gall.sh` (it honours `// Leaf:` / `// Flags:`), and every test passes
under g++ (`tall.sh`) and VC7.1/Wine (`tall.py`).

## What matched (39)

- **CAIStateGroupBase TU** (0x0097E140 anonymous-namespace `GetPBestTargetToFaceForCreature`, 0x0097E270
  `SetThingToNotAvoid`, 0x0097E2D0 `SetProximityForDirectApproach`): the names come from the Ego_r.pdb order, which
  matches retail 0x0097E140/E270/E2D0/E330. The setters use the UNCHECKED out-parameter GetInterface (result ignored,
  so the call is duplicated into both arms and the absent arm reads the never-written out slot). The face-target helper
  is `__fastcall` inside `namespace { }`, with const LowerBound 0x004365B0.
- **CAIStateGroup_OpinionReactionRoot** (0x009857D0 `HeroTitleAtHeroUpdate`, 0x009859A0 `HeroTitleToSelfUpdate`):
  a pack(1) class with ThingReactingTo at +0x6B. The body is `GetInterfaceT(&hero, 0x29)` and then
  `AIAction_SpeakAISpeechToTarget(REPEAT_ONCE, creature, target, title->HeroTitleComment{AtHero|ToSelf}Tag, true, 0x93)
  == COMPLETED`. The IsRunnable twins are at 0x009856C0 and 0x00985890.
- **Render STLport** (0x0098CCC0 `vector<CShaderRenderManager::CLightInfo>(n)`, 0x0098D820 `vector<C4DVector>::push_back`,
  0x0098DB20 `CIndexedArray<CCharString>::push_back`, 0x0098DD10 anonymous-namespace `CShaderStats` ctor): the vector
  ctor is the STLport base followed by `uninitialized_fill_n(T())`; the CLightInfo ctor sets IsEnabled(false) and
  UpdateFlag(true). push_back is a placement copy, else `_M_insert_overflow(finish, x, __false_type(), 1UL, true)`.
  CIndexedArray::push_back inserts `pair<const T,long>(item, Array.size())` into the map and then pushes onto the array.
  CShaderStats needed a keep-alive `new (where) CShaderStats` (and `#include <new>`).
- **CThreadedFileManager** (0x0098E320 `OnFileReadCompletion`; 0x0098E390/E410/E490/E510 `GetTotal{ObjectsPending,
  BytesPending,ObjectsFinished,BytesFinished}Count`; 0x0098F2A0 `ThreadMain`; 0x00990830 `_Rb_tree<CPendingObjectSortKey>
  ::_M_insert`; 0x00991240 `CPendingObjectMap` ctor):
  - The map walks needed the STATIC `_Rb_global::_M_increment(node)` form, which copies the node to a register and
    then stores it back. ThreadMain went from 172 to 176 bytes with that change.
  - The completion routine is `__stdcall`. It reaches the object through `hEvent` and calls the inline static
    `SetDirtyDiscError(true)`, or the hard-disk-error callback with "". It then sets LoadingStatus = 3, calls SetEvent,
    and unpauses the CFileInstaller when it exists.
  - `_M_insert` declares `less<CPendingObjectSortKey>::operator()` out of line (0x0098F570) and uses an explicit
    `template class`.
- **CCRC** (0x009914E0 `CalcFromPathname`, unreferenced): `return CalcFromString(file.ToLower())` with an inline
  CalcFromString. A named `lower` local took its own frame slot (182 vs 186 bytes).
- **CASoundManager** (0x00991910 ctor): the init-list copies of the CSoundManagerInit settings use descriptive member
  names, plus `bool Unknown21[3]` after Flag, then ResourceBank.Reset(new CResourceBank), SetMaximumMemory and
  UnloadDelay = 0.
- **Sound-bank sort family**, element `_STL::pair<long, CSoundBankRecord>`, all under `// Flags: /Gr` (0x00992750
  `__uninitialized_copy`, 0x009927F0 `__uninitialized_fill_n`, 0x009928F0 `__unguarded_linear_insert`, 0x00992AE0
  `__unguarded_insertion_sort_aux`, 0x00992CB0 `__unguarded_partition`, 0x00992E20 `__make_heap`, 0x009930C0
  `__linear_insert`, 0x00993420 `__insertion_sort`). Also 0x009936E0 `vector<pair<long,CSoundBankRecord>>::push_back`
  and 0x009937D0 `CVectorMap<long,CSoundBankRecord>::Insert`.
  - One shared model: CSoundBankRecord has two CCountedPointers (copying bumps Info->RefCount) and four CCharStrings,
    with operator= 0x00992630 and dtor 0x00992510 out of line.
  - `__insertion_sort` passes `CSoundBankEntry(*i)` and `__unguarded_insertion_sort_aux` uses `T(*i)`: the implicit
    record copy is inlined when it copies from `*iter`.
  - Insert is `Data.push_back(value_type(key, value)); Dirty = true;`.
- **vector<char>::reserve** (0x00997330): the STLport form `_M_allocate_and_copy` (`__copy_trivial` with a `last == first`
  check, then memmove), `_M_clear`, `_M_set`.
- **CAFile / NFile path helpers** (0x009974A0 `NFile::CalcCurrentPath`, 0x009974F0 `SetCurrentPath`, 0x00997510
  `GetProjectPath`, 0x009975E0 `IsForbiddenSymbolInComponent`, 0x00997620 / 0x009976D0 `SplitPath` wide and narrow,
  0x00997780 `MakePath`, 0x00997930 `GetFilenameFromPathname`, 0x00997DA0 `ParserIsSeparatorNext`):
  - The /GS rows are plain `wchar_t buf[260]` / `_MAX_*` arrays declared in drive, dir, fname, ext order, with
    `// Flags: /GS`.
  - IsForbiddenSymbolInComponent needs `bool forbidden = false; ... { forbidden = true; break; } return forbidden;`
    to get the `bl` flag. An early `return true` drops ebx (43 vs 55 bytes).
  - GetFilenameFromPathname is `CWideString fname, ext; SplitPath(path, 0, 0, &fname, &ext); return fname + ext;`
    with the wide SplitPath body inlined.
  - ParserIsSeparatorNext peeks a CParsedItem (inline ctor/dtor around the CCharString at +8) and tests
    `IsSymbol() && (GetSymbol() == '\\' || GetSymbol() == '/')` with the landed `__forceinline` CCharString::operator==(char).
  - The table at 0x0129A79C holds `<>:` 0x93 `/\|`, so retail's quote character is byte 0x93.

## Parked for Fable 5.1 (1)

- 0x009913D0 `_STL::map<unsigned long, CThreadedFileManager::CFileObject*>::operator[]` DIFFER(115v115): the pair temporary's `second = 0` store is scheduled before the iterator-copy and hidden-result pushes (retail places it after the hidden-result push); everything else is identical | tried: member-init and body iterator copy ctors, `__default_constructed`, direct `insert_unique`, out-of-line `map::insert`, named default value, converting pair ctor | next: lane gap: scheduling (temporary-init store ordering)

## Skipped

None. No row covers two functions.

## Label corrections

- 0x0097E270 is `CAIStateGroupBase::SetThingToNotAvoid` (catalog "AssignCreatureToTask_Helper"; landed callers call it
  SetNavigationIgnoreThing). 0x0097E2D0 is `SetProximityForDirectApproach` (catalog "SetTaskProperty_AtOffset").
  0x006CBE30 is PDB `CTCCreatureNavigation::SetPThingToNotAvoid`.
- These CThingCreatureBase callees have propagated manifest labels:
  - 0x00661E80 is `IsInMelee` (manifest "CText::GetAlignment").
  - 0x00662410 is `IsInMeleeAsAttacker`.
  - 0x006621A0 is `IsAbleToStrafe`.
  - 0x00661EC0 is `GetCombatManager const`.
  - 0x006E9360 is `CCombatManager::GetCombatantInfo` (manifest "CSurface::GetHeight").
  - 0x0073A6E0 is `CTCCreatureModeManager::GetCurrentMode` (manifest "CPixelShader::CountReferences").
- 0x009857D0 / 0x009859A0 are HeroTitleAtHeroUpdate / HeroTitleToSelfUpdate (catalog "CAIStateGroup_TryPerformSpeechAction"
  / "TryPerformAlternativeSpeechAction").
- 0x0098CCC0 is `vector<CLightInfo>(n)` (catalog "std_vector_Reallocate"). 0x0098DB20 is `CIndexedArray<CCharString>::push_back`
  (catalog "CVectorMap_Insert_Morphs"). 0x0098DD10 is the `CShaderStats` ctor (catalog "NHeroInformationScreens_CBase_ctor").
- 0x0098E320 is `CThreadedFileManager::OnFileReadCompletion` (catalog "CThreadManager_OnLoadingComplete"). Related:
  0x0098DED0 SetDirtyDiscError, 0x0098DEE0 GetHardDiskErrorCallback, 0x00401470 SetHardDiskErrorCallback (globals
  0x013BC868 / 0x013BC864).
- 0x0098E390..0x0098E510 are `GetTotal*Count` (catalog "CFileInstaller_*"). 0x0098F2A0 is `ThreadMain` (catalog
  "CThreadedFileManager_Shutdown"). 0x00990830 is `_Rb_tree::_M_insert` (catalog "RBTree_Insert"). 0x00991240 is the
  `CPendingObjectMap` ctor (catalog "LTextBinTree_Initialize").
- 0x009913D0 is `map<unsigned long,CFileObject*>::operator[]` (catalog "CThreadedFileManager_AddObject").
- 0x009914E0 is `CCRC::CalcFromPathname` (catalog "CParticleMorphs_HashFilename"). 0x009914B0 is CalcFromString.
- 0x00991910 is the `CASoundManager` ctor (catalog "NHeroInformationScreens_CItemDisplay_ctor"). Its callee 0x009FC520 is
  `CResourceList::SetMaximumMemory` (manifest "CInputProcessGameBase::AddToInterface"). The sound module is absent
  from Ego_r.pdb, so the member and record names are descriptive.
- Sort family: the catalog names Vector_CopyElement, Vector_InsertElements, Heap_Sift_Down, Heap_SiftDown,
  Heap_PartitionAndSwap, std_make_heap_Impl, Heap_SiftUp and Heap_Sift_Up are descriptive. The real names are the
  STLport internals listed above. 0x009936E0 is push_back. 0x009937D0 is `CVectorMap::Insert` (catalog
  "NScript_CIsThingAlive_*"; the landed WoundMorphs twin calls it Add).
- 0x00997330 is `vector<char>::reserve` (catalog "GrowableBuffer_Resize").
- 0x009974A0 is `NFile::CalcCurrentPath` (manifest "AddManualCameraEvent"; Ego_r.pdb puts it directly before
  CAFile::SetCurrentPath). 0x00997F80 is `CAFile::GetCurrentPath` (manifest "CInputProcessMain_GetCameraPath"): it caches
  0x013BCA10 from CalcCurrentPath.
- 0x00997620: the manifest gives five CCharString* parameters. This is the CWideString overload; 0x009976D0 is the
  CCharString one.
- 0x00997E90 is `CAFile::ParserReadPastSeparator` (it uses ReadNextItem 0x009BA080). The manifest repeats the label
  "ParserIsSeparatorNext", which belongs to 0x00997DA0.
- 0x0099AED0 is `CWideString::CWideString()` (manifest "CCharString"). 0x0099EAE0 is `CCharString::~CCharString` (manifest
  "_Cons_val<...CParticleMorphs::CEntry...>").
- 0x0099E5A0 is `CCharString::Find(char)` (manifest "FindCharPos"). 0x009B9870 is `CParsedItem::GetSymbol` / `GetString`
  (one body; manifest "CModeBlock::GetAnimFilter").
- 0x009974A0 has one caller (0x00997FDA, inside GetCurrentPath), so "AddManualCameraEvent" has no basis.
