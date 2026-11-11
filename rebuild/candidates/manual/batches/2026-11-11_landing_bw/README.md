# 2026-11-11 landing bw -- agentNF15: 224 landed rows respelt to proven callee names (NF14 queue + CVectorMap LowerBound family)

Source: `naming/2026-11-11_agentNF15/` (STATUS / NOTES / LESSONS / RENAMES / NAME_FIXES). vland --recheck-landed in two halves: 224
RELOCATION_MATCH, VC7.1 tests PASS (0x0047D60D / 0x004C7C40 / 0x005B4753 / 0x008389E0 / 0x00898B30 print their catalog PassPatterns, checked by
running the exes). g++ PREFAIL on 41 rows is pre-existing (STATUS.tsv).
- Largest targets: 0x0040F020 CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound (26 rows: row-local CInterfaceMap stand-ins became the explicit
  CVectorMap specialisation), 0x00485815 GetSubDef<CCreatureDef> 25, 0x007EAB90 CTCActionUseBase::OnActionUse 16, 0x00686D60
  CTCBase::PeekComponent 15, 0x006C6990 PostDeed 13, CThing::PCurrentThingManager 9, PeekMyPlayer 8, GetDef<CThingObjectDef> 7 ...
- Own names (RENAMES section 1): nine `vfunc_28` rows are the virtual CTC*ActionUse*::OnActionUse(CThing&); 0x007BDB66 is
  NThingShot::CTCShotDefinitionNymphFun::OnCollision; CTCInventory* / CThing / CTCPhysics* access and virtual spellings fixed (leaf-only).
- Wide-text declarations in 0x00581765 / 0x0071B840 respelt `const unsigned short*` (owner rule).
Header: CTCBase.h opt-in FABLE_RETAIL_CTCBASE_FLAT_PEEKCOMPONENT (row 0x0071AA50), additions only on the current header.
Rejected queue claim: 0x00415DD0 GetString (byte-identical twins, one source). Left: 39 LowerBound rows, invented-namespace rows (006e11f0, 007565e0,
007adbf0, 006a23e0, 008af850), 005b43a2 push_back (DIFFER 946v945, reverted). Pre-existing name conflict queued: rows 0x00784260 / 0x007846B0 call
0x006C5920 (RemoveDeedStillSearchingForWitnesses) as RemoveDeed. Queued land1011bw: 14 claims.
Note: the agent reported one accidental `git -C /dev/null status` (no repository, no output); nothing was written to the repo.
