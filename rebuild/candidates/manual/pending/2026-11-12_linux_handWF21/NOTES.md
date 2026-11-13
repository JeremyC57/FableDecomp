## 0x004CE1B0


## 0x004CE1B0 CSubtitledMessage::AddMasterShotCameraTagFromText -- RELOCATION_MATCH (20 grades)
- What made it match: port onto engine/CStringParser.h + engine/CVectorMap.h (Insert(key,value) value_type opt-ins, CArray push_back
  out of line), signature (float time, const CCharString& text), FindSymbol(const CCharString&, long&) const on g_MiscDefTypesSymbolMap,
  a CCameraTag local (scalar-replaced; only CameraPosition spills to +0x24). Three source facts fixed the rest: (1) the protagonist
  fields are assigned right after `CCameraTag tag;` at the top (one shared zero in eax at the pair build); (2) `position`/`focus`
  declared at the top, `view` inside the third-lookup block (frame slots); (3) lookups as `if (Find(first)) { ...; if (!Find(second))
  return false; ...; if (third != "") { if (Find(third)) view else return false; } Insert; return true; } return false;` -- puts the
  failure blocks after the success path and gives retail's cross-jump into the ")" exit with ecx=&third preloaded.
- Headers / flags: defines FABLE_RETAIL_CCHARSTRING, _STLP_LIST, _CSTRINGPARSER, _CARRAY_PUSHBACK_NOINLINE, _CVECTORMAP_INSERT_KEYVALUE,
  _CVECTORMAP_INSERT_VALUE_TYPE (all existing repo opt-ins; no overlay); no flags. Strict-headers regrade RELOCATION_MATCH.
- Names: CSubtitledMessage has no shared header (local model, as all landed CSubtitledMessage rows); enumerators copied from landed
  0x00712340. NAME_FIXES candidate (from DOSSIER): 0x004D18E0 landed as CRetailVector_SPair28::PushBack_SPair28_004d18e0, Ego_r
  004f04a0 says CArray<std::pair<float,CSubtitledMessage::CCameraTag> >::push_back.

## 0x006A5610


## 0x006A5610 CThingPhysical::GetFirstCylinderCollisionBetweenPoints -- RELOCATION_MATCH
- What made it match: (1) ported onto shared headers with the PDB callee names (GetRHSet().GetUp(), GetPos, GetMagnitudeAndNormalise,
  AccurateNormalise, GFGetShortestDistance2DSquaredBetweenPointAndLine with float&); (2) the GF 2D helper's REAL landed body (0x00A443A0)
  in the TU as `inline __declspec(noinline)` -- only then is `t` homed in the dead `to` arg slot (frame 0x5C -> 0x58); (3) natural nested
  if-blocks (`if (d2 > eps) { ... if (A && B && C) { ... if (Z tests) return true; } } return false;`) give retail's frame packing and the
  shared `fstp st(0)` exit; (4) default one-line C3DVector::Dot (the DOT_ORDERED_ZYX opt-in left the hit_normal pointer load late).
- Headers/flags: overlay opt-in FABLE_RETAIL_CTHINGPHYSICAL_CYLINDERCOLLISION (engine/CThingPhysical.h, declaration only); no flags/pragmas.
- Proven wrong in old draft (not landed): GetUp virtual / GetPosition / GetLength / Normalise / GetClosestPointOnLineSegment(float*).

## 0x007D71E0


## 0x007D71E0 CTCTavernGameCardPairs::ShuffleCards -- RELOCATION_MATCH (wf21, ~25 grades)
- What made it match: port onto shared headers (card_sorting_types.h, CDefPointer COUNTREF, CTCTavernGame HideMesh/PositionMesh opt-ins,
  STLP_RANDOM_SHUFFLE_2ARG + VECTOR_INSERT_OUTOFLINE + VECTOR_OVERFLOW_DECL_ONLY) with the STLport random_shuffle body in the TU (one site
  called, one inlined); `pdef->CardOrders[GetCardOrder()]` inline; counting-loop iterator advances; loop 1 as a scan whose match branch
  moves the card and `goto next` past ++i, compare `(*it)->Value == (*p)->Value`; loop 2 reads the partner card into a local `other`.
  Last residue (card reloaded after memmove / its store hoisted) was callee-body visibility: the REAL body of _M_insert_overflow
  0x007D5D60 placed in the TU as `__declspec(noinline)` under auto_inline(off) -> c2 knows push_back never captures `card`.
- Headers: overlay engine/CTavernGameCardBaseDef.h adds FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_CARDORDERS (std::vector<CCardPositionsDef>).
  Flags: none. Out-of-line real bodies in the TU: std::random_shuffle (0x007D7860 template), vector::_M_insert_overflow (0x007D5D60).
- No landed name proven wrong.

## 0x00E83230


## 0x00E83230 CFisticuffsClubRing::SetRelevantRegion -- RELOCATION_MATCH (author wf21, 2026-11-12)
- Matched: ported onto shared headers (Me/Game from CEntityScriptBase.h, parent CV_FisticuffsClubScript RelevantRegion/Village,
  CCharString text == opt-ins, FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT), CFisticuffsClubRing nested locally (no shared header).
  The plain form (823v820) called op= at all four sites; /d2inlT100 matched it (structure right, arm 1 hotter than written);
  source-level fix: arm 1's `ParentClass->Village = ...` inside a single-site `do { } while (0)` (lever of landed 0x00DA91A0).
- Defines: the same-TU block of 0x00E84B80 (no flags, no pragmas, no overlay headers).
- NAME_FIXES: 0x00E83230 = ?SetRelevantRegion@CFisticuffsClubRing@CV_FisticuffsClubScript@NScript@@AAEXXZ; landed Main 0x00E82950
  spells it `SetUpRing()`, manifest `_global::HandleVillageSelection` (evidence: DOSSIER sec. 2).

## 0x00B4AC10

## 0x00B4AC10 CEnginePrimitiveRenderer2D::CEnginePrimitiveRenderer2D -- RELOCATION_MATCH (17 grades)
- What made it match: port onto shared headers (overlay opt-ins in hdr/README.md) with corrected manager classes / PDB member names; AddPrimitiveManager's REAL body (landed 0x00B4ABB0) in the TU before the ctor under `#pragma auto_inline(off)` (fixed the whole Info-in-register release residue at once); LightVector and the three colours written through reference locals (`C3DVector& light = LightVector; light.X = ...`), which gives retail's `lea ecx,[esi+7Ch]` stores and the loop-index xor placement; FOV spelled `g_TwoPi * (FOV_2D * (1.0f/360.0f))` (literal 2pi folds the two multiplies; `x*c1*g_TwoPi` loads the constant first).
- Defines: FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS (class path of the shared MeshBase/StaticMeshesBase headers), FABLE_RETAIL_CENGINEPRIMITIVERENDERER2D(+_MEMBERS), _CENGINECOMPONENT, _CCOUNTEDPOINTER_RESET_CAMERA_DECL (Reset out of line), _CENGINEDEF_FOV_2D, _2DSPRITES_MANAGER, _2DPARTICLEGROUP_MANAGER, _CENGINEPRIMITIVE2DVIEWPORTMANAGER, _CENGINEPRIMITIVETYPEINFO(+_REGISTER). Flags: none.
- Local models (no shared header exists): CEnginePrimitiveManager2DStaticMeshes (0x318, ctor 0x00BA1AC0) : shared CEnginePrimitiveManagerStaticMeshesBase; CEnginePrimitiveManager2DAnimatedMeshes (0x690, ctor 0x00BD1F30) : local CEnginePrimitiveManagerAnimatedMeshesBase (0x690, ctor 0x00A84830) : shared CEnginePrimitiveManagerMeshBase.
- Globals: g_pEngineDef_01436e20 (descriptive, landed spelling), g_TwoPi 0x0128F608 (landed spelling of CEngineCamera_Process 0x00B314E0; other rows use `static const float kTwoPi` -- one address, two spellings: coordinator should unify).
- Name fixes (from DOSSIER, confirmed by match): landed _$E2 0x0122AD60 comment calls 0xBA1AC0 AnimatedMeshes (it is StaticMeshes); landed 0x00BBBF40 CEnginePrimitiveManagerParticleGroup ctor param is CArray<CEngineScene*>& per PDB, not CEngine*.

### Repair pass (verifier findings, all applied; still RELOCATION_MATCH, also under LANDVERIFY_STRICT_HEADERS=1)
1. Layer-72 constants respelled 0.40499997f / 0.80999994f -> 0x3ECF5C28 / 0x3F4F5C28 (retail pool 0x0139A3C4 / 0x0139A3BC); 0.405f/0.81f were 1 ulp high. (Both equal float(0.9f*0.9f) [/2], a likely original spelling, unproven.) Test updated.
2. 2D mesh managers now derive from their real direct bases (Ego_r ctors 0x00A55E60 / 0x00A84830, rtti_bases in labels for 0x00BA1AC0 / 0x00BD1F30); StaticMeshesBase via its shared header, AnimatedMeshesBase local (no header).
3. Source comment above AddPrimitiveManager names the landed row rebuild/src/compiled/00/b4/CEnginePrimitiveRenderer2D_AddPrimitiveManager_00b4abb0.cpp.


## Coordinator note at landing (2026-11-13): evidence for the name fixes
- 0x00BBBF40: ??0CEnginePrimitiveManagerParticleGroup@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z in Ego_r 0x00A71660 and FableWin 0x02EA4AD0; the landed row spells (CEngine*).
- 0x00B4B690: ??0CEnginePrimitiveManager2DParticleGroup@@QAE@AAV?$CArray@PAVCEngineScene@@@@@Z, FableWin 0x02D9D090 (+ thunk 0x018587F7); it has a PDB name (the overlay comment said it had none; corrected).
- _$E2 0x0122AD60: 0x00BA1AC0 is the 2DStaticMeshes ctor (vtable 0x012A5160 RTTI); AnimatedMeshes is 0x00BD1F30.
All three are in NAME_FIXES.tsv.

## 0x00973DC0

## 0x00973DC0 CAIStateGroup_JackOfBladesBase::SummonPillars -- RELOCATION_MATCH (8 grades)
- Matched by porting onto shared headers (RushToPos/Guildmaster idioms) + three facts: GetSubDef<T> body visible noinline
  (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE + CDefClassBase_GetSubDef.h: in-place release, no null test at loop exit),
  FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN (fstp st0 placement), and vslot +0x7c is ResetPosition (SetPosition is +0x78).
- Header overlay: engine/CAIStateGroup_JackOfBladesBase.h opt-in FABLE_RETAIL_JACKOFBLADESBASE_SUMMONPILLARS. Needs also
  FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD (GetWorld), FABLE_RETAIL_CWORLDMAP(_HEIGHTZ). No flags/pragmas.
- DOSSIER correction: vslot +0x7c on CTCPhysicsBase is ResetPosition(const C3DVector&) (slot 31), not SetPosition.
- CTCJackPillar has no shared header: minimal local class (StatGetInterfaceType + Enable decl), as landed 0x00976E20.
- Repair (verifier findings, both applied, still RELOCATION_MATCH incl. LANDVERIFY_STRICT_HEADERS=1): height offset is
  +0.1f (retail 0x00974061 fadd [0x0122EE58] = 0.1f; 3.0f at 0x0122DED4 is the Normalise constant), test updated to 0.1f;
  loop frame read respelled `GetWorld().GetFrame()` (same bytes as the old split `GetWorld(); CWorld::GetFrame();`).

## 0x00875B10


## 0x00875B10 NCameraModes::CTools::TestMeshForOccupationAndGetType -- RELOCATION_MATCH (wf21 author, ~20 grades)
- Made it match: (1) identity: retail mangling `W4EMeshType@2@` = NAMESPACE-scope NCameraModes::EMeshType (the dossier's
  "nested in CTools" was wrong; nested mangles `@12@`), const member, const& mesh / float& height, CMeshIntersectionData;
  (2) vertical probe `position + C3DVector(0,0,(float)(long)((0.5f-i)*2.0f)*size*3.0f)` (int copies after __ftol2; the inline form
  also fixes the extent load order -- the extent block is sensitive to this, not to its own operand order);
  (3) end `switch(hits){case 5: return 1; case 6: return 1; default: return 0;}` (merged cases give a range check; no default puts
  ret0 before ret1); (4) header opt-in C3DVector operator*(float) building a named result local (loop-2 Y product scheduling).
- Headers (overlay $W/hdr): FABLE_RETAIL_NCAMERAMODES_CTOOLS_MESHTYPE (NCameraModes_CTools.h), FABLE_RETAIL_C3DVECTOR_SCALE_NAMED_RESULT
  (C3DVector.h). Defines: FABLE_RETAIL_C3DVECTOR, _SQUAREDMAGNITUDE_XYZ, _NORMALISE_PAREN_SQUARES_ZYX, _CROSSWITHUP,
  FABLE_RETAIL_CPHYSICSMESHOBJECT_EDGES. Flags: none. Made-up names: enumerators MESH_TYPE_NONE / MESH_TYPE_OCCUPIED (descriptive).
- Repair (verifier pass): hdr/README.md now states the C3DVector.h SCALE_X_PAREN `#ifdef` became an `#elif` so NAMED_RESULT takes precedence; source unchanged, regrade RELOCATION_MATCH (also strict headers), test PASS.

## 0x00E6B900


## 0x00E6B900 NScript::CV_ChapelOfEvilScript::TeleportAcolytes -- RELOCATION_MATCH (7 grades)
- Ported onto shared headers (fable_string.h, CCountedPointer.h, CScriptThing.h, stlport/fable_containers.h via CQ_SunnyvaleMasterData.h -- do NOT add a local std::vector model, it collides; CV_ChapelOfEvilScript.h). Needed `inline CScriptThing::CScriptThing() {}` (inline ctor: CBaseClass call + vptr + PImp zero) and `FABLE_FORCEINLINE ~CScriptThing() {}`; `#pragma inline_depth(0)` regions around each terminate-return AND the marker reassignment (removing it from the reassignment grows the function).
- Last residue (eax/edx swap in the loop around slot 72/472 calls) was the operator== spelling: `PImp->IsEqualTo(*other.PImp)` (CCountedPointer operator->/operator*) matches; `PImp.Data->IsEqualTo(*other.PImp.Data)` does not.
- Flags: none. Overlay headers: none. Draft's `IsSameAs` was a made-up name for slot 78 = protected CScriptThing::IsEqualTo (FableWin.pdb).
- Repair (verifier): source now names the landed rows its inline callee bodies copy (ctor 0x006E7B40, dtor 0x004AA840, operator== 0x008AAE20) and the retail evidence of inlining: CBaseClass() call 0x0099A2D0 at 0xE6B94F + vptr 0x01238C8C stored at 0xE6B95C (the task text said 0xE6B94F for the store; xdis shows the store at 0xE6B95C); inline `call [edx+0x138]` at 0xE6B9BE. Still RELOCATION_MATCH (also strict headers); test PASS.

## 0x005EC270

## 0x005EC270 CTCTavernGame::ProcessInput -- RELOCATION_MATCH (author wf21, 2026-11-12)
- Made it match: port onto shared headers (CTCTavernGame/CTavernGameDef/CDefPointer/CInputProcess*/CPlayerGui/CSubtitleRenderer) with FrameUpdate's in-TU GetSubDef<CTavernGameDef> + SetState real bodies; case source order 5,46,45,44,47 (retail layout); case 5 "stay" arg read via inline FableWin.pdb `GetGameState()` (that one inline call was the whole eax/ecx/edx rotation residue); answer tails clear Alt, No, Yes in that source order (gives retail a8/ac/b0 stores).
- Headers (overlay $W/hdr): CInputProcessTavernGame.h FABLE_RETAIL_INPUTPROCESS_TAVERNGAME_FRIEND (friend for protected IsEventButtonPressed); CTCTavernGame.h FABLE_RETAIL_CTCTAVERNGAME_GETGAMESTATE (inline GetGameState). Flags: none.
- Landed names proven wrong (DOSSIER evidence): 0x00687D00 landed as GetEventGameActionVector = ?GetControlMovementFromGameActionEvent@CInputProcessGameBase@@QBE_NW4EGameAction@@ABVCInputEvent@@AAVC2DVector@@@Z; 0x00687CF0 landed as garbage T::Run = ?IsEventGameAction@CInputProcessGameBase@@QBE_NW4EGameAction@@ABVCInputEvent@@@Z.
- Tried without effect: dropping SetState / GetSubDef bodies (GetSubDef body required), /GS /G5 /G7 /Ob1 /Ob2 /Ox, stick scope, early-break / default / local switch var, GameWon-first.
- Repair (2026-11-13): SetState body dropped (declared only; still RELOCATION_MATCH, also strict headers); Shape comment now names the landed GetSubDef row CDefClassBase_GetSubDefCTavernGameDef_005ed4a0.cpp (body copied verbatim) and SetState row, and states source order Alt,No,Yes -> retail stores +0xA8 Yes, +0xAC No, +0xB0 Alt at 0x005EC4F7..0x005EC503. Test re-passed vt.py; addmatch rerun.

## 0x004093E0 (not landed)

## 0x004093E0 CUserProfileManager::SetDefaultValuesVideo -- DIFFER(841v841), 9 hunks, block placement only (wf21 author, ~100 grades)
Best source: differ/004093e0.cpp (= v68). Same instructions and registers as retail; the only residue is that retail lays the def
arm (else of the null test, 0x004095D4..0x00409676, ends `jmp 0x004094AD`) after the inlined ClampResolution loop's back-jmp, while
VC7.1 here keeps it straight after the null arm; the shifted offsets then flip three loop-alignment pads. The SAME layout appears in
SetDefaultValues 0x0040C790 and SetDefaultValuesForNewProfile 0x00409B70 (both carry a copy of this code), so it is a property of the
source, not a fluke.
What got it from 921 to 841 / same code:
- port onto fable_user_profile.h FABLE_RETAIL_CUSERPROFILEMANAGER (+ VIDEODEFAULTS overlay), engine/CDefPointer.h, CSystemManager
  GetDisplayManager(), FableWin inline CDisplayManager::GetBitDepth()/GetRefreshRate() (overlay), real PDB reference signatures;
- GetConfigOptionsDefaultsDef body in the TU (sibling lever; without it the def gets its own frame word / registers move);
- refresh pick loop as do { } while (++i < 4): a for loop is fully unrolled by VC7.1 (921 bytes) -- that alone fixed the
  register allocation (edi = def, ebx/ebp = width/height, depth in the frame slot);
- mode search through `const std::vector<CDisplayModeDesc>& modes = GetDisplayModes();` with GetDisplayModes' body in the TU
  (gives retail's `xor edx,edx ; mov [esi+48h],edx ; mov eax,[esi+80h] ; cmp eax,[esi+84h]` head);
- ClampResolution / ClampRefreshRate real bodies in the TU (auto-inlined like retail); unrotated loops only with early-return
  FABLE_FORCEINLINE predicates (IsModeWiderThan / IsRefreshRateAbove: MADE-UP names, as in the landed ClampResolution row;
  FableWin.pdb lists no such helper, so the real spelling is open). A while-loop mode search stops ClampResolution being inlined.
Tried without effect on the placement: if(!pDef)/if(def.IsNull())/def.Get()/==0 tests, arm order swapped (worse: def falls
through), else-if complementary tests, two separate ifs, per-call if/else and ternaries (threaded or 825 bytes), Antialiasing
outside/after the if-else (guarded or ternary), ClampAntialiasing duplicated into both arms (tail-merged, no move), locals defaulted
to -1 with one def if (795), hoisted max-effects local, const def / Gamma order / assignment init, ClampVideoOption real body
visible (auto_inline(off) or noinline), GetConfigOptionsDefaultsDef body absent, CDisplayModeDesc desiredMode local, /G5 /G7 /Oa
/GS /Ow /Ob1 /d2inlT20..200, QFE-4035 compiler (identical), toy probes (VC never moved an else arm in any toy).
Best next idea: find what makes c2 treat the def arm as the cold successor (it is laid out like an unlikely block): candidates are a
retail-only statement in the def arm that folds away (an assert-like construct / an inline accessor with its own test), or the
mode/clamp section being spelled with retail's real helpers (look for FableWin debug-build line info or the debug copy 0x01881E20
to read the original statement structure).
Globals: g_MemoryA/B are `unsigned long` in GFConfigDetection's landed source but retail tests them with `jle` (signed) here;
declared `long` in this row (type mismatch to resolve at landing; names reused). [Superseded by the repair section below: NAME_FIXES.tsv now handed back.]

## 0x004093E0 attempt 2 (wf21 author #2) -- still DIFFER(841v841), 9 hunks, same block-placement residue (~30 grades + toy probes)
Best source: differ/004093e0.cpp (= t1.cpp). New, kept: the clamp loops no longer need the made-up FABLE_FORCEINLINE predicates
(IsModeWiderThan / IsRefreshRateAbove): `unsigned long index = x; while (index >= 0 && <elem> > max) --index; x = index;` (folded
unsigned guard) gives retail's unrotated `cmp; jle exit; dec; sub; jmp top` loop in the row AND MATCHES the landed standalone
ClampResolution 0x00406D90 (q.py MATCH with that one-line change, $W/cr/cr1.cpp) -> suggest re-spelling the landed row at landing
and dropping its made-up helper. `for (i = x; i >= 0; --i) if (<= max) { x = i; break; }` gives the same bytes.
Tried without moving the def arm (do not repeat): per-call if/else, !-test per call, per-call arg ternaries (825); locals set by
if/else then one call set (795/809, no tail duplication); comma-expression ?: , do{..break..}while(false), goto forms; bool-local
test, raw pointer ==NULL, IsNull()!=false; inline-helper IsNull bodies with early return; __assume (release-ASSERT) at arm start,
end of null arm, in the clamp loop; whole if/else, null arm only, def arm only in __forceinline helpers; ClampAntialiasing in both
arms; ClampResolution / ClampRefreshRate / GetDisplayModes / GetConfigOptionsDefaultsDef bodies defined AFTER the row; folded
`< 0 ||` range tests in the Clamp bodies; for(;;)/break loop forms (rotated: wrong); flags /EHsc (EH frame), /Ox, /Oi- (same),
/Op (921). Probes ($W/toy2): VC7.1 lays out if/else arms in IL order (then-arm falls through from the test, the LAST arm falls into
the join) for every spelling tried, including folded `|| u<0` terms (those only swap which arm comes first); nothing ever put an
arm after a later loop. Removing ClampResolution / the whole second half / ApplyValues from the row does not move the def arm either.
Model of retail's layout: chain layout where null->join is the chosen fall-through edge and the def arm becomes its own chain,
emitted at the first hole (after the CR loop's back-jmp; same in the 0x00409B70 / 0x0040C790 copies). Our compile always picks
def->join. So in retail's c2 the null arm out-weighs the def arm, or its IL had the def arm later than the join.
Best next idea: work out which source fact makes c2 weight the null arm higher. Candidates not yet tried: a real Lionhead
CDefPointer operator-> / IsNull with a debug-check macro that leaves a folded branch in release (look in other landed rows with
CDefPointer null tests whose retail puts the non-null arm out of line, and copy what made them match); LTCG (/GL) codegen for this
TU (would need a link-time harness); an EH-on TU where every callee is known nothrow.

## 0x004093E0 attempt 3 (wf21 author #3) -- RELOCATION_MATCH (strict headers too), test vt.py PASS, addmatch.py done
What made it match: the block-placement residue was a missing STATEMENT (wf8 lesson): the null arm does NOT set Antialiasing;
ONE guarded statement after the detail if-else, `if (def.IsNull()) Antialiasing = 0;`, is threaded by c2 into the null arm. The
join then has the null arm as fall-through predecessor and the def arm stays behind the inlined ClampResolution loop, jumping back
(retail 0x004095D4..0x00409672). The def arm keeps `Antialiasing = def->Antialiasing;`. (Tried and wrong: AA via phi local, AA
ternary after the if-else, `if (!def.IsNull()) AA = def->AA` after, both AA assignments after as if/else; goto/late-IL def arm,
inline helper with early return, /Os /O1 /EHsc.)
Also: the local CNewFrontendGameComponent model was replaced by engine/CNewFrontendGameComponent.h; that needs
engine/CMainGameComponent.h instead of fable_game.h (fable_game.h's FABLE_RETAIL_CGAMECOMPONENT class collides with
engine/CGameComponent.h). Headers: overlay hdr/ (fable_user_profile.h VIDEODEFAULTS, engine/CDisplayManager.h VIDEODEFAULTS). Flags: none.
Clamp loops use `while (index >= 0 && ...)` (no made-up helpers); landed ClampResolution 0x00406D90 also matches that way ($W/cr/cr1.cpp).
Globals g_MemoryA/B declared `long` here (retail `jle`); GFConfigDetection's landed source has `unsigned long` -- resolve at landing.

## 0x004093E0 repair (verifier findings 1-7) -- still RELOCATION_MATCH (strict headers too), test vt.py PASS, addmatch.py rerun
- Source: dropped the unjustified CDefinitionManager::GetDef template body (0x00410890 stays out of line through the header's
  template declaration; landed row CDefinitionManager_GetDef00410890_00410890.cpp, named in the Callees comment); abs pragma comment
  now cites retail's inline `cdq ; xor ; sub` at 0x0040967A..0x004096C8; GetConfigOptionsDefaultsDef body now has the landed row's
  explicit `CCharString name("CONFIG_OPTIONS_DEFAULTS_DEF",-1)` temp, and the comment states the retail fact (Ego_r.pdb: same source
  file, 0x00406C90 ahead of SetDefaultValuesVideo 0x004073D0). It keeps the PDB accessors CMainGameComponent::Get() /
  CNewFrontendGameComponent::Get() instead of the landed row's made-up g_pMainGameComponent / g_pFrontendRuntime: those bind the
  real statics PMainGameComponent 0x013B86A0 / m_pNewFrontendGameComponent 0x013B871C under second names (NAME_FIXES).
- Globals: one name per address = GFConfigDetection 0x004017B0 spellings (descriptive, no PDB symbol). TYPE of 0x01375084 /
  0x01375088 / 0x0137509C / 0x013750A0 is `long` (3JA): signed compares in this row (jle on MemoryA/B), GetMaxResolution 0x00408900
  and GetMaxRefreshRate 0x00406890; GFConfigDetection only stores them, so its `unsigned long` (3KA) is the spelling to fix.
  Spelling conflicts: 0x01375080 g_MaxAntialiasing vs g_ConfigLevel (0x00409090), 0x01375084 g_MaxResolutionWidth vs
  g_FableConfigWidth (0x00408900), 0x01375088 g_MaxRefreshRate vs g_maxDisplayRefreshRate (0x00406890). All in NAME_FIXES.tsv.
- NAME_FIXES.tsv (10 rows): ClampVideoOption 0x004068D0 (?..@AAEXAAMMMMM@Z, float&), ClampAntialiasing 0x00409140 and
  ClampResolution 0x00406D90 (?..@AAEXAAKJ@Z, unsigned long&): the landed rows use pointers / public and bare or missing Leaf lines,
  and this row's header declares the PDB reference forms, so they must be respelled when this lands; plus the two component
  statics and the five globals above.
- Test: added a 1024x768 16-bpp mode beside the 32-bpp one (def BitDepth 16 -> mode 1, no def -> mode 2) and display rate 74 Hz
  (75 is nearer but above -> 70). Mutants: removing the `<= refreshRate` guard FAIL, removing `depth = def->BitDepth` FAIL
  ($W/rp_mut_guard.cpp, $W/rp_mut_depth.cpp).
- Verifier findings not applied: none (finding 5 applied for the statements; the landed row's global names are respelled the
  other way, via NAME_FIXES, because the shared headers carry the PDB statics).
- Note: $W/r1.cpp (an old author variant) was overwritten by the first repair variant.

