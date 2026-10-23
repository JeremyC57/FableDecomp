# Chunk 300 -- lessons from the authoring agent (fold into GOTCHAS at landing)
- VC7.1 shares frame slots by scope: an out-param local used only for a bool test gets its own slot unless it is scoped in a block.
- Parenthesise a constant product to keep retail's fmul order: `((R - 1.5f) * 0.01f) * x`, not `(R - 1.5f) * 0.01f * x`.
- A pooled literal epsilon folds `eps*eps`; `extern const float g_epsilon_0129ba3c` keeps `fld eps; fmul eps`.
- A vector modelled as a nested struct `{ _M_start; _M_finish; bool empty() const; }` stops CSE of start/finish across a flag store; separate pointer fields got CSE'd.
- An uninitialised by-ref argument object homed in a dead lookup-key slot must be 4 bytes (`long Reserved;`); an empty struct gets its own slot and grows the frame.
- A manager pointer (`GetWorld()->POpinionReactionManager`) must be a named local to be loaded before GetPItem.
- A member-assigned C3DVector (X, Y, then Z+k) gives the int copy of X/Y and an fadd for Z.
- Write a magnitude sum as `(end-middle).Magnitude() + (middle-start).Magnitude()` to get retail's order.
- A tail `CThingCreatureBase* parent = ParentThing;` caches it in esi.
- An inline `GetDefNameString()` with a named local and return gives temp+copy ctor; `inline void ClearFlag(){ m &= ~0x40; }` gives `and byte [mem],imm`.
- An out-param inline lower_bound places the component in first's register.
- A literal 4.0f fixes an fmul order; an inline CWorld wrapper changes vcall registers; a wrapper used in both arms of a ternary is cross-jumped (merged).
- When retail inlines a same-class callee at one constant call site only (e.g. OnHeroLeaveBoastingArea -> EnterCrowdState(9)), define the callee in the same TU: /O2 (/Ob2) reproduces the selective inlining by itself.
- Retail layouts drift from Ego_r: CTCCreatureNavigation is 4 bytes short after PossibleNavigators (12-byte vector), and CTCCombat is 0xc short before AlwaysBlockThings. Check the header against retail offsets before using it.
- `if (x > 0) --x;` right after an `x == 0` test compiles to jle; retail's `jl` needs `if (x >= 0)`.
- An inline default ctor of a CBase-derived interpolator (`C3DOrientationInterpolator() { }`) reproduces retail's `call CBase::CBase; mov [esp+n], vtable`.

Review at landing:
- Rows that needed more than 4 grades: 006e1620 (9), 0075c670 (16), 0078faf0/00795090 (12), 007b6810 (11), 0066fc90 (11), 0074ace0, 0080d0b0, 00784260, 00683770.
- Local base models next to an existing engine header: 00784260 and 007846b0 (CTCCreatureGroupBoastingCrowd.h exists, local model used); 00684210 (CTCCreatureOpinionOfHero.h); 0074a260/0074ace0 (CTCCreatureGenerator.h); 007306c0 (CTCDoor.h). Check the headers against the retail offsets before retyping.
- 007846b0 also defines EnterCrowdState (0x00784260) in its TU; the landing must keep that definition local (static TU copy) or mark it so it does not clash with the 00784260 object.
- Catalog row 007846b0 holds two functions: OnHeroLeaveBoastingArea ends at 0x0078491F, and 0x00784920 is the landed OnHeroBoast.
- Landed-source errors: CTCCombat.h and Ego_r-derived CTCCreatureNavigation offsets do not match retail in the rows above. The manifest labels 0x00686DA0/0x00686A80/0x00686A70 "GetDisplayEngine", 0x0049D870 "GFGetPreMainMemoryUsed" and 0x006A4D00 "DoSizeof" are propagated and wrong.
