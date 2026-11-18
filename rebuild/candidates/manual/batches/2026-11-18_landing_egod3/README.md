# 2026-11-18 landing egod3: 3 rows from the in-house Ego_d author egod3a, round 3 (1,757 bytes)

Owner: "start another round. dont forget that hand authoring is important". Rows: `../../pending/2026-11-18_egod3/` (12, released).

| row | name | bytes | what closed it |
|---|---|---:|---|
| 0x00A6C640 | CPhysicsMesh::CalculateBoundingInfo | 517 | the tail region was exactly 81 IL tuples (c2's region cap); GetMagnitude2D as `float squared = (X*X + Y*Y); return GFFastSqrt(squared);` (new opt-in FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_SQUARED_SUM; FableWin /Od size fits) gives retail's `fsqrt ; pop edi ; fstp` |
| 0x00A229E0 | CRightHandedSet::CRightHandedSet(const C3DAngles&) | 689 | Ego_d's 4th record of the name (the real locals); each of the nine rotation entries one parenthesised expression (no-code tuples move the region cut; exactly nine) |
| 0x00803280 | CTCTargetingPlayer::UpdateTargetThreatening | 551 | old local model ported onto shared headers with the landed same-TU sibling 0x00803D60's define / include block and Ego_d's locals; ParentThing passed directly to the three fastcall tests |

- Dry run and landing, `LANDVERIFY_STRICT_HEADERS=1`: 3 RELOCATION_MATCH + PASS. Warning RELOC_NAME 0x008645E0 (catalog label
  `CMainGameComponent::BeginInputLoading` is wrong: it is NCreatureAI::IsAwareOfThing; queued `land1118e3`).
- Headers: additions under new opt-ins only, every default path preprocesses the same (hdrcheck2), pinned asserts unmoved,
  assert_names_check 0 mismatches. New: C3DVECTOR_MAGNITUDE2D_SQUARED_SUM, NCREATUREAI_WILLING_TO_ATTACK (IsWillingToAttackTarget
  0x008648A0), engine/EOpinionPostDeedType.h (Ego_d enumerators; do not include in a TU with fable_nscript_forward.h's empty
  declaration), plus round 2's C3DBOUNDINGBOX_SETMINMAX, C3DBOUNDINGSPHERE_SET, CCIRCLE_SET, CPHYSICSMESH_CALCULATEBOUNDINGINFO and
  CENGINELANDSCAPEMESHBUILDER_GENERATIONTABLES (the last used by the parked 0x00BE2CD0 draft).
- Notes: the parentheses in 0x00A6C640's opt-in and 0x00A229E0 are marked FITTED (no-code tuple counts; MATCHING_PLAYBOOK "PAREN
  tuples"). Three landed GetMagnitude2D users need the default running sum, so the new body stays an opt-in. 0x00803280 spells the
  type test `GetType() == 1` (Ego_d: THING_TYPE_AI_CREATURE; no repo header has that enum yet).
- Purity clean; funclets 1,355 / 0 broken; bindings_multi_check at baseline.
