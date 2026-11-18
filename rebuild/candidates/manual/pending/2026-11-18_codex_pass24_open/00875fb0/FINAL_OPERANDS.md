# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x8 | `?ConstantFPS@CMainGameComponent@@1JA` | 0x00875FB6 | 0x01375550 | reloc_bindings |
| +0x17 | `__ftol2` | 0x00875FC6 | 0x00BFEA70 | reloc_bindings |
| +0x22 | `?GetFrame@CWorld@@SIJXZ` | 0x00875FD1 | 0x0049D870 | reloc_bindings |
| +0x56 | `__real@bf800000` | 0x00876004 | 0x0122DEE0 | IEEE literal payload |
| +0x64 | `?AccurateNormalise@C3DVector@@QAEAAV1@XZ` | 0x00876013 | 0x00A14440 | reloc_bindings |
| +0xA7 | `?AccurateNormalise@C3DVector@@QAEAAV1@XZ` | 0x00876056 | 0x00A14440 | reloc_bindings |
| +0xB1 | `__real@401921fb60000000` | 0x0087605F | 0x0124F2B8 | IEEE literal payload |
| +0x1C6 | `__real@3fc00000` | 0x00876174 | 0x0125AADC | IEEE literal payload |
| +0x1D8 | `__real@42c80000` | 0x00876186 | 0x01230A08 | IEEE literal payload |
| +0x1DD | `__CItanh` | 0x0087618C | 0x00BFECE2 | reloc_bindings |
| +0x1E2 | `__ftol2` | 0x00876191 | 0x00BFEA70 | reloc_bindings |
| +0x1F8 | `__real@3fa00000` | 0x008761A6 | 0x01265BB8 | IEEE literal payload |
| +0x1FD | `__ftol2` | 0x008761AC | 0x00BFEA70 | reloc_bindings |
| +0x224 | `__real@3f000000` | 0x008761D2 | 0x0122F59C | IEEE literal payload |
| +0x235 | `__real@3fa00000` | 0x008761E3 | 0x01265BB8 | IEEE literal payload |
| +0x23A | `__ftol2` | 0x008761E9 | 0x00BFEA70 | reloc_bindings |
| +0x263 | `__real@3f000000` | 0x00876211 | 0x0122F59C | IEEE literal payload |
| +0x274 | `__real@3fa00000` | 0x00876222 | 0x01265BB8 | IEEE literal payload |
| +0x279 | `__ftol2` | 0x00876228 | 0x00BFEA70 | reloc_bindings |
| +0x2B1 | `__real@3f000000` | 0x0087625F | 0x0122F59C | IEEE literal payload |

20 supported pairs out of 20. See the separate manual audit where supplied.
