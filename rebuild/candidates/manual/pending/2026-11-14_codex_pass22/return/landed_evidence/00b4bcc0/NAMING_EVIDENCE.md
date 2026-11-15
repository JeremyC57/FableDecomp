# 00B4BCC0 evidence

Identity is original private CEngineShadowRenderer::CalcClipVolume(const CEngineCamera&), exact PUBLIC spelling and owner/member signature in both PDBs. Native caller00B4EF66 passes the camera; complete native extent is00B4BCC0..00B4C543,2179bytes ending ret4. The13-byte gap to00B4C550 is alignment only. No fragment or unrelated trailing code is included.

NAME_PROOF.json and both raw complete type files establish engine/class/member identity. All13 actual relocated operands, COFF addends, two engine callees and all1.0/0.0 literal bytes are independently checked. The native callees are CMatrix4x4::SetInverse at00A5CB00 and the three-point CPlane::Initialise at00A42170. No symbol/address aliases or naming fixes remain. There are no reachable emitted helpers:18 total emitted sections are inventoried,17 unreferenced COMDATs carry no native address or matching claim.

The four new opt-ins extend the existing real class models. The matrix helper is the original x/y/z/w algorithm from donor02D797F0 and its complete procedure/method records; CPlane's three-point constructor forwards to its original Initialise, exactly as donor02D78D90. Camera PeekFrustrumPlane returns the indexed existing FrustrumPlanes member at+1C0. Original target locals and scopes are in ORIGINAL_LOCALS.json. The donor retains empty pl/cor visualization loops after debug drawing is stripped; this is explicit source provenance, not invented no-op padding. A minimal row /d2inlT81 expands the genuine matrix body. T80 fails; no forced inline or inline pragma is added.

The matrix debug profile reproduces87instructions/241bytes and identical operations/operands, with12 matrix-this reloads scheduled before fld here and after fld in the later original donor. DONOR_KERNEL_PROOF.json discloses the raw-byte difference; the acceptance proof is the complete exact native target with the matrix body expanded. No native address is assigned to that unreferenced standalone method.

The fixture contains exact complete target text and controlled external SetInverse/CPlane::Initialise boundaries.4096 complete scenarios compare projective corner inputs against an independent long-double matrix reference, all four plane triples/extrusions, frustum-facing selection/copy order, both resets and receiver camera sphere. Both VC71 /Od /W3 /GR- without /EHsc and GCC64 C++03 pass.27 native layout checks pass under VC71. HEADER_CONTRACT.json records136 identical profiles,131 successful and five disclosed missing-prerequisite experimental profiles. Main combined accepted-row regression remains the merge gate.

Exact bound engine spellings (both original PDB name sets, original owner descriptors and native caller operands):

- `0x00b4bcc0` — `?CalcClipVolume@CEngineShadowRenderer@@AAEXABVCEngineCamera@@@Z`
- `0x00a5cb00` — `?SetInverse@CMatrix4x4@@QAEXABV1@@Z`
- `0x00a42170` — `?Initialise@CPlane@@QAE_NABVC3DVector@@00@Z`
