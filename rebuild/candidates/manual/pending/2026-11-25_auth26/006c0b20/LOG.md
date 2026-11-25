# 0x006C0B20 LOG (AUTH26 / AUTH26b)
Retail 368 B. Statement order from FableWin /Od (0x024B1A30, 569 B = Ego_d code size 569; line map 937..958 fits it exactly):
`{` 937 | entrances 938 | GetAllThings stmt 939 | phero 940 | for(it=begin,end_it=end) 941 (`if(0);else for` macro: xor ecx,ecx/je) |
if(!IsAware(*it)) 943 | ret_pos= 945 | return true 946 | } 947/948 | if(!phero||!GFWorld2DDistanceSmallerThan(hero,house,20,false)) 949 |
if(!IsAware(phero,house)) 951 | ret_pos= 953 | return true 954 | return false 957 | } 958.
FableWin callees: CTCBase::GetWorldMap/GetWorld/GetPlayerManager/GetParentThing, CWorldMap::GetMapNumberAt(const C3DVector&) slot 16,
CThingFilter_InMap(map,mapno), CThingFilter_HasTC(TCI_REGION_ENTRANCE=99), GFFilter_Bind2, CWorld::PeekThingSearchTools,
GetAllThings<CThing,vector<CThing*>,CFilter_Bind2<HasTC,InMap>>, CPlayerManager::GetMainPlayer, CPlayer::GetPControlledCreature,
NCreatureAI::IsAwareOfPosition (fastcall), CThing::GetPos, GFWorld2DDistanceSmallerThan (inline in retail: 400 = 20^2).

g0: FableWin statements on shared headers + ALLTHINGSBODY + HASTC_OUTOFLINE (retail 0x6C1DD0 calls both filter operators out of line).
g0 => RELOCATION_MATCH (368 B) at the first grade.
g1: g0 + ALLTHINGSBODY_EGOD + CTHING_STATTYPEFLAGS + SCANINCLUSION_ISALIVE, to make the same-TU GetAllThings copy equal retail 0x6C1DD0 (g0's copy: 142 v 158 B, no alive/limbo split, no folded type test, early thing store)
g1 => RELOCATION_MATCH; GetAllThings copy 158 B EQ + relocs EQ vs retail 0x6C1DD0 (retcmp).
test 006c0b20_test.cpp vs g1.obj: vt PASS (8 cases A-H).
g1 strict headers: RELOCATION_MATCH.
mutants (test strength, not grades): m1 distance compare taken as already squared; m2 null hero skips the house test; m3 keeps walking to the last unseen entrance
mutants m1 m2 m3: test FAIL on each (killed). m1 grades RELOCATION_MATCH (the 20.0 v 400.0 constant is a masked relocation): the test, not the grader, catches it; g1 uses __real@43c80000 = 400.0 = retail [0x0125D824].
FINAL: 006c0b20.cpp (=g1) strict RELOCATION_MATCH, retcmp main 368 EQ / GetAllThings 158 EQ+relocs EQ, vt PASS, NAME_GATE PASS, addmatch written.
pick-any: all 8 bound copies EQ + relocs EQ (NOTES).
