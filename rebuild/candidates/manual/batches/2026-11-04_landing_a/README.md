# Landing 2026-11-04a: chunk 739 (CPSCUpdateNormal InterpolateTo / ReadBinary)

## Result
- **Landed 2 rows** (`logs/land.log`). Before landing, both were graded read-only on the merged tree with no overlay
  (`logs/new2.log`): RELOCATION_MATCH + PASS.

  | Address | Name | Grade |
  |---|---|---|
  | 0x00B0A920 | `NParticleEngine::CPSCUpdateNormal::InterpolateTo` (slot 14) | RELOCATION_MATCH, grade `retail_asm` |
  | 0x00B0B860 | `NParticleEngine::CPSCUpdateNormal::ReadBinary` (slot 15) | RELOCATION_MATCH |

  - 0x00B0A920 has no asm of its own. Its rounding is the bare `fistp dword` (first at 0x00B0A982), which no VC7.1 C++ spelling
    emits (`(long)x` gives `__ftol2`, `/QIfist` gives `fistp qword`; re-tested 2026-11-04). It calls the shared
    `engine/GFFloatToLongNear.h` helper, which has the `__asm` path and a plain C++ path under `FABLE_PORTABLE`
    (owner rule 2026-11-04: no asm first, isolated helper only where C++ is proven impossible).
  - No row source or test contains `__asm`, `naked`, `_emit` or `volatile`.
- **Not landed** (staged in `pending/2026-11-04_linux_hand739/`):
  - 0x00B0D360 DatabaseSerialise, DIFFER(2242v2240): the bit-0 merge of `RandomisePosDistVaryEnable` (retail xors the bool as a memory
    operand).
  - 0x00B0CAD0 UpdateAddParticle, DIFFER(2191v2191): seed register (ebp in retail), temp frame slots, cross-product x87 order.
  - 0x00B0DC20 Update (5602 bytes): not drafted; its head is decoded in the chunk NOTES.md.

## Headers (4 changed, each only behind its existing retail macro)
`C3DVector.h` (FABLE_RETAIL_C3DVECTOR), `CDataInputStream.h` (FABLE_RETAIL_CDATAINPUTSTREAM: `Read3DVector` 0x009950F0),
`CParticleSystem.h` (FABLE_RETAIL_CPARTICLESYSTEM), `CRightHandedSet.h` (FABLE_RETAIL_CRIGHTHANDEDSET).
- The overlay's base is 40c4d6d63. No header changed between that commit and the landing, and the overlay only adds lines.
- hdrcheck2 before the merge: `default=same` for all four (`logs/hdrcheck2_overlay_pre_merge.txt`). After the merge every macro
  reads `same` (`logs/hdrcheck2_overlay_post_merge.txt`).
- **Siblings:** every landed source that names FABLE_RETAIL_C3DVECTOR or FABLE_RETAIL_CDATAINPUTSTREAM (18) was rechecked with
  `--recheck-landed` on the merged tree (`logs/recheck.log`, `sibling_grades.tsv`). All 18 are RELOCATION_MATCH + PASS.
  - No header defines either macro; the two that mention it do so in comments only.
  - No landed source names FABLE_RETAIL_CPARTICLESYSTEM or FABLE_RETAIL_CRIGHTHANDEDSET.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04a.tsv`, 8 labels. `apply_labels.py` made 6 manifest renames: the three parked
  slots (12 UpdateAddParticle, 17 DatabaseSerialise, 9 Update), CQuaternion::SetFromAxisAngle 0x00A88D10,
  PeekEmitterVelocity 0x00AF3DB0, GetRandomSeed 0x00AEAAD0.
- **Queued PRIORITY** (tag `land0411a` in `naming/NAME_FIXES_QUEUE.tsv`): 0x00AEAAD0 GetRandomSeed, 0x00AF3DB0 PeekEmitterVelocity
  and 0x00B18110 CPSCAttractor::DatabaseSerialise. All three need a re-model, not just a respell:
  - GetRandomSeed needs the seed global's data symbol, which waits on the owner's cvdump.
  - PeekEmitterVelocity needs a packed emitter model.
  - DatabaseSerialise needs the `const CEmitterDatabaseSerialiseInfo&` parameter type.
- **`reloc_bindings.tsv`** is unchanged, and `bindings_multi_check.py` prints 0.
- `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0. `tools/sync_catalog_names.py` was not used: it rewrote 473 manifest rows
  to spaced template spellings that `sync_manifest_from_catalog.py` reverses, so those changes were discarded (GOTCHAS).

## Coverage (`write_decomp_dashboard.py`, `update_readme_progress.py`)
46,556 of 49,564 (93.931%), 54,091 of 57,609 known; 5,736,082 function bytes (56.08%); 54,104 landed rows.
