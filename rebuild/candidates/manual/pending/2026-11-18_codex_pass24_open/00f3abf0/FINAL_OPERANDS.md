# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x47 | `?CalcCompressionScript@CRangeCompressor@@AAEJAAV?$CArray@UCScriptEntry@CRangeCompressor@@@@PBEJJJ@Z` | 0x00F3AC36 | 0x00F3ABF0 | reloc_bindings |
| +0x8E | `?CalcCompressionScript@CRangeCompressor@@AAEJAAV?$CArray@UCScriptEntry@CRangeCompressor@@@@PBEJJJ@Z` | 0x00F3AC7D | 0x00F3ABF0 | reloc_bindings |
| +0xA3 | `?CalcCompressionScript@CRangeCompressor@@AAEJAAV?$CArray@UCScriptEntry@CRangeCompressor@@@@PBEJJJ@Z` | 0x00F3AC92 | 0x00F3ABF0 | reloc_bindings |
| +0xC1 | `??0?$CArray@K@@QAE@I@Z` | 0x00F3ACB0 | 0x00A96100 | FableWin CArray<unsigned long> constructor identity and retail reserve/zero fill with one argument. A prior catalog labeled the vector base. |
| +0x111 | `?CalcBestCompressionForBlock@CRangeCompressor@@AAEJABV?$CArray@K@@JAAUCScriptEntry@1@@Z` | 0x00F3AD00 | 0x00F3A890 | Binding remains unconfirmed |
| +0x128 | `?push_back@?$CArray@UCScriptEntry@CRangeCompressor@@@@QAEXABUCScriptEntry@CRangeCompressor@@@Z` | 0x00F3AD17 | 0x00F3B420 | Binding remains unconfirmed |
| +0x136 | `_free` | 0x00F3AD25 | 0x00BFEA14 | reloc_bindings |
| +0x172 | `?CalcCompressionScript@CRangeCompressor@@AAEJAAV?$CArray@UCScriptEntry@CRangeCompressor@@@@PBEJJJ@Z` | 0x00F3AD61 | 0x00F3ABF0 | reloc_bindings |
| +0x18C | `?CalcCompressionScript@CRangeCompressor@@AAEJAAV?$CArray@UCScriptEntry@CRangeCompressor@@@@PBEJJJ@Z` | 0x00F3AD7B | 0x00F3ABF0 | reloc_bindings |
| +0x1AC | `?push_back@?$CArray@UCScriptEntry@CRangeCompressor@@@@QAEXABUCScriptEntry@CRangeCompressor@@@Z` | 0x00F3AD9B | 0x00F3B420 | Binding remains unconfirmed |
| +0x1BA | `_free` | 0x00F3ADA9 | 0x00BFEA14 | reloc_bindings |
| +0x1CB | `_free` | 0x00F3ADBA | 0x00BFEA14 | reloc_bindings |
| +0x20A | `?push_back@?$CArray@UCScriptEntry@CRangeCompressor@@@@QAEXABUCScriptEntry@CRangeCompressor@@@Z` | 0x00F3ADF9 | 0x00F3B420 | Binding remains unconfirmed |
| +0x237 | `_free` | 0x00F3AE26 | 0x00BFEA14 | reloc_bindings |
| +0x248 | `_free` | 0x00F3AE37 | 0x00BFEA14 | reloc_bindings |

11 supported pairs out of 15. See the separate manual audit where supplied.
