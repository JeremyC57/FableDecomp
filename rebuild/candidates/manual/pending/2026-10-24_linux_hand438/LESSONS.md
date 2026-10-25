# Chunk 438 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A null-checked `new T(this, key)` whose null `je` falls straight into the shared epilogue (eax still holds the
  zero from operator new, no `xor eax,eax`) comes from a VOID function that discards the `new`, because the
  constructor registers the object. `return new T(...)`, in either the direct or the named-local form, adds a
  separate xor epilogue (0x00C37990, 102 v 82).
- When a constructor's scalar zero words come after an EH-tracked container member and retail stores them before
  the owner load and the state bump, model them as a sub-object with an inline zeroing constructor. Plain init-list
  or body stores let VC7.1 hoist `mov eax,[esi]` above them (0x00C372C0, 133 v 134).
- libpk 3-vectors copied as `mov eax,[src] ; mov [dst],eax` per component (one register, load/store
  interleaved) need a memberwise inline `operator=`. The implicit struct copy goes through `lea` + `ebp`
  (0x00C37AD0 200 v 192, 0x00C418F0 168 v 160).
- `T* p = (flags & N) ? &local : 0;` compiles to `and dl,N ; neg dl ; sbb edx,edx ; and esi,edx`. Two such
  pointers interleave with a zero-constructed local's stores (0x00C37AD0).
- x87 compare operand order: `fld st(0) ; fld [one] ; fucompp` is `1.0f != length` (the constant on the left);
  `length != 1.0f` loads the constant first and then `fld st(1)` (0x00C402E0).
- x87 sum-of-squares order follows the source in a body-level expression. `z*z + y*y + x*x` gives the
  z, y, x loads with zz computed first (0x00C402E0); `y*y + z*z + x*x` gives yy then zz (0x00C418F0).
- `index = f() + 1;` used at two subscripts gives the folded `[ecx+eax*4+4]` load and then `inc eax` for the store.
  Writing `[index + 1]` at both uses folds both displacements (0x00C42A70).
- VC7.1 reassociates `a * 128 * b` (and `b * (a * 128)`) into `imul` followed by `shl 7`. An explicit
  `(a << 7) * b` keeps retail's `shl` then `imul` (0x00C425E0).
- Two by-value 12/16-byte struct returns (`T a = box.Read();`) are built straight in their frame slots through
  the hidden pointer. One `if (!a.x || !b.y) return false;` puts the shared false block at the end of the function
  (0x00C38A40).
- A local count in a serialiser lives in the dead parser argument slot under both compilers; nothing to model
  (0x00C3B7A0).
- (pre-compaction, same chunk) `if (!p) return 0; return p->x;` reuses the tested zero eax, while the ternary
  adds `xor eax,eax` (0x00C3ABD0 / 0x00C3B160). `if (v == 0.0f) return 1; return 0;` keeps the 1-exit on the
  fall-through (0x00C3A8F0). Advancing a list through the member (`m_head = m_head->m_pNext`) keeps the null
  test of `delete node`; advancing through the local drops it (0x00C3C930). A FILE-STATIC global flag (address
  never taken) lets VC7.1 hoist a member load above the global store, which an extern blocks; an inline `Clear()`
  member on the pointee lets a same-base load rise above a byte store (0x00C3E760). An inline by-value
  `Find(key) { return Lookup(&key); }` gives retail's self-copy of the argument slot (0x00C37B90).
  `long n = count; while (--n)` gives the dec/je + dec/jne count-down (0x00C3C8C0). The body order of a switch
  follows the source order of its cases (0x00C3D710).
- The libpk float helpers 0x00C3FF70, 0x00C40EE0 and 0x00C40F50 are inline-asm in retail: a bare dword
  `fistp`, and a frndint/f2xm1/fscale block with `wait`s. They are retail_asm candidates (owner exception).
- LTCG keeps `this` in edx across a call to a known-clean out-of-line constructor (0x00C37BD0 touches only
  eax/ecx). VC7.1 without LTCG saves it in esi, and a same-TU noinline body does not change that (0x00C37C40):
  lane gap.
- The QFE-4035 compiler (`FABLE_VC71=$VC71_QFE` in front of q.py) did not move the 0x00C418F0 x87 spill order.

Review at landing:
- Rows past 4 grades: none. 0x00C37990 and 0x00C3E760 used exactly 4, counting probes.
- Local base models: libpk has no engine header (no PDB). Every class is a local model named with its address.
  CPKVector3 is libpk's own 12-byte vector, not the engine C3DVector, so do not retype these rows to
  engine headers.
- Already-landed sources whose names are wrong, all libpk code:
  - 0x00C3C9B0 / 0x00C3CA00 "CCombatActionPool::PushActive/PushFree", 0x00C3CA30 "CCombatActionPool::Touch"
    (it unlinks the free head: PopFree), 0x00C3D610 "CCombatActionManager::RecycleAction" and 0x00C3DCD0
    "CTCHeroStats::CreateCharacterMorphAction" are the libpk two-chain node pool and audio event manager.
    No combat code is involved.
  - 0x00C4CED0 "WordWrap_SetCallback" is the pooled node's Link(prev, next) (its Unlink 0x00C4CEF0 is already landed under that name).
  - 0x00C307E0 "CScriptHandleRegistry::CreateNewInstance" is the libpk log's NewMessage (it returns the debug
    stream).
  - 0x00C0DEC0 / 0x00C0DF10 are landed as NScript `operator&&<CPredicate_And<...>>`. They are the libpk
    CString/debug-stream `__fastcall operator<<(const char*)` and `operator<<(unsigned long)` (0x00C0DF00 is
    the long overload).
  - 0x00C3AB30 / 0x00C3B0C0 "CDateAndTime::CDateAndTime" are the index-map node constructors
    `SIndexNode(void* value, unsigned long index)`.
  - 0x00C2A600 "CEngineResourceManager::Render" is CSoundLoop's Release (refcount at +0x10); 0x00C2AAB0 is its
    `??_G`.
  - 0x00C3CFE0's class (named CAudioTimer here) is really the audio device: its body is inlined at 0x00C3D504.
- Catalog rows holding several functions: none found in this chunk.
