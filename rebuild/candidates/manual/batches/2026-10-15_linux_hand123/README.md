# Hand chunk 123 (CAnimComponentStrikeResponseAnim .. CBenTestScriptsScript): 31 of 40 byte-exact

All 31 sources in `src/` grade MATCH / RELOCATION_MATCH (`grades.txt`) and their tests print PASS under
g++ (`tall.sh`) and the lane's VC7.1 (`tall.py`). Nine rows are parked in `differ/`; none skipped.

## What matched (31)

- **CAnimComponentStrikeResponseAnim / CAnimComponentStrikeSpeedMultiplier** (005dbf50, 005dbee0, 005dca60):
  binary persist reads through the inlined `CDataInputStream::Read` of the landed TransferBinaryIn family
  (64-bit position range check, chunk memcpy, else the overflow reader). The string row needed the
  length read through an inner `CPersistTraits<unsigned long>::TransferBinaryIn`-style helper: its own
  `value` local takes the dead argument slot and the outer length stays in edi, which frees the slot for
  the ReadString temporary. `Initialise`: reset through virtual slot 8, a `CStringParser(text, 0,
  CCharString("", -1), 0)` local (0x28 bytes retail) and `CPersistTraits<float>().TransferIn(...)`
  whose empty temporary (user default ctor) is homed in the dead `text` slot.
- **CEngineCamera** (00b31700, catalog CAnimInit::Clear): constructor; embedded desc / debug-script
  array with inline ctors, scalar defaults as body assignments, a flag-guarded local `vector<long>` filled
  by eight `push_back(3)` (constant re-stored per iteration in the dead flag slot).
- **CAnimSet::Update** (0070d030): dense switch with jump table; the percentage arm is an inline setter
  fed by a float-returning inline getter (x87 copy of the new value, integer copy of the old one).
- **CAnimateThing** (0070c100, 0070e6a0): two flag scans over the transition/combination anim-set lists
  and a one/two-anim test (`count == alphas.size()` operand order fixes `cmp eax,edx`); two intrusive
  counted-pointer assignments after UpdateCachedAnims.
- **CAnimatedBlock::SaveBinary** (00a8e770): the landed CStaticBlock twin plus the virtual raw Write.
- **C3DAnimationInfo::CAnimationData::CanUnload** (00a25a70): the no-read-pending case as the `if` body,
  `IsFinished() ? true : false` for `test al,al ; setne al`, map end() in a local (head kept in esi).
- **CAnimationEntry** (005db630, 005dec70): TransferBinaryOut reads both pair fields before the first
  call (component pointer local); the copy+transition constructor default-constructs members, assigns
  `*this = other` through the inline memberwise operator=, then `new`s the transition component
  (count 1 stored by the pointee base's inline ctor, value stored after the null-check join).
- **C3DAnimationInfo::operator=** (00a2b4e0, catalog CAnimationEntry::operator=): memberwise, vptr not
  copied; the C3DVector must be a POD (implicit copy -> the retail `lea` pointer pair).
- **CAnimationSet::IsAppropriateEntryExisting<CIsCombatAnimAppropriateFollowingCombo>** (00738f90):
  predicate by const reference, inlined operator(), its out-pointer local in the dead argument slot, end
  pointer local.
- **CAppearanceDef::Transfer** (00473504): eleven typed context transfers + EnsureSorted; size TU
  (`#pragma optimize("s", on)`) for the all-eax address formation.
- **CArenaHeroGate::Main** (00d7df30): close / wait / open / wait / close with top-tested tick loops.
- **CAudioMixer::CloseAndCleanup** (00c715d0): stack MIXERCONTROLDETAILS restore + mixerClose.
- **NPlayerGui::CAutoPickupBar::AddObject** (00647511): `optimize("y",off)` + `("s",on)`; the time
  assignment written first in both paths gives the retail store order.
- **CBankFile** (009cbe10, 009cc450, 009cd330, 009cea70, 009d2fc0): ConvertPathToFilename (SplitPath on a
  ToWideString temporary built between the stack pushes and the call); GetTotalDataSize (the landed
  EntryExists / GetEntryDataSize inlined); WriteEntryData (catalog MakeWritable: MakeWritable inlined,
  seek, a CFileDataOutputStream local, copy, WriteZeros); EndUpdateEntries (scoped recovery-path local);
  the SGI `__unguarded_linear_insert` of the entry sort written as an explicit `__fastcall` template.
- **CBankFileAsync** (009d5820, 009d66a0): OpenFileReadOnly override (qualified base call, `new
  CThreadedFile` into Reset, Open with the GetBankPathname temporary in the dead slot);
  SetLoadingMemoryFailureHandler (counted pointer by value: inline operator= re-reading the block after
  the decrement + the parameter's inline destructor).
- **CBankFileAsyncData::WaitUntilFinished** (009d5b60): stack lock, shared early-success tail, explicit
  Unlock before the blocking wait.
- **CBankFileEntryUpdateData::SetFilenames** (009cc840): `resize(n, CCharString())` with the temporary in
  the dead slot, `*(begin() + i)` element access on both sides.
- **CCountedResourceMap::~CCountedResourceMap** (00b443b0, catalog ~CBankFileEntryUpdateData): inline map
  destructor (clear + free head) then the counted pointer's inline destructor.
- **CBankFileManager::Close** (009a7990): `while (!empty()) pop_back();` + inline map clear().
- **CCameraModeBase::Init** (0084e110, catalog CBase::Init): float captures from virtual getters, a POD
  CCamera roll (`rep movsd`).
- **CBaseIntelligentPointer::SetPItem** (00a01b90): early-out, landed ClearData inlined, join or create
  the shared record (`new` of a POD: no null check).
- **CBenTestScriptsScript::FadeEntityOut** (00efe9f0): float fade loop with the alpha in the push-ecx
  frame word.

## Parked for Fable 5.1 (9)

- 006e2a50 CTCTalk::SetUpSpeechSample DIFFER(152v152): the two SpeechSample re-reads load into ecx / edx where retail uses eax / ecx (`mov eax,[esi+0Ch] ; push eax` before BeginLoadingEntry, `mov ecx,[esi+0Ch] ; push ecx` before the duration call) | tried: base form, float local for GFGetTime, GetSampleDurationFrames defined above under auto_inline(off) | next: model GetLipSyncBank / the bank lookup differently (by-value helper) or lane gap: register allocation
- 006d7080 CAttackHistoryCombo::IsComboSuccess DIFFER(140v146): retail re-dereferences the history iterator through a copy (`mov edx,edi ; mov edx,[edx+8]` initially, `mov eax,edi ; mov eax,[eax+4] ; mov edx,[eax+8]` on advance) where the build CSEs it into `mov edx,eax` | tried: node pointers, forward const_iterator class, reverse_iterator class | next: by-value iterator helpers (`Value(const_iterator i)`, `PreviousValue(const_iterator i) { --i; return *i; }`) to block the CSE
- 00c564f0 CAudioContext::SetEAXGlobalProperty DIFFER(127v127): same instructions, block placement: retail puts the default converter right after the compare tree (`cmp eax,0A001h ; jmp` into a shared `je`) and the `return 0` / `return -3` blocks last; the build falls through into the -3 block | tried: shared case body (range tree), separate bodies, default first, switch + trailing if | next: lane gap: switch block layout (or the original enum/return-code helper)
- 009a7ca0 CBankFileManager::FindBank DIFFER(146v144): the alias ternary pushes per arm in retail (`je ; add eax,14h ; push eax ; jmp` / `push ebx`, shared copy-ctor call) where the build selects into eax and pushes once; plus an `int3` after the noreturn _CxxThrowException | tried: reference ternary, inline ResolveAlias helper, ternary of copy temporaries (214v144) | next: lane gap: int3 after _CxxThrowException (as 0x00994B10); ternary arm push still open
- 009ccb20 CBankFile::WriteZeros DIFFER(156v156): retail homes the fill-value AND allocator temporaries of the vector fill constructor in the one dead `count` slot (`lea ecx,[esp+18h]` / `lea edx,[esp+1Ch]`, frame 0xC); the build gives one of them a frame byte (frame 0x10, every offset shifted) | tried: allocator with / without user ctor, defaulted fill / allocator arguments | next: lane gap: two const-ref temporaries in one dead argument slot (as 0x005383DD)
- 00c35e30 _unattributed::ActivateCurrentTarget DIFFER(106v110): retail keeps a duplicated presence test (`test eax,eax ; je error ; test eax,eax ; je ok`) that the build folds away | tried: `HasEntries() && (!HasEntries() || valid)`, if-chain helper with `count ? valid != 0 : true` (adds setne) | next: find the original accessor pair (two differently typed views of the same word)
- 00412210 CRetailDeque<T*>::~CRetailDeque DIFFER(150v150): register allocation only (retail: finish copy in edx/edi/ebx/ebp, start walked through ecx; build: ecx/edx/edi/ebx + ebp) | tried: plain call (116v150), inline by-value _Destroy layer, memberwise user copy ctor (146v150), copy ctor over a POD body (150v150) | next: copy order inside the iterator copy ctor / a `const It&` inline layer
- 00c14b50 CSettingsProxy_012b29f8::SetOptionCC DIFFER(66v109): retail keeps the /EHsc frame (fs:[0], state -1 hoisted above the branch) although nothing can throw inside the CProfileLock scope; the lane drops the frame entirely | tried: /EHsc, /EHa (114v109: extra state-0 store) | next: lane gap: EH frame elision around a non-throwing lock scope
- 00c14bc0 CSettingsProxy_012b29f8::SetOptionCD DIFFER(66v109): twin of 00c14b50 (byte +0xCD) | tried: /EHsc, /EHa | next: lane gap: EH frame elision (same as 00c14b50)

## Skipped

None (no row holds two functions).

## Label corrections

- 00b31700 `CAnimInit::Clear` -> `CEngineCamera::CEngineCamera(bool)` (vtable 0x012A12A4; callers 0x00B26614, 0x00B2781A).
- 006e2a50 `CAnimateThing::SetUpAnimation` -> CTCTalk speech-sample setup (touches SpeechSample / SampleDurationFrames /
  MimeStartTime / PSpeechBank; neighbours are CTCTalk rows). Callee 0x00686D70 (`CAnimateThing::GetMeshBank`) is a
  `this`-ignoring global accessor used as the default speech bank.
- 00a2b4e0 `CAnimationEntry::operator=` -> `C3DAnimationInfo::operator=` (only caller: the 40-byte SGI copy 0x00A2BC70).
- 009ccb20 `CBankFile::MakeWritable` -> `CBankFile::WriteZeros(unsigned long)` (MakeWritable is inlined at its top).
- 009cd330 `CBankFile::MakeWritable` -> `CBankFile::WriteEntryData(position, source, size, padding)`.
- 009d2fc0 `CBankFile::InsertionSort_PackedEntry` -> SGI `__unguarded_linear_insert<unsigned long*, unsigned long, CEntryDataOffsetLess>`.
- 00c35e30 `CBankFile::OnInventoryChanged` -> an unidentified render-side `__stdcall` helper (caller 0x00C274C8).
- 00412210 `CBankFileAsyncData::FreeBuffer` -> an SGI-style `deque<T*>` destructor (callers 0x00408A0A, 0x00412870).
- 00c14b50 / 00c14bc0 `CBankFileAsyncData::OnFileReadCompletion` -> boolean setters in slots 1 / 2 of the RTTI-less vtable 0x012B29F8.
- 00b443b0 `CBankFileEntryUpdateData::~CBankFileEntryUpdateData` -> destructor of a {CCountedPointer, map} member (+0x44 of the
  object destroyed at 0x00B42A90).
- 0084e110 `CBase::Init` -> camera-mode Init (callers CGameCameraManager::SetCameraMode / PreloadScriptedCamera).
- 00c564f0 `CAudioContext::EAXSet` -> vtable 0x012B8C18 slot 8, EAXSetGlobal + OpenAL error mapping (the name collides
  with the landed CAudioContext::EAXSet 0x00C558A0).
- Callees: 0x009D2A40 (`Vector_InitFill`) is the byte-vector fill constructor; 0x009964E0 (`CDataInputStream::ReadWithChunking`)
  copies a source stream into an output stream; 0x009D5A30 / 0x009D6420 (`PopLoadErrorQueue` / `vector_push_back_4byte_list`)
  are the memory-failure handler's remove / add bank; 0x00738980 (`GetTCInterface_LowerBound`) is
  CAnimationEntry::GetComponent<CAnimComponentCombatComboChain>; 0x00C438F0 / 0x00C3C4A0 / 0x00C3D090 labels are propagated.
