# Naming evidence

- Ego_r 0x00574460 and FableWin 0x01E7E0A0 give the exact leaf `?PlayCollisionSound@CTCSoundPlayer@@QAEXABVC3DVector@@ABV?$CDefPointer@$$CBVCMaterialDef@@@@1@Z`.
- Native collision event construction confirms pos, pmaterial1, pmaterial2, bank and first_part. The final name reference is a fitted representation of the direct GetString result.
- The 0x00415DD0 GetString fallback and the private helper at 0x0066A920 have independent both-PDB and retail call evidence. The older masked matches resolve the string conversion to 0x00415D70 instead.
