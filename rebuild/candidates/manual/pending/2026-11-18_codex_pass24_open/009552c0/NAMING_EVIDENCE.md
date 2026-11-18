# 0x009552C0 naming evidence

Ego_r and FableWin agree on `?Update@CAIStateGroup_MazeShieldedChase@@UAEXXZ`. Ego_d puts Update in `ai_state_group_maze.obj` beside IsRunnable, IsStillRunnable, OnActivate, Cleanup, and the WillEnflameInto constructor. It identifies ret as EAIActionReturn, nav as bool, pthing_creature as CThingCreatureBase*, and p_maze_def as CDefPointer<const CMazeBattleDef>. The rebuilt object emits the exact Update name.

| Retail address | Original name | Evidence |
|---|---|---|
| 0x008C3510 | CDefClassBase::GetSubDef<CMazeBattleDef> | Both PDBs give the exact specialization. The Ego_d local and native call site at 0x009552E4 agree. The shared template body replaces the supplied stand-in. |
| 0x00954740 | CAIStateGroup_MazeShieldedChase::IsStillRunnable | Ego_r 0x00979200 and FableWin 0x02BF6CF0 give the protected non-virtual member. Ego_d TU order and retail signatures agree. |
| 0x00954130 | CAIStateGroup_MazeBase::HasEnemyBeenKnockedDown | Ego_r 0x009788E0 and FableWin 0x02BF7440 give the protected non-virtual member. The knockdown check at 0x00955572 uses it. |
| 0x009567F0 | CCombatAction_WillEnflameInto constructor | Ego_r 0x0097B3F0 and FableWin 0x02BF7F40 agree on CThingCreatureBase& and long. The constructor writes the expected vtable and Level. |
| 0x008BCA00 | CCreatureAction_CastWillLightningSpell constructor | Ego_r 0x008F5020 and FableWin 0x02A1E410 agree on CThingCreatureBase&, long, float, CThing&. The retail constructor is present at that address. |
| 0x00906AC0 | CCreatureAction_CombatIdle constructor | Ego_r 0x0092D680 and FableWin 0x02B3EBA0 agree on the two creature references. The native constructor installs vtable 0x012632B4. |
| 0x00906B00 | CCreatureAction_CombatBoast constructor | Ego_r 0x0092D6C0 and FableWin 0x02B3F890 agree on the two creature references. The native constructor installs vtable 0x012633FC. |
| 0x0062E900 | CCombatActionBase destructor | Established binding and the landed destructor body. The native callers destroy their completed combat-action temporaries through it. |

FableWin 0x02706A30 gives the mutable-source converting intelligent-pointer constructor. The production method uses that shared implementation. VC7.1 inlines the move argument construction and retains the look argument call. The native look call targets 0x007C2930.

The visible GetSubDef specialization was graded independently at its native 182-byte extent. All four relocations restore byte identity. The helper receipt lists the literal and three exact call targets in `SAME_TU_HELPERS.json`.
