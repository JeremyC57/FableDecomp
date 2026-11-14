# acctC hand-back wfB7 (parked rows + fix-up, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x00772BB0 | CTCShop::AddSaleItem | 2,585 | RELOCATION_MATCH (`/d2inlT116`) | PASS |
| 0x007DE710 | CTCWallMount::AttachObject | 2,578 | RELOCATION_MATCH | PASS |

Coordinator-verified after the fix-up (`coordverify.log`). The verifier's objections are fixed: both rows are on the shared
engine headers (CTCCreatedEntity, CTCStockItem, CTCWallMount, CTCTargeted, CTCHeroStats, ThingAlive); SetPCreator and
IS_THING_ALIVE are the PDB spellings. Local models left: CTCParticleAttacher, CTCWallMountEffects (no engine header; landed
siblings 0x007F8860, 0x00641260, 0x007DFAB0, 0x007EFE70 do the same).
REVIEW ITEMS:
- 0x00772BB0 inline-budget device: `// Flags: /d2inlT116` (smallest that matches; 116..400 all match; default calls
  CMatrix3x4::Set three times, 2163v2585). The force-inline header opt-in is no longer used. The row source defines the landed
  0x007767A0 body (CMatrix3x4::Set(const CRightHandedSet&, const C3DVector&)) above the row as a same-TU callee (0 callers,
  sits among the CTCShop unit's template instances), so the row object also defines that symbol: mind the link.
- New additive opt-ins: FABLE_RETAIL_CTCSTOCKITEM_SETASFORSALE (Ego_r ?SetAsForSale@CTCStockItem@@QAEX_N@Z, inline; store at
  0x007735B5; parameter name descriptive), FABLE_RETAIL_CTCWALLMOUNT_ATTACHOBJECT (declaration). Asserts pinned.
- FITTED (marked in the sources): orientation copied by assignment through a named reference (both rows);
  `const CPlayer* player` (0x007DE710); FABLE_RETAIL_CMATRIX3X4_SETINVERSEUSRT_SCALE_SQUARED (named `scale_squared` in the
  shared-header inverse; same behaviour; no out-of-line retail copy exists to prove the text).
- 0x007DE710 writes `++stats->NumMountedTrophies` directly; the PDB also has inline Get/SetNumMountedTrophies (same bytes).
- Tool gap: the strict-header regex in verify_and_land.py (`class Name {`) misses local classes with a base clause.
- The overlay still holds the unused FABLE_RETAIL_CMATRIX3X4_SET_RHSET_INLINE: drop it at landing.
