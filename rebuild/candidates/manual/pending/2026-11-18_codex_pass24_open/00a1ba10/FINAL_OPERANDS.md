# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x2C | `?GetNodeAt@CNavigatorManager@@QBEPBVCANavQuadTreeNode@@ABVC2DVector@@E@Z` | 0x00A1BA3B | 0x00A14F70 | reloc_bindings |
| +0x90 | `?Normalise@C2DVector@@QAEAAV1@XZ` | 0x00A1BA9F | 0x00A14540 | reloc_bindings |
| +0xD1 | `?GetNodeAt@CNavigatorManager@@QBEPBVCANavQuadTreeNode@@ABVC2DVector@@E@Z` | 0x00A1BAE0 | 0x00A14F70 | reloc_bindings |
| +0xE3 | `??8CANavQuadTreeNode@@QBE_NABV0@@Z` | 0x00A1BAF2 | 0x00A763F0 | reloc_bindings |
| +0xF9 | `?ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM@@3VC3DVector@@B` | 0x00A1BB0E | 0x013D27D0 | Native vector field and COFF addend. Refer to the donor initializer evidence. |
| +0x111 | `?ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM@@3VC3DVector@@B` | 0x00A1BB23 | 0x013D27D4 | Native vector field and COFF addend. Refer to the donor initializer evidence. |
| +0x11F | `?ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM@@3VC3DVector@@B` | 0x00A1BB2D | 0x013D27D8 | Native vector field and COFF addend. Refer to the donor initializer evidence. |
| +0x161 | `?GetPosInDirection@CNavigatorAStar@@ABE?AVC3DVector@@ABV2@ABVC2DVector@@M@Z` | 0x00A1BB70 | 0x00A1AD40 | NAME_FIXES_QUEUE |
| +0x16B | `__real@3fe0000000000000` | 0x00A1BB79 | 0x012316F0 | IEEE literal payload |
| +0x178 | `?Rotate@C2DVector@@QAEXM@Z` | 0x00A1BB87 | 0x00A141D0 | reloc_bindings |
| +0x18B | `?GetPosInDirection@CNavigatorAStar@@ABE?AVC3DVector@@ABV2@ABVC2DVector@@M@Z` | 0x00A1BB9A | 0x00A1AD40 | NAME_FIXES_QUEUE |
| +0x19E | `?Rotate@C2DVector@@QAEXM@Z` | 0x00A1BBAD | 0x00A141D0 | reloc_bindings |
| +0x1B1 | `?GetPosInDirection@CNavigatorAStar@@ABE?AVC3DVector@@ABV2@ABVC2DVector@@M@Z` | 0x00A1BBC0 | 0x00A1AD40 | NAME_FIXES_QUEUE |

13 supported pairs out of 13. See the separate manual audit where supplied.
