# 0x007C0AD0 naming evidence

The selected RTM object emits `?CreateShot@NThingShot@@YIPAVCThing@@JPAV2@0PBV2@MMMABV?$CDefPointer@$$CBVCMeleeCombatAbilityDef@@@@PBVC3DVector@@PBVCRightHandedSet@@@Z`. Ego_r donor 0x007BF280 and FableWin donor 0x026F98A0 carries this exact decorated spelling.
The Ego_d module record identifies `NThingShot::CreateShot` in `tc_shot_definition_base.obj`. NThingShot::CreateShot (debug length 3079).
Retail directly calls this member at 0x00787D1A in landed 0x00787CA5 CTCProjectileTrap::TriggerTrap.
Retail directly calls this member at 0x007C357A.
The parked code has a verified self leaf. The remaining differences prevent a full relocation identity audit.
