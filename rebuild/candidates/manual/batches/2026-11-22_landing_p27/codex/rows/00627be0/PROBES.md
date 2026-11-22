# 0x00627BE0 source probes

| Source | Bytes | Instruction groups | Observation |
| --- | --- | --- | --- |
| baseline | Compile or extraction failed | — | Packaged selected file. |
| geometry-actual-00627be0 | 1048 / 1056 | 40 | Compile and compare the actual prior function source on shared include tree. |

The measured source is still incomplete.

| Shared-source attempt | Bytes | Observation |
| --- | --- | --- |
| geometry-quickexpressions-shared through -shared3 | Compiler rejection | Resolve STLport map visibility, inventory enum-scope opt-in and shared inventory static interface type. |
| geometry-quickexpressions-shared4 | 1148 / 1056 | Actual inventory, expression, CManager, CList and CDataBank types compile. The default vector append body emits two mismatched mapped COMDATs. |
| geometry-quickexpressions-unsigned-cond | 1148 / 1056 | The original signed count promotes to unsigned when compared to size, as native requires. |
| geometry-quickexpressions-push-decl and -push-noinline | 1076 / 1056 | Calling the whole vector append COMDAT brings the native common switch tail back. |
| geometry-quickexpressions-getdef-body | 1060 / 1056, 264 / 335 instructions | Visible real definition lookup bodies recover lifetime slots, but both size-optimized owners differ. |
| geometry-quickexpressions-r2-both-defs and -r2-direct-deref | 1044 / 1056, 265 / 335 instructions | Shared GetSubDef scope restores one exact mapped helper, with two GetDef copies still differing. |
| geometry-quickexpressions-r2-subdef-only | 1068 / 1056 | All four mapped copies equal retail. The two external definition calls keep additional reloads. |
| geometry-quickexpressions-r2-release-countref and -r2-def-ref | 1068 and 1044 / 1056 | Alternative counted releases change copy and leaf lifetimes without reaching the retail schedule. |
| geometry-quickexpressions-r2-subdef-scope and -r2-subdef-deep | 1080 and 1044 / 1056 | Narrow construction scope and nested checks leave different paths and frame slots. |

The geometry-quickexpressions-partial-tested build retains the four matching mapped copies and passes the synchronized fixture. Its remaining instruction groups cover extra definition reloads, register choices and counted-pointer cleanup.

The geometry-quickexpressions-native-defs and native-shapes families compare the original definition-lookup body in speed and size scopes, the original debug check, and the checked-pointer and component-query alternatives. The retained geometry-quickexpressions-good-owners build has 265 of 335 matching normalized instructions and all six matching weak owners. Pointer declaration, null initialization, early-return, and cast variants leave the baseline register choices unchanged or introduce larger differences.

The same-TU visibility probes add the real category builder, the native definition-manager accessor, and the inventory category loop, independently and in combination. The original typed manager and inventory bodies add matching native owners while leaving the 1044-byte leaf unchanged. Alternate STLport iterator wrappers and allocator lifetime forms preserve the same register choices.

Both exact GetDef instantiations retain their 98-byte copies when declared before the leaf and defined after it, when their definition order is reversed, and when the index is const-qualified or copied. These variants preserve the same leaf grade.

The FableWin body loads the final result from the named `CList* pexpressions_list` at frame offset -0x54, rather than the `CComponent* pexpressions_list_component` at -0x58. Returning the actual typed local gives a 1048-byte leaf, 266 of 335 matched instructions and all six mapped copies agreeing. The synchronized fixture passes as 1ea95976e3bc.

The authentic CManager::GetInstance singleton body can emit its exact 41-byte size-optimized owner independently. Keeping it out of line with the corrected CList return leaves the leaf at 1048 bytes and 266 matched instructions. The singleton lives in the separate ui_manager object and is preserved as diagnostic evidence.

Extracting the sibling from the exact Activate object's own source produces the same raw 1048-byte code section as the selected standalone leaf. The combined object retains the 1315-byte Activate match across the original inventory GetTC spelling, alternate scoped vendor vector append definitions, the donor's exact named scalar declaration set, and the original wide and narrow string bodies. The 71-byte CManager factory and the 15-, 47-, 61-, and 12-byte string owner bodies match independently. They leave the sibling at 266 of 335 instructions.

The FableWin C13 line subsection for code at 0x021185E0 confirms the loop on line 232 and the typed result at line 304. The line-232 machine code uses the known VC6 for-scope compatibility branch. Restoring it in the standalone and paired sources keeps the leaf at 1048 bytes, with the same agreement and exact Activate. The full original inventory chain at 0x005BF03C, 0x005BF2C6, and 0x005BF482 also emits its exact 36-, 39-, and 33-byte bodies without moving either member. Reconstructing the actual category and list builder in the same object does not affect the leaf's frame allocation.

Main replayed the complete original vector append and growth paths with the two genuine category consumers visible together. The 18 visibility probes and 20 destructor probes all retained 1048/1056 and 266/335. In particular, removing the explicit append specialization from the combined diagnostic did not fix the two loop-carried begin pointers. The FableWin source-line records confirm that all four category calls use the same typed CList local at -0x54, and the final return also loads that local. The Ego_r optimized donor assigns the typed result and expression sub-definition to a common stack slot. The current VC7.1 allocator trace instead coalesces the hot component pointer and the result into ESI.

Both assigned leaves now coexist byte-exact in one genuine source TU: List 1056/1056 and Activate 1315/1315. All 57 List operands have supporting evidence. Use rows/00627be0/paired_exact_review/PAIRED_SOURCE.cpp with the cached paired header overlay. The source is still under review because emitted category and vector helpers differ. The full factorial ablations are recorded in runtime/quick_reduce_chain.log and runtime/quick_pair_specific.log. The chain must contain both actual category bodies and both vector bodies at once. Earlier probes retaining the generic overflow declaration inadvertently omitted that final link.

Accepted the complete shared TU after verifying both reserved functions, two native category builders, the real direct list-base destructor target and all other mapped copies. The two previously partial row views and fixture histories are retained locally. The duplicated two-object link is superseded by the original single-object ownership, recorded in STRONG_COPIES.json.
