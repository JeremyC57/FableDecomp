# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x26 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x00919265 | 0x004C73D0 | reloc_bindings |
| +0x2F | `__ftol2` | 0x0091926E | 0x00BFEA70 | reloc_bindings |
| +0x39 | `__ftol2` | 0x00919278 | 0x00BFEA70 | reloc_bindings |
| +0x6B | `?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ` | 0x009192AA | 0x008FD000 | reloc_bindings |
| +0x72 | `??$GetAllThingsInRoughArea@VCThingGameObject@@V?$vector@PAVCThingGameObject@@V?$allocator@PAVCThingGameObject@@@std@@@std@@VCThingFilter_HasTC@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_HasTC@@AAV?$vector@PAVCThingGameObject@@V?$allocator@PAVCThingGameObject@@@std@@@std@@@Z` | 0x009192B1 | 0x00919780 | NAME_FIXES_QUEUE |
| +0x85 | `?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ` | 0x009192C4 | 0x008FCFD0 | reloc_bindings |
| +0x9C | `?GFROR13@@YIJK@Z` | 0x009192DB | 0x00497890 | reloc_bindings |
| +0xAD | `?ConstantFPS@CMainGameComponent@@1JA` | 0x009192EB | 0x01375550 | reloc_bindings |
| +0xB4 | `?GetFrame@CWorld@@SIJXZ` | 0x009192F3 | 0x0049D870 | reloc_bindings |
| +0xCC | `_free` | 0x0091930B | 0x00BFEA14 | reloc_bindings |
| +0x11F | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x0091935E | 0x0040F020 | reloc_bindings |
| +0x151 | `?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ` | 0x00919390 | 0x008FCFD0 | reloc_bindings |
| +0x15D | `?GFFloatRandom@@YIMMPAK@Z` | 0x0091939C | 0x004978A0 | reloc_bindings |
| +0x19F | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x009193DE | 0x0040F020 | reloc_bindings |
| +0x1EB | `_free` | 0x0091942A | 0x00BFEA14 | reloc_bindings |

15 supported pairs out of 15. See the separate manual audit where supplied.
