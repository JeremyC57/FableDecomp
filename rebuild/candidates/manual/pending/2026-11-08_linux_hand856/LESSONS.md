# Chunk 856 lessons (GOTCHAS one-liners)

- `mov reg,[StreamPos]; add reg,N` (coalesced) v `mov eax,[StreamPos]; lea ecx,[eax+N]` at an inlined CDataInputStream::Read is a global register-pressure tie in VC7.1, not a Read / ReadType spelling: the landed 0x00A2F2D0 gets the coalesced form only inside its high-pressure loop (0x00AAD650 parked after 116 grades).
- Retail sinks an x87 `fstp` of an expanded CScriptThing::GetAngleXY result past the next inline null test (0x00DDAE40, 0x00E03F70); our VC7.1 never does at those sites under any inline / cast / local / flag spelling: park as x87-order.
- An empty-loop edge that jumps past a vector-free reload (ours) v to it (retail) is the dtor temporary taking the iterator's register; STLport dtor shape and loop spellings do not move it (0x0050A650).
- A row that matches only with an `inline_depth(0)` pragma (0x00E799F0) is a structural park: say so, and record the probe.
- Check every sweep of mechanical variants for brace pairing first: a dropped loop brace leaves cl.exe spinning under Wine (and EXTRA_FLAGSETS retries start a second cl.exe); kill your own by PID.
- Overlay headers copied from pending dirs: carry only what a row includes (12 of 29 copies here were unused), and regrade every row with the trimmed overlay.
