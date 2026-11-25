# NF401: Leaf-line fixes for the NF388 Q8 grading-artifact rows (B: 14, C: 3)

Result: 16 ACCEPT, 1 HOLD (009e15e0, a real DIFFER). Every hand-back source differs from HEAD by exactly one
`// Leaf:` line: 16 rows gain a line, and 00c5eca0 has its `// Leaf: Kill` line replaced. Nothing else changed. All 17 files are LF
with no BOM (checked in Python). 16 of them have no final newline (all except 009e15e0), and that is kept.

## Files
- `rows/<addr>.cpp`: the source to land (one Leaf line changed). `orig/<addr>.cpp`: the HEAD copy. `diffs/<addr>.diff`: the unified diff.
- `RESULTS.tsv`: addr, old grade (NF388 Q8), leaf, new grade (vland --recheck-landed), ACCEPT/HOLD, note.
- `SELF_SYMBOLS_CORRECTIONS.tsv`: the corrected 4-column self_symbols.tsv line for each address, the current line(s), and
  whether the current column 2 already agrees. NOT applied.
- `evidence/`: vland logs (HEAD, new, new with LANDVERIFY_STRICT_LEAF=1) and the base-flag disasm diff for 009e15e0.
- `tree/`: a private minimal tree. It symlinks build_candidates.ps1, oracles, tests, include and tools, and holds real copies
  of the 17 edited sources. recheck.py ran with this tree as cwd, so its own regex read the new Leaf lines (`new/payload.json`).

## Method
- The leaf is the full mangled symbol the row's object defines for the function at the retail address, taken from a base-flag
  (/O2 /Oy /W3 + entry flags) compile. Each leaf occurs exactly once as a function symbol in its object. In 00c5eca0 and
  00cac300 the leaf is also a substring of a `__ehhandler$` symbol, but that symbol sits at a nonzero offset inside the `$L`
  block and does not split it.
- Placement follows the landed convention: line 2, after the title comment or a leading `#pragma` (00647308). For 009e15e0,
  whose first lines are `#include`s, the line goes after the include block.
- Grades: `vland.py new/payload.json new/oracle.tsv --recheck-landed` gave 16 wins and 0 `WARN leaf` lines. A second pass
  with `LANDVERIFY_STRICT_LEAF=1`, which requires the graded symbol to BE the leaf, gave the same 16 MATCH/RELOCATION_MATCH
  PASS and 0 LEAF_MISMATCH. The oracle is identical to the HEAD oracle.
- Why the B rows misgraded: vland picks the largest function when the leaf matches nothing. In the /O2 object that was a
  30-byte `??_G` (004af220, 0056c840, 00b6d4b0), an 18-byte helper (00647308 `??9iterator`, 00c25790 / 00c620b0 inline
  ctors), the 40-byte inline `IsThingAlive` helper (the six IsThingAlive rows), or a 32-byte tie that max() resolved to the
  `??_G` (00c1d4e0). The landed bodies themselves were byte-exact all along.
- C rows: in 00c5eca0, `Kill` is a substring of `?NotifyKilled@`. 00aa9190 and 00cac300 had no Leaf line at all.
  recheck.py derived `??1CChunkedFile@` and `Initialize` from the catalog names, and those name the wrong function in the TU.
  00aa9190 is the 95-byte `??_GCChunkedFile@@UAEPAXI@Z`, as the source's own header says. 00cac300 is the 241-byte ctor.

## HOLD: 009e15e0 (real DIFFER)
The largest function already was the row's own `?Erase@CQuickDrawTriInfoVectorView@@...`, so the correct leaf grades the same
DIFFER(99v99). The lengths are equal but register allocation and scheduling differ:
- Built loads arg1 `[esp+4]` into eax before the pushes. Retail pushes ebx/ebp/esi first, then loads `esi=[esp+0x10]`.
- The ebx and ebp roles are swapped through the loop and the epilogue.
- The loop compare uses `lea edx` in the built code and `lea ecx` in retail.

The test compile hit vland's 180 s cl TIMEOUT in all three runs (HEAD, new, strict), giving TCC_FAIL, as NF388 also recorded.
Per the loop rule I did not chase it. Not reauthored. The Leaf line is still worth landing, but HOLD it with the row.

## self_symbols.tsv (not applied; one name per address kept)
- Column 4 (leaf_given) is stale for all 17 rows. It should become the leaf.
- Column 2 is wrong for 9 rows: the six IsThingAlive rows hold the helper `?IsThingAlive@@...`, 00c5eca0 holds
  `?NotifyKilled@...`, 00aa9190 holds `??1CChunkedFile@@UAE@XZ`, and 00cac300 holds `?Initialize@CDynamicOutputPin@@QAEJXZ`.
  Corrected lines are in SELF_SYMBOLS_CORRECTIONS.tsv. Column 3 should become the new grade.
- None of the chosen leaves is the self symbol of any other address. The only exceptions are the EH unwind funclets of
  00c5eca0 (01216360..01216394) and 00cac300 (012170c0/c8), which already carry the parent symbol by the existing funclet
  convention.
- 00cac300 needs a coordinator ruling. The object symbol, which is the only one that grades by name, is
  `??0CDynamicOutputPin@@QAE@PBDPAUCBaseFilter@@PAUCCritSec@@PAJPBG@Z`. The vendor (retail) spelling in the source's
  Vendor-proof line is `??0CDynamicOutputPin@@QAE@PADPAVCBaseFilter@@PAVCCritSec@@PAJPBG@Z`. I give the object spelling in
  the correction file.
- 007fb120 needs a ruling. Its corrected column 2 (`?IsThingAlive_007fb120@@YI_NAAUCHolder@@@Z`) is a new name for an address
  that reloc_bindings.tsv already names `?IsTargetAlive@CTCKraken@@QBE_NXZ`. For the other reloc-named rows, column 2
  already holds the object symbol, so nothing new is introduced.

## Existing naming debt seen (not HOLD reasons; out of scope for a Leaf-only change)
- STANDIN_LEAF on 14 rows. Address-only names: IsThingAlive_<addr> x6, CWound_00c5eca0. Leaf names another function than the
  catalog: 004af220, 00647308, 00b6d4b0, 00c25790, 00c620b0, 00aa9190, 00cac300.
- reloc_bindings.tsv names these targets differently from the object symbol:
  - 0056c840: `??1?$pair@QAVCMenuEntry@NUISystem@@VCBoast@CQuestManager@@@std@@QAE@XZ`
  - 00647308: `?OracleSymbolSize@CDrawQuestInfo@NPlayerGui@@QAEJXZ`
  - 007fb120: `?IsTargetAlive@CTCKraken@@QBE_NXZ`
  - 009e15e0: `?erase@?$vector@VCQuickDrawTriInfo@@...`
  - 00c25790: `?GetTimeImpl@CSequenceClock@@QBE?AUCSequenceTime@@XZ`
  - 00c620b0: `?ToVector@CGridCoordinate@@QBE?AUCGridVector@@XZ`
- WARN RELOC_CONFLICT / RELOC_NAME on 004af220 and 0056c840 (dtor stand-ins at 0099b510 / 0099eae0) and on 00b6d4b0
  (`??1A` / `??1B`). STANDIN_LEAF and RELOC_* are new in the logs only because vland prints them once a row matches; the HEAD
  log has none. This is existing debt that the fix now surfaces, not a change it makes.
  RELOC_CONFLICT means `??1CCharString@@QAE@XZ` binds to 0099b510 in 004af220/0056c840 and to 0099eae0 in landed 00433530:
  one symbol at two addresses. This is a naming fix the coordinator should take up; the rows stay ACCEPT per the task rule.
  WARN SHARED_HEADER_AVAILABLE on the IsThingAlive rows, 00aa9190 and 00cac300, and VENDOR_PROOF_MISMATCH, are unchanged from HEAD.
- 00cac300 VENDOR_PROOF_MISMATCH: this lane has no dxsdk-sum2004 tree under FABLE_THIRDPARTY. Same at HEAD. The leaf takes no part
  in it: verify_and_land.py:643 calls library_proof, which uses lib_verify.parse_claim, and that reads only the source's
  `// Vendor-proof:` line. So the vendor proof was not exercised here, but the Leaf spelling cannot change its outcome.

## Includers
Unwind funclets `#include` the sources of 00c5eca0 (01216360..01216394) and 00cac300 (012170c0/c8). The change is comment-only
and recheck.py reads each row's Leaf from its own text (the include is not expanded), so their grades cannot move. Not regraded.

## Locks
All 17 locks were taken as NF401 and are now `released`.
