# Hand chunk 2034 (CAIStateGroup_ScorpionKing .. CEnginePrimitiveManagerParticleDecalGroup): 0 of 2 byte-exact

## What matched (0)
None. 0x0097BDA0 is parked at DIFFER(2025v2009) with an identical call sequence and control flow; 0x00BB8750 was not started.

## Parked for Fable 5.1 (1)
- 0x0097BDA0 CAIStateGroup_ScorpionKing::UpdateCombat DIFFER(2025v2009): near-range arm: ours keeps battle_def in ebx (split piece #46,
  c2 prio 18) and reloads boss from [esp+34h] twice; retail keeps boss in ebx (0x0097BFD9 `mov ebx,[esp+18h]`, 0x0097C1C9 `mov ecx,ebx`)
  and reads the def as a transient (0x0097C160 `mov eax,[esp+10h]`), so retail's near Terminate path is `mov ecx,[esp+10h] ; jmp 0x0097BFA3`
  (shares the first release) where ours keeps a full `dec [ebx+4]` release + epilogue (+16 bytes); frame: retail {boss, radius} [esp+18h],
  {r, factor} [esp+28h], facing temporary [esp+2Ch]; ours {r, factor, radius} [esp+18h], facing [esp+28h], boss [esp+34h]; small
  schedules: `fstp st(0)` one instruction early (0x0097BE7E), far-arm `mov byte [eax+20h],1` early (retail after `mov ecx,ebp`,
  0x0097C1E2), `fld [esi+3Ch]` before `mov ecx,[esi+4]` at both tail-strike tests (0x0097C15D / 0x0097C237), shield GetTC key `lea` in ecx
  (retail eax, 0x0097C042); the loop pad is `lea ecx,[ecx]` v `lea esp,[esp]` only because of the 16-byte shift
  | tried: Charge() forced inline (its ChargeInto dtor then expands at f 1.0, retail calls 0x0097C900) and plain (c2 score 2348 v T74:
  never expanded) -> open-coded charge; FPS in the do-while condition (FPS loaded into edi before GetFrame, no aligned loop head) ->
  named frames_waited assigned at the end of the body (matches the loop); far test `HL - D < Health` / `Health > HL - D` (test ah,5 ; jp)
  -> `!(Health <= HL - D)` (test ah,1 ; je, as retail); GetSquaredMagnitude XYZ, GFFastOneOverSqrt NEWTON_PAREN / ORIGINAL_UNION,
  NORMALISE_SQUARE_HELPER (no change); free c2 probes (c2prio / frame packer, not graded): a named result at function scope, const-ref
  facing, SetCounting after the dot, Reset before the HL store, a named near-arm boss copy (copy-propagated away), boss declared at
  function top, `radius += 0.2f`, the operator* parameter temporary for the radius, the default (non-EXPANDED) GetTC: none moves the
  boss near piece (#19, 2 refs, prio -15: +8 in block 16 (K 4), +6 in block 53 (K 3), -28 over nine live-through side-attack blocks) or
  boss's frame weight (5). Diagnostic (not delivered): deleting one boss use (the far ResetStrikesSinceHitHero) gives boss weight 4 and
  reproduces retail's frame slots exactly ({r,factor} then {boss,radius}).
  | next: find the source shape that makes the near arm read boss ONCE in c2's IL (the frame packer then sees weight 4 and the near
  value becomes a 3-reference candidate that wins ebx over the def piece): dump c2's IL of the near arm (tuple list with the c2probe
  hooks) and compare a near-arm-only copy that survives copy propagation; also test the FableWin.pdb COMDAT order after UpdateCombat
  (SetCountingStrikesSinceHitHero, then the LEFT turn ctor before the RIGHT one, PrepareTailPlunge, CEventCreatureCharging), which
  suggests the source names the left turn first; the Left-first if/else spellings probed so far lay the Left arm out first (retail has
  Right as the fall-through, `test ah,41h ; jne Left`).

## Not attempted (1)
- 0x00BB8750 CEnginePrimitiveManagerParticleDecalGroup::RenderLandscapeDecalGroup (1951 bytes): the optional second row, not started
  (first row not matched, context budget). No source written and the body was not decoded (no outline claimed). Hand-backs to start from:
  pending/2026-11-12_acctB_{c2028,r3013}/ and 2026-11-11_acctB_{c2008,c2011,c2016}/.

## Skipped
None.

## Names (two sources each; see LABELS.tsv)
- Row: ?UpdateCombat@CAIStateGroup_ScorpionKing@@IAEXXZ (Ego_r + FableWin; donor order = retail order; landed Update calls it).
- Callees: see the row's header comment; new spellings proven here: CTCBoss::ResetStrikesSinceHitHero 0x007AED50,
  CTCBoss::SetCountingStrikesSinceHitHero (inline, FableWin), CAIStateGroupBase::AIAction_PumpCombatActions 0x0097D980,
  CThingCreatureBase::GetCombatTypeDef const 0x00666D10, CTCPhysicalShieldSpell::AllowReactionsOnHitFromAttacker 0x0079E580,
  NEntityEvents::CEventCreatureCharging (inline ctor), CScorpionKingBattleDef members (Ego_r TPI).
- Descriptive local names only (no made-up function names). The dead `C3DVector charge_start = GetCreature().GetPos();` local stands
  for the unused GetPos call retail makes before the charging event (0x0097C2BD); its real use (if any) is unknown.

## Header debt
- engine/CAIStateGroup_ScorpionKing.h, CCreatureAction_Scorpion*.h in the repo are generated flat layouts: retail classes added as
  opt-ins in hdr/ (see hdr/README.md). The landed 0x008C4E20 ChargeInto ctor and 0x0097C5E0 Update still use local models.

## Label corrections
- 0x0097C580 is UpdatePhase2, not SpawnBabies (NAME_FIXES.tsv); 0x007AED50 is ResetStrikesSinceHitHero; 0x0097C900 is the ChargeInto
  implicit destructor; 0x0097D980 belongs to CAIStateGroupBase.

## Continuation (agent 2034, pass 2) -- measurements
Tools added (all FREE diagnostics, none graded): tools/pr.sh <src> (compile-only, 1 s: size + near-arm head, `ebx=` check via
tools/batch.py <base> <variants.py>), tools/pb.sh <src> <tag> (c2prio_b trace with first/last piece blocks -> tmp/<tag>.prio/.json),
tools/bp.py <tag> (boss / battle_def pieces, prio, register, first/last block lines), tools/uid.py <tag> (creation ordinal of the
boss pieces), tools/pf.sh <src> <tag> <uid=prio> (hand883 c2pf: FORCE a candidate's priority at insertion, keeps the .obj),
tools/fw.sh <src> <tag> [uid=prio] (forced what-if + relocation-masked md.py summary; default 179=20 = boss near piece),
tools/c2pf/ (copy of pending/2026-11-08_linux_hand883/tools c2prio.py + c2dbg.py), tools/t13/ (copy of agent3011 t13 tools),
tools/pblk.py (hand883, per-pass block contributions; NOTE its "live" label is wrong for flag 16 = a referencing block).

1. The predecessor's piece labels were swapped: #46 is the FAR battle_def piece (lines 193..212, prio 18, ebx = retail
   0x0097C224) and #45 is the NEAR one (173..177, prio 13, refs 5: the DamageToCauseTailStrike read + the tail-plunge Terminate
   release whose null test is gone). The boss near piece is #19 (uid 179): +6 (SetCounting block 139-142, K3) +6 (Reset block
   179-184, K3) -27 (live through the turn attack / LookAt / tail action blocks) = -15. #45 = +6 (173) -5 (174-175) +8 +4 = 13.
   #45's only allowed register is ebx (its ebx cost 200 = boss piece's spill 2 x 100), so it takes ebx and boss stays in memory.
2. PROOF that this single decision is the whole allocator/frame residue: forcing the boss near piece (uid 179) to prio >= 13 (13
   ties and wins on key 54 v 2), or #45 (uid 184) to <= -15, gives 2009 bytes, retail's near arm (`mov ebx,[esp+18h]`, near
   Terminate `mov ecx,[esp+10h] ; jmp 0x0097BFA3`, #45 split into {173} dropped + {177} ecx) AND retail's frame exactly (no frame
   offset differs): md 611/624 with relocations masked, frame NOT masked (tmp/w0.md). So residue (2) is a consequence of (1):
   the frame weight of boss is post-allocation (5 -> 4 when the near piece is in ebx); the predecessor's delete-a-use diagnostic
   only mimicked it.
3. Remaining with the decision forced (fw.sh, 13 differing instructions): (a) `fstp st(0)` early at 0x0097BE7E; (b) near facing dot
   term order X,Z,Y v retail Z,Y,X (0x0097BFF7..0x0097C011) and operand order (retail loads to_enemy.Y / .X, facing.Z; ours loads
   facing.* in all three) and the side dot operand at 0x0097C092; (c) near tail compare: ours `mov eax,[esp+10h] ; fld [esi+3Ch] ;
   fsub ; mov ecx,[esi+4]` v retail `mov ecx,[esi+4] ; fld ; mov eax,[esp+10h] ; fsub` (0x0097C15A); far tail compare `fld` before
   `mov ecx,[esi+4]` (0x0097C234). These were hidden by the predecessor's frame-masked count.
4. `const C3DVector& facing = GetCreature().GetPhysicsTC()->GetFacingVector();` (natural, lifetime-extended temporary) fixes the
   near dot TERM order to retail's Z,Y,X (operand order still facing-first); unforced it changes nothing else (compile-only diff:
   only those 3 instructions move). `to_enemy.Dot(facing)` compiles identically to `facing.Dot(to_enemy)` (both with and without
   the reference). Dead-enum-local sweeps k=1..7 (r3004 lever, diagnostic) move the term order but never the operand order.
5. Measured NOT to move the boss-near / #45 decision (compile-only `ebx=` check and/or c2prio): Left-first turn forms
   (`<= 0.0f`, `!(... > 0.0f)` -> `jp`/`je` with Left laid out first; ternary also Left first), IsStillRunnable if/else around the
   tail-plunge success (T3: 2009 bytes by coincidence, layout changes), nested LookAt braces, `result` at near-arm top,
   pump/side swap (`if (!(facing_dot > 0.5f))`), HasEnemyBeenKnockedDown open-coded, radius inline, `else if` result return,
   far `if (result != F) {} else`, `GetCreature().GetPos();` statement for charge_start, boss declared at function scope,
   CDefPointer RELEASE_POINTEE (identical object) / RELEASE_COUNTREF, `CTCBoss& near_boss = *boss` (propagated), the QFE-4035 c2,
   forcing #33 before #69 (the first ebx tie at prio 28). Diagnostics that DO give boss ebx (not deliverable): dA (near damage read
   replaced by a constant: #45 disappears), D2 (near tail-plunge not-runnable path falls to the final check instead of returning:
   the 177 release disappears). T4 (`result == FINISHED && health` combined; NOT equivalent) merges the near battle_def piece with
   the final region (172..244, prio 63 -> ebp).
6. Arithmetic: with our near-arm IL the gap is 28 priority points (boss near needs +28 or #45 -28); K tweaks from named locals or
   constants move it by at most ~10, so retail's near-arm IL must differ structurally (fewer live-through blocks for the boss
   piece, an extra boss reference in each of its two blocks with ~K 7, or the 177 release / 173 read not forming one ebx piece).
Grades: g09 = probe/g09.cpp (= differ + `const C3DVector& facing`): DIFFER(2025v2009), md 532/624 unmasked, 561/624 frame-masked
(same as before; the term-order gain is hidden by frame masking).
7. More measured-and-rejected (compile-only, 1 s each): named turn-attack locals (`CCreatureAction_... attack(...)` per arm: size
   0x7EB, no ebx), a scoped named PrepareTailPlunge local (size 0x7AB, layout changes, no ebx), operand-swapped tail compares
   `HL - D >= Health` near and far (worse: 607/624 forced), `to_enemy.Dot(facing)`.
8. Charge(): FableWin.pdb places ?Charge@CAIStateGroup_ScorpionKing@@IAE?AW4EAIActionReturn@@ABVC3DVector@@M@Z (0x60 bytes, a
   regular function) right after UpdateCombat's COMDAT run, and CEventCreatureCharging's COMDATs sit BEFORE it, so the event is
   created in UpdateCombat itself; HasEnemyBeenKnockedDown also follows UpdateCombat in FableWin yet retail expands it, so a later
   definition does not stop VC7.1 expanding it. The charge-site GetPos (0x0097C2BD) may be Charge's unused `const C3DVector&`
   argument -- but it sits BEFORE the event, so the event would have to be inside Charge, which the FableWin COMDAT order argues
   against. Left as the descriptive dead local (an unused call result).

## Ranked untried ideas (resume here)
1. Find the IL shape behind retail's allocator decision (target: boss near piece colours before the battle_def near piece; test
   each spelling with `bash tools/pr.sh <src>` -> near head `mov ebx,DWORD PTR [esp+0x18]`, then tools/fw.sh for the rest):
   a. split-region formation: c2.dll 0x1078B51C..0x1078B625 builds piece regions; hook it (as hand883 TRACE.md S2 planned) and dump
      the region bitsets of #84's split at the second split pass (ev 725) to see why the 173 read and the 177 release form one piece
      (#45) in ours; retail needs them apart (they become {173} dropped + {177} ecx) or #45 coloured after the boss piece.
   b. the far charge path's first-split trigger (#69 GetPItem result -> ebx, prio 28 tie with #33 broken by key 535 v 132) decides
      the first battle_def cut; structure variants of the far charge path / final code change which blocks #84 keeps (T4 showed
      the near piece can merge with the final region) -- look for an EQUIVALENT spelling that puts a pre-coloured callee-saved
      holder between the 173 read and the 177 release, or merges #45 with a region whose ebx is taken.
   c. any construct that adds ~K7 boss references inside the SetCounting / Reset blocks without code (none found).
2. Near dot operand order (forced build: retail loads to_enemy.Y / to_enemy.X and facing.Z): record-id lever (r3005 tools
   /opt/fable_lane/w22/agent3005/tools/c2h/), with the const-ref facing kept.
3. Scheduling: fstp st(0) at 0x0097BE7E; near/far tail-compare load order (0x0097C15A / 0x0097C234): /opt/fable_lane/w22/agent2031/
   tools/sched.sh + skyfit.py, only after 1.
