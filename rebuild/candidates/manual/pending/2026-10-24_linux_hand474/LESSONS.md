# Chunk 474 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Same-TU stand-ins of out-of-line callees flip VC7.1's escape analysis (strongest lever this chunk).** Each is a
  noinline body that only reads or writes through its pointer arguments.
  - Without one, a local whose address goes to an external call is memory-resident for the whole function: saves
    `push [mem]`, compares `cmp [mem],reg`, and loops are not rotated into count-down form.
  - With one, VC7.1 keeps the value in a register and reloads only after the call. 0x00524EC0: the
    CPersistTraits<T>::TransferBinaryIn bodies took the diff from 561 to 184 lines. 0x00522160 / 0x00521EB0: a
    read-only `Compare` keeps the string data cached across the call.
- **map::operator[] stand-in.** A same-TU stand-in that reads the key by value moves the GetStates()[0] key
  temporaries' stores after their argument push (0x00546320).
- **Tag-taking STLport helpers.** A noinline body that never touches the tag homes the `__false_type` /
  `forward_iterator_tag` temporary in a LIVE argument slot. Seen with `_M_range_insert` / `_M_insert_dispatch` /
  `_M_insert_overflow` (0x00522160, 0x00523B40, 0x00545B10, 0x00560720).
- **Binary persistence: a record local, not scalars.** When retail reads fields into a struct local, model the
  record. 0x00524EC0 reads the dead-entity UID and CRC into a CPersistedDeadEntity local at +0x2c / +0x38, with the
  flag word +0x34 never written. Separate scalars give the wrong frame (0x88 instead of 0x84), the CRC slot shares
  the dead count, and the CRC save pushes a constant where retail re-reads memory.
- **The real TransferBinaryIn<long> body.** `T v; stream->Read(&v, sizeof v); *value = v;` over the inline
  CDataInputStream::Read (an `__int64` position check, then a fast path or ReadWithSrcChunkOverflow) compiles
  byte-identical to retail 0x0040FE60 / 0x0040F8A0. The body never touches `this`, so the empty traits object can sit
  in any dead or spill slot.
- **STLport list iterator.** Use `_List_iterator_base` + `_List_iterator<T, _Nonconst_traits / _Const_traits>` with
  the converting ctor `_List_iterator(const iterator&)`. A const_iterator then gets the trivial copy and is pushed
  directly, while an iterator is built in place (0x00545B10). A hand-written copy ctor on a single iterator type
  builds both in place.
- **STLport `vector::operator[]` is `*(begin() + n)`.** `_M_start[n]` changes the load order and leaves a dead
  `add eax,ebp` (0x00545B10).
- **CCountedPointer(T*) ctor tests the stored member.** `if (Data)` (not the parameter) gives retail's eax reuse on
  the null path (0x00545B10, 0x00546320).
- **Named casts cache in a register.** `CText* text = (CText*)ptr.Data;` keeps the pointer in esi across virtual
  calls, while `ptr->` re-reads Data (0x00546320).
- **Devirtualisation needs an object expression.** VC7.1 devirtualises `localStream.Write(...)` but not the same
  call through an inline member called on a reference (0x00522160).
- **The VC7.1 inline budget is per call site.** The same inline template is expanded at one site and called at
  another:
  - 0x00546320: the 1st CCountedPointer ctor is inlined, the later ones call 0x00429C15.
  - 0x00545B10: the CDeletion copy ctor and ~list are called in one branch and inlined in the other.
  - The real STLport model reproduces this without help.
- **STLport lives in namespace `std`.** Converting 9 sources and 2 parked files from `_STL::` to `std::` (keeping
  `_STLP_alloc_proxy`) re-graded RELOCATION_MATCH unchanged.
- Earlier lessons in this chunk:
  - Escaped vs non-escaped CDefPointer release forms differ (`dec [ecx+4]` vs `add eax,4; dec [eax]`).
  - Declaration order steers register choice.
  - Write a float product's terms z, y, x so VC7.1 folds the zero terms.
  - An `extern const` epsilon gives `fchs`.
  - A named info local fixes hidden-return order.
  - The CCountedPointer dtor clears Data, then Info (0x005DB6B0).
- **Known gaps (park early).**
  - Register colouring order between two address-taken counters (0x00524EC0).
  - The deletion.Method store sinking past the in-place iterator copy (0x005A887D / 0x005A9A87). Same-TU CDeletion
    ctor stand-ins, split-traits iterators, STLport-real or external insert bodies change nothing.

Review at landing:
- 0x005A887D used 5 grades, one over the 4-grade budget. It is parked in differ/.
- Matched rows that needed 4 grades: 0x00523B40, 0x005CCC91 and 0x00560720 (3 + the std-namespace re-grade).
- Local models next to an existing engine header (the header is the Ego_r layout with 16-byte containers and does not
  fit the retail offsets):
  - CTCInventoryTrade (0x005AE6AF, 0x005AE9DE, 0x005AEAEE, plus parked 0x005A887D / 0x005A9A87).
  - CTCHeroStats (parked 0x0057D23B).
  - CWorldMap (parked 0x00524EC0).
- Landed 0x005DBFD0 models callee 0x00A242C0 as "GetAbilityEntry"; it is CMeshDataBank::PeekAnimation (as
  0x005DB6B0 uses it).
- Landed 0x00520330 (AddPersistedDeadEntity) calls its map insert 0x0052A130 a "VC6 Dinkumware _Tree::insert". Retail
  STL is STLport, so check the comment.
- Landed models name 0x00993EB0 "WriteLong". The PDB name set has WriteSLONG / WriteULONG / WriteUHUGE (0x00993EB0 /
  0x00993EA0 / 0x00993EC0).
- The catalog labels both 0x00570009 and 0x005701DA "EnsureUndoAbility". 0x005701DA is EnsureUndoStat (Ego_r
  ?EnsureUndoStat@@YIXXZ).
- No catalog row in this chunk holds two functions.
