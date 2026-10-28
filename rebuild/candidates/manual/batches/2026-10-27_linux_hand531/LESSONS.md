# Lessons -- chunk 531

- A helper that holds `__asm` is never inlined by VC7.1 unless it is `__forceinline`; even then the first call site can stay
  out of line (0x00C3FF90 calls 0x00C3FF70 once and expands it twice), so model that callee as its own same-TU body.
- x87 order depends on whether a value lives in a named local. Passing the inline distance straight into the next call in one
  expression matched 0x00BF7DF0; a `float d = ...;` local swapped the fld/fstp order.
- Loops that `return band - 1;` with a fall-through `return 7;` inline into retail's `dec eax` / `mov eax,7` tail. Write the
  callee (GetLODZBandAtZ) as a real inline member instead of hand-unrolling the loop in the caller.
- STLport `vector::operator[]` written as `*(begin() + n)` fixed the loop register allocation in 0x00E81B40.
- A boolean "matches" flag (`bool m = true; if (a != b) m = false; ...`) reproduces retail's hoisted `mov al,1` / `xor al,al`.
- When a body branches on magnitude bands, write the conditions in retail's order (widest band first); VC7.1 keeps source
  order for the jcc chain. Return `long` when retail widens with movzx.
- libpk's CString is MFC's CString (AllocBuffer/CopyBeforeWrite/Release/ConcatCopy/ConcatInPlace/Insert). Port MFC line for
  line on top of the landed siblings. The LIB_ASSERT macro goes through the hook pointer 0x0143BB2C and CDebugBreak 0x00C16750.
- VC7.1 inline-budget limits can't be steered from source: ConcatInPlace inlines 7 of 10 GetData calls in retail. Splitting,
  reordering and temporaries did not change the per-function budget. Park it rather than burn grades.
- Frame-size residue (one dead 4-byte slot, 0x00BE2B70) and alias-free load scheduling in by-value struct copies (0x00BEEA00)
  did not respond to any source-level change I tried. Both look like optimizer-state differences, not source shape.
- Before trusting a source comment's "Ego_r.pdb" citation, grep ghidra_out/egor_pdb_names.tsv: SendBodyguardsToTheRightPlace on
  CQ_BreakSiegeScript is only in FableWin.pdb (fablewin_pdb_names.tsv).
- Check every external symbol in the object against the PDB name lists (`objdump -t` UND vs egor/fablewin tsv). That is how the
  CCharString/CWideString `operator=` returns (`ABV0` const&) and the `PB_W` wchar_t spelling were caught; /Zc:wchar_t is the
  flag that produces `_W`.
- Script slot names come from ghidra_out/cgsi_slots_aligned.tsv. Generating the padded CGameScriptInterface vtable from that file
  (gsi.py) saves hand-counting 400+ slots.
