# Chunk 611 (seventh parked-row chunk from the sweep ranking, 10 same-length rows 290-302 bytes): 1 of 10 landed

Landed (vland --land 1/1 RELOCATION_MATCH + PASS, multi-check 0): 0x00C74BB0 CEffectFilter::SetHighPass(long, float, unsigned long)
(libpk DSP, descriptive: `frequency = (float)pow(...); frequency *= reference;` moves the rate's mov/test ahead of the fmul, and a
named omega = frequency * 2pi keeps fmul/fdiv in retail order). Its landed caller CEffectPart260::Configure 0x00C783F0 bound the
address as SetPeak(float, float, unsigned long); the body fild's the first argument, so the caller was respelled (members
HighPassAttenuation / HighPassReference), the old binding retired and the caller rechecked RELOCATION_MATCH + PASS. Parked 9
(pending/.../differ/): 007bbc9f InitMovement 94 -> 28 lines (sin/cos kept as doubles; frame now matches), 005b690b edi/ebx swap in
the second loop, 0087ba90 vtable load register, 007805d0 def pointer reload (shared CDefPointer.h gives 302-303 bytes), 00793370 /
007981b0 / 007afe20 / 007b9b40 STLport vector members whose out-of-line helpers take LTCG register arguments, 00860ad0 scheduling
only. Six parked files had wrong own/callee spellings (OnInit virtual, InitMovement protected virtual, qualified base call, GetPos,
GetNearestNode const, AccurateNormalise, SetCameraRoll free function, SetVelocity on CTCPhysicsBase). 12 NAME_FIXES + 3 review rows
queued (prefix c611: landed GetNearestNode 0x00860A50 non-const, OnDie 0x0087BBF0 non-virtual, vector members without std::allocator,
CComponent list/map stand-in callees).
