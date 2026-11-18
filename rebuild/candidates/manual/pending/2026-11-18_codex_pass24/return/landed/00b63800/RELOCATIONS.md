# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0x4 | `?GPEngineWorldMap@NEngineInternal@@3PAVCEngineWorldMap@@A` | 0x00B63803 | 0x01436E8C | reloc_bindings |
| +0x76 | `?_M_insert_overflow@?$vector@KV?$allocator@K@std@@@std@@IAEXPAKABKABU__true_type@2@I_N@Z` | 0x00B63875 | 0x00464ACE | reloc_bindings |
| +0xB4 | `?_M_insert_overflow@?$vector@KV?$allocator@K@std@@@std@@IAEXPAKABKABU__true_type@2@I_N@Z` | 0x00B638B3 | 0x00464ACE | reloc_bindings |
| +0xF2 | `?_M_insert_overflow@?$vector@KV?$allocator@K@std@@@std@@IAEXPAKABKABU__true_type@2@I_N@Z` | 0x00B638F1 | 0x00464ACE | reloc_bindings |
| +0x13C | `?_M_insert_overflow@?$vector@KV?$allocator@K@std@@@std@@IAEXPAKABKABU__true_type@2@I_N@Z` | 0x00B6393B | 0x00464ACE | reloc_bindings |
| +0x17A | `?_M_insert_overflow@?$vector@KV?$allocator@K@std@@@std@@IAEXPAKABKABU__true_type@2@I_N@Z` | 0x00B63979 | 0x00464ACE | reloc_bindings |
| +0x1B8 | `?_M_insert_overflow@?$vector@KV?$allocator@K@std@@@std@@IAEXPAKABKABU__true_type@2@I_N@Z` | 0x00B639B7 | 0x00464ACE | reloc_bindings |
| +0x22A | `?_M_insert_overflow@?$vector@KV?$allocator@K@std@@@std@@IAEXPAKABKABU__true_type@2@I_N@Z` | 0x00B63A29 | 0x00464ACE | reloc_bindings |
| +0x26C | `?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A` | 0x00B63A6B | 0x01436E98 | reloc_bindings |
| +0x31B | `?ListRemove@CResource@@AAEXXZ` | 0x00B63B1A | 0x009FBFB0 | reloc_bindings |
| +0x32D | `?ListInsert@CResource@@AAEXPAV1@0@Z` | 0x00B63B2C | 0x009FBF90 | reloc_bindings |
| +0x34F | `?ListRemove@CResource@@AAEXXZ` | 0x00B63B4E | 0x009FBFB0 | reloc_bindings |
| +0x361 | `?ListInsert@CResource@@AAEXPAV1@0@Z` | 0x00B63B60 | 0x009FBF90 | reloc_bindings |
| +0x389 | `?BeginLoading@CFrameData@CGraphicInfoFrame@@QAEXKK@Z` | 0x00B63B88 | 0x009FDE80 | reloc_bindings |
| +0x392 | `??2@YAPAXI@Z` | 0x00B63B91 | 0x00BFEA1A | reloc_bindings |
| +0x3A1 | `??0CFrameData@CGraphicInfoFrame@@QAE@PBV1@@Z` | 0x00B63BA0 | 0x009FE380 | reloc_bindings |
| +0x3C7 | `?BeginLoading@CFrameData@CGraphicInfoFrame@@QAEXKK@Z` | 0x00B63BC6 | 0x009FDE80 | reloc_bindings |
| +0x3EC | `_free` | 0x00B63BEB | 0x00BFEA14 | reloc_bindings |

18 verified operand bindings. 0 operands still need an unambiguous instruction or binding.
