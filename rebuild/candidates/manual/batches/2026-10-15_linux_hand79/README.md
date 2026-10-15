# Hand chunk 79 (CSystemManager .. CTCActionUseBuyHouse): 37 of 40 byte-exact

Authored by agent79 (wave 25, Linux lane, DDK 13.10.4035 cl.exe under Wine); NOTES written by the coordinator from the
agent's final report (the owner's pause arrived as the agent finished: payload.json/oracle.tsv were already built and
gall.sh 37/37, tall.sh 37/37, tall.py 37/37 had passed).

## What matched (37)
- CSystemManager 009a52f0, 009a57b0; CSystemRegistry 0099cf10, 0099cf80 (GetSLONG), 0099d0c0 (WriteSLONG), 0099d100,
  0099d140 (WriteString, REG_SZ), 0099d180, 0099d1e0.
- Destructors 005bbbce, 006b5b00 (CIDrawEnvironmentTextureSet), 007f3de0, 0090ba20, 0098f500, 00a87140 (CPhysicsFacesSegmentLeaf).
- CTCAI* 007e0e30, 007abb80, 007fde00, 007fde70, 007fdef0; CSoundEmitterLH::CActiveSound ctor 00aab850 (vtable 0x0129E67C).
- Interface-lookup forwarders 007b9090 (CTCTavernTable), 007ba4f0 (CTCTavernBar), 007ebbc0, 007f80c0 (CTCActivationReceptorDoor), 008386e0 (CThingNoise).
- CTCActionUse* ctors/dtors 004da939, 004db366 (CTCCarriedActionUseRead), 00729360 (CTCCreatureExpression), 004dabbc,
  007ebdd0, 0072e690, 0072f040, 007eb9b0; others 0051b620 (vector<CLevelFiles> push of an empty record), 0058148d
  (CTCHaste::RemoveMultiplier), 0059474e (STL `__pop_heap_aux`, fastcall).
- `#pragma optimize("s", on)`: 005bbbce, 004da939, 004db366, 004dabbc, 0058148d, 0059474e. No extra flags.

## Parked for Fable 5.1 (2)
- 007eac10 CTCActionUseBase::OnCreate DIFFER(93v81): retail keeps the local def pointer in esi from GetSubDef through its release and stores its zero after the argument pushes; ours re-reads the frame slot | tried: plain CDefPointer local, raw pointer copy probe (83v81) | next: lane gap: CDefPointer local filled through `&local` re-read (GOTCHAS 24k; as hand82 0x007E3040)
- 007eac70 CTCActionUseBase::vfunc_6 DIFFER(63v63): retail loads ParentThing into ecx before the three argument pushes, ours after | tried: named string local, named thing local, inline GetThing() accessor, implicit CCharString conversion | next: lane gap: scheduling

## Skipped
- 007fa26c: not a function -- reached by a `je` from inside the preceding function, uses its saved registers and ends with its `ret 38h` (logged in SKIPPED_ROWS.tsv as `cold`).

## Label corrections
- 0099cf80 is GetSLONG (GetBOOL is 0x0099CFE0, which calls it and returns `!= 0`); 0099d0c0 WriteSLONG; 0099d140 WriteString.
- 0x007FEF40, 0x0058119B, 0x00800020, 0x005946B2, 0x00693EF0 also carry propagated names (details in each source header).
