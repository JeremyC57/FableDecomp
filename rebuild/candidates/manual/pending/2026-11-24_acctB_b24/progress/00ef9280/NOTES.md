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
