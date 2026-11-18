# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x1 | `?g_flipMeshCollideNormals_013b86ec@@3_NA` | 0x006AF7D0 | 0x013B86EC | reloc_bindings |
| +0xA2 | `?AccurateNormalise@C3DVector@@QAEAAV1@XZ` | 0x006AF871 | 0x00A14440 | reloc_bindings |
| +0xEB | `?AccurateNormalise@C3DVector@@QAEAAV1@XZ` | 0x006AF8BA | 0x00A14440 | reloc_bindings |
| +0x144 | `__real@00000000` | 0x006AF912 | 0x0122DEDC | IEEE literal payload |
| +0x153 | `__real@00000000` | 0x006AF921 | 0x0122DEDC | IEEE literal payload |

5 supported pairs out of 5. See the separate manual audit where supplied.
