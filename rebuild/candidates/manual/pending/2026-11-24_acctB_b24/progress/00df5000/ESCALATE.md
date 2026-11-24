# 0x00DF5000 CQ_TentacleKrakenBossFightScript::PhaseOne -- inline decision set proven, no evidenced source form (B24S2)
Base: c1/v4.cpp = FableWin 0x038AA1B0 form (TRetVoid `_Fail_Val` exits; both `for` loops in the for-scope form `if (false) {} else
for (long i ...)` -- FableWin xor/je openers at 0x038AA440 / 0x038AADB1; FableWin.pdb locals StartMissionMarker, DoorBlockVec,
TempParticle, TempDoor, EffectVec, i (long), seh_Hero, speech_movie, PauseMyEntitiesPlease, mymap, exit_trigger, _Fail_Val; ctor order
DoorBlockVec, TempParticle, TempDoor, EffectVec) + visible `inline CScriptThing::~CScriptThing() { }`. Its call sequence equals retail
except five sites (calldiff.sh): retail EXPANDS the 4 CScriptThing::operator= and vector<CScriptThing>::push_back.
c2 inline trace (inl/v4.txt, an.py; expand iff sc < 74):
- ~CScriptThing c1 60: f 0.5 (first while-loop exit) expanded = retail; f 0.3125 (for-loop temporaries / exits) and below called = retail.
- CScriptThing::operator= c1 66 (in loop, f 0.3125, sc 86) and c1 69 (TheKraken=, f 0.375, sc 85): called; retail expands.
- push_back c1 61 (f 0.3125, sc 80): called; retail expands.
So in retail's TU operator= and push_back cost less than ~CScriptThing at the same block, the opposite of every header text tried:
IMPLICIT_ASSIGNMENT (v8), CCOUNTEDPOINTER_RELEASEDATA (v6), STLport HTTP/CONSTRUCTORS mode of sibling 0x00DEFB10 (v7, v12),
PUSHBACK_FINISH_LOCAL_PASS24 (v11, c1 63). No /d2inlT threshold separates them (the dtor is the cheaper callee).
Bounding proof: v9 (`FABLE_FORCEINLINE CScriptThing::operator=`, the landed form of 26 rows e.g. 0x00D52E90, 0x00D74430) leaves only
push_back (DIFFER 3007v3071); p2 = v9 + push_back forced through a probe header (probe_hdr/stlport/fable_containers.h,
FABLE_DIAG_PUSHBACK_FORCEINLINE, diagnostic only) = RELOCATION_MATCH (fg.py only -- jump targets normalised, NOT raw-compared; run rawcmp.py / strict vland before relying on it). Header precedents for a force-inline STL member opt-in: FABLE_RETAIL_QUICK_MENU_PAIR_CONSTRUCT_PASS27 (_Construct) and FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_FORCEINLINE_ASSIGN. Decision: accept a push_back force-inline opt-in for this TU, or hold.
- CAVEAT (B24S2): the p1/p2 RELOCATION_MATCH above is fg.py only (jump targets normalised), NOT raw-compared; run rawcmp.py / strict vland on p2 before relying on it. Header precedents for a force-inline STL member opt-in: FABLE_RETAIL_QUICK_MENU_PAIR_CONSTRUCT_PASS27 (_Construct) and FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_FORCEINLINE_ASSIGN.
