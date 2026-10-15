# Hand chunk 147 (CMemoryAllocatorVariableSize .. CModeDazed): 34 of 40 byte-exact

## What matched (34)
- **CMemoryAllocatorVariableSize / CMemoryAreaChunk** (PDB layouts: allocator base 0xc1c, chunk 52 bytes, area 32 bytes; alignment globals 0x013D28D4/0x013D28D8, `ALIGNED_SIZE(n) = ((n + align - 1) >> shift) << shift`):
  - 00a64e70 InsertIntoTree -- red-black insert keyed (DataSize, address), a bool side flag per step, tail InsertFixup duplicated into three arms.
  - 00a65260 GetAllAllocatedAreas -- nested chunk/area walk; the globals are re-read after each array store (aliasing).
  - 00a65830 NewChunkDemandPaged -- the area size must be a local computed BEFORE the `DemandPaged = true` byte store (a char store may alias the globals, so the header size is recomputed for the pointer argument).
  - 00a65930 ctor -- every member an initialiser: argument-valued ones are stored before the vtable store, the zero ones (one shared register) after it.
  - 00a65060 CMemoryAreaChunk::RemoveAreaFromFreeList, 00a65410 CMemoryAreaChunk::Init -- straight PDB-field code.
- **CMemoryDataInputStream / CMemoryDataOutputStream**:
  - 00994960 ReadNullTerminatedWString -- wide twin of the landed 0x00995D30; GetPosition() must be a named local.
  - 00995ee0 ctor from a CMemoryDataOutputStream -- retail evaluates GetData()/size()/size() BEFORE the CBaseClassNonCopyable call: one `__declspec(novtable)` intermediate base with an inline (data, size, sizeCopy) ctor over both bases; StreamSource is a body assignment.
  - 00996cc0 Write -- 64-bit range check, `resize(n, char())` with the fill temporary in the dead size slot, inline memcpy.
- **CMemoryFile**: 009e9420 OpenFromFile -- CWideString returned into the dead `file` slot, size rounded to the block size (unsigned div/imul).
- **CMenuEntry**: 0053dfe0 ctor -- CClickable base with three polymorphic subobjects (+0, +4, +0x18), init-list zeros, InitializeRumbleActions in the body.
- **CTCBoastUI** (catalog CMesh): 0056ac10 ctor -- inline STLport `_Rb_tree` ctor (header proxy zeroed, header node malloc'd, red, leftmost/rightmost = header), vector zeros, flag bytes, AddToGlobalUpdateTCs.
- **CMeshBankManager**: 00a0a150 dtor -- per-node `CCountedPointer = 0` (release + block/object zero), list member dtor (out-of-line clear + free sentinel), tail jump to ~CBase.
- **CMeshDataBank**: 00a24050 SetAnimationMemoryPool / 00a24150 SetMeshDataMemoryPool -- CCountedPointer by value (operator= with re-read release; the parameter's dtor via a local block copy gives `dec ; jne`); 00a24330 NotifyEntryRemoved -- protected virtual, STLport `*(begin()+n)`.
- **CShaderDataBank** (catalog CMeshDataBank): 00a62050 CloseShaders -- indexed delete loop + SGI erase(begin(), end()) with the __copy_trivial `first == last` shortcut.
- **CMessageEventManager**: 0090e090 FindMostRecentMessage<Bind3<...>> -- landed template + inlined ThingSearchFilter; the "all five bits set" test is `!(~flags & 0x1f)` (`not ; test al,1Fh`).
- **SHandleAndTextLists** (catalog CMiniMapDisplay::ClearPrimitiveHandles): 004ae0e0 -- out-of-line dtor of a non-polymorphic record with two STLport vectors (explicit virtual element dtor `push 0 ; call [eax]`).
- **Creature modes** (CAnimFilterOfGroup family): 0073eac0 CModeBase ctor, 00741730 / 008bd7f0 CModeChargeForcePush, 008d7d60 CModeCreatureIsProjectile, 008d7de0 CModeRockTrollVulnerable, 008d82b0 CModeScorpionKingExhausted, 008c3700 CModeBriarRoseTeleportIntangible, 008ac3f0 CModeScreamerSuperAttack, 00740160 CModeStunned, 00740250 CModeDazed, 0073fdb0 CModeCrouch -- the CRC arguments of the inline CAnimFilterOfGroup ctor must come from the CCharString `GetPString()`/`GetLength()` ternary accessors (the landed 0x0073E8F0 form); the merged `if/else` form only matches when the next statement uses a parameter register and flips the register choice when constants or calls follow. CModeOutOfActionBase is byte-packed (`#pragma pack(1)`, attacker pointer at +0x6d). CModeCrouch: CAnimFilterOf2Groups inline ctor with two implicit CCharString temporaries (second in the `push ecx` word, first in the dead argument slot).
- **CModeConversation**: 008d9220 / 008d92c0 -- out-parameter GetInterface on ParentThing (interface 15, CTCTalk), PDB fields CurrentAttitude/InitialAttitude/Filter/ResetIdleAnim.
- **CModeBase**: 0073ac50 FrameUpdate -- `HasTC` returns the masked word (a bool narrows to `shr/and`); `T* p = 0; if (HasTC(k)) p = GetTC(k);`.
- **CModeBriarRoseTeleportInvulnerable**: 008e7850 OnDeactivate -- base call, state-byte check, RemoveTC, SetAlpha, IS_THING_ALIVE + Kill.

## Parked for Fable 5.1 (6)
- 00a659c0 CMemoryAllocatorVariableSize::Allocate DIFFER(140v140): retail reloads Alignment (+0xc14) into edx before AlignmentShift (+0xc18) into ecx after the global stores; ours loads ecx first | tried: inline AllocateArea (in-class: VC inlines the first call site only, exactly retail's split), local alignment, operand orders, SetCurrentAlignment helper | next: lane gap: scheduling-only
- 009e90c0 CMemoryFile::SetPosition DIFFER(112v118): retail `sub esp,8` (message word + separate thrown-int word) and `mov edx,[ecx+0Ch] ; cmp eax,edx`; ours `push ecx`, throws from the dead argument slot, compares against memory | tried: `throw 11`, named long, CFileError struct, error code declared first, inline thrower | next: a thrown class object with a user copy ctor / the message declared at function scope
- 009e99c0 CMemoryFile::CMemoryFile(const CMemoryFile&) DIFFER(114v112): retail puts the __copy result temporary in the same dead `other` slot as the empty iterator tag; ours gives it its own frame word (push ecx/pop ecx, all offsets +4) | tried: inline copy(), STLport __iterator_category/__distance_type helpers, direct __copy | next: lane gap: two call temporaries in one dead slot (chunk 132)
- 0053d4b0 NUISystem::CMenuEntry::Update DIFFER(135v139): frame swap (retail counted-pointer temp at [esp+8], wasVisible at [esp+0Fh]) and the inline release `mov edx,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edx ; test edx,edx ; mov [eax],edx` vs ours `dec [eax+4]` | tried: `--count`, pointer-to-count, split statements, volatile count, local declaration order | next: the same intrusive release in the asm-baked 0x0041AB00 / 0x0055AF60 / 0x00666D10 -- solve the shape once for all four
- 006178f0 CMessageEventManager::FindAllMessages<CFilter_Bind2<IsOfType,OccuredBetween>> DIFFER(126v127): `push ebx ; push esi` deferred past the empty-list test in ours (retail saves all four registers at entry); all other instructions match | tried: pointer loop, iterator struct, optimize("s") | next: lane gap: register-save placement (shrink-wrapping)
- 006af3f0 CMessageEventFilter_IsMyDeadLeader::operator() DIFFER(160v160): retail stores the second GetPItem() into the dead argument slot followed by a never-taken `xor al,al ; jne`; zeroes the CDefPointer slot after the GetDef pushes; release in ecx | tried: plain flow, volatile by-value helper, __forceinline release + member GetDefinitionManager | next: find the compiled-out inline (an ASSERT-style helper taking the thing by value) and a CDefPointer whose ctor VC schedules after the pushes

## Skipped
None (no row holds two functions or a catch funclet).

## Label corrections
- 0x008D7DE0 is NCreatureMode::CModeRockTrollVulnerable::CModeRockTrollVulnerable (vtable 0x01285774), not CModeCreatureIsProjectile.
- 0x008D82B0 is NCreatureMode::CModeScorpionKingExhausted::CModeScorpionKingExhausted (vtable 0x012858A0), not CModeCreatureIsProjectile.
- 0x008AC3F0 is NCreatureMode::CModeScreamerSuperAttack::CModeScreamerSuperAttack (vtable 0x01278338), not CModeBriarRoseTeleportInvulnerable.
- 0x00740160 is NCreatureMode::CModeStunned::CModeStunned (vtable 0x01267C74), not CModeDazed (0x00740250 is the real CModeDazed).
- 0x0056AC10 is CTCBoastUI::CTCBoastUI (vtable 0x0124DCFC), not CMesh::CMesh.
- 0x00A24150 is CMeshDataBank::SetMeshDataMemoryPool (writes +0x370 / attaches MeshResourceBank), not SetAnimationMemoryPool.
- 0x00A62050 is CShaderDataBank::CloseShaders (called by the landed CShaderDataBank deleting dtor 0x00A62860), not CMeshDataBank::FreeDeviceSpecificResources.
- 0x004AE0E0 is the out-of-line destructor of a non-polymorphic stack record (two vectors; callers 0x006E5990/0x006E6280/0x006E6DD0), not CMiniMapDisplay::ClearPrimitiveHandles.
- 0x006178F0 is FindAllMessages<Bind2<IsOfType, OccuredBetween>> (collects into a list and returns its size), not FindMostRecentMessage<Bind3<...>>.
- Callees: 0x00861E00 is the shared creature-mode base ctor (manifest says CTCCreatureNavigation), 0x00686800 is CTCBase::CTCBase (manifest CEngineSubPrimitive2DClockSprite), 0x00686ED0 is a CTCBase member that registers for global updates (manifest std::list::push_back), 0x00A0A720 is the bank list's clear() (manifest remove_if), 0x0073DFF0 is CAnimFilterOfGroup's group setter, 0x00A656E0 is CMemoryAllocatorVariableSize::AllocateArea, 0x0099B240 is CWideString's byte size.
- Engine headers off by 4 in retail: CMemoryDataOutputStream (position +0x14, not +0x18), CMemoryFile (Opened +0x1c, Pathname +0x20), CTCBoastUI (STLport containers 12 bytes), CMeshDataBank (CArray 12 bytes; banks/pools 0x74 below the PDB).
