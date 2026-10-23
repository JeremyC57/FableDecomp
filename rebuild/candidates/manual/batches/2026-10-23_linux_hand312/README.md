# Hand chunk 312 (C3DAnimFileLoader .. CAIStateGroup_JackOfBladesBase): 6 of 40 byte-exact

All six grade RELOCATION_MATCH with the RTM cl 13.10.3077 lane. Each has a behaviour test that passes under g++ (tall.sh) and VC7.1 (tall.py).

## What matched (6)
- **C3DAnimFileLoader**
  - 00a999b0 `C3DAnimFileLoader::C3DAnimFileLoader` registers the chunk-type tree under the "3DAF" root.
    - Shape: each prototype chunk is an unnamed temporary argument, so the out-of-line ctor's eax result is pushed.
- **C3DMeshMaterial**
  - 00abf100 `C3DMeshMaterial::LoadBinary` reads the scalars through the inlined `CDataInputStream::Read`.
    - The optional CFilenames block is a counted-pointee local.
    - Its generated dtor runs only the member dtors, because the base vptr store is dead.
- **C3DMeshPartitionElement**
  - 00ac21e0 `C3DMeshPartitionElement::ReadFromFile` fills the box from component-by-component C3DVector locals.
    - The radius goes into a named local before the centre copy.
    - The second recursive call becomes the loop back to the top.
- **CAIStateGroupBase**
  - 00980770 `AIAction_MoveToPossiblyBlockedObjectAndFace`
    - Each `bool reached` sits in its own scope. Retail packs it into the dead `route` argument slot; one shared local takes a frame byte.
    - The retry loops use `if (r == 4) continue; break;`.
    - The final branch is inverted (`result != 2 || !filter()` first), so the route-failure `return result` cross-jumps to the same tail retail picks.
    - The TestTC/GetTC flag form keeps the action-point pointer in ebp.
    - The CThing::GetPosition 0x4C73D0 / CThingCreatureBase::GetPosition 0x6A4D00 pair is routed per the lander rule.
- **CAIStateGroup_ApprenticeHero**
  - 0092a8a0 `PracticeSkills` is a twin of the landed SkillPractice 0x0092B3E0.
    - The carried-object calls take `const char*` arguments, so the CCharString temporaries are built before the pushes.
- **CAIStateGroup_CreatureTakeCover**
  - 0097a630 `Update` needed six changes:
    - three separate `IsStillRunnable` / `AttackTargetThing` exits (retail's preloaded `mov ecx,esi; je` form);
    - the firing-position pointer built by implicit conversion (a lea-readdressed temporary);
    - the reload animation name passed as `const char*`;
    - the cover list size as an unsigned count of 8-byte entries (`test ecx,0xfffffff8`);
    - CCreatureAction_ProjectileCrouch at its retail size of 0x124 (fixes `sub esp`);
    - `GFFloatRandom` as `__fastcall (float, unsigned long* seed)`.

## Parked for Fable 5.1 (15)
- 00a2f2d0 C3DMeshStats::LoadBinary DIFFER(1129v1129): volume loop LoadBinary argument setup, retail ecx=MemoryPool / eax=begin, ours swapped | tried: index loop, `*(begin()+i)`, named entry pointer, const-ref locals | next: lane gap: register allocation (scratch rotation)
- 00a8ad40 C3DMesh2::LoadBinary DIFFER(1621v1621): the two ushort counts (retail edi/edx, ours spills one) and the three block sizes (retail spills mainSize) | tried: named size locals, ReadUShort into ulong vs ushort, statement order | next: lane gap: register allocation
- 0097e380 CAIStateGroupBase::AIAction_MoveToThingProximity DIFFER(1240v1286): retail this=edi, nav=ebx (loaded before the loop), faceTarget=ebp; ours this=ebx, faceTarget spilled; route-end reach-test block layout | tried: thing twin of landed MoveToPosProximity, inline accessors, named nav local, continue/break loop | next: write the route-end test as its own inline helper; else lane gap: whole-function register caching
- 00980bd0 CAIStateGroupBase::AIAction_SpeakToTarget DIFFER(1212v1233): the repeat==1 half is exact; in the later-call half retail cross-jumps `return 3` and the IsThingAlive `return 1` to shared cold blocks, ours inlines the epilogues | tried: do{}while(0) result=4 exit, separate exits, IsThingAlive 0x6961F0 vs inline form | next: invert the later-call branches as in 00980770 (failure arm first) so the tails cross-jump
- 0092a3f0 CAIStateGroup_ApprenticeHero::PracticeAgainstApprentice DIFFER(1209v1188): the pairing loop's first-arrival block is sunk below the return, `Random()%10+10` folds as add (retail lea), operator= chain uses edi (retail ebx) | tried: 3 forms of the pairing loop, an __int64 id member for the 64-bit filters (fixed the ebp frame) | next: split the pairing loop into separate wait/join loops
- 0090e5d0 CAIStateGroup_BanditGossip::Update DIFFER(1367v1367): gossip component in ebp (retail frame word), `xor ebx` zero register, message-query local order, 5-bit kind test as and/cmp (retail xor/test) | tried: GetInterface out-param, named query locals, bitfield compare | next: model the kind as `(x ^ k) & 0x1f` test; lane gap: register vs frame word
- 009289b0 CAIStateGroup_BanditPlot::Act DIFFER(1131v1131): `Random()%300` div scheduled after the GFGetCurrentFrame call (retail before), so ebx/ebp swap downstream | tried: 6 forms (named locals, statement order, a GFGetCurrentFrame-first local, an inline Random accessor) | next: lane gap: div scheduling across a call
- 009663a0 CAIStateGroup_CarryCrateAndFollow::Update DIFFER(1165v1180): frame layout (flag 0x13, key 0x14, claim 0x20, temp-dtor flags 0x24, look vector 0x28, ...) and retail builds the LookAtPos vector in the argument slot after GetPosition | tried: 3 forms (lexical scopes, conditional EXPRESSION_WAIT temporary, named vector) | next: write the LookAtPos argument as an inline operator+ temporary (00980770 form)
- 009175e0 CAIStateGroup_Clean::Update DIFFER(1091v1064): wash-loop counter in a frame word (retail ebp), every local below the bounding centre sits 4 bytes higher, passes count add/mov (retail lea) | tried: 3 forms; GFFloatRandom fastcall signature fixed the call shape | next: give the counter its own lexical scope and shrink the bounding-centre temp
- 0091ec30 CAIStateGroup_CreatureRangedCombat2::Update DIFFER(1059v1047): flag-byte slots swapped (reachable at 0x12 vs arrival at 0x13), so the in-range loop spills the target pointer; hoisted brain load before the melee branch | tried: 4 forms (flag declaration order, const/non-const LowerBound routing, named flags) | next: declare the arrival flag in the loop's scope
- 00922860 CAIStateGroup_Drunk::Update DIFFER(1620v1612): frame-slot sharing (TestTC bools and the reached flag share one byte, key 0xc3 in the best-distance word, nav pointer in the drunk-component word) | tried: 2 forms (lexical scopes) | next: more `{}` scopes per phase; lane gap: frame packing
- 0094d020 CAIStateGroup_GameMaster::Update DIFFER(1043v1035): creature load hoisted above the GetInteractionTarget virtual call (+8) and `mov eax,[eax+4]; mov ecx,eax` before GetPVillage | tried: inline/named creature getters, SetGameMaster label fix | next: lane gap: the inlined lower_bound eax copy
- 0092d1f0 CAIStateGroup_GoToBed::FindFreeBed DIFFER(1034v1031): Rhset.Forward copy (retail fld/fstp X,Y, Z stored twice), BedToRightOfCreature true block not cross-jumped, ebx/edi swap | tried: 6 forms (find_if<CIsFreeBed> stand-in, statement-accumulated distance order) | next: copy Forward through a user copy-ctor C3DVector
- 009482a0 CAIStateGroup_GoToPub::GetSeat DIFFER(1072v1072): random index stays in edx (retail eax via if/else), bool result set after the pops (retail before), the 0x21 component in the frame (retail: the dead actionPointName slot) | tried: 7 forms | next: lane gap: locals in dead argument slots
- 00948880 CAIStateGroup_GoToPub::Drink DIFFER(1194v1206): the shout result flag is in a frame byte and the temp-dtor flags in ebx in retail, plus one more frame dword (actions 4 bytes higher) | tried: 3 forms (continue/break retry, const char* shout name) | next: size the sip action / add the missing frame dword (check the action object sizes)

## Skipped
- **Lane gap: inline-asm dword fistp rounding (asm_bake family)**
  - 00a4cec0, 00a4d330, 00a4d920, 00a53c50 and 00a9eb90 round inside the inlined quaternion slerp table lookup.
  - 009609b0 uses an ebp frame with 15.0/0.5/1.0 constants.
- **Other lane gaps**
  - 00a4c5e0 C3DAnimationSequenceData::ReadCompressedFromFile: the unsigned 64-bit size guard (`mul; test edx,edx; ja`) keeps a dead `jb` in this lane.
  - 00a4e300 C3DAnimation2::GetDeltaMovement: a 0x104-frame x87 quaternion-to-matrix pair; chunk-293 x87 operand-order gap.
  - 00ab2520 C3DAnimationBuilder::BuildMeshAnimation: an inline 3x4 matrix inverse and product; x87 operand-order gap.
- **Decoded only, no source authored.** These rows are 1.2–2 KB with large frames and heavy x87 or unresolved callees, and the budget did not reach them:
  - 0097fea0 FindAGuardAndYellForHelp: guard finder 0x8FD000/0x982450, opinion list walk, crime report 0x8112D0, and odd-address callees 0x65CE56/0x65C1FC/0x6585EE/0x65BFCE.
  - 0097a1b0 MoveFromCoverToFiringPosition: a 0x20C-frame arc search with 224 x87 ops.
  - 00949ab0 GoToPub::IsRunnable: conditional CCharString temporaries and an inline STLport list build.
  - 00a50510 InitialiseAnimationSequences.
  - 0094e7b0 EarthTroll::Update (394 x87 ops).
  - 00912ba0 CalculatePrettyPath.
  - 00951430 IntoGroundSwoop.
  - 00952890 UpdateGround.
  - 00973900 SummonPillarsOld.
  - 00974110 Summon (0x3CC frame).

## Label corrections
- Chunk-loader constructors:
  - 0x00AB0700 C3DAnimFileBoneMaskChunk ctor.
  - 0x00AAE2A0 C3DAnimFileMovementVectorChunk ctor.
  - 0x00AAE170 C3DAnimFileTimingEventChunk ctor.
  - 0x00AA8360 / 0x00AA8490 CChunkedFileSuperChunk ctor/dtor.
- 0x0092E790 is find_if<CIsFreeBed>.
- 0x0094D430 is SetGameMaster.
- 0x0092A8A0 is a SkillPractice::PracticeSkills twin.
- 0x006CB990 / 0x006CB9A0 are navigation route queries.
- 0x00A14260 is C3DVector::RotateAboutZ(float, bool).
- 0x0090EBE0 is FindMessage.
- 0x00981410 is AIAction_InteractWith.
- 0x00692620 is the action-id getter (labelled CWorld::DrawGetEnvironment).
- 0x007120D0 is ~CCreatureAction_AddRealObjectToInventory (labelled _Dest_val).
- 0x007446A0 is CTCPerceiveThing::GetPerceivedThing (labelled _Cons_val).
- 0x0049D810 is CWorld::GetPRandomSeed (labelled DoSizeof).
- 0x008FCF40 is CAIStateGroupBase::Terminate (labelled CXMVPlaybackInfo).
- 0x006A8AF0 is a village-task filter functor.
- 0x006AF290 / 0x006AF2B0 are message filters on the thing's 64-bit id (+0x18/+0x1c).
- 0x007FEF40 is CTCUserActionPointBase::FreeActionPoint(long). One label wrongly calls it EditCyclePreviousKeyCamera.
