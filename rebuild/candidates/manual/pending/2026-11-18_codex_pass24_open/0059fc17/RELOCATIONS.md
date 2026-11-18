# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0x15 | `?GetHairCardDef@CTCInventoryItem@@SI?AV?$CDefPointer@$$CBVCHairCardDef@@@@J@Z` | 0x0059FC2B | 0x005D9A10 | reloc_bindings |
| +0x2B | `?test@?$bitset@$0BBC@@std@@QBE_NI@Z` | 0x0059FC41 | 0x00410DE0 | reloc_bindings |
| +0x40 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x0059FC56 | 0x0040F020 | reloc_bindings |
| +0x5C | `?IsWearingAppearanceModifier@CTCHeroAttachableAppearanceModifiers@@QBE_NJW4EHeroAttachableAppearanceModifierType@@@Z` | 0x0059FC72 | 0x00706000 | NAME_FIXES_QUEUE |
| +0x6F | `?GetTattooCardDef@CTCInventoryItem@@SI?AV?$CDefPointer@$$CBVCTattooDef@@@@J@Z` | 0x0059FC85 | 0x005D9AA0 | reloc_bindings |
| +0x8A | `?test@?$bitset@$0BBC@@std@@QBE_NI@Z` | 0x0059FCA0 | 0x00410DE0 | reloc_bindings |
| +0x9F | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x0059FCB5 | 0x0040F020 | reloc_bindings |
| +0xD0 | `?GetTattoos@CTCHeroMorph@@QBEXAAV?$vector@V?$CDefPointer@$$CBVCTattooDef@@@@V?$allocator@V?$CDefPointer@$$CBVCTattooDef@@@@@std@@@std@@@Z` | 0x0059FCE6 | 0x0071DD50 | NAME_FIXES_QUEUE |
| +0x105 | `?HasAPermanentTattooCoveringArea@CTCHeroMorph@@QBEJXZ` | 0x0059FD18 | 0x0071C2D0 | Original long-returning method in both PDBs and native eax test |
| +0x115 | `??1?$vector@V?$CDefPointer@$$CBVCTattooDef@@@@V?$allocator@V?$CDefPointer@$$CBVCTattooDef@@@@@std@@@std@@QAE@XZ` | 0x0059FD28 | 0x00581353 | Native tattoo-vector iteration and typed caller, then CRT free |
| +0x166 | `??8?$CBasicString@D@@QBE_NABV0@@Z` | 0x0059FD75 | 0x004A93C0 | reloc_bindings |
| +0x17B | `?IsConflicting@CTattooDef@@QBE_NABV1@@Z` | 0x0059FD88 | 0x0059B679 | NAME_FIXES_QUEUE |
| +0x191 | `??1?$vector@V?$CDefPointer@$$CBVCTattooDef@@@@V?$allocator@V?$CDefPointer@$$CBVCTattooDef@@@@@std@@@std@@QAE@XZ` | 0x0059FD9E | 0x00581353 | Native tattoo-vector iteration and typed caller, then CRT free |
| +0x1A8 | `?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ` | 0x0059FDB5 | 0x006869D0 | reloc_bindings |
| +0x1AF | `??$GetDef@VCThingBaseDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z` | 0x0059FDBC | 0x0042AF3C | reloc_bindings |
| +0x1C5 | `??$GetSubDef@VCHeroTitleDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCHeroTitleDef@@@@@Z` | 0x0059FDD2 | 0x005B1E5A | reloc_bindings |
| +0x1DC | `?test@?$bitset@$0BBC@@std@@QBE_NI@Z` | 0x0059FDE9 | 0x00410DE0 | reloc_bindings |
| +0x1F1 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x0059FDFE | 0x0040F020 | reloc_bindings |
| +0x20B | `??1?$vector@V?$CDefPointer@$$CBVCTattooDef@@@@V?$allocator@V?$CDefPointer@$$CBVCTattooDef@@@@@std@@@std@@QAE@XZ` | 0x0059FE18 | 0x00581353 | Native tattoo-vector iteration and typed caller, then CRT free |

19 verified operand bindings.
