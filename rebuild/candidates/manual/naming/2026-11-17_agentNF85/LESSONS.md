# NF85 lessons

## Picking the function to grade
- **Grade with the full leaf, not a prefix, whenever a row has overloads.** For the hinted map insert_unique, a short leaf prefix made the
  grader pick the unhinted overload in the same object. xref2 then printed bindings from the wrong function. Use the exact old leaf
  (from `work/old_<addr>.txt`), or the full symbol once known. gg.sh rewrites the Leaf to the full symbol after a match.

## Binder claims
- **Probe a binder before porting it.** reloc_bindings.tsv is often stale. Five of the 31 WORKLIST binder claims already bound the right
  symbol: the CFaction rows 0x006B94E0 and 0x006B9510, then 0x004EB9A6, 0x00A41B60 and 0x004BBBC0 (`?helper_4bbb40` was long gone). One slice row (0x006276F0) makes no
  call at all. A five-second oldprobe saves a port.

## Same-TU bodies
- **A visible callee body changes register allocation.** In 0x004B39B0, retail reuses the finish read for `__copy`'s `last` across the
  UpperBound call. The old row wrote that out with hand-made locals.
  - The shared-header version gets it naturally: UpperBound's (noinline) body is in the TU, and VC7.1's mod/ref knows it writes no memory.
  - Before hand-writing locals for a "the compiler re-reads X" difference, try making the callee's body visible as noinline.

## Callees of a binder
- **Port a binder's stand-in callees in the same pass when they are small.** The q11087 binder's port left three callees with stand-in
  own names (UpperBound, `__copy`, `__destroy_aux`). Each took one grade as an explicit instantiation on the same headers. Other landed
  rows (0x004BB6B0, 0x004BB400, 0x00A33A70, 0x00A71220) already bound the exact spellings, which gave the second source for free.

## Twin recipes
- **Do not copy a twin row's local class model just because the twin landed with it.** 0x005A731F first copied 0x005A45F6's
  `class CThingPlayerCreature : public CThing { };`, because the header's default branch failed (C2039). The retail branch
  (FABLE_RETAIL_CTHINGPLAYERCREATURE over the CTHINGPHYSICAL / CTHINGGAMEOBJECT / CTHINGCREATUREBASE chain, as 0x0083B3A0 uses it)
  compiled and matched first time. Search `rebuild/src/compiled` for a row that defines the class's retail macro, and copy its define
  set.

## Overlays
- **Remove a nested opt-in nobody uses before hand-back.** The SORT_HEAP_BODY block had a POP_AUX_OUTOFLINE arm left from a try on a
  row that ended up needing no change. Run `grep -l '^#define <macro>$' rows/*.cpp` over every new macro.

## Hand-back checks
- **Count STANDIN_LEAF and RELOC_NAME warnings in the final vland log, and account for each one** (RENAMES s1, NAME_FIXES, or a checker
  artifact). Each RELOC_NAME here pointed to an outside callee with a stale self name worth a NAME_FIXES line.
- **Keep tool paths in one place.** pin_asserts.py lives under `tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/`. Guessing
  the path cost a non-zero exit. A trailing `ls` or `cat` of a file that may not exist does the same: end such checks with `; true`.
