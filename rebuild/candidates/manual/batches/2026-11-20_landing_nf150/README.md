# Landing nf150 (Account A, 2026-11-20): CEngine forwarders, CRenderManagerCore draw calls, navigator names (15 rows)

Agent NF150 (round 10) handed back 15 entries (13 done, 2 blocked on other rows); its files are in `naming/2026-11-20_agentNF150/`.

- Self renames (RENAMES section 1, 14 rows): each old self leaf is bound by no landed row and each new leaf is already bound, so every
  address's spelling set shrinks (advisor decision, owner delegated; the nf120 / nf139 rule). This covers 0x00A0AF40 (0x00BB8090 still binds
  the CRenderManagerCoreView stand-in) and 0x00A0AD40 (the July row 0x00BF4570 binds nothing there).
- File moves: 0x00B4A5F0 -> CEnginePrimitiveRenderer2D_SetLightingParameters_00b4a5f0.cpp, 0x00A14E20 -> CNavigatorManager_FrameUpdate_00a14e20.cpp
  (Module CNavigatorManager), with their tests.
- 0x00A1A170 CNavigatorAStar::IsEqual: the private header inline's COMDAT emitted through an explicit instantiation in an anonymous
  namespace (`// Fidelity: scaffolding, not retail`); accepted as an advisor decision (owner delegated) and written into EXTRA_RULES.md.
  objdump: the helper is in the per-TU `?A0x...` namespace.
- Header: engine/CEnginePrimitiveRenderer.h, add-only, eight new opt-ins with no existing users; `orig` equals HEAD.
- Recheck (`logs/recheck.log`): 6 MATCH + 9 RELOCATION_MATCH, every test PASS. rebind multi-check 1 (baseline); split gate 0 created;
  tablecheck: 0x00A20970 shrinks 2 -> 1.
- Includers (`logs/includer_recheck.txt`): 95 good; 1 bad, the July row 0x00BF4570 at its HEAD grade (316v4431).
- Link (`logs/link_landing.log`): one LNK2005, CRenderManagerCore::AttachTextureToStage, the header's FORCEINLINE_HELPERS COMDAT (now
  emitted by 0x00A0AEA0) against the strong owner 0x009A0CF0. HEAD rows 0x00BBF4A0 / 0x00BB2540 / 0x00BBC460 / 0x00BE0EB0 emit the same
  COMDAT, and HEAD 0x00BBF4A0 + 0x009A0CF0 give the same LNK2005 (`logs/link_control_head.log`), so it is pre-existing; queued as DEBT.
- Lock on 0x00BB8090 released (q6297's binder, blocked after 3 forms).
