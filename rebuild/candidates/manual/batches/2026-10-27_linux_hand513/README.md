# Chunk 513 (agent513, 2026-10-28): 32 of 40 re-queued single functions landed (under 128 bytes)

First re-queue chunk: 40 chunk_history orphans under 128 bytes (address order, 0x00407330..0x005E38C0): rows earlier
chunks held that never landed, parked or skipped. Most had been parked on 2026-10-14 under the QFE-4035 lane with notes
only in batch READMEs (hand25/30/31/34/37/38/52/54/57/60). One Opus 5.5 agent at high effort.

- **Landed:** 32 rows with `vland.py --land` (1 MATCH, 0x0055BAE0 NUISystem::CHoverable::OnHovered; 31 RELOCATION_MATCH;
  every test PASS under g++ and VC7.1; no RELOC_IDENTITY, RELOC_CONFLICT or RELOC_DUPLICATE). Def constructors, tree
  `_M_create_node` / `_M_clone_node`, CFrameRateSmoother, the NUISystem CMenuEntry / CEditBox / CList / CDynamicList
  rows, NInventory CItem / CCategory and the CAnimComponentFollowingOnAnim Initialise pair.
- **Parked (8, pending/2026-10-27_linux_hand513/differ/):** 0x0041DB1D CManager::CreateComponent (`push eax ; mov ecx,esi`
  order), 0x0044C6C2 CGameDefinitionManager ctor (`push 80000h` scheduled early), 0x00525D90 CThingManager::OnLevelUnload,
  0x0053848F / 0x005384DD CList::AddActionOnInactive / AddActionOnSelected (dead EH-state store), 0x005BB190
  `_List_base::clear` (retail destroys through `??_G`), 0x005DB5F0 CAnimComponentStrikeResponseAnim::TransferBinaryOut,
  0x005E38C0 CVectorMap<unsigned long, CAnimationEntry>::Insert (single `dec [ecx+4] ; jne` release).
- **Identities:** 29 proven labels written (ghidra_out/labels_chunk513_2026-10-27.tsv); probable: 0x0051C760 (??_G),
  0x005C08E6 / 0x005C0C24 (CItem copy and assignment); descriptive: the inline `ClampToZero` helper in 0x005BF00B.

Label corrections (NOTES.md): 0x0053D0C0 is MakeAvailableDynamically; 0x005C08E6 / 0x005C0C24 are NInventory::CItem copy
constructor / operator= (not CEnginePrimitive2DText::operator=); 0x005BB190 is `_List_base::clear` (not
CNavigatorQueueManager::FrameUpdate); 0x005E38C0 is CVectorMap::Insert (not CAnimationSet::Add); 0x005C13CD is find_if;
0x005C04BF is CCountedPointer<CPerItemData>::ReleaseData; 0x009D5060 is CDefString::SetString; 0x0040E170 is `std::__find`.

Review items: the allocator of 0x004C41D0 is class-keyed (V) while landed 0x004C4890 spells it struct-keyed (U); landed
0x0049B9E0 calls the private CalculateSmoothedTime as public; landed 0x00529230 / 0x00529150 use invented tree stand-ins.

Why the rows stalled: the old parks were graded under QFE 4035 and several "lane gaps" were source shape (invented base
layouts, a missing CCharString copy constructor, a missing tail-merge form); propagated catalog labels pointed earlier
attempts at the wrong classes.
