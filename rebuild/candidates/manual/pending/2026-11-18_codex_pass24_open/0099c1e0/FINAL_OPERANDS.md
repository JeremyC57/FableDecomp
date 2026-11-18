# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x5 | `?NoStrings@CWideString@@1JA` | Unpaired | Unpaired | Instruction sequence differs |
| +0x1C | `??_C@_11LOCGONAA@?$AA?$AA@` | 0x0099C205 | 0x0129A8E0 | Binding remains unconfirmed |
| +0x30 | `??_C@_11LOCGONAA@?$AA?$AA@` | Unpaired | Unpaired | Instruction sequence differs |
| +0x3D | `_wcsstr` | 0x0099C231 | 0x00BFEB2E | reloc_bindings |
| +0x52 | `?NoStrings@CWideString@@1JA` | 0x0099C243 | 0x013BCA20 | reloc_bindings |
| +0x63 | `?NoStrings@CWideString@@1JA` | 0x0099C24E | 0x013BCA20 | reloc_bindings |
| +0x7C | `?AllocStringData@CWideString@@IAEPAVCWideStringData@@PBGJ@Z` | 0x0099C267 | 0x0099B3C0 | reloc_bindings |
| +0x86 | `_wcslen` | 0x0099C273 | 0x00BFED94 | reloc_bindings |
| +0x9D | `?MakeOwnCopyOfData@CWideString@@IAEXXZ` | 0x0099C286 | 0x0099B560 | Binding remains unconfirmed |
| +0xA8 | `??_C@_11LOCGONAA@?$AA?$AA@` | Unpaired | Unpaired | Instruction sequence differs |
| +0xCE | `??$append@PBG@?$basic_string@GV?$char_traits@G@std@@V?$allocator@G@2@@std@@QAEAAV01@PBG0ABUforward_iterator_tag@1@@Z` | 0x0099C2B8 | 0x0099C7E0 | NAME_FIXES_QUEUE |
| +0x108 | `_free` | 0x0099C3C0 | 0x00BFEA14 | reloc_bindings |
| +0x111 | `??3@YAXPAX@Z` | 0x0099C3C9 | 0x00BFE9BC | reloc_bindings |
| +0x11A | `?NoStrings@CWideString@@1JA` | 0x0099C3D1 | 0x013BCA20 | reloc_bindings |
| +0x127 | `?NoStrings@CWideString@@1JA` | 0x0099C3E2 | 0x013BCA20 | reloc_bindings |
| +0x132 | `?MakeOwnCopyOfData@CWideString@@IAEXXZ` | Unpaired | Unpaired | Instruction sequence differs |
| +0x13F | `??_C@_11LOCGONAA@?$AA?$AA@` | Unpaired | Unpaired | Instruction sequence differs |
| +0x167 | `??$append@PBG@?$basic_string@GV?$char_traits@G@std@@V?$allocator@G@2@@std@@QAEAAV01@PBG0ABUforward_iterator_tag@1@@Z` | Unpaired | Unpaired | Instruction sequence differs |
| +0x187 | `_wcsstr` | 0x0099C3F6 | 0x0099B4D0 | Binding remains unconfirmed |
| +0x19E | `??0CWideString@@QAE@PBG@Z` | 0x0099C38C | 0x0099B6B0 | reloc_bindings (nearby instruction) |
| +0x1A8 | `??YCWideString@@QAEABV0@ABV0@@Z` | 0x0099C396 | 0x0099B8D0 | reloc_bindings (nearby instruction) |
| +0x1D1 | `_free` | 0x0099C3C0 | 0x00BFEA14 | reloc_bindings (nearby instruction) |
| +0x1DA | `??3@YAXPAX@Z` | 0x0099C3C9 | 0x00BFE9BC | reloc_bindings (nearby instruction) |
| +0x1E3 | `?NoStrings@CWideString@@1JA` | Unpaired | Unpaired | Instruction sequence differs |
| +0x1F8 | `?NoStrings@CWideString@@1JA` | Unpaired | Unpaired | Instruction sequence differs |
| +0x20B | `?UnassignString@CWideString@@IAEXXZ` | 0x0099C3F6 | 0x0099B4D0 | reloc_bindings (nearby instruction) |
| +0x241 | `_free` | 0x0099C41C | 0x00BFEA14 | reloc_bindings |
| +0x24A | `??3@YAXPAX@Z` | 0x0099C425 | 0x00BFE9BC | reloc_bindings |
| +0x252 | `?NoStrings@CWideString@@1JA` | 0x0099C42D | 0x013BCA20 | reloc_bindings |
| +0x25A | `?NoStrings@CWideString@@1JA` | 0x0099C435 | 0x013BCA20 | reloc_bindings (nearby instruction) |

19 supported pairs out of 30. See the separate manual audit where supplied.
