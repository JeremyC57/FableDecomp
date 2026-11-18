# Relocation audit

The exact 1,057-byte leaf has 24 verified COFF operands.

| Offset | Retail target | Emitted symbol | Verification |
|---|---|---|---|
| +0x010 | 0x00652FA4 | `?Draw@CDrawQuickAccessIcons@NPlayerGui@@UAEXABVCEnginePrimitivePrepareInfo@@@Z` | Landed base body, both PDB names and native receiver. |
| +0x017 | 0x00643D7C | `?GetPlayer@CDrawBase@NPlayerGui@@QAEAAVCPlayer@@XZ` | Exact package binding. |
| +0x01E | 0x00487DD0 | `?PeekPControlledCreature@CPlayer@@QBEPBVCThingPlayerCreature@@XZ` | Exact package binding. |
| +0x05A | 0x004365B0 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | Exact package binding. |
| +0x077 | 0x00643D7C | `?GetPlayer@CDrawBase@NPlayerGui@@QAEAAVCPlayer@@XZ` | Exact package binding. |
| +0x07F | 0x00445950 | `?GetNumberOfQuickAccessItemButtons@CGamePlayerInterface@@QBEJXZ` | Both PDB names, adjacent button-count accessors, typed receiver and native vector size body. |
| +0x096 | 0x00581FA4 | `?GetQuickAccessItemGraphicIndex@CTCInventory@@QBEJJ@Z` | Exact package binding. |
| +0x0B3 | 0x00438570 | `?PeekGuiDef@CPlayerGui@@SIABVCPlayerGuiDef@@XZ` | Exact package binding. |
| +0x0D6 | 0x00643CDA | `?GFAnchorToBottomLeft@NPlayerGui@@YI?AVC2DVector@@ABV2@@Z` | Exact package binding. |
| +0x0FB | 0x01375744 | `?HUDRingAlpha@@3JA` | Exact package binding. |
| +0x143 | 0x00643D5D | `?ScaleAlpha@CDrawBase@NPlayerGui@@QBEEE@Z` | Exact package binding. |
| +0x160 | 0x00643BE2 | `?GFToScreen@NPlayerGui@@YI?AVC2DVector@@ABV2@@Z` | Both PDB names, hidden vector-return argument and the retail two-float identity copy. |
| +0x16C | 0x00493AC2 | `??0CEnginePrimitive2DSprite@@QAE@_KABVC2DVector@@W4EEngine2DLayer@@KKW4EEnginePrimitiveBlendMode@@W4EEngineInterpolationSet@@MMEEEEK11@Z` | Exact package binding. |
| +0x1EE | 0x00643CDA | `?GFAnchorToBottomLeft@NPlayerGui@@YI?AVC2DVector@@ABV2@@Z` | Exact package binding. |
| +0x204 | 0x01375744 | `?HUDRingAlpha@@3JA` | Exact package binding. |
| +0x259 | 0x00643D5D | `?ScaleAlpha@CDrawBase@NPlayerGui@@QBEEE@Z` | Exact package binding. |
| +0x27A | 0x00643BE2 | `?GFToScreen@NPlayerGui@@YI?AVC2DVector@@ABV2@@Z` | Both PDB names, hidden vector-return argument and the retail two-float identity copy. |
| +0x288 | 0x00493AC2 | `??0CEnginePrimitive2DSprite@@QAE@_KABVC2DVector@@W4EEngine2DLayer@@KKW4EEnginePrimitiveBlendMode@@W4EEngineInterpolationSet@@MMEEEEK11@Z` | Exact package binding. |
| +0x2C0 | 0x013B8790 | `?PClass@?$CTBaseSingletonNoDefaultAlloc@VCPlayerGui@@@@0V?$CCountedPointer@VCPlayerGui@@@@A` | Exact package binding. |
| +0x2DB | 0x0122DCB4 | `__real@4f800000` | Compiler literal matches retail data. |
| +0x2F8 | 0x01375744 | `?HUDRingAlpha@@3JA` | Exact package binding. |
| +0x341 | 0x00643D5D | `?ScaleAlpha@CDrawBase@NPlayerGui@@QBEEE@Z` | Exact package binding. |
| +0x35C | 0x00643BE2 | `?GFToScreen@NPlayerGui@@YI?AVC2DVector@@ABV2@@Z` | Both PDB names, hidden vector-return argument and the retail two-float identity copy. |
| +0x36D | 0x00493AC2 | `??0CEnginePrimitive2DSprite@@QAE@_KABVC2DVector@@W4EEngine2DLayer@@KKW4EEnginePrimitiveBlendMode@@W4EEngineInterpolationSet@@MMEEEEK11@Z` | Exact package binding. |
