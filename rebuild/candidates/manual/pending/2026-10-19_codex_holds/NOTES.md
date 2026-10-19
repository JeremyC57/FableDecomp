# Codex 2026-10-19 update: rows held at Fable review (bytes match retail; held for the reasons below)

| address | reason |
|---|---|
| 00433fe0 | wrong callee: CPersistContext::Transfer float/long overloads swapped (0x431061 float, 0x431102 long) |
| 00555390 | wrong callee: ::operator new where retail calls the malloc import 0xBFEA0E |
| 007464a0 | wrong callee: ::operator new where retail calls the malloc import 0xBFEA0E |
| 00434450 | test exercises a hand-written stand-in, not the candidate |
| 004554c6 | test exercises a hand-written stand-in, not the candidate |
| 00457537 | test exercises a hand-written stand-in, not the candidate |
| 004585fe | test exercises a hand-written stand-in, not the candidate |
| 00459615 | test exercises a hand-written stand-in, not the candidate |
| 004642fa | test exercises a hand-written stand-in, not the candidate |
| 004643cf | test exercises a hand-written stand-in, not the candidate |
| 004c3d30 | test exercises a hand-written stand-in, not the candidate |
| 004de122 | test exercises a hand-written stand-in, not the candidate |
| 00519b80 | test exercises a hand-written stand-in, not the candidate |
| 0051a900 | test exercises a hand-written stand-in, not the candidate |
| 0056e6b6 | test exercises a hand-written stand-in, not the candidate |
| 0056fe7c | test exercises a hand-written stand-in, not the candidate |
| 0058376c | test exercises a hand-written stand-in, not the candidate |
| 00633420 | test exercises a hand-written stand-in, not the candidate |
| 006338d0 | test exercises a hand-written stand-in, not the candidate |
| 006339b0 | test exercises a hand-written stand-in, not the candidate |
| 00687310 | test exercises a hand-written stand-in, not the candidate |
| 007b7a70 | test exercises a hand-written stand-in, not the candidate |
| 007e3c70 | test exercises a hand-written stand-in, not the candidate |
| 00845bc0 | test exercises a hand-written stand-in, not the candidate |
| 0084bd40 | test exercises a hand-written stand-in, not the candidate |
| 008929d0 | test exercises a hand-written stand-in, not the candidate |
| 00892a80 | test exercises a hand-written stand-in, not the candidate |
| 008c35d0 | test exercises a hand-written stand-in, not the candidate |
| 008d0ba0 | test exercises a hand-written stand-in, not the candidate |
| 00b9d100 | test cannot fail (main only prints PASS) |
| 00bcea60 | test cannot fail (main only prints PASS) |
| 00d68f00 | test checks an unrelated local struct; CGameScriptInterface re-modelled with 87 placeholder slots |
| 00daac00 | test checks an unrelated local struct; CGameScriptInterface re-modelled with 87 placeholder slots |
| 00e398f0 | test checks an unrelated local struct; header comment describes another function; names CBaseObject ctor 0x99a380 as CBaseIntelligentPointer and vtable 0x127094c (CScriptGameResourceObjectScriptedThingBase) as CIntelligentThing |
| 00b45460 | test runs a simplified algorithm (divides by 255, never uses g_ByteToFloat); signature differs from PDB; comment still says DIFFER |
| 00a75f10 | test has no virtual dispatch; argument type differs from PDB (ENavQuadTreeNeighbours) |
| 00aed090 | test uses an EmitterModel without the real bitfields |
| 00953bc0 | test uses a copy_model stand-in |
| 00ad7400 | test uses a local Tree::clear; propagated CWound::Kill name contradicted by its own comment |
| 00a3b380 | test hand-transcribed with renamed types; #pragma pack(1) only to reach offsets |
| 00b16eb0 | test hand-transcribed with renamed types |
| 00b16f60 | test hand-transcribed with renamed types |
| 00b17f70 | test hand-transcribed with renamed types; WriteCBYTE bound to both 0x993ea0 and 0x993eb0 |
| 009a0080 | calls scalar operator new/delete where retail calls operator new[] 0xbfeb22 / operator delete[] 0xbfeb1c |
| 00e32200 | one symbol ??1CScriptThingArgument bound to two retail functions (0x4aa840 and 0x99a2e0); 436 placeholder virtual slots |
| 00914c60 | KidMap::LowerBound bound to both 0x40f020 and 0x4365b0 (distinct instantiations) |
| 00abf6b0 | WriteLong bound to both 0x993ea0 and 0x993eb0 |
| 0096e700 | ReachEpsilon bound to two different float pools (0.3 at 0x1238170, 1e-4 at 0x129ba3c); PreferredMargin/ReachEpsilon swapped |
| 00988490 | artificial volatile unsigned long m[16] forces store order |

Held rows need: a test that runs the candidate implementation (not a stand-in), callee/constant bindings matching retail (RELOCATION_MATCH masks them), and no artificial volatile. Re-grade with the Fable lane before landing.
