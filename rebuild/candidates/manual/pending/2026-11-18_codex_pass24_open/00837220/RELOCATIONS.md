# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0x2B | `?PeekPos@CThingPhysical@@QBEABVC3DVector@@XZ` | 0x0083724A | 0x006A4D00 | reloc_bindings |
| +0x3E | `?PeekPos@CThingPhysical@@QBEABVC3DVector@@XZ` | 0x0083725D | 0x006A4D00 | reloc_bindings |
| +0x47 | `?PeekPos@CThingPhysical@@QBEABVC3DVector@@XZ` | 0x00837266 | 0x006A4D00 | reloc_bindings |
| +0x9D | `?DELTA_NOTIONAL_ZERO@@3MB` | 0x008372BB | 0x0129BA3C | reloc_bindings |
| +0xA3 | `?DELTA_NOTIONAL_ZERO@@3MB` | 0x008372C1 | 0x0129BA3C | reloc_bindings |
| +0xC0 | `?Rotate@C3DVector@@QAEXMM@Z` | 0x008372DF | 0x00A14260 | reloc_bindings |
| +0xFB | `?GFastOneOverSqrtTable@@3PAKA` | 0x00837318 | 0x013CE558 | reloc_bindings |
| +0x12C | `__real@40400000` | 0x0083734A | 0x0122DED4 | IEEE literal payload |
| +0x136 | `__real@3f000000` | 0x00837354 | 0x0122F59C | IEEE literal payload |
| +0x188 | `?GFastOneOverSqrtTable@@3PAKA` | 0x008373A5 | 0x013CE558 | reloc_bindings |
| +0x1AC | `__real@40400000` | 0x008373CA | 0x0122DED4 | IEEE literal payload |
| +0x1B6 | `__real@3f000000` | 0x008373D4 | 0x0122F59C | IEEE literal payload |
| +0x1CE | `__real@00000000` | 0x008373F1 | 0x0122DEDC | IEEE literal payload (nearby instruction) |
| +0x241 | `?GFastOneOverSqrtTable@@3PAKA` | 0x0083745E | 0x013CE558 | reloc_bindings |
| +0x270 | `__real@40400000` | 0x0083748E | 0x0122DED4 | IEEE literal payload |
| +0x27A | `__real@3f000000` | 0x00837498 | 0x0122F59C | IEEE literal payload |

16 verified operand bindings. 0 operands still need an unambiguous instruction or binding.
