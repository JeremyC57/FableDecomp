# Naming evidence

- The verified COFF leaf is `?GetCyrixModelName@CCpuInfo@@QBEPBDXZ`.
- The retail bytes implement CPUID family/model selection and return Cyrix product strings. The automatically assigned Ego_d renderer record is unrelated.
- The native call at 0x00C52E40 and neighboring Intel/AMD namers identify the descriptive CCpuInfo::GetCyrixModelName. The binding table already carries that spelling.

## Behavioral fixture

The fixture runs the selected GetCyrixModelName method body for explicit CPU family and model values.
