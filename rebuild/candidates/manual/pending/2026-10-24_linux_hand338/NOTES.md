# Hand chunk 338 (LAddress .. Mass): 36 of 40 byte-exact

## What matched (36)
- **LAddress** -- 00841f60 `operator=`: memberwise copy. The string assign goes through `CRetailString::_M_assign_dispatch(first, last, const __false_type&)` @0x569140 behind a self-check. The tag needs a user ctor so the temporary costs no store.
- **LHNetworkLib::LHHttpDocumentParts / LHHttp2** (retail offsets, 12-byte STLport strings and vectors; DocumentParts is at +0x884):
  - 0083ea10 ~LHHttpDocumentParts: `delete[]`, zeroes the pointer and length.
  - 0083ea70 GetDocumentSize and 0083eaa0 GetDocument (intrinsic memcpy, returns 1000): pointer loops over the vector.
  - 0083eb00 ResetDocumentParts: `delete *it`, then an inline clear() through the `__copy_trivial` (CopyTrivial) form.
  - 0083f980 ResetRequest: `PreparedRequest = ""` through `CRetailString::Assign` @0x5693B0, then four zero stores.
  - 00840070 Close: `Host->Close(); delete Host; Host = 0; Reset(); return 0;`.
- **CSlotRequest_00c38cd0::Setup** (00c38cd0; descriptive placeholder name, class unknown): two Range.Set calls, two Start calls, then `m_slotA = m_slotC = m_slotB = -1` (a chained store emits the rightmost first) and `m_active = 1`.
- **CBasicString<char> / CCharStringData** -- 009a0190 and 009a0430 are byte twins: an explicit-instantiated, packed 13-byte CBasicString dtor; CCharStringData has an empty user dtor.
- **LSocket** (retail 0x9C layout):
  - 008f96e0 GetClientInformation.
  - 008f8c70 Close.
  - 008f8910 Bind: `return x != -1 ? true : false` drops a setne/mov pair.
  - 008f8370 CheckReadSize.
  - 008f82e0 IsWriteable: the winsock FD_ZERO/FD_SET macros, plus __WSAFDIsSet as a direct-call stub.
  - 008f8210 IsReadable: a `__forceinline SelectRead(timeval*)`, inlined twice.
  - 008f8440 TryToSendDataFromQueue: retail calls `erase(begin()+sent, begin()+sent)` with the same bound twice.
  - 008f8870 SetNonBlocking and 008f8ea0 Accept: /GS; the error arm uses a `__forceinline` string ctor/dtor; Accept's not-listening arm is the early return.
- **LSocketServer**:
  - 0090b080 RequestListener: a `bool result` variable gives the ebx-held constant; case 2 sets it true before Bind.
  - 0090b450 CheckForAcceptedConnectionsOnSocket: a static `__fastcall Think(bool)`, then `return !list.empty()`.
- **CDefinitionManager::GetNumberOfDefsOfType** (009ad3b0): calls `GetClassInfo(name)` @0x9AD2E0, then `Instances.size()`.
- **STLport `_Rb_tree::_M_erase` instances** (`CRetailTree_erase<addr>::Erase(node*)`): 0056de50, 0070adb0, 00b2ef20, 00bd8a10, 0056d440, 0098d120, 009aa7c0, 009d3e80. The body is: recurse right, destroy the value (the member dtors in reverse), free the node, then continue left.
- **boost::checked_delete<CScriptConversationManager>** (004ae810): `__fastcall` (YI in FableWin.pdb) and explicit-instantiated; the map's inline dtor clears it and then frees the head.
- **CBulletTimeManager::PrepareForLoad** (004c6080): three field stores, then an inline map clear() (Erase @0x4AA650).
- **DestroyRange_Tree_0051ac70** (0051ac70): a `__fastcall` range destroy of 12-byte trees. The tree's Erase is inline and directly recursive; VC7.1 expands it exactly once, which reproduces retail's one-level unrolled call to 0x4A9CB0.
- **~CEngineInternalPrimitiveAnimatedMesh** (00bd9250): the implicit (compiler-generated) dtor. It stores no vtable. It is pulled into the TU through a `new` factory with an inline ctor.
- **CWeaponTrailListRecord copy ctor** (00864f80): `Entries(other.Entries), Extra(other.Extra)`, reusing the landed CWeaponTrailList model (InsertDispatch @0x76FD40).
- **Mass::GetVel** (008eea30, MATCH): `Vel + C2DVector(r.Y, -r.X) * AVel`, with a C2DVector that has a user copy ctor.

## Parked for Fable 5.1 (4)
- 009bf5a0 CDisplayManager::RemoveDeviceResetCallback DIFFER(57v59): ours pushes esi after the empty-list je and pops it before the end test (`pop ecx` after free); retail saves esi at entry and restores it at the end with `add esp,4` | tried: a Find helper, STLport iterator + __find with a tag, a for/break loop | next: model the list as the real STLport `_List_base` with `_M_node` in an alloc proxy, and the erase as `_List_global::_Transfer`-style unlink
- 006e77f0 CGameScriptInterface::GetTextIDFromSymbol DIFFER(42v40): ours `mov esi,eax ; add esi,934h`, retail `lea esi,[eax+934h]` after PeekGuiDef | tried: reference, pointer, inline accessor, /Os | next: a named `CPlayerGuiDef* guiDef = PeekGuiDef();` local, then `guiDef->ScriptSprites.find(...)` and `guiDef->ScriptSprites.end()` (the landed 005beb57 shape)
- 008eea80 Mass::ApplyImpulse DIFFER(85v85): in the dot-product X term retail does `fxch st(2) ; fmul [impulse.X] ; faddp st(2) ; fxch st(1)`, ours `fld [impulse.X] ; fmul st(3) ; faddp st(1)` plus an extra `fstp st(0)` | tried: operand swap, 1..3 unused locals (5 grades in all) | next: a different C2DVector dot model (a free `operator*` or `Dot()` member, with the operands by value), shared with 008eeb40
- 008eeb40 Mass::ForceToVel DIFFER(144v146): retail forms both `point - Pos` lever arms (the second spilled to [esp+4] / point.Y) before the dot product and multiplies by force.X from memory; ours takes the dot right after the first arm (`fld [force.X] ; fmul st(3)`) | tried: torque before the second arm, both arms before the torque, a named perp local, one expression | next: the same dot-model change as 008eea80, plus a `Perp()` member returning C2DVector

## Skipped
- none. Bounds: no row is cut off or holds a second function. Several chunk lengths include the trailing int3 padding (for example 008eeb40 is 160 in the chunk but the body is 146; 0051ac70 is 128 vs 114), and the grader trims it.

## Label corrections
- 0x009A0190 / 0x009A0430: `CBasicString<char>::~CBasicString` and `CCharStringData::~CCharStringData` (byte twins), not LHHttpDocumentParts::~LHHttpDocumentParts.
- 0x00C38CD0: not LHHttp2::ResetRequest. It sets up a 0x180-byte slot record; caller 0x00C2D260; the class is unidentified.
- 0x009BF5A0: CDisplayManager::RemoveDeviceResetCallback, not LSocketServer::CloseSocket.
- 0x006E77F0: CGameScriptInterface::GetTextIDFromSymbol (it reads CPlayerGuiDef::ScriptSprites +0x934), not LTextSystem::GetIDFromSymbol.
- 0x009AD3B0: CDefinitionManager::GetNumberOfDefsOfType, not LTextSystem::GetIDFromSymbol.
- 0x0056D440, 0x0056DE50, 0x0070ADB0, 0x0098D120, 0x009AA7C0, 0x009D3E80, 0x00B2EF20, 0x00BD8A10: STLport `_Rb_tree::_M_erase` instances, not LTextTreeWalkThrough::BuildTreeArray.
- 0x004AE810: boost::checked_delete<CScriptConversationManager>, not LTextTreeWalkThrough::Destroy.
- 0x004C6080: CBulletTimeManager::PrepareForLoad, not LTextTreeWalkThrough::PrepareForLoad.
- 0x0051AC70: a range destroy of 12-byte STLport trees (`_Destroy_Range`), not LTextTreeWalkThrough::Destructor_DeleteNodes.
- 0x00BD9250: CEngineInternalPrimitiveAnimatedMesh::~CEngineInternalPrimitiveAnimatedMesh, not LTextSymbol::~LTextSymbol.
- 0x00864F80: the copy ctor of an 8-byte {list, dword} record (CWeaponTrailListRecord), not LinkedList::Init.
