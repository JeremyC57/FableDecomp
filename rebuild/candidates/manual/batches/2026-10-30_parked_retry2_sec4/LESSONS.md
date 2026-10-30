# Lessons from the resumed retry sweep, wave 2, section 4

1. **Try `CThing::IsDead() const` first on any IS_THING_ALIVE tail.** Write the accessor as `{ return (m_flags91 & 1) != 0; }`, use it inside a nested-if IS_THING_ALIVE, and declare the helper `inline`, not `__forceinline`. VC7.1 then emits retail's branchy `test byte [eax+91h],1; jne false; mov al,1` exit. A raw `!(flags & 1)` folds to a flag select instead (`mov al,1; test al,dl` or `xor al,0ff; and al,1`). Results: 00935ad0 matched, and 00939670 and 00921990 improved. `isdead.py <in> <out>` applies this mechanically.
2. **A module global stored next to a call argument may be a function-local static.** With `static float x; x = v;` the store schedules after the argument push. An `extern` global's store schedules before it (0092afe0, global 0x013BB6B4).
3. **When retail fetches the object before the argument** (`call GetPItem; mov edi,eax` before the argument's GetPosition), write the object as a named local, or reach it through an inline `operator->` on the intelligent pointer. VC7.1 evaluates the arguments of `p.GetPItem()->F(arg)` first. With `p->F(arg)`, where `operator->` is inline, it evaluates the object first (00931200, 85 -> 24 differing lines).
4. **A packed (`pack(1)`) record used as a push_back temporary lands 1-aligned in the frame.** Retail had it 4-aligned. A named local of the same type gets a 4-aligned slot (008a9880).
5. **A constructor that takes a C3DVector by value has VC7.1 build the temporary straight into the parameter.** Taking it by const reference changes the copy schedule; for 00970df0 it moved toward retail.
6. **`bool r = false; ... r = !cond; ... return r;` keeps a live flag register (bl)** that retail also keeps. Returning `!cond` directly lets VC fold the flag to a constant, which shifts the whole register plan (009478a0).
7. **Fixing a temporary's destructor name means modelling the hierarchy.** For a temporary whose class has no destructor of its own in retail, declare the base class (here CCombatActionBase) with its destructor. Do not add a fake derived destructor: the reloc then names the wrong symbol (RELOC_NAME).
8. **These loop shapes are canonicalised by VC7.1 and made no difference:**
   - `while (a) { if (b) break; }`
   - `while (a && !b)`
   - `for (;;) { if (!a || b) break; }`
   - do-while with goto, `for(;;)` with continue
   
   When retail aligns or does not rotate a loop and ours differs, the spelling of the loop is not the lever (00926060, 00944300).
9. **Check slot-sharing residue first.** Retail sometimes keeps a preset-to-zero local in a shared key slot in memory (009216e0, 00961ca0), and the out-pointer GetInterface forms did not reproduce it. Test that one first before spending attempts on colour swaps.
