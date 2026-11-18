# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0x3 | `?NoStrings@CCharString@@1JA` | 0x0099F7A1 | 0x013BD800 | Shared protected CCharString NoStrings and the retail counter also incremented in the constructor/copy/dtor siblings |
| +0x30 | `??YCCharString@@QAEABV0@D@Z` | 0x0099F7D3 | 0x0099F180 | reloc_bindings |
| +0x3C | `?NoStrings@CCharString@@1JA` | 0x0099F7DE | 0x013BD800 | Shared protected CCharString NoStrings and the retail counter also incremented in the constructor/copy/dtor siblings |
| +0x49 | `?NoStrings@CCharString@@1JA` | 0x0099F7EB | 0x013BD800 | Shared protected CCharString NoStrings and the retail counter also incremented in the constructor/copy/dtor siblings |
| +0x57 | `?UnassignString@CCharString@@IAEXXZ` | 0x0099F7FA | 0x0099E9B0 | reloc_bindings |
| +0x6D | `?UnassignString@CCharString@@IAEXXZ` | 0x0099F810 | 0x0099E9B0 | reloc_bindings |
| +0x73 | `?NoStrings@CCharString@@1JA` | 0x0099F815 | 0x013BD800 | Shared protected CCharString NoStrings and the retail counter also incremented in the constructor/copy/dtor siblings |

7 verified operand bindings. 0 operands still need an unambiguous instruction or binding.
