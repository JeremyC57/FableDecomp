# Chunk 643 -- 2 of 10 landed (2026-10-31)

Cut from the parked-row pool. Landed via `coordinator/land_chunk.sh 643` (vland --land 2/2 RELOCATION_MATCH + PASS, multi-check 0; new
bindings CSB::GetDistanceBetweenAgents 0x00A872F0, CSB::ShouldIgnoreAgent 0x00A87260 (PDB spellings), the never-written .bss bool
0x013D2D54 as `g_SBAgentsDisabled_013d2d54` (descriptive, no data name set), and the landed unary_negate::operator() 0x008AABC0 under its
FableWin spelling): CSBAgents::Process 0x00A873E0 -- the PDB takes the agent as a pointer (`PBVCSteeringAgent`), the PDB's inline
CSteeringAgent accessors fixed every vcall temp register, and an early-out that builds its own `Output = CKinematic(C2DVector(0,0))`
(with `force` declared after it) lets VC7.1 merge the zero stores ahead of the `je` and lay the frame out as retail; std::__find_if<C3DVector*,
unary_negate<CVectorFilter_FurtherFromPointThan> > 0x008AAC50 (catalog "find_if_3d_distance" is descriptive) -- the predicate written with
the PDB vector operators (`C2DVector delta(pos - CentrePos); return delta.GetSquaredMagnitude() > Radius * Radius;`) gives retail's x87
operand order; the landed out-of-line 0x008AABC0 still matches with that body. Eight rows stay DIFFER in
`pending/2026-10-27_linux_hand643/differ/` (0056f30b CloseInventory zero kept in ebx, 00b50370 CEngineShadowRenderer ctor -- catalog
CPSCOrbit is a propagated label -- constants in eax/ecx/edx, 009faf70 GenerateMipmapsWithGPU colouring tie, 00871290
CActionEventExplosion::SetDataFromString one register, 00c3d470 UpdateRenderState argument scheduling with the class unproven, 008bf500
GenericFireProjectileWeapon::FinishAction `&type` temp register, 00bbcc70 SetupPixelShader 411v415 register pressure, 00774730
CTCShop::SellItem esi/edi tie). Lessons: an early-out that copies a shared zero local can set the frame order, give it its own temporary;
respell parameter kind and the PDB's inline pointee accessors before touching register residues; a 3D `operator-` inside a 2D predicate
changes the x87 operand order, retry "x87 canonical order" parks with the PDB vector operators; STLport `*(begin()+n)` / `end()-begin()`
give retail's per-access mov/lea; `return strcmp()==0` gives `sete`, the CCharString null arm is the if/return form. 9 NAME_FIXES + 5
review rows queued (prefix c643): GetDataFromEventString in an anonymous namespace (policy decision; landed 0x00870D70 declares it with a
pointer out-param), CRenderTarget ctor non-const refs, InitialiseTextures private, GetPItem CBaseObject*, SetRenderTarget bool/ref,
SetViewport(const C2DBoxI&), RestoreCaptureBlock(), GFGetSystemManager, CSurface::Uninitialise (the last five carry many non-PDB bindings).
Labels: ghidra_out/labels_chunk643_2026-10-27.tsv.
