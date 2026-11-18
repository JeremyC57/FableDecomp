# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0x43 | `?GetAttackerProng@CCombatRing@CDefendingCombatantInfo@@QBEPBVCProng@2@ABVCThingCreatureBase@@@Z` | 0x006E89F2 | 0x006E87D0 | NAME_FIXES_QUEUE |
| +0x56 | `?GetProngVector@CProng@CDefendingCombatantInfo@@QBE?AVC3DVector@@M@Z` | 0x006E8A05 | 0x006E7FD0 | NAME_FIXES_QUEUE |
| +0x7B | `?GetCombatTypeDef@CThingCreatureBase@@QBEABVCCombatTypeDef@@XZ` | 0x006E8A2A | 0x00666D10 | reloc_bindings |
| +0x96 | `?GetCombatTypeDef@CThingCreatureBase@@QBEABVCCombatTypeDef@@XZ` | 0x006E8A45 | 0x00666D10 | reloc_bindings |
| +0xA4 | `__real@38d1b717` | 0x006E8A52 | 0x0129BA3C | IEEE literal payload |
| +0xB9 | `__real@3f000000` | 0x006E8A67 | 0x0122F59C | IEEE literal payload |
| +0xEF | `__real@00000000` | 0x006E8A9D | 0x0122DEDC | IEEE literal payload |
| +0xFE | `?GetRingRadius@CCombatWheel@CDefendingCombatantInfo@@QBEMJ@Z` | 0x006E8AAD | 0x006E8850 | NAME_FIXES_QUEUE |
| +0x106 | `?GetRingRadius@CCombatWheel@CDefendingCombatantInfo@@QBEMJ@Z` | 0x006E8AB5 | 0x006E8850 | NAME_FIXES_QUEUE |
| +0x130 | `__real@3f000000` | 0x006E8ADE | 0x0122F59C | IEEE literal payload |
| +0x13E | `__real@3f000000` | 0x006E8AEC | 0x0122F59C | IEEE literal payload |
| +0x14B | `__real@3f000000` | 0x006E8AF9 | 0x0122F59C | IEEE literal payload |

12 verified operand bindings. 0 operands still need an unambiguous instruction or binding.
