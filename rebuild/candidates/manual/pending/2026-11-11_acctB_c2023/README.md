# 2026-11-11_acctB_c2023 -- acctB hand-back, chunk 2023 (2 reserved rows: 1 matched, 1 not attempted and re-queued)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x0094C050 CAIStateGroup_SellToBuyer::Update (vtable 0x0128BF84 slot 12) | RELOCATION_MATCH 1859v1859, test PASS (g++ and VC7.1) | plain C++, no inline-budget device, no asm helper. True length is 1859: the reserved estimate's other 23 bytes are 13 int3 plus a separate 10-byte function at 0x0094C7A0 (`mov [ecx+0Ch],arg ; ret 4`) that is in neither functions.tsv nor RESERVED.tsv -- uncatalogued, for the coordinator. 7 grades |
| 0x00915B60 CAIStateGroup_MoveGoodsBase::PlaceGoods | not attempted (agent context) | name and extent proven, decoded outline and the models to build in NOTES.md; re-queued in a later acctB chunk |

Coordinator re-verification of the matched row (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade,
both test runs, read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT). Purity and padded-struct greps are clean.

Review items for landing:

- **0x0094BEF0 is misnamed in the landed tree.** It is landed as
  `rebuild/src/compiled/00/94/CAIStateGroupWorkBase_IsInWorkHours_0094bef0.cpp`; the agent proves it is
  `CAIStateGroup_WorkBase<CAIStateGroupBase>::StayInShop` (`?StayInShop@?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@IBE_NXZ`,
  Ego_r donor order directly before SellToBuyer::Cleanup). I checked the byte side: the StayInShop COMDAT that
  `rows/0094c050.cpp` emits (opt-in `FABLE_RETAIL_CAISTATEGROUP_WORKBASE_STAYINSHOP`) grades RELOCATION_MATCH against
  0x0094BEF0 (`q.py 0094bef0 rows/0094c050.cpp StayInShop`). NAME_FIXES.tsv carries the respell (6 lines incl. header:
  also 0x00798D10 GetMinDistFromStall, 0x00798D20 GetMaxDistFromCustomer, 0x00798D90 AddNewCustomer, 0x008644E0
  NCreatureAI::IsSpeaking). Grade only the Update leaf when landing this row.
- **`FABLE_RETAIL_CTCPHYSICSBASE_RADIUS_ACCESS`** makes `CTCPhysicsBase::Radius` (+0x34) public so the row can read the
  member directly (retail loads the PhysicsTC pointer before two calls and reads `[ebx+34h]` after them; the inline
  GetRadius() accessor is evaluated ahead of the calls). One source only (bytes): confirm the member's access against the
  Ego_r TPI before accepting, or keep it as header debt. Details in `hdr/README.md`.
- The wait-loop spelling (`bool waiting` flag plus a trailing `RefuseCustom(); Terminate();`) is the shape that reproduces
  retail's inline split and block layout; retail's exact construct is unproven (LESSONS.md).

To land: the row needs `hdr/` (new `engine/CAIStateGroup_SellToBuyer.h`, `engine/CCreatureInteraction.h`; opt-in blocks in
copies of CAIStateGroupBase.h, CAIStateGroup_WorkBase.h, CTCShopKeeper.h, CThingCreatureBase.h, NCreatureAI.h,
CTCPhysicsBase.h; `hdr/README.md` has the evidence per block). CAIStateGroupBase.h starts from the c2026 / c2017 copy
(PERFORMACTION_NOWAIT), so merge after those. Rebuild the oracle from Fable.exe (no retail bytes are in this folder).
