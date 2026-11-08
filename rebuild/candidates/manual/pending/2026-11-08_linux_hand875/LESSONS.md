# Chunk 875 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- q.py keeps ONE work dir per address ($S/wine/q/<addr>): two grades of the same address in parallel overwrite each other's object and
  report the other source's bytes (identical results for different spellings were the symptom). Keep a private copy of q.py whose work
  dir includes the source stem (this chunk: bin/q875.py) or grade one address at a time.
- `float d = vcall(); d += p;` vs `float d = vcall() + p;`: the two-statement form lets the unrelated `mov eax,[ebp+18h]` load schedule
  ahead of the `fadd` exactly as retail 0x00A1E634 (the "name the float call result" x87 rule, confirmed on a 5 KB function).
- A same-TU body of a tiny callee under `#pragma auto_inline(off)` (CSteeringAgent::IsCurrentNavigabilityBetter 0x00A1A1A0) lets VC7.1 keep
  edx live across its call (0x00A1F4A8..0x00A1F4BB), as retail does.
- Block-2 shape at 0x00A1F4A0: `iterator it = Route.begin(); iterator next = it; ++next;` and a TEMPORARY way point in the IsRouteClear call
  (Y-first float ctor) keep the front node in edx across the call; a named way point gives X-first loads + fxch and reloads the head.
- `LastWayPointVisited = position;` through C2DVector::operator=(const C3DVector&) stores straight into the member (retail 0x00A1FA9A);
  `= C2DVector(position)` stores a frame temp first.
- IsNonZero needed `__forceinline` (inline-budget device): Fable.exe has no out-of-line copy, while VC7.1 calls a plain-inline copy from an
  IsRouteClear-sized caller; /d2inlS10/15/30 and /d2inlT80/150 never expand it (S30 also expands GFGetSign/size/pop_front, which retail calls).

Review at landing:
- 0x00A1E5C0 needed 75 grades and is still parked (differ/00a1e5c0.cpp holds the best source and the c2prio evidence).
- The landed CNavigatorAStar_CanNavigateToPos_00a1bca0.cpp declares IsConnectedTo / IsAnyPartOfNodeWithinDistOfPos with non-PDB spellings
  (NAME_FIXES.tsv); CSteeringAgent_IsCurrentNavigabilityBetter_00a1a1a0.cpp's line 1 says `const` / long, the PDB says non-const bool.
