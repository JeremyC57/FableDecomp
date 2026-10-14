# 2026-10-14_linux_hand53 — hand-authored chunk 53 of the class-labelled 17–96-byte pool

Chunk 53 (`mkchunk.py 53`, 40 rows, CLandscapeBackgroundLoadTask .. CLandscapeGPUUpdateQueue, mostly deleting destructors with propagated
labels) was authored by the session coordinator on the Linux VC7.1 lane; the lander re-graded every row: **38 of 40** landed as
MATCH/RELOCATION_MATCH with behaviour tests (compiled with the lane's cl.exe). Two are parked in the lane's `differ/`. The lane notes follow.


Linux VC7.1 lane (Wine + DDK cl.exe), authored by the coordinator. Chunk 53 (`chunk53.tsv`, 40 rows): **38 of 40** graded
`RELOCATION_MATCH`/`MATCH` with behaviour tests (g++ and the lander's cl.exe); two parked. Sources from `gen.py`, tests from
`gen_tests.py`, `payload.json` / `oracle.tsv` from `finish.py`.

## The chunk's shape
Thirty rows are deleting destructors with propagated labels ("CLandscapeBackgroundPatch::`vector_deleting_destructor'",
"CLandscapeBackgroundSubNodeSplitBranch::`scalar_deleting_destructor'"): each is some class's `??_G` (flag bit 0 = free).
Three kinds, all reproduced:
- **No vtable store at all** (0x004A90F0, 0x0059271D, 0x005BC277, 0x006FB430, 0x009A9960, 0x00BF8C10): a NON-polymorphic class deleted with
  `delete`; VC7.1 emits `??_G<Class>@@QAEPAXI@Z` for it (probe: `struct SA { CCharString a, b; }; void Kill(SA* p) { delete p; }` emits
  `??_GSA@@QAEPAXI@Z`). Leaf `??_G<Class>@@QAEPAXI@Z`, forced by a `delete` helper.
- **Only the base's vtable stored** (0x0073E190, 0x00ABF0C0, 0x007441A0, 0x0092E3E0, 0x00BF8D10, 0x00AD88C0): the derived vtable folds into the
  base's (identical COMDAT folding, no RTTI difference), so the derived class is `__declspec(novtable)` with a NON-virtual destructor over a
  base whose inline destructor stores the base vtable; `delete` emits the `QAE` `??_G`. When the base chain has an out-of-line destructor two
  levels down (C3DOptimisedMeshBase : CBaseClassNonCopyable), the novtable-derived form loses the base's vtable store: make the base the FIRST
  MEMBER of a non-polymorphic class instead (0x00AD88C0).
- **Own vtable store** (the rest): a virtual destructor (`__forceinline`) with a `new T` factory to pull the vtable (and so `??_G`, leaf
  `??_G<Class>@@UAEPAXI@Z`; namespaced classes mangle as `??_GCObserver@NUISystem@@UAEPAXI@Z`).
- A class freed through the landscape memory manager (`mov edx,[01436EA8h] ; mov ecx,[edx+6B0h] ; push esi ; call FreeVirtual`) has its own
  `static void operator delete(void*)`; `delete m_pLoadTask` with that class's inline destructor reproduces the inlined release + free
  in CLandscapeBackgroundTreeNode's destructor (0x00BF73A0).
- An unoptimised TU (`mov [ebp-4],ecx` spills of `this`): `#pragma optimize("", off)` (0x00A44A40, CSuspendableProcessMicrothread).

## Label corrections
- The vtables name the classes: 0x01246020 NUISystem::CObserver, 0x012673CC CTCCreatureModeManager::CModeBase (with a CAnimFilterBase member at
  +8 and a name at +0xc), 0x0129C3E4 CGenericVar, 0x0129C840 CVolume (ReleaseVolume(1) then CBase::~), 0x0129C978 CRenderStateManager (clears
  its singleton 0x013CB508), 0x01241D04 CObstacleNavigatorExternalInterface over CNavigatorExternalInterface, 0x01264A8C CGameCameraBase over
  CAIGameCameraBase (two 4-byte lists at +0xa8/+0xac, their `clear` labelled "SortTreeRecursively"), 0x01275974 / 0x01276AEC two CPhysicsLeaf
  classes over NCameraModes::CSubComponentBase (a raw buffer at +0x10, an async-data member at +0xc discarded by 0x00850530 `__fastcall`),
  0x0129EF2C CIndexBuffer over CMovableResource (Release @ 0x00AC1EE0, labelled "CVolume::ReleaseVolume"), 0x012B8FC8 / 0x012B90D4 / 0x012BF6BC /
  0x012EA430 no-RTTI classes (a critical section, a Win32 handle, a COM-style `__stdcall` Release through the owner's +0xc interface, the
  chunk-51 script-thing functor as a member).
- "ClearTextures" covers nine unrelated bodies: two landscape-block frees (0x00BDA0D0, labelled "LandscapeLayerList::InsertNode"), an
  error-message clear plus a global flag (0x0143BB5C / 0x0143BC28), four CloseHandle bodies (two with a -1 sentinel, one of them a
  destructor), a timeKillEvent + critical-section reset.
- CLandscapeBackgroundSubNodeSplitBranch (vtable 0x012A8258) holds two embedded CLandscapeBackgroundTreeNode (0xe8 bytes: patch pointer +0xa0,
  tesselated byte +0xe3, a load task at +0x98) at +4 and +0xec; UpdateTesselation / InvalidateEdge / the constructor forward to both.
- CLandscapeBackgroundLoadTask is 16 bytes: the owned request object (+0, deleted through its slot 0), the file-block manager (+4), the
  request (+8), the issued flag (+0xe).
- CLandscapeGPUUpdateQueue::AddTask builds a one-element retail vector from a malloc'd word and calls the multi-task add (0x00525CD0,
  labelled "CThingManager::OnLevelsUnload").

## Parked (2; sources in differ/)
- 0x00525D90 `AddTask`: register choice only — retail keeps `this` in edi and the malloc'd word in esi (pushed to `free` from the register),
  the lane keeps `this` in esi and reloads the word from the vector's frame slot (74 v 74; a separate `word` local changes the frame order,
  not the registers).
- 0x00E7DFE0 `??_G CVtable_012ea430`: retail stores the derived vtable, computes the member address, stores the +0x14 member's vtable; the lane
  computes the `lea` first (52 v 52, the relocation offsets shift).
