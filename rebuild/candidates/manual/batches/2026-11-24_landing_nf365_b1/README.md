# nf365 b1: the GetDef "swap" in ten /O2 rows (GetDef<NUISystem::CUIMiscThingsDef> 116 -> 98 B)

Agent NF365 (Account A naming queue; hand-back `pending/2026-11-24_nf365_handback/`), from NF358's census and NF361's held/g1. Rows:
005fc2d0 0047a9f0 005fc8a0 005fcbe0 005fddd0 006001e0 00600b60 0061b610 0061e6d0 00622a00. Each row drops its row-local
`template<class T> GetDef(long, ...)` and defines FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY + FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
+ the NF349 per-type size-body macro, so the GetDef copy is the owner 0x0042B312's 98 B body (byte- and relocation-equal to the owner object).

Ruling (advisor decision, owner delegated, 2026-11-24): the swap is a copy repair gated by the drop test, provided the helper COMDATs it
brings equal those of already-landed shared-body rows. Step 1 (NF365, evidence/step1_fpcheck.txt): SetDynamicCast<T> 52 B and
Get<CDefClassBase> 3 B are byte-, relocation- and selection-equal to the landed GLOBAL_BODY + REREAD + SIZE_BODY rows 0x005E5F30 /
0x005E4BE0 / 0x006096E0 / 0x006D1250, so the swap brings the shared body's normal footprint.

Coordinator verification, HEAD and repo both built by the coordinator (`$S/v365H.sh`, `$S/v365N.sh`):
- vland `--recheck-landed`: 10/10 RELOCATION_MATCH PASS before and after; identical WARN sets.
- pk_all `--leaf` HEAD v repo: in every row only GetDef<CUIMiscThingsDef> turns NEQ -> EQ; 0 new NEQ; reachflip 0 NEQ flips (10 pairs);
  pickany_vs_retail 0 new NEQ lines.
- Drop test (`agent_tools/coordinator/droprow.py`, logs/droptest_coordinator.txt): 20 dropped DEFs, `?Assign@?$CDefPointer@$$CBVCUIMiscThingsDef...`
  and `?IsNull@?$CDefPointer@$$CBVCDefClassBase...`, all with no retail address (NOADDR) and no reloc_bindings line; no UND change.
Held for rulings: 00868e50 (held/g2: SetDynamicCast<CArmourDef> has no landed same-named reference), 0060c900 / 0060eeb0 (held/g3: their
already-NEQ strong same-TU CreateMapMarkers copy changes body). Skipped (leaf moves): 005ff190 4128v4176, 00605570 1841v1890, 00607330 9084v9124.
Note: in 005fc2d0 / 005fc8a0 / 005fcbe0 / 005fddd0 the "Same-TU body of the def lookup" comment above the now-empty auto_inline pair is stale;
left as is (comment-only fix for a later pass).
