# Relocation audit

The exact 383-byte leaf has 11 verified REL32 operands.

| Offset | Retail target | Emitted symbol | Verification |
|---|---|---|---|
| +0x05A | 0x0042A34E | `?_Rebalance_for_erase@?$_Rb_global@_N@std@@SAPAU_Rb_tree_node_base@2@PAU32@AAPAU32@11@Z` | Exact package binding. |
| +0x07F | 0x00BFE9BC | `??3@YAXPAX@Z` | Exact package binding. |
| +0x094 | 0x00BFEA14 | `_free` | Exact package binding. |
| +0x0B2 | 0x00BFE9BC | `??3@YAXPAX@Z` | Exact package binding. |
| +0x0DD | 0x00AC8360 | `?IsIllegal@CVertexPairContraction@QuadricOptimiserInternals@@QAE_NXZ` | Both PDBs, native CleanUp call at 0x00AC8360 and bool test at 0x00ACDA3C. |
| +0x0E9 | 0x00ACD900 | `?DetectFoldOver@COptimisedPrimitive@QuadricOptimiserInternals@@AAE_NABVCVertexPairContraction@2@@Z` | Both PDBs, landed neighbor and the 83-byte native vertex-pair overload. |
| +0x102 | 0x00AC8A60 | `?UpdateCost@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXABV?$CCountedPointer@VCVertexPairContraction@QuadricOptimiserInternals@@@@@Z` | Exact package binding. |
| +0x10C | 0x00ACD840 | `?PerformContraction@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXAAVCVertexPairContraction@2@@Z` | Both PDBs and exact name in the landed neighbor source. |
| +0x124 | 0x00AD3C30 | `?erase@?$_Rb_tree@MU?$pair@$$CBMV?$CCountedPointer@VCVertexPairContraction@QuadricOptimiserInternals@@@@@std@@U?$_Select1st@U?$pair@$$CBMV?$CCountedPointer@VCVertexPairContraction@QuadricOptimiserInternals@@@@@std@@@2@U?$less@M@2@V?$allocator@U?$pair@$$CBMV?$CCountedPointer@VCVertexPairContraction@QuadricOptimiserInternals@@@@@std@@@2@@std@@QAEXU?$_Rb_tree_iterator@U?$pair@$$CBMV?$CCountedPointer@VCVertexPairContraction@QuadricOptimiserInternals@@@@@std@@U?$_Nonconst_traits@U?$pair@$$CBMV?$CCountedPointer@VCVertexPairContraction@QuadricOptimiserInternals@@@@@std@@@2@@2@@Z` | Exact package binding. |
| +0x13A | 0x00BFE9BC | `??3@YAXPAX@Z` | Exact package binding. |
| +0x16E | 0x00BFE9BC | `??3@YAXPAX@Z` | Exact package binding. |
