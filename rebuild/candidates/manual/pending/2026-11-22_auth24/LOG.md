# 0x00839420 LOG (AUTH24; continues AUTH23's LOG, grades 1-3 there)
Not grades (mc.py builds, ours v ours only, tools/lf.py): p2 (AUTH23's best) v p3 = p2 ported onto the shared headers (CDefinitionManager /
CGameDefinitionManager / CMainGameComponent / CWorld / CWorldMap / CRegion GetPRegionDef / CMaterialDef (new opt-in
FABLE_RETAIL_CMATERIALDEF_SWORDHITEFFECTS, overlay hdr/) / ThingAlive.h / CCRC.h / NParticleEngine_CParticleUpdateParameters.h /
CTCDParticleEmitter.h FULL; CreateScaledEffect -> CTCDParticleEmitter::Create 5-arg): 1316 B both, the 7 differing lines are relocation
symbol spellings only (byte-neutral port).
Not grades: mini/sq_*.cpp, sub_*.cpp, st.cpp, ctor.cpp, XS1..XS4.cpp (the LOD statement alone): every existing GetSquaredMagnitude /
operator- / component-ctor opt-in and four statement shapes give the same product order Z*Z, Y*Y, X*X (x87 stack position, top first);
retail 0x00839736 has X*X, Z*Z, Y*Y. No existing opt-in moves it.
## Pre-registration (ONE grade): p4 = p3 + FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY + FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD +
FABLE_RETAIL_CCREATUREDEF_DIALOGUEVOICES (the inline StatGetTypeName the template body needs) + GFSqr from engine/GFWorldDistance.h.
Expectation: 0x0083968F becomes `dec [ecx+4]` (in place), pmaterial_def / pcreature_def slots [esp+14h] / [esp+10h] as retail, length 1308;
the only residue left is the squared-magnitude product order at 0x00839736..0x00839744.
4. p4 (pre-registered above): 00839420 NThingHelpers::CreateEffectOnHit DIFFER(1308v1308), 6 masked lines = exactly the predicted residue
   (0x00839736..0x00839744 product order; everything else retail, including the in-place `dec [ecx+4]` and the two slots).
Not grades (c2 probes, lane_tools/c2probe + pending/2026-11-20_c2_symbol_order kit in ../c2w; odall10): the product order is c2's
commutative-operand sort (GOTCHAS 466: key = hash16 of the product node = 3 * (symbol number << 5) + 0x30, 16 bits, descending). p4's
field symbols of the difference temporary: X #534 (made by the inliner), Y #679, Z #681 (made by the tree optimiser) -> H ff90 > fed0 > c870
= Z, Y, X. Retail's X, Z, Y needs Y / Z four or more numbers later (H wraps past #682). Ego_d line 129 (13 B) is a two-argument fastcall
cast: `GFDebugPDynamicCast(pthing_hitting, (const CThingCreatureBase*)0)` measures 13 B and makes the enclosing `if` 17 B (Ego_d 125: 17;
ours was 13 / 6). Alone it changes no byte and no number (p6a); with the debug body's one ASSERT kept as the release dead arm (existing
opt-ins FABLE_RETAIL_GFDEBUGPDYNAMICCAST_ASSERT + FABLE_RETAIL_ASSERT_DEAD_ARM, wfB3 precedent) the numbers become X #535, Y #712,
Z #714 -> H c8d0 > 0bf0 > 0b30 = X, Z, Y (p6b; mc.py build differs from p4 in 8 lines, all in that block).
Also measured, not used: NParticleEngine::MAKE_VECTOR_PARAM_FROM_BASE(0, "DIRECTION") gives Ego_d line 208's 40 B (ours 42), but c2 does
not expand it here (sz 25, c1 14: one over the always-inline bound 13; score 182 at f 0.012 v T 74) -> a call retail does not have.
## Pre-registration (ONE grade): p6b = p4 + line 129 as GFDebugPDynamicCast + the two assert opt-ins. Expectation: RELOCATION_MATCH.
5. p6b (pre-registered above): 00839420 NThingHelpers::CreateEffectOnHit RELOCATION_MATCH flags= [].
Not grades: pk.py on p6b's object: 9 EQ; GetSubDef<CCreatureDef> 182 v the owner's 162; CArray<pair>::push_back 5 v 70 (the sibling's
FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE makes it a forwarder); _M_insert_overflow<pair<ulong,C3DVector>> 222 v 222 bytes NEQ.
p7 = p6b without FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE: leaf identical (lf.py), push_back 70 B EQ 0x0076B120.
p8 = p7 + FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CCREATUREDEF_SIZE_BODY (new nested opt-in, the CEntitySoundDef device's twin): leaf identical,
GetSubDef<CCreatureDef> 162 B EQ 0x00485815 (its literal at 0x01231D3C holds "CCreatureDef"), every other symbol's size / class unchanged.
odall10 with CDefPointer::IsNull / Get declared throw() (od/hdrn, evidence only): lines 151 / 166 measure Ego_d's 77 / 52.
## Final (ONE grade): 00839420.cpp = p8 + header comment. Expectation: RELOCATION_MATCH (leaf bytes identical to p6b's by lf.py).
6. 00839420.cpp (p8 + header comment): RELOCATION_MATCH; pk.py 11 EQ + _M_insert_overflow (not leaf-bound, same differing copy in the HEAD
   object of 0x0076B120: debt); vt.py PASS; vland dry-run (strict headers) RELOCATION_MATCH PASS, WINS 1.
Not a grade: mini/egod87.cpp = the final text with Ego_d's ONE statement at line 87 (`const CThing* pthing_hitting =
hit_params.GetPThingHitting();`, no separate `= 0`): mc.py build, lf.py diff against final.obj = empty (1308 B, the zero store at
0x00839430 is still there). The draft's separate statement was never needed: removed.
## Final 2 (ONE grade): 00839420.cpp with line 87 as Ego_d has it. Expectation: RELOCATION_MATCH (leaf identical by lf.py).
7. Final 2 (line 87 as Ego_d): RELOCATION_MATCH; pk 11 EQ + the _M_insert_overflow debt; vt PASS; vland dry-run WINS 1. 8. Comment-only regrade of the hand-back file (the line-87 note corrected: mini/tcd0.cpp shows the entry zero store is not tcd_particle_emitter's initialiser; mini/noarm.cpp shows it does not depend on the assert opt-ins):
00839420 NThingHelpers::CreateEffectOnHit RELOCATION_MATCH flags= []
9. Final 4 (ONE grade; batched edit after the advisor's completion review: (ETCInterfaceType)132 -> TCI_BERSERK, the dead arm marked
   Inferred, lines 207 / 208 listed as open Ego_d deviations; no statement changed):
00839420 NThingHelpers::CreateEffectOnHit RELOCATION_MATCH flags= []
Grades (q.py runs on the row) used by AUTH24: 6 (p4, p6b, final, final 2, final 3 and final 4; final 2 removes the redundant `pthing_hitting = 0;` statement, final 3 / 4 are comment and enumerator-spelling edits; the leaf bytes are p6b's in all four); AUTH23 3 before. Not counted: 4 vland dry-runs, about 40 mc.py ours-v-ours builds (row variants and mini TUs), 6 c2 probe runs, 8 odall10 runs.
