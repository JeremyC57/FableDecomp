# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x1F | `__real@00000000` | 0x00A571BD | 0x0122DEDC | IEEE literal payload |
| +0x60 | `__real@00000000` | 0x00A571FE | 0x0122DEDC | IEEE literal payload |
| +0x6C | `__real@3f800000` | 0x00A5720A | 0x0122DED8 | IEEE literal payload |
| +0xB2 | `__real@00000000` | 0x00A57252 | 0x0122DEDC | IEEE literal payload |
| +0xE9 | `__real@3f000000` | 0x00A57289 | 0x0122F59C | IEEE literal payload |
| +0xF9 | `__real@3f000000` | 0x00A57299 | 0x0122F59C | IEEE literal payload |
| +0x117 | `?GetPointOnInfiniteLine@C2DLineF@@QBE?AVC2DVector@@ABV2@@Z` | 0x00A572B8 | 0x00A57140 | Binding remains unconfirmed |
| +0x281 | `__real@00000000` | 0x00A57411 | 0x0122DEDC | IEEE literal payload (nearby instruction) |

7 supported pairs out of 8. See the separate manual audit where supplied.
