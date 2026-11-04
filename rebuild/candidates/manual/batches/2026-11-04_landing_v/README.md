# Landing 2026-11-04v: chunk 758 (retry of 40 parked rows 1-16 bytes off retail, 186-261 bytes, C++ only), 15 landed

## Result
- **Landed 15 of 40** (`logs/land.log`), each graded read-only on the merged tree first (`logs/new2.log`): 2 MATCH and
  13 RELOCATION_MATCH, all PASS.
  - The 15 addresses: 0x0072C9C0, 0x00725B70, 0x00D361F0, 0x0096AE50, 0x00BCC700, 0x00A780B0, 0x00AD8DD0, 0x00B24400, 0x007BDD77,
    0x007BE3E0 / 0x007BE4E0 (byte twins), 0x005DCB00, 0x00A887B0, 0x007FEFE0 and 0x00A8FF50.
  - What fixed each one is in `agent_NOTES.md`. The main fixes:
    - retail class headers with the PDB's by-reference signatures;
    - a landed same-TU sibling copied verbatim;
    - same-TU noinline or `auto_inline(off)` callee bodies taken from landed rows (`_For_each`, GetSubDef, push_back,
      `_M_insert_overflow`);
    - the STLport `std::__find` template visible in the TU;
    - the shared `CArray<CArray<C3DVector> >` read through a const reference.
- **Asm rules (owner, 2026-11-04):** no asm, helper or `volatile`. The `auto_inline(off)` around a landed sibling's same-TU body
  (0x007BDD77) is the established EXTRA_RULES device.
- **Parked 25** (`pending/2026-11-04_linux_hand758/differ/`, 2-39 diff lines each). The residues are lea-vs-add forms, x87 operand
  order, frame slots and callee-saved register permutations. NOTES.md has the next idea for each.
- **Open, not blocking** (vland warnings; `bindings_multi_check.py` prints 0):
  - **RELOC_CONFLICT:**
    - 0x00725B70: its `_For_each` instantiation binds 0x00726610 here and 0x00727500 in the landed 0x00726BE0.
    - 0x0072C9C0: the non-const CVectorMap LowerBound binds 0x004365B0 here and 0x0040F020 in a landed row.
    - Retail keeps separate copies of these template bodies, so the shared symbols need their own retail spellings in a later
      naming pass.
  - **SHARED_HEADER_AVAILABLE:** 8 rows keep their landed siblings' local models.

## Headers (7 changed; each behind a new nested opt-in sub-macro)
- New sub-macros:
  - FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
  - FABLE_RETAIL_CIENGINE_CONVERTSCREENPOS (CIEngine.h / CEngine.h: slot 12 ConvertScreenPosToWorldPosAtViewDistance, CEngine slot 51
    IsActive)
  - FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SUMS
  - FABLE_RETAIL_CTCPHYSICSBASE_SEARCHTOOLS
  - FABLE_RETAIL_CTHINGMANAGER_ALLPAUSE
- CThing.h equals HEAD.
- CAIStateGroupBase.h, CThing.h and CMatrix3x4.h merged 3-way cleanly with chunks 755/756. The upstream blocks appear once each.
- hdrcheck2: `default=same` for every file. `ppcmp.py`: all 457 includers same, so no sibling recheck was needed.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04v.tsv` (19). `apply_labels.py` made 14 catalog and 10 manifest renames.
- **Queued PRIORITY** (`land0411v`):
  - 0x00A8FF50 ConvertToVBFormatNonBump. A landed caller binds it as CAnimatedBlock::TransformVertices.
  - 0x00637FC0 `NThingMap::CThingMapManager::PlaceThing(CTCMapwho&, const C3DVector&)`. MoveInMapwho / PlaceInMapwho bind wrong
    spellings.
  - 0x0090A700 MidAttack360 (was DragonRoar).
  - 0x0072D990 `~CCreatureAction_CastAssassinRush`.
  - 0x0085CDE0, probably NThingMap::CThingMap::Validate.
  - The Ego_r signatures of 0x005FBEA0 and 0x009FC950.
- `bindings_multi_check.py`: 0. `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.

## Coverage
46,693 genuine of 49,568; 5,818,453 function bytes (56.92%).
