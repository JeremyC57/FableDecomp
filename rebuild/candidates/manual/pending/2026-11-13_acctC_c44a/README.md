# acctC hand-back c44a (agentC44, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x0058807D | CTCInventory::OpenPCInventoryMenu | 4,997 | RELOCATION_MATCH | PASS |

Verified by the coordinator with `lane/coordverify.sh` (read-only vland, strict headers): `coordverify.log`.
Hand-authored from the outline in the idioms of the landed twin OpenPCSkillsMenu 0x00587BFF; matched on the 4th grade.
No header change is needed by this row, no new opt-in, no flag; only `#pragma optimize("s", on)` as its landed siblings.
`hdrdiff/` is the whole overlay delta of the work dir (most of it belongs to other rows): the row itself uses repo headers
plus existing opt-ins. Local models of CThingPlayerCreature and CAbilityDef, exactly as landed 0x005A8C66 / 0x00584B86.
The test is a twin-style model test (three scenarios); no mutants were run.

`open/00652fa4.cpp`: NPlayerGui::CDrawQuickAccessIcons::Draw, DIFFER(4653v4653), ONE instruction out of place (the flash
sprite's `fst [ebp-4]` one push early, 0x006540C2 v 0x006540C3). Review items if it closes: FITTED two-local
`CIntelligentPointer<T>::operator->` text (opt-in `FABLE_RETAIL_CINTELLIGENTPOINTER_ARROW_CHECKED_LOCALS`; FableWin shows the
default `return Get();` is wrong — 336 b / 92 i — but the asserts compile away so the text cannot be size-proven), the seal
spelling, FORCEINLINE ToC2DCoordI. EVIDENCED: sprite positions copied through the PDB-named inline
`NPlayerGui::GFToScreen(const C2DVector&)`.
