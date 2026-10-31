# Chunk 642 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Source order of paired bounds sets the prologue registers** (0x00B41C30). `minX, minY, maxX, maxY` (mins first) gives
  retail's "load y, x + radius in ebp" prologue and its ecx/edx re-centring block. The x-pair order `minX, maxX, minY,
  maxY` gives the same length with x + radius in ecx and an eax/ecx/edx block. Y-first and max-first orders were worse.
  Try every order of the four assignments before parking a "register scheduling" bounds row.
- **A same-length best attempt can still be wrong in its control flow** (0x007534B0). Its `je` targets differed from
  retail by one block: a null or dead creature must return false, but the nested `if (alive) { if (busy) return false; }`
  fell through. Check differing jump targets in a same-length residue before you tune scheduling.
- **To keep a shared `return false` block inline after an `||` chain, use one inline predicate** (0x007534B0).
  `if (!IsAbleToLook(creature, animation)) return false;`, where the predicate is `IsThingAlive(c) && !IsBlocking(a)`,
  keeps the `xor al,al` block right after the last test. That matches retail: the earlier tests jump forward to it and the
  last test falls into it. A direct `!a || b || c`, split returns, or an if/else-return all sink the block to the end of
  the function (near jumps, +12 bytes).
- **Frame-slot sharing follows scope** (0x005FB4B0). Inline-parameter temporaries (AddFloat's `value`) reuse the slot
  of a named local only when that local's scope is closed. Moving the out-pointer into an inner `{ ... return; }` block
  let the angle float take its slot. A named filter declared before the marker sits above it, as in retail. Slot order is
  a scope and declaration-order problem, not a register tie. Experiment with block structure before parking a frame-slot
  residue.
- **Ego_r inline accessors are often byte-neutral, so respell anyway.** None of the following moved a byte, and all are
  kept for names: GetCreature (two levels), GetParentCreature, PeekThingSearchTools (CTCBase -> CWorld), GFRandom(range,
  &seed) in place of the open-coded seed step, const QBE/IBE spellings, and reference parameters in place of pointers.
  CTCBase::GetParentThing() is not neutral: it hoists ParentThing into a callee-saved register for the whole function
  (0x0065B142, worse).
- **A `#pragma pack(1)` member can be replaced with a byte image plus a reference accessor**
  (`*(CIntelligentPointer<CThing>*)PActiveRangedWeapon_`). This gives the same bytes and keeps the source pure
  (0x005CB128).
- **A POD iterator struct is no shortcut to a spilled loop bound** (0x0086B9D0). Member operators turned the returns into
  hidden pointers (438 v 389).
- Register-colouring ties confirmed again (two to three new levers each, no movement): counted-pointer Data/Info
  (0x00ACD960), this/map (0x0086B9D0), creature/map pointer (0x00911A90), this/weapon address (0x005CB128) and scratch
  rotation (0x00713D50). Park these on sight.

Review at landing:
- 0x00B41C30 matched at the first grade of this wave. It needs no more review.
- Rows that took more than 4 grades this wave: 005fb4b0 (7) and 007534b0 (7, including the control-flow fix). The
  others took 2 or 3 each.
- Local base models next to existing engine headers: 00acd960 (COptimisedPrimitive.h has no namespace and flattens the
  multimap), 0065b142 (CTCVillage.h / CTCBase.h flatten the vptr), 00911a90 (CAIStateGroupBase.h flattens the vptr) and
  005cb128 (the pack-1 owner layout). All are parked.
- Errors in already-landed sources:
  - 0x005FAD40 TeleportHeroToGuildSealRecallLocation and 0x005FBD90 TeleportHeroToGuild are spelled non-const. The
    PDB has QBE for both.
  - 0x005FAD40 spells 0x013B86A0 as `g_pEngine` (CEngine*) and HandleMoveHeroToRegionGameEvent with a `const
    CGameEvent*`.
  - Landed callers spell 0x00661540 AngleBetweenPositions and 0x00A441E0 AngleDifference (stdcall).
  - All of these are in NAME_FIXES.tsv.
- Two catalog rows are mislabelled: 0x00ACD960 is Optimise(float), and 0x00ACD500 "COptimisedMesh::CollapseSegments" is
  COptimisedMesh::Initialise(params).
