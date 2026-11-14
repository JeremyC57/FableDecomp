# acctC workflow hand-back wfB4 (parked rows, 2026-11-14)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x00BCC800 | CEnginePrimitiveManagerWeaponTrails::RenderWeaponTrail | 3,205 | RELOCATION_MATCH (retail_asm via shared headers) | PASS |

Verified twice (workflow verifier + coordinator, `coordverify.log`). No fitted spelling, no assert, no flag.
REVIEW ITEMS:
- Retail asm reached only through existing shared headers GFFloatToLongFloor.h / GFFloatToLongNear.h (`// Retail-asm:` lines
  cite 0x00BCC96A..0x00BCC992 and 0x00BCCF70..0x00BCCF79).
- Local models of CEngineInternalPrimitiveWeaponTrail / CEnginePrimitiveManagerWeaponTrails: no engine header exists, landed
  siblings 0x00BCC700 / 0x00BCD490 do the same; members follow struct_layouts_egor.tsv.
- New opt-in FABLE_RETAIL_CENGINETREEFILER_FINDNODE_INDEX (evidence: its out-of-line copy 0x00BC9B30 byte-matches).
- Labels: 0x00AC20C0 / 0x00AC20D0 are CVertexBufferBase::IsBusy / Sizeof by this row's relocations but labelled
  CThingFilter_IsValid (shared identical bodies): RELOC_NAME warnings only.
`open/`: 0x00B8B780 (3921v3921) and 0x00B89C30 (6431v6431) near-misses, operand-order sites (NOTES.md).
