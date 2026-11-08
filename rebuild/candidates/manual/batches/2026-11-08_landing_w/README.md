# Landing 2026-11-08w: chunk 873 (game rows, Opus high), first pass: 2 of 10

Both RELOCATION_MATCH + PASS. The agent was sent back to work the other eight rows (four split extents now cut to their
true lengths, four not yet attempted).

| Row | Device |
|---|---|
| 0x00488D10 CPlayer::InitInterfaces (`?InitInterfaces@CPlayer@@AAEXXZ`) | plain C++: 66 `scoped_ptr::reset(new CInputProcessX(init))` in retail order on the new FABLE_RETAIL_INPUTPROCESS header family (CAInputProcess, CInputProcessGameBase, CInputProcessInventoryBase, 37 leaf opt-ins, 29 new leaf headers) and FABLE_RETAIL_PLAYER_INPUTPROCESSES in CPlayer.h |
| 0x005DD670 CAnimationEntry::StatRegisterClass | Inline-budget (owner decision 2026-11-08): `__forceinline` (MSC only, plain inline elsewhere) on CVectorMap::Insert(const K&, const V&) under the new opt-in FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE; retail expands all 26 Insert calls (Ego_r has no out-of-line Insert) and the caller budget then splits push_back 17 expanded / 9 called as retail |

- Headers: 55 shared headers gain additive opt-ins (hdrcheck: default path and every existing opt-in unchanged), 35 new headers
  (input-process family, CAnimComponentBase / CAnimationEntry / EAnimComponent / CAnimComponentStrikeSpeedMultiplier,
  CJoystickPositionTracker, CInputProcessGameBaseInit).
- Extents: OnHit 0x00839940 (5195), JackOfBladesStage1::Update 0x00975BB0 (4707), CTCDivineWrath::FrameUpdate 0x0078FD60 (9268),
  TesselateEdge 0x00BE3970 (2390) hold 14 more function starts; manifest boundary rows and BOUNDS_FIX rows added.
- Names: `ghidra_out/labels_landing_2026-11-08w.tsv`; respellings queued (land0811w), incl. InitInterfaces' binding (private AAE)
  and 0x013B8C1C CAnimationEntry::RegisteredComponentsByToken. Follow-ups: landed TransferBinaryIn 0x005DD090 should move to the
  shared CAnimComponentBase header (slot 4 Alloc / 5 Clone); rows on the older FABLE_RETAIL_CVECTORMAP_INSERT
  (`Insert(const value_type&)`, in neither PDB) should move to _INSERT_KEYVALUE.
