# Hand chunk 470 (ForEachInRoughArea .. CTCGraphicAppearanceStaticMeshFadeOutOnProximity): 13 of 40 byte-exact

Large-row pool (241-1024 bytes). 13 matched, 17 parked with a graded best source in differ/, 10 large rows not attempted.
Grades per row are in STATUS.tsv notes and probe/ledger.txt. No row used more than 4 grades.

## What matched (13)

- **CPlayerManager** (0044a3b0, catalog "HeroSwapState::Initialize"): the ctor. It stores the component and the definitions, builds an empty player vector, then appends the four `hero_swap_N.tng` names. Shape: STLport push_back inlined (placement copy with its null test). Each name is a named local in its own scope. The overflow call gets a same-TU stand-in that never touches the `__false_type` tag, so VC homes the tag on the name's slot as retail does.
- **GFSerialiseMapBinaryIn** (007adbf0, over map<CCharString,bool>): clear the map, read the count, then read each key and bool and insert them. Shape: the landed 0x007565E0 sibling with a bool value. The value is zeroed after the key's default construction. The value reader has a same-TU read-only stand-in; a declared-only reader gives a 0x28 frame.
- **LHHttp2::LHHttp2** (00840f50): the five STLport strings are built empty and the three vectors nulled, then the body does its resets.
  - The landed LAddress string model: `_String_base` plus `_STLP_alloc_proxy`.
  - The body stores sit in the order they fall between the assign calls.
  - One zero register (ebx); the memset compiles as `rep stosd`.
- **GFReadVectorMapMemDump** (0086deb0, CTextureMorph map): reads count, size and raw bytes, then the default element.
  - `GetData()` is `size() ? begin : this`, and size() is recomputed at each use.
  - The default element is a pair temporary whose CTextureMorph has a user zeroing ctor. That keeps one zero register (esi) for the stores and every 64-bit range compare; a POD value-init gives `test`.
- **std::__find_if<C3DVector*, CIsOutsideRadius>** (008c2f70, catalog "Geometry::FindClosestPointInRadius"): STLport's four-way unrolled random-access find_if in a /Gr TU, with the landed CIsOutsideRadius predicate.
- **NCreatureMode modes**:
  - **CModeSitBase::AnimUpdate** (008d6ac0, catalog CModeSitChair vfunc_2):
    - the creature is a named local read after the counter decrement;
    - the names are named locals in their own scopes;
    - the action is a SetCurrentAction temporary;
    - CAnimFrameInfo holds two floats (Ego_r.pdb).
  - **CModeCrouch::AnimUpdate** (008d8b00):
    - the planar speed is `sqrt(X*X + Y*Y)` straight on the members (Y loaded first);
    - each branch builds its own name and VC merges the shared tail.
  - **CModeIntangible::OnActivate / OnDeactivate** (008e7390 / 008e74f0):
    - the boss lookup is the landed one-ternary HasTC + GetTC on the const map;
    - `CModeBase::OnDeactivate` is the inline `Active = false`;
    - the bit store is a one-bit bitfield assignment.
- **LSocket::Listen / Open** (008f8cc0 / 008f9820, both /GS):
  - Listen: the report string is inlined at the first two sites and called out of line at the third (VC7.1's inline budget). This is modelled as a `__forceinline` ctor plus the out-of-line overload.
  - Open: the bind and connect failures are two identical report blocks that VC cross-jumps into one.
  - Each return keeps its own cookie-check epilogue.
- **CPhysicsCoordCartesian::FrameUpdate** (008fa3b0): velocity += acceleration * dt with each component clamped to +-100, then position += velocity * dt.
- **CThingSearchTools::GetAllThingsInRoughArea** (009793d0, catalog "GetPBestThingInRoughArea"):
  - the filter is Bind4<CanBeUsedAsCover, not1(IsOccupiableOccupied), IsNot, In2DRange>;
  - built on the landed sibling 0x007A3CA0 loop;
  - the pushed pointer is a named local assigned before the filter chain.

## Parked for Fable 5.1 (17)

- 006ce800 CThingSearchTools::ForEachInRoughArea<CThing,IsNot,FrameUpdateThing> DIFFER(535v535): at the level-loop exit retail reloads esi (this) before ebp (map iterator); ours reloads ebp first | tried: the landed 0x00518130 loop, an iterator struct v a raw pointer, a function-scope iterator | next: lane gap: rough-area loop-exit spill-reload order (GOTCHAS R&D E-18); permuter candidate
- 0063fd40 CThingSearchTools::GetAllThingsInRoughArea<CThing,list<CThing*>,Bind2<OverlappingCircle,CWorldMap::CThingFilterBase>> DIFFER(661v661): at the node-loop exit retail reloads esi (BRX), edi (BRY), edx (TLX), ebp (map); ours edx, esi, edi, ebp | tried: the landed list sibling 0x0080ECC0 loop with the landed 0x0063F2C0 filter inlined, map declared outside the loop | next: lane gap: node-loop exit reload order (E-18)
- 0066a1e0 CThingSearchTools::ForEachInRoughArea<CThingPhysical,Bind3<SolidToThing,PhysicsFlagsSolidToThing,OverlappingCircle>> DIFFER(725v725): the level-width load is `[ebx+eax+4]` in retail and `[eax+ebx+4]` in ours, and ours reloads the edi spill first at both exits | tried: GetCellPtr helper, both operand orders of offset + LevelInfo, a rowWidth local | next: the chunk-308 SIB-variable recipe (one user variable per indexed access, assigned after the loop guard); the reload order is the E-18 gap
- 00516830 CThingSearchTools::GetAllLeafThings<CThingAICreature,list,Bind2<IsInQuestOrNullQuest,OverlappingArea>> DIFFER(372v370): retail rounds the bounding radius with an inline-asm ceil (fld/fadd 0.5/fistp/fild/fsub 1.0, compare bits, dec; ebp frame, constants in frame locals); ours uses _ftol | tried: C++ reference body (1 grade) | next: retail_asm candidate: minimal `__asm` ceil with a `// Retail-asm:` line
- 00518340 CThingSearchTools::GetAllTCsInRoughArea<interface 0x3D,CThingFilter_IsInQuestOrNullQuest,vector> DIFFER(686v686): at the level-loop exit retail reloads edi (map iterator) before esi (this); ours esi first | tried: the 0x0084BA50 model, the filter through a named reference, a HasComponent helper (687, reverted) | next: lane gap: E-18 reload order. It flips against 0x0084BA50, which differs only in the filter, so a numbering/parity effect is likely; permuter candidate
- 0084ba50 CThingSearchTools::GetAllTCsInRoughArea<CTCPerceiveThing,Bind2<IsPerceivingThing,IsNot>,vector> DIFFER(702v702): ours reloads the edi spill first at the node and level exits (retail edx, esi, edi and ebp, edi) | tried: the landed list instance 0x0091BA40 loop | next: lane gap: E-18 reload order
- 0085b5b0 CThingSearchTools::GetAllThings<CThingGameObject,list,Bind4<IsNot,CombatIsStrikeToStrike,CombatOverlappingSphere,...>> DIFFER(365v365): retail loads the GetBoundingSphere `this` (ecx) before the argument pushes and builds the two out addresses in edx/eax; ours pushes the radius address through ecx first, and the result-list / loop-tail loads swap ecx and edx | tried: a map reference local, a named component local, the out-parameter GetInterface (368, reverted) | next: one-ternary GetInterface into a named CTCHitLocations* (EXTRA_RULES), or an inline GetBoundingSphere wrapper taking the component
- 00516200 CThingSearchTools::GetAllThings<CThingPhysical,list,Bind3<OverlappingArea,HasNavigationInformation,IsInQuestOrNullQuest>> DIFFER(440v423): the same inline-asm ceil as 00516830 | tried: C++ reference body (1 grade) | next: retail_asm candidate: minimal `__asm` ceil with a `// Retail-asm:` line
- 007c2d10 CThingSearchTools::GetAllThingsInRoughArea<CThing,vector<CThing*>,Bind3<IsNot,ShotsToCheckForCollisionWith,SolidToThing>> DIFFER(704v704): at the level-loop exit retail reloads edi (map iterator) before esi (this); ours esi first | tried: the landed 0x007FDB10 loop plus the landed 0x007C2800 filter bodies, dropping the typed temp (696, worse) | next: lane gap: E-18 reload order
- 007c5110 CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase,list<CIntelligentPointer<CThingCreatureBase>>,Bind2<IsNot,In2DRange>> DIFFER(697v697): at the node-loop exit retail reloads the scaled box as esi (BRX), edx (TLX), ebx (BRY); ours reloads edx first | tried: the landed 0x008786E0 aligned-frame loop with an STLport list insert, BRX-first box stores (rescheduled the box, order unchanged), a dead local (inert) | next: lane gap: E-18 node-loop reload order
- 00920200 CThingSearchTools::GetAllThingsInRoughArea<CThingObject,vector<CThingObject*>,Bind2<CanBeUsedAsCover,NotInList>> DIFFER(640v640): at the node-loop exit retail reloads the box bound into edi first; ours esi, edx, then edi | tried: an extern find v a same-TU find stand-in (frame), a stand-in ending in an external call (registers), a named `last` local | next: lane gap: E-18 reload order
- 008f9320 LSocket::Receive DIFFER(674v674): retail loads the recv socket argument into eax after pushing the buffer pointer (`push eax ; mov eax,[esi+38h] ; push eax`); ours hoists it into ecx before the pushes, and the dependent lea registers rotate | tried: argument spellings of the recv call and the resize temporary (4 grades) | next: a named socket local read right before the call; the landed Listen/Open report-string model for the cold block
- 00854180 NCameraModes::CScriptModuleSpline::Update DIFFER(364v364): frame 0x30 v 0x2c; retail packs the inlined GetTC key (0x3F) into the roll local's slot, ours gives the key its own slot | tried: pos/direction declaration order, scoped locals (4 grades) | next: put the key and the roll in disjoint scopes, or make the key a const local inside the inline GetTC (R-15 slot ranking)
- 008d9ca0 NCreatureMode::CModeScorpionKingVulnerable::OnDeactivate DIFFER(393v394): in the kill re-index retail emits `mov eax,[esi+0x88] ; push 1 ; lea ecx,[ebx+eax]`, ours `mov ecx,[esi+0x88] ; add ecx,ebx ; push 1` | tried: LowerBound key forms, index-expression spellings (4 grades) | next: the element address through a named base pointer (SIB-variable recipe)
- 008e28f0 NCreatureMode::CModeThunderLightningStorm::FrameUpdate DIFFER(369v375): VC7.1 sinks the duration subtraction below the `elapsed >= 0` test; retail computes it first (`fld st(1) ; fcomp [0.0]`) and pops three values on the skip path | tried: statement order, named float temps (4 grades) | next: lane gap: x87 scheduling
- 009c8c00 NGameText::CDataBank::NotifyEntryAdded DIFFER(469v469): retail builds each pushed address in ecx right before `lea ecx,<this>` and keeps the narrator value in eax until after the push; ours uses eax and stores NarratorIndex early | tried: a `__true_type` tag with a user ctor, a same-TU overflow stand-in, the narrator loop form (4 grades) | next: named pointer locals for the pushed addresses, or a same-TU push_back stand-in
- 0077ba40 NGraphicAppearance::CTCGraphicAppearanceStaticMesh::OnAppearanceDraw DIFFER(559v563): several differences remain:
  - frame 0x98 v 0x90: the position temporary gets its own slot, where retail reuses the CPrimitive temporary's;
  - the x87 interpolation sums run Z,Y,X in ours and X,Y,Z in retail;
  - ours loads RenderSizeX first, retail Scale;
  - &LightingChannel is CSE'd into ebx, which moves the ebp/ebx prologue pushes and the UniqueID loads.

  | tried: inline counted-pointer ops with the count in a second base, named bulletTime/alpha locals, named orientation/position locals (identical code), `(E-S)*t + S` (identical) | next: inline accessors (`GetThing()->GetUID()`, `GetAlpha()`): retail evaluates all call-containing args right to left (bulletTime, alpha, orientation, position, UniqueID) before any push. The source also needs the 76-byte CEnginePrimitiveStaticMesh from its ctor 0x0077BD70 (MeshEffect at +0x3C; the differ source still has 0x48/+0x38) and a named CPrimitive in its block

## Skipped

These 10 rows were not attempted, so there is no differ source. They are large (611-1023 bytes) and need new models; the grade budget went to the cheaper rows first. None of them is a two-function row.
- 008f8f50 LSocket::Send (927): an EH try/catch(...) handler (`Close(); return false`) plus /GS. The vector<char> appends are inlined as `resize(old + n)` + memcpy, and the try-scope dead stores of the inline helpers' locals stay in. The landed LSocket rows (Close 0x008F8C70, IsWriteable 0x008F82E0, ConvertLAddress2Sockaddr / CreateSocket) give the callees.
- 008e20f0 / 008e24f0 CModeGuildmasterPowerUpShield / Speed vfunc_3 (1019 / 1023): twins (the Speed body adds one GetInterface). x87 averaging of two bone positions, then a loop over two CIntelligentPointer vectors with GetPItem 0x00A01B50.
- 008e0a10 CModeScorpionKingVulnerable vfunc_3 (FrameUpdate, 776): five LowerBound GetInterface lookups (0xAD, 0x5A, 0x31, 0x7F), a CCharString temporary, and a creature-action temporary for SetCurrentAction 0x006644F0. The parked 008d9ca0 model covers the class.
- 008dfb00 CModeSummonerLightningOrb vfunc_3 (1001): the same family as the power-up modes.
- 0077b350 / 007e15c0 / 007a6760 / 0077c4d0 / 0077bec0: the other CTCGraphicAppearance* OnAppearanceDraw rows. The model is in differ/0077ba40.cpp:
  - MI: the CTCBase + CAGraphicAppearanceComponent `this`;
  - CPrimitive {Type, CEnginePrimitiveHandle, BankIndex} with the out-of-line ~CPrimitive 0x004C4160;
  - GetTintColour 0x004BD040 built in its argument slot;
  - the CEnginePrimitiveStaticMesh ctor 0x0077BD70.

  The same position-temporary frame gap is expected. 0077bec0 additionally does GetSubDef 0x0042B0A2 on the owner's def, uses the debug byte 0x013756F4, and builds a `new` (0x1C bytes) fade mesh effect (ctor 0x0077C210).

## Label corrections

- 0044a3b0 "HeroSwapState::Initialize" is CPlayerManager::CPlayerManager (vptr .?AVCPlayerManager@@).
- 007adbf0 "GFSerialiseMapBinaryIn<pair<long,long>,CCharString,...>" is GFSerialiseMapBinaryIn over map<CCharString,bool>.
- 008c2f70 "Geometry::FindClosestPointInRadius" is std::__find_if<C3DVector*,CIsOutsideRadius> (the landed remove_if 0x008C34A0 calls it).
- 008d6ac0 "CModeSitChair::vfunc_2" is CModeSitBase::AnimUpdate; landed 0x008D6C80 is CModeSitBase::FrameUpdate.
- 008d8b00 vfunc_2 = CModeCrouch::AnimUpdate; 008e7390 / 008e74f0 vfunc_5 / 6 = CModeIntangible::OnActivate / OnDeactivate; 008d9ca0 vfunc_6 = CModeScorpionKingVulnerable::OnDeactivate; 008e28f0 vfunc_3 = CModeThunderLightningStorm::FrameUpdate; 008fa3b0 vfunc_0 = CPhysicsCoordCartesian::FrameUpdate; 00854180 vfunc_1 = CScriptModuleSpline::Update; 009c8c00 vfunc_12 = NGameText::CDataBank::NotifyEntryAdded.
- 009793d0 "GetPBestThingInRoughArea<Bind4<IsAICreatureAwareOf,...>>" is GetAllThingsInRoughArea<CThing,vector<CThing*>,Bind4<CanBeUsedAsCover,not1(IsOccupiableOccupied),IsNot,In2DRange>> (caller GetCoverThingList 0x00978FD0).
- 0063fd40 "ForEachInRoughArea<CThingPhysical,Bind3<...>>" is GetAllThingsInRoughArea<CThing,list<CThing*>,Bind2<OverlappingCircle,CWorldMap::CThingFilterBase>>.
- 00920200 "GetAllThingsInRoughArea<CThingGameObject const,vector<CIntelligentPointer<CThing const>>>" is GetAllThingsInRoughArea<CThingObject (type mask 0x807),vector<CThingObject*>,Bind2<CanBeUsedAsCover,NotInList>>.
- 0084ba50 is the vector instance of GetAllTCsInRoughArea<CTCPerceiveThing,Bind2<IsPerceivingThing,IsNot>> (the landed 0x0091BA40 is the list instance).
- 00518340 "GetAllTCsInRoughArea<CTCPerceiveThing,Bind2<...>,list>" is GetAllTCsInRoughArea<interface 0x3D,CThingFilter_IsInQuestOrNullQuest,vector>.
- The six CTCGraphicAppearance* "vfunc_0" rows are OnAppearanceDraw(CTCGraphicAppearance&, const CGraphicAppearanceDrawInfo&) (Ego_r.pdb, slot 0 of the CAGraphicAppearanceComponent vtable).
- Callees:
  - Search tools:
    - 0x0049C770 "CVertexBufferWin32::DoSizeof" = CThingSearchTools::PeekAllThings;
    - 0x0091F680 "LinearSearch_StridedEquals" = an STLport find;
    - 0x006CE730 "CTCInterface_InsertPropertyToVector" = (anon)FrameUpdateThing::operator().
  - Graphics:
    - 0x004BD040 "Appearance_FloatToCoord" = CTCGraphicAppearance::GetTintColour;
    - 0x0099A2D0 "CBase::CBase" = CBaseClass::CBaseClass;
    - landed 0x004C4160 "SPrimitiveHandleHolder" (manifest ~CObject) = NGraphicAppearance::CPrimitive::~CPrimitive.
  - Strings and STL containers:
    - 0x0099E900 = CCharString::operator!=;
    - 0x007ACC50 = map _M_erase;
    - 0x0086D900 = vector<pair<ulong,CTextureMorph>>::resize;
    - 0x00412880 "_Umove<EWeaponClass*>" = vector<char>::resize(n);
    - 0x00993B10 = ~CMemoryDataInputStream.
  - Creature and boss:
    - 0x00664670 = CThing::SetCurrentAction(const CCountedPointer&);
    - 0x007AED10 "DrawGetEnvironment" = CTCBoss::GetBossPhase;
    - 0x00693EF0 = ~CCreatureAction_PlayAnimation.
  - Math: 0x00A14440 = C3DVector::Normalise.
