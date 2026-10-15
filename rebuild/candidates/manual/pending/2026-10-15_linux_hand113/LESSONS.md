# Chunk 113 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A dead `add eax,N` next to an x87 load at `[eax+N+k]` after a getter call is a NAMED POINTER local to a sub-record at +N
  (`CRec* r = &Get()->m_rec; *out = r->m_x * m_scale;`); the direct `Get()->m_rec.m_x` chain folds the offset and drops the add
  (0x00837060). So not every "unused lea/add beside the load" is the RTM lane gap of wave 24l.
- Two uses of the same 2D array element before a call: a reference local to ONE member (`CIter& next = A[d][p].NextObject;`) plus the
  element re-indexed in the compare (`next.m_node == A[d][p].Objects.find(key)`) gives retail's common `this + index*16` base with one
  `lea` per member; a reference to the whole element folds the first member's offset into the base (0x0098E770).
- A callee whose result retail compares straight from eax returns the raw node pointer; a 4-byte iterator struct (even without
  constructors, only member operators) came back through a hidden pointer and added a frame slot (0x0098E770).
- Inside a `??_G`, a member destructor of the form `DestroyElements(); if (m_pData) free(m_pData);` and an inline `Close()` holding a
  call plus an import call are emitted out of line unless `__forceinline` (0x00AACEA0, 0x00B0F990, 0x00991450).
- A polymorphic MEMBER object with an inline empty destructor keeps its vtable store in the owner's destructor when the base destructor
  call follows (0x00A63B30); a virtual called from a destructor body is a direct call (0x009971A0).
- A `??_G` that frees with `mov ecx,esi ; call` (no push / add esp) is a class-scope `static void __fastcall operator delete(void*)`
  inherited from the thing base (0x00838500, 0x004C73B0).
- Float members zeroed by BODY assignments of the literal `0` share the integer zero register (`mov [esi+8],edi` ..), unlike the
  init-list float zero that becomes an immediate store (0x005FA530).
- `pair<const K, Record>::pair(const pair&)` in a `/Os` TU needs the record's memberwise copy constructor `__forceinline` (an implicit or
  plain inline one is called out of line); the speed form colours this/src/dst esi/edi/ebx instead of retail's ebx/esi/edi (0x004F9784).
- STLport `vector<T>::erase(iterator)` = `if (pos + 1 != end()) __copy_trivial(pos + 1, _M_finish, pos); --_M_finish; return pos;` with the
  `__fastcall` trivial copy 0x00405BA0 (memmove); `--_M_finish` is `add [esi+4],-4` for 4-byte elements and `dec [esi+4]` for bytes
  (0x00485468, 0x0053C7EF). Emit it with an explicit class template instantiation.
- C3DVector equality/inequality are `int __fastcall` members (this in ecx, other in edx, plain `ret`) written as `x == o.x && ...` /
  `x != o.x || ...` (0x00C256F0, 0x00C25730).
- A function that reads its object from eax with edx + one stack argument (`ret 4`) is an LTCG custom calling convention: no source form
  reproduces it (0x00C9CB40, parked).

Review at landing:
- Rows that needed the full 4 grades (matched on the 4th): 0x0098E770, 0x004F9784.
- Local polymorphic models next to existing flat engine headers: CFileDataOutputStream.h (its "retail-only long" at +4 is the stream's
  CCharString name), CFileDataInputStream.h, CFontBank.h, CHandleBasedMemoryPool.h (Handles is a 0x0c-byte CArray in retail, so
  FirstUnusedHandle +0x444 and Name +0x448, not +0x448/+0x44c), CFontDataBank.h, CChunkedFileRawDataChunk.h, CTrophyDef.h.
- Catalog row holding two functions: 0x00837040 (0x00837040 + 0x00837050) -- skipped; needs a split via skipped_ledger.py.
- 0x004C79A0 ("CAnimateThing::GetMeshBank") ignores ecx; 0x00837060 models it as a free function.
- No errors noticed in already-landed sources.
