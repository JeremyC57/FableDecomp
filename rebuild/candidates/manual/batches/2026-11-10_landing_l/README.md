# 2026-11-10 landing l -- chunk 899 (never-attempted rows), 19 rows

Source: `pending/2026-11-10_linux_hand899/` (one Opus expert agent on 29 unworked rows, avoiding the Codex-reserved rows).
The agent matched 25. Nineteen landed (vland --land, no overlay: 11 MATCH + 8 RELOCATION_MATCH, tests PASS).

Landed: libpk / CODA vtable forwarders and small members, named descriptively per the owner's vendor-name rule (no PDB):
CCollectAllItems::Visit 0x00C10720; Validate on four item-table holders 0x00C1FD50 / FDF0 / FE90 / 0x00C202C0;
CSoundEmitter_00c37ad0::ReleaseA / ReleaseB 0x00C37A00 / 0x00C37A30; CCodaFrameStreamer_00c49650::GetFreeFrameCount 0x00C49610;
CUnprovenReverbParameters::SetRoom / SetRoomHF / SetRoomLF 0x00C53410..30; CKsObject ctor / IsValid / GetHandle / WriteStream
0x00C5DFD0..0x00C5E000; CVtable_012bf0c8::StartBlend / StopBlend 0x00C77410 / 30. Game code (two PDBs): the dynamic initializers
of CEngineInternalPrimitiveBase::CurrentInterpolationInfo 0x01224900 (new default-identical opt-in
FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTINTERPOLATIONINFO) and TableBuilder 0x01224970.

Coordinator changes before landing: 0x00C37A00 / 30 `Release` / `Stop` -> `ReleaseA` / `ReleaseB` (identical self-destroy bodies;
"Stop" did not describe the body); CEngineInlineTableBuilder keyed `struct` (Ego_r @STRUCT). Re-graded RELOCATION_MATCH + PASS.

Not landed:
- 0x00CA6170..90 / 0x00CAA720..40: DirectShow BaseClasses `adjustor{4}'` thunks. They match as C++, but strmbase is third-party
  vendor code (owner, 2026-11-08: vendor route via lib_verify, not chunk material); sources kept in the hand899 `vendor_route/`.
- 0x00C3EC10: EXTENT_WRONG (two 11-byte CCodaSystem forwarders), split recorded in `BOUNDS_FIX.tsv`.
- 0x005C9A95 / 0x005C8459 / 0x005B8743: identities settled (both PDBs), not drafted; they need a CTCInventoryWeapons header.
Warning at landing: RELOC_NAME 0x00C49C30 (queued). `bindings_multi_check.py` prints 1, unchanged.
Names: `ghidra_out/labels_landing_2026-11-10l.tsv`; queue `land1011l` (0x00C49C30 CMailboxQueue::GetCount, the global 0x0143B8E0,
strmbase CBaseInputPin::AddRef / Release 0x00CA53F0 / 0x00CA5510).
