# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x29 | `?DELTA_NOTIONAL_ZERO@@3MB` | 0x00A18FE7 | 0x0129BA3C | reloc_bindings |
| +0x3E | `?DELTA_NOTIONAL_ZERO@@3MB` | 0x00A18FFC | 0x0129BA3C | reloc_bindings |
| +0x53 | `?DELTA_NOTIONAL_ZERO@@3MB` | 0x00A19011 | 0x0129BA3C | reloc_bindings |
| +0x92 | `?GetNodeAt@CNavigatorManager@@QBEPBVCANavQuadTreeNode@@ABVC2DVector@@E@Z` | 0x00A19051 | 0x00A14F70 | reloc_bindings |
| +0xA9 | `?DELTA_NOTIONAL_ZERO@@3MB` | 0x00A19067 | 0x0129BA3C | reloc_bindings |
| +0xE4 | `?IsInSameMap@CNavigatorManager@@QBE_NABVC2DVector@@0@Z` | 0x00A190A3 | 0x00A14FD0 | reloc_bindings |
| +0x192 | `?DELTA_NOTIONAL_ZERO@@3MB` | 0x00A19150 | 0x0129BA3C | reloc_bindings |
| +0x1EE | `?GFastOneOverSqrtTable@@3PAKA` | 0x00A191AB | 0x013CE558 | reloc_bindings |
| +0x212 | `__real@40400000` | 0x00A191D0 | 0x0122DED4 | IEEE literal payload |
| +0x21C | `__real@3f000000` | 0x00A191DA | 0x0122F59C | IEEE literal payload |
| +0x28C | `?ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM@@3VC3DVector@@A` | 0x00A19258 | 0x01397134 | Binding remains unconfirmed |
| +0x29A | `?ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM@@3VC3DVector@@A` | 0x00A1924A | 0x01397130 | reloc_bindings (nearby instruction) |
| +0x2A8 | `?ADDITIONAL_HEIGHT_TO_CHECK_Z_FROM@@3VC3DVector@@A` | 0x00A1924A | 0x01397130 | reloc_bindings (nearby instruction) |
| +0x2FB | `?MIN_HEIGHT_TO_KEEP_ABOVE_GROUND_IN_FLIGHT@@3MB` | 0x00A192B7 | 0x0129CB48 | Binding remains unconfirmed |
| +0x30C | `?MIN_HEIGHT_TO_KEEP_ABOVE_GROUND_IN_FLIGHT@@3MB` | 0x00A192C8 | 0x0129CB48 | Binding remains unconfirmed |
| +0x38D | `?GFastOneOverSqrtTable@@3PAKA` | 0x00A19348 | 0x013CE558 | reloc_bindings |
| +0x3B1 | `__real@40400000` | 0x00A1936D | 0x0122DED4 | IEEE literal payload |
| +0x3BB | `__real@3f000000` | 0x00A19377 | 0x0122F59C | IEEE literal payload |
| +0x40A | `?MIN_HEIGHT_TO_KEEP_ABOVE_GROUND_IN_FLIGHT@@3MB` | 0x00A193C6 | 0x0129CB48 | Binding remains unconfirmed |

15 supported pairs out of 19. See the separate manual audit where supplied.
