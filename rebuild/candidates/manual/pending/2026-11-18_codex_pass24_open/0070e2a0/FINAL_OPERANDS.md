# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x1D | `?CalcInterpolatedMeshAnim@CAnimateThing@@ABE?AVC3DAnimationBlendState@@ABVCCompleteAnimation@1@ABVCDelayedAnimation@1@M@Z` | 0x0070E2BC | 0x0070D940 | Both PDBs and native same-TU blend calculation |
| +0x74 | `??1CBaseClass@@UAE@XZ` | 0x0070E313 | 0x0099A2E0 | reloc_bindings |
| +0x8F | `?CalcInterpolatedMeshAnim@CAnimateThing@@ABE?AVC3DAnimationBlendState@@ABVCCompleteAnimation@1@ABVCDelayedAnimation@1@M@Z` | 0x0070E32E | 0x0070D940 | Both PDBs and native same-TU blend calculation |
| +0xE6 | `??1CBaseClass@@UAE@XZ` | 0x0070E385 | 0x0099A2E0 | reloc_bindings |
| +0x110 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x0070E3AF | 0x004365B0 | reloc_bindings |
| +0x12E | `?GetPSkeletalMorph@CTCSkeletalMorph@@QBE?BV?$CCountedPointer@VC3DSkeletalMorph@@@@XZ` | 0x0070E3CD | 0x00786870 | Both PDBs and the native morph copy at 0x00786870 |
| +0x157 | `??3@YAXPAX@Z` | 0x0070E3F6 | 0x00BFE9BC | reloc_bindings |
| +0x16F | `?GetPSkeletalMorph@CTCSkeletalMorph@@QBE?BV?$CCountedPointer@VC3DSkeletalMorph@@@@XZ` | 0x0070E40E | 0x00786870 | Both PDBs and the native morph copy at 0x00786870 |
| +0x18F | `?SetSkeletalMorph@C3DAnimationBlendState@@QAEXABV?$CCountedPointer@$$CBVC3DSkeletalMorph@@@@@Z` | 0x0070E42E | 0x00A3C350 | Both PDBs and the landed animation blend method |
| +0x1B1 | `??3@YAXPAX@Z` | 0x0070E450 | 0x00BFE9BC | reloc_bindings |
| +0x1DE | `??3@YAXPAX@Z` | 0x0070E47D | 0x00BFE9BC | reloc_bindings |
| +0x1ED | `?GetPSkeletalMorph@CTCSkeletalMorph@@QBE?BV?$CCountedPointer@VC3DSkeletalMorph@@@@XZ` | 0x0070E48C | 0x00786870 | Both PDBs and the native morph copy at 0x00786870 |
| +0x20D | `?SetSkeletalMorph@C3DAnimationBlendState@@QAEXABV?$CCountedPointer@$$CBVC3DSkeletalMorph@@@@@Z` | 0x0070E4AC | 0x00A3C350 | Both PDBs and the landed animation blend method |
| +0x22F | `??3@YAXPAX@Z` | 0x0070E4CE | 0x00BFE9BC | reloc_bindings |
| +0x25C | `??3@YAXPAX@Z` | 0x0070E4FB | 0x00BFE9BC | reloc_bindings |

15 supported pairs out of 15. See the separate manual audit where supplied.
