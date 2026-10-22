# Chunk 911 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Persist-switch traits placement decides the frame: a named `CPersistTraits<T> traits;` with a user `{}` ctor inside
  the case-2 block homes in the dead value-arg slot for a 4-byte T (0x00410BE0); a function-scope `traits` with
  `DefaultVal()` zero-initialises before the jump (0x007AD6D0); a non-trivial traits (CFaction) is built/destroyed
  around the switch (0x006BABD0).
- In a switch whose locals need a dtor after it, ending the LAST arm in `return` instead of `break` keeps retail's
  `pop ebx` after the dtor call (0x006BABD0).
- An inline traits specialisation `*value = stream->ReadC2DVector()` (non-POD C2DVector, hidden-pointer return)
  tail-merges the binary-load arm as retail does (0x00800B90).
- Several results selected later by a switch: assign the default-column variable FIRST in every branch to get
  retail's ecx/edx allocation; a `long` copy of an unsigned char member avoids a spill (0x009FD2B0).
- Retail testing the same component bit twice (word `test` then byte `test`) is two different expression forms: a
  dword `HasTC` guard plus a byte-indexed `TestTC` inside the inline accessor; one form is threaded away (0x0092B110).
- Size TU + frame pointer (`#pragma optimize("s", on)` + `("y", off)`) is the shape when retail has compare-tree
  (`dec eax; je`) switches, a `[ebp-4]` flag word and ebx/esi pushed after an early exit (0x0042E3EE).
- `if (f & 4 || f & 0x40)` folds to one `test bl,44h` but lays the arm out below the else-if chain (retail
  `jne far`); `if (f & 0x44)` keeps it inline (0x0042E3EE).
- A virtual taking `unsigned short` reorders `push eax` after `mov ecx,esi` versus a `long` argument from another
  call site of the same slot (0x0042E3EE KeyPressed: GetKey arm v GetChar arm). No native wchar_t in the lane.
- Test hygiene: chunked `ReadLong` models must read into a 4-byte `int` on the 64-bit host.

Review at landing:
- `gall.sh`/shared `q.py` grade 0x006BABD0, 0x007B5470, 0x0092B110 DIFFER because the chunk248/chunk257/bnd oracles
  hold older mis-cut rows at the same addresses (326/371/440 bytes); grade them against chunk911's oracle (q911.py).
- Local base models next to existing engine headers: 0x00A64420/0x00A64440 (local CMemoryAreaChunk; no engine header
  found), 0x00BE5D40/0x00BE5D60 (retail-view CLandscapeBackgroundTreeNode Map +0xA4, CEngineLandscapeMap,
  CEngineMap), 0x0042E3EE (CNewFrontendGameComponent: PDB 336 bytes, retail 8 lower after the two STLport vectors).
- Already-landed source with a wrong name: 0x00A64130 is ReadFromParserAsULONG, landed as SLONG.
- 0x0064DD9A needed more than 4 grades and is parked (lane gap). 0x0042E3EE matched on its 5th grade (one was a compile fix).
- 0x0042E3EE refers to 0x0129BA3C as `extern const float g_epsilon_0129ba3c` (same name as landed users).
