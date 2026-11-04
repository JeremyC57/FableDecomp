# Landing 2026-11-04zg: chunk 769 (766's 12 unattempted groups-ledger bodies, mostly libpk), 2 landed + 4 held for vendor naming

## Result
- **Agent result:** 6 of 12 matched (RELOCATION_MATCH + PASS, read-only on the merged tree, `logs/new2.log`), 3 DIFFER, 1 lane gap,
  2 not graded. There is no asm, helper, pragma or `volatile` anywhere.
- **Landed 2** (`logs/land.log`):
  - 0x00862AA0 `CModeStandBase::UpdateWalkingAnim`, as Ego_r.pdb spells it: private, `AAE`.
  - 0x00C47F10 `CAsyncDecoder_00c48640::Run` (slot 5 of vtable 0x012B8020), on the placeholder class that the landed ctor 0x00C48640
    defines.
- **Held for vendor naming V2** (byte-matching sources, tests and labels in `pending/2026-11-04_linux_hand769/held_vendor_naming/`):
  - **0x00C12940 StartEvent:** only a descriptive name. The landed caller binds a garbled `?Lookup@CTCHeroExperience_00c12cd0` for
    it.
  - **0x00C89350 ProcessRamped and 0x00C38D60 CInstance::Update:** their leaves differ from the spellings their landed callers bind
    (parameter types; `KKK` against `JJJ`). Landing them now would give each address a second name while the vendor naming pass is
    working on libpk.
  - **0x00C54190 CDecodedStream::SetProperties:** the class name has one source, and the 16 setters it calls are landed under four
    class spellings (PROPOSAL queued).
- **Parked:**
  - 0x00C12E50 PlayStreamed (741v741: one store-order line)
  - 0x00C4DE00 KeepShadowCopy (edi/ebp swap)
  - 0x00C3F9D0 InitSubSystemB (VC folds four flag clears into one mask)
  - 0x00C6F3E0 (lane gap: LTCG ecx-register callee + MMX)
  - not graded: 0x00C271F0, 0x00C3BF30

## Coordinator name fix (Fable, two sources)
- Ego_r.pdb has `CModeStandBase::UpdateIdleAnim` 0x00862780 and `UpdateWalkingAnim` 0x00862AA0 as private (`AAE`).
- The landed definer 0x00862780 and caller 0x00862EC0 used the public `QAE` form. Both now declare the pair private.
- The two bindings were respelled.
- Re-graded read-only (`logs/recheck_cmodestandbase.log`): 0x00862780, 0x00862EC0 and 0x00862AA0 are all RELOCATION_MATCH + PASS.
  - 0x00862780's RELOC_NAME warnings on four callees predate this change.
- **Queued (`land0411zg`):**
  - `CModeStandBase::OnActivate` / `OnDeactivate` are virtual (`UAE`), but callers 0x008E7B90 / 0x008D8F30 bind `QAE`.
  - The 26 libpk NAME_FIXES from the agent; one is a PROPOSAL for a single class for the 0x00C53440 setter family.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04zg.tsv` (2). The agent's 17 libpk labels (`LABELS_vendorV2.tsv`) are held for the
  vendor naming pass.
- **Checks:**
  - `bindings_multi_check.py` prints 0.
  - `sync_manifest_from_catalog.py --dry-run` prints 0 / 0 / 0.
  - No headers changed.

## Coverage
46,855 genuine of 49,568; 5,844,414 function bytes (57.18%).
