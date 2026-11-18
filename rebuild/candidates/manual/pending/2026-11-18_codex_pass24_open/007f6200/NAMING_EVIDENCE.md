# CTCTargetingAI::UpdatePBestEnemyCreatureForAttack

The VC7.1 object exports `?UpdatePBestEnemyCreatureForAttack@CTCTargetingAI@@QAEX_N@Z`. The source-order record in Ego_d identifies `CTCTargetingAI::UpdatePBestEnemyCreatureForAttack`. The submitted extent is 1458 bytes.

- egor donor procedure at 007f15c0
- fablewin donor procedure at 02535510

- call at 0x007F6ACE in landed 0x007F69B0 CTCTargetingAI::FrameUpdate
- call at 0x00864D89 in landed 0x00864D00 NCreatureAI::IsToAttackTarget
- call at 0x0093D6F7 in landed 0x0093D580 CAIStateGroup_MeleeCombatNearbyBase::RequestMoney

The old enemy and receiver occupy opposite frame words near 007F621C. The later component-selection paths retain additional differences.

The selected-body fixture enumerates targeting flags and records target updates; its dependencies are controlled.
