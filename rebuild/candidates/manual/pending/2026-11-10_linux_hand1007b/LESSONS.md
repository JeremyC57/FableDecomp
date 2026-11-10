# Lessons, chunk 1007b
- Reload order after a clobber follows split-piece order, which follows candidate ids. Reusing one local for several children
  ties their ids; one named local per child changes the ids and can flip the split order (0x00589A30).
- Frame-slot packing follows local declaration; separate per-use locals moved the manager into retail's [ebp-4] (0x0058F649).
- `FABLE_RETAIL_CDELETION_BODIES` is the scheduling fix whenever a CDeletion is built and passed to SetDeletion.
- The same-TU `auto_inline(off)` GetDef<T>/GetSubDef<T> bodies of the landed 0x005B8743 plus CDEFPOINTER_RELEASE_POINTEE give retail's
  short def releases; split `const CWorld& world = PeekWorld();` into its own statement when GetDef's pushes come after PeekWorld.
- c2prio K counts candidates per block: a named temp with one def and two uses (both arms) survives as a candidate and raises the
  priority of everything in those blocks; a single-use per-arm copy is eliminated and changes nothing. The catch is that the def is
  hoisted to the branch (0x0058F016).
- Deferred else-arms (laid after a later chain break) are a cross-jump + pending-block effect; `continue` can force the case-4 shape
  but costs frame slots. Micro-TU if/else, switch, &&, ternary and named-pointer spellings never defer on their own.
- The QFE 4035 compiler lays out the reduced loop model exactly as RTM 3077 does, so it is no lever for layout residue.
