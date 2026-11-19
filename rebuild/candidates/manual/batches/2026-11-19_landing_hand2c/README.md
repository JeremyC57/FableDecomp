# Landing hand2c (2026-11-19): 6 of the 98 not-plain-C++ rows (1,708 bytes)

vland --land, LANDVERIFY_STRICT_HEADERS=1: RELOCATION_MATCH + PASS for all six (`logs/vland_land.log`); no flags. Header overlay merged
(all additive: five opt-ins + the new engine/CEngineSceneMap.h; `hdr_README.md` has one line per opt-in with evidence).

| row | notes |
|---|---|
| 0x006C06D0 CTCBuyableHouse::GetRentMoneyPos (312) | opt-in FABLE_RETAIL_CTCBUYABLEHOUSE_GETRENTMONEYPOS (declaration; Ego_r + FableWin name) |
| 0x007E2DF0 CTCWobble::OnInitialActivate (257) | opt-in FABLE_RETAIL_CTCWOBBLE_ONINITIALACTIVATE; GFFloatToLongCeil retail-asm helper; callee slot 75 is CTCPhysicsBase::SetRHSet |
| 0x00A62AE0 CPackedUIntArray::CalculateBias (130) | opt-in FABLE_RETAIL_CPACKEDUINTARRAY_CALCULATEBIAS (private inline static ReadNBytes, private static BitMasks[]); Ego_d local names, the one declaration order of 24 that homes the locals in the dead argument slots as retail |
| 0x00B947E0 CEngineSceneMap::Update (246) | new engine/CEngineSceneMap.h (Ego_d class record) + opt-in FABLE_RETAIL_CENGINESCENE_GETMAXAVERAGEPRIMITIVESPERCELL |
| 0x00F36010 NFractal::CPerlinNoise::Get2DNoise (442) | catalog label was GetGaussian2DNoise (wrong); opt-in FABLE_RETAIL_CPERLINNOISE_GETNOISE on the float-literal header (SCurve t*t*(3.0f-(2.0f*t)), Interp a+((b-a)*t)); GFFloatToLongFloor helper |
| 0x00F36540 NFractal::CPerlinNoise::GetGaussian2DNoise (321) | same opt-in and helper. Retail's own bytes settle the SCurve grouping: every tree change breaks both rows |

0x00F361D0 Get3DNoise shares FABLE_RETAIL_CPERLINNOISE_GETNOISE and is still open (one extra no-code tuple in its first 81-tuple
scheduling region; Fable advisor round 3). `NAME_FIXES.tsv`: 5 queued (GetRentMoneyPos, Get2DNoise, the SetRHSet callee of the landed
CTCWobble::FrameUpdate 0x007E2F00, ResizeLevels v the catalog's ResizeGrid, CPackedUIntArray::BitMasks v two landed spellings).
