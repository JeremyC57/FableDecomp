# Relocation review

Each COFF operand is compared with the corresponding retail instruction.

| Leaf offset | COFF operand | Retail instruction | Retail operand | Evidence |
| --- | --- | --- | --- | --- |
| +0x2 | `?PMainGameComponent@CMainGameComponent@@1PAV1@A` | 0x006E3891 | 0x013B86A0 | reloc_bindings |
| +0x2B | `?GetEntryDataType@CBankFile@@QBEKK@Z` | 0x006E38BA | 0x009CC210 | reloc_bindings |
| +0x40 | `?g_bForceSpeech@@3_NA` | 0x006E38CB | 0x013B86DA | Native byte read for the synchronous-load override. Source spelling is descriptive. |
| +0x5A | `?GetPTextEntry@CDataBank@NGameText@@QBE?AV?$CIVCountedPointer@$$CBVCTextEntry@NGameText@@@@K_N@Z` | 0x006E38E5 | 0x009C9070 | reloc_bindings |
| +0x6B | `?g_bForceSpeech@@3_NA` | 0x006E38F6 | 0x013B86DA | Native byte read for the synchronous-load override. Source spelling is descriptive. |
| +0xB4 | `?PSoundManager@@3PAVCASoundManager@@A` | 0x006E397D | 0x013B8394 | reloc_bindings |
| +0xC1 | `?GetPSoundBank@CASoundManager@@QBEPAVCASoundBank@@ABVCCharString@@@Z` | 0x006E398B | 0x009917E0 | reloc_bindings |
| +0xD0 | `??_C@_04HHBFPGHA@SND_?$AA@` | 0x006E399A | 0x01239ED4 | literal payload prefix |
| +0xD9 | `??H@YI?AVCCharString@@PBDABV0@@Z` | 0x006E39A3 | 0x0099F690 | reloc_bindings |
| +0xE1 | `?GetSoundIndexFromSymbol@CASoundBank@@QBEJABVCCharString@@@Z` | 0x006E39AB | 0x00A01920 | reloc_bindings |
| +0xEC | `??1CCharString@@QAE@XZ` | 0x006E39B6 | 0x0099EAE0 | reloc_bindings |
| +0x10D | `?PlaySpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@AAVCASoundBank@@J_NABVCCharString@@W4EDialogueLayer@NSpeechGainManager@@11@Z` | 0x006E39D7 | 0x006E3580 | reloc_bindings |
| +0x11F | `?g_bShowSpeechEmotes@@3_NA` | 0x006E39E9 | 0x013B86D9 | Native byte read before the emote fallback. Source spelling is descriptive. |
| +0x14B | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z` | 0x006E3A15 | 0x0040F020 | reloc_bindings |
| +0x168 | `?GetText@CTextEntry@NGameText@@QBE?AVCWideString@@XZ` | 0x006E3A32 | 0x009C7A40 | reloc_bindings |
| +0x181 | `?GetLength@CWideString@@QBEJXZ` | 0x006E3A4B | 0x0099B220 | reloc_bindings |
| +0x194 | `__real@3dcccccd` | 0x006E3A5D | 0x0122EE58 | IEEE literal payload |
| +0x1A5 | `??$GFLimitReturn@M@@YIABMABM00@Z` | 0x006E3A6F | 0x0057EF86 | reloc_bindings |
| +0x1BB | `?SetEmoteText@CTCEmoteIcon@@QAEXW4EEmotePriority@1@ABVCWideString@@MM@Z` | 0x006E3A85 | 0x0071AC70 | reloc_bindings |
| +0x1C8 | `??1CWideString@@QAE@XZ` | 0x006E3A92 | 0x0099B510 | reloc_bindings |
| +0x1E0 | `?g_bForceSpeech@@3_NA` | 0x006E393A | 0x013B86DA | Native byte read for the synchronous-load override. Source spelling is descriptive. Retail places the voiced-group arm before playback. |
| +0x203 | `?GetTextEntryFromGroup@CDataBank@NGameText@@QBEPBVCTextEntry@2@KABVCCharString@@AA_NPAJ_N@Z` | 0x006E395D | 0x009C9280 | reloc_bindings Retail places the voiced-group arm before playback. |

22 verified operand bindings. 0 operands still need an unambiguous instruction or binding.
