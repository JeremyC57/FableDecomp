# Chunk 607 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A last-found cache test that ASSIGNS the node in its own arm (`if (cache && cache->Key == *key) z = cache; else { search }`)
  colours z like retail (edx). Seeding z from the cache and re-testing it (`z = cache; if (!z || z->Key != *key)`) puts z in
  ecx. Fixed 0x00C1B7D0 at 76 diff lines, 213v213, after two sweeps had called it a register-swap gap.
- STLport list scans and push_back over raw node pointers make VC7.1 shrink-wrap the callee-saved pushes past an early
  `return` (pushes after the test, the early exit popping only ebx). Real list iterators with a user copy ctor and by-value
  begin()/end() put all four pushes back at entry (0x0082A890, 32 -> 24 lines). This is the 2026-10-21 retry-10i lesson
  again: apply it to every inlined list loop, not only to push_back.
- Iterate on a short out-of-line twin when the row inlines it: GFAYToVY 0x009E1DD0 (49 bytes) shows the same
  `mov ecx,eax` / `fstp` residue as GFABoxToVBox 0x009E2020, which made 15 probes cheap. `$S/w22/agent607/qx.py` grades any
  address straight from Fable.exe without an oracle row.
- Re-prove parked signatures against the PDB name set: 0x0082A890 takes `const CThing&` (its substring Leaf hid the
  pointer form), 0x0087B840 OnInitLeaf is virtual (UAE), and SetMarriageState is the private
  `AAEXPBVCThingAICreature@@W4EMarriageState@@@Z`.
- Residues no lever moved this round (park after 2-3 grades): two-call float comparators (0x00807250, 0x00807170), store
  sinking past `fstp [esp]` / push (0x004BC800), `pop edi` before an inlined base vtable store (0x005954AD), a load hoisted
  into the dead `this` register (0x006D00A0), and register colouring with equal use counts (0x0087B840, and 0x0092D6F0's
  two-block form).

Review at landing:
- 0x00C1B7D0 keeps a same-TU `__declspec(noinline)` stand-in body of CMapNodeAllocator::Release (0x00C1B1E0, already
  landed under the same symbol). The object therefore defines `?Release@CMapNodeAllocator@@QAEXPAPAUPKCTRRedblackNode@@@Z`
  next to the Remove leaf. Only Remove is graded and landed; vland showed no RELOC_IDENTITY.
- 0x00C1B7D0 calls Release with `(PKCTRRedblackNode**)&y`: the landed Release spelling names a different node type from the
  landed DeleteFixup (PKCTRRedblackMapNode). Both names are descriptive; pick one at a naming pass.
- NAME_FIXES.tsv: respell the landed callers of 0x0057BC6E (CTCWife::Died 0x006D0020, Divorce 0x006D0180), 0x00760DA0
  (UpdateDesiredPos 0x0087B910) and 0x0049D870, and the stale bindings at 0x00761B50.
- Rows that took more than 6 grades: 0x00C1B7D0 (8 incl. naming), 0x0082A890 (11), 0x0092D6F0 (10), 0x0087B840 (7),
  0x009E2020 (15 sibling probes).
- 0x005954AD's parked source is full of stand-in names (TreeNext_004292c0, CObserverMember_0052e050, CPlainVector, invented
  map/vector structs). It must be respelled even if it matches later.
