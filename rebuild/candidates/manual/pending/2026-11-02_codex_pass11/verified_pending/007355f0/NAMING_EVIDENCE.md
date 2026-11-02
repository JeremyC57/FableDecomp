# 007355f0 naming and layout evidence

Identity: both original name sets identify `?SetCombatType@CTCCombat@@QAEXABV?$CDefPointer@$$CBVCCombatTypeDef@@@@@Z` (Ego_r0074e5a0/FableWin02502920). Native callers00664b6a and0073655a pass a reference to a CDefPointer<const CCombatTypeDef>; native2525-byte span ends00735fcd with ret4, no padding/data. Full original Ego_r type25a80 and module252 `release\tc_combat.obj` independently prove the receiver, access, const template argument and original parameters/locals.

Recovered exact local spellings: pcombat_type, combat_type, using_xombat_wheel (the original typo), combat_man, and all nine separate pdef variables. Other locals are descriptive author choices, not claimed original recovery.

The nine definition types and order are in original module locals, GetSubDef instantiation name-set entries and native calls7380d0/7381f0/7382b0/738370/738430/7384f0/7385b0/6f0f60/6696b0. Native definitions all store their original CombatAbility member at+28. The first four copy CCombatAbilityBlockingData (native allocation30, ctor739760), next three CCombatAbilityMeleeAttackData (20,ctor737370), then CCombatAbilityStrafingData(1c) and CCombatAbilityProjectileAttackData(20). Native vtables1232484/12324ac/12324c0 plus RTTI and PDB names establish the last inline base/derived copies.

CCombatAbilityData original24-byte layout is PercentageChance+4,SecondsDuration+8,SecondsToWaitBeforeRepeat+c,Anim+10,private InterfaceType+14. GetCombatAbilityDataType is virtualslot3 (PDB+native call[vptr+c]). BlockingData is48 bytes in native because its original set is12 rather than donor16. MeleeData32,StrafingData28,ProjectileData32.

CTCCombat original full native152-byte layout: CombatManager reference+20,ParentCreature pointer+24,PCombatTypeDef+28,CombatAbilities+70. Native ctor/dtor and selected row member operands confirm original donor fields after substituting true12-byte STLport containers. Header default branch remains untouched under FABLE_RETAIL_COMBAT_TYPE.

Original CCombatantInfo fields Parent+0,AttackerInfo+4,DefenderInfo+34 (native full112bytes). AttackerInfo native48 bytes; Attacked+0 andUsingCombatWheel+1c give selected-row accesses+4 and+20. PDB original type plus native register calls/loads corroborate. CCombatManager original baseCBaseClassNonCopyable, maps12 rather than donor16 and DialogueManager36 rather than40 give92bytes. Only original public const GetCombatantInfo overload is declared: native6e9360 is first twin, matching donor const-first ordering;6e9390 is private nonconst twin.

Callees661e80 IsInMelee const and662410 IsInMeleeAsAttacker const are exact PDB members, read original+15c flag bits and are already corrected in NAME_FIXES_QUEUE. DeregisterCombatant6ed9c0 andRegisterCombatant6edc50 take actual creature references, not fake pointer/class variants. Existing wrong landed spellings will be recorded in NAME_FIXES.tsv.

Container names and bodies are adapted from original STLport4.6.2 `_map.h`, `_tree.h`, `_pair.h`; original namespace,class keys,const key type,iterator return and access are retained. No catalog labels are used as proof. Full raw evidence stays local under evidence/007355f0; final handoff excludes original PDB/game bytes.
