# Chunk 636 -- 2 of 10 landed (2026-10-31)

Cut from the parked-row pool. Landed via `coordinator/land_chunk.sh 636` (vland --land 2/2 RELOCATION_MATCH + PASS, multi-check 0):
- CCamera::GetAngleXZ 0x00A0C450 (catalog label Math_ComputeSphericalCoordinate was descriptive) -- the PDB inline
  C3DVector::InitialiseFromAngles(heading, pitch, 1.0f) with the landed Rotate 0x00A14260 body shape (named float cosYZ, `* length`
  products that fold at 1.0f) gives retail's x87 stack shape. The 2*pi double 0x0124F2B8 is referenced as the established
  `?g_twoPi@@3NB` (the agent's _TWO_PI was respelled at landing; 0x00853830 still spells it ?Scale@NCameraModes@@3NB -- owner call).
- CEnginePrimitiveManagerStaticMeshesBase::DrawImmediately 0x00BA6DC0 (private) -- the full PDB signature set incl. the FableWin
  inline CEngineInternalPrimitiveMorphInfo::GetHiddenMaterials whose early `return 0` lands the table pointer in ebp directly.
At landing two stale callers were respelled and rechecked RELOCATION_MATCH: 0x00BD1F70 (UpdateVisibilityVector@CEngineMeshVisibility ->
InitialiseHiddenMaterials@CEngineInternalPrimitiveMorphInfo(const C3DMesh2&)) and 0x00BA6FA0 (RenderMeshPrimitive now takes
CEngineSubPrimitiveStaticMesh&, ERenderPassType); three binding rows replaced. Eight rows stay DIFFER in
`pending/2026-10-27_linux_hand636/differ/` (00b39fb0 UpdatePrimitives fadd/push schedule, 006a3a70 _M_insert_overflow ebx/edi tie,
00978790 + 0097abf0 anonymous-namespace CThingCompare operator() twins (fstp/fnstsw order; `namespace { }` per source mirrors the two
PDB hashes), 00d9eb30 CMC_MinionGuard::Main lea/mov ecx order, 00a198c0 CNavigatorFlyer::GetRandomNavigablePos x87 canonical order,
00405220 TransferIMEColour movzx ch/cl packing, 009210a0 BeetleToggleHover::IsRunnable this/creature swap). Lessons: FableWin-only
inline accessor with early return 0 moves the result register; use the PDB inline helper even when the maths is identical; retail keeps
unreferenced out-of-line copies of inlined helpers; anonymous-namespace twins need no invented names; with CScriptThing.h define
operator== inline but never ~CScriptThing (+63 bytes); ebx/edi swap = "register colouring tie", park after two tries.
9 NAME_FIXES + 4 review rows queued (prefix c636). Labels: ghidra_out/labels_chunk636_2026-10-27.tsv.
