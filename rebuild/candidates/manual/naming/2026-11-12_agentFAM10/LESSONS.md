# agentFAM10 lessons, one line each

- Wine cl hangs on `#error` (a header that requires a macro, such as CScriptBase.h without FABLE_RETAIL_NSCRIPT), and q.py's SRC_FAIL path recompiles. Kill the leftover cl/q.py by PID after each timeout (gradeall.sh now does this).
- Rule slip: while orphaned cl.exe processes were still running, two of my compiles overlapped once. The one-compile rule was broken briefly; I killed my own PIDs 15748/15749/17571.
- g.py keeps .grades itself, so never append to it by hand. One manual append corrupted a line, and I repaired it.
- An overlay copy can go stale while the repo moves: Q22 added SETASDRAWING to NPlayerGui_CDrawBase.h after I copied it. Before hand-back, diff each overlay against the repo for '<' lines, then re-base and regrade.
- A flattened header's `void* __vftable` cannot emit a virtual delete. A same-layout `virtual ~T();` opt-in is the cheap fix for DeleteData rows.
- An implicit destructor is not the same as `~T(){}`: the user-declared one stores the vptr first.
- The CCountedPointer<T>::DeleteData identity comes from the `mov [eax+4], addr` in Reset/ctor, and FableWin.pdb has an exact Reset(T*) name to cross-check it.
- Old catalog tests that model a stand-in still pass when the row is renamed, so only change the test when the behaviour model is wrong.
