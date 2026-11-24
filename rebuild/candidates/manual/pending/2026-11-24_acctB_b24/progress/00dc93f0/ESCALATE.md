# 0x00DC93F0 CSecretPassageSkeleton::Main -- ESCALATE (B24B9, 2026-11-24)
Form: differ/00dc93f0.cpp (= b9/c2.cpp), FableWin.exe text: both `for` loops spelled `if (false) {} else for (int count = 0; count < LAST; ++count)`
(FableWin 0x03840793 / 0x03840B88 VC6 for-scope prologs), dead local `bool cut_scene` (INFERRED from FableWin, a later VC10 build: evidence for structure, not proof of retail's text; FableWin.pdb local, stores at 0x0384078F / 0x03840B66),
loop counters `count` (FableWin.pdb). Stock flags, no pragma, no overlay. DIFFER(4269v4507) 212 hunks.
Decision-set proof: c2 override of exactly the three implicit CScriptGameResourceObjectScriptedThingBase::operator= sites at f .7910 (two in the
collected_all block, one in the else block; stock sc 74 = threshold, called) to EXPAND -> RELOCATION_MATCH (on b9/c2.cpp: b9/differ_stb.obj; on the current differ/ = b9/d2.cpp without the WS FORCEINLINE ctor opt-in: b9/d2_stb.obj; first shown on b9/f1.cpp, b9/f1_stb.obj; kit b9/inl_force2.py,
FORCE_SPEC='??4CScriptGameResourceObjectScriptedThingBase@@|0.7910|1'). Every other decision (incl. the helmet ~MovieBase, now called at f .0417 =
retail) is already retail's.
Why no source form: c2 score (c2.dll 0x1078D2EC..0x1078D35A) sc = (c1*1e8/f + 6*c1)/7 < 74; needs f >= .8372 (x1.059..1.128 over the whole
function or after the j loop; b9/win.py shows no other decision flips in that window) or c1 <= 71. Main's /Od calls and conditional jumps align 1:1
with FableWin (only the dead tail differs, traced: no effect); retail's out-of-line ??4STB 0x008ABD10 equals our COMDAT byte for byte; the probes
listed in NOTES.md B24B9 leave f and c1 unchanged. So the notch lies outside Main's and ??4STB's text; CAUSE OPEN (two context probes negative: b9/ctx1 = ??4STB used by an
earlier function in the TU, and B24B4's sibling-grouping probe). Needed: the f1 frequencies scaled by 1.059..1.128, or ??4STB c1 <= 71. Not landable until the owner's object-grouping build mode or a ruling on the override.
