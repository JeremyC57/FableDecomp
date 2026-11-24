# 0x00EF9280 NScript::Cjoss_test_scriptsScript::Main -- B24A12 (2026-11-24), opened only (context ran out on rows 1-2)

- Extent check: the oracle is right. Retail body 0x00EF9280..0x00EF9ED5 `ret` = 3158 bytes (+ int3 to 0x00EF9EDF = 3168); a second
  `ret` at 0x00EF9E79 is an inner exit. No coordinator action needed.
- Why the draft is far short (1972 v 3158): retail EXPANDS ~CScriptThing (16 x ~CBaseClass 0x0099A2E0, 20 x operator delete 0x00BFE9BC
  = the 8 CScriptThing locals expanded at two exit pads) where the draft calls 0x004AA840; retail calls IsActiveThreadTerminating
  0x00CB7940 18 times, CScriptThing::operator= 0x008AB980 5 times (one site expanded), IsAlive 0x004AB130 3 times. The draft also
  re-defines CScriptThing members locally (operator=, IsAlive, MsgIsHitBy*) -- not allowed now; use engine/CScriptThing.h.
- FableWin 0x03AF7BD0 (Main, 0x2310 bytes) is a LATER build with statements retail does NOT have: "test_roll_cam" (slot 412),
  "cock_camd" (slots 70, 414), GetPos x2 + slot 105 after the pinky/perky test, THING_EXPLOSION_THROWING_ORB_BOMB (slots 70, 6, 104),
  a CEntityScriptBinding<Cjoss_test_scriptsScript> for SCRIPT_NAME_HERO + AddEntityScriptBinding + PostAddScriptedEntities, and
  "cock" (slots 72, 474). Retail has exactly 12 CCharString ctors (guy_script, bandit_level_cutscene, fireh, summoner, jim,
  SCRIPT_NAME_HERO x2, bob, my_generator, pinky, perky, dan_bandit) and none of those slots (0x670/0x678/0x1A4/0x1A0/0x768 absent).
  Use FableWin only for the shared statements' order and for TRetVoid / `if (false) {} else for` idioms.
- Next: port onto the model idioms (TRetVoid _Fail_Val, `return _Fail_Val.Return();`, FableWin loop form), CScriptThing from the header,
  then work the ~CScriptThing expansion split (two pads expand, the rest call).
- Baseline regrade of the draft: TIMEOUT (cl hangs on this draft, usually an #error in a header) -- run a gcc `cpp` pre-pass before grading.

## B24S2 (2026-11-24): PORTED, compiles (the old draft hung cl: engine/Cjoss_test_scriptsScript.h only in hand842's overlay)
- hdr/engine/Cjoss_test_scriptsScript.h: hand842's header copied whole (new file; class 0x48, CLevelScriptBase<Cjoss_test_scriptsData>).
- Source s2/v2.cpp = differ/: FableWin.pdb locals (straw_bob, jim, generator, camera [the 8th CScriptThing, unused in retail's
  statements = the draft's `dan`], pinky, perky, bandits_done, summoner, fire_heart, sum_stuff, _Fail_Val, pwr, sthings, n long),
  FableWin ctor order, TRetVoid exits, `if (false) {} else for (;; Game->NewScriptFrame()) { if (T) return ...` main loop and the
  for-scope bandit loop (FableWin xor/je at 0x03AF7D08 / 0x03AF917A), visible inline ~CScriptThing / operator= / IsAlive / MsgIsHitBy*.
- Grades: v1 (with STB OUTOFLINE_DTOR) 3447v3158 100 hunks; v2 (STB dtor inline, = retail: ~CBaseObject 0x0099A430 x2) 2622v3158
  80 hunks; v3/v4 (plain main loop) 2724; v5 (plain bandit loop) = v2.
- Inline trace (inl/v2.txt): exits at loop head (f 2.5) and fireh branches (f 0.625) expanded, all later exits called = retail's
  decision set for ~CScriptThing. Retail call counts: ~CBaseClass 16 / delete 20 (two expanded pads), ours 9 / 11 (one pad).
- Residues: (1) retail's two expanded pads 0x00EF9A67 / 0x00EF9C55 are NOT tail-merged because their STB release zero stores are
  scheduled differently (pad A Data then Info, pad B Info then Data); ours are identical and merge. (2) operator= expanded at 2 sites in
  retail (5 calls of 0x008AB980 for 7 assignments), ours 1 (fireh, f 0.625, c1 66); next candidate summoner= (f .1562 sc 116).
  (3) IsAlive called at 3 sites in retail (0x004AB130), ours 1 (sc 126 at f .0323): retail's later IsAlive sites have lower f.
- Next idea: FableWin's loop-block bools (performed_flourish, succ_flourish, flourish_hit, projectile_hit, depleted = results stored)
  -- they add no control flow but may change the pads' scheduling; then the frequency of the pinky / perky tests.
- B24S2 grade 7: v6 = v2 + FableWin's result bools (flourish_hit, projectile_hit, performed_flourish, succ_flourish, depleted) = unchanged 2622v3158 (dead stores vanish, no scheduling change). Not kept.
- Next idea (replaces the result-bools idea, ruled out by v6): retail's pad A zeroes the STB PImp Data then Info, pad B Info then Data -- grade with FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (Data-first release text), which this row has not tried; then the operator= / IsAlive frequency split.
