# Chunk 88 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport map walk that erases as it goes (`cur = it; ++it; if (...) Erase(cur)`): retail loads begin into ecx and copies it to
  esi BEFORE the entry test, with `mov ecx,esi` at the loop bottom. Only the order `Node* cur = header->left; Iter it; it.node = cur;
  while (cur != header) { it.Increment(); ...; cur = it.node; }` gives that; `it` declared first loads begin into esi, and a
  fresh `next` iterator inside the loop moves the copy into the loop head (0x004BFF60).
- A getter chain feeding a call that also takes a constant (`GetEngine()->GetRandom()->FloatRandom(1.0f)`) has VC7.1 hoist the
  `push 1.0f` above the getter calls; fetch the inner object into a local in its own statement to keep the push after them
  (0x0077C490).
- Coordinates converted with `(long)` into an on-stack pair: member-wise assignment (or aggregate) converts in source order
  (x first); a two-argument constructor converts y first because arguments are evaluated right to left (0x00A3F8B0).
- A `__stdcall` virtual overriding a slot of the SECOND base (at +0x0c) gives `mov esi,[esp+8] ; ... ; ret 4`, members at
  `[esi+4]` and `lea edx,[esi-0Ch]` when `this` is passed as the full object -- the COM secondary-interface shape; no thunk or
  cast needed in the source (0x00CAEC80).
- A function taking a scalar whose ADDRESS is passed on (`lea edx,[esp+8] ; push edx`) is a by-value parameter used as
  `&key` (`Find(&key)`), not a by-reference parameter (0x00C2A6D0).
- Virtual slot index = offset / 4: slot +0x44 needs 17 dummies before it (v00..v16) -- a one-off miscount costs a grade.
- A member pointer tested for null and then used for a COM call is read twice by VC7.1 on its own (`mov eax,[esi+18h]` before
  and after the early return); no `volatile` needed. When the pointer is first copied to a register for the `this` push
  (`mov esi,[esi+98h]`) it is CSE'd -- both shapes come from the same plain source (0x00CA9B10 vs 0x00CA9AE0).
- Critical-section guarded resets: zero stores share one `xor eax,eax`, are scheduled after the LeaveCriticalSection argument
  push, and the `return 0` needs a second `xor eax,eax` after the call (0x00CADDC0).
- Earlier session (first agent) lessons, already in the src/ headers: optimize("s") ctors show `push dword ptr [esp+0Ch]` and
  `and [reg+N],0` (0x004D8FDD); optimize("y",off)+("s",on) for `push ebp` frame ctors with an empty allocator temporary homed
  in the dead owner slot `lea eax,[ebp+0Bh]` (0x004DE839, 0x004E748A); `__forceinline virtual ~Derived` + non-virtual base dtor
  + a `new` factory to emit a `??_G` with the body inlined (0x0077B6B0); `__declspec(novtable)` for a dtor with no vtable store
  (0x007495D0); bit-field store from a bool merges with `xor/and/xor` (0x004BFA20); signed-char counters with `++c == 1` /
  `--c == 0` cross-jump one tail (0x004BFB00); an inline `unsigned long` getter that returns 1 when the byte is zero but a
  flag bit is set (0x004BE490).

Review at landing:
- 004bff60 needed exactly 4 grades (matched on the 4th); the other new rows needed 1-2.
- Local base models next to an existing engine header: CTCBase.h exists but is a flattened POD (`void* __vftable`) and cannot
  be a polymorphic base, so every CTC* ctor/dtor row uses a local CTCBase model. CTCGraphicAppearance has no engine header (local
  model shared through gen/model.txt + gen/mk.py).
- Landed source rebuild/src/compiled/00/ca/CAppearanceOwner00ca4990_Rebuild_00ca4990.cpp declares its callee
  `void CAppearance_00caddc0::Rebuild()`; the callee (0x00CADDC0, this chunk) returns `long` 0 -- harmless for that caller, but
  the declaration should become `long Rebuild();` if the two are ever linked.
- Rows renamed away from their catalog label (see NOTES "Label corrections"): 00a3f8b0, 00ca9ae0, 00ca9b10, 00ca9b40, 00caec80,
  00c2a6d0, 00caddc0 have descriptive method names (GetHeightAt, Resume, ResumeStream, SuspendStream, Unadvise, ReleaseByKey,
  Rebuild) and carry `// Leaf:` lines; the 0x00C0xxxx-0x00CAxxxx rows are third-party media/library code, not CTCGraphicAppearance.
- No catalog row held more than one function.
