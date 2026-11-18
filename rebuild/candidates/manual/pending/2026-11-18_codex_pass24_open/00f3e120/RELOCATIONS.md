# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0x1CE | `?BuildRings@CTriangleStripifier2@@AAEXJABV?$CArray@G@@ABV?$CArray@V?$CArray@J@@@@ABV?$CArray@V?$CArray@G@@@@AAV2@3AAV4@@Z` | 0x00F3E2ED | 0x00F3D980 | Both PDBs and Ego_d TU order plus the native ring-building calls |
| +0x1FA | `?BuildCacheOptimizedVertexList@CTriangleStripifier2@@AAEXABV?$CArray@G@@ABV?$CArray@V?$CArray@J@@@@ABV?$CArray@V?$CArray@G@@@@02AAV2@@Z` | 0x00F3E319 | 0x00F3BA70 | Both PDBs and Ego_d TU plus the native section selection calls |
| +0x208 | `?CalcSectionScore@CTriangleStripifier2@@AAEMABV?$CArray@G@@0ABV?$CArray@V?$CArray@J@@@@@Z` | 0x00F3E327 | 0x00F3D830 | Both PDBs and the native score return and call protocol |
| +0x229 | `__real@7149f2ca` | 0x00F3E347 | 0x01301CF4 | IEEE literal payload |
| +0x248 | `_free` | 0x00F3E367 | 0x00BFEA14 | reloc_bindings |
| +0x268 | `_free` | 0x00F3E387 | 0x00BFEA14 | reloc_bindings |
| +0x27C | `_free` | 0x00F3E39B | 0x00BFEA14 | reloc_bindings |
| +0x28D | `_free` | 0x00F3E3AC | 0x00BFEA14 | reloc_bindings |
| +0x29E | `_free` | 0x00F3E3BD | 0x00BFEA14 | reloc_bindings |
| +0x2D5 | `_free` | 0x00F3E3F4 | 0x00BFEA14 | reloc_bindings |
| +0x2F0 | `_free` | 0x00F3E40F | 0x00BFEA14 | reloc_bindings |
| +0x30C | `_free` | 0x00F3E42B | 0x00BFEA14 | reloc_bindings |
| +0x31D | `_free` | 0x00F3E43C | 0x00BFEA14 | reloc_bindings |
| +0x32E | `_free` | 0x00F3E44D | 0x00BFEA14 | reloc_bindings |
| +0x337 | `__real@00000000` | 0x00F3E455 | 0x0122DEDC | IEEE literal payload |
| +0x3EA | `?BuildRings@CTriangleStripifier2@@AAEXJABV?$CArray@G@@ABV?$CArray@V?$CArray@J@@@@ABV?$CArray@V?$CArray@G@@@@AAV2@3AAV4@@Z` | 0x00F3E509 | 0x00F3D980 | Both PDBs and Ego_d TU order plus the native ring-building calls |
| +0x40A | `?BuildCacheOptimizedVertexList@CTriangleStripifier2@@AAEXABV?$CArray@G@@ABV?$CArray@V?$CArray@J@@@@ABV?$CArray@V?$CArray@G@@@@02AAV2@@Z` | 0x00F3E529 | 0x00F3BA70 | Both PDBs and Ego_d TU plus the native section selection calls |
| +0x418 | `?CalcSectionScore@CTriangleStripifier2@@AAEMABV?$CArray@G@@0ABV?$CArray@V?$CArray@J@@@@@Z` | 0x00F3E537 | 0x00F3D830 | Both PDBs and the native score return and call protocol |
| +0x439 | `__real@7149f2ca` | 0x00F3E557 | 0x01301CF4 | IEEE literal payload |
| +0x458 | `_free` | 0x00F3E577 | 0x00BFEA14 | reloc_bindings |
| +0x478 | `_free` | 0x00F3E597 | 0x00BFEA14 | reloc_bindings |
| +0x48C | `_free` | 0x00F3E5AB | 0x00BFEA14 | reloc_bindings |
| +0x49D | `_free` | 0x00F3E5BC | 0x00BFEA14 | reloc_bindings |
| +0x4AE | `_free` | 0x00F3E5CD | 0x00BFEA14 | reloc_bindings |
| +0x4E1 | `_free` | 0x00F3E600 | 0x00BFEA14 | reloc_bindings |
| +0x4FC | `_free` | 0x00F3E61B | 0x00BFEA14 | reloc_bindings |
| +0x518 | `_free` | 0x00F3E637 | 0x00BFEA14 | reloc_bindings |
| +0x529 | `_free` | 0x00F3E648 | 0x00BFEA14 | reloc_bindings |
| +0x53A | `_free` | 0x00F3E659 | 0x00BFEA14 | reloc_bindings |
| +0x543 | `__real@00000000` | 0x00F3E661 | 0x0122DEDC | IEEE literal payload |

30 verified operand bindings. 0 operands still need an unambiguous instruction or binding.
