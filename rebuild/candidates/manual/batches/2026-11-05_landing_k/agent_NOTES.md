# Chunk 784: notes

## Results (1 matched, 5 parked)
All six row names are proven: each leaf symbol is in both Ego_r.pdb and FableWin.pdb, and the bodies agree.

| Row | Grade | Attempts | Shape / key spellings / residue |
|---|---|---|---|
| 0x009500E0 CAIStateGroup_JackDragon::OnActivate | RELOCATION_MATCH | 11 | Local CIntelligentPointer model with the `T* const&` ctor (source load after the base ctor). Same-TU `__declspec(noinline)` GetSubDef<CJackDragonDef> body. All five marker lookups share ONE block and one `CThing* marker` (the marker slot then holds the spill). `FrameWhenLastInRange = FrameOfLastSlam = GetWorld().GetFrame();` (chain store order). `DragonPosIntoGround3 = a + GetPos() + b + C3DVector(0,0,def->DragonGroundHeightOffsetBodge)`: the dead zero temporaries give retail's frame. |
| 0x007EBEA0 CTCActionUseBed::OnActionUse | parked DIFFER(2124v2145), rd2 488 | 12 | `// Flags: /GS` (pass `--flags /GS` to q.py). Inline `CIntelligentPointer(T&)` + default ctor, and the out-of-line T* ctor 0x004A96F0 + operator= 0x004A9760 only at the owner site. Nested village ifs with `else occupied = true`. Residue: register colouring (VC7.1 puts the hero in ebp and the IP vtable in ebx; retail puts the user in ebp, spills the hero to [esp+1Ch] and uses the vtable as an immediate) and FAIL-block cross-jumping (retail keeps the berserk FAIL apart). |
| 0x00BC6DB0 CEnginePrimitiveManagerText::RenderSubPrimitive | parked DIFFER(2217v2217), rd2 214 | 10 | Size match. Text position assigned member-wise (`pos.X = sub.Pos.X + viewport.TLX; pos.Y = viewport.TLY + sub.Pos.Y; pos.Z = sub.Pos.Z;`). KeepMaskFiltering read through a pointer to the +0x0C texture sub-object, which reproduces retail's `add ebx,0Ch` ... `+8` split. The CTextMaskTexture / CTextMaskSettings layout is DESCRIPTIVE (offsets proven, names not). Residue: SetRenderState operand / register order and the CScopedShadersEnable destructor compare form. |
| 0x00A92900 C3DPrimitive2::LoadBinary | parked DIFFER(2283v2274), rd2 59 | 11 | Same-TU noinline STLport _M_fill_insert / _M_insert_overflow. `CCountedPointer<C3DClothPrimitive>& cloth = ClothPrimitives[c]`. `unsigned char* indices = IndexBuffer->Lock(0,0,0);` as a local. Residue: register colouring only. Retail keeps the rep count in edi (spilled to the dead memory-pool param slot), and VC7.1 gives it its own slot. |
| 0x00491AD2 CSubtitleRenderer::ProcessEmbeddedHighlightTags | parked DIFFER(2240v2223), rd2 185 / rd3 113 | 10 | `#pragma optimize("s", on)` ONLY gives the biased `lea ebp,[esp-78h]` frame. The fable_string.h / CFontBank.h opt-ins (hdr/README.md). `Highlights()[m]` re-indexed at every use. Residue: VC7.1 peels the first newline-count compare against the known 0, and the frame is 0xAC vs 0xA0 (five extra temp slots). |
| 0x00A15890 CNavigatorManager::OutputWorld | parked DIFFER(1983v1973), rd2 446 | 13 | Out-of-line GetAreaCovering and blocked-node copy ctor (opt-ins). Local `__fastcall std::__find / __find_if` declarations with inline find / find_if. `first / blocked / start` declared once before the while loop (frame 0x94 vs 0x8C). Residue: the inliner budget. Retail calls push_back 0x00A17400 / 0x00A17440 out of line at the three pre-loop sites but expands the in-loop ones; VC7.1 inlines the first pre-loop site and calls the type helper out of line. Retail also has the zero register ebx and the file in edi. |

Next ideas: line 3 of each differ/ source.

## Callee names established (evidence)
- 0x0048DAA2 CFontBank::GetStringExtents(const CWideString&, bool) const (FableWin) and 0x0048DA62 CFontBank::GetStringWidth (Ego_r + FableWin), both called by ProcessEmbeddedHighlightTags. The landed 0x0048DAA2 name is wrong (NAME_FIXES).
- 0x00442870 ~vector<CSubtitleRenderer::CEmbeddedHighlight> (0x14-byte records, caller inside 0x00491AD2). The landed name is wrong (NAME_FIXES).
- 0x009A0EF0 CStreamingFontBank::SetUseFixedFunction (Ego_r order after SetAlphaMaskUVs 0x009A0EC0). Flag byte 0x013C5A7C: its name is descriptive.
- 0x00A14040 C3DVector::RotateXY: FlyOutOfGround's RotateAboutZ declaration is wrong. 0x00A8EDA0 SetupVertexBuffer is private with EVertexType. 0x00A93A60 CCountedPointer<C3DClothPrimitive>::DeleteData. 0x00A98600 vector<CCountedPointer<C3DClothPrimitive>>::resize.
- OutputWorld: GetNodeAt 0x00A14F70, GetNextNeighbour 0x00A15140, push_back 0x00A17400 (const CANavQuadTreeNode*) / 0x00A17440 (CNavBlockedQuadTreeNode*), _M_insert_overflow 0x00A17190 / 0x00A17270 / 0x004416E0, __find<C2DVector*> 0x00A169B0, __find_if<IsSameNavQuadTreeNode> 0x00A16550. The predicate IsSameNavQuadTreeNode is proven by FableWin.pdb: ??0IsSameNavQuadTreeNode@@QAE@PBVCANavQuadTreeNode@@@Z and ??RIsSameNavQuadTreeNode@@QBE_NPBVCANavQuadTreeNode@@@Z, a class with ctor(const CANavQuadTreeNode*) and operator() const, as modelled.
- The subtitle locals CWStringParser (0x00437260 ctor, ReadAsStringUntilCharacter 0x00A0CCD0, SkipWhitespace 0x00A0C910), CPlayerGuiDef TextBoxTextAreaTLPos +0x5A4 / BRPos +0x5AC, g_screenHeight 0x013961F0 and g_FableLeftAlignText 0x013B861B are local models; the two global names are DESCRIPTIVE.

## Labels to correct
See LABELS.tsv. 0x00A15890 is OutputWorld (the catalog says InitializeNavigationData). Also 0x0048DAA2, 0x00442870, 0x009A0EF0, 0x00A93A60 and 0x00A98600.

## Header debt
None. Every overlay edit is an opt-in (hdr/README.md). CNavigatorManager has no retail member-function declarations in its header: the row declares GetNodeAt / GetNextNeighbour in a local derived class. A later chunk could add them under FABLE_RETAIL_CNAVIGATORMANAGER.

## Verifier (read-only vland, overlay set)
009500E0 RELOCATION_MATCH PASS, no RELOC_IDENTITY. It warns SHARED_HEADER_AVAILABLE for the local CDefinitionManager, CTCPhysicsBase,
CTCTargeted, CThingSearchTools, CWorld, CAIBrain and CAIStateGroupBase models. These follow the landed JackDragon siblings (for example
FlyOutOfGround 0x00951B20 keeps local CTCPhysicsBase / CPhysicsBody models). Moving them to the shared headers needs retail
virtual-slot layouts in those headers (CTCPhysicsBase slots 30/31/38/63/66, the static fastcall CWorld::GetFrame), which is header
debt for a later chunk.
