# Proposals for LANDED rows (chunk 2041 continuation) -- NOT edited in the repo; for the main coordinator

The chunk 2041 overlay changes the shared `C3DVector::InitialiseFromAngles` body (hdr/engine/C3DVector.h, ANGLE_BODIES branch) to
the GFCos / GFSin text:

    float cosYZ=GFCos(angleYZ);
    X=GFSin(angleXY)*(cosYZ*length);
    Y=GFCos(angleXY)*(cosYZ*length);
    Z=GFSin(angleYZ)*length;

Evidence: FableWin.pdb COMDAT order (?GFSin@@YINM@Z 0x01DCB5D0 directly after ?InitialiseFromAngles@C3DVector@@QAEABV1@MMM@Z
0x01DCB530, i.e. InitialiseFromAngles is GFSin's first instantiation); 0x006AB820 is byte-exact only with this text (the old
`(float)(sin(..)*(cosYZ*length))` text leaves a PAREN tuple that swaps 0x006ABB81..8F); retail's unreferenced out-of-line copy
0x0068D4C0 grades RELOCATION_MATCH with it (and with the old text).

Header users need NO source change (re-graded under the overlay, all RELOCATION_MATCH): 0x00A22CA0, 0x00A0C570, 0x00746EC0, 0x008BD3F0.

Rows with a LOCAL copy of the body match with either text. To keep one body text everywhere, these re-spelled copies are proposed
(only the body lines + two local GFCos / GFSin inlines written with the row's own TWO_PI spelling changed):

| row | proposal | grade (q.py, VC7.1 RTM, no overlay) | landed test (host model, unchanged; g++) |
|-----|----------|------|------|
| 0x00A14040 C3DVector::RotateXY | C3DVector_RotateXY_00a14040.cpp | RELOCATION_MATCH | 00a14040_TEST PASS |
| 0x00A14260 C3DVector::Rotate(float, float) | C3DVector_Rotate_00a14260.cpp | RELOCATION_MATCH | 00a14260_TEST PASS |
| 0x00A0C450 CCamera::GetAngleXZ | CCamera_GetAngleXZ_00a0c450.cpp | RELOCATION_MATCH | 00a0c450_TEST PASS |
| 0x00CBE693 GetRandomPositionInsideRadius | global_GetRandomPositionInsideRadius_00cbe693.cpp | RELOCATION_MATCH | 00cbe693_TEST PASS |

The landed tests model the body themselves (they include no row source); the new text computes the same values (implicit v explicit
double -> float conversion, sin -> GFSin(angle) = sin(angle * 2pi)), so they need no change. Optional: landing the header change
alone is enough for 0x006AB820; these proposals only remove the second body text.
Other pending hand-backs carry their own copies of engine/C3DVector.h with the old body (c2022, c2026, c2029, c2034, c2042, r3002,
r3011, r3016, H40g, H40i, hand829). The only pending rows that define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES (c2022 0x007D3820, r3002 /
c2022 0x007F26B0, c2034 parked 0x0097BDA0) call neither InitialiseFromAngles nor sin / cos / GFSin / GFCos (grep); the new includes
(rebuild_abi.h, GFSin.h, GFCos.h) are inside that #ifdef. NOT graded here (they need their own overlays); re-grade them after the merge.
