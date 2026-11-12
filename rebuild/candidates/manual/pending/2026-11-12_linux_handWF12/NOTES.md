## 0x009FF220

## 0x009FF220 CGraphicInfoFrame::CFrameData::UploadFrame -- RELOCATION_MATCH (wf12, ~20 grades)
- What made it match: (1) port onto the shared headers with the FableWin inline helpers IsVolumeTexture / UploadToVolumeTexture /
  UploadToTexture defined in the row (GetBank()->Load...); (2) CGraphicFrame operator= and destructor as FABLE_FORCEINLINE header
  bodies with `data->~CGraphicFrameData(); ::operator delete(data)` -- VC7.1 refuses to auto-inline any function whose body expands
  ~CGraphicFrameData (even an empty one), and inside an inlined callee `delete data` stays a `push 1 ; call ??_G` (only at the top
  level is ??_G expanded); op= is `if(--rc==0)`, dtor is `--rc; if(rc==0)`; (3) the LoadVolumeGraphic header argument is
  `GraphicInfo->Header`: a PeekGraphicHeader() accessor lets c2 keep GraphicInfo across the IsBump call (the old "register tie").
- Headers: overlay hdr/ (macro FABLE_RETAIL_CFRAMEDATA_UPLOADFRAME, see hdr/README.md) + FREE, STATICBLOCKS, CCOUNTEDPOINTER,
  DEF_CLASS_BASE, CRESOURCELIST, CCHARSTRING, CWIDESTRING, CARRAY, CBANKFILE_ENTRYQUERY, CGRAPHICINFO_TEXTURETYPE. Flags: none.
- Names: NAME_FIXES.tsv (0x00A69B20 = CManagedTexture::operator=, 0x00A69AD0 = CManagedTexture::Release, UploadFrame takes bool).
- Repair (verifier findings, all applied; still RELOCATION_MATCH, also LANDVERIFY_STRICT_HEADERS=1):
  (1) hdr/engine/CGraphicFrame.h: `#include "fable_compiler.h"` now inside `#ifdef FABLE_RETAIL_CFRAMEDATA_UPLOADFRAME` (rebuild_abi.h
  does not pull it in); default path byte-identical to repo header. (2) test.cpp counts LoadVolumeGraphic vs LoadGraphicFrame calls
  and checks `force` is seen only by the frame path; the type==4 -> type==2 mutant (verify_evidence/mut2b.cpp) now FAILs.
  (3) final.cpp carries the provenance line for GetBank / IsFlatSequence / CGraphicFrame::Sizeof (FableWin01d2df40/03127100/02d96750,
  copied from landed 0x009FE460) and the FableWin inline symbols of IsVolumeTexture / UploadToVolumeTexture / UploadToTexture.

## 0x0082B7E0

## 0x0082B7E0 CMiniMapDisplay::DrawMarkers -- RELOCATION_MATCH (wf12, 7 grades)
- What made it match: full port onto shared headers (STLport vector/list, CFadeInFadeOutBase Open/IsOpen/Close, GetWorld().IsPerformingRegionLoad,
  PeekDisplayEngine().IsScreenFadeOut, IsPaused, GetPlayerManager().GetMainPlayer().GetPControlledCreature(), IsDead, AddAutoMarker), each loop binding
  `const CMarkerInfo& marker = *it`, and inline IsHeroMarker spelled with a `const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();` local (the direct
  `PeekGuiDef().MiniMapMarkerGraphics[0]` form gives `add eax,958h ; mov eax,[eax]` in the loops -- it is retail's tail spelling, dead add included).
- Headers: overlay opt-ins FABLE_RETAIL_CMINIMAPDISPLAY_DRAWMARKERS, FABLE_RETAIL_CFADEINFADEOUTBASE_OPEN, FABLE_RETAIL_CPLAYERGUIDEF_MINIMAPMARKERGRAPHICS
  (hdr/README.md); STLport FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE, _RESIZE_ONE_DECL, _STLP_LIST, _LIST_SIZE_DISTANCE. No flags/pragmas.
- Name fixes vs old draft: AddMarker->AddAutoMarker (0x0082A500), GetPlayer->GetMainPlayer, GetHeroThing->GetPControlledCreature, reserve->resize(n),
  Flash->AutoMarker, IsFullyOpen/FadeOut->IsOpen/Close (all per DOSSIER). Landed DrawMap 0x0082BE60 declares DrawMarkers' info non-const (PDB: const).

## 0x0084D910


## 0x0084D910 NCameraModes::CBase::UpdateCameraStretchAroundHero -- DIFFER(542v542), 4 hunks (author wf12, 2026-11-12)
Best source: differ/0084d910.cpp (ported: engine/C3DVector.h, GFGetInterpolatedValue.h, CThingCreatureBase.h, CPlayer.h,
NCameraModes_CBase.h (CYLINDER), CGameCameraManager.h (LOOKAROUND); overlay opt-ins in hdr/README.md; CBase::GetObservedThing
defined `inline` in the row as its real body `return CameraManager.GetObservedThing();`).
Residue: last block only -- retail copies the lookAt temp as `mov edx,[34]` early but `mov eax,[38] ; mov ecx,[3c] ; mov [edi+4],eax ;
mov [edi+8],ecx` after `fstp [3c]`; ours loads Y right after `fstp [38]`. Looks like a VC7.1 81-tuple region cut (LESSONS wf1/Q19).
What got it from 525/530 to 542v542/4 hunks (keep these):
- const member; delta = C3DVector(position.X-heroPos.X, position.Y-heroPos.Y, 0.0f) + separate float dz (frame dx/dy at 28/2c, dz at 14);
- new position as ONE expression `delta * lerp(1,curR/nomR,blend) + heroPos + C3DVector(0,0,lerp(1,GetObservedThingCurrentMeshHeight()/nomH,blend)*dz)`
  (named height/scale locals or a named sum `p` let VC7.1 hoist the sum above the virtual call: 530 bytes); lerp = GFGetInterpolatedValue
  or the explicit `(r-1)*blend+1` (same bytes);
- sqrt arg `delta.X*delta.X + delta.Y*delta.Y` (gives retail's dy-first load);
- `(float)exp((double)(d*d*-0.01f))`: fixes the d pop order after the blend store; the natural `(float)exp(d*d*-0.01f)` leaves 8 hunks
  (d pop + nomH store order + this residue). NB parentheses round the exp RHS also undo the fix -> the d-pop is itself a region-cut effect,
  so the real source may be the natural exp with some other tuple difference upstream in the same block.
Ideas tried with no effect on the residue (all still 4 hunks): lookAt as offset+position / position+offset / component ctor / named temp /
const-ref temp / C3DVector(offset+position) / chained `lookAt = offset + (position = ...)` / reference aliases; offset as const, const-ref,
ctor form; heroPos const / ctor; delta as position-heroPos + `delta.Z = 0`; header opt-ins C3DVECTOR_SUBTRACT_XY_GROUP, SCALE_X_PAREN,
CTCTARGETINGAI_ATTACK copy ctor, plain-inline vector ops (probe); guards as one if / nested if; hero C-cast / reference; block scopes;
exp arg orders; GFSqr; sqrtf / GetMagnitude2D; parens round ctor args (0.0f / height), round GFGetInterpolatedValue args;
flags /Oa /Ow /G5 /G6 /G7 (no help). Worse: ADD_XY_GROUP (22 hunks), lookAt `+=` forms, double d, nomH inline (fdivr).
Lookat ctor with paren on X (`C3DVector((offset.X+position.X), ...)`) moves the cut (6 hunks: fld [44] early) -> the region cut IS reachable.
Best next idea: sweep no-code tuple counts in the final block systematically (the block starts at 0x0084DA22 after the clamp join):
combine the natural exp with lookAt-ctor paren sets and paren sets on arithmetic inside the position expression until both the d-pop
and the lookAt Y copy land; or find the landed sibling FrameUpdate 0x0084DC10's idioms for its vector tail.
Names proved (NAME_FIXES): 0x006625C0 = CThingCreatureBase::GetNominalMeshRadius, 0x006625E0 = CThingCreatureBase::GetNominalMeshHeight
(landed as CTCWeapon::GetAnimationSpeedValue rows; Ego_r PDB names 0x0053D450/0x0053D480 + FableWin api, def +0x60/+0x5C = MeshRadius/MeshHeight).

## 0x0084D910 -- RELOCATION_MATCH (attempt 2, 2026-11-12)
What made it match: the residue WAS the 81-tuple region cut. A scripted 256-way sweep (a2/gen.py) over 8 no-code paren/cast sites in the
final block found 14 matching combos; chosen m61: `(float)(exp((double)((d * d) * -0.01f)))`, `GFGetInterpolatedValue(1.0f, (curR / nomR), blend)`,
`C3DVector(0.0f, 0.0f, (GFGetInterpolatedValue(1.0f, (GetObservedThingCurrentMeshHeight() / nomH), blend) * dz))`, lookAt = offset + position.
No natural-exp (no (double) cast) combo matched. Headers: overlay hdr/ (CGAMECAMERAMANAGER_GETOBSERVEDTHING, CTHINGCREATUREBASE_MESHSIZE); no flags.
Source: 0084d910.cpp, test 0084d910_test.cpp (vt PASS, strict regrade RELOCATION_MATCH). NAME_FIXES as attempt 1 (0x006625C0/E0).

## 0x0084D910 -- repair (2026-11-12)
Rebased hdr/engine/CGameCameraManager.h on the current repo header (only the GETOBSERVEDTHING block added; DEBUGPOINTS kept); source comment now names the (double) cast as a steering construct and cites the landed 0x0084F1B0 body + the four inlined `lea ecx,[eax+100h]; call 0x00A01B50` sites. Regrade RELOCATION_MATCH (also strict), vt PASS.

## 0x006507D5


## 0x006507D5 NPlayerGui::CDrawQuestInfo::CQuestInfoElementTimer::DrawAt -- RELOCATION_MATCH (grade 7 batch, ~20 grades)
- What made it match: port onto the Counter sibling (0x006509F8) verbatim on shared headers (fixes the font-in-edi register
  residue by itself), GetTimer via CMainGameComponent::Get()->PeekWorld().GetGameScriptInterface().GetTimer(TimerIndex), and the
  colour as an if/else around TWO Draw2DText calls (`if(seconds<11) Draw2DText(...,CRGBColour(255,100,100),...); else
  Draw2DText(...,CRGBColour(255,255,255),...);`) -- VC7.1 hoists the common pushes and A,R stores and merges the calls.
  Tried first (no match): ternary of colours as arg, per-channel ternary (setcc), local + Initialise if/else (several decl points),
  local assign if/else, non-forceinline helper (not inlined under /Os).
- Headers/flags: no overlay; `#pragma optimize("s",on)`; Counter's opt-ins with _CDRAWQUESTINFO_ELEMENTS in place of _COUNTER, plus
  FABLE_RETAIL_CMAINGAMECOMPONENT, _CMAINGAMECOMPONENT_WORLD, _CWORLD, _CWORLD_GAMESCRIPTINTERFACE, _NSCRIPT; includes CWorld.h, CGameScriptInterface.h.
- Names: no landed names proven wrong. The old draft's names (Format, AddPrimitive, GetTimerSeconds, GetTimerColour...) are all gone.
- Repair (verifier findings): added the `// Fidelity:` line for the branch-local double results (store [ebp+8] 0x006509E3, across
  ~CWideString call 0x006509E9, reload 0x006509EE -- verifier cited 0x006509EC, the fld is actually at 0x006509EE) and the GFToScreen
  evidence comment (inline box copy [ebp-0x3c]->[ebp-0x4c] at 0x00650875..0x00650897 before the sprite ctor call 0x006508E7; body
  copied from landed 0x006509F8). Comments only; still RELOCATION_MATCH (also strict headers), test PASS.

## 0x007AF490

## 0x007AF490 CTCBoss::KillHelpers -- RELOCATION_MATCH (8 grades)
- Matched after porting onto shared headers (CTCBoss, CThingAICreature retail chain + CTHING_ORIGINAL_VTABLE for ModifyHealth slot +0xEC,
  CSpecialEffectsDef GETEFFECT_BODY, CDefClassBase_GetSubDef template body, PeekBaseDef()), implicit `"..."` -> CCharString conversion
  (explicit CCharString(..) temp pushed &effect before constructing the string), and the knockdown spelled fully inline:
  `HelperCreatures[i]->SetCurrentAction(Knockdown(*HelperCreatures[i], 0, HelperCreatures[i]->GetPos(), C3DVector(0,0,1)))`
  (named creature/up locals left a CSE/register residue).
- Needs: FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE, overlay opt-ins FABLE_RETAIL_CTCBOSS_KILLHELPERS and
  FABLE_RETAIL_CCOMBATACTIONGENERICSTRIKERESPONSEKNOCKDOWN_IMPLICIT_DTOR (hdr/README.md). No flags/pragmas.
- Old draft spellings proven wrong (dossier): CCRC::CalcAndLookup 0x669D30 = CSpecialEffectsDef::GetEffectFromString; ChangeHealth = ModifyHealth; GetPosition = GetPos.
- Repair (verifier findings, both applied): row now cites why GETEFFECT_BODY (copy of landed 0x00669D30) is needed -- effect local
  [esp+0x24] (0x7af530) reused for the SetCurrentAction spill (0x7af5f6), def at [esp+0x18] for ebx loads 0x7af529/0x7af550; without it
  frame 0x170 and 23 hunks. Local IS_THING_ALIVE commented (engine/ThingAlive.h `thing!=0` fails C2676 on CIntelligentPointer). Still
  RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1); test PASS.

## 0x007F9960

## 0x007F9960 CTCTargetingSpirit::UpdateTargetList -- RELOCATION_MATCH (grade 3 of 3 calls, 7 variants)
- Ported onto the shared headers with the defines of sibling 0x00804020 (CTCTargetingPlayer::UpdateTargetList) + GFFilter_Bind3,
  std::sort __fastcall decl, CThingCompare_BestForTargeting2 8-arg ctor. Two last fixes: result pick as `CThing* pbest=0; if(!targets.empty()) pbest=targets[0];`
  (a ?: adds a jmp), and GetParentThing() spelled three times in the comparator args (a `CThing& parent` local swapped edi/ebp).
- Headers: overlay engine/CThingFilter_IsNotList.h opt-in FABLE_RETAIL_THINGFILTER_ISNOTLIST_INLINE_CTOR (inline PNotList(&list),PNotList2(0)).
  CThingFilter_In3DRange modelled locally in the row (no shared header exists; Ego_r layout Pos +0, Range +0xC). NEW overlay header
  engine/CTCTargetingSpirit.h (repair): full layout MaxTargetingDistance +0x38, Targets vector<CIntelligentPointer<CThing> > +0x3C,
  TargetedPos +0x48, sizeof 0x54 (retail ctor 0x004DEBD4 zeroes +0x3C..+0x44, Alloc 0x004E1006 news 0x54; PDB 0x58 = 16-byte debug
  vector). Row no longer declares the class locally. No flags. Still RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1).
- Name fixes vs old draft: GetPosition->GetPos, GetPCharacterThing->GetPControlledCreature, FindThings->GetAllThingsInRoughArea
  (landed 0x007F9C00 name `CThingSearchTools_Calls::GetAllThingsInRoughArea` is wrong: the full symbol is
  ??$GetAllThingsInRoughArea@VCThing@@V?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@V?$CFilter_Bind3@VCThingFilter_IsNotList@@VCThingFilter_IsTargetable@@VCThingFilter_In3DRange@@@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABV?$CFilter_Bind3@VCThingFilter_IsNotList@@VCThingFilter_IsTargetable@@VCThingFilter_In3DRange@@@@AAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z
  -- Ego_r.pdb 0x007F4D60 and FableWin.pdb 0x0184EF5E (two sources); handed back in NAME_FIXES.tsv).

## 0x0096CB40

## 0x0096CB40 CAIStateGroup_EatAtTable::GetSeat -- RELOCATION_MATCH (9 grades)
- Matched by porting onto shared headers (CThing/CTCThingOwner/CTCVillage/CTCTavernTable/CIntelligentPointer) and spelling the
  building test as `if (!IS_THING_ALIVE(building)) return false;` -- the hand-spelled `!b || b->IsDead()` moved the pre-push
  return-false block to the function end (560v553). HasTC must be the direct-test GetTC (no FABLE_RETAIL_CTCWEAPONTRAIL_UPDATE).
- Opt-ins: CTHING_GETTC_EXPANDED, CTHING_STATEACCESS (IsInLimbo), CINTELLIGENTPOINTER(_COMPARE_NULL_INT, _CTOR_OUTOFLINE,
  _ASSIGN_OUTOFLINE), CTCTHINGOWNER, IS_THING_ALIVE, STLP_LIST, CTCVILLAGE, CTCTAVERNTABLE_SEATS/_LAYOUT + NEW overlay opt-in
  FABLE_RETAIL_CTCTAVERNTABLE_GETSEAT (hdr/engine/CTCTavernTable.h: CTCTavernTable::GetSeat 0x007B8840, last param const CCharString*).
  No flags/pragmas.
- Names: GetBuilding (virtual const, slot 14, CThingBuilding*) per Ego_r EatAtHome/EatAtTavern overrides; no landed name proven wrong.
- Repair (verifier): `class CCharString;` in hdr/engine/CTCTavernTable.h moved inside its own `#ifdef FABLE_RETAIL_CTCTAVERNTABLE_GETSEAT`
  guard; hdrcheck2 now default=same[315]; row still RELOCATION_MATCH (also strict headers), test PASS.

## 0x007BDE31

## 0x007BDE31 NThingShot::CTCShotDefinitionFireballSpell::OnCollision -- RELOCATION_MATCH (9 grades)
- What made it match: port onto shared headers (CThing GETTC_EXPANDED, CTCShotDefinitionBase MOVEMENT+ONHITBYMELEE, CDefPointer,
  CTCSoundPlayer, CTCEntityEvents, CHitParameters, NEntityEvents_CEventHitBy, ThingAlive NOINLINE), criteria names as implicit
  temporaries per switch arm, and -- the decisive step -- the same-TU GetSubDef<CFireballSpellLevelDef> 0x007BE88C body visible out of
  line (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE + FABLE_RETAIL_CDEFINITIONMANAGER, engine/CDefClassBase_GetSubDef.h; GOTCHAS Q19):
  before it the level spilled, ebx was a hoisted zero and the def released with `dec [eax]; cmp [eax],0` (566v555).
- Headers/flags: overlay engine/CFireballSpellLevelDef.h opt-in FABLE_RETAIL_CFIREBALLSPELLLEVELDEF (polymorphic def : CSubDefClassBase,
  StatGetTypeName inline, SpellLevel +0x28). No flags; pragmas optimize("y",off)+("s",on). CTCBurntEffect has no shared header: local
  class with StatGetInterfaceType (FableWin) + Reset (Ego_r). Draft names fixed per DOSSIER (SetThingHitting, SpellLevel, EHeroAbility...).
- Repair (verifier findings): `(EHeroAbility)0x10` -> HERO_ABILITY_FIREBALL_SPELL (engine/EHeroAbility.h, same `push 10h`); header comment
  "game-object thing" corrected -- `test byte ptr [esi+6Ch],8` (0x007BDE55) is type-flag bit 8, in CThingCreatureBase (0xF) /
  CThingAICreature (0x1F) StatGetTypeFlags, not CThingGameObject (7) / CThingBuilding (0x47); evidence comment on the `& 8`. Still
  RELOCATION_MATCH (also strict headers); test uses the enum name and checks a 0x47-flag thing gets no burnt effect.

## 0x007D2CC0


## 0x007D2CC0 CTCTavernGameCoinGolf::EnableCourse -- RELOCATION_MATCH (author wf12, ~22 grades)
- What made it match: (1) ported onto shared headers (CThing ORIGINAL_VTABLE/STATEACCESS/GETSCRIPTNAME, GetSubDef<T> NOINLINE, CIntelligentPointer, C2DVector, vector<Coin>);
  (2) CBasicString<char>::operator==(const char*) body VISIBLE but noinline (new opt-in FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE in hdr/fable_string.h) plus the landed real body of
  CBasicString<char>::Compare 0x00411570 in the row file -> one null test serves both literal tests (588 -> 558 bytes); (3) CoinStartPos read through a const reference
  (XAngle store scheduled before the fld's); (4) `if (name == "" || name == "NULL") continue;` -- this alone swapped the name/def frame slots to retail's order.
- Headers: hdr/fable_string.h (FABLE_RETAIL_CBASICSTRING_TEXT_EQ_NOINLINE), hdr/engine/CTavernGameCoinGolfDef.h (FABLE_RETAIL_CTAVERNGAMECOINGOLFDEF_SUBDEF: class : CSubDefClassBase, Courses +0x50). No flags.
- Local models (no shared header): CTCTavernGameCoinGolf (ParentThing +4, Coins +0xFC, ZAngle/XAngle/CoinStartPos) and packed Coin, as the landed siblings do.
- Name fixes vs old draft: SetVisible->CThing::SetAsDrawable (slot 28), DetachTarget->SetAsRenderedThisFrame(unsigned long), NumCourses->Courses, Power->XAngle, AimAngle->ZAngle, AimX/AimY->CoinStartPos.
- Lesson: a frame-slot swap between a loop-local CCharString and an outer CDefPointer was a control-flow fact (`continue` skip vs a nested && condition), not callee visibility.

## 0x004085F0 (not landed)

## 0x004085F0 CUserProfileManager::ResetAssignedInputs -- RELOCATION_MATCH (7 grade calls)
- What made it match: (1) port onto shared headers + GetDef<T>(const CCharString&, CDefPointer<const T>*) const template BODY visible in the TU
  (shared header opt-in FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY; = landed row 00/41/CDefinitionManager_GetDef00410820_00410820.cpp)
  (out of line in retail at 0x00410820; visible body => no reload of &def after vector operator=: 622v557 -> 511v557);
  (2) a SEPARATE CDefPointer<const CControlsDef> local per scheme branch (VC7.1 hoists the three identical null-inits above the
  first branch; per-branch releases before ApplyValues give retail's uncross-jumped two-mode tail and ebp register choice).
  A single function-scope def (or one inner block) leaves ebx-for-ebp + a cross-jumped two-mode tail (513v557).
- Headers: overlay engine/CUserProfileManager.h opt-in FABLE_RETAIL_CUSERPROFILEMANAGER_RESETASSIGNEDINPUTS (nested in PASSIVEINPUTS:
  AssignedInputsAggressive +0x60, ResetAssignedInputs()). STLP_VECTOR_C is defined but not needed (VECTOR_ASSIGN graded the same). No Flags.
- Names: g_bLeftClickIsKing (0x013B85F4) is DESCRIPTIVE (no PDB/landed name found). 0x013B85F3 uses landed g_bAggressiveModeAvailable;
  0x013B8760 is DESCRIPTIVE g_StartupDefinitionManagerInstance (CCountedPointer<CStartupDefinitionManager>, presence test = IsNull). CControlsDef restated on CParentDefClassBase (only _quarantine header exists).
- Release variants of CDefPointer (POINTEE/COUNTREF/SELFDELETING) did not help; default in-place form is right.
- Repair (2026-11-12, verifier findings 1-4 all applied, still RELOCATION_MATCH, also strict): local GetDef body dropped for the header's
  FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY; 0x013B8760 respelled g_StartupDefinitionManagerInstance (CCountedPointer, IsNull);
  header comment names the 0x00410820 landed row and the retail fact (edi/ebp live across operator= at 0x004087E1..0x004087FA, 0x00408743).
- NAME_FIXES.tsv: 0x00410820 -> ??$GetDef@VCControlsDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCControlsDef@@@@@Z
  (was GetDef_00410820); 0x013B8760 -> g_StartupDefinitionManagerInstance (was g_value_/g_pool_/g_AutoTinyThunkObject_013b8760).

