# Landing 2026-11-05t: chunk 793 (0 of 6 rows landed, 6 parked)

One expert agent worked six x87-heavy engine rows (10+ attempts each, every attempt in `pending/2026-11-05_linux_hand793/ATTEMPTS.log`).
None matched, so nothing entered `src/compiled` and coverage is unchanged. The six best sources, their tests (PASS under g++) and the
agent's header overlay are in `pending/2026-11-05_linux_hand793/`.

## Parked
| Row | Grade | Residue |
|---|---|---|
| 0x00A58B50 NBBBMaths::CSphere::GetFirstIntersectionWithTriangle | DIFFER(2211v2207), 155 lines, ~57 attempts | x87 operand order inside coef_b / coef_c and 4 bytes of slot layout |
| 0x00A593F0 NBBBMaths::CCylinder::GetFirstIntersectionWithTriangle | DIFFER(3043v3027), 540 lines | the first ~230 instructions match; VC7.1 hoists points._M_start out of the edge loop, plus register colouring |
| 0x00ADE080 C3DBoundingBox::InitialiseFromOBB | DIFFER(2696v2735), 1280 lines | an SSE arm that looks hand-written (a pointer local stored and reloaded at once, 0x00ADE329 / 0x00ADE330 / 0x00ADE334); 4 intrinsic and 13 plain spellings tried |
| 0x00BC8A60 CEnginePrimitiveManagerLines::RenderLine | DIFFER(3323v3416), 1006 lines | the near-plane early return moved to the end, `this` in ebp, slot layout, the CRenderStateManager bookmark flip |
| 0x009A25D0 CStreamingFontBank::Render | DIFFER(3715v3722), 282 lines | slot order of the line / cursor floats, eax/edx colouring, the bookmark flip |
| 0x00715A70 NTCScriptedControl::CActionTalkBase::SetUpCamera | DIFFER(2860v3356), 1269 lines | VC7.1 merges focus cases 1..4 into one tail and hoists from.Z; retail keeps 0.0 on the FPU stack and writes each case out |

## Open decisions
- **Split the 0x00ADE080 row.** Its ledger extent holds a second function, 0x00ADEB30..0x00ADEBBB (`ret 4`):
  C3DBoundingBox::InitialiseFromSphere(const C3DBoundingSphere&) (Ego_r name; the C3DBoundingSphere layout, Center +0 and
  Radius +0xC, matches the bytes). The agent graded against a 2735-byte cut. Labelled now; the catalog split waits until the row
  lands.
- **The 0x00ADE080 SSE arm.** If it is proven to be original inline asm, ASM_RULES.md allows an isolated macro-wrapped `__asm` in a
  shared helper header with a `FABLE_PORTABLE` twin and a dual-path test; it needs the address-and-instruction proof first.

## Names and headers
- Labels: `ghidra_out/labels_landing_2026-11-05t.tsv` (the six rows, InitialiseFromSphere, the std::transform instantiation
  0x00A5A9D0, CMatrix4x4::AlignedTransformTransposedAssumeW 0x00A5CB90, CGraphicInfoFrame::CFrameData::BeginLoading 0x009FDE80).
- 5 queue lines (tag `land0511t`): 0x00A58720 is (anonymous namespace)::CTransformWorldCoordsToPlaneCoords::operator() (landed as
  CProjectOntoPlane::operator()); 0x00A5A9D0's binding Vector_InsertMany_PairFloat; 0x009FDE80's binding NUISystem::CFrameData from
  0x0041B550; 0x013CA2F8 / 0x013CA300 are CStreamingFontBank::AlphaMaskUVMul / AlphaMaskUVAdd (landed 0x009A0EC0 calls them
  g_StreamingFontAlphaMaskUV0/1; public access assumed).
- Headers: not copied into rebuild/include, because no landed row needs them. The overlay (new NBBBMaths_CSphere.h; opt-ins
  FABLE_RETAIL_C3DBOUNDINGBOX_OBB, FABLE_RETAIL_CMATRIX4X4_ALIGNEDTRANSFORM, FABLE_RETAIL_CSTREAMINGFONTBANK_GLYPHLOOKUP) is in
  `pending/2026-11-05_linux_hand793/hdr/`; the agent rechecked the 11 landed includers with it (all RELOCATION_MATCH PASS,
  SIBLINGS.tsv). Header debt is listed in `agent_NOTES.md`.
