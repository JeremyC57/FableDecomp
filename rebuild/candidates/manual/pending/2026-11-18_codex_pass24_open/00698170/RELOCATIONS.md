# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0xA | `?GetFOV@CCamera@@QBEMXZ` | 0x00698179 | 0x00A0BEB0 | reloc_bindings |
| +0x6E | `?DELTA_NOTIONAL_ZERO@@3MB` | 0x006981DC | 0x0129BA3C | reloc_bindings |
| +0x9D | `?DELTA_NOTIONAL_ZERO@@3MB` | 0x0069820B | 0x0129BA3C | reloc_bindings |
| +0xD6 | `?AccurateNormalise@C3DVector@@QAEAAV1@XZ` | 0x00698245 | 0x00A14440 | reloc_bindings |
| +0x121 | `?AccurateNormalise@C3DVector@@QAEAAV1@XZ` | 0x00698290 | 0x00A14440 | reloc_bindings |
| +0x179 | `?IsPosInViewingFrustrum@CTools@NCameraModes@@QBE_NABVC3DVector@@00M@Z` | 0x006982E8 | 0x00875120 | pass 24 naming evidence |
| +0x1D0 | `?IsPosInViewingFrustrum@CTools@NCameraModes@@QBE_NABVC3DVector@@00M@Z` | 0x0069833F | 0x00875120 | pass 24 naming evidence |
| +0x227 | `?IsPosInViewingFrustrum@CTools@NCameraModes@@QBE_NABVC3DVector@@00M@Z` | 0x00698396 | 0x00875120 | pass 24 naming evidence |
| +0x27E | `?IsPosInViewingFrustrum@CTools@NCameraModes@@QBE_NABVC3DVector@@00M@Z` | 0x006983ED | 0x00875120 | pass 24 naming evidence |

9 verified operand bindings. 0 operands still need an unambiguous instruction or binding.
