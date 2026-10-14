# Batch `2026-10-14_linux_hand24` — delegated hand-authoring chunk 24 (CCreatureAction_CastBattleCharge .. CCreatureAction_KrakenLowAttackInto): 40 of 40 landed

40 matched (payload.json / oracle.tsv; every row MATCH or RELOCATION_MATCH with behaviour PASS under VC7.1+Wine;
lander dry-run `verify_and_land.py payload.json oracle.tsv` = WINS 40/40, zero header warnings), 0 DIFFER, 0 skipped.
No flags/pragmas were needed anywhere. Generator: gen.py (sources src/, tests test/, grade.sh, runtests.py, log/).

## Coordinator notes
- oracle.tsv has the trailing int3 padding TRIMMED (33 of the 40 chunk rows carry padding, e.g. 0x8b9400 is 51 bytes of
  body in a 64-byte row). The lander compares the whole oracle row, so an untrimmed oracle reports DIFFER(51v64) for
  bodies that q.py (which strips padding itself) grades MATCH. Trim before landing any chunk oracle.
- `git status` showed `M docs/HANDOFF.md` and untracked `tools/decomp_pipeline/linux_lane/lane_tools/` (11:16-11:17 UTC,
  coordinator's own save of the lane tooling); this agent wrote nothing under the repo.

## Label corrections (manifest labels rejected; identities from rtti-vtables.tsv vtable stores / callers)
- `008cf990` "CastBattleCharge" -> CCreatureAction_TentacleStrikeLoop(CThingCreatureBase*, long) (vtable 0x1284EF4; members
  StartTick/Interrupt/StartHealth from the PDB at retail +0x114/+0x118/+0x11c).
- `00a445d0` "CastBattleCharge" -> CASuspendableProcess(bool, long unused, long) (vtable 0x129D428, base CBase 0x99A2F0; the
  second argument is never read; the timeslice is copied as a dword although the PDB says float).
- `008f4cb0` "CastTimeSpell" -> CCreatureAction_DragonFlight(owner, const CCharString&) (vtable 0x1288544).
- `009080b0` "DragonFlight" -> CCreatureAction_TrollProjectileAttack(owner, target, const CCharString&) (vtable 0x128E3AC;
  OverriddenAnim=1, AnimName copy-constructed in the init list).
- `006e4cb0` "CCreatureAction_JackFloatUp::Clone" -> `CRetailList_CActiveDialogueInfo::CreateNode(const CActiveDialogueInfo&)`:
  `this` unused, malloc(0x20), the 0x18-byte CActiveDialogueInfo (PDB: CIntelligentPointer<CThing> ThingSpeaking, SoundID,
  Layer, bool IsLayerHead, float OutputGain) placement-copied at +8 (null-checked), links left unset (SGI create-node
  shape). Callers 0x6E33E0 / 0x6E4E35.
- `0059aee5` "CCreatureAction_FireMissileWeapon::FrameUpdate" -> `_unattributed::PopValue` (class modelled as CBlockStack):
  unaligned 20-byte body in the 0x595000.. no-alignment module, 7 callers; `if (cursor != blockStart) --cursor; else
  PopBlock();` with the block restore @ 0x59AD35 (labelled "CTCCarryable::OnKill", also noise) as a tail jump. The source
  polarity matters: `== ... PopBlock() else` lays the jump out in the middle.
- Slot facts: CCombatActionBase FinishAction is slot 4, OnAnimationLoaded slot 68, Initialise slot 12, IsMeleeFlourish
  slot 21; AutoTurn IsToEvadeAllHits 45, IsToCauseRecoil 46, IsAbleToBeAttacked 47, IsToFaceTarget 73. 0x692940 is
  CCreatureActionBase::GetPWorld (GetPItem then 0x4C7AA0 = g_pGame->+0x24); 0x4C73D0 / 0x6A4D00 are CThing::GetPosition
  (`[this+0x60]+0xc`); the +0x60 component's slot 31 takes a C3DVector* (also seen in CTCCoopSpirit::SwapToHero).
- CMeleeCombatAbilityDef retail offsets: EvadeAllHits +0x64, CauseRecoil +0x65 (PDB +0x6c/+0x6d; its CDefString and
  vector members are 4 bytes narrower in retail).

## Shapes learned (beyond GOTCHAS)
- Zero-register cost model in constructors: 4+ zero stores (2 sound-pair + 2 members, one after the base call) get a
  callee-saved zero register (ebx/edi); 2 before the call get eax; 2 before + 1 after (DragonFlight 0x8f4d00) are all
  immediates. A float member zeroed in such an init list reproduces with `TurnRate(0)` on a `float` (no FPU, shares
  the register) -- the "float zero is an immediate" rule is about the literal, not the member type.
- A `const CCharString&` argument is pushed from its slot; a by-value CCharString parameter (0x87cf90) is addressed with
  `lea` in the argument area and its out-of-line destructor runs before the return.
- A bool predicate that merges two early `return false` into one tail is `if (A() || B()) return false;`, and the
  inverted counted-pointer null test without `xor eax,eax` is `bool has = ...GetPItem() != 0; return has ? false : true;`
  (0x87c8b0). `return byte ? true : false;` gives the frameless `test cl,cl ; setne al` (0x87c900).
- An argument evaluated BEFORE an immediate push (`call GetPItem ; push 2.0f ; call GetPosition`) is a named local
  (0x8445a0); a vcall object expression evaluated before the arguments is a named local too (0x842f70: the manager into
  ebp before the eight pushes).
- `call()->m_p60->vf(arg)` loads the object straight into ecx (`mov ecx,[eax+60h] ; mov eax,[ecx]`) only when the call
  result is first stored in a named local (`CThingCreatureBase* p = PCreature.GetPItem(); p->m_pPhysics->vf(..)`);
  the inline chain gives `mov eax,[eax+60h] ; mov edx,[eax] ; .. ; mov ecx,eax` (0x8f2f00, 3 attempts).
- Implicit memberwise copies of floats are register moves: model such members as `long` in copy ctors/Clone (ShieldHealth,
  OutputGain, TimesliceLength, StartHeight) -- a `float` would go through the FPU.
- `ActionTime = (long)(TotalTime * 0.3f)` is `fild ; fmul [pool] ; call __ftol2` (0x8f2c70); `TotalTime * 3 / 4` is the
  `lea [eax+eax*2] ; cdq ; and edx,3 ; add ; sar 2` sequence (0x923cd0).
- A `bool` parameter stored into an `unsigned char` member via the init list reproduces the `mov al,[esp+..] ; mov [esi+6],al`
  copy (CASuspendableProcess); a 4th constructor argument that is never read still costs its `ret 10h` slot (0x8bcd60).
