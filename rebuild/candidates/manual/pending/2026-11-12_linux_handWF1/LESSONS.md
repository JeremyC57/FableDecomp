## 0x00A114B0

- 0x00A114B0: the order of `fld a ; fmul b` in an inlined C3DVector cross product is not a spelling of operator^; it follows the temporaries around it (named local vs const-ref temp vs a later by-value copy) -- change those, not the header.
- 0x00A114B0: an integer copy that retail keeps behind a group of x87 stores means the stores go through an inline member (PostTranslate), not through direct member writes in the row.
- 0x00A114B0: `m.PostTranslate(C3DVector(v))` with the shared header gives `fld v.X ; fadd E41` plus Y/Z frame copies; by-value float parameters give no 12-byte slot.
- fg.py output must not be piped into head (BrokenPipe traceback); redirect to a file and print that.
- A frame store that retail emits "too late" (after a run of unrelated integer stores, just before the next x87 load) is the last instruction of a scheduling region: its reload lies beyond the 81-tuple cut. Move the cut with no-code PAREN tuples placed BEFORE it (here all three operator^ components parenthesised, `FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP`); parentheses after the cut only move the next one (0x00A114B0).
- Parentheses around a class-type expression add no tuple; only arithmetic parentheses do (0x00A114B0 sweep).
- VC7.1 never keeps the Z member (offset 8) of a 12-byte C3DVector local on the x87 stack while X and Y are enregistered: a cross product's Z is always a frame store + reload (0x00A114B0 probe A1).

## 0x007FF190

A hand-written vector::erase (next local + memmove) can leave register-tie-looking residues; spell STLport erase as `if (position + 1 != end()) __copy_ptrs(...)` -> __copy_trivial (shared stlport/fable_containers.h) -- row 0x007FF190.

## 0x00BEAB40

- (wf1 0x00BEAB40) A member load re-read for the store after a test of the same member (`mov ebp,[m] ; test ; ... ; mov edx,[m]`) is the store's value passed BY VALUE to an inline helper that holds the condition (`x = Combine(x, m)`); every direct spelling CSEs the two loads.
- (wf1 0x00BEAB40) Frame slot order of spilled locals does not follow declaration order; reassigning one variable (`t = f(t)`) instead of two (`i = f(t)`) moved the theme slots ahead of the loop-bound slots. Spill-STORE order at entry does follow declaration order.
- (wf1 0x00BEAB40) A helper argument that is a literal or static const folds in the front end (loads CSE again); the same constant in a local variable folds late and keeps the second load, which then shows as `mov reg,reg` after the test.
- (wf1 0x00BEAB40) Do not pipe xdis.py into `head`: BrokenPipe prints a traceback; redirect to a file and read that.
- (wf1 0x00BEAB40) Two candidates swapped between ebx and ebp with otherwise identical code is a one-point priority tie, not a spelling of the block: run c2prio --blocks, find the block whose K differs, and look for a NAMED local that costs no code (here `long endY = startY + 16;` at function top: +2 in the entry block, no hoisted-temp block; x 255 -> 259 against the theme ids' 256).
- (wf1 0x00BEAB40) A named loop-end local that VC7.1 rematerialises (`mov eax,[start] ; add eax,10h` at the latch, no frame slot) is still a named local in the source: retail's entry spill-store order (endX before startY) and the allocator ranking both need it declared.

## 0x00BFA920

- (wf1, 2026-11-12) Local-detail rows: read the collection type through its FableWin Peek* inline accessors (PeekReceivesShadows returns float) -- the 'register tie' at 0x00BFA920 (ebp=0 / ebx=0xFF / spilled mip bias) was the missing accessors, not the compiler; the handle operator= + temp dtor then need /d2inlT119.

## 0x009E3DF0

A "retail re-reads the table counts for the right shift" residue in CPixelFormat converters is FABLE_RETAIL_CPIXELFORMAT_POSSHIFT_FORMATREF (debug-body local-reference field sums), not a CSE setting: 0x009E3B60 and 0x009E3DF0 both match with it.

## 0x007F5150

- 0x007F5150: a -1/"not this name" return laid out BETWEEN the two arms of an expanded CCharString::operator==(const char*) means the body is nested in `if (name == "...") { ... } return X;`, not an early `if (!(name == ..)) return X;`.
- 0x007F5150: clamp arms that each carry their own `fstp st(0); fld const; pops; ret` (not merged with an earlier `return const`) are assignments to the result local (`if (g < 0) g = 0; else if (g > 1) g = 1; return g;`), not `return 0.0f` / `return 1.0f`.
- 0x007F5150: "this spilled to the frame, village in ebp, key constant in ebx" was a model error: plain unset `T* p; thing.GetTC(&p);` locals through the shared CThing.h (GETTC_EXPANDED) give retail's allocation; no register work needed.
- cl under Wine hangs on engine/CDefPointer.h without FABLE_RETAIL_DEF_CLASS_BASE and engine/CThingAICreature.h needs FABLE_RETAIL_CTHINGAICREATURE + CTHINGCREATUREBASE + CTHINGGAMEOBJECT + CTHINGPHYSICAL for the CThing base to be visible (run gcc `cpp` first).

## 0x00863CA0

0x00863CA0 C2DCoordI built in place vs pos.ToC2DCoordI(): the by-value coordinate keeps all four C2DBoxI corners in registers before the first store (add edi,eax instead of lea ecx,[eax+edi]); a NAMED empty filter with a user ctor gets its own frame word; a visible noinline search template body (ROUGHAREABODY) puts vector start straight in the iterator register.

## 0x00688D40

- A this/handle register swap left by a local-model draft (0x00688D40) vanished on the port to the shared headers: IS_THING_ALIVE + header CIntelligentPointer ctor/op= + default CDefPointer Release gave both release shapes and the retail registers with no forceinline.

## 0x006EA0C0

- An inline STLport `_M_find` whose header / found-node registers come out swapped (same bytes otherwise) can hinge on a named local for the object the lookup starts from: `T* attacker = it->second; attacker->Get..()` vs `it->second->Get..()` (0x006EA0C0).
- A CDefPointer out-handle reloaded after every use (`mov eax,[esp+..]` repeated, +18 bytes) means GetDef<T>'s body was visible: FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY (0x006EA0C0).

## 0x004B4260 (held back)

- A local container handed by const reference to a PRIVATE member defined earlier in the same retail source file: put that member's real body ahead of the row (0x004B4260 needs ActivateQuests 0x004B3CE0) -- declaration-only makes VC7.1 treat the array as captured and reshuffles every register (index in a register, the bool result in the frame).
- A flag set after an if/else whose arms differ only in one constructor argument may be written in BOTH arms in the source (`activated = true;` twice): VC7.1 merges the stores but the extra use changes which variable gets ebx.
- A string temporary whose constructor runs BEFORE the callee's other arguments are pushed is a named block-scoped local, not a temporary in the argument list.
- cl hanging under Wine (fg.py TIMEOUT) = a header `#error`; `g++ -E -I $W/hdr -I rebuild/include row.cpp` names it in a second (engine/CScriptBase.h needs FABLE_RETAIL_NSCRIPT).

