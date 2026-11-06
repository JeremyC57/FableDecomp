# Landing 2026-11-05l: chunk 785 (2 of 6 rows landed, 4 parked)

One expert agent authored the rows. Both landed rows are plain C++ and grade `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00DD3B80 | NScript::CQ_PrisonEscapeRescueMotherScript::PrisonLifeIntro | RELOCATION_MATCH PASS |
| 0x00EAC5B0 | NScript::CV_MurderTwistScript::CMurderTwistBrotherFriend::Main | RELOCATION_MATCH PASS |

- **Names.**
  - The ledger called 0x00DD3B80 `CQ_GuildTrainingScript::RunCeremonyCutscene`, but that function is landed at 0x00D49D50.
    0x00DD3B80 is PrisonLifeIntro: its only reference is the thread spawn at 0x00DD34EF in Main 0x00DD2CE0, and it uses the
    TEXT_QST_055_PRISON_INTRO_* literals.
  - 0x00EAC5B0's catalog module lacked CV_MurderTwistScript.
  - Labels: `ghidra_out/labels_landing_2026-11-05l.tsv`. 7 queue lines (tag `land0511l`), among them four C3DMeshFile chunk
    accessors (0x00ABA350 / 360 / 370, 0x00AB9F60) landed under propagated names, and the block-scoped static
    sorted_combination_anims at 0x013BAC20.
- **Headers** (`hdr_README.md`). Every change is a new opt-in macro or a new file; hdrcheck2 reads `default=same` for each one.
  - Opt-ins: CArray (PUSHBACK), CEdgeStipVertexArray (ACCESSORS), CPatchTesselationEdgeStrip (ACCESSORS),
    CWaterPatchTesselatorBase (HELPERS).
  - New headers: CTesselationData.h, CTVertexWaterBackground.h, CTVertexSeaBackground.h.
  - So far only the parked tesselator rows use them.
- **Landing:** `vland --land` (`logs/land.log`).

## Parked (`pending/2026-11-05_linux_hand785/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00BE4660 CWaterPatchTesselatorBase<CTVertexWaterBackground>::TesselateEdge (vtable 0x012A3334 slot 0; catalog vfunc_0) | DIFFER(2380v2349) | Registers of the inlined helpers' by-value parameter copies |
| 0x00BE4FE0 CWaterPatchTesselatorBase<CTVertexSeaBackground>::TesselateEdge | DIFFER(2404v2412) | Same as its twin |
| 0x0070D940 CAnimateThing::CalcInterpolatedMeshAnim | DIFFER(2572v2395) | Inline budget: GetProgressFromAlpha / push_back called before the loop and expanded inside it; _M_insert_overflow is called in one pass and inlined in the other |
| 0x00ACDB80 QuadricOptimiserInternals::COptimisedPrimitive::COptimisedPrimitive (/Gr) | DIFFER(2658v2674) | this and &Faces in swapped registers, vertex-loop scheduling, one hoisted bone-key copy. COptimisedPrimitive.h and C3DMeshFilePrimitiveChunk.h are flattened (header debt) |
