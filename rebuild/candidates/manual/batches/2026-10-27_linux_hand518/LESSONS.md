# Chunk 518 lessons

## GOTCHAS one-liners
- (chunk 518) Re-queued 2026-10-14 parks first need an RTM (13.10.3077) regrade. 00a15100, 00a23d80, 00a63630 and 00a76900 matched at once; their old "lane gaps" were the QFE-4035 lane.
- (chunk 518) A member published to a global and used later: if the local-copy form (`shift = m; g = shift; ... >> shift`) swaps the callee-saved registers, store the members straight to the globals and read the GLOBALS back in the expression. VC forwards the stored registers, and retail's ebx/edi choice follows (0x00A652D0).
- (chunk 518) Retail re-reads a counted pointer it just cleared (here calling through null): that needs a real `CCountedPointer<T>` member with inline ReleaseData / `operator->`. Flattened `X_Data` / `X_Info` header members let VC fold the load to `xor ecx,ecx` (0x00A69DA0).
- (chunk 518) Base-member flag stores that land BEFORE the derived vtable store belong in the base constructor's body (CANavQuadTreeNode sets LeafNode = true, Blocked = false). A derived `Blocked = true` merges into the earlier store. A derived member init (PRegion(0)) is scheduled above the vtable too (0x00A76A30, 0x00A76900).
- (chunk 518) A C2DVector copied in a member initialiser needs a memberwise user copy constructor, so the Y store stays ahead of the next member's store (0x00A76A30). It is the C3DVector rule again (0x00A31220 swap).
- (chunk 518) const / non-const virtual overloads take slots in REVERSE declaration order: declare `GetNeighbours() const` first so the non-const one is slot 1 (+4) (0x00A77420).
- (chunk 518) STLport `CArray::DeallocateMemory()` = `if (capacity() > 0) { clear(); CArray<T> empty; swap(empty); }`. `CVectorMap::DeallocateMemory()` = clear, swap, `Dirty = false`, then the empty array's destructor frees the old block at the end of the scope (0x00A39950, 0x00A39900).
- (chunk 518) `C3DAnimationStats::GetAllEventNames` calls an out-of-line `__fastcall` 3-argument copy, with the hidden back_insert_iterator result in the dead argument slot. GetAllEvents inlines copy / `__copy_aux` and calls `__copy` with the tag reference in edx. The second shape still parks on the temporary's slot (0x00A50BF0).
- (chunk 518) The STLport `map::find` written as the 4.6 lower-bound walk plus `if (y == header || comp(k, key(y))) y = header;` inside an inline const member matches 0x00A779B0 (the old "null-root branch" park was QFE).
- (chunk 518) Lane gaps confirmed under RTM:
  - the find tag temporary homed over a live argument slot (0x00A76C30, 0x00A76C70);
  - an LTCG callee assumed to keep ecx (0x00A77420);
  - the x87 spill / Y-first order of `GetPointOnInfiniteLine` (0x00A57140).

## Review at landing
- 0x00A13C20 C3DCoordI::operator-= (retail_asm): its `maxps xmm0,xmm1` at 0x00A13C4F is the only one in the exe and may be a post-build patch. Retail also ADDS Z (a retail bug, reproduced). Decide whether retail_asm is the right grade.
- 0x0066FB80 binds `?GetLength@C3DVector@@QBEMXZ` to 0xA14480, which is GetMagnitudeAndNormalise.
- 0x006637D0 binds an invented `?Normalise@C3DVector@X_006637d0...` to 0xA14440, which is AccurateNormalise.
- 0x004473A0 binds `?Reset@CProcessedInput` to 0xA0D300; the real name is Clear.
- RemoveLOD 0xA249A0:
  - The landed binding `?RemoveLOD@C3DMeshInfo@@QAEXK_N@Z` differs from the PDB `?RemoveLOD@C3DMeshInfo@@ABEXK_N@Z`, which this batch uses.
  - The landed RemoveLOD source declares it QBE (public const).
- The landed LowerBound 0xA316B0 uses a global pair, a struct key and a non-const return. This batch uses the PDB `?LowerBound@?$CPreAllocVectorMap@KVC3DHelperPoint@@...@QBEPBU?$pair@KVC3DHelperPoint@@@std@@ABK@Z`.
- The landed C3DCoordI::operator+= 0xA13BE0 returns a non-const reference; the PDB says `ABV0` (const reference).
- The landed HasDummyObject 0xA2EA60 returns unsigned char; the PDB says bool.
- 0x00A557E0:
  - Landed as `std::copy` over STLport set<CCharString> iterators.
  - Its only caller, 0x00A4EF90, iterates the PDB `multimap<CCharString, float>` TimingEvents keys. This batch binds it with the multimap const_iterator.
  - Reconcile to one spelling (the real template is a key-only copy).
- The landed 0x00A55880 `__copy` and 0x00A55500 reserve spell the element `pair<CCharString, int/long>`. The PDB GetAllEvents says `pair<CCharString, float>` (M).
- Globals 0x013D28D8 / 0x013D28D4:
  - Landed as `g_LastAllocatorChunkCounter_013d28d8` / `g_LastAllocatorUsage_013d28d4`. This batch keeps those spellings so each address has one name.
  - They hold CMemoryAllocatorBase AlignmentShift (+0xC18) and Alignment (+0xC14). Rename them everywhere at once.
- 0x009F9F70 is landed as `CTextureUninitialiseView::Uninitialise`; this batch binds `?Uninitialise@CTexture@@QAEXXZ`.
- 0x0099A2F0 CBaseClassNonCopyable constructor:
  - The PDB says protected (`??0CBaseClassNonCopyable@@IAE@XZ`), which this batch's nav sources bind.
  - The landed CTextureManager model declares it public (QAE).
- 0x00A7D0E0 is landed as the descriptive `Find_00a7d0e0(long*, ...)`. It is STLport `__find(first, last, const T&, const random_access_iterator_tag&)`, used by the parked IsNeighbourOf rows.
- 0x00A6A330: CPreallocTexturePool is 0x54 bytes in retail and 0x58 in the PDB (the CArray is 12 bytes). Check the header generator's container size for CArray.
