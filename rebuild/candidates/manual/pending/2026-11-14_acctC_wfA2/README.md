# acctC workflow hand-back wfA2 (unworked rows, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x0068CB90 | CInputProcessControlFreeCamera::ProcessInput (catalog said "constructor") | 2,339 | RELOCATION_MATCH | PASS |

Verified twice (workflow verifier + coordinator, `coordverify.log`). Catalog length 2,656 was wrong (true 2,339).
Review items: `FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP` is FITTED (precedent: landed 0x00A114B0); a force-inline opt-in on
GFAngles' rounded-angle helper (shared header) is a device on byte evidence; `CInputEvent unused;` emits real ctor/dtor
bytes in retail (0x0068CB9B), it is not a dead local. WARN RELOC_NAME 0x00A0D390 (catalog label).

`open/`: 00dce230 CCrateTeamMember::Main DIFFER(2589v2589), 0048e78e DrawButtonGraphics DIFFER(2662v2662),
00439070 DrawPreparePrimitivesForEngine DIFFER(2613v2613), 00435530 DoRender (stale draft only).
