# Chunk 433 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Pool walks (`for (i < pool.m_count) { e = pool.Get(i); ... }`) only get retail's registers (this esi, index edi, element ebx) when the count and the element come through inline accessors on the owner (`GetNumEvents() { return m_events.m_count; }`, `GetEvent(i) { return m_events.Get(i); }`); direct member access or a same-TU Get body swap esi/edi (0x00C123B0, 0x00C12680).
- A missing EH state-0 store plus a `mov eax, fs:[0]`-first prologue means the callee called between a scoped lock's ctor and dtor is a nothrow body in the same TU: give it a same-TU body under `#pragma auto_inline(off)` (0x00C1A070, the map Find 0x00C1AE50).
- Declare `operator delete(void*) throw()` as VC7.1 `<new>` does; without `throw()` an extra EH state store appears before an inline vector `_Tidy` in a destructor (0x00C14560 / 0x00C14630).
- A recursive in-order search whose zero return has its own epilogue is the tail-recursive form (`if (node == &Sentinel) return 0; ...; return F(node->Right, ...)`); the while-loop form merges the zero return into the found exit (0x00C18D90).
- Dinkumware `copy_backward` on scalar pointers: retail's `ret 4` `__fastcall` row is the 3-argument wrapper with `_Copy_backward(..., _Scalar_ptr_iterator_tag)` inlined; the tag-taking helper out of line is `ret 8` (0x00C13D40).
- An inline helper taking the key BY VALUE and forwarding its address (`FindEntry(void* key) { return Find(&key); }`) reproduces retail's fresh frame copy of the key per call (dead arg slot, then `push ecx` word) (0x00C14AD0).
- A release-build trace macro is an inline empty `TraceOutput(const CString&)`: the argument temporaries are still built and destroyed; the two-argument form builds them right to left (0x00C12720, 0x00C127C0).
- One `result = 0` variable assigned in each branch (name / switch cases) and returned once gives retail's `xor eax,eax` BEFORE the first test (so the tested pointer goes to ecx) and the switch default sharing the last case's epilogue; per-path `return`s give an extra epilogue with its own `xor eax,eax` (0x00C196A0).
- A /GS frame 4 bytes larger than your buffer + locals: the array is `[MAX_PATH + 1]` (0x105 rounds to 0x108) even though every Read passes 0x104 (0x00C16E10).
- Merged identical `return -1` blocks: separate `if`s keep the survivor at the LAST test's fall-through; a `||` or a positive final test moves it to the function end; retail 0x00C16E10 keeps it after the FIRST test (unsolved, parked).
- `(float)unsigned_value` = `fild` + `test`/`jge` + 2^32 `fadd` fixup (0x00C15CD0); `x <= 0.0f` -> `test ah,41h; jp`, `x >= y` -> `test ah,1; jne`; `(float)pow(a, b)` under `#pragma intrinsic(pow)` -> `_CIpow` (0x00C1BDA0); a named `scaled` local keeps the two separate fmuls (0x00C1BA40).
- A `||` of two calls each taking a CString temporary: VC7.1 tracks the conditional temporaries with construction-flag bits (1, 2) in ebx mirrored to a frame word and destroys each only when built (0x00C16C10).
- LTCG register arguments are easy to spot: the callee pushes / reads esi, edi or ebx without ever setting or saving them, and the caller loads them (often by `lea`) right before the call. In this chunk: 0x00C16C10 (esi), 0x00C16CE0 (esi), 0x00C16D90 (edi + esi to its callee), 0x00C17640 (ebx + esi to its callee 0x00C174A0), 0x00C1BF00 (esi, edi, ebx). Park after one reference grade.
- libpk uses Dinkumware STL (MSVCP71 basic_string imports, 16-byte vectors: allocator word + first/last/end), not STLport: name its vector/algorithm rows in `std`, not `_STL`.

Review at landing:
- 0x00C12680 used the full 4-grade budget before matching (inline accessors + inline `IsPlaying`); 0x00C1B7D0 and 0x00C16E10 are parked at 4 grades.
- 0x00C13840 / 0x00C138F0 are int3-after-noreturn: land against the 113-byte spans with differ/<addr>.test.cpp.
- Local models only (no engine headers exist for these libpk classes): CChickenSign, CAutoDeletePointer, CFileManager, PKCTRRedblackTree/Map, PKCStaticQueue, the sound-event manager. The CChickenSign layout follows the landed 0x00C1BC90 model.
- Already-landed sources whose names or models look wrong:
  - The sound-event manager (vtable 0x012B2748) is landed under several unrelated names (C3DMesh2, CInventory, CTCHero, CPanel for 0x00C11CB0 UpdateEvents); 0x00C125E0 CountSelectedBindings carries `// Leaf: GetTexturesInUse`, which does not match its function name.
  - CFileManager ctor 0x00C1AC40 and dtor 0x00C1A230 model different layouts (ctor: +4 0x18-byte critical section, +0x1c allocator, +0x20 the PKCTRRedblackMap whose ctor takes the allocator -- landed as a "dispatcher"; dtor: a 0x40-byte table at +4).
  - 0x00C1AEA0 landed as `CLODMapOwner` is the PKCStaticQueue destructor; 0x00C1B6D0 `CThingMapManager` is the static-queue wrapper (probably a libpk mailbox).
  - 0x00C1B1E0 landed as `CTexture::GetNoLevels` is the node allocator's Release (`Free(*p); *p = 0`).
  - 0x00C2A2A0 landed as `CLocalDetailCacheMap::CloseStaticMap` is PKCThread::Join (called by both worker-queue destructors before the delete).
  - 0x00C19680 landed as `_Cons_val` is the object map's GetScope (`record ? record + 0xc : m_pOwner + 0x3c`).
  - 0x00C2DA00 landed as `NScriptCVMurderTwistScript_Read` is the libpk segment file's Read forwarder (called with (buffer, 0x104) and compared with 0x104 by 0x00C16E10).
  - 0x00C2E0C0 (landed as a CSubPrimitiveList ctor) and 0x00C2EED0 / 0x00C2EF30 (landed as WaypointData) are built as locals by the libpk bank code 0x00C16CE0 / 0x00C16D90; their names are likely propagated too (not verified further).
