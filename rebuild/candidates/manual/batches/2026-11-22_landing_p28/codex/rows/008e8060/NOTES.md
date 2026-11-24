# NCreatureMode::CModeBriarRoseConcentrate::OnActivate

Ego_r, FableWin, and the retail mode vtable reference at 0x0126855C agree on the overridden activation method. The owner is declared in engine/NCreatureMode_CModeBriarRoseConcentrate.h. All engine and STLport classes used by the source come from the shared headers.

The donor source establishes the constant-FPS timer, marker search, two physics resets, counted imposter pointers and const carrying query. The activation uses the three-component marker average, then projects the facing direction onto the horizontal plane before normalization. The action constructor takes two creature references. The only new header member is the gated CTCAbsorbHits::AddHitsRemaining, proved by FableWin and the addition at 0x008E8335.

The earlier VC7.1 leaf was 1,777 of 1,784 bytes. Its 12 mapped COMDATs and their relocation targets matched. That candidate differed in temporary packing and register choices around the timer and marker and effect blocks.

The native behavior fixture runs the activation body with deterministic engine doubles across 96 scenarios. It checks both marker choices, missing definitions, null or dead spawned things, action assignment, hit counts and effect attachment.

## Latest verification

Verified all 1,784 bytes and all 503 instructions. The 92 leaf operands resolved at their exact retail sites. All 25 mapped inline copies passed. The marker-search copy at 008E8AC0 independently matched its full 153 bytes and all three callee targets. The 96-case VC7.1 fixture passed. Implicit CCharString arguments in the marker, spawn, hit and particle-effect calls restored the stack slots. The authentic GetSubDef, effect lookup and marker search bodies preserved the original marker pointer across calls. The effects definition remains in the scope shown by Ego_d.
