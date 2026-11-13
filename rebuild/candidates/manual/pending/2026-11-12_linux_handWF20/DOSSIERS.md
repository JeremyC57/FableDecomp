## 0x00956060

# DOSSIER 0x00956060 CAIStateGroup_MazeTutorialBattleTeleport::Update (786 bytes)

## 1. Retail shape (xdis 00956060 0x322)
- thiscall, no args, plain `ret` (0x0095634E). Frame `sub esp,158h`, push ebx/ebp/esi/edi. NO frame pointer (ebp is a GPR: nav pointer / search tools), NO SEH (no fs:0) -> /O2 /Oy, no EH frame although locals have dtors (vector free, CDefPointer release, TeleportOut action dtor 0x0062E900 called explicitly).
- Free functions called __fastcall (GetValidPositions ecx=&vec edx=&map; remove_if ecx/edx; __copy ecx/edx; GFROR13; static GetFrame) -> the AI TU is /Gr (PDB: ?GetValidPositions@NCreatureHiding@@YIX..., GetFrame SIJXZ).
- `nop` at 0x0095620F = loop-head alignment; loop head 0x00956210 (GetWorld/GetWorldSeed/GFRandom) is AFTER the nav GetTC.
- Random range test KEPT in retail: 0x00956235 `test edi,edi ; je -> xor ebx,ebx` around `div edi`.
- Def released at the end with NO null test: 0x00956338 `dec [ebx+4] ; jne ; call [vtbl+4]`. Vector freed with null test (`test eax,eax ; je ; push ; call free`).

## 2. Identity
- Name: CAIStateGroup_MazeTutorialBattleTeleport::Update -- manifest label CORRECT (class+method).
- Mangled leaf: ?Update@CAIStateGroup_MazeTutorialBattleTeleport@@UAEXXZ (public virtual void Update()).
  Sources: ghidra_out/egor_pdb_names.tsv (Ego_r 0x0097AC80), ghidra_out/_fablewin_api.tsv (FableWin 0x0184F454 / 0x02BFBB40),
  rebuild/manifest/rtti-vtables.tsv: vtable 0x0128C2D8 slot 12 = 0x00956060 (slot 12 = Update for all Maze siblings: TutorialBattle 0x00955C90, DefensiveMelee 0x00955620, ShieldedChase 0x009552C0; landed RespondToLightning Update 0x00956380 says "Vtable slot 12").
  xref.py: only reference is the vtable (.rdata 0x0128C308 = 0x0128C2D8+0x30).
- Base: CAIStateGroup_MazeBase (Ego_r: PTargetThing CIntelligentPointer<CThing> +0x14, EnemyKnockDownFrame long +0x1C) <- CAIStateGroupBase.

## 3. Callees (landed file / landed name / PDB) -- draft spellings that are WRONG marked [X]
| addr | landed row | correct name | draft spelling |
|---|---|---|---|
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | CThing::GetPos() const (?GetPos@CThing@@QBEABVC3DVector@@XZ) | [X] GetPosition |
| 0x008C3510 | 00/8c/CDefClassBase_GetSubDefCMazeBattleDef_008c3510.cpp | CDefClassBase::GetSubDef<CMazeBattleDef>(CDefPointer<const CMazeBattleDef>*) const, ret 4 | [X] GetSubDef(CDefPointer&) non-const ref |
| 0x008FCFC0 | 00/8f/CAIStateGroupBase_GetWorld_008fcfc0.cpp | CAIStateGroupBase::GetWorld() const -> CWorld& (?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ) | [X] GetPWorld returning CWorld* |
| CWorld vslot 12 (+0x30) | engine/CWorld.h:111 | virtual CWorldMap& CWorld::GetWorldMap() (0x004AE8B0) | ok-ish (draft returns pointer) |
| [world+0x20] | engine/CWorld.h:196,308 | PThingSearchTools_ptr via PeekThingSearchTools() const / GetThingSearchTools() | [X] ThingSearchTools field |
| 0x0074A5D0 | 00/74/NCreatureHiding_GetValidPositions_0074a5d0.cpp | NCreatureHiding::GetValidPositions(vector<C3DVector>&,const CWorldMap&,const CThingSearchTools&,const C3DVector&,float,bool,const CTCShapeManager*) __fastcall | [X] missing namespace NCreatureHiding |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | CBaseIntelligentPointer::GetPItem() const | ok |
| 0x008AC060 | 00/8a/STL_removeif_008ac060.cpp | std::remove_if<C3DVector*, unary_negate<CVectorFilter_FurtherFromPointThan> > __fastcall ret 10h | [X] RemoveWithinRadius / CPositionWithinRadius |
| 0x0074CA20 | 00/74/global_CopySElement120074ca20_0074ca20.cpp | STLport __copy<T*> random-access (12-byte elems) -- inline vector::erase body calls it | [X] CopyPositions |
| 0x008FCF40 | 00/8f/CAIStateGroupBase_Terminate_008fcf40.cpp | CAIStateGroupBase::Terminate() | ok |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound (NON-const) | [X] CInterfaceMap::LowerBound |
| 0x0049D810 | 00/49/CWorld_GetWorldSeed_0049d810.cpp | CWorld::GetWorldSeed() const (?GetWorldSeed@CWorld@@QBEPAKXZ) | [X] GetRandomSeed |
| 0x00497890 | 00/49/global_GFROR13_00497890.cpp | GFROR13 (engine/GFROR13.h / GFRandom.h) | ok |
| (inline) | engine/GFRandom.h | GFRandom(unsigned long range, unsigned long* seed) __fastcall inline, `if(range) return *seed%range; return 0;` | [X] CWorld::Random |
| 0x006CB620 | 00/6c/CTCCreatureNavigation_IsAbleToNavigateFromPosToPos_006cb620.cpp | CTCCreatureNavigation::IsAbleToNavigateFromPosToPos(const C3DVector&,const C3DVector&,float) const | ok (draft non-const) |
| 0x008BCF20 | 00/8b/CCreatureActionTeleportOut_CCreatureActionTeleportOut_008bcf20.cpp | CCreatureAction_TeleportOut(CThingCreatureBase&, C3DVector&, bool) (engine/CCreatureAction_TeleportOut.h:9, under FABLE_RETAIL_CCREATUREACTION_TELEPORT) | [X] draft takes pointers |
| 0x0097DB70 | 00/97/CAIStateGroupBase_AIActionPerformActionAndWait_0097db70.cpp | EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase&, bool) ret 8 (CAIStateGroupBase.h:105-109) | [X] pointer arg / long return |
| 0x0062E900 | 00/62/CCombatActionBase_CCombatActionBase_0062e900.cpp | CCombatActionBase::~CCombatActionBase (temp action dtor) | ok |
| 0x0049D870 | 00/49/CWorld_GetFrame_0049d870.cpp | static CWorld::GetFrame() (?GetFrame@CWorld@@SIJXZ) | [X] GetFrameCount |
| 0x00BFEA14 | 00/bf/global_free_00bfea14.cpp | free (vector dealloc) | ok |
| def vslot 1 | CDefPointer.h | CDefPointeeBase Delete() on refcount 0 | ok |

## 4. Members
- this+0x04 PBrain (CAIStateGroupBase.h, FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS); PBrain+0x18 creature (CAIBrain::GetCreature, FABLE_RETAIL_CAIBRAIN_GETCREATURE + FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE -> CThingAICreature&).
- this+0x14 PTargetThing CIntelligentPointer<CThing> (Ego_r CAIStateGroup_MazeBase; header engine/_quarantine/CAIStateGroup_MazeBase.h; FABLE_RETAIL_CINTELLIGENTPOINTER). Draft called it PTarget [X].
- this+0x20 FrameOfLastTeleport long, +0x24 LastTeleportPos C3DVector (engine/CAIStateGroup_MazeTutorialBattleTeleport.h, generated, flattened: LastTeleportPos_X/Y/Z, no vptr modelling).
- creature+0x20 UsingComponents bitset<274> (bit 7 = TCI_NAVIGATION, `test cl,cl ; jns`), creature+0x44 Components CVectorMap -> CThing.h GetTC under FABLE_RETAIL_CTHING_GETTC_EXPANDED (exact retail shape: key local, map pointer local, ternary end-or-greater, non-const LowerBound 0x0040F020). CTCCreatureNavigation::StatGetInterfaceType() = TCI_NAVIGATION (CTCCreatureNavigation.h:66).
- creature+0x70 PBaseDef CDefPointer<const CThingBaseDef> (CThing.h:74; PeekBaseDef() const). Draft "Definition" [X].
- CMazeBattleDef retail +0x4C EnflameRange (engine/_quarantine/CMazeBattleDef.h:31, retail -4 shift from Ego_r +0x50; sibling IsRunnable 0x00954B50 uses TutorialTeleportTime retail +0x5C vs donor +0x60 -- consistent).
- world+0x20 PThingSearchTools_ptr (CWorld.h:308), world vslot 12 GetWorldMap (FABLE_RETAIL_CWORLD).
- Locals: teleportPos C3DVector esp+0x28; def esp+0x24; vector<C3DVector> esp+0x14/0x18/0x1C; TeleportOut temp esp+0x44 (0x120+ bytes); radius temp / nav share esp+0x20.
- Draft models LOCALLY (all have shared headers): C3DVector (engine/C3DVector.h), CWorld (engine/CWorld.h), CThing/CThingCreatureBase (CThing.h, CThingCreatureBase.h), CInterfaceMap (CVectorMap.h), CBaseIntelligentPointer (CIntelligentPointer.h), CAIStateGroupBase (CAIStateGroupBase.h), CAIBrain (CAIBrain.h), CDefPointer/CDefBase (CDefPointer.h, FABLE_RETAIL_DEF_CLASS_BASE), CMazeBattleDef (_quarantine), CDefClassBase (CParentDefClassBase.h), CTCCreatureNavigation (CTCCreatureNavigation.h), CCreatureAction_TeleportOut / CCombatActionBase (CCreatureAction_TeleportOut.h), positions vector (stlport/fable_containers.h std::vector), predicate (engine/CVectorFilter_FurtherFromPointThan.h), GetValidPositions (engine/NCreatureHiding.h), Random (engine/GFRandom.h), the class itself (CAIStateGroup_MazeTutorialBattleTeleport.h).

## 5. Callee visibility
- GetSubDef<CMazeBattleDef> 0x008C3510: sibling IsRunnable 0x00954B50 (same class, same TU) DEFINES it in-TU under `#pragma auto_inline(off)`; BriarRoseWillUse Update 0x00956FE0 notes "retail's LTCG saw its body" (same-TU noinline stand-in). Opt-in macro FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE exists (used by Guildmaster Update 0x00977E00). A visible body is the likely reason the def release after it has no null test.
- remove_if 0x008AC060: out of line COMDAT (fastcall, by-value 16-byte unary_negate copied into `sub esp,10h`). Use std::remove_if + std::not1 with FABLE_RETAIL_STLP_NOT1 / FABLE_RETAIL_STLP_REMOVE_COPY_IF + stlport/position_algorithms.h as JackOfBlades 0x00971ED0 does.
- vector::erase(first,last) and erase(pos): INLINE in retail (only __copy 0x0074CA20 called). Do NOT use FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE / _ERASE_RANGE_OUTOFLINE.
- GetPos, GetWorld, GetWorldSeed, LowerBound, GetPItem: out of line (not inlinable). GFRandom, GetTC, GetCreature, PeekThingSearchTools, GetFrame(static call kept), CDefPointer dtor: inline.

## 6. Sibling landed rows (idioms)
- rebuild/src/compiled/00/97/CAIStateGroupJackOfBladesStage1_GetRushToPosForLunge_00971ed0.cpp (VERIFIED_MATCH, Flags: none): THE template -- `std::vector<C3DVector> v; NCreatureHiding::GetValidPositions(v,GetWorld().GetWorldMap(),GetWorld().PeekThingSearchTools(),pos,11.0f,false,0); std::remove_if(v.begin(),v.end(),std::not1(CVectorFilter_FurtherFromPointThan(pos,10.0f))); GFRandom(n,GetWorld().GetWorldSeed())`. Defines: FABLE_RETAIL_CAISTATEGROUPBASE, _MEMBERS, _GOTOBED_WORLD, FABLE_RETAIL_CINTELLIGENTPOINTER(+_GET_ORIGINAL_CHAIN,_COMPARE_NULL_INT), CTHING, CTHINGPHYSICAL, CTHINGGAMEOBJECT, CTHINGCREATUREBASE, CWORLD, IS_THING_ALIVE, STLP_REMOVE_COPY_IF, STLP_NOT1; includes CWorld.h, CVectorFilter_FurtherFromPointThan.h, NCreatureHiding.h, GFRandom.h, stlport/position_algorithms.h.
- rebuild/src/compiled/00/97/CAIStateGroupGuildmaster_Update_00977e00.cpp (slot-12 Update, TeleportOut + PerformActionAndWait): FABLE_RETAIL_CTHING_GETTC_EXPANDED, CAISTATEGROUPBASE_GETCREATURE/_AIACTIONS/_PERFORMACTION/_GOTOBED_WORLD, CAIBRAIN_GETCREATURE, CCREATUREACTIONBASE, CCOMBATACTIONBASE_SOUNDCTOR, CCREATUREACTION_TELEPORT, CDEFCLASSBASE_GETSUBDEF_NOINLINE, DEF_CLASS_BASE, C3DVECTOR.
- rebuild/src/compiled/00/95/CAIStateGroupMazeTutorialBattleTeleport_IsRunnable_00954b50.cpp (same class): local same-TU GetSubDef<CMazeBattleDef> body with auto_inline(off); GetWorld().GetFrame(); FrameOfLastTeleport.
- rebuild/src/compiled/00/97/CAIStateGroupCreatureUseCoverBase_GetHiddenCoverPos_00970930.cpp: nav via bit-7 HasTC + one-ternary GetTC, then IsAbleToNavigateFromPosToPos.
- rebuild/src/compiled/00/95/CAIStateGroupBriarRoseWillUse_Update_00956fe0.cpp: GetSubDef + release with no null test.

## 7. Old draft model errors vs disassembly
- Names (section 3) and local stand-ins instead of shared headers -- respell all.
- Random: draft's CWorld::Random was folded (range known non-zero from do/while). Retail keeps `test edi,edi`. Use engine/GFRandom.h GFRandom(size, GetWorld().GetWorldSeed()); loop likely `while(v.size()!=0)` / size re-read, not a cached count proven non-zero. NOTES.md "next" suggests exactly this.
- GetValidPositions arg order: retail evaluates GetWorld() (map world, kept in edi) FIRST, then creature, then GetWorld()->+0x20 (search tools, ebp), pushes 0,0,15.0f, then GetPos(creature), then the virtual GetWorldMap() last. Draft wrote world-map, then search-tools as separate GetPWorld() calls; JackOfBlades spelling (GetWorld().GetWorldMap(), GetWorld().PeekThingSearchTools(), GetCreature().GetPos()) is the candidate.
- Predicate: retail calls GetPItem (PTargetThing) BEFORE computing radius `EnflameRange*2` (fld [def+4c]; fadd st,st; fstp temp), then GetPos -- i.e. `CVectorFilter_FurtherFromPointThan(PTargetThing->GetPos(), def->EnflameRange*2.0f)` wrapped in std::not1 (operator-> of CIntelligentPointer calls GetPItem first). Second filter radius 7.0f (0x40E00000), centre creature GetPos. Each erase is `v.erase(remove_if(v.begin(),v.end(),...), v.end())` with end() loaded before remove_if.
- Control flow matches draft: size==0 -> Terminate() then STILL teleports to creature start pos; loop: navigable -> teleportPos=v[idx]; else erase(v.begin()+idx); exhausted -> teleportPos=LastTeleportPos; then AIAction_PerformActionAndWait(CCreatureAction_TeleportOut(GetCreature(), teleportPos, false), false); FrameOfLastTeleport=GetWorld().GetFrame(); LastTeleportPos=teleportPos; Terminate().
- Nav GetTC is fetched ONCE before the loop (uninitialised if absent).
- Creature start pos copied first (before def/GetSubDef); def initialised to 0 after the &def push (CDefPointer default ctor); vector zero-init after GetSubDef.

## 0x00F39730

# DOSSIER 0x00F39730 CSurfaceCompositor::StartComposition (796 bytes)

## 1. Retail shape (xdis 00f39730 0x32c)
- __thiscall, `ret 4` (one bool arg), `sub esp,20h ; push ebx,ebp,esi,edi`, NO frame pointer (no /Oy-), NO SEH, no /GS. Speed-optimised (setters fully expanded). Flags: none needed (same as siblings).
- Frame 0x20: two C2DExtentsI hidden-result temps (+0x10, +0x18) and the C2DBoxF temp (+0x20). The Y float (fild [eax+4], first GetDimensions call) is spilled into the dead bool-param slot ([esp+0x34]) -- normal VC7.1 temp reuse; C2DBoxF(0,0,X,Y) args are evaluated right-to-left (Y first, then X).
- Register map: edi=this, ebp=CDisplayManager*, esi=CRenderManager* (core), ebx=0xF for first state then reused as table values.
- Sequence: [this+8]=1 ; StateManager(0x013CB508)->StartCaptureBlock (inline: StateStack[n].State=&BookmarkDummyState(+0x3A24), mask<<=1, ++n) ; AlphaTarget(+0x2C)=arg ; GFGetSystemManager()->+0x60 (display, ebp) ; GFGetSystemManager()->+0x60->+8 (core, esi) ; SetRenderState(0xF, ColourWriteEnable +0x2840) ; RenderTarget->call 0xA10E20 ; RenderTarget->AttachTarget(*DestSurface) 0xA10E60 ; display->SetRenderTarget(*RenderTarget) 0x9BF160 (result dropped) ; WasRendering(+9)=1 ; if(!display->Rendering(+0x214)) { display->BeginRender() 0x9BEF20 (result dropped); WasRendering=0 } ; core->SetDefaultRenderStates() 0xA059E0 ; OldWindow(+0x18)=core->GetCurrentAWindow() 0xA0AAA0 (4 dword copy) ; core->SetAWindow(C2DBoxF(0,0,(float)DestSurface->GetDimensions().X,(float)...Y)) 0xA0AA80 ; four inline SetRenderState: +0x2EE4 / +0x2F84 with D3DTextureUVModeTable[0] (0x01396DC8), +0x30C4 / +0x3164 with D3DTextureFilterModeTable[0] (0x01396E58) ; this->SetBaseBlendModes() 0xF38FF0 ; LayerCount(+0x28)=0.
- Each table value is RELOADED from memory after a block that wrote state (0xF398FC, 0xF399CC) -> the table is a non-const static (aliasing), i.e. the header's `static unsigned long D3DTextureUVModeTable[4]`, not an immediate.

## 2. Identity (two sources) -- manifest label is CORRECT
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x00BDE060) and FableWin.pdb (naming_stragglers/FableWin.exe.signatures.tsv 0x0336DC20): `?StartComposition@CSurfaceCompositor@@QAEX_N@Z` = public `void StartComposition(bool)`, non-const.
- Ego_r order: EndComposition 0xBDD310, ctor 0xBDD370, SetBlendMode 0xBDD3F0, SetBaseBlendModes 0xBDD970, StartComposition 0xBDE060, CompositeQuad 0xBDE360 == retail order EndComposition 0xF38930, ctor 0xF38990, SetBlendMode 0xF38A10, SetBaseBlendModes 0xF38FF0, StartComposition 0xF39730, CompositeQuad 0xF39A50 (one TU, CSurfaceCompositor.cpp).
- Only caller: 0x00B2B3A1 inside CEngineSurfaceCompositionManager::RenderToTexture 0x00B2B180 (not landed; compositor on its stack). rebuild/manifest/functions.tsv:55495 agrees (thiscall, void, bool).
- Param = AlphaTarget (stored to +0x2C, Ego_r member AlphaTarget); name the arg e.g. `alphaTarget` (param name not in any source -> descriptive).

## 3. Callees (retail target -> landed file -> PDB name)
| target | landed | PDB / correct spelling | draft |
|---|---|---|---|
| 0x009A4EC0 | 00/9a/global_GFGetSystemManager_009a4ec0.cpp | ?GFGetSystemManager@@YIPAVCSystemManager@@XZ (__fastcall) | ok |
| 0x00A10E20 | 00/a1/CRenderTarget_DetachTarget_00a10e20.cpp (MISNAMED stub, detaches +0x14 surface) | **?DetachZBuffer@CRenderTarget@@QAEXXZ** (Ego_r order DetachTarget 0xB7EE20, DetachZBuffer 0xB7EE30, Uninitialise 0xB7EE40 == retail 0xA10E10/0xA10E20/0xA10E30; body tests [ecx+0x18] = ZBufferSurface.PD3DSurface) | WRONG: draft calls `DetachTarget()` and comments 0x00A10E10. -> NAME_FIXES: 0x00A10E20 CRenderTarget::DetachZBuffer |
| 0x00A10E60 | 00/a1/CRenderTarget_AttachTarget_00a10e60.cpp | ?AttachTarget@CRenderTarget@@QAEXABVCSurface@@@Z -- takes `const CSurface&` | draft passes CSurface* (wrong type; call AttachTarget(*DestSurface)) |
| 0x009BF160 | 00/9b/CDisplayManager_SetRenderTarget_009bf160.cpp (view class) | ?SetRenderTarget@CDisplayManager@@QAE_NAAVCRenderTarget@@@Z -- `bool SetRenderTarget(CRenderTarget&)` | draft: void(CRenderTarget*) (wrong) |
| 0x009BEF20 | 00/9b/CDisplayManager_BeginRender_009bef20.cpp | ?BeginRender@CDisplayManager@@QAE_NXZ (bool) | draft: void |
| (inline) +0x214 | -- | ?IsRendering@CDisplayManager@@QBE_NXZ (FableWin only -> inline in release) reading `Rendering` | draft reads raw field |
| 0x00A059E0 | not landed | ?SetDefaultRenderStates@CRenderStateManager@@QAEXXZ (declared in CRenderStateManager.h) | draft put it on CRenderManagerCore (ok via inheritance) |
| 0x00A0AAA0 | landed only as ICF twin 00/a0/CVertexBufferWin32_DoSizeof_00a0aaa0.cpp (`mov eax,[ecx+3A3Ch]; add eax,204h; ret`) | **?GetCurrentAWindow@CRenderManagerCore@@QBEABVC2DBoxF@@XZ** (Ego_r 0xB74050 right after the two SetAWindow 0xB74030/40 == retail 0xA0AA80/90/A0); declared `const C2DBoxF& GetCurrentAWindow() const;` in CRenderManagerCore.h | WRONG: draft `GetAWindow` non-const |
| 0x00A0AA80 | 00/a0/CRenderManagerCore_SetAWindow_00a0aa80.cpp | ?SetAWindow@CRenderManagerCore@@QAEXABVC2DBoxF@@@Z | ok |
| 0x009F2F90 | 00/9f/CSurface_GetDimensions_009f2f90.cpp | ?GetDimensions@CSurface@@QBE?AVC2DExtentsI@@XZ (const) | ok (must stay out of line: do NOT define the CSurface.cpp per-TU inline opt-in) |
| 0x00F38FF0 | not landed | ?SetBaseBlendModes@CSurfaceCompositor@@IAEXXZ (protected) | ok |
| inline | CRenderStateManager.h retail branch | StartCaptureBlock(void*), SetRenderState (private), AddStateToUpdateList, SetColourWriteEnable, SetTextureUVModeU/V(long,TEXTURE_UV_MODE), SetTextureFilterModeMin/Mag(long,TEXTURE_FILTER_MODE) | draft hand-rolled `SetState` |
| inline | CRenderStateCaptureBlock.h | ?Capture@CRenderStateCaptureBlock@@QAEXXZ (FableWin only, no Ego_r/retail copy -> inline): Open=true; StateManager->StartCaptureBlock(this) | draft: `m_composing=true; g_pRenderStateManager->CreateCaptureBlock()` |

Globals: 0x013CB508 = `CRenderStateCaptureBlock::StateManager` (static, CRenderStateCaptureBlock.h), NOT g_pRenderStateManager. 0x01396DC8 = `CRenderStateManager::D3DTextureUVModeTable` (index 0 = TEXTURE_UV_WRAP), 0x01396E58 = `CRenderStateManager::D3DTextureFilterModeTable` (index 0 = TEXTURE_FILTER_POINT) -- draft's g_renderStateDefault_* are made-up names.

## 4. Members (ghidra_out/struct_layouts_egor.tsv)
CSurfaceCompositor (0x30, vtable 0x01301CF0, : CBaseClass) -- NO shared header; siblings model it locally (SetBlendMode 0x00F38A10 is the template):
+0x04 ECompositeBlendType CurrentBlend; +0x08 CRenderStateCaptureBlock StateCaptureBlock (bool Open); +0x09 bool WasRendering; +0x0C CSurface* DestSurface; +0x10 CCountedPointer<CRenderTarget> RenderTarget (retail reads Data word +0x10: operator->/Get, CCountedPointer.h line 152/154); +0x18 C2DBoxF OldWindow; +0x28 long LayerCount; +0x2C bool AlphaTarget.
Draft names wrong: m_composing/m_wasRendering/m_pSurface/m_pRenderTarget/m_savedWindow/m_quadCount/m_flag -> use PDB names above.
CRenderStateManager (engine/CRenderStateManager.h, FABLE_RETAIL_CRENDERSTATEMANAGER): StateStack +0x08 (CStateStackElement State/Value), StateUpdateList +0x2008, StateStackSize +0x2808, StateUpdateListSize +0x280C, CurrentBookmarkMask +0x2814, DeviceState +0x2818, BookmarkDummyState +0x3A24. CStateInfo: CurrentState +0, DesiredState +4, BookmarkMask +8, DirtyListFlag +0x10.
CDeviceState offsets touched: ColourWriteEnable +0x28 (abs +0x2840), TextureUVModeU[0] +0x6CC (+0x2EE4), TextureUVModeV[0] +0x76C (+0x2F84), TextureFilterModeMin[0] +0x8AC (+0x30C4), TextureFilterModeMag[0] +0x94C (+0x3164).
CDisplayManager (engine/CDisplayManager.h, FABLE_RETAIL_CDISPLAYMANAGER): PRenderManager +0x08 (scoped_ptr, GetRenderManager() inline, needs FABLE_RETAIL_SCOPED_PTR), Rendering +0x214 (private; no IsRendering() declared -> author must add an inline `bool IsRendering() const {return Rendering;}` opt-in, or the header has none today). SetRenderTarget is NOT declared in the header (add declaration opt-in `bool SetRenderTarget(CRenderTarget&); // 0x009BF160`).
CSystemManager (engine/CSystemManager.h, FABLE_RETAIL_CSYSTEMMANAGER): GetDisplayManager() line 71 (PDisplayManager +0x60).
CRenderManagerCore (engine/CRenderManagerCore.h, FABLE_RETAIL_CRENDERMANAGERCORE): : CRenderStateManager; GetCurrentAWindow/SetAWindow declared.
C2DBoxF (engine/C2DBoxF.h): members TLX,TLY,BRX,BRY; 4-float ctor under FABLE_RETAIL_C2DBOXF.
Locally modeled in draft but shared header exists: C2DBoxF, CSurface, CRenderStateManager, CRenderManagerCore, CDisplayManager, CSystemManager, CRenderStateCaptureBlock, C2DExtentsI (C2DExtentsI.h), CStateInfo (nested in CRenderStateManager).

## 5. Callee visibility / opt-ins
- State setters: inline FABLE_FORCEINLINE in CRenderStateManager.h retail branch (no retail copies). 0xF to ColourWriteEnable: either `SetColourWriteEnable(true,true,true,true)` (FABLE_RETAIL_CRENDERSTATEMANAGER_COLOURWRITE4, the 4-bool PDB overload SetBlendMode already uses with mask 7) or `SetColourWriteEnable(true)` (FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE, `enable ? 15 : 0`). Both are real PDB overloads; the prior residue (0xF materialised in ebx, `push ebx`) may depend on which: try the 4-bool form first (same TU as SetBlendMode, which uses COLOURWRITE4), then the 1-bool form.
- SetRenderState has an EARLY_RETURN variant (FABLE_RETAIL_CRENDERSTATEMANAGER_EARLY_RETURN); header notes bookmark test/or operand order varies per block in retail (lane gap). Retail here: `mov edx,[esi+2814h]; mov ecx,[eax+8]; test edx,ecx; jne; or ecx,edx; mov [eax+8],ecx` in all 5 blocks.
- Capture(): declared out-of-line only in CRenderStateCaptureBlock.h; retail inlines it (Open store at 0xF39739 then StartCaptureBlock). Needs a new inline opt-in (e.g. Capture(){Open=true;StateManager->StartCaptureBlock(this);}) -- EndComposition 0xF38930 shows Restore() inline too (Open=false; RestoreCaptureBlock()).
- Out of line in retail (keep as calls): GetCurrentAWindow 0xA0AAA0, SetAWindow, GetDimensions 0x9F2F90 (do not define the CSurface per-TU inline), SetDefaultRenderStates, SetBaseBlendModes (same TU, earlier, 0x740 bytes, never inlined), DetachZBuffer/AttachTarget (CRenderTarget.cpp), BeginRender/SetRenderTarget.
- No STLport helpers.

## 6. Siblings
- 00/f3/CSurfaceCompositor_SetBlendMode_00f38a10.cpp (best template): `// Flags: none`; defines FABLE_RETAIL_SCOPED_PTR, _CRENDERSTATEMANAGER_COLOURWRITE4, _CSYSTEMMANAGER, _CDISPLAYMANAGER, _CRENDERMANAGERCORE, _CTEXTURE, _CPIXELFORMAT, _C2DBOXF; includes engine/CSystemManager.h, CDisplayManager.h, CRenderManager.h, CBaseClass.h, CRenderStateCaptureBlock.h; local `class CSurfaceCompositor : public CBaseClass` with PDB members; `CRenderManager& rm = GFGetSystemManager()->GetDisplayManager()->GetRenderManager();`.
- 00/f3/CSurfaceCompositor_EndComposition_00f38930.cpp (local models, no flags; two GFGetSystemManager calls: display then core).
- 00/f3/CSurfaceCompositor_CompositeQuad_00f39a50.cpp, 00/f3/CSurfaceCompositor_CSurfaceCompositor_00f38990.cpp (CCountedPointer.h shared header), 00/f3/CSurfaceCompositor_scalardeletingdestructor_00f39d10.cpp.
- Same setter usage: 00/9d/CRenderManager2D_Render2DDrawList_009da9f0.cpp (SetTextureFilterModeMin(0,..), SetTextureUVModeU(0,..)), 00/bb/CEnginePrimitiveManagerParticleGroup_RenderSubPrimitive_00bbc460.cpp.

## 7. Old draft model errors
1. Hand-rolled SetState/CTrackedRenderState/local CRenderManagerCore instead of the header's SetColourWriteEnable / SetTextureUVModeU(0,TEXTURE_UV_WRAP) / SetTextureUVModeV(0,TEXTURE_UV_WRAP) / SetTextureFilterModeMin(0,TEXTURE_FILTER_POINT) / SetTextureFilterModeMag(0,TEXTURE_FILTER_POINT) -- table loads must come from the static D3D tables indexed by enum 0 (retail reloads them after each write).
2. Capture modeled as `m_composing=true; g_pRenderStateManager->CreateCaptureBlock()` -> StateCaptureBlock.Capture() inline (StartCaptureBlock(this) on CRenderStateCaptureBlock::StateManager).
3. Wrong callee: DetachTarget (0xA10E10) -> retail calls DetachZBuffer (0xA10E20).
4. Wrong signatures: AttachTarget(const CSurface&), SetRenderTarget(CRenderTarget&) returns bool, BeginRender returns bool, GetCurrentAWindow() const (not GetAWindow); display->Rendering should be IsRendering() inline.
5. Order matches retail otherwise (WasRendering=true; if(!IsRendering()){BeginRender(); WasRendering=false;}; OldWindow assigned before SetAWindow; C2DBoxF(0.0f,0.0f,(float)X,(float)Y) with two separate GetDimensions() calls).
6. Residue in history (786v796): 0xF must sit in ebx (`mov ebx,0Fh ; cmp ecx,ebx`) and stores `mov [eax+4],ebx`; the draft's immediate gave a shorter body -- likely fixed by the real header setter expression (bool->`?:` mask) rather than a literal.

## 0x00523CF0

# DOSSIER 0x00523CF0 CThingManager::RuntimeSaveGamePersistentEntitiesInLoadedMaps (799 B)

## 1. Retail shape (xdis 00523cf0 0x32f)
- thiscall, `ret 4` (one bool arg). No frame pointer (sub esp,0x44 first). No SEH. ebx/edi pushed at entry, ebp/esi pushed
  only after the `GetNoMaps() > 1` guard (shrink-wrapped). Loop top 0x523D30 aligned by `lea ecx,[ecx]`.
- Outer loop: `for (long map = 1; map < WorldMap.GetNoMaps(); ++map)`, signed (jle/jl), rotated (guard + bottom test), the
  count recomputed every iteration via `add eax,0x20` (&Maps) then (finish-start)/0x48 (0x38E38E39, sar 4) = CArray<CMapInfo>::size().
  map in ebx. A strength-reduced frame word [esp+0x18] = map*0x18 (init 0x18, +0x18/iter) = &GamePersistentEntityScripts[map] offset.
- Frame 0x44 (offsets with 4 pushes): +10 CCharString text (text path) | +14 list iterator / hero | +18 map*24 | +1C "" temp
  (binary path) | +20 thing | +24..2F vector<CThing*> things | +30..3B vector<CCharString> names | +3C..53 CMemoryDataOutputStream
  (0x18) | +58 = arg `binary` (also used as the empty __true_type tag slot for _M_insert_overflow, same trick as sibling 523B40).
- Per map: `if (!WorldMap.IsMapLoaded(map)) continue;` (vtable slot 10 +0x28). things zeroed (xor ebp,ebp; stores +24/+2C/+28)
  BEFORE the `World.IsSerialisingHeroState()` ([this+0x24]+0x102) test.
  * hero branch (+0x102 true): CMainGameComponent::Get() (0x013B86A0) +0x1C -> GetMainPlayer 0x449970 (non-const) ->
    GetPControlledCreature 0x487DC0; `if (hero && !hero->Dead && hero->CurrentMapNumber == map)` then DIRECT call
    _M_insert_overflow(finish=0, &hero, tag, 1, true) (push_back on a known-empty vector: finish==eos folded).
  * list branch: walk CClassFactory::ClassList (std::list<CClass*>, head ptr at this+0x18): it=head->next, end re-read every
    iteration (`cmp eax,[edi+18h]`), iterator spilled at +14. thing=node+8 spilled +20. Order: `thing->IsGameStatePersisted()`
    CALL FIRST, then `test esi,esi` (thing != 0), then Dead bit (+0x91 &1), then movsx +0x8e == map, then inline push_back
    (cmp finish,eos; placement new WITH null check `test ebp,ebp` => operator new(size_t,void*) throw() as in <new>/stlport;
    else _M_insert_overflow(finish,&thing,tag,1,true), finish reloaded from +28). After loop `xor ebp,ebp` (zero reg re-made).
- names vector zeroed AFTER the branch (+30/+34/+38), then SerialisingGameState(+0x9A)=1, then `if (binary)`.
  * binary: CMemoryDataOutputStream ctor 996AF0; ReserveSize(0x2000) 996BC0; SaveEntitiesBinary(stream,2,things,names) 522160;
    `lea esi,[edi+0A0h]`; GetLength 994B40 evaluated BEFORE PeekData 995E70; SetGamePersistentSaveData(map,PeekData(),GetLength()) 523860;
    CCharString("",-1) 99EBF0 at +1C (0x0122D70E); SetGamePersistentScript(map,tmp) 5237F0; ~CCharString; ~stream 996B30.
  * text: CCharString() 99E4B0 at +10; Reserve(0x8000) 99EF90; SetUseFastExtend(true) 99EF70; SaveEntitiesToScript(text,2,things,names)
    521EB0; SetGamePersistentScript(map,text); THEN INLINE: resize(needed>size()?needed:size()) 52C6A0 with needed=map+1 in ebp
    (size() computed twice, 0x2AAAAAAB/sar 2); rec=GamePersistentEntityScripts._M_start + [esp+18]; rec.UncompressedSize(+0x10)=0;
    rec.Valid(+0x14)=1; ~CCharString.  == body of the landed SetGamePersistentSaveData 0x523860 with size==0 folded (no compressor).
    NB: siblings 523B40 / 5239C0 CALL 523860 out of line with (x,0,0); here retail expanded it. Try SetGamePersistentSaveData(map,0,0)
    with its body visible (same TU, defined earlier) / FABLE_FORCEINLINE copy; else spell the three statements by hand.
  * SerialisingGameState=0; names dtor inlined (loop ~CCharString 99EAE0 over [start,finish), then free 0xBFEA14 if start);
    things dtor inlined (`if (start) free(start)`).

## 2. Name (proven, two sources)
- ?RuntimeSaveGamePersistentEntitiesInLoadedMaps@CThingManager@@AAEX_N@Z: ghidra_out/egor_pdb_names.tsv (Ego_r 0x0052E950)
  + FableWin signatures (017F7A42 / 01D36960 in naming_stragglers/FableWin.exe.signatures.tsv and _fablewin_api.tsv).
  Private, void, (bool). Manifest label CORRECT. Sibling PDB names: RuntimeSaveGameHero (AAEX_N, retail 523B40, landed),
  RuntimeSaveLevelPersistentEntitiesInLoadedMaps (AAEXXZ). Caller: 0x00524381 inside CThingManager::SaveGameState (0x00524290,
  not landed; it also calls RuntimeSaveGameHero at 0x0052434C).

## 3. Callees (landed file under rebuild/src/compiled/00/xx/; PDB spelling)
- slot 10 CWorldMap::IsMapLoaded(long) const (engine/CWorldMap.h, FABLE_RETAIL_CWORLDMAP). Draft "IsMapLoaded(long)" non-const + fake m00..m09.
- 0x449970 CPlayerManager::GetMainPlayer() -> CPlayer& (non-const)  CPlayerManager_GetMainPlayer_00449970.cpp. Draft returned CPlayer* (wrong).
- 0x487DC0 CPlayer::GetPControlledCreature() const -> CThingPlayerCreature*  (draft: GetHeroCreature returning CThing* -- WRONG name).
- 0x4C8280 CThing::IsGameStatePersisted() const (engine/CThing.h under FABLE_RETAIL_CTHING_STATEACCESS). Draft non-const.
- 0x436AC0 std::vector<CThing*>::_M_insert_overflow(pos,const T&,const __true_type&,size_type,bool) (stdvector_Minsertoverflow_00436ac0.cpp). Draft "InsertGrow" WRONG.
- 0x996AF0/0x996B30 CMemoryDataOutputStream ctor/dtor; 0x996BC0 ReserveSize(unsigned long) (draft "Reserve" WRONG);
  0x994B40 GetLength() const (draft "GetSize" WRONG); 0x995E70 PeekData() const -> const void*. engine/CMemoryDataOutputStream.h (FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM).
- 0x522160 CThingManager::SaveEntitiesBinary(CMemoryDataOutputStream&, long, const vector<CThing*>&, vector<CCharString>&)
  (draft "SavePersistentEntitiesInRegions", non-const vector -- WRONG); 0x521EB0 SaveEntitiesToScript(CCharString&, long, const vector<CThing*>&,
  vector<CCharString>&) (draft "SaveEntitiesAsText" WRONG). Both landed, same TU, earlier addresses.
- 0x523860 NThingManager::CEntityRuntimePersistence::SetGamePersistentSaveData(long,const void*,unsigned long) (PDB QAEXJPBXK; landed
  CEntityRuntimePersistence_SetGamePersistentSaveData_00523860.cpp). 0x5237F0 SetGamePersistentScript(long,const CCharString&)
  (draft "SetGamePersistentSaveText" WRONG). 0x52C6A0 vector<CEntityScriptInfo>::resize(size_type) (landed).
- CCharString: 0x99E4B0 (), 0x99EBF0 (const char*, long=-1), 0x99EAE0 ~, 0x99EF90 Reserve(long) (?Reserve@CCharString@@QAEXJ@Z; landed
  file mislabelled C3DAnimationBlendState_AddCombinationBlend_0099ef90.cpp -- manifest label wrong, use the PDB name), 0x99EF70
  SetUseFastExtend(bool). Not declared in fable_string.h: siblings declare them locally (see 523B40).
- 0xBFEA14 free (toolchain). Global 0x013B86A0 = CMainGameComponent::PMainGameComponent via static Get() (engine/CMainGameComponent.h);
  draft "g_pPlayerManagerOwner" WRONG.

## 4. Members
- this+0x18 CClassFactory::ClassList (std::list<CClass*>; Ego_r TSV ClassList) -- engine/CClassFactory.h FABLE_RETAIL_CCLASSFACTORY has it
  only as `unsigned long ClassList[1]` (opaque, protected). Draft "Things" local node struct.
- this+0x24 CWorld& World; World+0x102 bool SerialisingHeroState, inline IsSerialisingHeroState() (FableWin+Ego_r) in engine/CWorld.h
  (FABLE_RETAIL_CWORLD). Draft "Settings->HeroOnly" WRONG names.
- this+0x28 CWorldMap& WorldMap; +0x20 CArray<CMapInfo> Maps (sizeof CMapInfo 0x48, engine/CMapInfo.h). ?GetNoMaps@CWorldMap@@QBEJXZ
  (inline, Ego_r+FableWin) is NOT in engine/CWorldMap.h -- needs `long GetNoMaps() const { return Maps.size(); }` (add opt-in).
- this+0x9A bool SerialisingGameState; this+0xA0 NThingManager::CEntityRuntimePersistence EntityRuntimePersistence (engine/CThingManager.h;
  needs FABLE_RETAIL_CCLASSFACTORY). Persistence +0xC GamePersistentEntityScripts (CArray<CEntityScriptInfo>, 24B: Script, SaveData,
  UncompressedSize +0x10, Valid +0x14) -- engine/NThingManager_CEntityRuntimePersistence.h (private, friend CThingManager).
- CThing +0x8E short CurrentMapNumber (GetCurrentMapNumber() const inline, FableWin), +0x91 bit0 Dead (IsAlive()/IsDead() inline) -- engine/CThing.h.
- CMainGameComponent +0x1C PPlayerManager; GetPlayerManager() under FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER +
  FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER.
- Draft models LOCALLY (all have shared headers): CThing, CPlayer, CPlayerManager, CWorldMap(Manager), CThingManager, CEntityRuntimePersistence,
  CMemoryDataOutputStream, CCharString, CMainGameComponent, CWorld ("CSaveSettings").

## 5. Visibility
- Same TU (ThingManager.cpp, earlier addresses): SaveEntitiesToScript 521EB0, SaveEntitiesBinary 522160, SetGamePersistentScript 5237F0,
  SetGamePersistentSaveData 523860 (expanded here!), RuntimeSaveEntities 5239C0, RuntimeSaveGameHero 523B40. Sibling 523B40 uses
  noinline same-TU stand-ins for the two savers and for vector<CThing*>::_M_insert_overflow (body reads x, never the tag) -- copy them.
- STLport: vector<CThing*>::_M_insert_overflow 0x436AC0 out of line (COMDAT); vector<CEntityScriptInfo>::resize 0x52C6A0 out of line.
  stlport/fable_containers.h macros: FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE / _NOINLINE (not wanted: push_back is inline here),
  FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE (for resize), list: FABLE_RETAIL_STLP_LIST_* (iteration needs none).
- vector<CCharString> dtor: inline-expanded (523B40 uses FABLE_FORCEINLINE ~vector).

## 6. Siblings
- rebuild/src/compiled/00/52/CThingManager_RuntimeSaveGameHero_00523b40.cpp (closest twin: hero path + both save paths; local STLport
  vector model, FORCEINLINE ~vector, noinline saver stand-ins, CMainGameComponent::Get()->PlayerManager->GetMainPlayer().GetPControlledCreature()).
- CThingManager_RuntimeSaveEntities_005239c0.cpp (// Flags: none; copies 523B40 model verbatim).
- CEntityRuntimePersistence_SetGamePersistentSaveData_00523860.cpp (the expanded body: `unsigned long needed=index+1; resize(needed>size()?needed:size())`).
- NThingManagerCEntityRuntimePersistence_SetGamePersistentScript_005237f0.cpp (shared CArray.h + MAX macro form).
- CThingManager_RuntimeLoadEntities_00522720.cpp (shared-header style: FABLE_RETAIL_CCHARSTRING/CCLASSFACTORY/CARRAY/SCOPED_PTR/
  CGAMECOMPONENT/CMAINGAMECOMPONENT + engine/CThingManager.h).
- No `#pragma optimize` in any sibling; default flags.

## 7. Draft model errors vs disassembly
- Hero path used inline push_back -> retail calls _M_insert_overflow directly (finish==eos==0 known). Write it so c2 folds: the
  vector is constructed immediately before (zero stores precede the branch) -- the sibling 523B40 got this fold with push_back + its
  same-TU _M_insert_overflow stand-in; the draft's local InsertGrow (no body) can't fold.
- Map count: must be WorldMap.GetNoMaps() = Maps.size() through &Maps (add eax,0x20), not a raw end-start.
- Frame 0x44 vs draft 0x4C: retail has distinct slots for text CCharString (+10) and the "" temp (+1C); stream 0x18; no extra slots.
- Text tail must be the folded SetGamePersistentSaveData(map,0,0) (resize then UncompressedSize=0, Valid=true), indexed through the
  map*24 induction word; draft hand-wrote it with a local Max template (ok shape, wrong ownership/names).
- All callee names above flagged WRONG; persistence is private in the shared header (friend CThingManager covers it).

## 0x00918B90

# DOSSIER 0x00918B90  CAIStateGroup_Gaze::Update  (802 bytes)

## 1. Retail shape (xdis 00918b90 0x332)
- __thiscall, `ret` (no args). NO frame pointer (`sub esp,34h ; push ebx,ebp,esi,edi`) -> /Oy on, no optimize("y",off).
- NO SEH prologue even though two dtor-bearing locals exist (CIntelligentPointer temp, CDefPointer navDef) -> no /EHsc
  unwinding in this TU (siblings carry `// Flags: none`).
- Frame 0x34: [esp+13] byte bool (HasTC result, later passed as `bool& moving` -- SAME SLOT), [esp+14] navDef (CDefPointer),
  [esp+18] intelligent-pointer temp (vptr 0x0127B4D8), reused for the GetTC key 0x2F and the RotateXY float arg, and read
  uninitialised as the TC pointer on the !HasTC path; [esp+20..28] pos; [esp+2c..34] facing; [esp+38..40] jitter.
- ebp = CMainGameComponent::ConstantFPS (0x01375550) loaded AFTER the `div 6`, BEFORE the GetFrame call -> FPS must be read
  in source before CWorld::GetFrame() (named local / evaluated first). Old draft: frame 0x38, ebp unused (STATUS.tsv).
- Two loop-alignment pads `lea ebx,[ebx+0]` (8d9b00000000) at 0x918DFA and 0x918E7A (loop tops of LookAt and DoNothing loops).
- Epilogue: early-Terminate path at 0x918BE3 (no navDef yet). Second Terminate path (TC user busy) at 0x918C5F tests navDef
  (`test ecx,ecx ; je`) then jumps into the shared `dec [ecx+4] ; jne ; call [vtbl+4]` tail; the normal path falls into
  that tail WITHOUT a null test.

## 2. Name / class / signature (two+ sources)
- ?Update@CAIStateGroup_Gaze@@MAEXXZ  -> `protected: virtual void CAIStateGroup_Gaze::Update()`.
  Sources: vtable CAIStateGroup_Gaze 0x0128FDEC slot 12 = 0x00918B90 (rebuild/manifest/rtti-vtables.tsv; CAIStateGroupBase.h
  slot 12 `virtual void Update() = 0`), xref = 5 vtable refs (0x0128FE1C Gaze + GazeOutside/AtHome/AtWork/+0x012917B4 inherit it),
  labels_rtti_port.tsv, engine_api.tsv, bsim_port_audit_final.tsv (0.746 vs Ego_r ?Update@CAIStateGroup_Gaze 0x0093C5F0),
  FableWin _fablewin_api.tsv 0x017ADA9B. Manifest label is CORRECT. Mangled leaf: ?Update@CAIStateGroup_Gaze@@MAEXXZ.

## 3. Callees (landed file | landed name | PDB name)  ** = draft spelling disagrees with PDB
- 0x00918880 00/91/CAIStateGroupGaze_FindFreeGazePos_00918880.cpp | FindFreeGazePos | ?FindFreeGazePos@CAIStateGroup_Gaze@@IAEPAVCThingGameObject@@XZ (protected). ok
- 0x00A01B10/B50/B90/C10 00/a0/CBaseIntelligentPointer_* | ctor/GetPItem/SetPItem/dtor | CBaseIntelligentPointer (CIntelligentPointer.h). ok
- 0x008FCF40 00/8f/CAIStateGroupBase_Terminate_008fcf40.cpp | ?Terminate@CAIStateGroupBase@@QAEXXZ. ok
- 0x006CD2F0 00/6c/CDefClassBase_GetSubDefCCreatureNavigationDef_006cd2f0.cpp |
  ??$GetSubDef@VCCreatureNavigationDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCCreatureNavigationDef@@@@@Z
  ** draft: CTCCreatureNavigation::GetSubDef(CDefPointer&) on "PNavigation +0x70" -- WRONG. ecx = creature->PBaseDef.Object
  (CThing +0x70 = CDefPointer<const CThingBaseDef> PBaseDef, CThing.h static-assert), arg = CDefPointer<const CCreatureNavigationDef>*.
  Spelling: `GetCreature().PBaseDef->GetSubDef(&navDef)` (or PeekBaseDef()/GetPBaseDef()).
- 0x0040F020 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap::LowerBound (out of line) -- via CThing::GetTC<T> expanded body.
  ** draft local CInterfaceMap::LowerBound stand-in defined in-TU with auto_inline(off): replace with CThing.h FABLE_RETAIL_CTHING_GETTC_EXPANDED.
- 0x00916640 00/91/CTCExclusiveUse_SetUser_00916640.cpp (landed as placeholder `CClass_00916640::AssignTarget_00916640`) |
  PDB ?SetUser@CTCExclusiveUse@@QAEXPAVCThing@@@Z (Ego_r 0x0080C470, FableWin 0x017F33BB).
  ** draft: CTCGazeMarker::AssignCreatureToTask -- WRONG. TC type 0x2F = TCI_EXCLUSIVE_USE (ETCInterfaceType.h:52);
  CTCExclusiveUse::User (CIntelligentPointer<CThing>) +0x0C (struct_layouts_egor). Also flag landed placeholder name -> NAME_FIXES.
- 0x008FCFC0 00/8f/CAIStateGroupBase_GetWorld_008fcfc0.cpp | ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ (returns CWorld&). ** draft GetPWorld.
- 0x0049D810 00/49/CWorld_GetWorldSeed_0049d810.cpp | ?GetWorldSeed@CWorld@@QBEPAKXZ (const, unsigned long*). ** draft GetPRandomSeed.
- 0x004978A0 global_GFFloatRandom | ?GFFloatRandom@@YIMMPAK@Z (__fastcall float(float range, unsigned long* seed)). ok
- 0x00A14040 00/a1/C3DVector_RotateXY_00a14040.cpp | ?RotateXY@C3DVector@@QAEXM@Z. ** draft RotateAboutZ.
- 0x00A146C0 C3DVector_GetRandomPointWithinUnitSphere | ?GetRandomPointWithinUnitSphere@C3DVector@@SI?AV1@PAK@Z (STATIC member
  __fastcall). ** draft global free function.
- 0x004C73D0 00/4c/CThing_GetPosition_004c73d0.cpp | ?GetPos@CThing@@QBEABVC3DVector@@XZ. ** draft GetPosition.
- 0x0097E890 CAIStateGroupBase_AIActionMoveToPosProximity | ?AIAction_MoveToPosProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@MAA_NW4EMovementType@@_N44PBVCThing@@4M@Z. ok
- 0x0097D580 CAIStateGroupBase_AIActionLookAtPos | AIAction_LookAtPos(EAIActionRepeat,C3DVector,long,float). ok
- 0x008FCFD0 GetPWorldSeed | ?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ. ok
- 0x00497890 GFROR13 | ?GFROR13@@YIJK@Z (returns long). draft returns unsigned long (minor).
- 0x0049D870 00/49/CWorld_GetFrame_0049d870.cpp | ?GetFrame@CWorld@@SIJXZ (static __fastcall long). ** draft GetWorldFrame.
- 0x0097E200 AIAction_DoNothing. ok
- vcall [this vtbl+0x38] slot 14 = ?GetOwner@CAIStateGroup_Gaze@@MAEPAVCThingBuilding@@XZ (0x00918EC0 returns 0; CAIStateGroup_Gaze.h
  declares it). ** draft "GetLookTarget" returning CThing* -- WRONG.
- vcall [PhysicsTC vtbl+0x118] slot 70 = CTCPhysicsBase::GetFacingVector() const (hidden-result overload; CTCPhysicsBase.h:121-126).
  ** draft "CThingPlacement::GetForward".
- Data: 0.2f imm 0x3E4CCCCD, 0.1f @0x0122EE58, 0.25f @0x0123078C, 1.0f imm, LookAt tolerance imm 0x3B360A13 = 0.0027777f
  (** draft 0.00277778f -> likely 0x3B360B61, WRONG constant; use 0.0027777f per hand277 LESSONS), flags 0x11, ConstantFPS @0x01375550.

## 4. Members
- this (CAIStateGroup_Gaze.h under FABLE_RETAIL_CAISTATEGROUP_GAZE, needs FABLE_RETAIL_CAISTATEGROUPBASE[_MEMBERS]):
  PBrain +0x04; GazeMarker CIntelligentPointer<CThingGameObject> +0x14; ChanceOfRunning float +0x1C; MinJourneyForRunning float +0x20.
- CAIBrain +0x18 Creature (CThingAICreature*) -> GetCreature(): FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE + FABLE_RETAIL_CAIBRAIN_GETCREATURE
  (returns CThingAICreature&).
- CThing (retail CThing.h): +0x24 UsingComponents word 1 (bit 15 = type 47) [HasTC]; +0x44 Components (CVectorMap) [GetTC];
  +0x60 PhysicsTC (CTCPhysicsBase*); +0x70 PBaseDef (CDefPointer<const CThingBaseDef>).
- CTCPhysicsBase +0x0C Position (inline `const C3DVector& GetPosition() const`, CTCPhysicsBase.h:233) -- retail copies pos BEFORE
  the slot-70 call; CThing::GetPos 0x004C73D0 is the out-of-line `PhysicsTC->Position` used for the creature.
- CThingAICreature +0x170 = WorkBuilding (CIntelligentPointer<CThingBuilding>) -> inline ?GetWorkBuilding@CThingAICreature@@QBEPAVCThingBuilding@@XZ
  (FableWin 0x017AC8A8): FABLE_RETAIL_CTHINGAICREATURE_WORKBUILDING (nested in _SAVE block per CThingAICreature.h:135-146; body in includer).
  ** draft calls it "LookAtThing" on CThingCreatureBase.
- CTCExclusiveUse +0x0C User (CIntelligentPointer<CThing>); no shared header found (grep ls engine: none) -> local model OK (only SetUser + User).
- CCreatureNavigationDef +0x34 bool AvoidDynamicObstacles (egor; engine/CCreatureNavigationDef.h exists). ** draft "AvoidOtherCreatures".
- CDefPointee refcount +0x04, Delete/Release virtual slot 1: CDefPointer.h (Release variants FABLE_RETAIL_CDEFPOINTER_RELEASE_*).
- Locally modelled in old draft though a shared header exists: C3DVector, CThing, CThingGameObject, CBaseIntelligentPointer/
  CIntelligentPointer, CDefPointer, CCreatureNavigationDef, CTCCreatureNavigation(bogus), CAIBrain, CWorld, CAIStateGroupBase,
  CAIStateGroup_Gaze, CThingCreatureBase/AICreature, CVectorMap(as CInterfaceMap), CTCPhysicsBase(as CThingPlacement),
  CMainGameComponent (g_FramesPerSecond extern instead of CMainGameComponent::GetConstantFPS()).

## 5. Callee visibility
- GetTC<T>/HasTC: __forceinline in retail (no out-of-line copy); bit test here is the SHIFT form `shr edx,0Fh ; and dl,1 ;
  mov [esp+13h],dl ; je` = materialised bool -> FABLE_RETAIL_CTHING_HASTC_MATERIALISED (CThing.h:296) + FABLE_RETAIL_CTHING_GETTC_EXPANDED
  (key 0x2F stored inside the test, map pointer local in ebx = this+0x44, end read `[ebx+4]`). The !HasTC path loads the
  uninitialised out-pointer (`mov ebx,[esp+18h]`), i.e. the TC pointer is declared uninitialised and used regardless.
- LowerBound 0x0040F020 out of line (non-const). CIntelligentPointer<CThingGameObject>(T*) ctor INLINE here (base ctor + vptr store
  + SetPItem), even though an out-of-line copy exists at 0x00918170 in the same TU neighbourhood -> check
  FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR. operator= inline (GetPItem on temp, SetPItem on GazeMarker): FABLE_RETAIL_CINTELLIGENTPOINTER(_ASSIGN).
- C3DVector arithmetic + GetLength __forceinline (C3DVector.h). Z self-copy `mov ecx,[esp+28h] ; mov [esp+28h],ecx` after the
  jitter = `pos = pos + C2DVector-ish` 3-arg ctor with Z unchanged; C3DVector.h offers FABLE_RETAIL_C3DVECTOR_ADD2D(_DIRECT)
  `operator+(const C2DVector&)` returning C3DVector(X+v.X,Y+v.Y,Z) -- plausible original (pos = pos + C2DVector(jitter)*0.25f?).
- Random(6): seed inline `*s = *s*0x24A1+0x24DF; *s = GFROR13(*s); % 6` with no zero test (const range).
- Same-TU neighbours (0x00918190..0x00919020 all Gaze family) -- FindFreeGazePos (0x00918880) defined earlier in the TU.

## 6. Sibling rows
- 00/91/CAIStateGroupGaze_FindFreeParticularGazePos_00918640.cpp -- the ONLY sibling on the shared header. Defines:
  FABLE_RETAIL_CCHARSTRING, _CCLASS, _DEF_CLASS_BASE, _CTCBASE, _CTHING, _CTHINGAICREATURE, _CTHINGPLAYERCREATURE, _CTHINGPHYSICAL,
  _CTHINGGAMEOBJECT, _CTHINGCREATUREBASE, _CTHING_GETTC_EXPANDED, _CINTELLIGENTPOINTER, _CTCVILLAGE, _CTCTHINGOWNER,
  _CAISTATEGROUPBASE, _CAISTATEGROUPBASE_MEMBERS, _CAISTATEGROUPBASE_GETCREATURE, _CAIBRAIN_GETCREATURE, _CAISTATEGROUP_GAZE.
  Copy this include/define block.
- 00/91/CAIStateGroupGaze_InitCreaturePosition_00918ac0.cpp, CAIStateGroupGazeAtWork_InitCreaturePosition_00918b50.cpp,
  CAIStateGroupGaze_FindFreeGazePos_00918880.cpp -- older local models (no pragmas, no Flags).
- 00/93/CAIStateGroupMinionFaceHero_Update_009357f0.cpp -- `unsigned long fps = CMainGameComponent::GetConstantFPS();` + GetFrame +
  DoNothing wait idiom; 00/93/CAIStateGroupBanditSitRoundCampfire_Idle_0093a540.cpp -- "frame rate read into a local BEFORE the
  seed call". CMainGameComponent.h:189 offers `static long __fastcall GetConstantFPS()`.
- 00/91/CAIStateGroupLookAtInterestingThings_Update_009195d0.cpp -- neighbouring Update (uses auto_inline(off) GetSubDef stand-in).
- AIAction_MoveToPosProximity opt-in: FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS + _MOVETOPOSPROXIMITY (row 0x00940960); LookAtPos
  under _MEALACTIONS.

## 7. Old draft model errors (vs disassembly)
- Wrong callee/member identities (sec. 3/4): GetSubDef target, SetUser, GetOwner vs WorkBuilding, GetFacingVector, AvoidDynamicObstacles.
- LookAt sum operand order: retail `fld facing.Z ; fadd pos.Z` (and X/Y likewise) => `facing + pos`, draft writes `pos + facing`.
- LookAt tolerance constant (0.00277778f vs retail 0x3B360A13 = 0.0027777f).
- Comparison order of slot-14: retail calls WorkBuilding.GetPItem (edi) FIRST, then GetOwner, `cmp eax,edi ; jne skip-jitter`
  => try `if (GetCreature().GetWorkBuilding() == GetOwner())` or a named local for the work building.
- FPS: read CMainGameComponent::GetConstantFPS() into a local before GetFrame (retail ebp); end = GetFrame() + (r%6+3)*fps
  (retail: edi=GetFrame, ebx=(r+3)*fps, add). Loop `while (GetFrame() < end && DoNothing()==4)` signed jge -- draft ok.
- Normal-path def release has no null test in retail; draft's CDefPointer dtor `if (Object)` keeps one -> use the shared
  CDefPointer Release variant (study FABLE_RETAIL_CDEFPOINTER_RELEASE_* ) or note VC drops the test only when navDef deref'd
  before (`navDef->AvoidDynamicObstacles` in the move loop) -- the busy-path copy keeps `test ecx,ecx`.
- `bool& moving` shares [esp+13] with the HasTC bool: declare one bool used for both? (e.g. GetTC result variable reused, or VC slot
  sharing of disjoint-scope locals -- put the TC lookup in its own scope).
- MoveToPos args right: (2, pos, 0.0f, moving, movement, true, false, navDef->AvoidDynamicObstacles, 0, true, 0.0f); movement 3/5.
- Retail `rand <= ChanceOfRunning` gate (`fcomp [esi+1Ch] ; test ah,41h ; jp`) then `length > MinJourneyForRunning` (`test ah,41h ; jne`).

## 0x00972990

# DOSSIER 0x00972990 -- CAIStateGroup_JackOfBladesStage1::OnActivate (803 bytes)

## 1. Retail shape (xdis 00972990 0x333)
- __thiscall, `ret` (no args), NO frame pointer (sub esp,0x30 ; push ebx/ebp/esi/edi) -> plain /O2 /Oy, no SEH (no fs:0) even
  though the vector local has a dtor: dtor is inline `if(start) free(start)` at 0x00972C97..0x00972CAC (call 0x00BFEA14 _free).
- edi=this, ebx = creature ([[this+4]+0x18], later reused as 0 constant). Frame 0x30:
  +0x10..+0x1c : CCharString temps (all 6), box1 {TLX,TLY,BRX,BRY}, filter2 (index stored at +0x10 by 0x00972B83), and the
                 CIntelligentPointer temp (vptr 0x01238C6C at +0x10, PData +0x14) -- all share the same words
  +0x20        : TC key 0x7f, hits fallback, filter1 index (0x00972ABC), shield out ptr (=0 at 0x00972B5B), TC key 0xad / boss fallback
  +0x24..+0x2c : vector<CThing*> {start,finish,eos} zeroed at 0x00972A8C..94 (after first ctor push, before the "MARKER_SPOT_LIGHT" CCharString ctor)
  +0x30..+0x3c : box2 own words (TLX +0x30, TLY +0x34, BRX +0x38, BRY +0x3c)
- Box schedule both times: ftol X -> ebp, ftol Y -> eax; lea ecx,[ebp-50] (TLX) ; lea edx,[eax-50] (TLY) ; add eax,50 (BRY) ;
  add ebp,50 (BRX last). LESSONS.md line 5: member-by-member function-scope rectangle gives `lea [r-x]; add` and own words;
  block-scoped/ctor shares the next slot. Here box1 SHARES the string word (+0x10) and box2 has own words -> the two blocks are spelled differently.
- Loop: size=(finish-start)>>2 in esi; `cmp esi,0; jbe end` TWICE (0x00972B1F and 0x00972B25), xor ebp; body tests [thing+0x91]&1 (IsDead); Kill(true); inc; jb.
- Interface tests: 0x7f -> [thing+0x2c] sign bit (bitset word 3, bit 31: `test eax,eax ; jns`); 0xad -> [thing+0x34] `test ch,0x20` (word 5 bit 13).
  0x7f path: `lea esi,[ebx+0x44] ; mov ecx,esi ; ... ; mov esi,[esi+4]` = GETTC_EXPANDED map-pointer form exactly.
- Order: creature loaded, HasTC/lookup 0x7f, GetWorld() -> [eax+0x4c] CombatManager into ebp, push 1, PTargetThing.GetPItem, push creature, RegisterCombatant.
- End: SpeechCounter(+0x30)=0 written BEFORE vector dtor (0x00972C9B).

## 2. Name / signature (two sources)
- vtable CAIStateGroup_JackOfBladesStage1 0x0128D6B4 slot 2 = 0x00972990 (rebuild/manifest/rtti-vtables.tsv); CAIStateGroupBase.h line 53: slot 2 = OnActivate.
- FableWin.exe.signatures.tsv / egor_pdb_names: ?OnActivate@CAIStateGroup_JackOfBladesStage1@@UAEXXZ (public virtual void()).
  Manifest label CORRECT. Leaf: `?OnActivate@CAIStateGroup_JackOfBladesStage1@@UAEXXZ`. xref: only ref is the vtable (.rdata 0x0128D6BC).
- Stage1 header (engine/CAIStateGroup_JackOfBladesStage1.h) does NOT yet declare OnActivate -- needs an opt-in declaration
  (siblings Base 0x009720C0 / Stage2 0x00972490 / Stage3 0x00972670 OnActivate are all slot 2).

## 3. Callees (address, landed file, landed/PDB name; DRAFT-MISMATCH flagged)
| addr | landed row | PDB/landed name | draft spelling |
|---|---|---|---|
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound (non-const) | CInterfaceMap::LowerBound (local) MISMATCH |
| 0x008FCFC0 | 00/8f/CAIStateGroupBase_GetWorld_008fcfc0.cpp | ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ (CWorld& const) | GetPWorld() returning CWorld* MISMATCH |
| CWorld+0x4c | inline | ?GetCombatManager@CWorld@@QAEAAVCCombatManager@@XZ, member PThingCombatManager_ptr (CWorld.h:236,323; opt-in FABLE_RETAIL_CWORLD_COMBATMANAGER) | `CombatManager` member MISMATCH |
| 0x006EDC50 | 00/6e/CCombatManager_RegisterCombatant_006edc50.cpp | ?RegisterCombatant@CCombatManager@@QAEXAAVCThingCreatureBase@@0_N@Z (refs!) | pointers MISMATCH |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ | ok (use CIntelligentPointer<T>::Get()) |
| 0x0099EBF0 / 0x0099EAE0 | 00/99/CCharString_Constructor_0099ebf0.cpp / CCharString_CCharString_0099eae0.cpp | ??0CCharString@@QAE@PBDJ@Z / dtor | ok (implicit temp from literal) |
| 0x00815820 | 00/81/CTCHitLocations_GetPHitLocationFromName_00815820.cpp | CTCHitLocations::GetPHitLocationFromName(const CCharString&) -> NHitLocations::CHitLocation* (CTCHitLocations.h:66, opt-in FABLE_RETAIL_CTCHITLOCATIONS_FROMNAME) | GetHitLocation MISMATCH |
| 0x00816390 | 00/81/CHitLocation_SetEnabled_00816390.cpp | ?SetEnabled@CHitLocation@NHitLocations@@QAEX_N@Z (namespace NHitLocations) | global CHitLocation MISMATCH (namespace) |
| 0x0044C6B0 | 00/44/CGameDefinitionManager_Get_0044c6b0.cpp | ?Get@CGameDefinitionManager@@SIAAV1@XZ (static __fastcall, returns CGameDefinitionManager&) | returns CDefinitionManager* MISMATCH |
| 0x009AD410 | 00/9a/CDefinitionManager_GetDefGlobalIndexFromName_009ad410.cpp | ?GetDefGlobalIndexFromName@CDefinitionManager@@QBEJABVCCharString@@@Z (const, long) | non-const MISMATCH |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | ?GetPos@CThing@@QBEABVC3DVector@@XZ | GetPosition MISMATCH |
| 0x00BFEA70 | 00/bf/global_ftol2_00bfea70.cpp | __ftol2 (toolchain) | (long) casts ok |
| 0x008FD000 | 00/8f/CAIStateGroupBase_PeekThingSearchTools_008fd000.cpp | ?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ (const&, opt-in FABLE_RETAIL_CAISTATEGROUPBASE_PEEK) | non-const pointer MISMATCH |
| 0x0096BCB0 | 00/96/CThingSearchTools_GetAllThingsInRoughArea_0096bcb0.cpp | ??$GetAllThingsInRoughArea@VCThing@@V?$vector@PAVCThing@@...@VCThingFilter_HasDefGlobalIndex@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_HasDefGlobalIndex@@AAV?$vector@...@Z (const, template <T,Container,Filter>, vector&) | CRoughArea / CThingList* MISMATCH |
| 0x007FCEF0 | 00/7f/CThingSearchTools_GetPFirstThingInRoughArea_007fcef0.cpp | template GetPFirstThingInRoughArea<CThingFilter_HasDefGlobalIndex,CThing>(const C2DBoxI&, const F&, T**) | CRoughArea MISMATCH |
| 0x004C9B80 | 00/4c/CThing_Kill_004c9b80.cpp | ?Kill@CThing@@QAEX_N@Z | ok |
| 0x00A01B10/0x00A01B90/0x00A01C10 | 00/a0/CBaseIntelligentPointer_*.cpp | ctor / ?SetPItem@..@@QAEXPAVCBaseObject@@@Z / ??1CBaseIntelligentPointer@@UAE@XZ | ok |
| 0x00BFEA14 | 00/bf/global_free_00bfea14.cpp | _free (toolchain) via STLport malloc allocator (stlport/fable_containers.h:336 deallocate -> free) | hand CThingList MISMATCH: use std::vector<CThing*> |
- NOT a call: CTCBoss::SetPhaseStartPos(const C3DVector&) (?SetPhaseStartPos@CTCBoss@@QAEXABVC3DVector@@@Z, FableWin) is the inline
  3-dword copy at 0x00972C84..94. Draft wrote a made-up member `ArenaCentrePos`: WRONG name.

## 4. Members
- this+0x04 CAIStateGroupBase::PBrain (CAIBrain*); [PBrain+0x18] creature = CAIBrain GetCreature (opt-ins FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE + FABLE_RETAIL_CAIBRAIN_GETCREATURE -> CThingAICreature&).
- this+0x1c PArenaCentre CIntelligentPointer<CThing>, +0x24 PTargetThing CIntelligentPointer<CThingCreatureBase>, +0x30 SpeechCounter long:
  engine/CAIStateGroup_JackOfBladesBase.h under FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE (Ego_r names).
- this+0x5c PShieldThing CIntelligentPointer<CThing>: engine/CAIStateGroup_JackOfBladesStage1.h (protected; base size 0x4c, HasAttacked +0x4c, ZigZagCounter +0x50, PObstruction +0x54).
- CThing+0x20 UsingComponents bitset<274>, +0x44 Components CVectorMap, +0x91 Dead:1 (IsDead()) -- engine/CThing.h with FABLE_RETAIL_CTHING + FABLE_RETAIL_CTHING_GETTC_EXPANDED (GetTC(id,&p) exact map-pointer shape).
- CWorld+0x4c PThingCombatManager_ptr (GetCombatManager()).
- CTCBoss+0x22 PhaseStartPos C3DVector (struct_layouts_egor.tsv: CTCBoss 34 C3DVector PhaseStartPos; private) -- CTCBoss.h:52 needs
  FABLE_RETAIL_C3DVECTOR_PACKED (C3DVector.h:73, owner-approved alignment 1); SetPhaseStartPos is NOT declared in CTCBoss.h yet (needs an opt-in inline `{PhaseStartPos=pos;}`).
- CTCBoss interface id 0xad = StatGetInterfaceType 173 (CTCBoss.h); CTCHitLocations id 0x7f.
- CThingFilter_HasDefGlobalIndex: engine/CThingFilter_HasDefGlobalIndex.h (unsigned long DefGlobalIndex, inline ctor). C2DBoxI: engine/C2DBoxI.h (TLX,TLY,BRX,BRY; ctors (tl,tl,br,br), (C2DCoordI,radius), FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS).
- Classes the draft models LOCALLY that have shared headers: C3DVector, CCharString, CThing (+CThingCreatureBase), CVectorMap (CInterfaceMap),
  CTCHitLocations, NHitLocations_CHitLocation, CTCBoss, CBaseIntelligentPointer/CIntelligentPointer, CThingSearchTools, C2DBoxI (CRoughArea),
  CThingFilter_HasDefGlobalIndex, CDefinitionManager, CGameDefinitionManager, CCombatManager, CWorld, CAIBrain, CAIStateGroupBase,
  CAIStateGroup_JackOfBladesBase/Stage1 (it even flattens the base into Stage1), IS_THING_ALIVE (engine/ThingAlive.h FABLE_RETAIL_IS_THING_ALIVE).

## 5. Callee visibility
- Same TU (Stage1 source, neighbours): 0x00971ED0 GetRushToPosForLunge, 0x00972280 IsRunnable, 0x00972340 HasHitsCausedRush, 0x00972CC0
  MeleeCombat (landed), 0x00974770 PowerUpSword, 0x00975BB0 Update. None called here.
- GetAllThingsInRoughArea<CThing,vector,HasDefGlobalIndex> 0x0096BCB0 and GetPFirstThingInRoughArea 0x007FCEF0 are template COMDATs -> body
  likely VISIBLE to retail's compiler (Guildmaster FindAndStoreBodyguards 0x00977B10 notes: "the search is a template instantiation in retail's
  TU, so VC7.1 sees that the vector's address does not escape" and uses a noinline same-TU stand-in). Opt-ins: FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY,
  _FIRSTTHING_BODY (CThingSearchTools.h:84,108).
- CVectorMap::LowerBound 0x0040F020 out of line (GETTC_EXPANDED). GetWorld / PeekThingSearchTools / GetPos / GetPItem / SetPItem out of line.
- Inline in retail: CIntelligentPointer<CThing>(T*) ctor (base ctor call + vptr 0x01238C6C store + SetPItem), operator= (GetPItem then SetPItem; FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN),
  GetCombatManager, SetPhaseStartPos, GetCreature, vector size/dtor, IsDead, IS_THING_ALIVE (first PArenaCentre GetPItem, then GetPItem again for the 0x91 test, then a third GetPItem for GetPos).

## 6. Sibling landed rows to copy idioms from
- rebuild/src/compiled/00/97/CAIStateGroupJackOfBladesStage1_MeleeCombat_00972cc0.cpp (same TU, shared headers): defines FABLE_RETAIL_JACK_STAGE1_UPDATE,
  _JACK_STAGE1_HASHITSCAUSEDRUSH, _CAISTATEGROUPBASE, _MEMBERS, _GOTOBED_WORLD, _GETCREATURE, _CAIBRAIN_GETCREATURE, _CAISTATEGROUP_JACKOFBLADESBASE,
  _CINTELLIGENTPOINTER, _CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN, _COMPARE_NULL_INT, _DEREFERENCE, _CTHING, _CTHINGPHYSICAL, _CTHINGGAMEOBJECT,
  _CTHINGCREATUREBASE, _CTHINGAICREATURE, _CWORLD, _CWORLD_COMBATMANAGER, _CCOMBATMANAGER, _IS_THING_ALIVE, _C3DVECTOR. No pragmas.
- 00/97/CAIStateGroupGuildmaster_FindAndStoreBodyguards_00977b10.cpp: closest shape (def-index lookup by name, filter, C2DCoordI centre, C2DBoxI
  +-55, vector<...> + free, boss TC 0xad lookup sharing the filter/string word); FABLE_RETAIL_CTHING_GETTC_EXPANDED, CCHARSTRING; noinline stand-in for the search.
- 00/97/CAIStateGroupJackOfBladesStage1_IsRunnable_00972280.cpp (FABLE_RETAIL_CTHING_GETTC_EXPANDED, FABLE_RETAIL_CINTELLIGENTPOINTER).
- 00/97/CAIStateGroupJackOfBladesStage2_OnActivate_00972490.cpp, Stage3_OnActivate_00972670.cpp, Base_OnActivate_009720C0.cpp (older local-model rows).
- 00/8e/NCreatureModeCModeGuildmasterPowerUpShield_OnActivate_008e0060.cpp (HasDefGlobalIndex + intelligent pointer; uses #pragma auto_inline(off/on) pairs).

## 7. Old draft model errors (pending/2026-10-22_linux_hand278: DIFFER 792v803, "frame-slot packing")
- Code shape was aligned; the 11-byte gap is all frame packing (draft: -4 frame). Retail packs by type class (LESSONS.md:20); retail facts above
  (+0x10 shared string/box1/filter2/IP temp; +0x20 shared key/filter1/shield/hits; box2 own words +0x30).
- Wrong names (must fix anyway): GetPWorld->GetWorld, GetHitLocation->GetPHitLocationFromName, CHitLocation->NHitLocations::CHitLocation,
  GetPosition->GetPos, ArenaCentrePos->PhaseStartPos via SetPhaseStartPos, CRoughArea->C2DBoxI, CThingList->std::vector<CThing*>,
  CGameDefinitionManager::Get returns CGameDefinitionManager& (not CDefinitionManager*), comment "free" ok.
- Draft's box1 and box2 are both block-scoped ctor temporaries; retail gives them different slots -> try box1 as block temp (shares +0x10) and box2 member-by-member/own scope.
- The draft's `if (PeekThingSearchTools()->GetPFirstThingInRoughArea(...)) PShieldThing = CIntelligentPointer(shield);` matches retail's
  ctor+SetPItem+GetPItem+SetPItem+dtor sequence (0x00972BDF..0x00972C14); keep it but via shared CIntelligentPointer<CThing>(T*) ctor + ASSIGN opt-in.
- Draft keeps `shield=0` init inside block -- retail stores 0 at +0x20 at 0x00972B5B before the CCharString ctor (i.e. declared before the name temp).
- Loop: retail double `cmp esi,0 ; jbe` -- draft for-loop over hoisted count was already aligned per NOTES.

## 0x006C8E90

# DOSSIER 0x006C8E90 CThingFilter_IsEligibleWitness::operator() (804 bytes)

## 1. Retail shape (xdis 006c8e90 0x334)
- thiscall, `ret 4`, NO ebp frame (`push ecx; push ebp; ...` -- the `push ecx` is a 4-byte spill slot holding `this`),
  no SEH. ebp = thing (arg loaded `mov ebp,[esp+0xc]`), edi = this, also spilled to [esp+8]/[esp+0x10]; reloaded into ebx
  in the unknown-threat block because edi is reused there for the navigation TC. Default /O2 (siblings have no pragmas/Flags).
- Every LowerBound key is homed in the dead `thing` ARG slot ([esp+0x14] before `push ebx`, [esp+0x18] after) => thing arg is
  a reference held in ebp; the key local of the GETTC_EXPANDED template lands there.
- Early `thing.Type != 1` exit is placed at the END (0x6c91ac: pop edi; xor al,al; pop ebp; pop ecx; ret 4) -- before `push esi`.
- Bit tests in place: 0x31 = `test [ebp+24h],20000h`; 0x49 = `mov eax,[ebp+28h]` hoisted ABOVE the ReverseFactionSearch branch,
  then `test ah,2` in both arms; 0x34 = `test [ebp+24h],100000h`; 7 = `mov al,[ebp+20h]; test al,al; jns`.
- Control flow in order:
  1. `thing.GetType()==1` (CClass::Type byte +0x10) else false.
  2. if HasTC(0x31): GetTC -> CTCCreatureModeManager::GetCurrentMode() (0x73a6e0, returns CModeBase&) -> vslot 1 GetType() == 0xD -> false.
     (no else-load: HasTC/GetTC pair, not stale PeekTC)
  3. HasOpinions(thing) [ecx=this+8, 0x6a80a0], DeedTypeEnabled(thing) [this+0xc, 0x6c8d40]: false -> return false.
  4. if UseIsNot(+0x27): IsNotRecipient(thing) [this+0x18, 0x6a76a0] false -> return false.
  5. result = true (`mov al,1`); if UseFaction(+0x28): ReverseFactionSearch(+0x29) ? IsEnemy(thing) (INLINED: TC 0x49 ->
     CTCEnemy::IsEnemyOfFaction 0x76c7d0 with [this+0x20]) : IsAllied(thing) (INLINED: 0x76c7f0 with [this+0x1c]); no TC -> false.
     `if(!result) return false; if(!UsePerceiving(+0x26)) return result(=true)`.
  6. perceiving = UseHearing(+0x25) ? NCreatureAI::IsAwareOfThingIndirectly(thing, Perpetrator, false) (fastcall: ecx=thing as
     creature, edx=[this+0]) : IsPerceivingPerpetrator(thing) INLINED: PeekTC(0x34) with UNINITIALISED out pointer (no-TC path
     `mov esi,[esp+18h]` = stale arg slot), `xor bl,bl` then GetPerceivedThing()==[this+0x10] && ((IncludeHearing[this+0x14] &&
     Heard +0x32) || LineOfSight +0x2d || Smelt +0x31).
  7. sees = perceiving && HasTC(0x34) && GetTC(0x34)->GetPerceivedThing()==Perpetrator && IgnoreFOVLineOfSight(+0x2f) -> al.
  8. if !UnknownThreatSearch(+0x24) return sees.  else `sete al; test al` (bool !sees materialised); if sees -> false.
  9. if PDeed && IS_THING_ALIVE(PDeed->PTarget) [ecx=PDeed+0x18] && IsAwareOfThingIndirectly(thing, PDeed->PTarget.GetPItem(), false)
     -> return true (PDeed reloaded from [edi+4] between the calls; `push 0` before GetPItem).
  10. if PDeed && HasTC(0x34): perceive=GetTC(0x34) (ebx; end read via ecx here, not esi); if HasTC(7) (`jns`) : nav=GetTC(7) (edi);
      r = perceive->GetSightRadius() (fstp [esp+18h] -- spilled to arg slot); this reloaded; `deedPos = &PDeed->Position`
      computed BEFORE `thing.GetPos()`; dx=pos.X-deed.X, dy=pos.Y-deed.Y; `dy*dy + dx*dx < r*r` (fld st0;fmul st1 first = dy*dy);
      then nav->IsPosNavLineOfSightToPos(thing.GetPos(), PDeed->Position) (deed pos pushed first = 2nd arg) -> true.
      Fail -> false. Shares the `return true` epilogue at 0x6c90c5.

## 2. Identity
- Manifest label CThingFilter_IsEligibleWitness::operator() is CORRECT. Mangled leaf:
  `??RCThingFilter_IsEligibleWitness@@QBE_NABVCThing@@@Z` (Ego_r egor_pdb_names.tsv 0x006ff2a0; FableWin _fablewin_api.tsv
  0x017f5a58/0x01f27da0; FableWin signatures.tsv). => `bool operator()(const CThing& thing) const` -- the draft's `CThing*`
  parameter is WRONG (reference, const).
- Callers: 0x6c64bf (CTCHeroOpinionDeedLog::InformRelevantThingsOfDeed 0x006C6040) and 0x6c92ce (landed
  rebuild/src/compiled/00/6c/CThingSearchTools_GetAllThings_006c9280.cpp, which already names it `operator()(const CThing&) const`).
- Layout (struct_layouts_egor.tsv, 44 bytes): Perpetrator CThing* +0, PDeed COpinionDeed* +4, HasOpinions CThingFilter_HasTC +8,
  DeedTypeEnabled CThingFilter_IsDeedTypeEnabled +0xc, IsPerceivingPerpetrator CThingFilter_IsPerceivingThingAndIsThingPerceived
  +0x10 {CThing* PerceivedThing +0, bool IncludeHearing +4}, IsNotRecipient CThingFilter_IsNot +0x18 {CThing* Thing},
  IsAllied CThingFilter_IsAlliedWithFaction +0x1c {CFaction* PFaction}, IsEnemy CThingFilter_IsEnemyOfFaction +0x20 {CFaction* PFaction},
  bools UnknownThreatSearch +24 UseHearing +25 UsePerceiving +26 UseIsNot +27 UseFaction +28 ReverseFactionSearch +29.
  No shared header for CThingFilter_IsEligibleWitness (local model OK; copy from landed ctor 0x006C8DE0). Draft's sub-filter
  member names are right except CThingFilter_IsDeedTypeEnabled::DeedType type is EOpinionDeedType and IsNot::Thing is CThing*.

## 3. Callees (retail addr -> landed file -> landed/PDB name; DRAFT MISMATCH flagged)
- 0x004365B0 00/43/CVectorMap_LowerBound_004365b0.cpp: CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound const. Draft used a
  local CInterfaceMap -> use engine/CVectorMap.h via CThing.h GETTC_EXPANDED.
- 0x0073A6E0 00/73/CTCCreatureModeManager_GetCurrentMode_0073a6e0.cpp: `?GetCurrentMode@CTCCreatureModeManager@@QBEAAVCModeBase@1@XZ`
  -- const, RETURNS CModeBase& (nested CTCCreatureModeManager::CModeBase). DRAFT: non-const, returns pointer, local CMode. vslot 1 =
  `virtual unsigned long GetType() const` (CTCCreatureModeManager.h:16). TC 0x31 = TCI_ENTITY_MODE_MANAGER (ETCInterfaceType.h:54;
  CTCCreatureModeManager::StatGetInterfaceType).
- 0x006A80A0 00/6a/CThingFilterHasTC_operator_006a80a0.cpp `??RCThingFilter_HasTC@@QBE_NABVCThing@@@Z` (out of line). Header
  engine/CThingFilter_HasTC.h (opt-ins FABLE_RETAIL_CTHINGFILTER_HASTC_EXTERNAL / _OUTOFLINE).
- 0x006C8D40 00/6c/CThingFilterIsDeedTypeEnabled_operator_006c8d40.cpp `??RCThingFilter_IsDeedTypeEnabled@@QBE_NABVCThing@@@Z` (out of line).
- 0x006A76A0 00/6a/CThingFilterIsNot_operator_006a76a0.cpp `??RCThingFilter_IsNot@@QBE_NABVCThing@@@Z`; header engine/CThingFilter_IsNot.h.
- 0x0076C7D0 00/76/CTCEnemy_IsEnemyOfFaction_0076c7d0.cpp `?IsEnemyOfFaction@CTCEnemy@@QBE_NPBVCFaction@@@Z` (const, const CFaction*).
- 0x0076C7F0 00/76/CTCEnemy_IsAlliedWithFaction_0076c7f0.cpp IsAlliedWithFaction(const CFaction*) const. Both declared in
  engine/CTCEnemy.h:47-48 (opt-ins FABLE_RETAIL_CTCENEMY_FACTION / _QUERIES; check which branch holds lines 47-48). TC 0x49 = TCI_ENEMY.
- 0x00864700 00/86/NCreatureAI_IsAwareOfThingIndirectly_00864700.cpp: `?IsAwareOfThingIndirectly@NCreatureAI@@YI_NPBVCThingAICreature@@PBVCThing@@_N@Z`
  (__fastcall, namespace NCreatureAI, 3rd param `check_line_of_sight`). DRAFT NAME WRONG: "IsAwareOfThingImplementation" is the
  propagated catalog label of 0x00864250. No NCreatureAI.h declaration exists -> declare `namespace NCreatureAI { bool __fastcall
  IsAwareOfThingIndirectly(const CThingAICreature*, const CThing*, bool); }` locally (as the landed row does). Thing -> creature
  cast: need `(const CThingAICreature*)&thing` (ecx=ebp unchanged, so no base adjustment).
- 0x007446A0 00/74/CTCPerceiveThing_GetPerceivedThing_007446a0.cpp `?GetPerceivedThing@CTCPerceiveThing@@QBEPBVCThing@@XZ` (const,
  returns const CThing*) -- out of line in retail; the default CTCPerceiveThing.h branch has it INLINE (line 60), the
  FABLE_RETAIL_CTCPERCEIVETHING branch declares it out of line (line 189). DRAFT: non-const.
- 0x007448F0 00/74/CTCPerceiveThing_GetSightRadius_007448f0.cpp: landed `?GetSightRadius@CTCPerceiveThing@@QBEMXZ` (reads
  SightRadius +0x40; catalogued as GetExtendedSightRadius). DRAFT NAME WRONG (GetExtendedSightRadius). Const. The sibling
  0x00864700 calls the same address as GetSightRadius. Header: FABLE_RETAIL_CTCPERCEIVETHING branch lacks GetSightRadius
  (it has GetExtendedSightRadius under FABLE_RETAIL_CTCTARGETINGAI_ATTACK) -- see how row 0x864700 declares it (it includes
  CTCPerceiveThing via the CThing route; may need a declaration or new opt-in).
- 0x0069CE20 00/69/global_ISTHINGALIVE_0069ce20.cpp `??$IS_THING_ALIVE@V?$CIntelligentPointer@$$CBVCThing@@@@@@YI_NABV?$CIntelligentPointer@$$CBVCThing@@@@@Z`
  (template, __fastcall, const CIntelligentPointer<const CThing>&). DRAFT: non-template fn over CBaseIntelligentPointer.
- 0x00A01B50 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ`
  (returns CBaseObject*); engine/CIntelligentPointer.h:27 declares it, `Get()` inline (line 69) = static_cast<T*>(GetPItem()).
  Retail calls GetPItem directly then passes eax -- `PDeed->PTarget.Get()` should inline to that.
- 0x004C73D0 00/4c/CThing_GetPosition_004c73d0.cpp: `?GetPos@CThing@@QBEABVC3DVector@@XZ`. DRAFT NAME WRONG (GetPosition).
  CThing.h:411 declares `const C3DVector& GetPos() const`.
- 0x006CB610 00/6c/CTCCreatureNavigation_IsPosNavLineOfSightToPos_006cb610.cpp `?IsPosNavLineOfSightToPos@CTCCreatureNavigation@@QBE_NABVC3DVector@@0@Z`
  (const, two const C3DVector&). DRAFT NAME/SIGNATURE WRONG (IsNavLineOfSight(const C3DVector*, ...)). Header CTCCreatureNavigation.h:72
  under FABLE_RETAIL_CTCCREATURENAVIGATION_LINEOFSIGHT (nested in FABLE_RETAIL_CTCCREATURENAVIGATION). TC 7 = TCI_NAVIGATION.

## 4. Members read
- thing +0x10 byte == 1: CClass::Type (engine/CClass.h:30, FABLE_RETAIL_CCLASS, which CThing.h defines itself); accessor
  `unsigned long GetType() const { return Type; }` -- landed idiom `thing.GetType() == 1` (54+ rows). Draft's `CThing::State` is a made-up name.
- thing +0x20 std::bitset<274> UsingComponents, +0x44 CVectorMap Components: engine/CThing.h (retail asserts lines 405-406).
  Opt-in `#define FABLE_RETAIL_CTHING_GETTC_EXPANDED` gives `PeekTC(ETCInterfaceType, const T**) const` / `GetTC(id, T**)` with the
  key local + map pointer + conditional end select exactly as the retail lookups here. Siblings 0x6c8c50 / 0x6c8d40 use it with
  `thing.PeekTC(TCI_ENEMY, &enemy)`. HasTC(type) default = `UsingComponents.test(type)` (line 309) -- check it emits the in-place
  word test (the HasTC filter body at 0x6a80a0 is range-checked, but these inline tests are not; draft used a raw word test).
- COpinionDeed (engine/COpinionDeed.h): Position C3DVector +0x8, PTarget CIntelligentPointer<const CThing> +0x18.
- CTCPerceiveThing (struct_layouts_egor): LineOfSight +0x2d, IgnoreFOVLineOfSight +0x2f, Smelt +0x31, Heard +0x32 (Perceiving +0x2c,
  SightRadius +0x40). PDB accessors (FableWin only => inline in release): IsThingLineOfSight, IsThingIgnoreFOVLineOfSight,
  IsThingSmelt, IsThingHeard (`?IsThingHeard@CTCPerceiveThing@@QBE_NXZ` etc.). Header CTCPerceiveThing.h: members exist under
  FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS (private) with only IsThingLineOfSight inline; default/SMELL branches declare
  IsThingSmelt/IsThingHeard/IsThingIgnoreFOVLineOfSight NON-inline (lines 31-35). GAP: no opt-in yet with all four inline +
  out-of-line GetPerceivedThing + GetSightRadius; author may need a new nested opt-in (e.g. FABLE_RETAIL_CTCPERCEIVETHING_THINGFLAGS).
- Local models in the draft that have shared headers: CThing (engine/CThing.h), CInterfaceMap (engine/CVectorMap.h), C3DVector
  (engine/C3DVector.h), CBaseIntelligentPointer (engine/CIntelligentPointer.h), COpinionDeed (engine/COpinionDeed.h), CMode/
  CTCCreatureModeManager (engine/CTCCreatureModeManager.h), CTCEnemy (engine/CTCEnemy.h), CTCPerceiveThing
  (engine/CTCPerceiveThing.h), CTCCreatureNavigation (engine/CTCCreatureNavigation.h), CThingFilter_HasTC
  (engine/CThingFilter_HasTC.h), CThingFilter_IsNot (engine/CThingFilter_IsNot.h).

## 5. Callee visibility
- Same TU (retail 0x6c5xxx-0x6c93xx = CTCHeroOpinionDeedLog / opinion-deed source): CThingFilter_IsAlliedWithFaction::operator()
  is out of line at 0x006C8C50 (landed) yet INLINED here; IsEnemyOfFaction has no retail copy (FableWin-only symbol) and is inlined;
  IsPerceivingThingAndIsThingPerceived::operator() FableWin-only -> inline. So define all three inline in the class (plain inline;
  retail inlined them into this 804-byte body -- if VC7.1 calls them out of line, try __forceinline as the draft did).
- IsDeedTypeEnabled (0x6c8d40), HasTC filter (0x6a80a0), IsNot (0x6a76a0) are called out of line: declare only.
- GetPerceivedThing / GetSightRadius / GetPos / IsPosNavLineOfSightToPos / GetCurrentMode / GetPItem / IS_THING_ALIVE: declare only (out of line).
- No STLport map/tree helpers involved; LowerBound const is the only container callee.

## 6. Siblings (all default flags, no `// Flags:` or #pragma)
- 00/6c/CThingFilterIsAlliedWithFaction_operator_006c8c50.cpp: `#define FABLE_RETAIL_CTHING_GETTC_EXPANDED`, engine/CThing.h,
  engine/CVectorMap.h, `const CTCEnemy* enemy; if (thing.PeekTC(TCI_ENEMY, &enemy)) return enemy->IsAlliedWithFaction(m_faction);`.
- 00/6c/CThingFilterIsDeedTypeEnabled_operator_006c8d40.cpp: same opt-in; uninitialised `PeekTC` out pointer reproduces the
  stale arg-slot load (exactly like step 6 here).
- 00/6c/CThingFilterIsEligibleWitness_CThingFilterIsEligibleWitness_006c8de0.cpp: the class layout + sub-filter ctors (local CThing model).
- 00/86/NCreatureAI_IsAwareOfThingIndirectly_00864700.cpp: `#define FABLE_RETAIL_CTHING` + CThing.h, the same sight-radius /
  GetPos / IsPosNavLineOfSightToPos idiom and spellings.
- 00/6c/CThingSearchTools_GetAllThings_006c9280.cpp: the caller.

## 7. Old draft model errors
- Signature `operator()(CThing* thing) const` -> must be `(const CThing& thing) const` (mangled ABVCThing). Likely the cause of the
  ebp/edi swap: with const-ref everything is const and the accessors chosen differ.
- Wrong callee names: IsAwareOfThingImplementation -> NCreatureAI::IsAwareOfThingIndirectly; GetExtendedSightRadius -> GetSightRadius;
  GetPosition -> GetPos; IsNavLineOfSight(ptr,ref) -> IsPosNavLineOfSightToPos(const&, const&) const; GetCurrentMode returns
  CModeBase& (const); GetPerceivedThing const returning const CThing*; IS_THING_ALIVE is the template over CIntelligentPointer<const CThing>.
- `State` -> CClass::Type via GetType().
- Direct member reads of Heard/LineOfSight/Smelt/IgnoreFOVLineOfSight should be the inline IsThing* accessors.
- Local CThing/CInterfaceMap/etc models: use shared headers + FABLE_RETAIL_CTHING_GETTC_EXPANDED (the draft's GetInterface==PeekTC
  shape is already correct; GetTC for the HasTC/GetTC pairs in steps 2, 7, 10).
- Control flow is otherwise right: early type exit, faction ternary (enemy when reversed), perceiving ternary on UseHearing, `!sees`
  materialised bool, unknown-threat block with PDeed null-tests repeated, `dy*dy + dx*dx < r*r`, deed-position address taken before GetPos.
- Recorded residue (hand307 STATUS): DIFFER 796v804, ebp/edi swapped + 8-byte frame vs retail `push ecx` 4-byte spill.

## 0x0049BE90

# DOSSIER 0x0049BE90 -- CParticleEnvironmentInteraction::IsParticleCollidingAt (808 bytes)

## 1. Retail shape (xdis 0049be90 0x338)
- __thiscall, `ret 0x18` (6 dword args), returns bool in al. No frame pointer (`sub esp,0x90` first, no push ebp/mov ebp,esp),
  no SEH (no fs:0) -> plain /O2 /Oy, no /Oy- pragma, no optimize("s").
- Prologue: sub esp,0x90; push ebx; mov ebx,ecx (this); `cmp [ebx+4],0` BEFORE pushing esi/edi; this spilled to [esp+0x30]
  (=[esp+0x34] after the late `push ebp`) and reloaded at 0x49C059 (`mov ebx,[esp+0x34]`) after the mesh block.
  push ebp happens only on the PWorld!=0 path (shrink-wrapped); the PWorld==0 path pops edi/esi/ebx only.
- Register roles retail: esi=&pos (arg1), edi=&hitPos (arg4) for the whole world path, ebp=&hitNormal (arg5), ebx=this then the
  vector iterator. Frame: end vector [esp+0x38..0x40], things vector [esp+0x28..0x30], nearest [esp+0x14]=FLT_MAX(0x7F7FFFFF),
  found byte [esp+0x13], per-thing hit byte [esp+0x12] == the SAME byte as the filter temporary (`lea ecx,[esp+0x16]` after one
  push = esp+0x12), CMeshIntersectionData at [esp+0x44] (dist at +0x5c), CPhysicsMeshObject temp at [esp+0x60].
- end = pos + velocity: X is `fld pos.X; fadd vel.X`, Y/Z are `fld vel.Y; fadd pos.Y` (operand order flips after X) -- check
  C3DVector.h FABLE_RETAIL_C3DVECTOR_ADD_XY_GROUP vs plain operator+ (draft's plain one already gave 806v808, so likely fine).
- Ground path: both GetGroundSizeZAt calls happen before the compare (first result fstp [esp+0x14], second stays in st0);
  `groundZ >= pos.Z` branch discards st0 (fstp st0). Second branch writes (end.X,end.Y,groundZ) -- groundZ of the START (retail bug,
  the draft keeps it). hitGround=1 stored before hitPos. Normal: 3rd PeekWorldMap call, GetGroundFaceNormalAt(hitPos) returns by
  value into a temp [esp+0x18] then copied into *ebp.
- No-world path: `fld 0.0 (0x0122DEDC); fcomp pos.Z; test ah,41h` -> taken when 0.0f > pos.Z; hitPos.Z is stored from edx
  (the integer 0 of the PWorld null test); hitNormal (0,0,1) built through stack temps [esp+0x14]/[esp+0x18].

## 2. Name / class / signature (manifest label CORRECT)
- Ego_r ghidra_out/egor_pdb_names.tsv 004a6240 and FableWin ghidra_out/_fablewin_api.tsv 01853b2b / 01d2b5e0 (also
  naming_stragglers/FableWin.exe.signatures.tsv): `?IsParticleCollidingAt@CParticleEnvironmentInteraction@@UBE_NABVC3DVector@@0_NAAV2@2AA_N@Z`
- Third source: rebuild/manifest/rtti-vtables.tsv: CParticleEnvironmentInteraction vtable 0x0122ED8C slot 1 = 0x0049BE90 (only xref =
  the vtable word at 0x0122ED90). Base: NParticleEngine::CAEnvironmentInteraction (rebuild/include/engine/CAEnvironmentInteraction.h
  declares slot 1 `virtual bool IsParticleCollidingAt(const C3DVector&,const C3DVector&,const bool,C3DVector&,C3DVector&,bool&)const=0`).
- => `virtual bool IsParticleCollidingAt(const C3DVector& pos, const C3DVector& velocity, const bool checkMeshes, C3DVector& hitPos,
  C3DVector& hitNormal, bool& hitGround) const;` public virtual CONST (UBE). Draft declares it non-const and non-virtual: wrong mangling
  (leaf must be the UBE form). Class is global (not in NParticleEngine); base is NParticleEngine::CAEnvironmentInteraction.
- Param names are descriptive (not in PDB name sets).

## 3. Callees (landed file / landed name / PDB name)
| addr | landed row | PDB spelling | draft problem |
|---|---|---|---|
| 0x00BFEA70 __ftol2 (x2) | CRT | - | ok ((long) casts) |
| 0x0049C490 | NOT landed (functions.tsv label `GetAllThingsInRoughArea<CThing const,std::vector<CThing const *>,CThingFilter_HasPhysicsMesh>`, ret 0Ch; body calls 0x006A6F50 = landed CThingFilter_HasPhysicsMesh::operator() rebuild/src/compiled/00/6a/CThingFilterHasPhysicsMesh_operator_006a6f50.cpp) | Ego_r `??$GetAllThingsInRoughArea@$$CBVCThing@@V?$vector@PBVCThing@@...@VCThingFilter_HasPhysicsMesh@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_HasPhysicsMesh@@AAV?$vector@PBVCThing@@...@Z` (const member template) | DRAFT WRONG: `CThingFilter_All` and vector<CThing*>; real = filter CThingFilter_HasPhysicsMesh, container std::vector<const CThing*>, T = const CThing, method const |
| 0x006B0850 | 00/6b/CTCPhysicsBase_GetPhysicsMeshObject_006b0850.cpp | ?GetPhysicsMeshObject@CTCPhysicsBase@@QBE?AVCPhysicsMeshObject@@XZ | ok |
| 0x00A12450 | not landed | ?GetPointIntersectionFromTo@CPhysicsMeshObject@@QBE_NABVC3DVector@@0AAVCMeshIntersectionData@@@Z | draft uses POINTERS; PDB takes const C3DVector&, const C3DVector&, CMeshIntersectionData& (shared CPhysicsMeshObject.h line 39 has the ref form) |
| 0x00A127D0 | 00/a1/CPhysicsMeshObject_CPhysicsMeshObject_00a127d0.cpp | ??1CPhysicsMeshObject@@UAE@XZ (virtual) | draft declares non-virtual dtor (GetHeightZBelowPos row declares it virtual) |
| 0x00BFEA14 free | CRT (STLport vector dealloc) | - | ok |
| vslot 13 (+0x34) on PWorld | 00/4a/CWorld_PeekWorldMap_004ae8c0.cpp | ?PeekWorldMap@CWorld@@UBEABVCWorldMap@@XZ | DRAFT WRONG: invents CParticleEnvironment::GetWorldMap returning a pointer; real = CWorld::PeekWorldMap() const returning const CWorldMap& (CWorld.h slot 13) |
| 0x0063A300 | 00/63/CWorldMap_GetGroundSizeZAt_0063a300.cpp | ?GetGroundSizeZAt@CWorldMap@@QBEMABVC3DVector@@@Z | ok |
| 0x0063AB10 | 00/63/CWorldMap_GetGroundFaceNormalAt_0063ab10.cpp | ?GetGroundFaceNormalAt@CWorldMap@@QBE?AVC3DVector@@ABV2@@Z | ok (CWorldMap.h:189) |
- Global 0x013B86A0 = CMainGameComponent::PMainGameComponent via inline `CMainGameComponent::Get()` (?Get@CMainGameComponent@@SIPAV1@XZ);
  +0x24 via inline `GetWorld()` (?GetWorld@CMainGameComponent@@QAEAAVCWorld@@XZ, FableWin), +0x20 via inline
  `CWorld::GetThingSearchTools()` / `PeekThingSearchTools()`. Draft's `g_pMainGameComponent` / `World` / `ThingSearchTools` are wrong spellings.

## 4. Members
- this+4: Ego_r struct_layouts_egor.tsv `CParticleEnvironmentInteraction +4 CWorld* PWorld` (also WindVelocity +8, WindActive +0x14,
  CameraPos +0x15 packed; sizeof 36). DRAFT WRONG name/type: `CParticleEnvironment* PEnvironment`. No shared header
  CParticleEnvironmentInteraction.h exists; siblings model it locally under `#pragma pack(push,1)` deriving nothing (see 6).
  Better: derive from NParticleEngine::CAEnvironmentInteraction (engine/CAEnvironmentInteraction.h) and add `CWorld* PWorld`.
- CMainGameComponent +0x24 PWorld (scoped_ptr<CWorld>): engine/CMainGameComponent.h, opt-ins FABLE_RETAIL_CMAINGAMECOMPONENT +
  FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD + FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD.
- CWorld +0x20 PThingSearchTools_ptr (scoped_ptr<CThingSearchTools>): engine/CWorld.h (FABLE_RETAIL_CWORLD for the polymorphic class
  with slot 13 PeekWorldMap; GetThingSearchTools()/PeekThingSearchTools() always present).
- CThing +0x60 PhysicsTC: engine/CThing.h (GetPhysicsTC() const returns const CTCPhysicsBase*), FABLE_RETAIL_CTHING.
- CTCPhysicsBase::GetPhysicsMeshObject const: engine/CTCPhysicsBase.h:252 (FABLE_RETAIL_CTCPHYSICSBASE).
- CPhysicsMeshObject (engine/CPhysicsMeshObject.h, FABLE_RETAIL_CPHYSICSMESHOBJECT), CMeshIntersectionData (engine/CMeshIntersectionData.h:
  IntersectionPosition, Normal, DistanceSquared, size 28), C3DVector (engine/C3DVector.h, FABLE_RETAIL_C3DVECTOR, operator+ / unary
  operator- line 167, _ADD_XY_GROUP), C2DBoxI (engine/C2DBoxI.h members TLX,TLY,BRX,BRY; FABLE_RETAIL_C2DBOXI_CTORS gives
  C2DBoxI(long,long,long,long) via Initialise), CWorldMap (engine/CWorldMap.h), CThingSearchTools (engine/CThingSearchTools.h).
- Draft-local classes that HAVE shared headers: C3DVector, C2DBoxI, CMeshIntersectionData, CPhysicsMeshObject, CTCPhysicsBase, CThing,
  CThingSearchTools, CWorld, CMainGameComponent, CWorldMap, (CParticleEnvironment = CWorld). CThingPtrVector should be std::vector<const
  CThing*> from stlport/fable_containers.h. CThingFilter_HasPhysicsMesh has no header (local empty class, as row 0x00863CA0 does).

## 5. Callee-body visibility (Q32 lesson)
- 0x0049C490 (the GetAllThingsInRoughArea instance) sits right after this row: 0x49C1C0..0x49C480 are tiny COMDAT STL helpers
  (vector begin/end/size, allocator, destroy, _M_allocate 0x49C2E0, dtor 0x49C320, _M_insert_overflow 0x49C370), then 0x49C490,
  then ClipAndConvertWorldMapAreaToLocalArea 0x49C700 / GetAllThingMapsInArea 0x49C6F0. => the template was instantiated IN THIS TU,
  body visible to retail's compiler. The landed CWorldMap::GetHeightZBelowPos 0x0063E480 needed exactly this (same-TU stand-in
  under `#pragma auto_inline(off)`: "with its body visible VC knows `things` does not escape and keeps the vector's start in ebp").
  The draft's residue (iterator/flag/hitPos register allocation) is very likely this escape effect: the draft declares the callee
  extern. Opt-ins: FABLE_RETAIL_CTHINGSEARCHTOOLS + FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY (CThingSearchTools.h:211, the landed
  body shape of 0x0075BC40/0x00969FB0/0x00863E70) or _ORIGINAL_ROUGHAREA (:163); needs CTCMapwho (_SEARCHID), NThingMap_CThingMap.h
  as row 0x00863CA0 includes.
- Inline accessors retail expanded: CMainGameComponent::Get(), GetWorld(), CWorld::GetThingSearchTools(), CThing::GetPhysicsTC().
- Out of line (call): PeekWorldMap (virtual), GetPhysicsMeshObject, GetPointIntersectionFromTo, ~CPhysicsMeshObject, GetGroundSizeZAt,
  GetGroundFaceNormalAt; vector dtor inlined (test/free).

## 6. Sibling rows
- 00/49/CParticleEnvironmentInteraction_GetCameraPos_0049bdc0.cpp (slot 6) and _GetWindVelocity_0049be10.cpp (slot 3): same TU,
  local `#pragma pack(push,1)` class {vptr, CWorld* PWorld +4, WindVelocity +8, WindActive +0x14, CameraPos +0x15}; no Flags, no opt-ins;
  WindVelocity uses an inline CWorld::GetWind() accessor.
- 00/63/CWorldMap_GetHeightZBelowPos_0063e480.cpp: the same thing-search/mesh-cast idiom, same-TU callee stand-in with
  `#pragma auto_inline(off)`, CMainGameComponent::Get(), (*it)->GetPhysicsTC().
- 00/86/CThingBodyReorienter_FillBodyReorientationPossibleIntersecting_00863ca0.cpp (Flags: none): the HasPhysicsMesh filter idiom:
  local `class CThingFilter_HasPhysicsMesh { CThingFilter_HasPhysicsMesh(){} bool operator()(const CThing&) const; }`,
  `GetThingSearchTools().GetAllThingsInRoughArea<CThing>(box, filter, things)` with opt-ins FABLE_RETAIL_CTHING, _CTCPHYSICSBASE,
  _CMAINGAMECOMPONENT(+_GETWORLD,_WORLD), _CWORLD, _CTHINGSEARCHTOOLS, _CTHINGSEARCHTOOLS_ROUGHAREABODY, _CTCMAPWHO(+_SEARCHID),
  _C2DBOXI_RADIUS_INITIALIZERS; includes stlport/fable_containers.h, engine/NThingMap_CThingMap.h, engine/CTCMapwho.h.
- 00/41/NParticleEngineCAEnvironmentInteraction_GetGravityVector_00415cd0.cpp includes engine/CAEnvironmentInteraction.h (base).

## 7. Old draft model errors
- Non-const, non-virtual member; class models PEnvironment/CParticleEnvironment::GetWorldMap (pointer) instead of CWorld* PWorld +
  PeekWorldMap() const returning a reference (retail reloads [this+4] and calls slot 13 three times -- keep three calls).
- Callee template: wrong filter (CThingFilter_All -> CThingFilter_HasPhysicsMesh), wrong container (vector<CThing*> -> vector<const CThing*>),
  body invisible (likely cause of the regalloc residue, see 5). Retail does NOT pre-initialise a filter (empty class, address only),
  and the 'hit' byte shares the filter's slot: declare the filter as a temporary in the call or a local whose scope ends before the loop.
- GetPointIntersectionFromTo: pointers -> references; ~CPhysicsMeshObject should be virtual.
- Global/accessor spellings: g_pMainGameComponent->World->ThingSearchTools -> CMainGameComponent::Get()->GetWorld().GetThingSearchTools()
  (or PeekThingSearchTools() since the template is const).
- Control flow matches retail (mesh scan, found -> hitGround=false return true; else ground at pos, else at end with start groundZ, else
  false; no world -> plane Z=0); keep it.

## 0x00839100 (not landed)

# DOSSIER 0x00839100 -- CThingGameObject::GetWeaponTrailGraphicIndex (795 bytes)

## 1. Retail shape (xdis 00839100 0x32b)
- __thiscall, plain `ret` (no args), long in eax. NO frame pointer (sub esp,0xc; push ebx/ebp/esi/edi) => speed TU, no
  `optimize("y",off)`, no `optimize("s")`; no SEH frame (dtor of the def pointer is open-coded at every exit, no EH state).
- Frame 0xc: [esp+0x13] bool `special` byte; [esp+0x14] = the CDefPointer<const CWeaponDef> out slot (zeroed before the call),
  read into EDI right after the call (`mov edi,[esp+14h]`), then the SAME slot is reused for the reverse iterator `current`;
  [esp+0x18] = the GetTC key temp / iterator copy / find key (0) temp.
- Regs: ebx=this, ebp=wielder (this or carrier), edi=weapon def for the whole body.
- Exits: GetSubDef fails -> `test edi,edi; je; dec [edi+4]; jne; call [vtbl+4]` then `or eax,-1`.
  Berserk exits reload the slot (`mov ecx,[esp+14h]; test ecx,ecx`) -- null-tested release; all other exits release through
  edi WITHOUT a null test (`dec [edi+4]` / `mov eax,[edi+4]; dec eax; mov [edi+4],eax`).
- Loop alignment padding `lea esp,[esp]; lea ecx,[ecx]` before the loop head at 0x839310 (speed /O2 loop alignment).

## 2. Identity (two sources)
- Name: Ego_r.pdb 0x00813850 and FableWin.pdb 0x0184D7C6 / 0x027F0F20:
  `?GetWeaponTrailGraphicIndex@CThingGameObject@@QBEJXZ` => public, const, returns long, no params.
  Manifest label is CORRECT. Signature: `long CThingGameObject::GetWeaponTrailGraphicIndex() const;`
- Not virtual (QBE, no vtable slot). Caller: 0x0076E910 (not landed; manifest has no row file).
- engine/CThingGameObject.h does NOT declare it yet (grep) -- the row must add a declaration (or opt-in) for it.

## 3. Callees (retail addr -> landed file / landed name / PDB name)
- 0x0042B1A6 rebuild/src/compiled/00/42/CDefClassBase_GetSubDefCWeaponDef_0042b1a6.cpp
  `CDefClassBase::GetSubDef<CWeaponDef>(CDefPointer<const CWeaponDef>*) const` (`??$GetSubDef@VCWeaponDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCWeaponDef@@@@@Z`).
  DRAFT WRONG: calls it `CThingBaseDef::GetWeaponTrailDef` with a local CWeaponTrailDef -- the def is CWeaponDef.
  Called as `PBaseDef->GetSubDef(&weaponDef)` (ecx=[this+0x70]).
- 0x004365B0 rebuild/src/compiled/00/43/CVectorMap_LowerBound_004365b0.cpp -- const CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
  (via CThing::PeekTC/GetTC in engine/CThing.h under FABLE_RETAIL_CTHING_GETTC_EXPANDED). Draft's local CInterfaceMap = wrong spelling.
- 0x00705170 rebuild/src/compiled/00/70/CTCCarryable_GetPCarriedByThing_00705170.cpp `CTCCarryable::GetPCarriedByThing() const`
  (Ego_r+FableWin `?GetPCarriedByThing@CTCCarryable@@QBEPBVCThing@@XZ`); engine/CTCCarryable.h declares it. Draft: `GetCarrier`/`Carrier` -- wrong; member is PCarryingTC +0x10.
- 0x00787C18 (unaligned, 3 bytes `mov al,1; ret`) landed rebuild/src/compiled/00/78/global_operator_00787c18.cpp as
  `ReturnTrue_ret0_00787c18` (`?fn_operator@@YI_NXZ`, fastcall). Called here on the TCI_TRAP (125) component: the method is
  CTCTrapBase::IsCurrentTrapToCauseKnockdown() const (Ego_r `?IsCurrentTrapToCauseKnockdown@CTCTrapBase@@QBE_NXZ` at Ego_r 0x0078EF7D,
  a 3-byte body before IsTriggered 0x0078EF80 = same `mov al,1;ret`; declared in engine/CTCTrapBase.h line 32; TCI_TRAP in
  CTCTrapBase::StatGetInterfaceType). Likely an ICF-shared stub -> candidate NAME_FIXES row (descriptive landed name). Draft: `CTCSpecialMove::IsSpecialMoveActive` -- made up.
- 0x0078BED0 / 0x0078BEE0 landed as `CountReferences::CountReferences` (`?M@C_0078bed0@@QAEKXZ` / `?M@C_0078bee0@@QAEKXZ`) -- WRONG.
  Real: CTCBerserk::GetWeaponTrailAttack() const / GetWeaponTrailKnockDown() const (Ego_r `?GetWeaponTrailAttack@CTCBerserk@@QBEJXZ` 0x00792F30,
  `?GetWeaponTrailKnockDown@CTCBerserk@@QBEJXZ` 0x00792F40, consecutive as retail; bodies read PDef_Object(+0x68)->+0x148/+0x14c).
  Interface 0x84 = TCI_BERSERK. NAME_FIXES candidates. Draft: CTCCharge::GetTrailGraphic/GetSpecialTrailGraphic -- made up.
  special -> KnockDown (0x78BEE0), else Attack (0x78BED0).
- 0x00733EE0 rebuild/src/compiled/00/73/CTCCombat_IsMeleeFlourishValid_00733ee0.cpp `CTCCombat::IsMeleeFlourishValid() const`
  (Ego_r `?IsMeleeFlourishValid@CTCCombat@@QBE_NXZ`). Interface 0x30 = TCI_COMBAT. Draft: CTCHand::IsActive -- wrong.
- 0x00766050 rebuild/src/compiled/00/76/CTCObjectAugmentations_HasCombinationOfAugmentations_00766050.cpp
  `CTCObjectAugmentations::HasCombinationOfAugmentations(EObjectAugmentationType) const` (engine/CTCObjectAugmentations.h line 41;
  NOTE the header also has an inline body at line 89 -- retail calls it OUT OF LINE, so the row must not let it inline). Interface 0xBF = TCI_OBJECT_AUGMENTATIONS.
- 0x0083AEA0 rebuild/src/compiled/00/83/CRetailTreeIterator_Decrement_0083aea0.cpp (`CRetailTreeIterator::Decrement`) -- STLport
  _Rb_tree_iterator::operator--() emitted out of line thiscall (ecx=&iter, returns this). Called 3 ways: reverse_iterator operator*
  (copy current to [esp+18], --copy, deref), operator++ (--current at [esp+14]).
- 0x0083B170 rebuild/src/compiled/00/83/stdRbtree...find_0083b170.cpp -- `_Rb_tree<...>::find(const key&)` non-const?/const, hidden
  iterator return (push &key; push &result), key placeholder CUnprovenKey_0083b170. Real type: the WeaponTrails map
  `map<EObjectAugmentationType,CWeaponTrailGraphicSet>` (Ego_r CWeaponDef.WeaponTrails) -- the placeholder could be respelled.
- CCreatureActionBase virtuals via [wielder+0x100]: +0x4c (slot 19), +0x48 (slot 18), +0xd8 (slot 54), called in that order, `||`.
  Counting engine/CCreatureActionBase.h virtual decls from 0: 18=IsKnockdownStrike, 19=IsBlockCounter, 54=PreventSneaking -- UNVERIFIED
  (rtti-vtables.tsv only lists slot 0). Semantically more plausible with a +1 shift (18 IsBlockCounter, 19 IsMeleeFlourish,
  54 IsDecapitatingAttack). Author: verify slot indices against the header's actual vtable (offsetof asserts / another landed caller)
  and use the header's class rather than the draft's v00..v53 stand-in.
- Release: def vtable slot 1 (+4) through CDefPointer dtor.

## 4. Members
- this/wielder (engine/CThing.h): UsingComponents bitset +0x20 (bits 71 [ebx+28] byte sign, 125 [ebx+2c]&0x20000000,
  132 [ebp+30]&0x10, 48 [ebp+24]&0x10000, 191 [ebx+34] sign); Components (CVectorMap) +0x44; ThingTypeFlags +0x6c (&8 = creature);
  PBaseDef +0x70 (CDefPointer<const CThingBaseDef>). Use FABLE_RETAIL_CTHING_GETTC_EXPANDED (PeekTC/GetTC out-param form, LowerBound out of line).
  Note which thing: 0x47 (carryable), 0x7d (trap), 0xbf (augmentations) are looked up on THIS (ebx); 0x84 (berserk), 0x30 (combat),
  and the +0x6c/+0x100 reads on the WIELDER (ebp).
- wielder +0x100: CThingCreatureBase::PCurrentAction (CCountedPointer<CCreatureActionBase>; engine/CThingCreatureBase.h
  PCurrentAction_Data +0x100 / Info +0x104, under FABLE_RETAIL_CTHINGCREATUREBASE). Cast wielder to CThingCreatureBase when flag 8.
- CTCCarryable PCarryingTC +0x10 (engine/CTCCarryable.h, FABLE_RETAIL_CTCBASE): `if (carryable->PCarryingTC) wielder = carryable->GetPCarriedByThing();`
- CTCCombat IsInMiddleOfFlourishAction +0x94 (bool; engine/CTCCombat.h line 148, Ego_r CTCCombat +0xa8 'IsInMiddleOfFlourishAction').
- CWeaponDef (engine/CWeaponDef.h, needs FABLE_RETAIL_DEF_CLASS_BASE): FlourishSpecialWeaponTrail +0x7c `long[2]` {Attack,Knockdown}
  (Ego_r CWeaponTrailGraphicSet{long Attack; long Knockdown;}); WeaponTrails +0x84 opaque `unsigned char[0xc]` =
  map<EObjectAugmentationType,CWeaponTrailGraphicSet> (header ptr +0x84, node key +0x10, Attack +0x14, Knockdown +0x18).
  The header models the map as bytes: the row needs a typed view (or an opt-in giving the real std::map type from stlport/fable_map.h).
  CWeaponDef.h opt-ins: only FABLE_RETAIL_CWEAPONDEF_TOOHEAVY (irrelevant).
- Local models in draft that have shared headers: CInterfaceMap (CVectorMap.h), CDefBase/CDefPointer (CDefPointer.h, CDefClassBase.h),
  CWeaponTrailDef (CWeaponDef.h), CThingBaseDef (CThingBaseDef.h), CTCCarryable, CTCCombat (as "CTCHand"), CTCObjectAugmentations,
  CTCBerserk (as "CTCCharge"), CTCTrapBase (as "CTCSpecialMove"), CCreatureActionBase, CThing, CThingGameObject, STLport tree node/iterator (stlport/fable_map.h).

## 5. Callee visibility
- KEY: GetSubDef<CWeaponDef> 0x0042B1A6 is an unaligned template COMDAT => body visible to retail. Opt-in
  FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE + #include "engine/CDefClassBase_GetSubDef.h": per its comment (row 0x0066BC10) the
  visible noinline body lets VC7.1 keep the def read back in a register (`mov edi,[esp+..]` after the call, release through edi,
  no reload) -- EXACTLY this row's residue (draft's CDefPointer stayed address-taken). Same pattern landed in DropWeapon 0x00845CE0
  ("weapon def held in edi"); 41 landed rows use the macro.
- CVectorMap::LowerBound const 0x004365B0: out of line (via CVectorMap.h; GETTC_EXPANDED form).
- STLport tree: operator-- (0x83AEA0) and find (0x83B170) are COMDATs at aligned addresses after this TU's functions (0x83ADA0..0x83B170
  are this TU's tree helpers: operator*, end, iterator ctors, ==, _S_value, _Select1st at 0x83ADA0-0x83AE20 all out of line too).
  fable_map.h: iterator::operator-- is inline calling static _M_decrement (cdecl) -- retail instead calls a thiscall operator-- out
  of line; needs a noinline/out-of-line operator-- for this instantiation. find: FABLE_RETAIL_STLP_TREE_FIND_TEMPLATE (decl-only member
  template, hidden-iterator call) / FABLE_RETAIL_STLP_TREE_CONSTFIND(_TEMPLATE_BODY) / FABLE_RETAIL_AUGMENTATION_PARTICLES (noinline
  const find for an EObjectAugmentationType-keyed map, CTCObjectAugmentations rows 0x007677E0) -- check which matches `cmp eax,[esi]`
  against header after the call. Reverse iteration: STLport reverse_iterator (*it copies current and decrements the copy) --
  see rebuild/src/compiled/00/7a/CTCEntityEvents_GetEvent_007afb90.cpp lines 60-96 for the idiom.
- HasCombinationOfAugmentations has an inline body in CTCObjectAugmentations.h:89 -- must stay a call.
- 0x787C18 / 0x78BED0 / 0x78BEE0 / 0x733EE0 / 0x705170 are separate TUs -> declaration only.

## 6. Siblings
- rebuild/src/compiled/00/83/CThingGameObject_GetMeleeHitParameters_00838bc0.cpp (auto_inline(off) islands, pack(1))
- rebuild/src/compiled/00/83/CThingGameObject_GetShotHitParameters_00838ec0.cpp (`// Flags: none`, FABLE_RETAIL_BASE_OBJECT,
  FABLE_RETAIL_CINTELLIGENTPOINTER, FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND)
- rebuild/src/compiled/00/83/CThingGameObject_OnCollision_00839080.cpp (CVectorMap.h), CThingGameObject_UpdateStandingOnThing_00838aa0.cpp
- rebuild/src/compiled/00/84/CCreatureActionDropWeapon_DropWeapon_00845ce0.cpp (GetSubDef<CWeaponDef> + GETSUBDEF_NOINLINE, def in edi)
- rebuild/src/compiled/00/73/CTCCombat_IsMeleeFlourishValid_00733ee0.cpp (FABLE_RETAIL_CTHING_GETTC_EXPANDED usage)
- None use optimize pragmas (speed TU, no /Oy-).

## 7. Draft model errors
- The address-taken CDefPointer (residue): fix with the visible noinline GetSubDef body (section 5), not with release-form tricks.
- Draft wraps `special` in GetInterface(&move,0x7d) on THIS -- correct object, wrong class/method (CTCTrapBase::IsCurrentTrapToCauseKnockdown).
- Flourish branch: `if (combat && combat->IsMeleeFlourishValid() && !combat->IsInMiddleOfFlourishAction)` then
  `graphic = special ? def->FlourishSpecialWeaponTrail[1] : [0]; if (graphic > 0) return graphic;` (jg) -- shape right, names wrong.
- Augmentation loop: retail iterates rbegin..rend with STLport reverse_iterator calling out-of-line operator-- (draft hand-rolled ok
  in shape); retail does NOT skip key 0 (draft adds `key &&`? -- retail tests the key `test eax,eax; je` before calling HasCombination,
  so the skip is real) ; key read from node+0x10 via operator* after a decrement of a copy.
- After loop: find(EObjectAugmentationType(0)) on WeaponTrails; `== end()` -> -1 path (shared with no-def exit at 0x839400:
  null-tested release + or eax,-1); else special ? Knockdown(+0x18) : Attack(+0x14).
- Draft uses local CThing stand-in with m_presentBits etc. -- use engine/CThing.h (GETTC_EXPANDED).

## 0x00AD9F10 (not landed)

# DOSSIER 0x00AD9F10 CVertexGroupBlockOptimiser::AddBlocks (806 B = 0x326; draft header's "0x324" is wrong)

## 1. Retail shape (xdis 00ad9f10 0x336)
- __thiscall, `ret 4` (one arg), 4 exits all `pop..; add esp,40h; ret 4`. No frame pointer (/Oy), no SEH (no /EHsc state
  despite a local vector with a dtor), speed idioms (`lea esp,[esp]`/`lea ebx,[ebx]` loop-alignment nops) -> default /O2, no pragmas.
- Prologue `sub esp,40h; push ebx; push esi; push edi`; **ebp pushed late** (0xAD9F7C, after the two early-return tests,
  shrink-wrapped) and holds the hoisted constant 0. The first early-exit path pops only edi/esi/ebx (0xADA22D).
- `this` in esi throughout; ebx = &OptimalBlockList (this+0xC), also spilled to [esp+24h] (later [esp+28h] after push ebp).
- `start` arg slot [esp+54h] is reused: InsertBlock's `added` out-ref, then the per-candidate new-group counter (draft does this - correct).
- Recursion: 4 self-calls (0xAD9FEE, 0xADA1B7, 0xADA1FE) + caller Process 0x00ADA385.

## 2. Identity (two sources -> manifest label CORRECT)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv @Ego_r 0x00c5d610) and FableWin.pdb (ghidra_out/_fablewin_api.tsv 0x0183c6c9,
  naming_stragglers/FableWin.exe.signatures.tsv): `?AddBlocks@CVertexGroupBlockOptimiser@@AAEXK@Z`
  = **private: void __thiscall CVertexGroupBlockOptimiser::AddBlocks(unsigned long)** (non-const). Mangled leaf `?AddBlocks@CVertexGroupBlockOptimiser@@AAEXK@Z`.
- Caller: Process 0x00ADA240 (landed rebuild/src/compiled/00/ad/CVertexGroupBlockOptimiser_Process_00ada240.cpp; PDB ?Process@...QAEXAAVCVertexGroupBlockList@@_N@Z). Not virtual (no vtable).

## 3. Callees (landed file | landed name | PDB name | draft spelling)
| addr | landed file (rebuild/src/compiled/00/..) | landed / PDB name | draft | flag |
|---|---|---|---|---|
| 0x00AD8FE0 | ad/CVertexGroupBlockOptimiser_CalcCurrentBestTheoreticalResultNewBlock_00ad8fe0.cpp | CalcCurrentBestTheoreticalResultNewBlock() `AAEKXZ` (private, non-const, ulong) | MinimumBlocksForCurrent | **WRONG** |
| 0x00AD9D10 | ad/..._CalcCurrentBestTheoreticalResultNewBlock2_00ad9d10.cpp | CalcCurrentBestTheoreticalResultNewBlock2() `AAEKXZ` | MinimumBlocksForRemaining | **WRONG** |
| 0x00AD94F0 | ad/CVertexGroupBlockOptimiser_FindBestStartingBlock_00ad94f0.cpp | FindBestStartingBlock() `AAEKXZ` (proven: builds map<ulong,CGroupInfo>; labels_chatgpt_pass9) | FindLargestUnusedBlock (a different PDB fn, ABEKXZ const) | **WRONG** |
| 0x00AD8DD0 | ad/CVertexGroupBlockOptimiser_CanInsertMoreBlocks_00ad8dd0.cpp | CanInsertMoreBlocks() const `ABE_NXZ` | CanInsertMoreBlocks() non-const | const missing |
| 0x00AD92E0 | ad/CVertexGroupBlockOptimiser_InsertBlock_00ad92e0.cpp | InsertBlock(unsigned long, unsigned long&) `AAEXKAAK@Z`, ret 8 | same | ok |
| 0x00AD9470 | ad/CVertexGroupBlock_RemoveLast_00ad9470.cpp | CVertexGroupBlock::RemoveLast(unsigned long) `QAEXK@Z` | same | ok |
| 0x00AD9030 | ad/CVertexGroupBlock_CVertexGroupBlock_00ad9030.cpp | ??0CVertexGroupBlock@@QAE@XZ | ctor | ok |
| 0x00A96DA0 | a9/CEntryListBuffer00a96da0_..._00a96da0.cpp (landed under a propagated class name; manifest says ~CVertexGroupBlock) | ??1CVertexGroupBlock@@QAE@XZ | dtor | ok (landed file misnamed) |
| 0x006699C0 | 66/CRetailTreeulongkey_find_006699c0.cpp | map<unsigned long,long>::find (folded COMDAT, 12 bodies), hidden-result `ret 8` | CGroupMap::find | ok |
| 0x00ADB570 | ad/CGuiControlTreePane_SortTreeRecursively_00adb570.cpp (**mislabel**: body is `mov edx,[ecx]` thiscall list distance loop) | really list<CVertexGroupBlock>::size() const | CBlockList::size | NAME_FIXES candidate |
| 0x00ADD020 | ad/STLlistCVertexGroupBlock_operator_00add020.cpp | ??4?$list@VCVertexGroupBlock... operator= | operator= | ok |
| 0x00ADCE10 | ad/STLlistCVertexGroupBlock_pushback_00adce10.cpp | list<CVertexGroupBlock>::push_back, ret 4 | ok | |
| 0x00ADC790 | ad/STLlistCVertexGroupBlock_popback_00adc790.cpp | list<CVertexGroupBlock>::pop_back (landed says __fastcall, plain ret; ecx=list) | ok | |
| 0x00ADB960 | ad/stdvector_reserve_00adb960.cpp | vector<CVertexGroupBlockOptimiser::CBlockInfo>::reserve, ret 4 | CCandidateArray::reserve | type name wrong |
| 0x00ADBFB0 | ad/stdvector_pushback_00adbfb0.cpp | vector<CBlockInfo>::push_back(const CBlockInfo&) | CCandidateArray::push_back | type name wrong |
| 0x00ADC890 | ad/STL_sort_00adc890.cpp (`// Flags: /Gr`) | std::sort<CBlockInfo*>(first,last) fastcall, uses CBlockInfo::operator< (Cost) | SortCandidates | name wrong |
| 0x00BFEA14 | bf/global_free_00bfea14.cpp | free | free | ok (vector dtor inline) |

## 4. Members (retail = STLport sizes; Ego_r offsets differ: Dinkumware 16-B vectors, 12-B lists)
No shared header exists for CVertexGroupBlockOptimiser, CVertexGroupBlockList, CBlockInfo (grep rebuild/include: none).
Shared header exists ONLY for CVertexGroupBlock: rebuild/include/engine/CVertexGroupBlock.h, opt-in
`FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS` (pulls engine/CArray.h + stlport/fable_map.h; gives private Groups CArray<unsigned long> +0,
GroupToIndexMap std::map<unsigned long,long> +0xC, inline const GetGroupCount()/GetGroup(i)/HasGroup(g) -- PDB QBEKXZ/QBEKK/QBE_NK).
The draft models CVertexGroupBlock LOCALLY although this header exists (verifier warns).
Optimiser members used (struct_layouts_egor.tsv names; retail offsets as all landed siblings):
- +0x00 InputList : CVertexGroupBlockList { CArray<CVertexGroupBlock> Blocks } (24-B elements, `imul 2AAAAAABh; sar 2`); PDB accessor GetBlockCount() const / GetBlock(i) const.
- +0x0C OptimalBlockList, +0x10 Blocks : std::list<CVertexGroupBlock> (4 B, node ptr).
- +0x14 CurrentBlock : CVertexGroupBlock*.   +0x18 RemainingBlocks : vector<bool> (retail one byte/element, CArray<bool> in PDB sig of CalcMinimumBlocks).
- +0x44 MaxN ulong, +0x4C MaxIterations ulong, +0x50 Greedy bool, +0x54 Iterations ulong. (Unused here: +0x24 CurrentBlockSegmentGroups,
  +0x30 CurrentBlockSegmentIndices, +0x3C CurrentBlockSegmentCount, +0x40 MaxGroupIndex, +0x48 OptimalSize, +0x58 Debug.)
- CBlockInfo (nested, Ego_r 12 B): BlockIndex ulong +0, Cost long +4, Benefit long +8; `bool operator<(const CBlockInfo&) const` (??M, Cost<).
  Retail fills {j, newGroups, groupCount} at [esp+38h..40h] -> BlockIndex=j, Cost=new groups, Benefit=block group count.
STLport opt-ins available: fable_containers.h `FABLE_RETAIL_STLP_LIST` (+ `FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE`, back() = end()-- -> `mov edx,[ecx+4]; add edx,8` as retail 0xADA1AA),
fable_map.h find variants (`FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_NOINLINE`, `..._CONSTFIND_TEMPLATE_BODY`, `FABLE_RETAIL_STLP_TREE_MFIND_DECL`),
CArray.h `FABLE_RETAIL_CARRAY*`. Landed CVertexGroupBlock rows use local models + `#pragma auto_inline(off)` around a same-TU noinline find body.

## 5. Callee visibility (same TU = vertex-group-block-optimiser .cpp, 0x00AD8C30..0x00ADA3F0, all landed)
- Same-TU, defined earlier, called out of line: CanInsertMoreBlocks 8DD0, NewBlock 8FE0, InsertBlock 92E0, RemoveLast 9470,
  FindBestStartingBlock 94F0, NewBlock2 9D10 (all aligned, real members). VC7.1 saw their bodies but did not inline (large / called recursively-adjacent).
- **Inline in AddBlocks**: HasMoreBlocks (loop at 0xAD9F87, PDB ?HasMoreBlocks ABE_NXZ const), the first OptimalBlockList.size() (loop at 0xAD9F50),
  CVertexGroupBlock::GetGroupCount/HasGroup (HasGroup inlined but its map find stays a CALL to 0x006699C0), vector<CBlockInfo> ctor + dtor (free at 0xADA224),
  list::back(), CBlockInfo copy.
- **Out of line although template/inline**: list::size() (0xADB570, called 3x later in the same function while the first use was inlined -- inline budget),
  map::find 0x006699C0 (IsProperSubsetOf 0x00AD8CA0 models it as a same-TU `__declspec(noinline)` body under `#pragma auto_inline(off)`),
  list push_back/pop_back/operator=, vector reserve/push_back, std::sort.
- The find call passes (result*, key*) with key in a frame local [E+1Ch] and the iterator result [E+20h] compared with [ebx]=map header (`it == GroupToIndexMap.end()` -> ++count).

## 6. Sibling landed rows (all rebuild/src/compiled/00/ad/)
- CVertexGroupBlockOptimiser_CanInsertMoreBlocks_00ad8dd0.cpp -- `// Flags: none`, no pragmas; full STLport vector/map/list model, same-TU HasGroup auto-inlined; indexed loops (`InputList.Blocks[i]`, `block.Groups[j]` count hoisted) -> retail count-down strength reduction.
- CVertexGroupBlockOptimiser_CountRemainingGroups_00ad8ed0.cpp -- class layout donor (InputList, lists, CurrentBlock, vector<bool> RemainingBlocks, ...); no pragmas.
- CVertexGroupBlockOptimiser_CalcCurrentBestTheoreticalResultNewBlock_00ad8fe0.cpp -- CBlockList::size() for-loop model.
- CVertexGroupBlock_IsProperSubsetOf_00ad8ca0.cpp -- `#pragma auto_inline(off)` + noinline same-TU find (0x006699C0), FABLE_FORCEINLINE HasGroup, named iterator local.
- CVertexGroupBlockOptimiser_Process_00ada240.cpp -- the caller; STLport list/map/vector model incl. CBlockList, CVertexGroupBlockList.
- STL_sort_00adc890.cpp (`// Flags: /Gr`), stdvector_pushback_00adbfb0.cpp -- carry the CBlockInfo class (nested in CVertexGroupBlockOptimiser).

## 7. Draft model errors vs disassembly
- Callee names wrong (MinimumBlocksForCurrent/Remaining, FindLargestUnusedBlock, SortCandidates, CCandidate*, CGroupMap) -- see table; CanInsertMoreBlocks must be const; CBlockInfo field names BlockIndex/Cost/Benefit.
- Local CVertexGroupBlock while engine/CVertexGroupBlock.h exists (FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS: uses GetGroupCount/GetGroup/HasGroup).
- HasMoreBlocks: retail reuses its count (eax) as the reserve() argument (`push eax` at 0xADA011) -- InputList size computed once and live across the CurrentBlock test; draft calls InputList.size() again for reserve (CSE may or may not merge).
- Order of evaluation: at 0xADA157.. retail computes Blocks.size(), Optimal.size(), then Optimal.size() AGAIN, THEN calls NewBlock2 -> spelling `OptimalBlockList.size() <= ...NewBlock2()` or `size >= ...` both must evaluate size first; top test likewise computes inline Optimal size before calling NewBlock (draft's explicit `optimal` local is fine there).
- Candidate loop: retail inner loop is pointer + count-down ([esp+14h] counter, ebp = group ptr, ebx = &CurrentBlock->GroupToIndexMap reloaded per j), skips the `<= space` compare when count==0 (jbe to push) -- natural from `for k<count` + unsigned compare; previous attempts lost because the zero went to ebx.
- Candidate vector: `CArray`/`std::vector<CBlockInfo>` default-ctor (3 zero stores from ebp), first/last read (edi=end, ebp=begin) BEFORE the Greedy test; dtor frees ebp=begin.
- Empty-candidate path: `CVertexGroupBlock* saved=CurrentBlock` read before temp ctor; push_back(CVertexGroupBlock()) temp dtor then `CurrentBlock=&Blocks.back()` -- draft order matches.
- Non-empty path: `next = Greedy ? 0 : BlockIndex+1` (retail `xor eax,eax; test cl,cl; jne; mov eax,[edi]; inc eax`).
- Prior residue (hand310 NOTES): 826 v 806, only regalloc: zero in ebx not ebp; 9 variants tried (see rebuild/candidates/manual/pending/2026-10-23_linux_hand310/NOTES.md).

## 0x008C12D0 (not landed)

# DOSSIER 0x008C12D0 -- CCombatAction_StrikeResponseJackOfBladesIntoRush::Initialise (808 bytes)

## 1. Retail shape (xdis 008c12d0 0x338)
- `sub esp,3Ch ; push ebx ; push ebp ; push esi ; mov esi,ecx ; lea ebp,[esi+8] ; push edi` ... `pop edi/esi/ebp/ebx ; add esp,3Ch ; ret` (no arg: `ret`).
  thiscall, no frame pointer (/Oy default, no optimize("y",off)), no SEH/EH frame, /O2 speed idioms (no optimize("s")). Frame 0x3C.
- Register plan: ebp = &PCreature (+8) for the whole body; esi = this; edi = map/boss -> def; ebx = 0 (xor) then hitter.
- Flow: GetTC(0xAD) inline (`test ch,20h` = UsingComponents bit 173 at +0x34; key 0xAD stored at [esp+18h] INSIDE the test; LowerBound 0x40F020;
  `cmp eax,end ; je ; cmp [eax],0ADh ; jle ; mov eax,end` then `mov edi,[eax+4]`); absent path reads stale [esp+18h] (boss never null-initialised).
  def slot [esp+14h] = ebx(0); GetPItem; `push &def ; mov ecx,[eax+70h] ; call 0x8C33E0` (creature fetched before &def pushed).
  hits=boss->GenericCounter+1 compared with def[+40h]: `jg -> 0 ; jne -> 0 ; else 1` stored in [esi+148h], then `cmp [esi+148h],bl` (re-read).
  !CauseRush: `call 0x865F40 ; jmp release` (release with def still in edi). Rush path: GetFacingVector (slot 70 +0x118, hidden ret ptr [esp+40h]),
  PointOfHit(+11C) - GetPos; IS-alive test on PThingThatHit (+114, `test byte [eax+91h],1`), hitter delta; away=(0,0,?) zeroed X,Y only;
  inline fast-inv-sqrt Normalise (0x17C, table 0x013CE558, 3.0f 0x122DED4, 0.5f 0x122F59C); dot; `fld qword [1277128h] ; fcos ; fxch ; fxch ; fcompp ; test ah,41h`;
  two CCharString temporaries (0x12802FC back / 0x12802DC front) tail-merged into one SetCombatAnimation call + one ~CCharString;
  fpatan*1/2pi (0x1237F08), WrapAngle via __CIfmod 0xBFEB28 (fmod intrinsic), PhysicsTC slot 66 (+0x108) SetFacingAngleXY;
  `call 0x857520` (CCombatActionBase::Initialise, direct); then `mov edi,[esp+14h]` (def reloaded) and release `dec [edi+4] ; jne ; call [vft+4]` -- NO null test.

## 2. Identity
- Name CONFIRMED: `?Initialise@CCombatAction_StrikeResponseJackOfBladesIntoRush@@UAEXXZ`, `virtual void Initialise()` (public virtual, void, no params, non-const).
  Sources: (a) rtti-vtables.tsv: CCombatAction_StrikeResponseJackOfBladesIntoRush vtable 0x012808EC slot 12 = 0x008C12D0; slot 12 is Initialise in
  CCombatActionBase (0x01259E44 slot12 = 0x00857520 ?Initialise@CCombatActionBase) ; (b) Ego_r.pdb egor_pdb_names.tsv has the exact leaf
  (Ego_r address 0x008F94F0, a different binary). xref: only reference is the vtable (.rdata 0x0128091C). Manifest label is right.
- Leaf as in the drafts: `// Leaf: ?Initialise@CCombatAction_StrikeResponseJackOfBladesIntoRush`.

## 3. Callees (landed file / landed name / PDB name / draft spelling)
| addr | landed row | landed/PDB name | draft spelling | flag |
|---|---|---|---|---|
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ (returns CBaseObject*) | local CBaseIntelligentPointer returning CThing* | use engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER) + cast, as siblings |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound(const ETCInterfaceType&) | CInterfaceMap::LowerBound(const long*) EXTERN | WRONG spelling; also siblings define it same-TU noinline (engine/CVectorMap.h) |
| 0x008C33E0 | 00/8c/CDefClassBase_GetSubDefCJackOfBladesBattleDef_008c33e0.cpp | ??$GetSubDef@VCJackOfBladesBattleDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCJackOfBladesBattleDef@@@@@Z (const, CDefPointer<const T>*) | non-const, CDefPointer<CJackOfBladesBattleDef>* | const + const T missing |
| 0x00865F40 | NOT landed (parked 2026-10-23_linux_hand314, DIFFER 1106v1161) | ?Initialise@CCombatAction_GenericStrikeResponseBase@@UAEXXZ (Ego_r) | same | ok (other TU) |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | ?GetPos@CThing@@QBEABVC3DVector@@XZ | GetPosition | WRONG -> GetPos |
| vslot 70 (+0x118) | CTCPhysicsBase 0x006B1410 | ?GetFacingVector@CTCPhysicsBase@@UBE?AVC3DVector@@XZ (const) | GetFacing() non-const | WRONG -> GetFacingVector() const |
| vslot 66 (+0x108) | CTCPhysicsBase 0x006B1260 | ?SetFacingAngleXY@CTCPhysicsBase@@UAEXM@Z | SetFacingAngleXY | ok |
| 0x0099EBF0 / 0x0099EAE0 | 00/99/CCharString_Constructor_0099ebf0.cpp / CCharString_CCharString_0099eae0.cpp | CCharString(const char*,long) / ~CCharString (fable_string.h) | local | use fable_string.h |
| 0x008574D0 | 00/85/CCombatActionBase_SetCombatAnimation_008574d0.cpp | ?SetCombatAnimation@CCombatActionBase@@QAEXABVCCharString@@@Z | SetAnimationName | WRONG -> SetCombatAnimation |
| 0x00BFEB28 | 00/bf/global_CIfmod_00bfeb28.cpp | __CIfmod (toolchain lib; fmod intrinsic) | fmod intrinsic | ok |
| 0x00857520 | 00/85/CCombatActionBase_Initialise_00857520.cpp | ?Initialise@CCombatActionBase@@UAEXXZ | ok | ok (engine/CCombatActionBase.h FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE declares Initialise + SetCombatAnimation(const CCharString&)) |
| def vslot 1 | -- | CDefPointeeBase::Delete (engine/CDefPointer.h names it Delete, count IVRefCount) | Destroy / RefCount | respell |
| global 0x013CE558 | -- | ?GFastOneOverSqrtTable@@3PAKA (naming wave 18, sibling 0x008C2550) | g_InvSqrtMantissaTable | WRONG -> GFastOneOverSqrtTable |

## 4. Members
- this +0x08 PCreature (CCreatureActionBase, CIntelligentPointer<CThingCreatureBase> in Ego_r; landed 0x857520 models it so).
- this +0x114 PThingThatHit (CIntelligentPointer), +0x11C PointOfHit (C3DVector): engine/CCombatAction_GenericStrikeResponseBase.h has them only as
  opaque `unsigned char[8]/[12]`, PROTECTED, base CCombatActionBase sizeof 0x148 asserted. No opt-in macros.
- this +0x148 CauseRush (bool): engine/CCombatAction_StrikeResponseJackOfBladesIntoRush.h is generated opaque padding, not derived from the base
  (PDB +0x160). Siblings 0x008C2550 / 0x008C1600 / 0x008C2810 model the class LOCALLY for that reason (accepted).
- CThing (engine/CThing.h, asserts): UsingComponents +0x20 (bitset; HasTC), Components +0x44 (CVectorMap), PhysicsTC +0x60 (GetPhysicsTC()),
  PBaseDef +0x70 (CDefPointer<const CThingBaseDef>; retail reads the word and calls GetSubDef on it), Dead bit0 of +0x91 (IsDead/IsAlive).
  Opt-in: FABLE_RETAIL_CTHING_GETTC_EXPANDED gives GetTC(ETCInterfaceType, T**) with exactly retail's in-test key store + conditional-expression end test (used by landed 0x857520).
  Draft names m_presentBits/m_interfaces/m_pPhysics/m_pDef/m_flags91 are all made up.
- CTCBoss (engine/CTCBoss.h, retail size 72): GenericCounter +0x10 (long, PRIVATE, no accessor); StatGetInterfaceType()=173 (0xAD). Opt-ins
  in header: FABLE_RETAIL_JACK_STAGE1_UPDATE / _CTCBOSS_EFFECTS / _SETHELPER / _KILLHELPERS / _LIVINGHELPERS (none needed; private access is the issue).
- CJackOfBladesBattleDef (engine/CJackOfBladesBattleDef.h, sizeof 0x80 retail): NumGetHitsBeforeRush long at retail +0x40 (PDB +0x44). Needs StatGetTypeName (landed GetSubDef row defines it inline).
- CDefPointer (engine/CDefPointer.h, requires FABLE_RETAIL_DEF_CLASS_BASE): Object +0, Release() null-tests (all variants: plain, _RELEASE_COUNTREF, _RELEASE_POINTEE,
  _SELFDELETING). Retail has NO null test -> shared header doesn't give it; siblings use a local no-null-test CDefPointer stand-in.
- Classes the old draft models locally that have shared headers: C3DVector, CCharString(fable_string.h), CThing, CBaseIntelligentPointer,
  CVectorMap(CInterfaceMap), CDefPointer, CDefClassBase(CDefClassBase_GetSubDef.h), CJackOfBladesBattleDef, CTCBoss, CTCPhysicsBase,
  CCombatActionBase, CCombatAction_GenericStrikeResponseBase, CCombatAction_StrikeResponseJackOfBladesIntoRush.

## 5. Callee visibility
- Same TU (retail neighbours 0x008C1600 GetValidRushPosition, 0x008C2530 ctor, 0x008C2550 FinishAction, inline IsTo* at 0x8C3A70..): the GetSubDef<CJackOfBladesBattleDef>
  COMDAT 0x008C33E0 sits after the TU's functions -> body visible (template). Landed siblings use a same-TU __declspec(noinline) GetSubDef STAND-IN
  (body only writes out->Object), which also drops the null test. engine/CDefClassBase_GetSubDef.h has FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE (real body, noinline).
- CVectorMap::LowerBound 0x0040F020: both landed siblings in this TU define it SAME-TU NOINLINE (engine/CVectorMap.h default branch; "VC7.1 then knows the callee does not capture the key").
  The old draft declared it EXTERN, and NOTES' tried-list never lists a visible LowerBound -> UNTRIED lever for the &PCreature ebp vs edi register gap
  (an escaping key address changes what is live/aliased across GetSubDef). Alternative: FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND (inline) -- wrong here (retail calls it).
- Other callees (GetPItem 0xA01B50, GetPos 0x4C73D0, CCharString, SetCombatAnimation, the two base Initialise, physics virtuals) are other-TU, out of line.
- STLport: none called.

## 6. Siblings to copy idioms from
- 00/8c/CCombatActionStrikeResponseJackOfBladesIntoRush_FinishAction_008c2550.cpp (same class, same TU, same GetTC(0xAD)+GetSubDef prologue):
  fable_compiler.h, engine/CVectorMap.h (same-TU LowerBound), FABLE_RETAIL_CINTELLIGENTPOINTER + engine/CIntelligentPointer.h, local
  CDefPointer_CJackOfBladesBattleDef, noinline GetSubDef stand-in, FABLE_FORCEINLINE CThing::GetSubDef wrapper "so the creature is fetched before &def is pushed",
  FastInvSqrt with GFastOneOverSqrtTable, `#pragma intrinsic(sqrt)`. No // Flags, no optimize pragma.
  NAME_FIX candidates there: CJackOfBladesBattleDef "MaxRushCount" +0x40 is PDB NumGetHitsBeforeRush; CTCRushCounter::RushCount +0x10 (iface 0xAD) is CTCBoss::GenericCounter.
- 00/8c/CCombatActionStrikeResponseJackOfBladesIntoRush_GetValidRushPosition_008c1600.cpp (same class/TU): local CDefPointer without null test, same-TU noinline LowerBound
  and GetSubDef stand-ins, def declared BEFORE the boss lookup there (order differs from this row: here boss first).
- 00/8c/CCombatActionStrikeResponseMazeDoForcePush_FinishAction_008c2810.cpp (sister strike response; fable_compiler.h, math.h, engine/CVectorMap.h, CIntelligentPointer).
- 00/85/CCombatActionBase_Initialise_00857520.cpp: FABLE_RETAIL_CTHING_GETTC_EXPANDED + engine/CThing.h + engine/CVectorMap.h + CIntelligentPointer; GetTC(TCI_x,&p).
- 00/8c/CCreatureActionJackOfBladesUnblockableMidAttackNoBlend_Initialise_008c0460.cpp (CCharString temp + direct CCombatActionBase::Initialise).
- No row in 0x008C0000-0x008C3FFF carries `// Flags:`; 5 rows use `#pragma optimize("t", on)` (none needed here: shape is /O2).

## 7. Old draft model errors vs disassembly
- Control flow / x87 / strings: already in retail order (DIFFER 809v808); residue is ONLY register allocation (&PCreature edi vs retail ebp; def in ebp; frame 0x38 vs 0x3C because the def slot is reused for the angle).
  Retail keeps the def slot [esp+14h] alive to the end (reload `mov edi,[esp+14h]` after 0x857520) and the angle in its own slot [esp+10h].
- LowerBound extern (escaping key) instead of the sibling same-TU noinline body -- untried; GetInterface uses `if` re-assignment rather than the
  GETTC_EXPANDED conditional expression (retail bytes fit both; header form is the lane default).
- GetSubDef non-const with CDefPointer<CJackOfBladesBattleDef> (PDB: const member, CDefPointer<const T>*); GetDef() accessor invented (PBaseDef +0x70).
- Wrong callee spellings: SetAnimationName->SetCombatAnimation, GetPosition->GetPos, GetFacing->GetFacingVector() const, Destroy/RefCount->Delete/IVRefCount, g_InvSqrtMantissaTable->GFastOneOverSqrtTable, CInterfaceMap::LowerBound(const long*)->CVectorMap<..>::LowerBound(const ETCInterfaceType&).
- Local C3DVector/CThing/CTCBoss/CJackOfBladesBattleDef models where shared headers exist (CTCBoss::GenericCounter is private: needs a local model or friend).
- The CauseRush computation `if (hits > N) false; else if (hits == N) true; else false;` matches retail's `jg/jne` pair; keep it.

