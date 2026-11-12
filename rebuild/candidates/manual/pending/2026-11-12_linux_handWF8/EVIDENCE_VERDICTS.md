# Workflow wf8 verdicts (two independent Opus lenses per row: mechanical + evidence; one Opus repair pass)

## 0091b3a0 -- VERIFIED, author attempts 1

- What made it match: The match comes from the port onto the shared headers plus a named CCreatureAction_PlayAnimation local. In the repair, the direct read of LastPerceivedPos through the friend grant produces the same bytes as the inline accessor did (`lea edi,[ebp+20h]`).

## 00b81f30 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: Unchanged from the author's version: a real CCountedPointer<const C3DMesh2> local with early returns, a shared CArray for ClothPrimitives, CParticleContext as a polymorphic base at +0, the inline pair HasLOD / GetBestCurrentDetailLevel, and the loop naming order. The const-to-non-const change to MeshInfo is codegen-neutral because all the accessors are const.
- OPEN at hand-back: The model class CEngineClothSimulationMeshBase pads +0x18..+0xA8 with a made-up member `unsigned char _transforms[0x90]`, but Ego_r.pdb (struct_layouts_egor.tsv) names those bytes as three CMatrix3x4 members CurrentTransform, LastTransform and InverseTransform (PDB +28/+76/+124, retail +0x18/+0x48/+0x78, 0x30 bytes each), and the shared header rebuild/include/engine/CMatrix3x4.h exists. The names-right rule allows a made-up name only when no original is found, so the pad should be replaced by the three PDB-named CMatrix3x4 members.

## 008c61c0 -- VERIFIED, author attempts 1

- What made it match: The source is unchanged apart from the Identity comment. The original match came from the shared headers, the inline normalise in the row, and the visible GetSubDef template body. Overlay opt-ins: FABLE_RETAIL_CCREATUREACTION_TROLLWHACKGROUNDBASE_CLASS and FABLE_RETAIL_CCOMBATACTIONBASE_OWN_VIRTUALS.

## 006d0630 -- MATCH_WITH_OPEN_ISSUES, author attempts 1

- What made it match: The source is unchanged from the author's match, which uses the FIND_MEMBER_TEMPLATE_OUTOFLINE opt-in (find with its body visible but noinline). The removed DECL branch was never used by the matched source.
- OPEN at hand-back: The hdr/stlport/fable_map.h overlay (FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_OUTOFLINE, with its rewritten line-201 `#ifdef` -> `#elif` and a `__declspec(noinline)` find<KT>) is not needed: grading the row with an overlay that holds only hdr/engine/CTCWife.h still gives RELOCATION_MATCH, so the source comment's claim that this branch is what keeps find<long> out of line is wrong, and the overlay plus its `#define` should be dropped before landing.
- OPEN at hand-back: The row's Identity comment gets the callers wrong. It says the function is "called by CCreatureActionGiveMoneyToThing::PerformAction 0x00845040 (0x00844D68, 0x0084521A)". The PDB spelling is CCreatureAction_GiveMoneyToThing, with an underscore (ghidra_out/engine_api.tsv: ?PerformAction@CCreatureAction_GiveMoneyToThing@@UAEXXZ at 0x00845040). Also, call site 0x00844D68 is not inside 0x00845040: it falls inside ?PerformAction@CCreatureAction_GiveItemToThing@@UAEXXZ at 0x00844BC0 (rebuild/manifest/functions.tsv). The comment should name both callers, CCreatureAction_GiveItemToThing::PerformAction 0x00844BC0 (0x00844D68) and CCreatureAction_GiveMoneyToThing::PerformAction 0x00845040 (0x0084521A).

## 0083d990 -- VERIFIED, author attempts 1

- What made it match: Moving the row off its local structs onto the shared headers (fable_string.h, engine/CThing.h, engine/CThingPhysicalSwitch.h, engine/CStringParser.h) in the landed TrackNode 0x00837E40 idiom: `const char*` param, `CStringParser parser(script)` with the header default args, `break` on EndThing, and `TriggerableObjectScriptName = (value == "NULL") ? CCharString("") : value;`. The last 4 bytes came from making the bodies of CBasicString<T>::Compare / operator==(const T*) visible as row-local template bodies (landed 0x00837E40 / 0x009BB110 precedent). With them the key sits in the frame slot, the value in the dead `script` arg slot, and the key's PStringData stays cached in esi across both compares, while 0x004115A0 is still called out of line. No flags, no pragmas. Source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf8/0083d990/0083d990.cpp

## 0072eb40 -- VERIFIED, author attempts 1

- What made it match: The move onto the shared headers alone. The old 467v463 residue (`shr ; and` against retail's `test dword ptr [esi+28h],80000h`) came from the draft's local bool HasTC; engine/CThing.h's __forceinline HasTC (FABLE_RETAIL_CTHING_HASTC_FORCEINLINE) plus the expanded GetTC (FABLE_RETAIL_CTHING_GETTC_EXPANDED) give retail's in-place bit test. Signatures were corrected from Ego_r.pdb and FableWin.pdb: the `push 1` before each GetPrice call is AddMoney's bool, not a GetPrice argument. Source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf8/0072eb40/0072eb40.cpp; test: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf8/0072eb40/0072eb40_test.cpp (vt.py PASS). One made-up name, listed in NOTES.md: enumerator HOUSE_QUESTION_DRESS (= 3), which has no name in either PDB.

## 00ba4a40 -- VERIFIED, author attempts 1

- What made it match: Unchanged by the repair (comment and README edits only). The row was already matched by porting it onto the shared headers, putting FABLE_FORCEINLINE on PeekGraphicInfoFrame, and declaring texture_type before the bank-index read.

## 007bbe34 -- VERIFIED, author attempts 1

- What made it match: Three header facts on top of the port, none of them a spelling of the row body:
- the CRightHandedSet constructor body visible but still called out of line (CTOR_INLINE) gives the temporary its own slot, so the frame is 0x5C;
- the default C3DVector::operator*(float), without the X-product grouping the Predictive sibling uses, keeps the 1.5 constant on the x87 stack (`fld K ; fmul st(1..3) ; fst Z ; fstp st(0) ; fstp st(0)`);
- CROSS_XY_GROUP moves the scheduling-region cut so the ApplyAcceleration push comes after the velocity products.
The edi copy of the target comes from passing a cast temporary to IS_THING_ALIVE<const CThing*>.
0.333f and 1.5f are file `static const float` (this TU's own .rdata at 0x0126DF24 / 0x0126DF28) and the angle is a file static float (0x013BAE7C). All three names are descriptive, since none was found in any PDB or string, and are listed in NOTES.md.

## 00a128e0 -- VERIFIED, author attempts 1

- What made it match: Moving the row onto the shared headers as the landed siblings 0x00A11D10 / 0x00A11F20 spell it (named `CMatrix3x4 inverse; inverse.SetInverse(PositionMatrix); C3DVector local; inverse.TransformPoint(pos, local);`, first argument `const CCharString&`), then `float scaled_radius = radius * OO_Scale;` as a separate local declared after the transform and before the named counted copy `CIVCountedPointer<const CPhysicsMesh> mesh(Mesh);`. VC7.1 homes that local in the dead `pos` argument slot ([esp+50h]) as retail does. The dossier's open risk (cofactor operand order) needed no header math change. Source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf8/00a128e0/00a128e0.cpp; test: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf8/00a128e0/00a128e0_test.cpp; payload.json and oracle.tsv written by addmatch.py in the same directory; NOTES.md appended.

## 00b903d0 -- VERIFIED, author attempts 1

- What made it match: The port onto the shared headers alone. The old ebx/ebp "register tie" was a model error: the draft's made-up pointer-taking `DrawIndexed(..., long type, ...)` helper was replaced by the PDB's inline seven-argument `CShaderRenderManager::DrawBuffersIndexed(CVertexBufferWin32&, K, K, CIndexBuffer&, K, K, EPrimitiveType)`, called with `*primitive->VertexBuffer`, `*primitive->IndexBuffer` (scoped_ptr), and `block->GetStartIndex()`, `GetPrimitiveCount()`, `GetPrimitiveType()` passed directly. The block count and vertex count are read into locals before the loop through `GetStaticBlockCount()` / `GetVertexCount()`, and the fixed-function test goes through `IsFixedFunctionVertexMode()`. No flags, no pragma, no forceinline. Final source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf8/00b903d0/src.cpp; payload.json and oracle.tsv are written in the same directory.

