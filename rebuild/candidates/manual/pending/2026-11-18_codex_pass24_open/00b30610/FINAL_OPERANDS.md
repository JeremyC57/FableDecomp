# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x14 | `__real@bf800000` | 0x00B30622 | 0x0122DEE0 | IEEE literal payload |
| +0x24 | `__real@bf800000` | 0x00B30632 | 0x0122DEE0 | IEEE literal payload |
| +0x44 | `__real@bf800000` | 0x00B30653 | 0x0122DEE0 | IEEE literal payload |
| +0x67 | `__real@bf800000` | 0x00B30676 | 0x0122DEE0 | IEEE literal payload |
| +0x70 | `__real@bf800000` | 0x00B3067F | 0x0122DEE0 | IEEE literal payload (nearby instruction) |
| +0x7F | `__real@bf800000` | 0x00B3067F | 0x0122DEE0 | IEEE literal payload |
| +0x95 | `__real@bf800000` | 0x00B306A6 | 0x0122DEE0 | IEEE literal payload |
| +0xA2 | `__real@bf800000` | 0x00B306B3 | 0x0122DEE0 | IEEE literal payload (nearby instruction) |
| +0xB3 | `__real@bf800000` | 0x00B306B3 | 0x0122DEE0 | IEEE literal payload |
| +0x26F | `__real@00000000` | 0x00B3089D | 0x0122DEDC | IEEE literal payload |

10 supported pairs out of 10. See the separate manual audit where supplied.
