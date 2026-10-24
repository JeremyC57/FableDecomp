# Chunk 374 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **STLport `_M_insert_overflow(.., __false_type, ..)` (004edd13, 004f9fa3, 004fa434).** Build it in a frame-pointer size TU: `optimize("y", off)` then `("s", on)`. Write `__uninitialized_copy` / `__uninitialized_fill_n` as `__forceinline`, and declare the per-element placement copy as an out-of-line `template <class T1, class T2> void __fastcall _Construct(T1*, const T2&)`. That gives retail's `mov ecx,dst ; mov edx,src ; call`. The old block goes through the out-of-line `_M_clear`.
- **STLport `_M_insert_overflow(.., __true_type, ..)` (005135d0).** Write `__copy_trivial` exactly as `(last == first) ? result : (char*)memmove(result, first, n) + n`, so the pointer compare comes before the subtraction. A plain `memmove` call reorders the code, and memmove intrinsic pragmas break it.
- **Size-TU copy helpers (004f94d2, 004f9db7, 004f9f62, 004fa3e3).** `__copy_ptrs` / `__copy_backward_ptrs` (`__fastcall`, `ret 8`) need the element's memberwise `operator=` declared `__forceinline`; otherwise `/Os` calls the implicit one out of line. With a frame pointer (`("y", off)`), the backward copy keeps `result` in its argument slot.
- **Same-TU callee bodies (005003e0).** When a function calls out-of-line `map::find` and `map::erase(iterator)`, giving those callees same-TU `#pragma auto_inline(off)` bodies is the lever. VC7.1 then knows the callees' register use, keeps the frame at one word, and keeps the `creature` register parameter out of memory. Declarations alone give a 3-word frame (187v166).
- **STLport list in callers (005003e0).** Model `push_back(x)` as `insert(end(), x)`, `size()` as a distance walk, and the iterator with a user copy ctor; otherwise the iterator temporaries are scheduled differently.
- **Version-chain `GetMaxSize` (00515fa0).** The form that matches is an early return plus `MAX` over the recursive call: `if (version == GetClassVersionNumber()) return MAX(size, sizeof(T)); return MAX(TOld::GetMaxSize(version, size), sizeof(T));`. Other forms fail:
  - a named local folds the inner test (53 bytes);
  - `MAX` over a ternary stops the inlining (72 bytes);
  - `__forceinline` overinlines (158 bytes).
- **rb-tree `erase(iterator)` inline shape (004fe5b0).** Retail's STLport shape is:
  - `_Rb_global<bool>::_Rebalance_for_erase` is `__cdecl` (`_STLP_CALL`, `add esp,10h`);
  - then `_Destroy(&value)`;
  - then `if (p) free(p)`;
  - then `--_M_node_count`.
- **Hidden-pointer CWideString returns (004faff0, 004fb0a0).** Static `__fastcall` CWideString returners build the result in a NAMED local (`CWideString path = CAFile::MakePath(..); return path;`). The default locals are constructed in declaration order, followed by the extension literal.
- **`__adjust_heap` (00515570).** The comparator call uses the by-value argument slot itself (no copy), and the comparator is a plain empty class with the implicit copy, so `__push_heap` gets it as a dword push. topIndex homes in the spare `push ecx` word.
- **Empty comparator copied into an inlined callee (005154c0).** The implicit copy is a byte move (`mov al,[..] ; mov [..],al`) and costs a register swap. An empty user copy ctor (plus default ctor) removes the move and fixes the registers. Retail still homes that empty copy over a LIVE spilled local (`push ecx` frame, `lea ecx,[esp+18h]` = `last`'s slot). The lane gives it a word of its own, the 0x00A76C70 / 0x007703B0 "empty tag over a live slot" gap.
- **CReplaceSerialise `ConvertPreviousUnknownVersionUp` (005148d0, 00516010).** Retail spills the hidden result pointer and keeps the SRA'd counted-pointer fields in registers. The lane keeps the hidden pointer and spills `result.Object`. Form changes do not move this; treat it as a lane gap.
- **`vector(const vector&)` with tag temporaries (004f932c).** A same-TU `__destroy_aux` body drops the second value-initialised tag `stosb`. Declaring `~vector()` alongside it pushes the element copy ctor out of line, so the two levers do not combine.
- **One-byte `vector(n)` (005136b0).** STLport's one-byte `uninitialized_fill_n` is a `memset`, which VC7.1 expands inline (`rep stosd` + `rep stosb`). A single `xor edx,edx` serves all the null initialisers.

Review at landing:
- 004f932c, 005148d0 and 00515fa0 used all 4 grades. 00515fa0 matched on the 4th.
- Local base models next to existing engine headers:
  - CParentDefClassBase (`unsigned char _body[0x39]`, retail 0x39 vs the donor header's 0x3D) in 004f9db7, 004f9f62, 004f9fa3, 004fa3e3, 004fa434.
  - The local CWorldMap model (the retail LoadedScriptedMapBrushes map is 12 bytes vs the donor's 16) in 004fe550, 004fe5b0.
  - Check whether `engine/CParentDefClassBase.h` / `engine/CWorldMap.h` should carry the retail sizes instead.
- `#pragma pack(1)` is used only where the PDB size is truly odd:
  - the 5-byte COldFileFormatMapHeader serialise classes (00515fa0 and the parked 005148d0 / 00516010);
  - the 13-byte CWorldMap::CPlayerPositionInfo (00515720).
- Already-landed 0x0051CBB0: its comment says GetMaxSize uses a named local, but retail 0x00519630 calls 0x00515FA0 twice, which fits the if-form found here.
- Already-landed 0x00942A00 names 0x0082D2D0 `CThingBuilding::PeekPMaterial`. Retail reads +0x3C bit 2 of the returned def, which is CSimBuildingDef::Property, so the callee is PeekPSimDefinition.
- Descriptive names to confirm: 00515420 `CLineList::AddFront` (no caller survives), and 005155f0 `CNavigationAreaUpdate::CSwitchableBlockage` ctor (identified from the PDB layout and the neighbouring C2DLineF list bodies).
- 005154c0 (parked) models CIsHouseLargerThan with empty user ctors. That is a lane device to remove the byte copy, and it disagrees with the callers' dword push of the comparator, which needs the implicit copy (as in 00515570). Do not land that class model as-is.
- No catalog row in this chunk holds two functions. 00513210 is one body plus int3 padding.
