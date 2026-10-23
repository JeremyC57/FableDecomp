# Chunk 322 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- 00dbeb20: retail inlines ~CScriptThing at the first site and calls 0x4AA840 out of line after that. An inline dtor plus `#pragma inline_depth(0)` after the first site reproduces it.
- 00d9d030: to put a local's dtor BEFORE a trailing statement (e.g. NewScriptFrame), wrap the local in an inner `{}` block.
- 00da8510: CTimer-style accessors on g_pGameScriptInterface (0x0143E8F8) must be `__forceinline`. A named local for a def value read before the global reproduces retail's load order.
- 00dbf2d0: the CEntityScriptBinding<T> ctor must be `__forceinline` (retail inlines the vtable store, the CCharString copy-ctor 0x99EC30 and the member stores at each AddEntityScriptBinding site).
- 00dbf2d0: N consecutive "frame + terminate check" waits are unrolled in retail; write them out (a `for` loop stays a loop under VC7.1).
- 00dbf2d0: an idle tail entered by `jmp` into its test from one path and into its body from another is `for(;;){NewScriptFrame(); if(IsActiveThreadTerminating()) return;}`, with a trailing `if(IsActiveThreadTerminating()) return;` on the other path; VC tail-merges that into the loop test. `while(!term) frame;` duplicates the test instead.
- 00dc8c80: when retail calls ~CScriptThing 0x4AA840 / ~vector 0x8AC970 out of line at most return sites but keeps the inline body at a few, wrap each out-of-line `return;` in `{ #pragma inline_depth(0) / return; / #pragma inline_depth() }`, and open `#pragma inline_depth(0)` for the rest of the function where every later site is out of line (reset after the closing brace). The pragma is per statement, so it can be applied site by site. It also fixed the stack-slot order.
- 00dc08c0 (parked): a `for (; !IsActiveThreadTerminating(); Game->NewScriptFrame())` loop puts the CTimer dtors before the frame call (no extra {} needed) and gives retail's stack layout.
- 00e12580: an inline counted-pointer `operator=` (compare Info, release old, copy, increment) under a `__forceinline CScriptThing::operator=` gives retail's inlined assignment; an out-of-line op= shifts the frame.
- 00e12580: when the post-wait return and the closing brace call dtors out of line, open `#pragma inline_depth(0)` from that return over the remaining statements and at the closing brace.
- 00e12580: `__forceinline void SpawnFunction(CSpawnedFuncBase* f, const CCharString& region = CCharString(""))` (a default reference temp) gives retail's region-temp frame slot; the `const char*` form is 4 bytes off.
- 00de3f90: VC7.1 devirtualizes virtual calls on a local object; call through `const CScriptThing& thing = speaker;` to keep retail's virtual IsAlive.
- 00de3f90: separate identical early-return blocks are tail-merged as in retail; one `||` condition is not.
- 00de3f90: `__forceinline` CScriptThing ==/!= that call impl slot 78 IsEqualTo match; non-forced ones stay out of line.
- 00e8b020: `while(!IsDone())` with the whole loser path inside the threshold `if` gives retail's rotated main loop.
- 00e8b020: a C3DVector local in its own `{}` block shares a frame slot with an earlier scoped object (frame -8).
- 00e8b020: `const CScriptDef* def = g_pScriptDef;` before the call gives retail's early edi preload; `GetHealth(Me) <= def->X` gives `test ah,1 ; je`.
- 00e8b020: build the named flag predicate before the CIsThingAlive local; an and-predicate with a user-declared dtor stores its own vptr.
- 0086bfb0 / 006bc510: stream reads through inline `ReadLong()/ReadByte()/ReadFloat()` helpers that RETURN the value make loop counts loop-invariant (down-counting loops) and let VC reuse the dead stream-parameter slot as the read buffer.
- 006bc510: a `std::string = stream->ReadPresizedString()` followed by a def lookup whose temp dtor sits AFTER the lookup is a forceinline helper taking `const CCharString&` (the temp) that assigns the member and looks up by the member.
- 004b3ce0: a set's `find(k) == end()` written with a free `operator==(const it&, const it&)` evaluates end() first, as in retail.
- 004b1d30: a TC bitset test compiled as `test dword ptr [thing+0x20+w], mask` comes from an inline `HasTC` returning the masked WORD; returning `bool` (`!= 0` or `?:`) gives `shr reg,n ; test reg,1`.
- 004b1d30: GetTC's lower_bound tail matches with `CTCMap& map = TCs; it = map.lower_bound(key); end = map._M_finish; if (it == end || type < it->first) it = end;` (the constant `type`, not the address-taken key).
- 00556a40: STLport `list::insert` is position-first: read `_M_node` before malloc, cache `prev = pos->_M_prev`, then link.
- Empty tag structs need a user ctor (`__true_type() {}`) so no zero store is emitted.

Review at landing:
- 006bc510 keeps a local CRegion model: rebuild/include/engine/CRegion.h has PDB-4 16-byte vectors/map (Maps +0x10 ...), retail is 12-byte (Name at +0x18). The header needs the 12-byte container layout before the source can include it.
- Rows past 4 grades: 00e12580, 00de3f90, 00e8b020, 0086bfb0, 004b3ce0, 00556a40, 004b1d30.
- 0x41E5F2 / 0x41D21B (CFrontEndManager::GetInstance / CManager::CreateComponent) sit at non-16-aligned addresses in a size-optimised TU.
- RELOC_CONFLICT (pre-existing, not from this chunk): the landed source 00418b7a binds `??1CBaseClass@@UAE@XZ` to 0x0099A300, while seven chunk-322 rows (00d643a0, 00d67270, 00dc0300, 00dc8c80, 00e12580, 00e1c580, 00e1cfd0) bind it to 0x0099A2E0. Both copies store vtable 0x01231710. labels_identity_corrections_2026-10-19 names 0x0099A2E0 ~CBaseClass and 0x0099A300 ~CBaseClassNonCopyable, so 00418b7a is the one to fix.
