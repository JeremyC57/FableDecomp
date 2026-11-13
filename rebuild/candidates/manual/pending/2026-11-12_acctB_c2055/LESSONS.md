# Chunk 2055 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00821A80) STLport `vector::_M_insert_overflow` must be VISIBLE and out of line (`FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE`),
  not declaration-only, wherever retail calls the COMDAT from an expanded push_back on a LOCAL vector: with the body visible VC7.1
  keeps `_M_finish` in a CPU location across the construct store (0x00821EE7 `mov edx,[esp+30h] ; cmp edx,ecx ; ... ; mov ecx,edx`);
  with the declaration alone it reloads `_M_finish` after every store and the frame grows by 8 bytes. One define moved this row from
  630 to 743 of 796 equal instructions.
- (0x00821A80) A handle assignment that READS THE SOURCE AGAIN (`if (Object != other.Get()) { Release(); Object = other.Get(); if
  (Object) AddRef(); }`) keeps the destination's release null test when the source is an address-taken local (a hidden-result
  temporary): the stored pointer is a second load, not the tested value. Retail 0x00821F5B `test edi,edi ; je ; dec dword ptr [ebp]`.
  Caching the pointer in a by-value parameter (`Assign(other.InternalGetObject())`) lets VC7.1 drop the test (`dec dword ptr [edi]`)
  and swaps the two registers. A one-second probe of just the loop reproduced the difference (see below).
- (0x00821A80) Two locals set under one condition and stored once after the join are if / else ASSIGNMENTS
  (`if (c) { x = a + 1; y = b + 1; } else { x = a; y = b; }`): VC7.1 hoists both loads above the branch, leaves `inc ; inc` in the
  arm and merges the two identical spill stores after the join (0x00821F95..0x00821FB5). `long x = a; if (c) ++x;` stores x after
  EACH definition; two ternaries give a two-arm diamond with the store in both arms.
- (0x00821A80) A loop whose miss handling exists twice in source (two `else` blocks clearing the same entry) costs a loop temporary
  two priority points in c2's allocator against one shared block behind a `bool` flag; here that flipped which candidate got ebx
  and moved the constant zero to another register for the WHOLE function. When a register map is wrong everywhere, look for a
  duplicated cold block in the first loops before touching anything else.
- (0x00821A80) Declaration order decides where a `lea` of a member reference lands inside a 64-bit member copy: `T& r = p->Member;`
  BEFORE `Count = p->Count;` gives `mov edx,[hi] ; lea edi,[ebx+16h] ; mov [hi],edx` (0x00821B51..0x00821B57).
- (0x00821A80) `f("LITERAL")` through an implicit `const char*` -> CCharString conversion re-addresses the temporary with `lea`;
  the explicit `f(CCharString("LITERAL"))` passes the constructor's eax.
- (0x00821A80) FableWin debug sizes settle a helper's TEXT even when retail expands it: the by-name GetDef<T> is `def.Get()` +
  `SetDynamicCast` (40 instructions / 139 bytes at /Od /EHsc), one instruction less than the `!def.IsNull()` + `*out = def` text.
- Method: when a residue is confined to one loop or block, cut a probe that keeps that loop and its register pressure (the rest of
  the function replaced by a few statements) and compile it alone: about one second per variant against one minute per row grade.
  The size-block and release-test spellings were both found that way (about 60 probe compiles, 3 row grades).

Review at landing:
- This row took 41 full-row grades (limit 4 in the old prompt; the owner's no-cap rule applied).
- Three shared headers gain opt-in blocks (hdr/README.md, chunk 2055 worker section): engine/CDefPointer.h
  `FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD`, engine/CDefinitionManager.h `FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_INLINE`,
  stlport/fable_containers.h `FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE`. Default text unchanged; 11 landed rows regraded unchanged.
  The row includes "stlport/fable_containers.h" first: stlport/fable_string.h includes "fable_containers.h" by its own directory, so
  an overlay copy is only seen if it is included first.
- Parked rows with an expanded push_back on a local vector and an 8-byte frame excess, or with a missing release null test after an
  inlined SetDynamicCast / GetDef, are candidates for the two new opt-ins.
- gall.sh does not export FABLE_HDR_OVERLAY: run it with the variable set (without it q.py ran into its 200 s timeout here).
- The landed row file for 0x0050EE40 is named CLayer::SetAllPolys but is `??0CFileLargeThemePalette@@QAE@XZ` (NAME_FIXES.tsv).
- 0x0042B144 GetDef<T>(long, ..): T is unresolved; FableWin's GetDef<CDefClassBase>(long) is 39 instructions / 128 bytes against
  41 / 140 for the other instantiations, so if T is CDefClassBase that instance has a different text than the shared template body.
