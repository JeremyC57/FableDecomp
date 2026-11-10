# Identity and verification

Exact Leaf: `?GetPositionMatrixFromRHSet@C3DMeshObject@@SI?AVCMatrix3x4@@ABVCRightHandedSet@@@Z`. Ego_r donor00b8c4f0 and FableWin donor031f6920 both supply this name. Retail callers00724726/0075a5ed/0075af3c/0075b287 pass a right-handed set and use a returned matrix; the neighbour SetPositionMatrix00a357b0 has the same orientation convention. Shared headers provide PDB-derived CRightHandedSet (Up+0, Forward+12) and C3DVector (X/Y/Z+0/4/8), and CMatrix3x4 union layout E11 through E43,48bytes. These layouts agree with every native load/store. The row's C3DMeshObject declaration contains only the proven static member, no substitute object layout.

GetUp/GetForward/GetRight and C3DVector cross operator are the supplied shared inline bodies. Their identities are recorded by the shared headers and PDB name sets. No new header, helper, callee or global was introduced. The fabricated SetFromAxes helper in the supplied draft is removed.

The selected object has exactly one Leaf at offset0 in its185byte section. It equals the retail cut byte for byte with ZERO relocations and no padding removed. The code starts at00a358f0 and returns at00a359a8 (last byte of assigned extent).

Standalone C++03 test copies the actual selected function and the shared math operations it uses. It checks100 pairs of dyadic vectors, including degenerate, orthogonal, parallel, reversed and non-unit axes: all12 matrix fields and unchanged inputs. Matrix values have no ownership effects; the test does not rely on return-value elision.
