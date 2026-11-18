# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x1B | `?NullChar@CCharString@@1DB` | 0x00A36A9A | 0x0129AAF4 | reloc_bindings |
| +0x2A | `?Calc@CCRC@@SIKKPBXK@Z` | 0x00A36AA9 | 0x004014A0 | reloc_bindings |
| +0x32 | `?FindDummyObjectsBegin@C3DMeshStats@@QBEPBU?$pair@KVC3DDummyObject@@@std@@K@Z` | 0x00A36AB1 | 0x00A2E960 | reloc_bindings |
| +0x46 | `??0C3DDummyObject@@QAE@XZ` | 0x00A36AC8 | 0x00A2E4B0 | reloc_bindings |
| +0x20F | `?GFastOneOverSqrtTable@@3PAKA` | 0x00A36C95 | 0x013CE558 | reloc_bindings |
| +0x240 | `__real@40400000` | 0x00A36CC0 | 0x0122DED4 | IEEE literal payload |
| +0x24A | `__real@3f000000` | 0x00A36CCA | 0x0122F59C | IEEE literal payload |
| +0x29E | `?GFastOneOverSqrtTable@@3PAKA` | 0x00A36D1D | 0x013CE558 | reloc_bindings |
| +0x2C3 | `__real@40400000` | 0x00A36D42 | 0x0122DED4 | IEEE literal payload |
| +0x2CD | `__real@3f000000` | 0x00A36D4B | 0x0122F59C | IEEE literal payload |

10 supported pairs out of 10. See the separate manual audit where supplied.
