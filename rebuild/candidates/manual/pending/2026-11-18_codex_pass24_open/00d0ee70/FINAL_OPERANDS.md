# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x13 | `??_C@_05JECOADMH@Gate1?$AA@` | 0x00D0EE82 | 0x012C86A4 | literal payload prefix |
| +0x1C | `??0CCharString@@QAE@PBDJ@Z` | 0x00D0EE8B | 0x0099EBF0 | reloc_bindings |
| +0x53 | `??_7CScriptThing@@6B@` | Unpaired | Unpaired | Instruction sequence differs |
| +0x6F | `??3@YAXPAX@Z` | 0x00D0EEDA | 0x00BFE9BC | reloc_bindings |
| +0x83 | `??1CBaseClass@@UAE@XZ` | 0x00D0EEEE | 0x0099A2E0 | reloc_bindings |
| +0x8C | `??1CCharString@@QAE@XZ` | 0x00D0EEF7 | 0x0099EAE0 | reloc_bindings |
| +0x9F | `?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ` | 0x00D0EF0A | 0x00CB7940 | reloc_bindings |
| +0xB6 | `?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ` | 0x00D0EF21 | 0x00CB7940 | reloc_bindings |
| +0xC9 | `?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ` | 0x00D0EF79 | 0x00CB7940 | reloc_bindings |
| +0xD8 | `??_C@_0P@NFGFFBHG@BanditCampMain?$AA@` | 0x00D0EF88 | 0x012C8F78 | terminated literal payload |
| +0xE1 | `??0CCharString@@QAE@PBDJ@Z` | 0x00D0EF91 | 0x0099EBF0 | reloc_bindings |
| +0xF9 | `??1CCharString@@QAE@XZ` | 0x00D0EF6A | 0x0099EAE0 | reloc_bindings (nearby instruction) |
| +0x108 | `?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ` | 0x00D0EF79 | 0x00CB7940 | reloc_bindings (nearby instruction) |
| +0x117 | `??_C@_0P@NFGFFBHG@BanditCampMain?$AA@` | 0x00D0EF88 | 0x012C8F78 | terminated literal payload (nearby instruction) |
| +0x120 | `??0CCharString@@QAE@PBDJ@Z` | 0x00D0EF91 | 0x0099EBF0 | reloc_bindings (nearby instruction) |
| +0x138 | `??1CCharString@@QAE@XZ` | 0x00D0EFAF | 0x0099EAE0 | reloc_bindings |
| +0x14B | `?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ` | 0x00D0EFC2 | 0x00CB7940 | reloc_bindings |
| +0x15A | `??_C@_0P@NFGFFBHG@BanditCampMain?$AA@` | 0x00D0EFD1 | 0x012C8F78 | terminated literal payload |
| +0x163 | `??0CCharString@@QAE@PBDJ@Z` | 0x00D0EFDA | 0x0099EBF0 | reloc_bindings |
| +0x17B | `??1CCharString@@QAE@XZ` | 0x00D0EFF8 | 0x0099EAE0 | reloc_bindings |
| +0x186 | `?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ` | 0x00D0F003 | 0x00CB7940 | reloc_bindings |
| +0x1BF | `?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ` | 0x00D0F03E | 0x00CB7940 | reloc_bindings |
| +0x1EB | `?erase@?$vector@KV?$allocator@K@std@@@std@@QAEPAKPAK0@Z` | 0x00D0F06A | 0x0046496F | reloc_bindings |
| +0x243 | `?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ` | 0x00D0F0C2 | 0x00CB7940 | reloc_bindings |
| +0x26B | `?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ` | 0x00D0F0EA | 0x00CB7940 | reloc_bindings |
| +0x297 | `?erase@?$vector@KV?$allocator@K@std@@@std@@QAEPAKPAK0@Z` | 0x00D0F116 | 0x0046496F | reloc_bindings |
| +0x2C2 | `?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ` | 0x00D0F141 | 0x00CB7940 | reloc_bindings |
| +0x305 | `?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ` | 0x00D0F184 | 0x00CB7940 | reloc_bindings |
| +0x32D | `?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ` | 0x00D0F1AC | 0x00CB7940 | reloc_bindings |
| +0x359 | `?erase@?$vector@KV?$allocator@K@std@@@std@@QAEPAKPAK0@Z` | 0x00D0F1D8 | 0x0046496F | reloc_bindings |
| +0x384 | `?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ` | 0x00D0F203 | 0x00CB7940 | reloc_bindings |
| +0x396 | `?pGlobalScriptDef@@3V?$CDefPointer@$$CBVCScriptDef@@@@A` | 0x00D0F214 | 0x0143E90C | reloc_bindings |
| +0x3AC | `??1?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@QAE@XZ` | 0x00D0F22F | 0x008AC970 | reloc_bindings |
| +0x3B5 | `??1?$vector@KV?$allocator@K@std@@@std@@QAE@XZ` | 0x00D0F23D | 0x00BFEA14 | Binding remains unconfirmed |
| +0x3C4 | `?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ` | 0x00D0F24F | 0x00CB7940 | reloc_bindings |
| +0x3DB | `?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ` | 0x00D0F2FA | 0x00CB7940 | reloc_bindings |
| +0x3EC | `??0CBaseObject@@IAE@XZ` | 0x00D0F30B | 0x0099A380 | reloc_bindings |
| +0x3F3 | `??_C@_00CNPNBAHC@?$AA@` | 0x00D0F312 | 0x0122D70E | literal payload prefix |
| +0x3FF | `??_7CScriptGameResourceObjectMovieBase@@6B@` | Unpaired | Unpaired | Instruction sequence differs |
| +0x40C | `??0CCharString@@QAE@PBDJ@Z` | 0x00D0F32B | 0x0099EBF0 | reloc_bindings |
| +0x42A | `??1CCharString@@QAE@XZ` | 0x00D0F349 | 0x0099EAE0 | reloc_bindings |
| +0x442 | `??0CBaseObject@@IAE@XZ` | 0x00D0F361 | 0x0099A380 | reloc_bindings |
| +0x453 | `??_7CScriptGameResourceObjectScriptedThingBase@@6B@` | Unpaired | Unpaired | Instruction sequence differs |
| +0x4B1 | `??0?$map@VCCharString@@VCScriptGameResourceObjectScriptedThingBase@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@4@@std@@QAE@XZ` | 0x00D0F3D0 | 0x00CDBF70 | reloc_bindings |
| +0x4B8 | `??_C@_0BK@FHOKBAMK@CS_BANDITCAMP_ALARM_OUTER?$AA@` | 0x00D0F3D7 | 0x012C9AE8 | terminated literal payload |
| +0x4C1 | `??0CCharString@@QAE@PBDJ@Z` | 0x00D0F3E0 | 0x0099EBF0 | reloc_bindings |
| +0x4D3 | `?RunCutsceneMacro@@YIXABVCCharString@@AAV?$map@VCCharString@@VCScriptGameResourceObjectScriptedThingBase@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@4@@std@@PAV?$map@VCCharString@@_NU?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@_N@std@@@3@@3@PAV?$map@VCCharString@@V1@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@V1@@std@@@3@@3@_N4@Z` | 0x00D0F3F2 | 0x00CBFB7D | reloc_bindings |
| +0x4DC | `??1CCharString@@QAE@XZ` | 0x00D0F3FB | 0x0099EAE0 | reloc_bindings |
| +0x4E3 | `??_C@_0M@EJJBBODP@Gate2Guard1?$AA@` | 0x00D0F402 | 0x012C8A74 | terminated literal payload |
| +0x4EC | `??0CCharString@@QAE@PBDJ@Z` | 0x00D0F40B | 0x0099EBF0 | reloc_bindings |
| +0x51C | `??_7CScriptThing@@6B@` | Unpaired | Unpaired | Instruction sequence differs |
| +0x538 | `??3@YAXPAX@Z` | 0x00D0F436 | 0x004AA840 | Binding remains unconfirmed |
| +0x54C | `??1CBaseClass@@UAE@XZ` | 0x00D0F43F | 0x0099EAE0 | Binding remains unconfirmed |
| +0x555 | `??1CCharString@@QAE@XZ` | 0x00D0F3FB | 0x0099EAE0 | reloc_bindings (nearby instruction) |
| +0x56C | `?_M_erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@VCScriptGameResourceObjectScriptedThingBase@@@std@@@2@@Z` | 0x00D0F448 | 0x00CDBFB0 | Binding remains unconfirmed |
| +0x593 | `_free` | 0x00D0F451 | 0x007E74D0 | Binding remains unconfirmed |
| +0x59F | `??1CScriptGameResourceObjectScriptedThingBase@@UAE@XZ` | Unpaired | Unpaired | Instruction sequence differs |
| +0x5B3 | `??1?$CCountedPointer@VCScriptGameResourceObjectMovieBase@@@@QAE@XZ` | 0x00D0F465 | 0x006E7AB0 | reloc_bindings |
| +0x5BF | `??_7CScriptGameResourceObjectBase@@6B@` | Unpaired | Unpaired | Instruction sequence differs |
| +0x5C4 | `??1CBaseObject@@MAE@XZ` | 0x00D0F476 | 0x0099A430 | reloc_bindings |
| +0x5CB | `??_C@_0BF@KJCONHPN@DefensiveGuardBandit?$AA@` | 0x00D0F47D | 0x012C9AD0 | terminated literal payload |
| +0x5E4 | `??0CCharString@@QAE@PBDJ@Z` | 0x00D0F496 | 0x0099EBF0 | reloc_bindings |
| +0x602 | `??1CCharString@@QAE@XZ` | 0x00D0F43F | 0x0099EAE0 | reloc_bindings (nearby instruction) |
| +0x627 | `?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ` | Unpaired | Unpaired | Instruction sequence differs |
| +0x67F | `?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ` | 0x00D0F531 | 0x00CB7940 | reloc_bindings |
| +0x68E | `??_C@_00CNPNBAHC@?$AA@` | 0x00D0F540 | 0x0122D70E | literal payload prefix |
| +0x697 | `??0CCharString@@QAE@PBDJ@Z` | Unpaired | Unpaired | Instruction sequence differs |
| +0x69E | `??_C@_0BF@LPJACHIF@DefensiveGuardLeader?$AA@` | Unpaired | Unpaired | Instruction sequence differs |
| +0x6A7 | `??0CCharString@@QAE@PBDJ@Z` | 0x00D0F549 | 0x0099EBF0 | reloc_bindings |
| +0x6AE | `??_C@_0BO@EEDJCEMH@CREATURE_BANDIT_LEADER_LEVEL2?$AA@` | 0x00D0F550 | 0x012C9AB8 | Binding remains unconfirmed |
| +0x6B7 | `??0CCharString@@QAE@PBDJ@Z` | 0x00D0F559 | 0x0099EBF0 | reloc_bindings |
| +0x6FD | `??_7CScriptThing@@6B@` | Unpaired | Unpaired | Instruction sequence differs |
| +0x71D | `??3@YAXPAX@Z` | 0x00D0F5AD | 0x004AA840 | Binding remains unconfirmed |
| +0x731 | `??1CBaseClass@@UAE@XZ` | 0x00D0F5B6 | 0x0099EAE0 | Binding remains unconfirmed |
| +0x73A | `??1CCharString@@QAE@XZ` | 0x00D0F5BF | 0x0099EAE0 | reloc_bindings |
| +0x743 | `??1CCharString@@QAE@XZ` | 0x00D0F5C8 | 0x0099EAE0 | reloc_bindings (nearby instruction) |
| +0x74C | `??1CCharString@@QAE@XZ` | 0x00D0F5C8 | 0x0099EAE0 | reloc_bindings |
| +0x78C | `??3@YAXPAX@Z` | 0x00D0F60B | 0x00BFEA14 | Binding remains unconfirmed |
| +0x7A0 | `??1CBaseClass@@UAE@XZ` | Unpaired | Unpaired | Instruction sequence differs |
| +0x7A9 | `??1?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@QAE@XZ` | 0x00D0F628 | 0x008AC970 | reloc_bindings (nearby instruction) |
| +0x7B2 | `??1?$vector@VCScriptThing@@V?$allocator@VCScriptThing@@@std@@@std@@QAE@XZ` | 0x00D0F61F | 0x008AC970 | reloc_bindings (nearby instruction) |
| +0x7BB | `??1?$vector@KV?$allocator@K@std@@@std@@QAE@XZ` | Unpaired | Unpaired | Instruction sequence differs |

61 supported pairs out of 82. See the separate manual audit where supplied.
