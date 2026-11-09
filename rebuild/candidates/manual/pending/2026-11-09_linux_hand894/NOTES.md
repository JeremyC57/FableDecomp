# Chunk 894 notes

14 matched (13 twins + stretch 0x00C720C0), 9 parked (twin 0x009AA440 + 8 stretch rows). Every src/ row has a test/ file that
passes under g++ -std=c++03 (tall.sh) and under VC7.1 /Od /W3 through verify_and_land.behaviour_of (tall.py: 14/14 PASS).

## Identities proven (row: evidence)
- 0x00436D60 / 0x00863E70 / 0x008ADCC0 / 0x00978250: CThingSearchTools::GetAllThingsInRoughArea<CThing, vector<CThing*>, F> with
  F = OverlappingCircle (0x0063EF50) / HasPhysicsMesh (0x006A6F50) / HasTC (0x006A80A0) / HasScriptName (0x006A7E30); the filter
  call is the only differing relocation against twin 0x0096BCB0, and both PDB name sets hold the four instances.
- 0x004AD810: GetAllThings<const CThing, vector<const CThing*>, CThingFilter_None> (FableWin CWorld TU; caller 0x004A0520; the
  overflow callee 0x0049C370 is vector<const CThing*>). Written on real STLport list/vector models with PeekClassList 0x0049C770.
- 0x00702D10: CThingObject::DrawGetEditDrawInfo (vtable 0x01264DCC slot 25; twin 0x0083D710).
- 0x00739030: CTCCombat::ResetBlockPercentageChance<CCombatAbilityBlockUnarmedAttackDef> (GetSubDef 0x007380D0 pushes the class name).
- 0x00772490: CTCShop::IsBarberShop (CShopDef +0x99; twin IsTattooShop +0x9A).
- 0x007921A0: descriptive CUnprovenRbTree_007921A0::begin(): no reference anywhere in the exe; probably an STLport
  _Rb_tree::begin() COMDAT in the CTCDivineWrath TU. The real owner still needs proving.
- 0x00B84190: CEngineClothSimulationAnimatedMesh ??_G (vtable 0x012A39C0 slot 0, RTTI).
- 0x00C6EEF0: libpk CVtable_012b8a8c::Process (SSE2 gain-ramp mixer; vtable 0x012B8A8C slot 2; twin 0x00C6FE10). Wrappers in
  namespace Row00c6eef0 (MixGainRampUpSSE2 etc., retail 0x00C6EE60 / 0x00C6EE90 / 0x00C6EEC0) are same-TU bodies, not rows.
- 0x00F28990: vector<CHeroSoulsWaveDef>::_M_fill_insert (element CHeroSoulsCreatureDef via NumWaveCreatures / Creatures).
- 0x00F3FB60: vector<CArray<unsigned short> >::resize(size_type) -> resize(n, x) 0x00F3FAF0.
- 0x00C720C0: CAudioSource::StartExponentialFade (descriptive libpk name already used by two landed TUs; see NAME_FIXES).

## Callee names used (all bound in reloc_bindings.tsv unless noted)
GetSquaredDistanceToInfiniteLine 0x00A442E0; GetSubDef<CCombatAbilityBlockUnarmedAttackDef> 0x007380D0; CThingFilter_* operator()
as above (0x006A80A0 must use the ABVCThing spelling); __ftol2 0x00BFEA70; ScaleAccumulate16Ramp 0x00C6ED70 (same-TU body) and
ScaleAccumulate16 0x00C6ECD0 (extern).

## Header debt
- 0x00B84190 / 0x00C720C0 tests model their classes locally (tests must be self-contained); sources use the engine headers.
- 0x008C7280 (parked): local CDefPointer / C3DVector / CCharString / CTrollBattleDef models; the method is declared non-virtual
  (obj leaf QAE, retail UAE). There is no engine/CCreatureAction_TrollGroundPound.h; build it from engine/CCombatActionBase.h.
- 0x00A17620 (parked): local CPhysicsMesh with a pad up to BoundingSphere +0x74 (no engine/CPhysicsMesh.h).

## Unmatched rows and next steps
- 0x009AA440: bytes identical to the landed 0x009A8FE0 (~pair<const CCharString, CBankFileManager::CBankInfo>). It is a second
  COMDAT copy from another TU that instantiates map<CCharString, CBankInfo>. There are no references, and neighbours 0x009AA210 /
  0x009AA470 twin 0x009A7670 / 0x009A9010. Needs a twin-alias record from the coordinator, or a FableWin contribution scan for the
  second TU.
- 0x00A17620: commutative fadd order (member first in retail). 32 grades. The hand892 scheduler dump shows the double cast fixes
  the order but adds a 0x269 tuple. Next step: find what makes c2 pick the member as the first operand (candidate-id rule) in the
  plain float DAG.
- 0x00D259A0: parent pointer in eax (retail esi). 9 grades, including a shared-header rebase on engine/CQ_DragonBossFightScript.h
  (now the differ source). Next step: c2prio --trace on a form that keeps `this` live into the store block.
- 0x00492ED1: the ToWideString temporary is homed in the dead pauseGame slot. Next step: frame-packer replay (hand886 pack.py).
- 0x00C11520: one store before `push eax`. Settle the method name, which is descriptive (vtable 0x012B2748 slot 27); the catalog's
  Inventory_ProcessNavigationItem is unproven. SEventRecord ctor 0x00C20390 is named per vendor-V1 DECISIONS (??0SEventRecord@@QAE@XZ).
- 0x008C7280: rebase on the shared headers first, then measure the `range` frame word.
- 0x009350C0: x87 scheduling. Next step: the hand892 sched.sh dump against retail's order (tools copied to tools/h892, paths fixed).
- 0x009E1960 / 0x009E1A80: guard jump threading. 13 grades of source forms and both optimisation devices failed. Next step: make
  `negative` not provably false on the guard edge.

## Tools left in tools/
gen_rough.py, chk.sh, rstr.py, refs.py, gp.sh (grade a probe with its own Leaf / Flags), and h892/ (the hand892 tools with this
lane's paths; `bash tools/h892/sched.sh <src> <out.json> '{"target":..,"pat":[..]}'` works).
