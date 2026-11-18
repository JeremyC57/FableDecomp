# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0xE | `??_C@_0N@CLJGIAFN@SIMPLE_NOISE?$AA@` | 0x007A76FD | 0x01268A5C | terminated literal payload |
| +0x17 | `??0CCharString@@QAE@PBDJ@Z` | 0x007A770A | 0x0099EBF0 | reloc_bindings |
| +0x23 | `?MakeNoise@CThingCreatureBase@@QAEXABVCCharString@@@Z` | 0x007A7716 | 0x00662110 | reloc_bindings |
| +0x2C | `??1CCharString@@QAE@XZ` | 0x007A771F | 0x0099EAE0 | reloc_bindings |
| +0x43 | `??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z` | 0x007A7736 | 0x00669580 | reloc_bindings |
| +0x71 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x007A7764 | 0x00A01B50 | reloc_bindings |
| +0x7C | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x007A776F | 0x00A01B50 | reloc_bindings |
| +0x8C | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x007A777F | 0x00A01B50 | reloc_bindings |
| +0x95 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x007A7788 | 0x004C73D0 | reloc_bindings (nearby instruction) |
| +0x9E | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x007A7791 | 0x004C73D0 | reloc_bindings |
| +0xEF | `??_C@_0BB@ONKKHDIL@SHIELD_EFFECT_01?$AA@` | 0x007A77E2 | 0x0126CCD0 | terminated literal payload |
| +0x120 | `??0CCharString@@QAE@PBDJ@Z` | 0x007A7813 | 0x0099EBF0 | reloc_bindings |
| +0x131 | `??_C@_0BB@JANNIHMO@SHIELD_EFFECT_04?$AA@` | 0x007A7824 | 0x0126CCBC | terminated literal payload |
| +0x138 | `??_C@_0BB@NPJMBBAJ@SHIELD_EFFECT_03?$AA@` | 0x007A782B | 0x0126CCA8 | terminated literal payload |
| +0x13F | `??_C@_0BB@MGIHCAEI@SHIELD_EFFECT_02?$AA@` | 0x007A7832 | 0x0126CC94 | terminated literal payload |
| +0x148 | `??4CCharString@@QAEABV0@PBD@Z` | 0x007A783B | 0x0099EFE0 | reloc_bindings |
| +0x15D | `?GetEffectFromString@CSpecialEffectsDef@@QBE_NABVCCharString@@AAJ@Z` | 0x007A7850 | 0x00669D30 | reloc_bindings |
| +0x16E | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x007A7861 | 0x004C73D0 | reloc_bindings |
| +0x179 | `?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z` | 0x007A786C | 0x006E0880 | reloc_bindings |
| +0x184 | `??0CBaseIntelligentPointer@@QAE@XZ` | 0x007A7877 | 0x00A01B10 | reloc_bindings |
| +0x191 | `??_7?$CIntelligentPointer@VCThing@@@@6B@` | 0x007A7881 | 0x01238C6C | Original native vtable used by the shield effect pointer temporary |
| +0x196 | `?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z` | 0x007A7889 | 0x00A01B90 | reloc_bindings |
| +0x19F | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x007A7892 | 0x00A01B50 | reloc_bindings |
| +0x1AA | `?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z` | 0x007A789D | 0x00A01B90 | reloc_bindings |
| +0x1B3 | `??1CBaseIntelligentPointer@@UAE@XZ` | 0x007A78A6 | 0x00A01C10 | reloc_bindings |
| +0x1BA | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x007A78AD | 0x00A01B50 | reloc_bindings |
| +0x1C9 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x007A78BC | 0x00A01B50 | reloc_bindings |
| +0x1D9 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x007A78D2 | 0x00A01B50 | reloc_bindings |
| +0x1F8 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x007A78F1 | 0x0040F020 | reloc_bindings |
| +0x216 | `??_C@_00CNPNBAHC@?$AA@` | 0x007A7909 | 0x0122D70E | literal payload prefix |
| +0x21F | `??0CCharString@@QAE@PBDJ@Z` | 0x007A7912 | 0x0099EBF0 | reloc_bindings |
| +0x232 | `?AttachToThing@CTCDParticleEmitter@@QAEXAAVCThing@@JABVCCharString@@JM@Z` | 0x007A7929 | 0x006E0BE0 | reloc_bindings (nearby instruction) |
| +0x23B | `??1CCharString@@QAE@XZ` | 0x007A7932 | 0x0099EAE0 | reloc_bindings (nearby instruction) |
| +0x247 | `?SetEmitterOrientation@CTCDParticleEmitter@@QAEXABVCRightHandedSet@@@Z` | 0x007A793E | 0x006E0480 | reloc_bindings |
| +0x250 | `?SetAlwaysUpdate@CTCDParticleEmitter@@QAEX_N@Z` | 0x007A7947 | 0x006E0590 | reloc_bindings |
| +0x257 | `??_C@_0BB@OJBPKGIP@LEFT_FEET_EFFECT?$AA@` | 0x007A7952 | 0x0126CC80 | terminated literal payload |
| +0x260 | `??0CCharString@@QAE@PBDJ@Z` | 0x007A795B | 0x0099EBF0 | reloc_bindings |
| +0x271 | `?GetEffectFromString@CSpecialEffectsDef@@QBE_NABVCCharString@@AAJ@Z` | 0x007A796C | 0x00669D30 | reloc_bindings |
| +0x27C | `??1CCharString@@QAE@XZ` | 0x007A7977 | 0x0099EAE0 | reloc_bindings |
| +0x28E | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x007A7989 | 0x004C73D0 | reloc_bindings |
| +0x299 | `?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z` | 0x007A7994 | 0x006E0880 | reloc_bindings |
| +0x2A4 | `??0CBaseIntelligentPointer@@QAE@XZ` | 0x007A799F | 0x00A01B10 | reloc_bindings |
| +0x2B1 | `??_7?$CIntelligentPointer@VCThing@@@@6B@` | 0x007A79A9 | 0x01238C6C | Original native vtable used by the left foot effect pointer temporary |
| +0x2B6 | `?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z` | 0x007A79B1 | 0x00A01B90 | reloc_bindings |
| +0x2BF | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x007A79BA | 0x00A01B50 | reloc_bindings |
| +0x2CA | `?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z` | 0x007A79C5 | 0x00A01B90 | reloc_bindings |
| +0x2D3 | `??1CBaseIntelligentPointer@@UAE@XZ` | 0x007A79CE | 0x00A01C10 | reloc_bindings |
| +0x2DA | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x007A79D5 | 0x00A01B50 | reloc_bindings |
| +0x2E9 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x007A79E4 | 0x00A01B50 | reloc_bindings |
| +0x2F9 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x007A79FA | 0x00A01B50 | reloc_bindings |
| +0x318 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x007A7A19 | 0x0040F020 | reloc_bindings |
| +0x336 | `??_C@_00CNPNBAHC@?$AA@` | 0x007A7A31 | 0x0122D70E | literal payload prefix |
| +0x33F | `??0CCharString@@QAE@PBDJ@Z` | 0x007A7A3A | 0x0099EBF0 | reloc_bindings |
| +0x352 | `?AttachToThing@CTCDParticleEmitter@@QAEXAAVCThing@@JABVCCharString@@JM@Z` | 0x007A7A51 | 0x006E0BE0 | reloc_bindings (nearby instruction) |
| +0x35B | `??1CCharString@@QAE@XZ` | 0x007A7A5A | 0x0099EAE0 | reloc_bindings (nearby instruction) |
| +0x367 | `?SetEmitterOrientation@CTCDParticleEmitter@@QAEXABVCRightHandedSet@@@Z` | 0x007A7A66 | 0x006E0480 | reloc_bindings |
| +0x370 | `?SetAlwaysUpdate@CTCDParticleEmitter@@QAEX_N@Z` | 0x007A7A6F | 0x006E0590 | reloc_bindings |
| +0x377 | `??_C@_0BC@EIGMAOBO@RIGHT_FEET_EFFECT?$AA@` | 0x007A7A7A | 0x0126CC6C | terminated literal payload |
| +0x380 | `??0CCharString@@QAE@PBDJ@Z` | 0x007A7A83 | 0x0099EBF0 | reloc_bindings |
| +0x395 | `?GetEffectFromString@CSpecialEffectsDef@@QBE_NABVCCharString@@AAJ@Z` | 0x007A7A94 | 0x00669D30 | reloc_bindings |
| +0x3A0 | `??1CCharString@@QAE@XZ` | 0x007A7A9F | 0x0099EAE0 | reloc_bindings |
| +0x3B3 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x007A7AB1 | 0x004C73D0 | reloc_bindings |
| +0x3BE | `?Create@CTCDParticleEmitter@@SIPAVCThing@@JABVC3DVector@@_N@Z` | 0x007A7ABC | 0x006E0880 | reloc_bindings |
| +0x3C9 | `??0CBaseIntelligentPointer@@QAE@XZ` | 0x007A7AC7 | 0x00A01B10 | reloc_bindings |
| +0x3D6 | `??_7?$CIntelligentPointer@VCThing@@@@6B@` | 0x007A7AD1 | 0x01238C6C | Original native vtable used by the right foot effect pointer temporary |
| +0x3DB | `?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z` | 0x007A7AD9 | 0x00A01B90 | reloc_bindings |
| +0x3E4 | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x007A7AE2 | 0x00A01B50 | reloc_bindings |
| +0x3EF | `?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z` | 0x007A7AED | 0x00A01B90 | reloc_bindings |
| +0x3F8 | `??1CBaseIntelligentPointer@@UAE@XZ` | 0x007A7AF6 | 0x00A01C10 | reloc_bindings |
| +0x3FF | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x007A7AFD | 0x00A01B50 | reloc_bindings |
| +0x40E | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x007A7B0C | 0x00A01B50 | reloc_bindings |
| +0x41E | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | 0x007A7B22 | 0x00A01B50 | reloc_bindings |
| +0x43D | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x007A7B41 | 0x0040F020 | reloc_bindings |
| +0x45B | `??_C@_00CNPNBAHC@?$AA@` | 0x007A7B59 | 0x0122D70E | literal payload prefix |
| +0x464 | `??0CCharString@@QAE@PBDJ@Z` | 0x007A7B62 | 0x0099EBF0 | reloc_bindings |
| +0x477 | `?AttachToThing@CTCDParticleEmitter@@QAEXAAVCThing@@JABVCCharString@@JM@Z` | 0x007A7B79 | 0x006E0BE0 | reloc_bindings |
| +0x480 | `??1CCharString@@QAE@XZ` | 0x007A7B82 | 0x0099EAE0 | reloc_bindings |
| +0x48C | `?SetEmitterOrientation@CTCDParticleEmitter@@QAEXABVCRightHandedSet@@@Z` | 0x007A7B8E | 0x006E0480 | reloc_bindings |
| +0x495 | `?SetAlwaysUpdate@CTCDParticleEmitter@@QAEX_N@Z` | 0x007A7B97 | 0x006E0590 | reloc_bindings |
| +0x4A2 | `??1CCharString@@QAE@XZ` | 0x007A7BA4 | 0x0099EAE0 | reloc_bindings |

80 verified operand bindings. 0 operands still need an unambiguous instruction or binding.
