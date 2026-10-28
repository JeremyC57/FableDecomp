# Chunk 518 (agent518, 2026-10-28): 28 of 40 re-queued single functions landed (under 128 bytes)

Re-queue chunk of 40 chunk_history orphans under 128 bytes (address order, 0x009F4C00..0x00A779B0): graphic and mesh
data banks, vectors, camera, navigation nodes and animation stats. One Opus 5.5 agent at high effort.

- **Landed:** 27 rows with `vland.py --land` (8 MATCH, 19 RELOCATION_MATCH), plus 1 retail_asm row
  (payload_retail_asm.json): 0x00A13C20 C3DCoordI::operator-=, RELOCATION_MATCH. Every test PASS under g++ and VC7.1;
  no RELOC_IDENTITY, RELOC_CONFLICT or RELOC_DUPLICATE.
- **Review item, 0x00A13C20:** retail holds `maxps xmm0, xmm1` (0F 5F C1, 0x00A13C4F) after the Z store of an otherwise
  plain x87 body, on registers the function never loads, and adds Z where X and Y subtract. VC7.1 cannot emit that
  instruction from C++, so the row lands as retail_asm under the evidence rule; the sequence is unique in Fable.exe and
  may be a post-build patch rather than source.
- **Parked (12, pending/2026-10-27_linux_hand518/differ/):** register choice and scheduling residue in CGraphicDataBank
  (0x009FD3A0, 0x009FD660, 0x009FDBA0), CCamera::SetView, CProcessedInput::Clear (loop roll vs /Os encodings),
  C3DMeshInfo::Reset, C3DMeshStats::HasHelperPoint, C3DAnimationStats::GetAllEvents, C2DLineF::GetPointOnInfiniteLine
  (x87 order), the two IsNeighbourOf (find tag over a live argument slot), CNavSwitchableLeafNode::Unlock (LTCG ecx).
- **Identities:** 24 proven labels written (ghidra_out/labels_chunk518_2026-10-27.tsv); probable: the template instances
  0x009F7D10, 0x00A31220, 0x00A39900, 0x00A39950.

Label corrections (NOTES.md, 14): e.g. 0x009F7D10 list::insert (not CKinematic::operator=), 0x009FD6C0 CGraphicHeader ctor,
0x00A0D300 CProcessedInput::Clear, 0x00A14440 / 0x00A14480 / 0x00A14510 the vector normalisers, 0x00A2EA20 HasHelperPoint,
0x00A4EF90 / 0x00A50BF0 GetAllEventNames / GetAllEvents (both labelled ChangeScene), 0x00A6A330 CPreallocTexturePool ctor.

Review items (LESSONS.md): wrong landed bindings at 0x0066FB80, 0x006637D0, 0x004473A0; RemoveLOD access/const, LowerBound
0x00A316B0, operator+= / HasDummyObject return types, template types at 0x00A557E0 / 0x00A55880 / 0x00A55500, CTexture::
Uninitialise 0x009F9F70, CBaseClassNonCopyable ctor IAE vs QAE, Find_00a7d0e0 is STLport `__find`; globals 0x013D28D8 /
0x013D28D4 are AlignmentShift / Alignment (landed as g_LastAllocator*); CPreallocTexturePool is 0x54 bytes in retail
(0x58 in the PDB).
