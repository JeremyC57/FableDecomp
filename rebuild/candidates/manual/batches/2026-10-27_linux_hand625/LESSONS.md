# Chunk 625 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Retail forms `this` before pushing the argument for a CIntelligentPointer assignment (`lea ecx,[esp+8]` before
  `push edi`, `lea ecx,[esi+24h]` / `mov ecx,esi` before `push eax`). That order means each conditional arm held its own
  whole `member = CIntelligentPointer<T>(x);` statement, and VC7.1 tail-merged the arms. One assignment after the
  if/else pushes first and can swap the callee-saved registers. Seen in 0x0093CAB0 (2 arms) and 0x006C0F00 (3 arms).
  This extends the switch-arm lesson from chunks 614-623 to if/else arms.
- Z, Y, X x87 register loads with X on top and X squared first (`fld [p+8] ; fld [p+4] ; fld [p] ; fld st(0) ; fmul st(1)`)
  come from an inline X*X+Y*Y+Z*Z on a const C3DVector& that is NOT copied. Copying the vector to a local changes the
  load or square order. Seen in 0x005FA990.
- Bitfields can sit in a mem-initialiser list (`Flag(false)`). VC7.1 emits one `and byte ptr [mem],0F0h` there, at the
  point where the init list runs. This is useful for placing a flag clear before body stores (0x00BDDF30: right slot,
  but retail splits the load from the and/store).
- A `double` local compared with `0.0f` folds the literal to double (`fld qword [0.0]`) but gives the
  `fld [zero] ; fld st(1)` compare order and keeps the value live past the merge. A `float` local gives the dword zero
  and the same order, but the value dies at its last use (`fdivrp`). (0x00C720C0)
- A reference local to an opaque header member (`T& m = reinterpret_cast<T&>(Member);`) makes VC7.1 keep the member
  address in a callee-saved register instead of `this`. Cast at each use instead (0x006C0F00).
- Tooling: q.py's work directory is `$S/wine/q/<addr>` unless QTAG is set. Two concurrent grades of the same address
  clobber each other and report each other's bytes. Set a distinct QTAG per probe (bin/gp.sh does).

Review at landing:
- 0x006C0F00 uses `reinterpret_cast<CIntelligentPointer<CThing>&>(MainSignPost)` because engine/CTCBuyableHouse.h keeps
  the member opaque. If the header gains the real type, drop the casts and regrade.
- 0x005FA990 keeps a local CWorld and CWorldMap next to the existing engine/CWorld.h and engine/CWorldMap.h. CWorld.h
  flattens the vptr and the vector. The CWorld virtual slots 0-12 are placeholders (v00..v12), as in landed sources.
- 0x00C720C0 needed more than 4 grades (9 probes over two waves). The best form is 2 masked lines.
- 0x009BEE40, 0x00BDDF30, 0x00EC3270 and 0x0051D870 had more than 4 grades in total, counting earlier waves.
- 0x0051D870 differ/ still uses the 2026-10-15 stand-in names. Its proven identity is
  std::vector<CNavigationAreaUpdate::CNavigationLayerUpdate>::resize(unsigned int).
- 0x00EC3270 differ/ should be respelled NScript::CV_RandomPopulationSimScript::GetCurrentPopulationLevel(POPULATION_TYPES).
- NAME_FIXES.tsv: 0x00A01B50/0x00A01B90/0x00A01C10/0x00686A80/0x00686A70 each have dozens of landed spellings. The
  coordinator must respell those callers.
