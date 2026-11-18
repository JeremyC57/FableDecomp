# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x17 | `?Save@CThingPhysical@@MAEXAAVCCharString@@@Z` | 0x00837586 | 0x006A4BA0 | Both PDBs mark the protected override. Native sibling call and the base-qualified call at 0x00837586. |
| +0x2D | `?PCurrentThingManager@CThing@@0PAVCThingManager@@A` | 0x0083759B | 0x013B8A1C | reloc_bindings |
| +0x3E | `?IsLevelLess@CThing@@QBE_NXZ` | 0x008375AD | 0x004C7930 | reloc_bindings |
| +0x48 | `?RemapUIDForSerialisation@CThingManager@@QBE_K_N0K_K@Z` | 0x008375B7 | 0x0051E210 | reloc_bindings |
| +0x53 | `??_C@_03LKGCEJK@?$DL?$AN?6?$AA@` | 0x008375C2 | 0x012353B8 | literal payload prefix |
| +0x5E | `?GFUHUGEToCharString@@YI?AVCCharString@@_K@Z` | 0x008375CD | 0x0099F930 | reloc_bindings |
| +0x64 | `??_C@_0O@EJNAIHIH@LinkedToUID1?5?$AA@` | 0x008375D3 | 0x01272E28 | terminated literal payload |
| +0x6D | `??H@YI?AVCCharString@@PBDABV0@@Z` | 0x008375DC | 0x0099F690 | reloc_bindings |
| +0x78 | `??H@YI?AVCCharString@@ABV0@PBD@Z` | 0x008375E7 | 0x0099F600 | reloc_bindings |
| +0x80 | `??YCCharString@@QAEABV0@ABV0@@Z` | 0x008375EF | 0x0099F0A0 | reloc_bindings |
| +0x89 | `??1CCharString@@QAE@XZ` | 0x008375F8 | 0x0099EAE0 | reloc_bindings |
| +0x92 | `??1CCharString@@QAE@XZ` | 0x00837601 | 0x0099EAE0 | reloc_bindings |
| +0x9B | `??1CCharString@@QAE@XZ` | 0x0083760A | 0x0099EAE0 | reloc_bindings |
| +0xB1 | `?PCurrentThingManager@CThing@@0PAVCThingManager@@A` | 0x0083761F | 0x013B8A1C | reloc_bindings |
| +0xC2 | `?IsLevelLess@CThing@@QBE_NXZ` | 0x00837631 | 0x004C7930 | reloc_bindings |
| +0xCC | `?RemapUIDForSerialisation@CThingManager@@QBE_K_N0K_K@Z` | 0x0083763B | 0x0051E210 | reloc_bindings |
| +0xD7 | `??_C@_03LKGCEJK@?$DL?$AN?6?$AA@` | 0x00837646 | 0x012353B8 | literal payload prefix |
| +0xE2 | `?GFUHUGEToCharString@@YI?AVCCharString@@_K@Z` | 0x00837651 | 0x0099F930 | reloc_bindings |
| +0xE8 | `??_C@_0O@ELJGDJNO@LinkedToUID2?5?$AA@` | 0x00837657 | 0x01272E18 | terminated literal payload |
| +0xF1 | `??H@YI?AVCCharString@@PBDABV0@@Z` | 0x00837660 | 0x0099F690 | reloc_bindings |
| +0xFC | `??H@YI?AVCCharString@@ABV0@PBD@Z` | 0x0083766B | 0x0099F600 | reloc_bindings |
| +0x104 | `??YCCharString@@QAEABV0@ABV0@@Z` | 0x00837673 | 0x0099F0A0 | reloc_bindings |
| +0x10D | `??1CCharString@@QAE@XZ` | 0x0083767C | 0x0099EAE0 | reloc_bindings |
| +0x116 | `??1CCharString@@QAE@XZ` | 0x00837685 | 0x0099EAE0 | reloc_bindings |
| +0x11F | `??1CCharString@@QAE@XZ` | 0x0083768E | 0x0099EAE0 | reloc_bindings |
| +0x12C | `??_C@_04HCDDPBNL@TRUE?$AA@` | 0x0083769B | 0x0122D6D4 | literal payload prefix |
| +0x133 | `??_C@_05MAJJAKPI@FALSE?$AA@` | 0x008376A2 | 0x0122D6CC | literal payload prefix |
| +0x138 | `??_C@_03LKGCEJK@?$DL?$AN?6?$AA@` | 0x008376A7 | 0x012353B8 | literal payload prefix |
| +0x13E | `??_C@_06EDAFEOLI@Start?5?$AA@` | 0x008376AD | 0x01272E10 | literal payload prefix |
| +0x145 | `??_C@_00CNPNBAHC@?$AA@` | 0x008376B4 | 0x0122D70E | literal payload prefix |
| +0x14E | `??0CCharString@@QAE@PBDJ@Z` | 0x008376BD | 0x0099EBF0 | reloc_bindings |
| +0x159 | `??H@YI?AVCCharString@@ABV0@PBD@Z` | 0x008376C8 | 0x0099F600 | reloc_bindings |
| +0x164 | `??H@YI?AVCCharString@@ABV0@PBD@Z` | 0x008376D3 | 0x0099F600 | reloc_bindings |
| +0x16F | `??H@YI?AVCCharString@@ABV0@PBD@Z` | 0x008376DE | 0x0099F600 | reloc_bindings |
| +0x177 | `??YCCharString@@QAEABV0@ABV0@@Z` | 0x008376E6 | 0x0099F0A0 | reloc_bindings |
| +0x180 | `??1CCharString@@QAE@XZ` | 0x008376EF | 0x0099EAE0 | reloc_bindings |
| +0x189 | `??1CCharString@@QAE@XZ` | 0x008376F8 | 0x0099EAE0 | reloc_bindings |
| +0x192 | `??1CCharString@@QAE@XZ` | 0x00837701 | 0x0099EAE0 | reloc_bindings |
| +0x19B | `??1CCharString@@QAE@XZ` | 0x0083770A | 0x0099EAE0 | reloc_bindings |
| +0x1A8 | `??_C@_04HCDDPBNL@TRUE?$AA@` | 0x00837717 | 0x0122D6D4 | literal payload prefix |
| +0x1AF | `??_C@_05MAJJAKPI@FALSE?$AA@` | 0x0083771E | 0x0122D6CC | literal payload prefix |
| +0x1B4 | `??_C@_03LKGCEJK@?$DL?$AN?6?$AA@` | 0x00837723 | 0x012353B8 | literal payload prefix |
| +0x1BA | `??_C@_04KIGNBILF@End?5?$AA@` | 0x00837729 | 0x01272E08 | literal payload prefix |
| +0x1C1 | `??_C@_00CNPNBAHC@?$AA@` | 0x00837730 | 0x0122D70E | literal payload prefix |
| +0x1CA | `??0CCharString@@QAE@PBDJ@Z` | 0x00837739 | 0x0099EBF0 | reloc_bindings |
| +0x1D5 | `??H@YI?AVCCharString@@ABV0@PBD@Z` | 0x00837744 | 0x0099F600 | reloc_bindings |
| +0x1E0 | `??H@YI?AVCCharString@@ABV0@PBD@Z` | 0x0083774F | 0x0099F600 | reloc_bindings |
| +0x1EB | `??H@YI?AVCCharString@@ABV0@PBD@Z` | 0x0083775A | 0x0099F600 | reloc_bindings |
| +0x1F3 | `??YCCharString@@QAEABV0@ABV0@@Z` | 0x00837762 | 0x0099F0A0 | reloc_bindings |
| +0x1FC | `??1CCharString@@QAE@XZ` | 0x0083776B | 0x0099EAE0 | reloc_bindings |
| +0x205 | `??1CCharString@@QAE@XZ` | 0x00837774 | 0x0099EAE0 | reloc_bindings |
| +0x20E | `??1CCharString@@QAE@XZ` | 0x0083777D | 0x0099EAE0 | reloc_bindings |
| +0x217 | `??1CCharString@@QAE@XZ` | 0x00837786 | 0x0099EAE0 | reloc_bindings |
| +0x22C | `??_C@_00CNPNBAHC@?$AA@` | 0x0083779B | 0x0122D70E | literal payload prefix |
| +0x244 | `??_C@_03LKGCEJK@?$DL?$AN?6?$AA@` | 0x008377B3 | 0x012353B8 | literal payload prefix |
| +0x24A | `??_C@_0M@DDCGHBPB@ScriptName?5?$AA@` | 0x008377B9 | 0x01239BD8 | terminated literal payload |
| +0x253 | `??H@YI?AVCCharString@@PBDABV0@@Z` | 0x008377C2 | 0x0099F690 | reloc_bindings |
| +0x25E | `??H@YI?AVCCharString@@ABV0@PBD@Z` | 0x008377CD | 0x0099F600 | reloc_bindings |
| +0x266 | `??YCCharString@@QAEABV0@ABV0@@Z` | 0x008377D5 | 0x0099F0A0 | reloc_bindings |
| +0x26F | `??1CCharString@@QAE@XZ` | 0x008377DE | 0x0099EAE0 | reloc_bindings |
| +0x278 | `??1CCharString@@QAE@XZ` | 0x008377E7 | 0x0099EAE0 | reloc_bindings |
| +0x28A | `??1CCharString@@QAE@XZ` | 0x008377F9 | 0x0099EAE0 | reloc_bindings |
| +0x295 | `??_C@_00CNPNBAHC@?$AA@` | 0x00837804 | 0x0122D70E | literal payload prefix |
| +0x29A | `??8?$CBasicString@D@@QBE_NPBD@Z` | 0x00837809 | 0x004115A0 | reloc_bindings |
| +0x2A5 | `??_C@_04HIBGFPH@NULL?$AA@` | 0x00837814 | 0x012393E4 | literal payload prefix |
| +0x2B3 | `??0CCharString@@QAE@PBDJ@Z` | 0x00837822 | 0x0099EBF0 | reloc_bindings |

66 supported pairs out of 66. See the separate manual audit where supplied.
