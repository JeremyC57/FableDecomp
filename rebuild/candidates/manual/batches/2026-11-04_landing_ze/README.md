# Landing 2026-11-04ze: chunk 768 (parked follow-up, 14 rows), 5 landed

## Result
- **Landed 5 of 14** (`logs/land.log`). All are RELOCATION_MATCH + PASS, and each was graded read-only on the merged tree first
  (`logs/new2.log`).
  - 0x005EBFA0 `CTCTavernGame::CameraUseCameraPoint`. A fabricated stand-in was replaced by the real callee chain, with a same-TU
    noinline `GetAllThings` body.
  - 0x0096DBC0 `CCreatureAction_ScreamerIntoAggressive::Initialise`. Two full `AddMode(new ...)` arms replace a ternary.
  - 0x00969E70 `CAIStateGroup_SleepOnFloor::IsRunnable`, now on retail headers. It uses a named `C2DVector` centre and a same-TU
    noinline `GetAllThingsInRoughArea`.
  - 0x00753380 `CTCLook::FrameUpdateHeadInterested`. An early `if (!target && !IsLookingAtCamera()) { CentreHead(); return; }` and
    the retail headers fixed it.
  - 0x0070D780 `CAnimateThing::AddCombinationAnim`, with STLport members, the implicit `CAnimSet` copy constructor and a same-TU
    noinline `CAnimSet::Init` (PDB signature). The `CWorld` local model is now `engine/CWorld.h`.
- **Asm rules:** no asm, helper, pragma or `volatile` in any row or test.
- **Parked 9** (`pending/2026-11-04_linux_hand768/differ/`, at least 10 grades each). Residues:
  - 0x00C3A7D0: libpk; retail emits `fst; fcomp 0` where ours emits `fcom; fst`.
  - 0x0083DC80 / 0x00837B30: OldPosition add order, `PhysicsTC` loaded twice in retail, frame 0x80 against our 0x8c. These two still
    use local models with `#pragma pack`; the next step is `CThingPhysical.h` / `CTCPhysicsBase.h`.
  - 0x0089DA70: loop register swap.
  - 0x009D5BF0: retail spills `this` at entry.
  - 0x00A76770: the QuadTreeLevel byte is kept in dl.
  - 0x00ED9BB0: only retail's entry dead store `mov [esp+4],0` is missing.
  - 0x00F35C60: late b11 load.
  - 0x00BCFB00: register and frame allocation around `DrawBuffersIndexed`.
- **Owner rule:** this is the last parked-row agent for now; free slots go to authoring and the vendor naming pass.

## Headers (10 changed, new sub-macros, `default=same`; 1 new header `NThingMap_CThingMap.h`)
- **New sub-macros:**
  - `FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY` / `_ROUGHAREABODY`
  - `CTHING_STATTYPEFLAGS`, `CTCMAPWHO_SEARCHID`
  - `C2DVECTOR_FROM3D` / `_INITIALISE`, `C2DBOXF_CENTRE`
  - `CTCTAVERNGAME_CAMERAPOINT`, `CTCTARGETINGBASE_LOOKATCAMERA`
  - `CTCGRAPHICAPPEARANCE_SETALPHA` / `_FOCALPOS`, `CTCCARRYING_ALLCARRIED`, `CANAVQUADTREENODE_FRIENDNODE`
- **Includers:** `ppcmp.py` shows all 606 the same.
- **hdrcheck2:** `CTCTargetingBase.h` reports `(cpp-err)` on default and on the new macro alike. The header does not preprocess on
  its own, and that is not new.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04ze.tsv` (18 labels, all two-source).
  - Corrections:
    - 0x00969FB0 is the `CFilter_Bind2<IsMySleepPoint, unary_negate<IsOccupiableOccupied>>` vector instance of
      GetAllThingsInRoughArea.
    - 0x00A55A30 is `C2DBoxF::ToC2DBoxI`.
    - 0x0049D150 is `CTCPhysicsBase::GetPosition(float)`.
    - 0x0070D1A0 is `CAnimSet::Init` (private).
    - 0x007528A0 CentreHead and 0x007530D0 FrameUpdateHeadLookAtPos are private.
    - Also corrected: 0x006A2220, 0x007F5860 and 0x004C11D0.
- **Coordinator fixes, on landing:**
  - The agent's 14 mangled labels had gone into the catalog verbatim, under module `_global`. They were rewritten to catalog form
    (`Class::Method`) and re-applied.
  - The same was done for three mangled labels from landing zc: 0x004304E0, 0x0049124E and 0x00627BE0.
  - 0x0070D780 had the catalog class `CTCAnimation`. It is now `CAnimateThing`: Ego_r.pdb 0x00577D20 and the source Leaf agree.
  - `finish_landing.sh` now converts mangled names itself.
- **PROPOSAL only** (`land0411ze`): 0x006A8930 / 0x006A8A30 (filter operators) and 0x00C3A7D0 `CRampedParam::ApplyGainToFrame`.
- **Checks:** `bindings_multi_check.py` prints 0. `sync_manifest_from_catalog.py --dry-run` prints 0 / 0 / 0.

## Coverage
46,845 genuine of 49,568; 5,840,780 function bytes (57.15%).
