# 0x012133C0 Unwind@012133c0 -- RELOCATION_MATCH (payload.json + oracle.tsv written; vland strict self-check PASS, no WARN/RELOC lines)
- Identity: EH unwind block of CPhysicalPlayer::Play @ 0x00C271F0 (landed, RELOCATION_MATCH, /EHsc /MD). Proof: parent prologue
  `push 0x012133C8` at 0x00C271F2 (xref.py); handler thunk `mov eax,0x0136CCB4 ; jmp ___CxxFrameHandler`; FuncInfo 0x0136CCB4 unwind map
  entry action = 0x012133C0 (abs ref from .rdata 0x0136CCB0). Funclet = `lea ecx,[ebp-60h] ; jmp 0x00C0FFE0` (frame-object dtor).
- Not code of its own: the row is the standard funclet stub that #includes the parent's landed source (same form as the ~400 landed
  Unwind@ rows, tools/decomp_pipeline/families/funclets.py), with `funclet_offset: 0` and flags /EHsc /MD.
- Difference from other funclet rows: the oracle length 18 covers the 8-byte funclet AND the 10-byte __ehhandler thunk (Ghidra merged
  them), which is why families/funclets.py skipped it (its "thunk follows" check reads base+18). The $L block is exactly 18 bytes, so the
  extractor at offset 0 reproduces it (RELOCATION_MATCH).
- Test: the parent's behaviour test with the pass string renamed (vt.py PASS).
- Tool fix: every Wine cl call segfaulted (rc -11, vt.py "TCC_FAIL" with no errors) while TMPDIR/TMP pointed at the session scratchpad;
  `unset TMPDIR TMP` before the tools fixes it.
