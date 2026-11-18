# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0xC | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00903F3B | 0x00A01B50 | reloc_bindings |
| +0x1B | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00903F4A | 0x00A01B50 | reloc_bindings |
| +0x82 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00903FB1 | 0x00A01B50 | reloc_bindings |
| +0xA0 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x00903FCF | 0x0040F020 | reloc_bindings |
| +0xBE | `??2@YAPAXI@Z` | 0x00903FED | 0x00BFEA1A | reloc_bindings |
| +0xD5 | `??0CActionMoveTo@NTCScriptedControl@@QAE@ABVCThing@@W4EMovementType@@_N2@Z` | 0x00904004 | 0x00905290 | Both PDBs and the landed constructor body |
| +0xE1 | `?AddAction@CTCScriptedControl@@QAEXPAVCActionBase@1@@Z` | 0x00904010 | 0x007137D0 | NAME_FIXES_QUEUE |

7 verified operand bindings. 0 operands still need an unambiguous instruction or binding.
