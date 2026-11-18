# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0xB | `??_C@_00CNPNBAHC@?$AA@` | 0x0067E29A | 0x0122D70E | literal payload prefix |
| +0x15 | `?CheckCRC@CPersistContext@@IAEXPBD@Z` | 0x0067E2A4 | 0x00404500 | reloc_bindings |
| +0x2D | `?WriteEBOOL@CDataOutputStream@@QAEX_N@Z` | 0x0067E2BC | 0x00993E30 | reloc_bindings |
| +0x3D | `?TransferBinaryIn@?$CPersistTraits@_N@@QAEXAAVCMemoryDataInputStream@@AA_N@Z` | 0x0067E2CC | 0x00403EB0 | reloc_bindings |
| +0x42 | `??_C@_00CNPNBAHC@?$AA@` | 0x0067E2D1 | 0x0122D70E | literal payload prefix |
| +0x4C | `?CheckCRC@CPersistContext@@IAEXPBD@Z` | 0x0067E2DB | 0x00404500 | reloc_bindings |
| +0x63 | `?WriteSLONG@CDataOutputStream@@QAEXJ@Z` | 0x0067E2F2 | 0x00993EB0 | reloc_bindings |
| +0x73 | `?TransferBinaryIn@?$CPersistTraits@W4EEngineGraphicType@@@@QAEXAAVCMemoryDataInputStream@@AAW4EEngineGraphicType@@@Z` | 0x0067E302 | 0x00632470 | reloc_bindings |
| +0x78 | `??_C@_00CNPNBAHC@?$AA@` | 0x0067E307 | 0x0122D70E | literal payload prefix |
| +0x82 | `?CheckCRC@CPersistContext@@IAEXPBD@Z` | 0x0067E311 | 0x00404500 | reloc_bindings |
| +0x9A | `?WriteEBOOL@CDataOutputStream@@QAEX_N@Z` | 0x0067E329 | 0x00993E30 | reloc_bindings |
| +0xAA | `?TransferBinaryIn@?$CPersistTraits@_N@@QAEXAAVCMemoryDataInputStream@@AA_N@Z` | 0x0067E339 | 0x00403EB0 | reloc_bindings |
| +0xAF | `??_C@_00CNPNBAHC@?$AA@` | 0x0067E33E | 0x0122D70E | literal payload prefix |
| +0xB9 | `?CheckCRC@CPersistContext@@IAEXPBD@Z` | 0x0067E348 | 0x00404500 | reloc_bindings |
| +0xCB | `??$GFSerialiseVectorBinaryOut@VCDegradableInfo@@@@YIXAAVCDataOutputStream@@AAV?$vector@VCDegradableInfo@@V?$allocator@VCDegradableInfo@@@std@@@std@@@Z` | 0x0067E35A | 0x0067E720 | Both PDBs and the native same-TU vector writer |
| +0xD5 | `??$GFSerialiseVectorBinaryIn@VCDegradableInfo@@@@YIXAAVCMemoryDataInputStream@@AAV?$vector@VCDegradableInfo@@V?$allocator@VCDegradableInfo@@@std@@@std@@@Z` | 0x0067E364 | 0x0067F430 | Both PDBs and the native same-TU vector reader |
| +0xEB | `??$sort@PAVCDegradableInfo@@VCIsHealth1GTHealth2@CDegradableDef@@@std@@YIXPAVCDegradableInfo@@0VCIsHealth1GTHealth2@CDegradableDef@@@Z` | 0x0067E37A | 0x0067F3D0 | Native introsort then final insertion sort, matching the retail STLport template |

17 supported pairs out of 17. See the separate manual audit where supplied.
