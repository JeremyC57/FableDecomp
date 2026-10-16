# Hand chunk 175 (CThingPhysicalSwitch .. CThingVillage): 38 of 40 byte-exact

## What matched (38)
- CThingSearchTools (31): GetPFirstThing / GetPFirstLeafThing / GetPBestThing / GetPBestLeafThing instantiations, all from one shared template model (gen/model.txt + gen/methods.txt, per-row spec gen/rows.py, generator gen/gen.py, tests gen/testgen.py). Shape: the type test copies `T::ThingTypeMask` into a local (a constant enum operand folds the CThing mask-0 test away; retail keeps `xor al,al ; jne`); CThing::IsIncludedInThingScans inline (`not cl ; test cl,1` form); in GetPBest the candidate is re-read as `*it` for each use so VC keeps retail's `test esi,esi` null test (VC7.1 drops a null test on a pointer it saw dereferenced); Bind2/Bind3 hold filters by reference; IsChild / IsCreatureType / CIsHopscotchBoard / GetMostRecentlyCreatedThing inline, the rest out of line; 0x0065F744 is a size TU (`#pragma optimize("s", on)`: ebp frame, IsIncludedInThingScans out of line).
- CThingPlayerCreature::SerialiseGameState (0x006A98F0, vtable slot 3): base call `if` body with shared `mov al,1`; def value kept on x87 for MaxHealth and the Health==0.0f top-up; bitfield set gives `or byte ptr`.
- CThingSoundEmitter::~CThingSoundEmitter (0x00836A50): clearing the attached-thing CIntelligentPointer written as a named cleared pointer + `CThing* item = cleared.GetPItem(); member.SetPItem(item);` (the operator= form schedules `lea edi` before GetPItem).
- CThingSwitch::ConstructFromParams (0x0083BD80): the empty script name is a named CCharString in its own block (built before the pushes, destroyed before the result test).
- CThingTrackNode::SetTrackName (0x00836EE0): previous-node recursion + next-node tail call that VC turns into the loop.
- CThingTrackNode::DrawGetEditDrawInfo (0x008374F0, slot 25): CRGBColour(r,g,b,a) body-assigning ctor temporary, `CCharString("") + "TrackNode"`, UsesEffectRadius = false.
- CThingTrackNode::OnInitialActivate (0x00837830): two 64-bit UID resolves with the zero in ebx, isolated node marked start+end.
- CThingVillage::OnCreate (0x0083B610, slot 8): block-scoped named CCharString component names, appearance pointer read into a named local before GetCurrentGraphic, CEngineGraphic is the 14-byte PDB record (`#pragma pack(2)`, copied as 3 dwords + word).

## Parked for Fable 5.1 (2)
- 0083db60 CThingPhysicalSwitch::GetCurrentGraphic DIFFER(162v125): our CDefPointer dtor re-reads the escaped def after `dec [eax+4]` (retail `mov esi,[esp+4] ; test esi ; dec [esi+4] ; jne` then `lea eax,[esi+82h]`), and per-arm defs tail-merge | tried: one shared def, per-arm scoped defs (104v125), dtor via local, by-value GetSwitchDef helper inline/__forceinline | next: lane gap: CDefPointer release without re-read of the escaped local
- 006ac390 CThingPlayerCreature::IsExpressionAvailable DIFFER(153v151): retail `mov ecx,[eax+4] ; mov eax,[edi+70h] ; push eax`, ours `mov ecx,[edi+70h] ; mov eax,[eax+4] ; push ecx ; mov ecx,eax` (only the inventory call; the def holder now matches) | tried: && chain, named inventory local, __forceinline Find with reference key | next: lane gap: looked-up component used as this loaded straight into ecx

## Skipped
- none

## Label corrections
- 0x0049C770 is CThingSearchTools::PeekAllThings-style getter (`ThingManager+0x18`), not CVertexBufferWin32::DoSizeof.
- 0x006A80A0 ("Bitset_Test") is CThingFilter_HasTC::operator() (bitset test, range 0x112); so 0x006D3FB0 is GetPFirstThing<CThingFilter_HasTC,CThing> and 0x006156E0's filter is HasTC (labels say HasDefGlobalIndex / AICreatureHasProperty).
- 0x007E6EC0 calls CThingFilter_HasDefName (0x006A7BE0); 0x00777000 calls IsNot + In2DRange (label HasTC + InMap); 0x009223A0 calls IsNot, IsNot, a script-name filter (label HasTC/InRegion/IsRegionEntranceActive); 0x0093F930 calls In2DRange (label HasDefGlobalIndex); 0x004A9DA0's second filter is 0x006A7960 (CThingFilter_InRegion, label In2DRange). Same template shapes, only names differ.
- 0x007EE660: mask 0x407 = CThingVillage (label CThingMarker const, whose mask is 0x8007); 0x00512F60: mask 0x47, not CThingGameObject (mask 7, 0x008ABDF0).
- 0x0093B640 / 0x00939AF0 call CIsNearCampfireWithSpace::operator() 0x00939A50 (labels CIsFreeBonfire / CIsFreeHopscotchBoard).
- 0x006156E0 walks type list 0xB, 0x00685EE0 list 1: both labelled CThingAICreature, at most one is.
- 0x0099E900 is CCharString::operator!=(const CCharString&) (label less<CCharString>::operator()); 0x004C7990 is the global definition-manager getter (label CViewBase::GetDisplayEngine); 0x0051ED80 GetThingWithUID (label CStopWatch::GetTicks); 0x00593666 is GetDef<CExpressionDef> (label GetDef<CHeroPostcardGeneratorDef>).
- vfunc labels: CThingPlayerCreature slot 3 = SerialiseGameState, CThingTrackNode slot 25 = DrawGetEditDrawInfo, CThingVillage slot 8 = OnCreate.
