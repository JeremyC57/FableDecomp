- Wine cl.exe segfaults (rc -11, vt.py "TCC_FAIL" with no error text) when TMPDIR/TMP point at the session scratchpad: `unset TMPDIR TMP` first.
- fg.py shows only the first SRC_FAIL errors (8 of 102 on GFMain): compile with /Zs through verify_and_land's env for the full list (00402510/zs.py).
- An "unlisted" start (gap split) grades fine but stock vland stops at OUTSIDE_MANIFEST; add the start in memory for the self-check (00407a70/vland_split.py).
- EH funclet rows: an oracle that spans funclet + __ehhandler thunk (18 bytes) still matches at funclet_offset 0 (the $L block is exactly that).
- Header debt rows: first drop the draft's dead code (helper bodies landed elsewhere) and stale defines; most needed opt-ins already exist.
- vland RELOC checks cover functions only: data-global spelling conflicts pass silently, check reloc_bindings for every global by hand.
- (B24C2) Register/slot residue in a TU with FableWin debug copies: run fwsize.py on the row and fix every helper it calls DIFF; the
  FableWin-exact FindWaterVert text (early return + while loops) fixed the frame layout of 0x00BE4FE0 at once.
- (B24C2) `or reg,-1` constant hoist missing: look for a CSE'd load that steals the register (re-read through the accessor, not the ref).
