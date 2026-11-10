# Naming and relocation evidence

- GetAngleXZ exact public const member: Ego_r 00b8b690 and FableWin 031e1ca0. Independent retail bindings: callers00723753/007599b3 are the physics GetFacingAngleXZ members; donor ordering ctor/GetAngleXZ/MakePerpendicular/Rotate/Interpolate agrees with retail.
- CRightHandedSet class key V, protected Up at0 and Forward at12,24bytes: original shared header and observed loads. C3DVector public float X/Y/Z at0/4/8,12bytes: PDB TPI0x2783 and native arithmetic.
- GetAngleXY: FableWin01acf1e0; native row's atan2(X,Y), angle wrapping and turns conversion agree. CrossWithUp FableWin01fa1780; native heading Y,-X,0 copies agree. InitialiseFromAngles FableWin01dcb530; native sin/cos products and float narrowing agree. Return references/const copied from exact PDB entries.
- AccurateNormalise: Ego_r00b81e30 and FableWin031d74f0, exact `?AccurateNormalise@C3DVector@@QAEAAV1@XZ`; target00a14440 already correctly bound by006637d0/00759d30. Wrong older Normalise spellings are noted in NAME_FIXES.tsv, not reused.
- __CIfmod at00bfeb28 and __CIacos at00bfeba2 match existing CRT relocation bindings. They are compiler intrinsic calls, not fabricated declarations.
- All remaining relocations are compiler float/double constants, verified by comparing COFF literal bytes with retail at the decoded target. They are literals rather than introduced global names.
- No local replacement classes in the row: CRightHandedSet and C3DVector use shared headers with additive opt-ins. Default header paths must retain the original preprocessed text.
