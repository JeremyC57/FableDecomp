# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0x22 | `?GetObservedThing@CBase@NCameraModes@@QBEPBVCThingGameObject@@XZ` | 0x00873E31 | 0x0084F1B0 | reloc_bindings |
| +0x29 | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x00873E38 | 0x004C73D0 | reloc_bindings |
| +0x44 | `?GetObservedThing@CBase@NCameraModes@@QBEPBVCThingGameObject@@XZ` | 0x00873E53 | 0x0084F1B0 | reloc_bindings |
| +0x51 | `?GetStaticFocalPos@CTCGraphicAppearance@NGraphicAppearance@@QBE?AVC3DVector@@XZ` | 0x00873E60 | 0x004C10E0 | reloc_bindings |
| +0x77 | `?Get@CUserProfileManager@@SIAAV1@XZ` | 0x00873E86 | 0x0040D2A0 | reloc_bindings |
| +0x8F | `__real@00000000` | 0x00873E9D | 0x0122DEDC | IEEE literal payload |
| +0x9E | `__real@00000000` | 0x00873EAC | 0x0122DEDC | IEEE literal payload |
| +0xA6 | `__real@3f800000` | 0x00873EB4 | 0x0122DED8 | IEEE literal payload |
| +0xB5 | `__real@3f800000` | 0x00873EC3 | 0x0122DED8 | IEEE literal payload |
| +0xBB | `__real@3f000000` | 0x00873EC9 | 0x0122F59C | IEEE literal payload |
| +0x11F | `_free` | 0x00873F2E | 0x00BFEA14 | reloc_bindings |
| +0x13D | `?GetObservedThing@CBase@NCameraModes@@QBEPBVCThingGameObject@@XZ` | 0x00873F4C | 0x0084F1B0 | reloc_bindings |
| +0x144 | `?PeekPlayer@CThing@@QBEABVCPlayer@@XZ` | 0x00873F53 | 0x004C7A10 | reloc_bindings |
| +0x14B | `?GetCurrentMode@CPlayer@@QBE?AW4EPlayerMode@@XZ` | 0x00873F5A | 0x00633BE0 | reloc_bindings |
| +0x155 | `?Get@CUserProfileManager@@SIAAV1@XZ` | 0x00873F64 | 0x0040D2A0 | reloc_bindings |
| +0x175 | `__real@bf800000` | 0x00873F83 | 0x0122DEE0 | IEEE literal payload |
| +0x17E | `?Get@CUserProfileManager@@SIAAV1@XZ` | 0x00873F8D | 0x0040D2A0 | reloc_bindings |
| +0x197 | `?Get@CUserProfileManager@@SIAAV1@XZ` | 0x00873FA6 | 0x0040D2A0 | reloc_bindings |
| +0x1B7 | `__real@bf800000` | 0x00873FC5 | 0x0122DEE0 | IEEE literal payload |
| +0x1C0 | `?Get@CUserProfileManager@@SIAAV1@XZ` | 0x00873FCF | 0x0040D2A0 | reloc_bindings |
| +0x1E0 | `__real@bf800000` | 0x00873FEE | 0x0122DEE0 | IEEE literal payload |
| +0x1F1 | `_malloc` | 0x00874000 | 0x00BFEA0E | reloc_bindings |
| +0x249 | `__real@3f800000` | 0x00874057 | 0x0122DED8 | IEEE literal payload |
| +0x27D | `__real@3f800000` | 0x0087408B | 0x0122DED8 | IEEE literal payload |
| +0x2E7 | `?Rotate@C3DVector@@QAEXABV1@M@Z` | 0x008740F6 | 0x00A13C60 | reloc_bindings |
| +0x2F8 | `__real@3b360b61` | 0x00874106 | 0x01238E00 | IEEE literal payload |
| +0x320 | `__real@3b360b61` | 0x0087412E | 0x01238E00 | IEEE literal payload |
| +0x329 | `?GetObservedThing@CBase@NCameraModes@@QBEPBVCThingGameObject@@XZ` | 0x00874138 | 0x0084F1B0 | reloc_bindings |
| +0x330 | `?PeekPlayer@CThing@@QBEABVCPlayer@@XZ` | 0x0087413F | 0x004C7A10 | reloc_bindings |
| +0x344 | `?PMainGameComponent@CMainGameComponent@@1PAV1@A` | 0x00874152 | 0x013B86A0 | reloc_bindings |
| +0x34B | `?GetPPlayerInterface@CMainGameComponent@@QAEPAVCGamePlayerInterface@@XZ` | 0x0087415A | 0x00416359 | reloc_bindings |
| +0x354 | `?GetObservedThing@CBase@NCameraModes@@QBEPBVCThingGameObject@@XZ` | 0x00874163 | 0x0084F1B0 | reloc_bindings |
| +0x35B | `?PeekPlayer@CThing@@QBEABVCPlayer@@XZ` | 0x0087416A | 0x004C7A10 | reloc_bindings |
| +0x362 | `?GetJoystickDeviceNumber@CPlayer@@QBEJXZ` | 0x00874171 | 0x00487510 | reloc_bindings |
| +0x36E | `?IsGameActionEventInQueue@CGamePlayerInterface@@QBE_NW4EGameAction@@W4EInputButtonEventType@@J@Z` | 0x0087417D | 0x00446B10 | reloc_bindings |
| +0x37B | `?GetObservedThing@CBase@NCameraModes@@QBEPBVCThingGameObject@@XZ` | 0x0087418A | 0x0084F1B0 | reloc_bindings |
| +0x392 | `?GetObservedThing@CBase@NCameraModes@@QBEPBVCThingGameObject@@XZ` | 0x008741A1 | 0x0084F1B0 | reloc_bindings |
| +0x3B5 | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x008741C4 | 0x004365B0 | reloc_bindings |
| +0x414 | `?GetObservedThing@CBase@NCameraModes@@QBEPBVCThingGameObject@@XZ` | 0x00874223 | 0x0084F1B0 | reloc_bindings |
| +0x41B | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | 0x0087422A | 0x004C73D0 | reloc_bindings |
| +0x47F | `__real@3eaaa64c` | 0x0087428D | 0x01277778 | IEEE literal payload |
| +0x491 | `__real@3eaaa64c` | 0x0087429F | 0x01277778 | IEEE literal payload |
| +0x4A7 | `__real@3eaaa64c` | 0x008742B5 | 0x01277778 | IEEE literal payload |
| +0x4ED | `?DELTA_NOTIONAL_ZERO@@3MB` | 0x008742FB | 0x0129BA3C | reloc_bindings |
| +0x4F3 | `?DELTA_NOTIONAL_ZERO@@3MB` | 0x00874301 | 0x0129BA3C | reloc_bindings |
| +0x509 | `?AccurateNormalise@C3DVector@@QAEAAV1@XZ` | 0x00874318 | 0x00A14440 | reloc_bindings |
| +0x539 | `__real@bf800000` | 0x00874347 | 0x0122DEE0 | IEEE literal payload |
| +0x554 | `__real@3f800000` | 0x00874362 | 0x0122DED8 | IEEE literal payload |
| +0x56C | `__CIacos` | 0x0087437B | 0x00BFEBA2 | reloc_bindings |
| +0x572 | `__real@3e22f983` | 0x00874380 | 0x01237F08 | IEEE literal payload |
| +0x57C | `__real@00000000` | 0x0087439C | 0x0122DEDC | IEEE literal payload (nearby instruction) |
| +0x596 | `__real@00000000` | 0x008743A4 | 0x0122DEDC | IEEE literal payload |
| +0x5A3 | `__real@bf800000` | 0x008743B1 | 0x0122DEE0 | IEEE literal payload |
| +0x5AD | `__real@3ec00000` | 0x008743BB | 0x01277774 | IEEE literal payload |
| +0x5BE | `__real@00000000` | 0x008743CC | 0x0122DEDC | IEEE literal payload |
| +0x602 | `__real@3ec00000` | 0x00874410 | 0x01277774 | IEEE literal payload |
| +0x60B | `?Rotate@C3DVector@@QAEXABV1@M@Z` | 0x0087441A | 0x00A13C60 | reloc_bindings |
| +0x637 | `__real@bf800000` | 0x00874445 | 0x0122DEE0 | IEEE literal payload |
| +0x652 | `__real@3f800000` | 0x00874460 | 0x0122DED8 | IEEE literal payload |
| +0x66A | `__CIacos` | 0x00874479 | 0x00BFEBA2 | reloc_bindings |
| +0x670 | `__real@3e22f983` | 0x0087447E | 0x01237F08 | IEEE literal payload |
| +0x69E | `__real@00000000` | 0x008744AC | 0x0122DEDC | IEEE literal payload |
| +0x6AB | `__real@bf800000` | 0x008744B9 | 0x0122DEE0 | IEEE literal payload |
| +0x6C7 | `__real@40400000` | 0x008744D5 | 0x0122DED4 | IEEE literal payload |
| +0x6D2 | `__real@40400000` | 0x008744E0 | 0x0122DED4 | IEEE literal payload |
| +0x6DB | `__real@40400000` | 0x008744E9 | 0x0122DED4 | IEEE literal payload |
| +0x6FF | `__real@3e800000` | 0x0087450D | 0x0123078C | IEEE literal payload |
| +0x711 | `__real@3e800000` | 0x0087451F | 0x0123078C | IEEE literal payload |
| +0x722 | `__real@3e800000` | 0x00874530 | 0x0123078C | IEEE literal payload |
| +0x752 | `?Get@CUserProfileManager@@SIAAV1@XZ` | 0x00874561 | 0x0040D2A0 | reloc_bindings |
| +0x77D | `?GetObservedThing@CBase@NCameraModes@@QBEPBVCThingGameObject@@XZ` | 0x0087458C | 0x0084F1B0 | reloc_bindings |
| +0x787 | `?GetObservedThing@CBase@NCameraModes@@QBEPBVCThingGameObject@@XZ` | 0x00874596 | 0x0084F1B0 | reloc_bindings |
| +0x7BB | `?DELTA_NOTIONAL_ZERO@@3MB` | 0x008745C9 | 0x0129BA3C | reloc_bindings |
| +0x7CE | `__real@3e000000` | 0x008745DC | 0x0123095C | IEEE literal payload |
| +0x7E0 | `__real@41200000` | 0x008745EE | 0x01237F0C | IEEE literal payload |
| +0x7E8 | `__real@3dba2e8c` | 0x008745F6 | 0x01277544 | IEEE literal payload |
| +0x808 | `__real@41000000` | 0x00874616 | 0x0125916C | IEEE literal payload |
| +0x80E | `__real@3f800000` | 0x0087461C | 0x0122DED8 | IEEE literal payload |
| +0x82A | `__real@3e800000` | 0x00874638 | 0x0123078C | IEEE literal payload |
| +0x83F | `__real@3e800000` | 0x0087464D | 0x0123078C | IEEE literal payload |
| +0x8C9 | `?Rotate@C3DVector@@QAEXABV1@M@Z` | 0x008746D8 | 0x00A13C60 | reloc_bindings |
| +0x929 | `?AddAttractorGoTo@CBase@NCameraModes@@IAEXABVC3DVector@@M@Z` | 0x0087473A | 0x0084E1A0 | reloc_bindings |

82 verified operand bindings. 0 operands still need an unambiguous instruction or binding.
