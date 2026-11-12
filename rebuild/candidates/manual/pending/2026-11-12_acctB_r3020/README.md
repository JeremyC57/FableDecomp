# acctB hand-back, row 0x00A12EE0 CPhysicsMeshObject::GetNavigationLevelVolumes: matched (was parked at 1118v1144)

| address | result | notes |
|---|---|---|
| 0x00A12EE0 `?GetNavigationLevelVolumes@CPhysicsMeshObject@@QBEXAAV?$CVectorMap@JV?$CCountedPointer@VCContainmentVolume@@@@...@@@Z` | RELOCATION_MATCH 1144v1144, test PASS (g++ and VC7.1) | 190 logged grades over two sessions (`grades.log`) |

Re-verified by the acctB local coordinator (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` holds the grade, both test runs and the read-only vland log. Purity and padded-struct greps clean. Nothing is landed. vland: 1 win, no RELOC_CONFLICT / RELOC_IDENTITY / NOT_GENUINE; four `WARN RELOC_NAME` lines (catalog labels that
are descriptive or unmangled where the row binds the STLport / `CCountedPointer<CContainmentVolume>::DeleteData`
spellings: 0x00A110D0, 0x008274B0 and the two insertion-sort helpers).

## What closed it

- **Session 1:** `TransformPlane` expanded (FableWin emits it as the first COMDAT after the row) with helper texts
  that equal FableWin's debug sizes; `const C3DMeshVolume& mesh_volume = it->second;` used twice gives retail's
  second induction pointer (0x00A12F08); `const CPhysicsMesh* mesh = Mesh.Get(); if (mesh)`.
- **Session 2 (one swapped pair of products in the inlined TransformPoint):**
  `const CPlane& source_plane = source_planes[i]; CPlane plane = source_plane;`. The order follows hash keys over
  compiler record ids, which are handed out in chunks of 32; this statement moves one record across a chunk edge.

## Review items

- **FITTED:** the `source_plane` reference, the `mesh_volume` reference, the `mesh` pointer, TransformPlane's
  statement order, the `_LTR` TransformPoint text (one of three repo texts for one retail body), and
  `FABLE_FORCEINLINE` on TransformPlane (plain inline is called out of line, 887v1144; the source has the
  `// Inline-budget:` line). The match sits on a record-count edge: one or two more dead locals break it.
- **PROVEN:** TransformPlane is an inline (FableWin COMDAT); the helper sizes; the callee addresses.
- **Headers:** opt-ins `TRANSFORMPLANE` (rewritten), `CPLANE_ACCESSORS`, `CIVCOUNTEDPOINTER_GET`
  (`hdr/README.md`); none has another landed user. No name fixes.
