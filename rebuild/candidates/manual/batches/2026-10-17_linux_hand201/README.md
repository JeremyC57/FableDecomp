# Hand chunk 201 (CHeroMorphDef .. CLandscapeBackgroundPatch): 1 of 40 byte-exact
## What matched (1)
- CHitLocationDef::Transfer at 004585fe: packed local layout and explicit typed persist overloads reproduce the frame-pointer call sequence; RELOCATION_MATCH.
## Parked for Fable 5.1 (38)
- 00789238 CHitParameters::CHitParameters DIFFER(174v178): constructor is close but register scheduling and member initialization order differ | tried: packed/unpacked local layouts | next: recover exact source member declaration order.
- 004e39e6 CJackOfBladesBattleDef::Transfer DIFFER(200v200): LEA register rotation differs across repeated typed calls | tried: primitive and wrapper field models | next: lane gap: scheduling.
- 004e80e3 CKrakenDef::Transfer DIFFER(222v222): repeated LEA register rotation differs | tried: primitive and wrapper field models | next: lane gap: scheduling.
- 009f4b10 CInputManager::IsEventProcessed DIFFER(239v240): device-type branch layout differs by one byte | tried: switch and decrement/goto forms | next: lane gap: scheduling.
- 009f4d40 CInputManager::IsEventInQueue DIFFER(281v213): loop local/pointer scheduling differs | tried: indexed and pointer-offset forms | next: recover original iterator form.
- Remaining rows: retail disassembly captured in dis_all.txt; defer to Fable 5.1 authoring.
## Skipped
- 00be5ca0 contains multiple padded function bodies in one row; skipped per chunk rules.
## Label corrections
- None.
