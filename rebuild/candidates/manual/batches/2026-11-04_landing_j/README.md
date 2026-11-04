# Landing 2026-11-04j: chunk 747 (retry of the 65 parked bare-`fistp` rows), 11 landed

## Result
- **Landed 11 rows** (`logs/land.log`), each graded read-only on the merged tree first (`logs/new2.log`): RELOCATION_MATCH + PASS.
  Every one is grade `retail_asm`: the row is plain C++ and its only asm is the shared isolated helper (`__asm` path + `FABLE_PORTABLE`
  C++ path + the existing dual-path tests in `rebuild/tests/portable/`).

  | Address | Name | Helper |
  |---|---|---|
  | 0x004385F0 | CPlayerGui::UpdateCrime | GFFloatToLongNear |
  | 0x006378E0 | NThingMap::CThingMapManager::GetThingMapwhoLevel | GFFloatToLongFloor |
  | 0x009FCA40 | CGraphicFrameData::InitialiseAsSurfaceSection | GFFastFloatToLong |
  | 0x00A76E00 | CNavQuadTree::GetPreferabilityCostForNode | GFFloatToLongFloor + GFFloatToLongNear |
  | 0x00A88D10 | CQuaternion::SetFromAxisAngle | GFFloatToLongNear |
  | 0x00AA3C50 | C3DKeyframe::BlendWith (was labelled CRightHandedSet::Interpolate) | GFFloatToLongNear |
  | 0x00B05E10 | NParticleEngine::CPSCSingleSprite::SmoothFadeValue | GFFloatToLongNear |
  | 0x00B16220 | NParticleEngine::CPSCOrbit::InterpolateTo | GFFloatToLongNear |
  | 0x00B18820 | NParticleEngine::CPSCLight::Update | GFFloatToLongNear |
  | 0x00B1AD90 | NParticleEngine::CPSCLight::SmoothFadeValue | GFFloatToLongNear |
  | 0x00B84580 | CEngineSubPrimitiveRenderer::AddSubPrimitive | GFFloatToLongNear |

- **Owner rule check (C++ exhausted first, then intrinsics, then macro vs function isolation):** the per-site table is
  `pending/2026-11-04_linux_hand747/ATTEMPTS_TABLE.md` (raw `ATTEMPTS.tsv`), 13+ spellings per site.
  - Plain C++ always emits `__ftol2`; floor/ceil spellings emit a CRT call plus `__ftol2`.
  - `/QIfist` emits `fistp qword`.
  - `_mm_cvtss_si32` / `_mm_cvttss_si32` emit `cvtss2si` / `cvttss2si`.
  - At 0x00AA3C50 the rounding result is dead: every C++ spelling deletes it, so it survives in retail only because it is asm.
  - Macro form vs `__forceinline` function form: the function form matches at all 12 helper groups, the macro form at 9, so no new
    macro or helper was added.
  - No row source or test contains `__asm`, `naked`, `_emit` or `volatile`; each row has a `// Retail-asm:` line with its own fistp address.
- **Not landed** (`pending/2026-11-04_linux_hand747/`): 17 parked rows whose residue is not rounding-related (bitfield test form, frame
  slots, register choice, x87 scheduling; dl 2..50), and 37 deferred rows (large first drafts, skeletons, invented stand-in types, or no
  fistp of their own). STATUS.tsv / NOTES.md have each residue and next idea. 0x0057C4F1 / 0x00646762 need a Ceil helper variant that loads
  1.0f with `fld1`.

## Headers (3, each behind its retail macro; repo copies unchanged since the overlay base ab98c12c3)
`C4DVector.h` (FableWin operator- / operator+ and a `__forceinline Interpolate`), `CANavQuadTreeNode.h` (friend CNavQuadTree),
`CNavQuadTree.h` (GetPreferabilityCostForNode). hdrcheck2 `default=same` (`logs/hdrcheck2_pre_merge.txt`).
- **Siblings:** `includers.py` reaches 50 landed sources/tests; `ppcmp.py` (`logs/ppcmp.txt`) finds 41 token-identical and 9 different.
  All 9 rechecked on the merged tree (`logs/recheck.log`): RELOCATION_MATCH + PASS.

## Names
- vland wrote the 7 rows not in the chunk's LABELS.tsv to the catalog with bare names (no class). `sync_manifest_from_catalog.py` would
  then have moved their manifest module to `_global`; that run was reverted and the catalog names qualified from each row's leaf symbol
  through the labels file instead (GOTCHAS).
- **Labels:** `ghidra_out/labels_landing_2026-11-04j.tsv`, 20 labels; `apply_labels.py` (`logs/apply_labels.log`).
- **Queued PRIORITY** (`land0411j`, 17 entries): chunk NAME_FIXES.tsv, e.g. 0x00437AE0 UpdateStat (ABE, CStatBase&), 0x00487B70
  GetPPlayerCharacterThing, 0x00A4C1F0 (`C3DKeyframe&, unsigned long`), 0x00A567A0 (out `C3DVector&`).
- `bindings_multi_check.py`: 0. `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0 after the label fix.
