# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x37 | `?DiscardAsyncData@CBankFileAsyncData@@SIXAAV?$CCountedPointer@VCBankFileAsyncData@@@@@Z` | 0x00B6DC76 | 0x009D68A0 | reloc_bindings |
| +0x56 | `??3@YAXPAX@Z` | 0x00B6DC95 | 0x00BFE9BC | reloc_bindings |
| +0x86 | `??3@YAXPAX@Z` | 0x00B6DCC5 | 0x00BFE9BC | reloc_bindings |
| +0xA7 | `?Uninitialise@CVertexBufferWin32@@QAEXXZ` | 0x00B6DCE6 | 0x00A63200 | reloc_bindings |
| +0xB5 | `?Uninitialise@CIndexBuffer@@QAEXXZ` | 0x00B6DCF4 | 0x00AC1EE0 | reloc_bindings |
| +0xE4 | `??9CCharString@@QBE_NABV0@@Z` | 0x00B6DD23 | 0x0099E900 | reloc_bindings |
| +0xFE | `?Uninitialise@CVertexBufferWin32@@QAEXXZ` | 0x00B6DD3D | 0x00A63200 | reloc_bindings |
| +0x10C | `?Uninitialise@CIndexBuffer@@QAEXXZ` | 0x00B6DD4B | 0x00AC1EE0 | reloc_bindings |
| +0x127 | `??9CCharString@@QBE_NABV0@@Z` | 0x00B6DD66 | 0x0099E900 | reloc_bindings |
| +0x142 | `?DiscardAsyncData@CBankFileAsyncData@@SIXAAV?$CCountedPointer@VCBankFileAsyncData@@@@@Z` | 0x00B6DD81 | 0x009D68A0 | reloc_bindings |
| +0x161 | `??3@YAXPAX@Z` | 0x00B6DDA0 | 0x00BFE9BC | reloc_bindings |
| +0x191 | `??3@YAXPAX@Z` | 0x00B6DDD0 | 0x00BFE9BC | reloc_bindings |
| +0x1D0 | `??H@YI?AVCCharString@@ABV0@0@Z` | 0x00B6DE0F | 0x0099F570 | reloc_bindings |
| +0x1DC | `?FindIndexBySymbol@CBankFile@@QBEKABVCCharString@@@Z` | 0x00B6DE1B | 0x009CCDC0 | reloc_bindings |
| +0x1E7 | `??1CCharString@@QAE@XZ` | 0x00B6DE26 | 0x0099EAE0 | reloc_bindings |
| +0x1FB | `??4CCharString@@QAEABV0@ABV0@@Z` | 0x00B6DE3A | 0x0099EFB0 | reloc_bindings |
| +0x212 | `?GetAsyncEntry@CBankFileAsync@@QAE?AV?$CCountedPointer@VCBankFileAsyncEntry@@@@K@Z` | 0x00B6DE51 | 0x009D58D0 | reloc_bindings |
| +0x21A | `??4?$CCountedPointer@VCBankFileAsyncEntry@@@@QAEAAV0@ABV0@@Z` | 0x00B6DE59 | 0x00A5F6D0 | reloc_bindings |
| +0x223 | `??1?$CCountedPointer@VCBankFileAsyncEntry@@@@QAE@XZ` | 0x00B6DE62 | 0x009C9FC0 | reloc_bindings |
| +0x232 | `?GetEntryDataSize@CBankFile@@QBEKK@Z` | 0x00B6DE71 | 0x009CC240 | reloc_bindings |
| +0x238 | `?g_HighLoadingPriority@@3W4EThreadedFilePriority@@A` | 0x00B6DE76 | 0x0139A728 | reloc_bindings |
| +0x249 | `?BeginReadDataAutoAlloc@CBankFileAsyncEntry@@QAE?AV?$CCountedPointer@VCBankFileAsyncData@@@@KKW4EThreadedFilePriority@@@Z` | 0x00B6DE88 | 0x009D6100 | reloc_bindings |
| +0x251 | `??4?$CCountedPointer@VCBankFileAsyncData@@@@QAEAAV0@ABV0@@Z` | 0x00B6DE90 | 0x009A4440 | reloc_bindings |
| +0x25A | `??1?$CCountedPointer@VCBankFileAsyncData@@@@QAE@XZ` | 0x00B6DE99 | 0x009A4400 | NAME_FIXES_QUEUE |
| +0x269 | `?IsFinished@CBankFileAsyncData@@QBE_NXZ` | 0x00B6DEA8 | 0x009D54E0 | reloc_bindings |
| +0x278 | `?GetData@CBankFileAsyncData@@QAEPAXXZ` | 0x00B6DEB7 | 0x009D5530 | reloc_bindings |
| +0x282 | `??0CMemoryDataInputStream@@QAE@PBXK@Z` | 0x00B6DEC1 | 0x00994930 | reloc_bindings |
| +0x28E | `?LoadSea@CEngineWaterRenderer@@QAEXAAVCMemoryDataInputStream@@@Z` | 0x00B6DECD | 0x00B6D420 | Binding remains unconfirmed |
| +0x2AD | `??3@YAXPAX@Z` | 0x00B6DEEC | 0x00BFE9BC | reloc_bindings |
| +0x2DD | `??3@YAXPAX@Z` | 0x00B6DF1C | 0x00BFE9BC | reloc_bindings |
| +0x2F5 | `??1CDataStream@@UAE@XZ` | 0x00B6DF34 | 0x00993B10 | reloc_bindings |

30 supported pairs out of 31. See the separate manual audit where supplied.
