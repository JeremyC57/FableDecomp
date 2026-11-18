# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0x16 | `??_C@_06GAKCLA@6x86MX?$AA@` | 0x00C52C55 | 0x012B8920 | literal payload prefix |
| +0x21 | `??_C@_09FLPPDEGD@Cyrix?5III?$AA@` | 0x00C52C60 | 0x012B8914 | literal payload prefix |
| +0x2F | `??_C@_04BKFDOIOK@6x86?$AA@` | 0x00C52C6E | 0x012B890C | literal payload prefix |
| +0x3A | `??_C@_03GKIECKIF@GXm?$AA@` | 0x00C52C79 | 0x012B8908 | literal payload prefix |
| +0x40 | `??_C@_09EEKGDCPH@?$DMunknown?$DO?$AA@` | 0x00C52C7F | 0x012B8830 | literal payload prefix |
| +0x46 | `??_C@_07HCKBBAOH@MediaGX?$AA@` | 0x00C52C85 | 0x012B8900 | literal payload prefix |

6 verified operand bindings. 0 operands still need an unambiguous instruction or binding.
