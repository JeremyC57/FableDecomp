# Landing: six re-port sets, hand3c2 b8-b11 / b13, hand3c3 b1-b2 (Account B, 2026-11-20)

27 rows, all MATCH / RELOCATION_MATCH with tests passing, under strict vland `--land` and the widened shared-header policy. Audit:
`audit_REPORT.md` (Fable 5.2 auditor; every fixed set re-run on the tree of 01:20-01:50 UTC). The set was regraded here after merging
Account A's nf111 / nf113: 27 of 27 pass, and hdrcheck2 gives default=same for all 19 changed headers plus one new. All 17 same-TU copies
in the landed rows were checked after keypayload and are their owners' text verbatim.

**New rows (10):**
| address | row | notes |
|---|---|---|
| 0x00B99300 | CEnginePrimitiveManagerRepeatedStaticMeshes::ProcessLightingSW | audit fix: Ego_d statics `aa` / `bb`; public wrapper SetVertexShaderConstantsDirect instead of an invented friend |
| 0x00646762 | NPlayerGui::CStatBar::DrawStatBarOnLeft | same-TU GFAnchorToBottomLeft copy made the owner's text verbatim |
| 0x00ADF960 | CParticleSimulation::SatisfyDistanceConstraints | /Zi; `max()` is windef.h's macro verbatim through the new opt-in FABLE_RETAIL_WIN32_MINMAX in engine/Win32Types.h (coordinator decision: retail got it from windows.h) |
| 0x00591B7F | CTCInventory::UpdateContextSensitiveItems | /Gr |
| 0x00A48F10 | CLipSyncSampleData::GetFrameInterpolate | |
| 0x009ACEE0 | CDefinitionManager::ParseEnumForSymbols | hand3c3 |
| 0x00A4C1F0 | C3DAnimationSequenceData::GetKeyframeInterpolate | hand3c3; inline helper chain written to Ego_d's line-table shapes; `// Inferred:` goto choice |
| 0x00A52760 | C3DAnimationSequenceData::InlineGetKeyframeInterpolate | hand3c3 (uncalled COMDAT copy) |
| 0x00A53860 | C3DAnimationSequence::InlineGetKeyframeInterpolate | hand3c3 (uncalled COMDAT copy) |
| 0x009FDE80 | CGraphicInfoFrame::CFrameData::BeginLoading | e17 (Ego_d lane); new engine/C3DExtentsI.h |

**Re-ports (17):** 0x00643CDA GFAnchorToBottomLeft; nine C3DExtentsI / CVolume rows (0x009FD9B0, 0x00A67530, 0x00A677D0, 0x00A67860,
0x00A67950, 0x00A67A20 now CVolume::DrawBox, 0x00A685E0, 0x00A68910, 0x00A699D0) moved off every local class model; 0x0098B930
SetVSConstants in Ego_d spelling, with its copies in 0x00989230 / 0x00989CB0; 0x00774C60 StockDisplay's copies; 0x00A56670
GFGetHermiteSplinePosition and its copy in 0x00A567A0; 0x00518DC0 filter operator().

## Coordinator decisions
- **b10 max():** windef.h's macro in the shared Win32Types.h shim (retail-faithful, portable), chosen over the CRT `__max` or a hand expansion.
- **hand3c3 b2 helpers:** each row keeps its own text of GFFastArcCos / GFFastSin / CQuaternion::Dot / SetSlerp, as 13-19 landed rows already
  do per helper. Moving them into shared headers (a new GFFastArcCos.h plus nested opt-ins) is queued as a follow-up wave.
- **navedge held:** 0x00A78800's audit fix needed a scoped `#pragma inline_depth(0)`, a device retail did not have (owner rule
  2026-11-02). The set's other rows depend on its respelling, so all 8 navedge rows (with hand3c2 b5) wait until 0x00A78800 matches without it.
- **hand3c2 b12 0x00BADE40 held:** its new CEngineSubPrimitiveSprite.h would newly fail landed 0x00BB1740 / 0x00BACE10 under the widened
  policy; they get re-ported first.

## For Account A
name_fixes/ (one file per set). Among them: CVolume::DrawBox 0x00A67A20, the Ego_d statics `aa` / `bb`, SetVertexShaderConstantsDirect
0x00B9C060, PeekRenderInfo / PeekLights, GetContextSensitivePreferredItemPriority 0x00581BDE, GetQuickAccessIndexFromButtonIndex 0x005819F2,
GFastArcCosTable / GFastCosTable, the two InlineGetKeyframeInterpolate copies, and the EVolumePool / EGraphicBankNo enumerators.

## Open items found by the audit
- Landed regression: 0x00589A30 CTCInventory::OpenPCMapMenu grades DIFFER(549v549) on today's tree with no overlay (being investigated).
- Landed rows the widened policy flags beside these sets: 0x00A77420, 0x00A68370 (7 models now), 0x00BB1740, 0x00BACE10.
- Tests that model rather than link the row (pre-existing): 0x00A68910, 0x00774C60, 0x00518DC0, 0x0098B930.
