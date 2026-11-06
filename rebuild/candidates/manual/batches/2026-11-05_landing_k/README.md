# Landing 2026-11-05k: chunk 784 (1 of 6 rows landed, 5 parked)

One expert agent authored the rows; a container restart interrupted it once, and it was resumed. The landed row is plain C++ and
grades `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x009500E0 | CAIStateGroup_JackDragon::OnActivate | RELOCATION_MATCH PASS (10 grades) |

- **Shared-header warnings.** The row warns SHARED_HEADER_AVAILABLE for 7 row-local models: CDefinitionManager, CTCPhysicsBase,
  CTCTargeted, CThingSearchTools, CWorld, CAIBrain and CAIStateGroupBase.
  - The landed JackDragon siblings use the same local models, for example FlyOutOfGround 0x00951B20.
  - Moving them onto the shared headers needs retail virtual-slot layouts in those headers: CTCPhysicsBase slots
    30/31/38/63/66 and the static fastcall CWorld::GetFrame. That is header debt (agent_NOTES.md).
  - The verifier's default (non-strict) mode accepts the warnings, as for landing 2026-11-05a.
- **Headers** (`hdr_README.md`). Every change is a new opt-in macro, and hdrcheck2 reads `default=same` for each one.
  - fable_string.h: CWIDESTRING_TEXT_EQ, CWIDESTRING_FORCEINLINE_EQ, CWIDESTRING_SUBSTRING, CCHARSTRING_STARTSWITH.
  - CFontBank.h: CFONTBANK_STRING_HELPERS.
  - CANavQuadTreeNode.h: CANAVQUADTREENODE_AREA_OUTOFLINE.
  - CNavBlockedQuadTreeNode.h: CNAVBLOCKEDQUADTREENODE_COPY.
  - CRenderManager2D.h: CRENDERMANAGER2D_VIEWPORTBOXFLOAT.
  - So far only parked rows use them.
- **Landing:** `vland --land` (`logs/land.log`).
  - Labels: `ghidra_out/labels_landing_2026-11-05k.tsv` (0x009500E0, and 0x00A15890 = CNavigatorManager::OutputWorld).
  - 7 queue lines (tag `land0511k`):
    - 0x0048DAA2 CFontBank::GetStringExtents;
    - 0x00442870 ~vector<CEmbeddedHighlight>;
    - 0x009A0EF0 CStreamingFontBank::SetUseFixedFunction;
    - 0x00A8EDA0 SetupVertexBuffer's parameter types;
    - 0x00A93A60 CCountedPointer<C3DClothPrimitive>::DeleteData;
    - 0x00A14040 RotateXY;
    - 0x00A98600 vector<CCountedPointer<C3DClothPrimitive>>::resize.

## Parked (`pending/2026-11-05_linux_hand784/`), each after 10+ attempts
| Row | Grade | Residue |
|---|---|---|
| 0x00A15890 CNavigatorManager::OutputWorld | DIFFER(1983v1973) | Inline budget: retail calls push_back out of line before the loop but expands it inside; frame 0x94 v 0x8C |
| 0x007EBEA0 CTCActionUseBed::OnActionUse | DIFFER(2124v2145) | Register colouring (hero, IP vtable) and FAIL-block cross-jumping |
| 0x00BC6DB0 CEnginePrimitiveManagerText::RenderSubPrimitive | DIFFER(2217v2217) | SetRenderState operand / register order, CScopedShadersEnable dtor compare |
| 0x00A92900 C3DPrimitive2::LoadBinary | DIFFER(2283v2274) | Register colouring: retail keeps the rep count in edi, spilled to a dead parameter slot |
| 0x00491AD2 CSubtitleRenderer::ProcessEmbeddedHighlightTags | DIFFER(2240v2223) | VC7.1 peels the first newline-count compare; frame 0xAC v 0xA0 |
