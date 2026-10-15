# Hand chunk 113 (CThingTrackNode .. CUIMiniMapMarker): 38 of 40 byte-exact

## What matched (38)
- **Deleting destructors labelled "CThingTrackNode::vector_deleting_destructor" (13, labels propagated; real classes from the vtable slot 0 in rtti-vtables.tsv)** --
  007874c0 CTCSkeletalMorph, 007b06c0 CTCFishingSpot, 007e0db0 CTCEffectOnDie (CTCBase base 0x00686830), 00838500 CThingTrackNode (genuine),
  00996f30 CFileDataOutputStream, 009971a0 CFileDataInputStream, 009a0b30 CFontBank, 00a03aa0 CHandleBasedMemoryPool, 00a63b30 CFontDataBank,
  00aa8d80 CChunkedFileRawDataChunk, 00aacea0 CSoundEmitterLH, 00b0f990 NParticleEngine::CPSCUpdateNormal (CBase 0x0099A300 / 0x0099A2E0 root), plus
  00991450 CThreadedFile. Shape: polymorphic chain with an out-of-line root destructor, `__forceinline virtual ~T()` + a `new T` factory
  (Leaf `??_G<Class>`); members with out-of-line destructors die in reverse order; an intermediate base's vtable store = an inline empty
  `virtual ~CDataStream() {}` / `~CChunkedFileChunk() {}`; a polymorphic MEMBER with an inline destructor keeps its vtable store
  (CFontDataBank +0x110); a destructor body calling a virtual (`Close()`) binds it statically; CThingTrackNode frees through the thing
  class's `static void __fastcall operator delete(void*)` (`mov ecx,esi ; call`); array members whose destructor is `DestroyElements();
  if (data) free(data);` and the inlined `Close()` need `__forceinline` (else called out of line once).
- **CThingTrackNode** -- 00836da0 PeekTrackStartNode: `CThingTrackNode* node = this; while (node) { if (node->IsStart) return node; node =
  node->PreviousNode; }` reproduces the rotated loop with the entry null test (layout as the landed ??1 0x00836D00: IsStart +0xec,
  previous link +0xd4). 00837060 DrawGetDrawSizeX: the dead `add eax,0E6h` next to `fmul [eax+0EEh]` is a NAMED POINTER local to the
  packed record at +0xe6 (`CMeshInfoSize* size = &GetMeshBank()->m_size; *out = size->m_x * m_scale;`); the direct chain folds the add.
- **CThreadMessage** -- 00cb0e30 Send: one `&m_lock` in edi for both unlocks, E_FAIL early return when no worker thread, reply read into a
  local before the unlock; dllimport Win32 calls. 00cb0eb0 TryReceive: int (not bool) result, zero-timeout wait, optional out pointer.
- **CThreadedFile / CThreadedFileManager** -- 0098ec40 Close (void; the two flag clears share `xor al,al`, OpenFlag first).
  0098e770 RemoveObjectFromPendingMap: device row `DeviceID*7` in a long local; `CPendingIterator& next = DeviceQueue[d][p].NextObject;
  if (next.m_node == DeviceQueue[d][p].Objects.find(key)) ++next;` gives retail's shared `this+index*16` base (`lea ecx,[eax+4]` /
  `lea ebp,[eax+10h]`); find returns the node pointer in eax; the erase indexes again; the key is one local homed in the dead argument slot.
  00485468 / 0053c7ef (catalog "FinishAndGetFileHandle", propagated) are STLport `vector<T*>::erase(iterator)` and
  `vector<unsigned char>::erase(iterator)`: `if (pos + 1 != end()) __copy_trivial(pos + 1, _M_finish, pos); --_M_finish; return pos;`
  with the `__fastcall` trivial copy 0x00405BA0, emitted through an explicit template instantiation (Leaf `erase`).
- **CTimer / CCounter** -- 00cd4450 `CTimer() : TimerIndex(g_pThingManager->CreateTimer())` (slot 87); 00cd4490 is the CCounter
  constructor (slot 653 GetWorldFrame, the slot the landed CCounter members read).
- **CTools** -- 0069bf30 GetRollingAverage is the C2DVector instantiation: `(average * count + value) / (count + 1.0f)` with inline
  C2DVector operators (user constructor -> hidden result, `operator/` multiplies by one reciprocal), `this` unused (`ret 10h`).
  00c62150 GetWorld: static `__stdcall` taking a reference, a five-hop pointer chain.
- **C3DVector** -- 00c256f0 / 00c25730 (catalog "CTriangleInfo::IsIdentical", propagated): `int __fastcall operator==` / `operator!=`
  members; `&&`/`||` chains of `x == other.x` (right operand loaded first), int results.
- **CVtable_012b1a54** -- 00c08750 (catalog "CTreeElement::CTreeElement", propagated): three-argument constructor of the no-RTTI class of
  the landed 0x00C08720; plain init list in declaration order.
- **CTrophyDef** -- 004d36fe constructor: `#pragma optimize("s", on)` (`or [esi+3Ch],-1` from CDefString's inline `TablePos(-1)`),
  seven CCharString defaults. 004d7fad destructor: the landed def-destructor family (novtable most-derived + novtable CSubDefClassBase,
  CDefPointeeBase inline destructor store, tail jump to 0x009FC550).
- **CDrawMapInfo** -- 0050b490 (catalog "CTurncoatDef::operator=", propagated): memberwise copy constructor (string copy ctor
  0x0099EC30), Pos/Extents written as separate longs.
- **CTyler** -- 00e83620 OnPredicateFail: nested `m_pGSI->SetFightTarget(Me, m_pGSI->GetHero())` (outer vtable in ebx before the inner
  call), flag test then tail jump to CleanupAfterFight or release. 00e83680 CleanupAfterFight: release, flag clear, thing slot 67(true),
  interface slot 452.
- **COpinionDeedEffectsDef** -- 00456d69 (catalog "CUILocaleGraphicsDef", propagated): list(const allocator&) with the empty
  allocator temporary in the high byte of the `push ecx` slot (`lea eax,[esp+7]`), the STLport vectors' inline zeroing after it.
- **CQuestDesc / CUIMiniMapMarker / pair** -- 0047bf90 is CQuestDesc() (999, "No description yet", status 0, empty vector);
  005fa530 CUIMiniMapMarker() (float zeros as BODY assignments of `0` share the integer zero register); 00612980 the marker's
  memberwise copy constructor; 004f9784 is `pair<const long, record>::pair(const pair&)` of a map node value: `#pragma
  optimize("s", on)` + a `__forceinline` memberwise copy constructor of the record (12-byte STLport tree + two words).

## Parked for Fable 5.1 (1)
- 00c9cb40 CTopologySizeGrid::Hora DIFFER(18v18): retail reads the grid from eax (`mov ecx,[eax+10h] ; imul ecx,[esp+4] ; add ecx,edx ; mov edx,[eax] ; mov eax,[edx+ecx*4]`), ours from ecx | tried: `__fastcall Hora(grid, column, row)` | next: lane gap: LTCG custom calling convention (object passed in eax; all six callers 0x00C9D553.. are in the same module)

## Skipped
- 00837040 CThingTrackNode::vfunc_27: the 32-byte row holds two functions (0x00837040 `GetDisplayEngine()->slot 9` tail jump, int3 padding,
  then the 0x00837050 getter `return GetMeshBank()->[+0xe6]`).

## Label corrections
- 007874c0 -> CTCSkeletalMorph::`scalar deleting destructor' (vtable 0x0126B6F4); 007b06c0 -> CTCFishingSpot (0x0126D51C);
  007e0db0 -> CTCEffectOnDie (0x0126FEB4); 00996f30 -> CFileDataOutputStream (0x0129A6F8); 009971a0 -> CFileDataInputStream (0x0129A728);
  009a0b30 -> CFontBank (0x0129AB38); 00a03aa0 -> CHandleBasedMemoryPool (0x0129C964); 00a63b30 -> CFontDataBank (0x0129DA84);
  00aa8d80 -> CChunkedFileRawDataChunk (0x0129E4B8); 00aacea0 -> CSoundEmitterLH (0x0129E6EC); 00b0f990 -> NParticleEngine::CPSCUpdateNormal
  (0x0129FC1C) -- all labelled "CThingTrackNode::vector_deleting_destructor".
- 00485468 / 0053c7ef "CThreadedFile::FinishAndGetFileHandle" -> std::vector<T*>::erase / std::vector<unsigned char>::erase.
- 00cd4490 "CTimer::CTimer" -> CCounter::CCounter.
- 0069bf30 "CTools::GetRollingAverage<C3DVector>" -> GetRollingAverage<C2DVector> (the C3DVector one is 0x0084EAF0).
- 00c08750 "CTreeElement::CTreeElement" -> CVtable_012b1a54::CVtable_012b1a54(void*, long, long).
- 00c256f0 / 00c25730 "CTriangleInfo::IsIdentical" -> C3DVector::operator== / operator!=.
- 0050b490 "CTurncoatDef::operator=" -> CDrawMapInfo::CDrawMapInfo(const CDrawMapInfo&).
- 00456d69 "CUILocaleGraphicsDef::CUILocaleGraphicsDef" -> COpinionDeedEffectsDef::COpinionDeedEffectsDef.
- 0047bf90 "CUIMiniMapMarker::CUIMiniMapMarker" -> CQuestDesc::CQuestDesc; 004f9784 -> std::pair<const long, record>::pair(const pair&).
- Callee labels seen propagated: 0x00686830 "~CChunkedFileChunk" (= CTCBase::~CTCBase), 0x0099EAE0 "_Cons_val<...>" (= ~CCharString),
  0x00787340 "CCombatWheel::ResetRings" (a CArray destructor), 0x004C73B0 "operator_new" (= the thing class operator delete),
  0x009CE180 "NHeroInformationScreens::CBase::~CBase" (= CBankFile::~CBankFile), 0x0098F9D0 "BinaryTree_RotateOrRebalance" (= map iterator ++),
  0x00405BA0 "Std_Move_Backward" (= __copy_trivial).
