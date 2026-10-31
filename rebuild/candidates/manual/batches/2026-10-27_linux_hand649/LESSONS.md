# Chunk 649 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- 0x0093DAE0 (`CAIStateGroup_MeleeCombatNearby2::IsCombatSequenceActionPending`): the retail register choice needs the
  Ego_d.pdb inline accessors, not raw member chains. `CAIBrain::GetCreature() const { return *Creature; }`,
  `CAIStateGroupBase::GetCreature() const { return PBrain->GetCreature(); }`, `CWorld::GetCombatManager()` and
  `CAIStateGroup_MeleeCombatNearby2::GetCombatManager()`, plus a named `CCombatManager& manager = GetCombatManager();`
  reference local before the call. A raw `PBrain->Creature` loads the creature into edx; dereferencing
  `*GetWorld().PThingCombatManager` directly gives 50 bytes instead of the retail size.
- 0x009E21D0 / 0x009E1DD0 (GFACoordToVCoord / GFAYToVY): the Y path is `(float)((float)VIRTUAL_SCREEN_HEIGHT * y) / dims.Y`.
  The explicit outer `(float)` truncation makes the compiler evaluate the product early and spill it into the dead
  argument slot, the way retail does. The X path (GFAXToVX) has no outer cast. Both read the size through
  `GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions()`, which returns `const C2DExtentsI` by value
  (C2DExtentsI needs user constructors and a copy constructor to get the hidden-return shape).
- Renderer SetState trio (0x00BF01E0, 0x00B5E0F0, 0x00B6FA40): VC7.1 picks the `test`/`or` operand roles by a register
  priority tie-break that depends on how many expression temps the whole function has. Dead loads through a param or
  `this` pointer change the count and flip the roles: 0x00BF01E0 needs 3 or more dead `effect->StencilBit` loads,
  0x00B5E0F0 needs 5 or more dead member loads, and 0x00B6FA40 needs fewer temps, so dead loads make it worse. Dead
  loads of globals, constants, pointer copies and extra inline-wrapper params do NOT change the count. This is a lever
  probe, not a reconstruction (see lever_dead_loads/).
- The real Ego_r.pdb model for these is `CRenderStateManager::CStateInfo`, `SetRenderState`, `AddStateToUpdateList` and
  `CDeviceState`. Modelling it as member functions fixes the test/or roles with no dummies, but it moves `mov ecx,eax`
  after `mov edi,esi`, so it is still one residue away.
- rd.py-style reloc-masked diffs must not mask jump targets: masking `j*` operands hid the 0x0086BFB0 jmp residue.

Review at landing:
- Owner decision on the dead-load lever: lever_dead_loads/00bf01e0.cpp and lever_dead_loads/00b5e0f0.cpp are
  RELOCATION_MATCH only because of dead loads. They are marked "LEVER PROBE, NOT A RECONSTRUCTION". Do not land
  them without an owner ruling.
- Bonus bonus/009e1dd0.cpp (GFAYToVY, tag a649_1dd0b) is RELOCATION_MATCH with a g++ test. It is not a chunk-649 row,
  so it is not in payload.json; land it separately if 0x009E1DD0 is still open.
- Respell the callers listed in NAME_FIXES.tsv:
  - GetPWorld becomes `?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ` in landed 0x0093E240, 0x0093DDE0, 0x0093DB70
    and 0x0093DCD0.
  - Landed GFAXToVX 0x009E1DA0 has view-struct names and `volatile`; it should use the GFGetSystemManager /
    GetRenderTargetDimensions spellings.
  - GFVYToAY uses `g_VirtualHeight`; the correct name is `?VIRTUAL_SCREEN_HEIGHT@@3NA`.
- Member mislabel in landed 0x00BF1030: "m_stencilRef" at +0x2BC4 is StencilOpPass, and "m_stencilPass" at +0x2BEC
  is StencilRef (Ego_r.pdb CDeviceState).
- 0x00563480: the pending copy is not the one-residue version reported earlier. Re-derive from the earlier grade
  before reworking it.
- Several rows were graded more than 4 times because probes ran in parallel. The grade logs carry a QTAG for each run.
