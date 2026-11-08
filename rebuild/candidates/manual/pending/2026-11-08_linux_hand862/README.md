# Chunk 862 (2026-11-08): seven tiny static initializers + two never-attempted rows — 7 of 9 matched (landed `batches/2026-11-08_landing_k/`)

Matched (all RELOCATION_MATCH on the first grade): the `_$E1` initializers of RIGHT_ / LEFT_ / BACKWARDS_RIGHT_ / BACKWARDS_LEFT_FORTY_FIVE_DEGREES_ORIENTATION
(0x0121AC70 / ACA0 / ADC0 / ADF0), the dynamic initializers of `hit_pos` / `hit_dir` (0x0121F590 / F5C0; both PDBs), and the initializer of
NScript::CQ_SunnyvaleMasterData::RelationSuffixes[3] (0x01224CE0; array name descriptive, chunk 780's spelling).

| Parked row (differ/) | Best | md | Attempts | Residue |
|---|---|---|---|---|
| 0x00531EC0 NUISystem::CComponent::Update(float) | 3455v3642 | 407 (827 at start) | 63 + 18 diagnostics | inline budget only: retail calls ConvertVector at loops 1-2 and expands it at loop 3 (diagnostic /d2inlT52 reproduces the split); ~_List_base out of line for THIS_INSTANCE |
| 0x00DAADD0 NScript::CQ_NewOakValeIntroScript::Init | 3406v3306 | 350 (first 609 instructions identical) | 22 + 98 diagnostics | retail expands `_Construct<CCharString>` at the first 22 push_backs and calls 0x0044B110 at the last 20; every knob was all-or-nothing |

- **Owner question (raised by the agent):** is a per-TU inline knob acceptable for rows like 0x00DAADD0 / 0x00531EC0 whose only residue is the
  inline-budget split? (Current rule: no -- inline-budget parks.)
- Header: engine/NUISystem_CComponent.h opt-in FABLE_RETAIL_NUISYSTEM_CCOMPONENT_MEMBERS (Deletion +0xD4 as CDeletion, PositionChildren +0x11C as
  std::set; 9 landed includers rechecked) -- serves only the parked 0x00531EC0, not landed.
- Research rules that helped: the member `operator+` x87 rule (swap the operands: md 597 -> 414); c2prio showed the case-1 register difference
  follows the inline decision, not an allocator tie.
- Labels: LABELS.tsv; 0x0122AEF0 (landed as CFontBankStateBlock::CreateCRC) is the RelationSuffixes atexit destructor `_$E2`, and 0x00535000
  (landed as Move_CountedPointer) is STLport `__copy<CCountedPointer<NUISystem::CComponent>*,...>` -- both queued `land0811l`.
