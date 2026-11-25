# NF423 hand-back (2026-11-25): scripted T1 caller respells

158 rows landed as `2026-11-24_landing_nf423_{b1,c1,c2,c3,g1,g2}`: each caller now binds its target's PDB name instead of a free
__fastcall stand-in; every object is identical to HEAD apart from that callee name (objgate ALL_EQ, vland PASS -> PASS).
c1..c3 are the 120-row CThingSwitch family (`?AutoTinySecondMemberDestructor@@YIXPAX@Z` -> `??1?$CCountedPointer@VCEntityScriptBase@@@@QAE@XZ`
0x00CE1000). Table: 36 deletes, 14 adds (the c1 rewitness was moot: c3's delete removed that line).
Lander note: b1/g1/g2 RESULTS gave the new CALLEE in the `new` column; the coordinator set it to each row's unchanged self symbol
(the landing recheck compares the graded self) and relanded.
The generator (`gen423.py` and its pipeline) covers two shapes; RESIDUE.tsv gives every other T1 wrong spelling a reason code.
Rulings (coordinator, precedent): r1 yes (callee-only respell of a row T3 names for its own self debt; the line stays open);
r2 yes (bodiless local class for a qualified destructor call; SHARED_HEADER_AVAILABLE is header debt as for NF412 / NF417).
