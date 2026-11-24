- Wine VC7.1 cl/wineserver abort with "free(): invalid pointer" (rc -6/-11, SRC_FAIL with no text) when TMPDIR is set: unset TMPDIR TMP.
- c2 compiles inline callees bottom-up and the caller inherits the callee's own-pipe body; a callee's IL size (trace `sz`) is spelling-insensitive.
- A device that changes an inline callee's own pipe (inline_depth(0) on its definition) also changes the TU's pick-any COMDAT of it: check it against the owner address (0x00E241B0 here) before trusting the device.
- The c2 inline trace (agent3009 inl tool, copied to $W/inl/run.sh) runs in seconds on a one-statement TU and ~6 min on a 14 KB Main.
- (B24A2) An implicit dtor's inline size is fixed by its base chain (MovieBase over GROBase: sz 25 always; compile order, base-dtor
  placement and inline keyword do not change it), so "expanded movie + called timers on one exit path" cannot come from one `return`
  under any /d2inlS/T: the movie must be destroyed by a separate statement (goto/scope exit). /d2inlS15 frees the movie; then the cold
  returns need inline_depth(0) for the called ~CTimer.
- (B24A2) `#pragma inline_depth(1)` round a return does NOT stop a depth-1 dtor (~CTimer) from expanding under /d2inlS15; it only
  stops depth-2 callees. Read callee counts AND exit-block contents before trusting a "same size" grade.
- (B24A2) Frame slot order = density (IL refs*1000/size): every `goto` that replaces a `return` removes one dtor reference per local
  in scope; with 8 such gotos a 12-byte local (Hostage) loses to a 4-byte one (GoadLevel). Check with fmap.sh before grading.
