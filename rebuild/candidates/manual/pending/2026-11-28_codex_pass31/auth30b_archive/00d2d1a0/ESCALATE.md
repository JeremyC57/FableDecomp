# 0x00D2D1A0 CBattleMinion::Main -- ESCALATE (AUTH30, 2026-11-27)
- Bytes: c0 (Codex, two overlay opt-ins) RELOCATION_MATCH 620/620.
- NEQ copies, each checked against HEAD objects compiled here (head/):
  - `??_GCIsThingAlive` (39 B, vftable-reached): identical (bytes and relocations) to landed 0x00DA7830 and 0x00DA7D30. HEAD debt.
  - `??1CIsThingAlive` (14 B at 0x00E241B0, unreached): identical to the same two. HEAD debt.
  - `??_GCBaseClass` (30 B, unreached): identical to landed 0x00E50FD0 and 0x00EA2720. pk_all's 0x0099A470 comes from a wrong
    self_symbols line (NAME_ANSWERS: 0x0099A470 is ??_GCBaseClassNonCopyable, PROVEN_RTTI). Debt.
  - `??_GCScriptThing` (46 B v 87 at 0x004AB170, reached through ??_7CScriptThing): HEAD copies are 30 B (00DA7830, 00E50FD0) or
    87 B (00DA7D30, 00EA2720), never 46. NEW: this blocks.
- The 46-byte body is shaped by the row's own devices: ~CScriptThing defined under `#pragma inline_depth(1)`, plus several more
  inline_depth regions round shared-header includes and FORCEINLINE ctor/dtor definitions of shared classes.
- vftables agree. One RTTI-less table is not placed: `CSpawnedFunc1<CQ_EndGameFocalSitesScript, CScriptThing>`.
- Question: same per-TU inline-budget class as 00e0ca50 (the leaf and the copy need different ~CScriptThing inline settings).
  Object mode, or a ruling on HEAD-equivalent forms?
Grades: c0 only.
