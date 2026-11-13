# Chunk 2029 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 2029, 0x005FE090) A CCountedPointer built from a call result and tested right after (`CCountedPointer<C> list(Build()); if
  (!list.IsNull())`) gives retail's threaded null path (`je` straight into the else arm, the failed-new path keeping its own `Info = 0`
  block) only with FABLE_RETAIL_CCOUNTEDPOINTER (the ctor tests the stored member); the default `if (data)` merges both paths and hoists the
  arm prologue above the test (0x005FE23C).
- (chunk 2029, 0x005FE090) A zero-initialised counted pointer declared before a `const CDataBank&` reference gives
  `mov ecx,[PMainGameComponent] ; <zero stores> ; mov edx,[ecx+14h]`; declared after it, the zero stores follow the +0x14 load.
- (chunk 2029, 0x005FE090) A screen builder's object construction order can be read off the destructor run at its end (reverse order,
  each `dec [Info]` pad naming its frame slot): 10 locals placed without a grade.
- (chunk 2029, 0x00915B60) c2 hoists (PRE) the load of a by-value vector's `_M_start` into a loop's test block when the vector is read at
  every path after the loop (an index use and the destructors of the returns that follow): the value then lives in a callee-saved register
  from inside the loop. An early predecessor of the shared final `return` block (an else arm before the loops) stops it (fragment probe
  probe/f/h1.cpp / h2.cpp).
- (chunk 2029, 0x00915B60) An address-taken `bool` flag declared at function scope conflicts with every materialised HasTC byte; declared at
  the top of a block it shares the 1-byte slot with the HasTC bytes before the block (retail [esp+13h]) and the later ones get their own slot
  ([esp+2Fh]). Measured with the frame-packer trace, no grade.
- (chunk 2029, 0x00915B60) Identical `{ free; return false; }` copies are cross-jumped WHOLE into the survivor only when their rotation
  register equals the survivor's (eax here); a copy whose `mov reg,[index] ; push reg` got ecx / edx keeps that pair and jumps into the
  survivor's call. Each explicit copy also consumes one rotation step where it is laid out (one step after the LookAt loop moved every
  `lea reg,[key] ; push reg` of the crate / solid-to-thing section by one register).

Review at landing:
- 0x00915B60 is parked after pass 1 only (7 grades; the session ran out of context): passes 2 and 3 are still available. Start from
  differ/00915b60.cpp (= probe/p12.cpp) and NOTES.md "Pass-1 findings".
- NAME_FIXES.tsv: landed 0x00916640 (CClass_00916640::AssignTarget_00916640) is CTCExclusiveUse::SetUser; landed 0x00915810 is the
  (CThingCreatureBase&, const CCharString&) PutGoodsDown ctor; FetchGoods 0x00915890 keeps its invented local signature.
- New shared headers (overlay only): engine/CTCExclusiveUse.h, CTCCrateStack.h, CTCUserActionPointBase.h (campfire_types.h has a generated
  declaration list of CTCUserActionPointBase: a TU must not include both).
