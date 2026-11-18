# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0xA | `?MakeOwnCopyOfData@C3DAnimationBlendState@@AAEXXZ` | 0x00A3C3B9 | 0x00A3C1A0 | reloc_bindings |
| +0x30 | `?CreateInterpolate@C3DAnimationBlendState@@CI?AV?$CIVCountedPointer@VC3DAnimationStateBase@@@@ABVC3DAnimationStateBase@@0M@Z` | 0x00A3C3DF | 0x00A3BF10 | reloc_bindings |
| +0xC4 | `??_7C3DAnimationStateAlpha@@6B@` | 0x00A3C470 | 0x01265128 | reloc_bindings |
| +0xD1 | `?resize@?$vector@VC3DAnimationStateAlpha@@V?$allocator@VC3DAnimationStateAlpha@@@std@@@std@@QAEXIABVC3DAnimationStateAlpha@@@Z` | 0x00A3C480 | 0x00A3D500 | reloc_bindings |
| +0x2ED | `??3@YAXPAX@Z` | 0x00A3C69C | 0x00BFE9BC | reloc_bindings |

5 supported pairs out of 5. See the separate manual audit where supplied.
