# Landing 2026-11-07d: held chunk 820 row 0x00DA91A0 CMapKeeper::Main (owner ruling)

Owner ruling 2026-11-07 on the chunk 820 question: "If the code behaves exactly the same and byte matches when you cast, then it
should be fine." The held source `pending/2026-11-06_linux_hand820/differ/00da91a0.cpp` (a same-type
`reinterpret_cast<const CScriptThing&>(escort).IsAlive()` on a by-value local, which keeps the call virtual as retail does) landed
unchanged except for a ruling note appended to its line-3 comment.

| Row | Grade |
|---|---|
| 0x00DA91A0 NScript::CQ_MinionClifftopChaseScript::CMapKeeper::Main | RELOCATION_MATCH + PASS, landed |

No new reloc bindings; the one RELOC_NAME notice reports an existing placeholder label of the map dtor 0x00CDBFB0.
