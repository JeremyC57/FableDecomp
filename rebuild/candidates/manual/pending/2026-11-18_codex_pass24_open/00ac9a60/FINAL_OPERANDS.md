# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x13 | `?erase@?$vector@VCOptimisedEdge@QuadricOptimiserInternals@@V?$allocator@VCOptimisedEdge@QuadricOptimiserInternals@@@std@@@std@@QAEPAVCOptimisedEdge@QuadricOptimiserInternals@@PAV34@0@Z` | 0x00AC9A72 | 0x00AD3CC0 | Binding remains unconfirmed |
| +0x8A | `??0?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@QAE@ABV01@@Z` | 0x00AC9AE9 | 0x00AD3A10 | NAME_FIXES_QUEUE |
| +0x9E | `??0?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@QAE@ABV01@@Z` | 0x00AC9AFD | 0x00AD3A10 | NAME_FIXES_QUEUE |
| +0xB1 | `?GetIntersection@CIndexList@QuadricOptimiserInternals@@QBE?AV12@ABV12@@Z` | 0x00AC9B10 | 0x00AC8960 | Binding remains unconfirmed |
| +0xDF | `?GetFaceNormal@COptimisedPrimitive@QuadricOptimiserInternals@@ABE?AVC3DVector@@ABVCOptimisedFace@2@@Z` | 0x00AC9B51 | 0x00AC7CB0 | reloc_bindings |
| +0xF2 | `?GetFaceNormal@COptimisedPrimitive@QuadricOptimiserInternals@@ABE?AVC3DVector@@ABVCOptimisedFace@2@@Z` | 0x00AC9B51 | 0x00AC7CB0 | reloc_bindings (nearby instruction) |
| +0x10C | `__real@bf7d70a4` | 0x00AC9B6A | 0x0129F054 | IEEE literal payload |
| +0x120 | `?JoinAdjacentFaces@COptimisedPrimitive@QuadricOptimiserInternals@@AAE_NKK@Z` | 0x00AC9B7F | 0x00AC8AE0 | Binding remains unconfirmed |
| +0x189 | `?_M_erase@?$_Rb_tree@KKU?$_Identity@K@std@@U?$less@K@2@V?$allocator@K@2@@std@@AAEXPAU?$_Rb_tree_node@K@2@@Z` | 0x00AC9BE8 | 0x004CF810 | reloc_bindings |
| +0x192 | `_free` | 0x00AC9BF1 | 0x00BFEA14 | reloc_bindings |
| +0x1C2 | `_free` | 0x00AC9C23 | 0x00BFEA14 | reloc_bindings |
| +0x1E9 | `?_M_erase@?$_Rb_tree@KKU?$_Identity@K@std@@U?$less@K@2@V?$allocator@K@2@@std@@AAEXPAU?$_Rb_tree_node@K@2@@Z` | 0x00AC9C48 | 0x004CF810 | reloc_bindings |
| +0x1F2 | `_free` | 0x00AC9C51 | 0x00BFEA14 | reloc_bindings |
| +0x222 | `_free` | 0x00AC9C83 | 0x00BFEA14 | reloc_bindings |
| +0x249 | `?_M_erase@?$_Rb_tree@KKU?$_Identity@K@std@@U?$less@K@2@V?$allocator@K@2@@std@@AAEXPAU?$_Rb_tree_node@K@2@@Z` | 0x00AC9CA8 | 0x004CF810 | reloc_bindings |
| +0x252 | `_free` | 0x00AC9CB1 | 0x00BFEA14 | reloc_bindings |
| +0x282 | `_free` | 0x00AC9CE3 | 0x00BFEA14 | reloc_bindings |

14 supported pairs out of 17. See the separate manual audit where supplied.
