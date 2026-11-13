# acctC open drafts after agentC22 (2026-11-13) — nothing matched

| address | name | grade | state |
|---|---|---|---|
| 0x00B89C30 | CEngineScreenEffectOutlineGlow::ApplyOutline | DIFFER(6431v6431), one hunk (bookmark load order switches one block late, 0x00B8AF59) | parked |
| 0x00B8B780 | CEngineBillboardGenerator::RenderBillboard | DIFFER(3921v3921), 3 operand-order sites (0x00B8BC75, 0x00B8BCE2, 0x00B8C393) | parked |
| 0x00BCC800 | CEnginePrimitiveManagerWeaponTrails::RenderWeaponTrail | DIFFER(3207v3205), frame-slot permutation and a node kept in a register | parked |
| 0x00BAA870 | CEnginePrimitiveManagerRepeatedZSprites::RenderFromSortList | not graded: header port incomplete | open |

Sources ported to the lane's current render overlay; `hdr_vs_c20_overlay.diff` is the additive header delta against the
overlay staged in `2026-11-13_acctC_c19a` (+ `open_00ba2350/hdr_C20.diff`). Details in NOTES.md.
