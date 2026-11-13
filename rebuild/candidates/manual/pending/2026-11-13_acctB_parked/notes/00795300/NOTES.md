# 0x00795300 CTCUnholyPower::FrameUpdate (9268 bytes) -- first authoring pass (rapid fire), DIFFER

Best: differ/00795300.cpp (= work/v7.cpp) DIFFER(9571v9268). Whole function written (every block); call sequence already matches
retail except the inline-budget sites below. Grades: 11 (ATTEMPTS.log). Identity: CTCUnholyPower::FrameUpdate (vtable 0x0126BFDC
slot 9, rtti-vtables.tsv); manifest label CTCDivineWrath::FrameUpdate is wrong (twin 0x0078FD60).

What moved it: shared headers + overlay opt-ins (hdr/README.md); dropping CVECTORMAP_INLINE_LOWERBOUND (retail calls LowerBound
0x0040F020 at every GetTC); inline GetCurrentButtonHeldState; implicit PerEffectInfo/PerCreatureInfo dtors; IP ASSIGN body
(op= 0x00668B80/0x004A9760 called at the cold map sites, expanded in the effect refresh -- c2 decides it by itself); map sweeps written
as if/else (one shared ~creature per iteration, like retail's 0x7963DC / 0x796EC3 join); GETTC_ORIGINAL_CHAIN instead of
GETTC_EXPANDED (retail-like call split in the spell-over block: the ResetCreatureHitState copy now out of line).
What did not move it: C2DBoxI ctor spellings (3 variants, box store order residue stays); /d2inlT57 / T52 (uniform threshold
changes the early bitset tests too -- retail expands those); `if (!alive || health<=0) {..} else ++it` (identical code).

Inline-budget model (c2 trace, tools/inl/v6.txt, copy of agent2003's inl tool): default T=74; score = cost scaled by static
site frequency; a size penalty (~+31) starts when the function's accumulated size g passes ~9000. Retail needs (a) sweep-1
map::erase (ours sc 58 @ f 2.46) refused, (b) sweep-2 `++it` (ours sc 52 @ f 21) refused, (c) ~list after sweep 2 (sc 58) refused,
(d) the penalty already active at the first spell-over decision (ours g ~8400 there, penalty from ~9000). So retail's source puts
~600-800 more inline-IR before the spell-over loop than ours, or the sites have lower frequencies.
Other residues: frame 0x240 v 0x230 (this at [esp+38h] in retail, ours +0x50); 2nd cancel sound (stamina path) retail calls
CDefString::GetString 0x00415DD0, ours ??B (explicit .GetString() lost the tail merge); GetPItem order around the OnHit vcalls.

Best next ideas: (1) find the extra pre-spell-over IR: candidates are a K (kill/explode) helper or extra temporaries in the K blocks,
the victim-effect blocks as an inline member, C3DVector temporaries in the dir/hit-position code; rerun the trace
(tools/inl/inl_host.py, see its use in this file's history) after each change and watch g at the first spell-over decision;
(2) frame: compare retail slot map in OUTLINE.md "Frame slots" with ours (work/ours.lst) -- scope the K-block locals
(murder CDefPointer, creature_effects, effect) and the hit/dir locals as retail reuses them; (3) the stamina-path cancel sound.
Tools left: work/cc.py (full error list), work/calls.py + work/cmp.py (ordered call-sequence diff v retail), tools/inl (trace).
Made-up names: none (local classes without shared headers: CTCDRumble, CTCNymph, CThingFilter_CanBeHitByEpicSpell,
NEntityEvents::CEventHitBySpecialAbility / CEventKilledCreature / CEventDied, CMessageEventDef -- PDB names, minimal declarations).
Flags: none yet.

## attempt 2 (rapid fire) -- progress notes
- Inline split is a FREQUENCY fact (c2 regime-0 score sc = c1*(1/f+6)/7, T 74; pen = sz*(a+g)/18000 when a+g>9000).
  Loop forms decide f: `for(..;..;++x)` keeps code after it at 5x, `while`/`do` drop back, sweep `for(;;)` w/o increment = 9x body.
  R1 (= differ now): objects loop `while`, creature-search-2 `for` with gotos->continue, effect refresh `while`,
  sweep erase as `next=it; ++next; erase(it); it=next;`, sweep-1 alive test `!(creature.Get() && !creature->IsDead())`,
  sweep-2 keep test `creature.Get() && !creature->IsDead() && !(health<=0)` -> every release-phase call/expand split = retail.
- Still open: spell-over needs a+g > 9000 at its first bitset test (ours ~8550; K1's last test at 7668 must stay <= 9000)
  AND loop body f <= ~0.2 (ours 0.3125). Probe P1 (+20 dummy C3DVector copies, +500 g) confirmed pen fixes test/Reset-copy, not the IP copy.
  K2's `C3DVector effect_pos = GetPos()` must not be a copy-ctor site in retail (pen would refuse it): spell `p; p = GetPos();`?
- T1/T2 (w2/): spell-over wrapped in `if (DurationOfSpellFrames <= 0) {...}` after the release `return` (code-identical: c2 folds
  the re-test) halves the spell-over f (body 0.156) = what retail needs. FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE makes the
  spell-over loop-top copy the template ctor `??$?0VCThingCreatureBase@@@?$CIntelligentPointer...@QAE@AAV0@@Z` and the
  ResetCreatureHitState arg the copy ctor: two symbols = retail's two byte-identical COMDATs 0x0078E5B0 / 0x006EF410.
- PROBE P3 (T2 + 24 dummy `C3DVector pv(owner->GetPhysicsTC()->GetPosition())` before the spell-over, +600 g, and the K-block
  `C3DVector effect_pos; effect_pos = creature->GetPos();`) reproduces retail's WHOLE call list except the stamina-path
  GetString. So the only structural gap left: retail has ~550..1300 more inline IL (g) before the spell-over than ours, with no
  code (K1's last bitset test must stay a+g <= 9000: ours 7673+15). Screened, no effect: GETTC_EXPANDED (-500 g), LIST_CONSTRUCT_HELPER,
  HASTC_FORCEINLINE, ASSIGN_ORIGINAL_CHAIN, __assume in IP operator-> (+86 only). fwsize shows our helper texts = FableWin except
  C3DVector operator- (28i v 34i), Normalise (29i v 48i), IsDead (14i v 10i), GetPBaseDef (13i v 17i), GetString (15i v 19i).
- U4/U5 (= differ now): + FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND (FableWin Find text with the Compare member: +252 g, code-neutral)
  + FABLE_RETAIL_C3DVECTOR_SUBTRACT_NAMED_RESULT (overlay opt-in, +92 g). g at spell-over start 8396 -> 8740; needed ~8920
  (loop-top template IP copy a=10 must see a+g>9000). Explicit `.GetString()` on the stamina-path cancel sound gives retail's
  CDefString::GetString call but loses the PlaySoundW tail merge (frame +0x10): unsolved (U6/U7).
- Best next idea: find the last ~+200 code-neutral IL before the spell-over (other FableWin-original helper texts: Normalise is
  48i in FableWin v 29i ours, GetPBaseDef 17i v 13i, CDefString::GetString 19i v 15i; STLport 4.6 _STLP_alloc_proxy rebind layer
  FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND from pending 2026-11-12_acctB_f4007 hdr); then spell K's effect position as
  `C3DVector effect_pos; effect_pos = creature->GetPos();` (K2's copy must not be a scored copy-ctor site once pen is on: probe P3).
  Measure each with w2/tr.sh <name> + w2/gsum.py (g at K1's last test must stay <= ~8985).

## attempt 2 -- final state (DIFFER 9494v9268, differ/00795300.cpp = w2/U14.cpp)
Grades this attempt: ~30 (ATTEMPTS.log) + ~25 c2 inline traces (w2/tr.sh <name> -> w2/<name>.json; w2/gsum.py prints g at K1's last
expanded bitset test and at the spell-over start; w2/reg.py = per-region byte lengths between aligned calls, after work/calls.py+cmp.py).
What moved it (all code-neutral or retail-shaped; every release-phase call/expand split now = retail):
 1. loop forms (frequency): objects loop `while` + `++it` at the end, creature-search-2 `for` (continue via goto label), effect refresh `while`;
 2. sweep erase as `MapIt old_it = it; ++it; erase(old_it);` (prefix ++ c1 56 < postfix 66; `it` stored before the call as retail);
 3. sweep-1 test `!(creature.Get() && !creature->IsDead())`, sweep-2 keep test `creature.Get() && !creature->IsDead() && !(health<=0)`;
 4. `if (DurationOfSpellFrames > 0) {release; no return} else if (DurationOfSpellFrames <= 0) {spell over}` (re-test folded by c2;
    halves the spell-over f; release now falls through to the common ~effects like retail's jmp 0x0079771A);
 5. FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE (loop-top copy = template ctor, Reset arg = copy ctor: retail's 2 COMDATs);
 6. g raisers (code-neutral, retail texts): CVECTORMAP_ORIGINAL_FIND +252, C3DVECTOR_SUBTRACT_NAMED_RESULT (overlay) +92,
    STLP_ALLOC_PROXY_REBIND (overlay) +61, objects `IS_THING_ALIVE(object)` +15, `GetTC(&p)` TCI-less form for 6 TC classes +40;
 7. K blocks' effect position `C3DVector effect_pos; effect_pos = creature->GetPos();` (K2's copy-ctor site would be refused once pen is on).
Blocker: retail's c2 caller-size penalty is on at the spell-over head; ours turns on ~100-125 IL later (loop-top IP copy at g 8916+a 10,
needs > 9000). Probe P6 (U14 + 5 dummy `C3DVector pv(owner->GetPhysicsTC()->GetPosition())` = +~125 g) reproduces retail's whole call list
except the stamina-path GetString. Do NOT add dummies; find the missing retail-real IL (tried, no effect: GETTC_EXPANDED (-500),
LIST_CONSTRUCT_HELPER, HASTC_FORCEINLINE, ASSIGN_ORIGINAL_CHAIN, STLP_ALLOC_CREATE, __assume in IP operator-> (+86, not retail)).
Best next ideas: FableWin /Od sizes still differ for Normalise (48i v 29i), GetPBaseDef (17i v 13i), CDefString::GetString (19i v 15i),
C3DVector operator- (34i v 32i named) -- the right texts would each add IL per use; any +~110 code-neutral IL before the spell-over
finishes the call split (K1's last test must stay a+g <= 9000: now 8097+15). Then: stamina cancel sound (explicit .GetString() gives the
call but loses the tail merge -- try a named CCharString / different temp scope), frame 0x250 v 0x230, GetPItem order around OnHit.
Names: none made up. Landed spellings seen wrong (already in DOSSIER): 0x007940F0 is CTCUnholyPower::PlaySoundW (landed as CTCDivineWrath).
Headers: hdr/README.md (+ C3DVector SUBTRACT_NAMED_RESULT, stlport ALLOC_PROXY_REBIND this attempt). Flags: none.

## attempt 3 -- progress notes (best: DIFFER(9509v9268) hunks=444 = w3/g1.cpp = differ/00795300.cpp; was 9494 / 517 hunks)
What moved it:
 1. String-literal arguments by IMPLICIT conversion (`GetEffectFromString("RELEASE_EFFECT", effect)`, GetHelperPoint, GetDef, AddTC):
    retail constructs the CCharString temp BEFORE pushing the other args and pushes `lea reg,[temp]` (0x795EAB..0x795ECD); the
    explicit `CCharString("..")` temp is built last and passes eax. Fixed ~10 regions and raised c2 g (+~140 at the spell-over).
 2. `long effect = 0;` declared before `CCharString name("VICTIM_EFFECT");` (retail stores 0 before the ctor, 0x796048).
 3. FableWin header-inline accessors used (sizes SAME as FableWin): HasUnholyPowerActionInitialised (0x02211510), IsCharging
    (0x0280F2F0, `CurFramesForEnvChange<NumFramesForEnvChange`) -- overlay opt-in FABLE_RETAIL_CTCUNHOLYPOWER_INLINE_ACCESSORS.
 4. CDefPointer ASSERT as __assume in operator->/operator* (overlay FABLE_RETAIL_CDEFPOINTER_ASSERT_ASSUME): -45 hunks, frame 0x250->0x240.
    (Motivation: 0x007959B2 releases the HERO_ABILITY def with no null test and no reload; ours still tests/reloads there.)
 5. TCI-less GetTC(&nymph) (+4 g, local CTCNymph gets the FableWin inline StatGetInterfaceType).
With g1 the ordered call list = retail except (a) cancel sounds: ours tail-merges both arms with the table call expanded, retail
calls CDefString::GetString 0x00415DD0 on the stamina arm, (b) K1 CEventDied AddEvent tail-duplicated in ours (zero register ebp).
KEY FINDING (stamina loop form decides two things at once):
 - `for (; Acc > 1.0f; ++stamina_modify) Acc -= 1.0f;` (g1/e1): c1 frequency after the loop x5 -> build-up `~list<CThingCreatureBase*>`
   (0x006F09D0, retail CALLS it at 0x7956F8) is expanded, which adds ~140 g so the spell-over head is right by accident.
 - `while` (k1 = g1 + explicit `.GetString()` on the stamina cancel arm): retail-correct ~list call AND stamina-arm GetString call
   (f 0.031 -> sc 92 refused), but then (i) ours does not tail-merge the two cancel arms (ours pushes `edi`=0 before the GetString call
   and passes eax; retail pushes 0/lea after) and (ii) the spell-over head is short of g again (template copy at g 8899, needs >= 8991).
 So the true source = while-loop (k1 shape) + ~100 more code-neutral IL somewhere, and the zero register (xor edi,edi at entry: ours
 `cmp eax,edi`/`push edi` where retail has test/push 0) is a separate residue that also breaks the cancel-arm tail merge.
Tried, no help: Normalise FableWin-size texts (LOCALS 47i, TRY4 48i = FableWin size but -22 g, same calls); stamina loop do-while /
swapped body; IP operator-> __assume (+86 g on k1 -> 8985 at the copy, still short; +6..+8 hunks); explicit GetString on one/both arms
with the for loop (no change); `while` spell-over loop and a redundant inner `if (D<=0)` (trace only: S1/S2 in w2).
Tools: w2/tr.sh <name> now takes ~60 s (c2 trace); w2/gsum.py; work/calls.py + cmp.py (ordered calls v retail); w2/reg.py (region sizes).
## attempt 3 -- final state: differ/00795300.cpp = w3/r2.cpp DIFFER(9501v9268) hunks=459 (differ/00795300_g1_fewest_hunks.cpp = g1, 9509/444)
r2 = g1 + `while` stamina loop + explicit `.GetString()` on the stamina cancel arm + IP operator-> __assume (overlay
FABLE_RETAIL_CINTELLIGENTPOINTER_ASSERT_ASSUME, +86 g) + sweep-1 test `offset.GetSquaredMagnitude() <= reach*reach + 5.0f` with
FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ (+9 g). Trace R2b: template IP copy refused at g 8994 (needs a+g>9000), IS_THING_ALIVE and
bitset::test refused -> the ORDERED CALL LIST IS RETAIL'S except (a) no tail merge of the two cancel-sound arms (extra PlaySoundW) and
(b) K1's CEventDied AddEvent duplicated. g margin is thin (+6): any code-neutral IL change before the spell-over can flip the copy back.
Both remaining call residues come from the ZERO REGISTER: ours `xor edi,edi` at entry (cmp eax,edi / cmp esi,edi / push edi in the
start region); retail has `test` and `push 0` there and sets edi=0 only at the release start (0x79570C). Not moved by: reading the button
state later (p1/p2). Frame 0x240 v 0x230: ours box/pos/hit objects sit at other offsets (hit 0xd8 v 0xc8; build-up box 0x90 v 0x74):
the local set/order differs, not one object.
Other residue seen: operator- in the sweep-1 range test loads Z,Y,X (ours) v X,Y,Z (retail 0x79625E) -- SUBTRACT_NAMED_RESULT and the
default text both load Z,Y,X: retail's subtraction text is something else (component-wise assignment?).
Best next ideas: (1) find why c2 keeps 0 in edi from entry (count/weight of zero uses in the start region; try `FinishMove`/cancel code
as a shared local lambda-like block or a bool `cancel` flag set in both arms with ONE cancel sequence after -- retail's shared tail
0x795473 could be ONE source copy reached from both arms); (2) operator- text with X,Y,Z load order; (3) replace the unproven
__assume / GetSquaredMagnitude g-raisers with the real missing IL once found.
