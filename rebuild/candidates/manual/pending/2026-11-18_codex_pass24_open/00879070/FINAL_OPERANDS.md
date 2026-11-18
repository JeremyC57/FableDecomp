# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x2B | `?GetObservedThing@CBase@NCameraModes@@QBEPBVCThingGameObject@@XZ` | 0x0087909A | 0x0084F1B0 | reloc_bindings |
| +0x32 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x008790A1 | 0x004C73D0 | reloc_bindings |
| +0x4D | `?GetObservedThing@CBase@NCameraModes@@QBEPBVCThingGameObject@@XZ` | 0x008790BC | 0x0084F1B0 | reloc_bindings |
| +0x54 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x008790C3 | 0x004C73D0 | reloc_bindings |
| +0x65 | `__real@3fcccccd` | 0x008790D3 | 0x012777F0 | IEEE literal payload |
| +0xEB | `__real@3b360b61` | 0x00879159 | 0x01238E00 | IEEE literal payload |
| +0xF1 | `__real@3e800000` | 0x0087915F | 0x0123078C | IEEE literal payload |
| +0x177 | `?TWO_PI@@3NB` | 0x008791E5 | 0x0124F2B8 | reloc_bindings |
| +0x1F4 | `?TWO_PI@@3NB` | 0x00879262 | 0x0124F2B8 | reloc_bindings |
| +0x20E | `?TWO_PI@@3NB` | 0x0087927C | 0x0124F2B8 | reloc_bindings |
| +0x21E | `?DELTA_NOTIONAL_ZERO@@3MB` | 0x0087928C | 0x0129BA3C | reloc_bindings |
| +0x235 | `__real@3f800000` | 0x008792A3 | 0x0122DED8 | IEEE literal payload |
| +0x267 | `__real@40800000` | 0x008792D5 | 0x0123088C | IEEE literal payload |
| +0x279 | `__real@00000000` | 0x008792E9 | 0x0122DEDC | IEEE literal payload |
| +0x290 | `?DELTA_NOTIONAL_ZERO@@3MB` | 0x00879300 | 0x0129BA3C | reloc_bindings |
| +0x2C3 | `__real@3f000000` | 0x00879333 | 0x012777F8 | IEEE literal payload |
| +0x2C9 | `__real@3f000000` | 0x00879339 | 0x012777F8 | IEEE literal payload |
| +0x2D9 | `__real@00000000` | 0x00879349 | 0x0122DEDC | IEEE literal payload |
| +0x2F4 | `__real@3f800000` | 0x00879364 | 0x0122DED8 | IEEE literal payload |
| +0x30D | `__real@3f000000` | 0x0087937D | 0x012777F8 | IEEE literal payload |
| +0x313 | `__real@3f000000` | 0x00879383 | 0x012777F8 | IEEE literal payload |
| +0x31D | `__real@00000000` | 0x0087938D | 0x0122DEDC | IEEE literal payload |
| +0x338 | `__real@3f800000` | 0x008793A8 | 0x0122DED8 | IEEE literal payload |
| +0x361 | `?GetLookDirectionFromParams@CProjectileWeapon@NCameraModes@@ABE?AVC3DVector@@MM@Z` | 0x008793D2 | 0x00878DB0 | Binding remains unconfirmed |
| +0x379 | `?GetLookDirectionFromParams@CProjectileWeapon@NCameraModes@@ABE?AVC3DVector@@MM@Z` | 0x008793EA | 0x00878DB0 | Binding remains unconfirmed |
| +0x382 | `?AccurateNormalise@C3DVector@@QAEAAV1@XZ` | 0x008793F3 | 0x00A14440 | reloc_bindings |
| +0x38B | `?AccurateNormalise@C3DVector@@QAEAAV1@XZ` | 0x008793FC | 0x00A14440 | reloc_bindings |
| +0x39F | `?GetPosFromParams@CProjectileWeapon@NCameraModes@@ABE?AVC3DVector@@MM@Z` | 0x00879410 | 0x00878BE0 | NAME_FIXES_QUEUE |
| +0x3B3 | `?GetPosFromParams@CProjectileWeapon@NCameraModes@@ABE?AVC3DVector@@MM@Z` | 0x00879424 | 0x00878BE0 | NAME_FIXES_QUEUE |
| +0x404 | `?AccurateNormalise@C3DVector@@QAEAAV1@XZ` | 0x00879475 | 0x00A14440 | reloc_bindings |
| +0x40D | `?AccurateNormalise@C3DVector@@QAEAAV1@XZ` | 0x0087947E | 0x00A14440 | reloc_bindings |

29 supported pairs out of 31. See the separate manual audit where supplied.
