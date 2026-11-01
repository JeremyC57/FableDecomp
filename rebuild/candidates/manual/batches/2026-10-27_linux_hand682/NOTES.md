# Hand chunk 682 (CTCCoopSpirit .. CTCWife static init): 7 of 10 byte-exact

All seven are RELOCATION_MATCH (every row carries relocations: vtables, callees, string literals or constants);
tests PASS under g++ and VC7.1 (`tmp/tall_vc.log`).

## What matched (7)
- 0x0121E260 `CTCWife::WifeAppearanceDialogueGroupStemNames` dynamic initializer (`_$E1`): a braced `CCharString[16]`
  static member initialised from the 16 `TEXT_AI_WIFE_OPINION_ACT_CLOTHING_<X>_` literals through the implicit
  `CCharString(const char*, long = -1)` conversion, `atexit` of the `_$E2` destructor. Identity: Ego_r lists three CTCWife
  dynamic initializers in .CRT$XCU order (MarriageEnabledMaleVoice 0x0121E1F0 "AM3", CourtingAdviceGroupNames 0x0121E210,
  then this); landed 0x00985F40 indexes 0x013BAB40 by EWifeAppearanceCategories.
- 0x00F26250 / 0x00F34330 `CPersistContext::TransferVectorOfSubComponents<CArenaCreatureDef>` / `<CFeatDef>`: the landed
  0x00F349F0 template body (CheckCRC, MODE_RESET / SAVE_BINARY / LOAD_BINARY / LOAD_TEXT switch over Mode +0x18, `__assume(0)`
  default) with the real STLport two-step `resize(n)` -> `resize(n, _Tp())`, the per-element wrapper
  `CPersistContext_TransferableVectorOfSubComponents<T>` for the text arm, and `engine/CDefClassBase.h` under
  `FABLE_RETAIL_DEF_CLASS_BASE`. CArenaCreatureDef (0x38: two CCharString + two long, user dtor 0x00F25E40) and CFeatDef (0x64,
  no user dtor: the implicit `~CFeatDef` inlines as the CDefPointeeBase vtable store + `~CResource` 0x009FC550).
- 0x00B513B0 `CEngineWeatherRenderer::~CEngineWeatherRenderer`: user empty destructor; the members are the landed 0x00B52250
  models (CVertexBufferWin32 / CIndexBuffer with inline `Uninitialise()` destructors, CVertexShader / CPixelShader releasing
  their reference-counted data, the CEngineWeatherSettings member with an inline empty virtual destructor) over a
  CEngineComponent base (dtor 0x00B59730). Needed `<stddef.h>` for the FABLE_STATIC_ASSERT offsetof.
- 0x00BBBBC0 `CEnginePrimitiveManagerDecal::~CEnginePrimitiveManagerDecal`: implicit destructor (no vtable store of its own)
  emitted by defining the PDB constructor with an empty body; base chain CEnginePrimitiveManager : CEnginePrimitiveRenderUtil :
  CBaseClassNonCopyable from RTTI 0x012A5B6C.
- 0x00D6ED60 `NScript::CQ_HangingTreeEvilScript::~CQ_HangingTreeEvilScript`: implicit destructor, the landed
  ~CQ_HangingTreeGoodScript 0x00D782E0 recipe (FABLE_RETAIL_NSCRIPT headers, inline `~CTimer` = DeregisterTimer slot 88,
  STLport vector destructors with `free`), members from the landed constructor 0x00D68F00.
- 0x00788617 `CTCMeleeTrap::ProcessPossibleStrike(float, float, const CPointPair&, const CPointPair&, CThing&)`: a /Os TU
  (`#pragma optimize("s", on)` alone gives the biased `lea ebp,[esp-64h]` frame); the triangle `vector<C3DVector>` lives in its
  own block so its `free` precedes the hit test and the direction reuses its slot; the hit built inside
  `if (hitLocation != 0) { ... }`; `pair<CIntelligentPointer<CThing>, float>` converted by the out-of-line pair
  constructor 0x00788D68 on `HitThings.push_back` (the intelligent-pointer constructors 0x004A96F0 / 0x004A9730 store the
  `CIntelligentPointer<CThing>` vtable 0x01238C6C; the `<const CThing>` instantiation is 0x0056C860, vtable 0x01237F1C); `if (lengthSquared > EPSILON * EPSILON) AccurateNormalise()` (landed
  0x00805A90 form). Ego_r's GetPCombatFirstHitLocationIntersectionWithTriangle takes `const vector<C3DVector>&` (the landed
  0x008169B0 comment calls it a CTriangle).

## Parked for Fable 5.1 (3)
- 0x0078891D CTCMeleeTrap::PerformPossibleTrapDamage DIFFER(463v466): retail builds the filter temporaries, then GetPos + two
  __ftol2 (centre), then GetWorld, then the radius __ftol2 + C2DBoxI temp; the lane evaluates the radius before the centre;
  the CTrapDef pointer is loaded late in retail; the three 12-byte vectors take mirrored frame slots (trails lowest in
  retail); the pair loop keeps the vector start in ecx (`pop ebx ; idiv ebx`, ebx re-zeroed) where retail re-reads it
  (`pop ecx ; idiv ecx`) | tried: temporaries in the call / named filter locals / named Bind4, C2DCoordI(long,long) and
  C2DCoordI(const C3DVector&) temps, named centre, named pos reference, inline def, GetTC out-param / pointer forms,
  iterator-object vector, block scopes, declaration-order experiment, out-of-line vector-of-vectors destructor (the lever
  that fixed the epilogue) | next: a search helper that takes the centre before the object expression (an inline
  CTCBase/CThingSearchTools wrapper), or lane gap: argument evaluation order of call-containing temporaries.
- 0x00783850 CTCBoastingCrowdControl::Startup DIFFER(443v443): the CThingCompare_Nearest position copy -- retail
  `mov edx,[eax] ; mov ecx,[eax+4] ; mov [esp+20h],edx ; mov edx,[eax+8] ; mov [esp+24h],ecx` (bitwise 12-byte copy
  interleaved with the argument `lea`s), the lane three load/store pairs | tried: implicit / user copy ctor, body assignment,
  component-wise init, by-value compare, pointer-returning GetTC (fixed the stats call), expression-form region compare
  (fixed the movsx/edi order), named map/region locals, explicit template instantiations, read-only stand-ins, /GS |
  next: lane gap: 12-byte struct copy scheduling (compare temporary built from a const& call result).
- 0x00670820 CTCCoopSpirit::UpdateMovement DIFFER(483v483): retail squares Z, Y then X (X*X kept on the x87 stack at
  [esp+8]) and loads the velocity copy Z, Y, X; the lane squares Z, X then Y whatever the sum order | tried: every sum
  order and parenthesisation in GetLength and the fast Normalise, named squares, named lengthSquared, shared
  GetLengthSquared, copy-ctor init order and body order, getters/setters, operator=, the alias-read trick (R&D round 3) |
  next: lane gap: x87 operand rank of CSE'd squares.

## Skipped
- none.

## Label corrections
- 0x0121E260 catalog `_gapscan::sub_0121e260` = CTCWife::WifeAppearanceDialogueGroupStemNames dynamic initializer; the
  landed 0x00985F40 spells the array `g_WifeAppearanceLinePrefix` (NAME_FIXES).
- 0x00F26250 / 0x00F34330 catalog `CPersistContext::Transfer<signed_char>` = TransferVectorOfSubComponents<CArenaCreatureDef>
  / <CFeatDef>; 0x00F344C0 (catalog CAIStateGroup_HoverHornetQueen / CVtable012fb778) = CFeatDef::CFeatDef; 0x00F263D0
  (CFireheartMinigameDef) = CArenaCreatureDef::CArenaCreatureDef; 0x009BC120 gapscan = TransferVectorOfSubComponentsLoadText.
- 0x00BBBBC0: the landed ??_G 0x00BBBBA0 binds it as `CEnginePrimitiveManagerDecal_dtor` (PDB `??1CEnginePrimitiveManagerDecal@@UAE@XZ`).
- 0x0078891D: the landed UpdateTrap 0x00788AEF spells it `MeleeTrapHelper` (`__stdcall`); it is the private
  `CTCMeleeTrap::PerformPossibleTrapDamage(float, float)` __thiscall member.
- 0x007898CC: landed as `CFileInstaller::CActiveFile::OnReadFinished` / labelled `StdList_Dtor_007898cc`; it is
  `vector<vector<pair<C3DVector,C3DVector>>>::~vector` (only call site 0x00788AE3).
- 0x00A14440 reloc binding `GetScaled` = C3DVector::AccurateNormalise (Ego_r); 0x00868CA0 binding `SetEditBoxRegion` =
  CHitParameters::AddDamageWithAttribute; 0x00788D68 binding `CTCMoneyReceiver` = the pair converting constructor (calls.py
  names are the catalog's, the sources use the PDB spellings).
- 0x0066E630 is `vector<C3DVector>::push_back` (three calls from 0x00788617); 0x00789515 is
  `vector<pair<CIntelligentPointer<CThing>,long>>::push_back`. The landed 0x00788D68 file names the pair
  `pair<CIntelligentPointer<const CThing>,long>` and 0x004A9730 "for <const CThing>": both constructors store the
  `CIntelligentPointer<CThing>` RTTI vtable 0x01238C6C (the `<const CThing>` constructor is 0x0056C860, vtable 0x01237F1C,
  as the landed 0x0064A2DB binds it) -- NAME_FIXES.

## Names proven vs descriptive
- Proven (two sources): all seven matched rows and the three parked rows (Ego_r name + caller/vtable evidence in each
  source header). `CTCCoopSpirit::GetMaxVelocity` is inline returning the literal 1.027f (Ego_r name, retail constant 0x0125B250).
- Descriptive: `g_framesPerSecond` 0x01375550 (no PDB name; the landed majority spelling), `g_boastingEnabled` 0x013833D8
  and `BOASTING_CROWD_MARKER_SCRIPT_NAME` 0x013BADEC (parked Startup only), the local enum spellings
  `TC_INTERFACE_WEAPON_TRAIL = 0x4c`, `TC_INTERFACE_HIT_LOCATIONS = 0x77`, `TC_INTERFACE_HIT_LOCATIONS_2 = 0x7f` (values are
  retail; the ETCInterfaceType enumerator names are not in the PDB name set).

## Header debt
- engine/CTCPhysicsBase.h, engine/CTCBase.h, engine/CThingCreatureBase.h are flattened (no virtuals): the parked
  UpdateMovement models CTCBase/CTCPhysicsBase with the landed 0x007AF0F0 virtual prefix (slot 66 SetFacingAngleXY).
- engine/CEngineWeatherRenderer.h / CEngineComponent.h / CVertexShader.h / CIndexBuffer.h / CVertexBufferWin32.h /
  CMovableResource.h are flattened: 0x00B513B0 and 0x00BBBBC0 use local member models with `// Layout:` lines.
- CTCTrapBase / CTCMeleeTrap / CTrapDef / CWorld (PThingSearchTools +0x20) / CHitParameters / CThingSearchTools have no
  usable engine header: local models in 0x00788617 and the parked 0x0078891D (Ego_r offsets minus the 4-byte-per-container
  STLport shift).
- CLevelScriptBase<T> destructor and CTimer destructor are not in the generated headers (0x00D6ED60 defines them inline).
- Landed GetCrowdGroup 0x00783730 says CrowdSizesSuppressed +0x18; Ego_r (12-byte map) puts it at +0x1c (Startup uses +0x1c).

## Callee names
- 0x00410DE0 `?test@?$bitset@$0BBC@@std@@QBE_NI@Z`, 0x0040F020 CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound,
  0x0076EA60 CTCWeaponTrail::GetAllTrailsLastWeaponTrailPoints, 0x007896AF / 0x007896E6 std::_Transform (anonymous-namespace
  CGetThingFromIntelligentPointer), 0x004C73D0 CThing::GetPos, 0x00BFEA70 __ftol2, 0x00686A70 CTCBase::GetWorld,
  0x00434740 C2DBoxI::C2DBoxI(const C2DCoordI&, long), 0x0078971D CThingSearchTools::GetAllThingsInRoughArea<CThingGameObject,
  vector<CThingGameObject*>, CFilter_Bind4<...>>, 0x008169B0 CTCHitLocations::GetPCombatFirstHitLocationIntersectionWithTriangle,
  0x00A14440 C3DVector::AccurateNormalise, 0x00789238 / 0x00789358 CHitParameters ctor/dtor, 0x00868CA0
  CHitParameters::AddDamageWithAttribute, 0x004A96F0 / 0x004A9730 CIntelligentPointer<const CThing> ctors, 0x00788D68 the
  pair converting ctor, 0x00789515 the HitThings push_back, 0x00A01C10 ~CBaseIntelligentPointer, 0x00BFEA14 free,
  0x0099EBF0 CCharString::CCharString(const char*, long), 0x004012BC _atexit, 0x00993EB0 CDataOutputStream::WriteLong,
  0x00993CA0 CMemoryDataInputStream::ReadWithSrcChunkOverflow, 0x00404500 CPersistContext::CheckCRC, 0x009BC120
  TransferVectorOfSubComponentsLoadText, 0x00F26490 / 0x00F34580 vector<T>::resize(n, x), 0x00B59730 ~CEngineComponent,
  0x00A63200 CVertexBufferWin32::Uninitialise, 0x00AC1EE0 CIndexBuffer::Uninitialise, 0x00BACDB0 CEnginePrimitiveManager ctor,
  0x0099A300 ~CBaseClassNonCopyable, 0x009FC550 ~CResource.

## vland (read-only run, tmp/vland.log)
- 7 WINS, no RELOC_IDENTITY / DIFFER. Warnings: SHARED_HEADER_AVAILABLE for CDataInputStream / CDataOutputStream (0x00F26250,
  0x00F34330: the landed 0x00F349F0 recipe's inline stream models were kept because the generated headers are Dinkumware-
  shifted and the inline Read/ReadLong bodies are what retail inlines -- header debt) and CTCWife (0x0121E260: only the static
  array is declared, the generated header has no statics); RELOC_NAME warnings are catalog labels (WriteSLONG = WriteLong,
  the CFeatDef / CArenaCreatureDef constructors, `_$E2` = this TU's static destructor, labelled CreateCRC); the
  RELOC_CONFLICT on `_$E2` is expected: every static-initializer TU has its own `_$E2` (landed 0x01219F00's is 0x01228930).
