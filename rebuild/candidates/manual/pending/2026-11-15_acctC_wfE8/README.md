# acctC workflow hand-back wfE8 (unworked rows round 3, 2026-11-15)


`open/` holds the best drafts of the batch's unmatched rows (full residues in NOTES.md):
- 0x00B03CA0: parked, DIFFER(6482v6468) /d2inlT1537 /d2inlS15. Re-authored on the matched same-TU sibling CPSCRenderMesh::AddToBoundingSphere 0x00B01900 (wfD6): same preamble, class model and flags, written in the idioms of the landed twin 0x00AFCC30. No local C3DVector/CMatrix3x4/etc. models are left; the only local clas
- 0x00B10B10: near_miss, DIFFER(4280v4276) /d2inlT170. Not reworked in this run; regraded unchanged after the overlay sync. Case-2 residue (0x00B1118F): retail copies the GetRandomUnitVector result three times through the returned eax pointer. Tried hypotheses and next idea are in NOTES.md.
- 0x00A8B990: near_miss, DIFFER(1608v1624) /Gr. Regraded only, unchanged. Register-allocation residue in the bone loop (retail keeps _M_finish in edi and spills Info to [esp+34h]). About 190 grades by chunk 864, parked.
- 0x00A91640: near_miss, DIFFER(4330v4344) /d2inlT108. Regraded, unchanged: 6 struct lines. Measured the frame with a new tool, tools/fmap2.py: retail's low slots 0x10-0x18 hold items that ours puts at 0x30-0x44, and a block of 33 references sits 8 bytes lower in ours. This is a c2 frame-packer residue (0x9C v 0xA
- 0x00A2F740: near_miss, DIFFER(3048v3051) /Gr. Ported from SRC_FAIL onto today's headers in about 22 grades (struct diff 135 to 77, calls equal by target). Closed by: /Gr (retail calls __destroy_aux with ecx/edx), GetRoot's body made visible (the final root release has no reload), FABLE_FORCEINLINE SetCent
- 0x00ACA830: near_miss, DIFFER(7240v7256) /Gr. Not reworked in this run; regraded unchanged (112 struct lines, 66 v 66 calls). Register/slot residue and the x87 IsNonZero product order (Z,Y,X) are in NOTES.md.

