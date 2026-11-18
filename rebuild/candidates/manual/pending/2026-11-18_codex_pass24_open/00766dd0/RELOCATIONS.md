# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0x42 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x00766E11 | 0x0040F020 | reloc_bindings |
| +0x63 | `?PlayAttachedCriteria@CTCSoundPlayer@@QAEKABVCCharString@@_N11J@Z` | 0x00766E32 | 0x0066BD00 | reloc_bindings |
| +0xE2 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x00766EB1 | 0x0040F020 | reloc_bindings |
| +0x103 | `?PlayAttachedCriteria@CTCSoundPlayer@@QAEKABVCCharString@@_N11J@Z` | 0x00766ED2 | 0x0066BD00 | reloc_bindings |
| +0x165 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00766F34 | 0x00A01B50 | reloc_bindings |
| +0x170 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00766F3F | 0x00A01B50 | reloc_bindings |
| +0x186 | `?HasCombinationOfAugmentations@CTCObjectAugmentations@@QBE_NW4EObjectAugmentationType@@@Z` | 0x00766F55 | 0x00766050 | reloc_bindings |
| +0x19B | `?HasAugmentationOfType@CTCObjectAugmentations@@QBE_NW4EObjectAugmentationType@@@Z` | 0x00766F6A | 0x007660C0 | reloc_bindings |
| +0x1B0 | `?HasAugmentationOfType@CTCObjectAugmentations@@QBE_NW4EObjectAugmentationType@@@Z` | 0x00766F7F | 0x007660C0 | reloc_bindings |
| +0x1D0 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x00766F9F | 0x004C73D0 | reloc_bindings |
| +0x1D9 | `?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z` | 0x00766FA8 | 0x006E0880 | reloc_bindings |
| +0x1E4 | `??0CBaseIntelligentPointer@@QAE@XZ` | 0x00766FB3 | 0x00A01B10 | reloc_bindings |
| +0x1F1 | `??_7?$CIntelligentPointer@VCThing@@@@6B@` | 0x00766FBD | 0x01238C6C | reloc_bindings |
| +0x1F6 | `?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z` | 0x00766FC5 | 0x00A01B90 | reloc_bindings |
| +0x1FF | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00766FCE | 0x00A01B50 | reloc_bindings |
| +0x207 | `?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z` | 0x00766FD6 | 0x00A01B90 | reloc_bindings |
| +0x210 | `??1CBaseIntelligentPointer@@UAE@XZ` | 0x00766FDF | 0x00A01C10 | reloc_bindings |
| +0x217 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00766FE6 | 0x00A01B50 | reloc_bindings |
| +0x222 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00766FF1 | 0x00A01B50 | reloc_bindings |
| +0x232 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x00767003 | 0x00A01B50 | reloc_bindings |
| +0x251 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x00767022 | 0x0040F020 | reloc_bindings |
| +0x283 | `?AttachToThing@CTCDParticleEmitter@@QAEXAAVCThing@@JABVCCharString@@JM@Z` | 0x0076704E | 0x006E0BE0 | reloc_bindings |
| +0x2B9 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x0076707F | 0x00A01B50 | reloc_bindings |
| +0x2C4 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x0076708A | 0x00A01B50 | reloc_bindings |
| +0x353 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x0076710A | 0x004AC0F0 | Needs binding evidence |
| +0x35B | `?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z` | Unresolved | Unresolved | Instruction pairing differs |
| +0x3BF | `?resize@?$vector@V?$CIntelligentPointer@VCThing@@@@V?$allocator@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@QAEXI@Z` | 0x00767137 | 0x00739B40 | reloc_bindings |
| +0x3FA | `?AttemptToStartParticle@CTCObjectAugmentations@@AAEXAAV?$CIntelligentPointer@VCThing@@@@JJABVCCharString@@1@Z` | 0x0076716F | 0x00766660 | Both original PDBs, private helper and native idle-particle rearm call |
| +0x447 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x007671B8 | 0x00A01B50 | reloc_bindings |
| +0x452 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x007671C3 | 0x00A01B50 | reloc_bindings |

28 verified operand bindings. 2 operands still need an unambiguous instruction or binding.
