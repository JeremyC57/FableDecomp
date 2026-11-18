# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0x6 | `??0CEngineComponent@@QAE@XZ` | 0x00B50375 | 0x00B59710 | reloc_bindings |
| +0xC | `??_7CEngineShadowRenderer@@6B@` | 0x00B5037A | 0x012A2374 | reloc_bindings |
| +0x69 | `??0CBaseClass@@QAE@XZ` | 0x00B503D8 | 0x0099A2D0 | reloc_bindings |
| +0x6F | `??_7CRenderTarget@@6B@` | 0x00B503DD | 0x0129C800 | reloc_bindings |
| +0x7A | `??_7CSurface@@6B@` | 0x00B503E9 | 0x0122F84C | reloc_bindings |
| +0x18C | `?InitialiseTextures@CEngineShadowRenderer@@AAE_NXZ` | 0x00B504FB | 0x00B4F8E0 | reloc_bindings |

6 verified operand bindings. 0 operands still need an unambiguous instruction or binding.
