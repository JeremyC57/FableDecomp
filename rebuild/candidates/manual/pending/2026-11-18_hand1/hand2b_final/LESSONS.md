- FableWin.pdb has intact module streams: llvm-pdbutil dump -l -modi=<N> /tmp/claude-0/egod_pdb/FableWin.4k.pdb gives per-line code offsets (VC10 debug) for every TU; module index from FableWin.modules.txt. Pins statement shapes without the lost fable7 tools.
- vland payload: the per-row flag key is "flags" (verify_and_land reads c.get("flags")); "compiler_flags" is silently ignored (row graded at default flags).
- At /O2 VC7.1's front end evaluates the LHS of `a[n] = f()` first (unlike /Od); an unnamed operator[] result is forwarded into the store, a named reference/pointer local is not and raises K in the blocks it spans (0x00F3ABF0).
- When a row's own body is pinned by the line tables and only c2 allocation differs, try the shared headers' existing ORIGINAL_* opt-ins of the inlined callees first (0x00AE3C30 closed by FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE, an inline helper's original locals/scope).
- A /d2inlT inline-budget residue can be a wrong callee spelling: FableWin line sizes showed Reset(NULL) (16 bytes) where the draft had ReleaseData() (14); the Reset spelling matches at default flags (0x00B6DC40).
- FableWin.exe is now on disk: read the donor's instructions per line (thunk jmp -> fablewin_pdb_names.tsv) -- it pins call order,
  named iterator locals, for-init variables, const vs non-const iterators, `if (0) {} else for` idioms and inline-callee bodies.
- A real variable initialised before a loop is scheduled before the loop-entry test; a strength-reduced compiler temp is
  initialised in the preheader after it. Retail's placement tells you which one it was.
- Include order changes c1 ordinals / tie keys only; it cannot close a priority gap (58 v 56) -- check prio before burning grades.
- vland needs the lane env (source $S/wine/env.sh) or verify_and_land falls back to Windows paths.
