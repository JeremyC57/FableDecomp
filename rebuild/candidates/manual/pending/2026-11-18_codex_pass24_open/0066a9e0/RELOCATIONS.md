# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0x1 | `?g_bSoundEnabled@@3_NA` | 0x0066A9E0 | 0x0137E974 | reloc_bindings |
| +0x19 | `?GetSampleBank@CThing@@QBEAAVCASoundBank@@XZ` | 0x0066A9F8 | 0x004C7A80 | reloc_bindings |
| +0x20 | `??_C@_0O@HGEHPMJL@SE_COLLISION?$DL?$AA@` | 0x0066A9FF | 0x0125B1A4 | terminated literal payload |
| +0x2B | `??0CCharString@@QAE@PBDJ@Z` | 0x0066AA0A | 0x0099EBF0 | reloc_bindings |
| +0x5A | `?GDefStringTable@@3VCDefStringTable@@A` | 0x0066AA39 | 0x013CA828 | reloc_bindings |
| +0x5F | `?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z` | 0x0066AA3E | 0x009D49B0 | reloc_bindings |
| +0x75 | `??H@YI?AVCCharString@@ABV0@0@Z` | 0x0066AA54 | 0x0099F570 | reloc_bindings |
| +0x83 | `?InternalPlaySoundEventAtPos@CTCSoundPlayer@@AAEKAAVCASoundBank@@ABVC3DVector@@ABVCCharString@@_N3@Z` | 0x0066AA62 | 0x0066A920 | pass 24 naming evidence |
| +0x8C | `??1CCharString@@QAE@XZ` | 0x0066AA6B | 0x0099EAE0 | reloc_bindings |
| +0x95 | `??1CCharString@@QAE@XZ` | 0x0066AA74 | 0x0099EAE0 | reloc_bindings |
| +0x9E | `??1CCharString@@QAE@XZ` | 0x0066AA7D | 0x0099EAE0 | reloc_bindings |
| +0xC8 | `?GDefStringTable@@3VCDefStringTable@@A` | 0x0066AAA7 | 0x013CA828 | reloc_bindings |
| +0xCD | `?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z` | 0x0066AAAC | 0x009D49B0 | reloc_bindings |
| +0xE3 | `??H@YI?AVCCharString@@ABV0@0@Z` | 0x0066AAC2 | 0x0099F570 | reloc_bindings |
| +0x105 | `?GDefStringTable@@3VCDefStringTable@@A` | 0x0066AAE4 | 0x013CA828 | reloc_bindings |
| +0x10A | `?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z` | 0x0066AAE9 | 0x009D49B0 | reloc_bindings |
| +0x123 | `?GDefStringTable@@3VCDefStringTable@@A` | 0x0066AB02 | 0x013CA828 | reloc_bindings |
| +0x128 | `?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z` | 0x0066AB07 | 0x009D49B0 | reloc_bindings |
| +0x136 | `??_C@_01ICJEACDI@?$DL?$AA@` | 0x0066AB15 | 0x0123542C | literal payload prefix |
| +0x148 | `??H@YI?AVCCharString@@ABV0@0@Z` | 0x0066AB27 | 0x0099F570 | reloc_bindings |
| +0x153 | `??H@YI?AVCCharString@@ABV0@PBD@Z` | 0x0066AB32 | 0x0099F600 | reloc_bindings |
| +0x15E | `??H@YI?AVCCharString@@ABV0@0@Z` | 0x0066AB3D | 0x0099F570 | reloc_bindings |
| +0x16C | `?InternalPlaySoundEventAtPos@CTCSoundPlayer@@AAEKAAVCASoundBank@@ABVC3DVector@@ABVCCharString@@_N3@Z` | 0x0066AB4B | 0x0066A920 | pass 24 naming evidence |
| +0x172 | `?g_InvalidSoundHandle@@3KB` | 0x0066AB50 | 0x0129D25C | reloc_bindings |
| +0x180 | `??1CCharString@@QAE@XZ` | 0x0066AB5F | 0x0099EAE0 | reloc_bindings |
| +0x189 | `??1CCharString@@QAE@XZ` | 0x0066AB68 | 0x0099EAE0 | reloc_bindings |
| +0x192 | `??1CCharString@@QAE@XZ` | 0x0066AB71 | 0x0099EAE0 | reloc_bindings |
| +0x19B | `??1CCharString@@QAE@XZ` | 0x0066AB7A | 0x0099EAE0 | reloc_bindings |
| +0x1A4 | `??1CCharString@@QAE@XZ` | 0x0066AB83 | 0x0099EAE0 | reloc_bindings |
| +0x1C9 | `?GDefStringTable@@3VCDefStringTable@@A` | 0x0066ABA8 | 0x013CA828 | reloc_bindings |
| +0x1CE | `?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z` | 0x0066ABAD | 0x009D49B0 | reloc_bindings |
| +0x1E4 | `??H@YI?AVCCharString@@ABV0@0@Z` | 0x0066ABC3 | 0x0099F570 | reloc_bindings |
| +0x1EE | `?InternalPlaySoundEventAtPos@CTCSoundPlayer@@AAEKAAVCASoundBank@@ABVC3DVector@@ABVCCharString@@_N3@Z` | 0x0066ABCD | 0x0066A920 | pass 24 naming evidence |
| +0x1F4 | `?g_InvalidSoundHandle@@3KB` | 0x0066ABD2 | 0x0129D25C | reloc_bindings |
| +0x200 | `??1CCharString@@QAE@XZ` | 0x0066ABDF | 0x0099EAE0 | reloc_bindings |
| +0x209 | `??1CCharString@@QAE@XZ` | 0x0066ABE8 | 0x0099EAE0 | reloc_bindings |
| +0x229 | `?GetString@CDefString@@QBE?AVCCharString@@XZ` | 0x0066AC08 | 0x00415DD0 | reloc_bindings |
| +0x23F | `??H@YI?AVCCharString@@ABV0@0@Z` | 0x0066AC1E | 0x0099F570 | reloc_bindings |
| +0x249 | `?InternalPlaySoundEventAtPos@CTCSoundPlayer@@AAEKAAVCASoundBank@@ABVC3DVector@@ABVCCharString@@_N3@Z` | 0x0066AC28 | 0x0066A920 | pass 24 naming evidence |
| +0x252 | `??1CCharString@@QAE@XZ` | 0x0066AC31 | 0x0099EAE0 | reloc_bindings |
| +0x25B | `??1CCharString@@QAE@XZ` | 0x0066AC3A | 0x0099EAE0 | reloc_bindings |
| +0x265 | `??1CCharString@@QAE@XZ` | 0x0066AC44 | 0x0099EAE0 | reloc_bindings |

42 verified operand bindings. 0 operands still need an unambiguous instruction or binding.
