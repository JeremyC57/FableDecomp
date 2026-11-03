# cons11b LESSONS

- **One inline body can need different source shapes in different rows; test the variants on every row before
  choosing.** GetTC<T> (`const bool&` local vs direct `if (HasTC(...))`) and CDefPointer::Release (`Release()==0` vs
  in-place `--IVRefCount`) each had one form that matched every row. Codex had fitted each one to a single row.
- **Change a shared inline at the narrowest class that proves it.** Moving the delete into
  CIVCountedPointeeBase::Release matched both target rows but broke 17 landed CIVCountedPointer rows. The same codegen
  change made inside CDefPointer::Release broke none.
- **VC7.1 materialises a value it must return.** `if (x->Release() == 0)`, where Release returns `--count`, gives
  load/dec/store/test. `if (--x->count == 0)` gives `dec [mem]; jne`. Likewise a `const bool&` local for a bitset test
  gives shr/and/mov to the stack, where a direct `if` gives `test ah,ah`.
- **An explicit specialization takes the primary template's calling convention.** To get a YI (fastcall) STLport helper
  from a shared `inline` template, compile the TU with /Gr. Member functions stay thiscall. This is now used by
  0047a9f0, 00521620 and 0052b030.
- **Respelling a landed callee binding without asserting the caller's own element type:** keep the caller's descriptive
  element, and call the proven helper through pointer casts. This produces no code; the 0052b030 relocations show both
  the callee spellings.
- **Wine cl.exe hangs on a fatal compile error** (an `#error`). Run `pp.sh` (gcc -E) first, and bound every compile.
  `tools/q.py` now has a 120 s run_tool. The coordinator flagged a 4.5-minute hang from the unbounded version.
- **Never put `&` inside a run_in_background command.** The shell returns at once and the jobs become untracked orphans.
  Also never recheck against a live overlay you are still editing: freeze a copy (`tmp/hdr_snap`) first.
- **hdrcheck.py can print `same` when cpp failed on both sides.** A header whose includes `#error` without its macros
  gives the same truncated output for both. `tools/hdrcheck2.py` prints cpp errors and line counts; headers it flags need
  a textual diff.
- **Unquoted heredocs** expand backticks and `$?` inside Python and comment text. Use `<<'EOF'` and pass values through
  the environment.
