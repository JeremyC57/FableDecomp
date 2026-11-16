# 2026-11-16 landing gi -- five near misses closed by hand (agent w24C)

Owner: "work on the closest parked rows that arent reserved by other accounts / ais". Agent w24C took 11 parked rows whose residue
was one instruction, one register choice or a few bytes, and rewrote the feeding statements by hand. Five land here, all with their
behaviour tests passing, `LANDVERIFY_STRICT_HEADERS=1`, on the repo headers (`logs/vland_land.log`).

| row | function | grade | what closed it |
|---|---|---|---|
| 0x00989830 | CShaderRenderManager::InitialiseLight | MATCH | pair index `(index - 1) >> 2` (unsigned) and component `(long)(index - 1) & 3` named before the pair address; shared engine/CShaderLight.h |
| 0x00C44090 | CCodaSource::EAXSet (descriptive) | RELOCATION_MATCH | written on the shared Coda headers; two `CODA.SetError` call sites, the first followed by `return`: VC7.1 merges them, giving retail's `mov ecx` before `push eax` |
| 0x00C44100 | CCodaSource::EAXGet (descriptive) | RELOCATION_MATCH | twin of 0x00C44090 |
| 0x0090E3D0 | CMessageEventManager::FindMostRecentMessage<...IsValidGossipee...> | RELOCATION_MATCH | landed twin 0x00934E50's shared-header instantiation with CThingFilter_IsValidGossipee, `/d2inlT107` |
| 0x00BCF4F0 | CEngineInternalPrimitive2DAnimatedMesh::Initialise | RELOCATION_MATCH | `FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS` and `WorldPos.EndPosition` assigned directly instead of through a reference (`/d2inlT144`) |

Headers: engine/CCodaSource.h and engine/CCodaBufferQueue.h gain one opt-in, `FABLE_RETAIL_CCODASOURCE_EAX` (additions only, no
assert lines; `hdr_notes/README.md`). Whole-tree equivalence check (exact): 56,377 landed rows identical input except 0x00CBFB7D
(`__FILE__` path only).

Names: EAXSet / EAXGet are DESCRIPTIVE member names (they forward four arguments to CSalVoice slots 15 / 16, which engine/CSalVoice.h
names EAXSet / EAXGet; the only callers 0x00C52A10 / 0x00C4F470 compare the first argument with the EAX 4.0 FXSlot0 property-set GUID
at 0x012B9468); the vtable slot names added to the overlay are descriptive too. Queued as `land1116gi`. `NAME_FIXES.tsv` also has
two low-confidence suggestions (0x00C43780 / 0x00C437A0 SetLPFilter / SetEQFilter) that are NOT queued.

Not landed:
- 0x00BF0740 SetupOutlineStencilStates: RELOCATION_MATCH + PASS (`pending/2026-11-16_w24/parked_C/`), but its leaf is carried by the
  landed 0x00BF0B20, which is really SetupOutlineFillStates (queued land1113fy / land1115ga). Lands after that respelling.
- 0x00896B60, 0x0086BE40, 0x00871FD0, 0x008EEC70: parked with measured residues (`STATUS.tsv`, near drafts in
  `pending/2026-11-16_w24/parked_C/`). 0x00BA50A0 already matches inside acctC's open TU; left to acctC.
Lesson (`LESSONS.md`): VC7.1 c2 schedules a basic block in regions of at most 83 DAG nodes, and non-emitting tuples (parenthesised
float sub-expressions, float locals kept on the x87 stack) count toward the cut.
