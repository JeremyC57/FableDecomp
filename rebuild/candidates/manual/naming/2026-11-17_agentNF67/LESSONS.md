- AI state-group Update rows port in 3 grades with a sibling's macro block plus one class-level opt-in per class (state group, combat action) wrapping the generated layout.
- A named creature local (`CThingAICreature* c = &GetCreature();`) after an inline GetTC lets VC7.1 cache the creature across the LowerBound call; retail reloads it from the cached PBrain, so pass GetCreature() straight into the temporary's constructor (0x00961880).
- EAIActionRepeat: ACTION_REPEAT_UNTIL_FINISHED is 1, ACTION_REPEAT_CALLBACK is 2 (stand-ins pass the raw 1).
- Where an original row matched with fake same-TU "stand-in" callee bodies (to steer VC7.1's mod/ref), the port should use the callees' REAL bodies
  (copied from their landed rows onto the shared headers), visible and `__declspec(noinline)`: retail had them in the same TU. 0x00869020 matched
  RM with the real GetArmourWeightsFromThing (0x00868E50) and GetArmourResponseAugmentationDamageMultiplier (0x00868A80) texts.
- Do not wrap such helpers in `#pragma auto_inline(off)`: implicit members first instantiated inside the region (pair destructors) become
  out-of-line calls in the graded function too. `__declspec(noinline)` on the helper alone is enough.
- Opt-in macros must be defined before the FIRST include of an overlaid header (fable_map.h is pulled in by many engine headers); a row that
  includes it at the very top before its #defines gets the default path (0x00869020 a1 SRC_FAIL).
- A one-getter callee with a junk landed name can usually be pinned by its neighbours: retail keeps a class's accessor run in PDB order
  (0x007BED30..0x007BED80 mirror Ego_r 0x007BDC00..0x007BDC50 at +0x1130).
- A residue measured on a hybrid draft (some local models still in) is not evidence about the whole port: 0x008B9E50's recorded frame-packing blocker vanished once every class came from its shared header.
- A temporary that retail packs into a dead argument slot but VC7.1 gives its own local is the signature of an opaque callee that receives its address: make the callee's library body visible and noinline (0x008A7980: list::remove_if; the fable_map.h TREE_CTOR_DTOR_OUTOFLINE comment documents the same cure).
