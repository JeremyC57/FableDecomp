## 0x0091B3A0

# DOSSIER 0x0091B3A0 CAIStateGroup_SeekPerceivedThing::RunToPerceivedThingPosition (455 B)

## 1. Retail shape (xdis 0091b3a0 0x1d7)
- __thiscall, no args, plain `ret` (0x91b566). this -> esi for the whole body.
- Prologue `push ebp; mov ebp,esp; and esp,-8; sub esp,0xd8; push ebx; push ebp; push esi; push edi`: 8-ALIGNED frame (VC7.1
  alignment frame, NOT /Oy-); ebp is then reused as a general register (= perception component). The old draft already reproduced it.
- NO SEH prologue even with two dtor'd locals (CCharString temp, CCreatureAction_PlayAnimation) -> TU built without /EHsc
  (same as analogue 0x0093C6D0 SetWhatsThatAnimation, also no EH frame).
- `lea ebx,[ebx]` pad at 0x91b41a: inner loop head aligned to 0x91b420.
- Register roles: esi=this, ebp=CTCPerceiveThing* (lives across both loops), edi=&perception->LastPerceivedPos (lea [ebp+0x20])
  then anim pointer, ebx=creature for 2nd IsInRange, then 0 for the action args.
- Stack: [esp+0x13] byte = HasTC bit (shr ecx,0x14; and cl,1; mov [esp+13],cl) later REUSED as `bool reached`;
  [esp+0x18] = key local (0x34) reused for `result` (EAIActionReturn); [esp+0x1c..0x24] = C3DVector pos copy;
  [esp+0x14] CCharString temp; [esp+0x28] CSoundPair; [esp+0x30] action object.
- Not-has path: ebp = uninitialised [esp+0x18] (the component local is never initialised, like 0x00933B30).
- Control flow:
  L3f5: if (AwarenessTimer == 0.0f) return;    // fld [0x122DEDC]=0.0f first, fld [ebp+18]; fucompp; test ah,44h; jnp exit
        if (IsStopping()) return;
  L420: if (AwarenessTimer == 0.0f) goto L3f5;  // inner loop re-test jumps back to the OUTER test
        pos = LastPerceivedPos (fld x kept on x87, fst [esp+1c], y/z via eax/ecx, then a by-value C3DVector copy pushed)
        result = AIAction_MoveToPosProximity(2, pos, 0.5f, reached, 5, true,false,true, 0(NULL), true, 0.0f);   // pushes 0,1,0,1,0,1,5,&reached,0.5f,vec,2
        if (Creature->IsInRange(pos,3.5f) && !Creature->IsInRange(perception->GetPerceivedThing()->GetPos(),3.5f)
            && !perception->LineOfSight && Creature->IsInRange(perception->LastPerceivedPos,3.5f)) {
            anim = Creature->GetRandomAnimFromName(CCharString("WHATS_THAT"/*0x01273694*/,-1), -1);  // string dtor right after
            CCreatureAction_PlayAnimation action(creature, anim, CSoundPair(), false, true, false, 0, 0);
            AIAction_PerformActionAndWait(action, false);   // ~CCreatureActionBase 0x693EF0 after
        }
        if (result == 4) goto L420; else goto L3f5;
  The creature is reloaded (this->PBrain->Creature) before every call: no cached creature local.
  2nd IsInRange: push 3.5f FIRST, then GetPerceivedThing()/GetPos(), creature (ebx) loaded before the push -> `Creature->IsInRange(...)`
  with the creature expression evaluated before the args.
  The anim's creature arg to the ctor is re-read from PBrain (edx/[edx+18]) after GetRandomAnimFromName.

## 2. Name / signature (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0093eca0) AND FableWin.pdb (_fablewin_api.tsv 02bd3b90 / naming_stragglers):
  `?RunToPerceivedThingPosition@CAIStateGroup_SeekPerceivedThing@@AAEXXZ` -> PRIVATE, `void RunToPerceivedThingPosition()`, non-const, non-virtual.
- Only caller: 0x0091B749 inside 0x0091B570 (CAIStateGroup_SeekPerceivedThing::Update, PDB ?Update@...@@UAEXXZ; unlanded DIFFER, same pending dir).
- Manifest label is CORRECT. Sibling PDB members: OnActivate, CapturePerceivedThing (AAEXXZ), Cleanup, IsRunnable, Update, Alloc, GetName.

## 3. Callees (landed row / PDB) -- draft disagreements flagged
| addr | landed file | PDB name | draft spelling (wrong?) |
|---|---|---|---|
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound (non-const copy) | draft local CInterfaceMap::LowerBound -- use CThing.h GetTC (FABLE_RETAIL_CTHING_GETTC_EXPANDED) |
| 0x008FD010 | 00/8f/CAIStateGroupBase_IsStopping_008fd010.cpp (lands `long ... const`) | ?IsStopping@CAIStateGroupBase@@IBE_NXZ = protected `bool IsStopping() const` | draft non-const |
| 0x0097E890 | 00/97/CAIStateGroupBase_AIActionMoveToPosProximity_0097e890.cpp | ?AIAction_MoveToPosProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@MAA_NW4EMovementType@@_N44PBVCThing@@4M@Z | draft: `long`, `bool*` reached, `CThing*` -- PDB: EAIActionReturn, bool&, const CThing* |
| 0x006A4DF0 | 00/6a/CThingPhysical_IsInRange_006a4df0.cpp | ?IsInRange@CThingPhysical@@QBE_NABVC3DVector@@M@Z `bool IsInRange(const C3DVector&,float) const` | draft puts it on CThingCreature, non-const. NO shared-header declaration exists (grep CThingPhysical.h: none) |
| 0x007446A0 | 00/74/CTCPerceiveThing_GetPerceivedThing_007446a0.cpp | ?GetPerceivedThing@CTCPerceiveThing@@QBEPBVCThing@@XZ `const CThing* GetPerceivedThing() const`, NO arg | draft passes 3.5f and class CTCPerception -- WRONG |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | ?GetPos@CThing@@QBEABVC3DVector@@XZ | draft "GetPosition" -- WRONG spelling |
| 0x0099EBF0 | 00/99/CCharString_Constructor_0099ebf0.cpp | ??0CCharString@@QAE@PBDJ@Z | ok |
| 0x00662D80 | 00/66/CThingCreatureBase_GetRandomAnimFromName_00662d80.cpp | ?GetRandomAnimFromName@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@J@Z (const, returns const CAnimationEntry&) | draft on CThingCreature, returns pointer, non-const |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ??1CCharString@@QAE@XZ | ok |
| 0x00842570 | 00/84/CCreatureActionPlayAnimation_CCreatureActionPlayAnimation_00842570.cpp | ??0CCreatureAction_PlayAnimation@@QAE@AAVCThingCreatureBase@@ABVCAnimationEntry@@ABVCSoundPair@@_N33JJ@Z (refs!) | draft uses pointers (same bytes) |
| 0x0097DB70 | 00/97/CAIStateGroupBase_AIActionPerformActionAndWait_0097db70.cpp | ?AIAction_PerformActionAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@_N@Z (const&) | draft pointer + long |
| 0x00693EF0 | 00/69/CCreatureActionBase_CCreatureActionBase_00693ef0.cpp | ??1CCreatureActionBase@@UAE@XZ (virtual) | draft non-virtual dtor (still direct call since complete object) |
Float 0.0f @ 0x0122DEDC; 0.5f / 3.5f are immediates; "WHATS_THAT" @ 0x01273694.

## 4. Members
- this+0x04 CAIStateGroupBase::PBrain (CAIBrain*) -- engine/CAIStateGroupBase.h (FABLE_RETAIL_CAISTATEGROUPBASE[_MEMBERS]).
- PBrain+0x18 CAIBrain::Creature (CThingAICreature*; PDB struct_layouts: `CThingAICreature * Creature`; FableWin: reference, private)
  -- engine/CAIBrain.h; FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE + inline GetCreature(); CAIStateGroupBase::GetCreature() inline
  under FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE (+ CAIBrain GetCreature). Draft names it PCreature -- WRONG (Creature).
- Creature+0x20/+0x24 CThing::UsingComponents bitset (bit 0x34 = TCI_PERCEIVE_THING -> dword +0x24, bit 20), +0x44 Components
  CVectorMap -- engine/CThing.h, GetTC<T>(id,&p) FABLE_FORCEINLINE under FABLE_RETAIL_CTHING_GETTC_EXPANDED (non-const GetTC -> LowerBound 0x40F020; PeekTC would hit const 0x4365B0 -- so use GetTC).
- CTCPerceiveThing (engine/CTCPerceiveThing.h, FABLE_RETAIL_CTCPERCEIVETHING + FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS):
  +0x18 float AwarenessTimer (draft "Alertness" WRONG) -> inline `IsAwareOfThing() const {return AwarenessTimer!=0.0f;}`;
  +0x20 C3DVector LastPerceivedPos (private in that branch; NO inline GetLastPerceivedPos in the retail branch -- only a declaration
  in the SMELL branch; no out-of-line copy in either PDB => it is an inline accessor; author needs a nested opt-in adding
  `const C3DVector& GetLastPerceivedPos() const { return LastPerceivedPos; }`);
  +0x2d bool LineOfSight (draft "Seen" WRONG) -> inline `IsThingLineOfSight() const`.
  GetPerceivedThing() declared out-of-line in that branch (matches retail call).
- CAIStateGroup_SeekPerceivedThing.h: generated flat header (FrameLastCaptured +0x14, sizeof 0x18), no retail opt-in; the row
  needs the class declared with base CAIStateGroupBase (siblings model it locally).
- Locally modelled in draft though a shared header exists: C3DVector (engine/C3DVector.h), CThing/CInterfaceMap (CThing.h, CVectorMap.h),
  CCharString (fable_string.h / FABLE_RETAIL_CCHARSTRING), CSoundPair (CSoundPair.h), CThingCreature(Base) (CThingCreatureBase.h,
  FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY declares GetRandomAnimFromName), CTCPerception (CTCPerceiveThing.h),
  CCreatureActionBase (CCreatureActionBase.h), CCreatureAction_PlayAnimation (CCreatureAction_PlayAnimation.h, FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION:
  ctor (CThingCreatureBase&,const CAnimationEntry&,const CSoundPair&,bool,bool,bool,long,long)), CAIBrain (CAIBrain.h), CAIStateGroupBase.

## 5. Visibility
- Inline/visible in retail (no call emitted): HasTC + GetTC expansion (only LowerBound out of line), IsAwareOfThing, IsThingLineOfSight,
  GetLastPerceivedPos, CAIStateGroupBase::GetCreature / CAIBrain::GetCreature, CSoundPair() ctor (two zero stores), C3DVector copy.
- Out of line: everything in the callee table. Opt-ins: FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS + _MOVETOPOSPROXIMITY (MoveToPosProximity decl),
  FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION (PerformActionAndWait decl), FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS (IsStopping bool const),
  FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY (GetRandomAnimFromName const&), FABLE_RETAIL_CTCPERCEIVETHING(_AWARENESS).
- HasTC note (CThing.h FABLE_RETAIL_CTHING_HASTC_FORCEINLINE comment): retail here materialises the presence bit in a stack byte
  (`shr; and cl,1; mov [esp+13h],cl`) -- this is the PLAIN-inline HasTC result stored; do NOT define HASTC_FORCEINLINE.
- No STLport helpers involved. Same-TU neighbours: OnActivate 0x91B2F0, Cleanup 0x91B330, IsRunnable 0x91B340 precede it; Update 0x91B570 follows.

## 6. Siblings
- 00/91/CAIStateGroupSeekPerceivedThing_IsRunnable_0091b340.cpp (same class; old local model, inlined GetInterface type 0x34, 0.0f@0x122DEDC test).
- 00/91/CAIStateGroupSeekPerceivedThing_OnActivate_0091b2f0.cpp (same class, local model).
- 00/93/CAIStateGroupStealthSeekEnemy_SetWhatsThatAnimation_0093c6d0.cpp -- closest analogue: same WHATS_THAT CCharString temp +
  GetRandomAnimFromName + CCreatureAction_PlayAnimation(creature, anim, CSoundPair(), false,true,false,0,0) + ~CCreatureActionBase; no EH.
- 00/93/CAIStateGroupMinionCuriosity_Update_00933b30.cpp -- `#define FABLE_RETAIL_CTHING_GETTC_EXPANDED` + PeekTC(TCI_PERCEIVE_THING,&p) uninitialised component.
- 00/95/CAIStateGroupBehaviourBase_UpdateFocusAttentionOnThingActive_009578a0.cpp -- full shared-header opt-in set:
  CCHARSTRING, CCLASS, DEF_CLASS_BASE, CTHING, CTCBASE, CINTELLIGENTPOINTER(_COPY,_CONVERT_MUTABLE), CTCPERCEIVETHING(_AWARENESS),
  CAISTATEGROUPBASE(_MEMBERS,_AIACTIONS), CAISTATEGROUPBEHAVIOURBASE; includes engine/fable_ai_action_enums.h; do-while on AI_ACTION_IN_PROGRESS.
- None of these carry #pragma optimize or // Flags: lines (default lane flags).

## 7. Draft model errors
- GetPerceivedThing(3.5f): extra argument (retail calls with none) -> stray push; wrong class (CTCPerception).
- Loop shape: draft `while(Alert && !IsStopping()) { do { if(!Alert) break; ... } while(result==4); }` -- semantically matches, but
  retail's inner `!Alert` jumps to the OUTER Alert test (L3f5), and result==4 jumps to L420 else L3f5. Consider writing it as
  `while (IsAwareOfThing() && !IsStopping()) { EAIActionReturn r; do { if(!IsAwareOfThing()) break; ... } while (r == AI_ACTION_IN_PROGRESS /*4*/); }`
  -- keep, but make `result` an EAIActionReturn and `reached` a bool passed by reference.
- `GetInterface` lambda-model: use GetTC (non-const, LowerBound 0x40F020) from CThing.h GETTC_EXPANDED; component local uninitialised.
- LastPerceivedPos read twice: once copied into the `pos` local (passed by value and to first IsInRange), once by reference for
  the 4th IsInRange (edi=lea [ebp+20h]) -- keep both reads; draft's `target` copy + direct member are right.
- Names: Alertness->AwarenessTimer (via IsAwareOfThing()), Seen->LineOfSight (IsThingLineOfSight()), PCreature->Creature,
  GetPosition->GetPos, IsInRange on CThingPhysical (const, bool), GetRandomAnimFromName returns const CAnimationEntry&,
  PlayAnimation ctor/PerformActionAndWait take references, IsStopping bool const.
- Draft's 3.5f comment in header already notes the GetPerceivedThing bug; NOTES.md line 79 confirms.

## 0x00B81F30

# DOSSIER 0x00B81F30 CEngineClothSimulationStaticMesh::RunSimulation (455 bytes)

## 1. Retail shape (xdis 00b81f30 0x1d7)
- thiscall, plain `ret` (no args). NO frame pointer (`sub esp,0x10; push esi`), NO SEH, speed-optimised (not /Os). No pragmas needed.
- `this` spilled to a frame slot at entry (0xB81F36 `mov [esp+8],esi`) and reloaded for ClearParticleSims in loop 1 (0xB8207B) and as the
  CParticleContext& arg of SimulationStep (0xB820AA).
- Early exit: UpdateTransform; `call [eax+0x20]` (slot 8 CalcAndSetRunState); false -> ClearParticleSims; ret (before ebx/ebp/edi pushes).
- Mesh local CCountedPointer<const C3DMesh2> at [esp+0x18]/[esp+0x1c] zeroed via ebx=0 BEFORE the LOD loop.
- LOD loop (0xB81F67..0xB81F8E) is NOT rotated: `cmp eax,4; jae` at top, `jmp` back. IDENTICAL bytes to sibling AnimatedMesh 0xB82138..0xB8215E,
  which landed with: `bool IsLODPresent(unsigned long lod) const { return lod < 4 && LODDataSizes[lod] != 0; }` and
  `long GetLoadedLOD() const { if (MeshData) { for (unsigned long i = 0; IsLODPresent(i); ++i) if (MeshData->LODMeshes[i]) return i; } return -1; }`
  -> USE THAT EXACT FORM (draft's inline `i < 4 && LODDataSizes[i]` rotates).
- GetMesh(&mesh, lod, 4) false -> inline release that TAIL-MERGES into the end-of-function release (0xB81FB6 `jmp 0xB820DA`).
- Empty ClothPrimitives (start==finish at prim+0x20/+0x24) -> `lea ecx,[esp+0x18]; call 0x00A36EC0` (out-of-line ~CCountedPointer) then ret.
- Primitive address = Primitives(+0x94) + PrimitiveIndex*0xa0 (lea x*5, shl 5), kept in ebx.
- esi becomes &ParticleSims (this+0xc) from 0xB81FF5 on.
- Loop 1 (ParticleSims empty): count=(finish-start)>>3 in ebp; resize(count) 0x00B840A0; i in frame slot [esp+0x10]; guard `test ebp,ebp; jbe`,
  bottom test `inc; cmp eax,ebp; jb` (unsigned for loop). Body: new(0x80) + ctor 0x00AE0600 (null -> 0), Reset(ParticleSims[i], p) 0x00B82A50,
  then SetupSimulation(ParticleSims[i].Data, g_ParticleMemoryPool) with ecx = ClothPrimitives[i].Data->ParticleProgram.Data; false ->
  ClearParticleSims(this from frame) and fall into loop 2 (break). Note retail loads ParticleSims[i].Data (eax) BEFORE the program (ecx).
- Loop 2: j in edi, size re-read each iteration ((finish-start)>>3), guard `test eax,eax; jbe`. Body loads sim = ParticleSims[j].Data first
  (spilled to [esp+0x10]), then cloth = ClothPrimitives[j].Data (ebp); SavePositions(sim) with ecx=[ebp+4]; then SimulationStep(sim, *this)
  with ecx=[ebp+4] reloaded (program read twice -> do NOT cache program in a local; cache the C3DClothPrimitive*).
- End: inline release (dec RefCount; ==0 -> DeleteFunc(Data) via [eax+4] with ecx=[eax+8]; operator delete 0x00BFE9BC); no Data/Info zero stores.

## 2. Name / class / signature
- Vtable: rebuild/manifest/rtti-vtables.tsv `CEngineClothSimulationStaticMesh 012a3998 slot 6 = 00b81f30` (xref: abs .rdata 012a39b0 = +0x18).
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv): 00a2ab10 `?RunSimulation@CEngineClothSimulationStaticMesh@@UAEXXZ`.
- FableWin.pdb (naming_stragglers/FableWin.exe.signatures.tsv): 017d4808 same mangled name. -> manifest label CORRECT (two sources).
- `virtual void RunSimulation();` public, non-const, no params. Mangled leaf `?RunSimulation@CEngineClothSimulationStaticMesh@@UAEXXZ`.
- Bases (ghidra_out/rtti_bases.tsv): StaticMesh : CEngineClothSimulationMeshBase : CEngineSimulationBase : CParticleContext (all @0)
  -> pass `*this` as `const CParticleContext&`, no adjustment.

## 3. Callees
| addr | landed file | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x00B81050 | 00/b8/CEngineClothSimulationMeshBase_UpdateTransform_00b81050.cpp | UpdateTransform | ?UpdateTransform@CEngineClothSimulationMeshBase@@IAEXXZ (protected) | ok |
| slot 8 (+0x20) -> 0x00B81180 | 00/b8/CEngineClothSimulationStaticMesh_CalcAndSetRunState_00b81180.cpp | CalcAndSetRunState | ?CalcAndSetRunState@CEngineClothSimulationStaticMesh@@MAE_NXZ (virtual, protected, bool) | WRONG: draft `IsSimulationEnabled` |
| 0x00B81D60 | 00/b8/CEngineClothSimulationMeshBase_ClearParticleSims_00b81d60.cpp | ClearParticleSims | ?ClearParticleSims@CEngineClothSimulationMeshBase@@QAEXXZ | ok |
| 0x00A25E70 | 00/a2/C3DMeshInfo_GetMesh_00a25e70.cpp | C3DMeshInfo::GetMesh | ?GetMesh@C3DMeshInfo@@QBE_NAAV?$CCountedPointer@$$CBVC3DMesh2@@@@KK@Z (const, REFERENCE) | WRONG: draft non-const, pointer arg |
| 0x00A36EC0 | 00/a3/Destval...COptimisedPrimitive_00a36ec0.cpp | ~CCountedPointer<CUnprovenPointee_00a36ec0> (explicit instantiation of shared header dtor) | ~CCountedPointer<const C3DMesh2> COMDAT (byte-identical across T) | draft inlined it |
| 0x00B840A0 | 00/b8/vectorEWeaponClass_UmoveEWeaponClass_00b840a0.cpp (wrong label, propagated) | _Umove<EWeaponClass> | ?resize@?$vector@V?$CCountedPointer@VCParticleSimulation@@@@...@std@@QAEXI@Z (Ego_r 00bf9f40, FableWin 01831be3) | draft `resize` ok |
| 0x00BFEA1A | operator new | | | ok |
| 0x00AE0600 | 00/ae/CParticleSimulation_CParticleSimulation_00ae0600.cpp | ??0CParticleSimulation@@QAE@XZ | same | ok |
| 0x00B82A50 | 00/b8/CCountedPointer_Reset_00b82a50.cpp | Reset | ?Reset@?$CCountedPointer@VCParticleSimulation@@@@QAEXPAVCParticleSimulation@@@Z | ok (use shared header + FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL as sibling) |
| 0x00AE0050 | 00/ae/CParticleProgram_SetupSimulation_00ae0050.cpp | SetupSimulation(CParticleSimulation*, CMovableResourceMemoryPool*) | ?SetupSimulation@CParticleProgram@@QBE_NAAVCParticleSimulation@@AAVCMovableResourceMemoryPool@@@Z (const, refs) | draft non-const/pointers (codegen-equal) |
| 0x00ADF5B0 | 00/ad/CParticleSimulationUser00adf5b0_SavePositions_00adf5b0.cpp | CParticleSimulationUser_00adf5b0::SavePositions (descriptive stand-in) | probable: ?SaveStateToInterpolateFrom@CParticleProgram@@QAEXAAVCParticleSimulation@@@Z (Ego_r 00beb250 next to Peek/GetInitialSimulation; FableWin 01816a64): copies Positions->SavedPositions, ecx=program unused. ONE source + behaviour; candidate NAME_FIX, not proven |
| 0x00AE2CB0 | 00/ae/CParticleProgram_SimulationStep_00ae2cb0.cpp | SimulationStep(CParticleSimulation&, const CParticleContext&) const | ?SimulationStep@CParticleProgram@@QBEXAAVCParticleSimulation@@ABVCParticleContext@@@Z | draft passes (CParticleContext*)this pointer |
| 0x00BFE9BC | operator delete | | | ok |
| global 0x013D2D70 | | g_ParticleMemoryPool (3 landed rows) | - | WRONG: draft `g_pParticleMemoryPool` |

## 4. Members
- this+0x0c ParticleSims: PDB `CArray<CCountedPointer<CParticleSimulation> >` (retail 12-byte STLport vector; engine/CArray.h exists, used by Render 0x00B812F0).
- this+0xa8 MeshInfo `C3DMeshInfo*`, +0xac PrimitiveIndex `ulong` (PDB 172/176 less 4 for 16->12 CArray). No engine/ header for CEngineClothSimulation*,
  CEngineSimulationBase, C3DClothPrimitive -> must be local (every landed sibling models them locally).
- C3DMeshInfo (engine/C3DMeshInfo.h): MeshData +0x0c `CIVCountedPointer<CMeshData>` (private, under FABLE_RETAIL_C3DMESHINFO_BOUNDINGBOX), LODDataSizes +0x50
  `ulong[4]`; GetMesh via FABLE_RETAIL_C3DMESHINFO_GETMESH (needs FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK, CMeshDataBankEntry base). No LOD
  helper in header; CMeshData +0x08 LOD pointers [4] (descriptive; sibling `LODMeshes`). Sibling 0x00B82100 modelled C3DMeshInfo locally.
- C3DMesh2 +0x94 Primitives `C3DPrimitive2*` (engine/C3DMesh2.h, retail Materials 0x8c asserted; PDB 156 less 8).
- C3DPrimitive2 +0x20 ClothPrimitives `CArray<CCountedPointer<C3DClothPrimitive> >` (PDB; engine/C3DPrimitive2.h has it only as opaque
  `unsigned char ClothPrimitives[12]`), sizeof 0xa0 asserted. Draft's "ParticleSystems" = ClothPrimitives.
- C3DClothPrimitive +0x04 ParticleProgram `CCountedPointer<CParticleProgram>` (PDB; Render 0x00B812F0 local model). Draft's "Def->Program".
- Draft-local classes with a shared header: CCountedPointer (engine/CCountedPointer.h), C3DMeshInfo, C3DMesh2, C3DPrimitive2 (draft
  "C3DMeshPrimitive"), CParticleSimulation (engine/CParticleSimulation.h), CParticleProgram (engine/CParticleProgram.h; Render uses
  `#define CParticleProgram CParticleProgramLayout` trick), CArray (draft CRetailVector).

## 5. Visibility
- 0x00A36EC0 called out of line at ONE exit only, the other two releases inline: sibling 0x00B82100 reproduced this with a local
  `CMeshPointer { Data; Info; void Destroy(); /*0x00A36EC0 decl only*/ FABLE_FORCEINLINE void Release(); }`, manual zeroing, explicit
  Release()/Destroy(). Shared-header alternative: FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY / _VISIBLE_CALLED_DTOR exist but would make
  ALL exits call it. Here retail also tail-merges the GetMesh-fail release into the final release (sibling did not) -> one shared release
  path (e.g. `if (GetMesh(...)) { ...body... } mesh.Release(); return;` structure or goto).
- Callees at neighbouring addresses in the same TU (defined earlier): UpdateTransform 0x00B81050, CalcAndSetRunState 0x00B81180,
  ClearParticleSims 0x00B81D60, Reset 0x00B82A50, resize 0x00B840A0 (STLport out of line). All called, none inlined; retail spills `this`
  across calls - no live-register-across-call evidence found to require visible bodies (Render 0x00B812F0 needed a visible noinline
  CalculateFadeOut for edx liveness; check if ClearParticleSims call at 0xB8207B relies on anything - it reloads ecx, so no).
- Opt-ins: FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL (Reset decl only; used by sibling 0x00B82100).

## 6. Siblings
- 0x00B82100 CEngineClothSimulationAnimatedMesh::RunSimulation (rebuild/src/compiled/00/b8/CEngineClothSimulationAnimatedMesh_RunSimulation_00b82100.cpp):
  THE template. No pragmas/flags; `#include "fable_compiler.h"`, `#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL` + engine/CCountedPointer.h,
  everything else local; GetLoadedLOD/IsLODPresent, CMeshPointer Release/Destroy, loop forms.
- 0x00B812F0 CEngineClothSimulationStaticMesh::Render: engine/CArray.h, CParticleProgramLayout define trick, local C3DClothPrimitive/C3DPrimitive2.
- 0x00B81180 StaticMesh::CalcAndSetRunState, 0x00B81D60 ClearParticleSims, 0x00B81050 UpdateTransform: local class models, no pragmas.

## 7. Old draft errors
1. LOD loop: inline `i < 4 && LODDataSizes[i]` condition -> rotated; use IsLODPresent(i) helper form (proven by sibling).
2. Uses a real destructor on `CCountedPointer` local -> all releases inline; retail calls 0x00A36EC0 at the empty-cloth exit and tail-merges
   GetMesh-fail into final release.
3. GetMesh-fail and empty exits `return` without modelling the release paths explicitly.
4. Loop 1: i/count register assignment wrong (retail count in ebp, i in frame); retail loads sim before program for SetupSimulation (name both before call as sibling).
5. Loop 2: draft loads Def then sim; retail loads sim first then cloth primitive; program re-read for each call.
6. Names: IsSimulationEnabled -> CalcAndSetRunState; g_pParticleMemoryPool -> g_ParticleMemoryPool; ParticleSystems -> ClothPrimitives;
   CParticleSystemDef -> C3DClothPrimitive / ParticleProgram; GetMesh const + reference; SimulationStep(CParticleSimulation&, const CParticleContext&) const.

## 0x008C61C0

# DOSSIER 0x008C61C0 -- CCreatureAction_TrollWhackGroundBase::WhackGround (457 bytes)

## 1. Retail shape (xdis 008c61c0 0x1d9)
- thiscall, `void()`, plain `ret` (0x008C6388). NO frame pointer (`sub esp,4Ch` then push ebx/ebp/esi/edi) -> /Oy (default /O2).
- NO SEH prologue although CCharString/CDefPointer/vector locals have dtors -> TU built without /EHsc (no EH state).
- Register roles: edi=this, esi=&PCreature (this+8), ebp=0 constant, ebx=def (CDefPointer Data) / later scratch.
- Frame (esp after pushes): +0x10 CDefPointer<const CSpecialEffectsDef> (zeroed via ebp), +0x14 CCharString effectName (ctor 0x99E4B0),
  +0x18 CCharString temp (slot-23 return), +0x1c long effect, +0x20..+0x2b diff vector (only Y spilled at +0x24),
  +0x2c normalised temp, +0x38 named direction (copied from +0x2c through eax/ecx/edx), +0x44 std::vector<CCharString> (3 dwords zeroed),
  +0x50 C3DVector helper position.
- Normalise: fsqrt (inline sqrt on X*X+Y*Y+Z*Z, sum order Z? -> computed as Z*Z kept, Y*Y(spill)+... : fld Z;fmul;fld Y;fmul;faddp;fld X? -- see bytes 0x8C6275..0x8C6287:
  st = dz*dz + dy*dy, then + dx*dx; i.e. source order likely X*X+Y*Y+Z*Z with VC scheduling), `fcom [0x0129BA3C]` = DELTA_NOTIONAL_ZERO
  global (memory operand, not literal; IcicleAttack row lesson g3), `test ah,41h ; jne zero` => `if (len > DELTA_NOTIONAL_ZERO)`,
  `fdivr [0x0122DED8]` = 1.0f/len, else C3DVector(0,0,0) stored as three `mov dword 0`.
- Control: GetSubDef fails or slot82 fails -> 0x8C6348 (reload ebx=def); GetEffectFromString fails or slot22 fails -> 0x8C634C (ebx already def).
  Quake always runs; then ~CCharString(effectName); then inline CDefPointer release `cmp ebx,0; je; dec [ebx+4]; jne; call [vtbl+4]`.

## 2. Identity (two sources)
- Ego_r.pdb names: `?WhackGround@CCreatureAction_TrollWhackGroundBase@@AAEXXZ` (egor_pdb_names.tsv 0x008FE9D0) and FableWin.pdb
  (_fablewin_api.tsv 0x017D95BF / 0x02A27F00). => PRIVATE non-virtual `void WhackGround()`. Manifest label CORRECT.
- Only caller: 0x008C6390 `jmp 008C61C0` = vtable 0x0128E0EC slot 3 PerformAction (rtti-vtables.tsv) => `PerformAction(){ WhackGround(); }`
  tail-call. Landed 0x008C6390 row is a free-function forwarder stand-in (`CCreatureAction_TrollWhackGroundBase_PerformAction`).
- Same-class vtable: slot 12 Initialise 0x8C5FD0, slot 81 InitAnimation 0x907F90, slot 82 GetEffectName 0x907FA0 (FableWin
  `?GetEffectName@CCreatureAction_TrollWhackGroundBase@@MBE_NAAVCCharString@@@Z` = protected virtual `bool GetEffectName(CCharString&) const`).
- Base chain: CCombatActionBase (retail 0x114, engine/CCombatActionBase.h) : CCreatureActionBase (0xa8). Shared header
  engine/CCreatureAction_TrollWhackGroundBase.h is a generated pad-only 0x114 layout (no virtuals) -- needs a model on CCombatActionBase.

## 3. Callees (PDB name vs old-draft spelling)
| addr | landed row / name | PDB name | draft spelling |
|---|---|---|---|
| 0x0099E4B0 | 00/99/CCharString_CCharString_0099e4b0.cpp | CCharString::CCharString() | ok |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | CCharString::~CCharString() | ok |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | CBaseIntelligentPointer::GetPItem() const (via CIntelligentPointer<T>::operator->, FABLE_RETAIL_CINTELLIGENTPOINTER) | ok-ish, but PCreature is CIntelligentPointer<CThingCreatureBase> |
| 0x00669580 | 00/66/CDefClassBase_GetSubDefCSpecialEffectsDef_00669580.cpp | `??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z` bool GetSubDef<CSpecialEffectsDef>(CDefPointer<const CSpecialEffectsDef>*) const | WRONG: "CTrollDef", non-const |
| 0x00669D30 | 00/66/CSpecialEffectsDef_GetEffectFromString_00669d30.cpp | `?GetEffectFromString@CSpecialEffectsDef@@QBE_NABVCCharString@@AAJ@Z` (const CCharString&, long&) const | WRONG: "GetWhackEffect", long* |
| vslot 82 +0x148 | 00/90/..._GetEffectName_00907fa0.cpp (stub returning false) | GetEffectName(CCharString&) const | WRONG: "GetWhackHelperName(CCharString*)" |
| vslot 22 +0x58 | CCreatureActionBase.h | IsActionWithStrikeEventHelperName() const | WRONG: "IsEffectWanted" |
| vslot 23 +0x5c | CCreatureActionBase.h | CCharString GetStrikeEventHelperName() const (by value, hidden ret at +0x18) | WRONG: "GetEffectHelperName" |
| 0x004C0560 | not landed (manifest GetHelperPoint, accepted) | `?GetHelperPoint@CTCGraphicAppearance@NGraphicAppearance@@QBE_NABVCCharString@@JKAAVC3DVector@@M@Z` | draft: no namespace, non-const, C3DVector* |
| 0x008F7F50 | 00/8f/NCombatActionHelpers_CreateEffect_008f7f50.cpp | `?CreateEffect@NCombatActionHelpers@@YIXABVCThingCreatureBase@@JABVC3DVector@@1@Z` __fastcall (creature&, long, const C3DVector& dir, const C3DVector& pos) | draft: global, CThing*, pointers |
| 0x00414EA0 | 00/41/stdvectorCCharString_vector_00414ea0.cpp | std::vector<CCharString>::~vector() | WRONG: "CEffectList" |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp (CThing::GetPos) | `?GetPos@CThing@@QBEABVC3DVector@@XZ` (CThing.h:411 declares it) | WRONG: "GetPosition" |
| 0x007F1B70 | 00/7f/CTCDRumble_Create_007f1b70.cpp | `?Create@CTCDRumble@@SIPAVCThing@@ABVC3DVector@@W4EQuakeStrength@@W4EQuakeLength@@_N@Z` | WRONG: "CQuake::Create(pos,long,long,bool)" |
- Rumble call = `CTCDRumble::Create(PCreature->GetPos(), QUAKE_STRENGTH_MEDIUM(1), QUAKE_LENGTH_SHORT(0), false)` (enums found in rebuild/include, grep EQuakeStrength).
- 1e-4f 0x0129BA3C = `DELTA_NOTIONAL_ZERO` (`extern const float`, C3DVector.h:26 under FABLE_RETAIL_C3DVECTOR); 1.0f 0x0122DED8 literal.

## 4. Members
- this+0x08 PCreature: CIntelligentPointer<CThingCreatureBase> (CCreatureActionBase.h, FABLE_RETAIL_CCREATUREACTIONBASE + _FULL_MEMBERS or _PCREATURE).
- CThing+0x60 PhysicsTC, +0x64 PTCGraphicAppearance (NGraphicAppearance::CTCGraphicAppearance*), +0x70 PBaseDef CDefPointer<const CThingBaseDef>
  (engine/CThing.h with FABLE_RETAIL_CTHING; static asserts 0x60/0x70; GetPhysicsTC(), PeekBaseDef(), GetPos()).
- CTCPhysicsBase +0x0c Position, +0x18 OldPosition (struct_layouts_egor.tsv lines 6916/6917; accessors GetPosition() const, GetLastPosition() const
  in CTCPhysicsBase.h:233/250, FableWin ?GetLastPosition@CTCPhysicsBase@@QBEABVC3DVector@@XZ).
- direction = Position(+0xc, from 2nd GetPItem) - OldPosition(+0x18, from 1st GetPItem, ebx): `(PCreature->GetPhysicsTC()->GetPosition() - PCreature->GetPhysicsTC()->GetLastPosition())`
  (VC evaluates the operator- argument first => first GetPItem is the right operand). Draft named +0xc "Target" and +0x18 "Position": WRONG names.
- Locally-modelled classes in old draft that have shared headers: CCharString (fable_string.h), C3DVector (engine/C3DVector.h), CDefPointer (engine/CDefPointer.h),
  CDefClassBase/CSpecialEffectsDef (engine/CSpecialEffectsDef.h, used by landed 0x669D30 with FABLE_RETAIL_SKYBEAM_ORIGINAL/CCHARSTRING/CDEFSTRINGTABLE/DEF_CLASS_BASE),
  CThing (engine/CThing.h), CTCPhysicsBase (engine/CTCPhysicsBase.h), CIntelligentPointer (engine/CIntelligentPointer.h), CCreatureActionBase / CCombatActionBase,
  CCreatureAction_TrollWhackGroundBase. Draft also fakes CEffectList/CTrollDef/CPhysicsState/CQuake.

## 5. Visibility
- All callees are out-of-line calls in retail (none expanded). GetSubDef<CSpecialEffectsDef> lives at 0x669580 (other TU) -> declare only, do NOT
  emit the template body here (contrast IcicleAttack 0x8C6DB0 where GetSubDef<CSnowTrollDef> 0x8C7A70 was same-TU).
- Inline-expanded here: operator- , the by-value normalise (sqrt + DELTA_NOTIONAL_ZERO + 1/len, zero vector else), CDefPointer dtor (null-tested release),
  CIntelligentPointer::operator-> (forwards to GetPItem), GetPhysicsTC/GetPosition/GetLastPosition, vector<CCharString> ctor (3 zero stores) -- its dtor is a call.
- No PDB name found for a by-value "normalised copy" of C3DVector (only Normalise/AccurateNormalise in-place, GetMagnitudeAndNormalise); C3DVector.h has
  FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE (GetAccurateMagnitude = (float)sqrt(X*X+Y*Y+Z*Z)) -- likely ingredient: `float len = diff.GetAccurateMagnitude();
  if (len > DELTA_NOTIONAL_ZERO) dir = diff * (1.0f/len) / diff / len ... else C3DVector(0,0,0)` written so the result lands in a temp then a named local.
  Note retail keeps sqrt result unrounded on st (no fstp/fld round-trip) -- watch that (float)sqrt does not force a store.
- STLport: ~vector<CCharString> 0x414EA0 out of line (declare dtor, landed row binds `??1?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@QAE@XZ`).

## 6. Siblings
- 00/8c/CCreatureActionIceTrollIcicleAttack_PerformAction_008c6db0.cpp: RELOCATION_MATCH, same lane idioms (DELTA_NOTIONAL_ZERO global, CDefPointer,
  CIntelligentPointer FABLE_FORCEINLINE operator->, GetPItem 0xA01B50, PhysicsTC Position/OldPosition, GetGraphicAppearanceTC()). No pragmas/flags line. Best template.
- 00/8c/CCreatureActionTrollThrowProjectile_FrameUpdate_008c67a0.cpp: FABLE_RETAIL_CTHING, FABLE_RETAIL_CTHING_GETTC_EXPANDED, FABLE_RETAIL_CINTELLIGENTPOINTER.
- 00/66/CSpecialEffectsDef_GetEffectFromString_00669d30.cpp: shared engine/CSpecialEffectsDef.h opt-ins.
- 00/8c/CCreatureActionJackFloatUp_PerformAction_008c0fb0.cpp, CCreatureActionNymphGetHit_PerformAction_008ca310.cpp: same GetSubDef<CSpecialEffectsDef>+GetEffectFromString
  call pair but with local stand-ins (do not copy their models).
- 00/8f/NCombatActionHelpers_CreateEffect_008f7f50.cpp (callee signature), 00/7f/CTCDRumble_Create_007f1b70.cpp (/O2 /Oy /W3 /Gr flags line, enums).

## 7. Old-draft model errors
- Wrong names for 8 callees (table in 3) and swapped physics member names (Target/Position vs Position/OldPosition).
- Unused local is std::vector<CCharString>, not an "effect list" (declared inside the inner block, before the direction).
- CreateEffect takes references and a CThingCreatureBase&; GetHelperPoint takes C3DVector& and is const; GetEffectFromString takes long&.
- DELTA_NOTIONAL_ZERO must be the extern global (draft used literal 0.0001f -> `fcom` still memory but pooled; use global as IcicleAttack did).
- Residue history (hand262 NOTES): 456v457, frame 0x40 vs 0x4c, this in ebx vs edi; retail has three distinct 12-byte slots (diff, normalised temp, named direction)
  and copies temp->direction through integer regs => by-value return function + named `C3DVector direction = ...` with a user-visible copy (try header
  copy-ctor variant FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST-style `C3DVector(const C3DVector&)`), and a named diff local.

## 0x006D0630

# DOSSIER 0x006D0630 CTCWife::ReceivedNiceGiftIsOpinionReactionHappening (459 bytes)

## 1. Retail shape (xdis 006d0630 0x1db)
- __thiscall, `ret 4` (one arg), returns bool in al. NO frame pointer (`sub esp,1Ch ; push ebx/ebp/esi/edi`) -> /Oy on, default /O2 (no "y" off pragma).
- No SEH frame (no fs:0) even though two CDefPointer locals have dtors -> nothing between ctor/dtor can throw in EH terms; plain /O2 TU.
- Locals: [esp+18h] objectDef handle, [esp+14h] giftDef handle (both zeroed first), [esp+13h] result byte (=1 then 0).
  Epilogue releases giftDef (0x14) THEN objectDef (0x18) (declaration order objectDef, giftDef), inline CDefPointer dtor `dec [ecx+4]; jne; call [vtbl+4]`.
- this kept in esi, def index in ebp (loaded from [esp+30h]); the index arg slot [esp+30h] is later REUSED as the 0x51 key temp for LowerBound.
- Block layout: nice/married test -> deed block (ebx = deed type) -> shared release/return -> ring tail (0x6d0765, jumps back into the deed block SKIPPING `result=0`) -> not-ring branch (find/insert, 0x6d0776) placed LAST -> its "deed 0x52" tail jumps to 0x6d06b8 (the `result=0` store).
- The "is wedding ring" bool is materialised (`mov al,1 / xor al,al ; test al,al`) -> a named bool local (or an inline function returning bool) evaluated before the `result=true` store.
- `mov byte [esp+13h],1` scheduled between `test al,al` and `je` -> `bool result = true;` declared after the ring bool.

Control flow (exact):
```
objectDef, giftDef zero; ring = GetWorld().DefinitionManager->GetDef(index,&objectDef) && objectDef->GetSubDef(&giftDef) && giftDef->IsWeddingRing;
result = true;
if (ring && FrameGotMarriedToThePlayer > CWorld::GetFrame()) {     // jle -> else-branch
    if (IsCourtingHero() && OpinionAttitude == 12) { ReceivedWeddingRing = true; deed = 0x3F; /* result stays true */ }
    else { deed = 0x4A; result = false; }
  post: hero = PeekWorld().PeekMyPlayer()->GetPControlledCreature();
    if (hero && !hero->IsDead() && hero->HasTC(0x51)) { log = GetTC(0x51) (LowerBound 0x40F020 inline);
        log->PostDeed(deed, hero->GetPos(), ParentThing, ParentThing, 1, false,false,0,false,0); }
} else {
    it = GiftsReceived.find(index);                    // 0x5937E1 out of line, hidden iterator result
    if (it == end()) { GiftsReceived.insert(pair(index, GetFrame())); FrameLastReceivedNiceGift = GetFrame(); }
    else if (GetFrame() - it->second > PHeroMarriageDef->SecondsBetweenReceivingGiftOpinionReactionCanHappen * CMainGameComponent::GetConstantFPS())
        { it->second = GetFrame(); FrameLastReceivedNiceGift = GetFrame(); }
    else { deed = 0x52; result = false; goto post; }
}
return result;
```
Note PostDeed position is the HERO's GetPos (ecx=edi), target and victim both ParentThing ([esi+4], pushed twice).

## 2. Name / signature (two sources)
- ghidra_out/egor_signatures.tsv / egor_pdb_names.tsv: Ego_r 0x00704610 `?ReceivedNiceGiftIsOpinionReactionHappening@CTCWife@@QAE_NJ@Z`.
- FableWin.pdb (_fablewin_api / naming_stragglers): 0x0248D890 same mangling; a matcher row maps 6d0630 -> it (score 0.949, true).
- Mangled leaf: `?ReceivedNiceGiftIsOpinionReactionHappening@CTCWife@@QAE_NJ@Z` = public, non-const, `bool (long)`. Manifest label is CORRECT.
- Not in CTCWife vtable (rtti-vtables.tsv no hit); not declared in engine/CTCWife.h (author must add a declaration / opt-in branch).
- Callers (xref): 0x00844D68 and 0x0084521A (CCreatureActionGiveMoneyToThing::PerformAction 0x00845040 calls it with -1:
  `wife->IsMarriedToThePlayer() && !wife->ReceivedNiceGiftIsOpinionReactionHappening(-1)`; that file declares it locally line 310).

## 3. Callees (landed file / landed name / PDB)
| addr | landed file | name | note |
|---|---|---|---|
| 0x00686A70 | 00/68/CTCBase_GetWorld_00686a70.cpp | `?GetWorld@CTCBase@@IBEAAVCWorld@@XZ` | CTCBase.h:160 `CWorld& GetWorld() const`. Draft called it "GetDefWorld" -> WRONG name. Then +0x10 read inline = CWorld::DefinitionManager (CWorld.h:135 PeekDefinitionManager inline; opt-in FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER). NOT CTCBase::GetDefinitionManager (that is out of line 0x006869D0). |
| 0x0042AF3C | 00/42/CDefinitionManager_GetDefT_0042af3c.cpp | CDefinitionManager::GetDef<T>(long, CDefPointer<const T>*) | folded body (13 instantiations). Sibling CGameScriptInterface_IsGiftRomantic_00894bf0 binds it as GetDef<CThingBaseDef> via FABLE_RETAIL_CDEFINITIONMANAGER. Draft "GetObjectDef" -> wrong. |
| 0x006D3EF0 | 00/6d/CDefClassBase_GetSubDefCGiftDef_006d3ef0.cpp | `??$GetSubDef@VCGiftDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCGiftDef@@@@@Z` | draft "GetGiftDef" -> wrong. Use `def->GetSubDef(&gift_def)` (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY in the IsGiftRomantic sibling). |
| 0x0049D870 | 00/49/CWorld_GetFrame_0049d870.cpp | `?GetFrame@CWorld@@SIJXZ` static __fastcall | draft "GetWorldFrame" -> wrong. |
| 0x006CF890 | 00/6c/CTCWife_IsCourtingHero_006cf890.cpp | `?IsCourtingHero@CTCWife@@QBE_NXZ` | ok. |
| (inline) 0x006CF900 | 00/6c/CTCWife_IsInLoveWithHero_006cf900.cpp | `?IsInLoveWithHero@CTCWife@@QBE_NXZ` | body = `IsCourtingHero() && OpinionAttitude==12` -- EXACTLY the inline test at 0x6d069e..0x6d06ad. Same class/TU, earlier address -> visible; retail inlined it here. Spell as `IsInLoveWithHero()` (needs an inline body visible in the TU). |
| 0x00686A80 | 00/68/CTCBase_PeekWorld_00686a80.cpp | `?PeekWorld@CTCBase@@IBEABVCWorld@@XZ` | CTCBase.h:150 `const CWorld& PeekWorld() const`. Draft "GetWorld" (returning CWorldPlayers*) -> wrong. |
| 0x0049D9D0 | 00/49/CWorld_PeekMyPlayer_0049d9d0.cpp | `?PeekMyPlayer@CWorld@@QBEPBVCPlayer@@XZ` | CWorld.h:267. Draft "GetCurrentPlayer" -> wrong. |
| 0x00487DC0 | 00/48/CPlayer_GetPControlledCreature_00487dc0.cpp | `?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ` | draft "GetCharacterThing" -> wrong. |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound | reached via inline CThing::GetTC<T>. |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | draft "GetPosition" -> wrong (PDB GetPos). |
| 0x006C6990 | (not landed) | `?PostDeed@CTCHeroOpinionDeedLog@@QAEJW4EOpinionDeedType@@ABVC3DVector@@PBVCThing@@PAVCThingAICreature@@W4EOpinionPostDeedType@@_N5PBVCFaction@@5J@Z` | engine/CTCHeroOpinionDeedLog.h (opt-in FABLE_RETAIL_CTCHEROOPINIONDEEDLOG, has StatGetInterfaceType = TCI_HERO_OPINION_DEED_LOG). |
| 0x005937E1 | 00/59/stdRbtree..._find_005937e1.cpp | `??$find@J@?$_Rb_tree@J,pair<const long,long>,...>@std@@QAE?AU?$_Rb_tree_iterator...@ABJ@Z` (member TEMPLATE find<long>) | out of line, `ret 8` hidden result. Opt-in FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE (fable_map.h:201) gives the template spelling; map::find forwards. Draft's "gift map find" ok; manifest label SetMiniMapRegionExitTextOffsetX wrong (draft already says). |
| 0x004E539F | 00/4e/CRetailTreelongkeyins004e5425_insert_004e539f.cpp | _Rb_tree<long,pair<const long,long>,...>::insert_unique(const V&) | called out of line as `GiftsReceived.insert(pair)` (map::insert -> insert_unique, fable_map.h:458). Sibling CTCWife_OnSerialise_006d2bd0 calls the same: `GiftsReceived.insert(std::make_pair(..))`. Do NOT enable FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY-style visibility (keep the call). |

## 4. Members
- CTCWife (engine/CTCWife.h, opt-in FABLE_RETAIL_CTCWIFE; retail sizeof 0xEC, offsets after the map are PDB-4):
  +0x04 ParentThing (CTCBase; inline GetParentThing() exists, CTCBase.h:138), +0x0C `CDefPointer<const CHeroMarriageDef> PHeroMarriageDef`, +0x10 `EOpinionAttitudeType OpinionAttitude`,
  +0x20 `long FrameGotMarriedToThePlayer`, +0x78 `std::map<long,long> GiftsReceived`, +0x84 `long FrameLastReceivedNiceGift`, +0x91 `bool ReceivedWeddingRing`.
- CHeroMarriageDef (engine/CHeroMarriageDef.h) +0x48 `long SecondsBetweenReceivingGiftOpinionReactionCanHappen` (draft "SecondsBetweenNiceGiftReactions" WRONG).
- CGiftDef (engine/CGiftDef.h) +0x2C `bool IsWeddingRing` (draft "IsNiceGift" WRONG; +0x28 is EGiftType Type).
- CWorld +0x10 DefinitionManager (CGameDefinitionManager*) (CWorld.h:135).
- CThing (engine/CThing.h, FABLE_RETAIL_CTHING): +0x91 Dead bit -> IsDead(); +0x20 UsingComponents bitset (HasTC: `test [edi+28h],20000h` = bit 0x51); +0x44 Components CVectorMap -> GetTC<T>(&out) template (forceinline). Murdered 0x006D00A0 uses `hero->GetTC(&deedLog)`; OnGreet uses FABLE_RETAIL_CTHING_GETTC_EXPANDED.
- Global 0x01375550 = `CMainGameComponent::ConstantFPS` via inline `CMainGameComponent::GetConstantFPS()` (fable_game.h:73 / CMainGameComponent.h) -- draft "g_FramesPerSecond" WRONG.
- Deed enum (engine/EOpinionDeedType.h): 63 OPINION_DEED_TYPE_RECEIVE_WEDDING_RING (0x3F), 74 (0x4A) OPINION_DEED_TYPE_HIGH_PRIORITY_INCOMPREHENSION, 82 (0x52) OPINION_DEED_TYPE_WIFE_GIFT_RECEIVE_ALREADY_GOT. Post type 1.
- OpinionAttitude 12: siblings define `enum EOpinionAttitudeType { OPINION_ATTITUDE_LOVE = 12 };` (CTCWife.h forward-declares the enum, row defines enumerators).
- Draft LOCAL models that have shared headers: C3DVector, CDefBase/CDefPointer (engine/CDefPointer.h), CGiftDef, CThingObjectDef (use CThingBaseDef + CDefClassBase::GetSubDef), CDefinitionManager, CWorld, CHeroMarriageDef, CThing (+CInterfaceMap = CVectorMap), CTCHeroOpinionDeedLog, CPlayer, CTCWife, the whole gift map (std::map<long,long> via stlport/fable_map.h), EOpinionDeedType.

## 5. Callee visibility
- Visible/inline in retail: IsInLoveWithHero (0x6CF900, same TU, earlier) -> inlined; CThing::IsDead/HasTC/GetTC<T> (forceinline), CDefPointer ctor/dtor, CWorld::PeekDefinitionManager (+0x10), CMainGameComponent::GetConstantFPS, CTCBase::GetParentThing.
- Out of line: GetWorld/PeekWorld (CTCBase, 9-byte bodies but called), CWorld::GetFrame (called 6x, never folded), PeekMyPlayer, GetPControlledCreature, GetDef<T>, GetSubDef<CGiftDef>, IsCourtingHero, LowerBound, GetPos, PostDeed, tree find<long> (template), insert_unique.
- Opt-ins: FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE (find<long> call), FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER, FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY (used by IsGiftRomantic; check it does not inline GetSubDef here -- retail calls it), FABLE_RETAIL_CTHING_HASTC_FORCEINLINE (CThing.h:301) if HasTC fails to inline.

## 6. Sibling landed rows (copy idioms)
- 00/6d/CTCWife_Murdered_006d00a0.cpp: shared-header model of the identical hero/deed-log tail. Defines CCHARSTRING, CCLASS, DEF_CLASS_BASE, CTCBASE, CTHING, CINTELLIGENTPOINTER, CTHINGPHYSICAL, CTHINGGAMEOBJECT, CTHINGCREATUREBASE, CTHINGPLAYERCREATURE, CTHINGAICREATURE, PLAYER, CPLAYERMANAGER, CGAMECOMPONENT, CMAINGAMECOMPONENT, SCOPED_PTR, CWORLD, CTCWIFE, CTCWIFE_MARRIAGEEND...; `CTCHeroOpinionDeedLog* deedLog; if (hero->GetTC(&deedLog)) deedLog->PostDeed(...)`; `if (!hero || hero->IsDead()) return;`. Flags: none.
- 00/6c/CTCWife_OnGreet_006cfc00.cpp: PostDeed with ParentThing, ParentThing, (EOpinionPostDeedType)1 -- same argument pattern as here.
- 00/89/CGameScriptInterface_IsGiftRomantic_00894bf0.cpp: `CDefPointer<const CThingBaseDef> def; if (GetDef(index,&def)) { CDefPointer<const CGiftDef> gift_def; if (def->GetSubDef(&gift_def)) ... }` on shared headers (defines BASE_OBJECT, DEF_CLASS_BASE, CCHARSTRING, CDEFSTRINGTABLE, STLP_LIST, CDEFINITIONMANAGER, CGAMEDEFINITIONMANAGER, CRESOURCELIST, CRESOURCE, CDEFCLASSBASE_GETSUBDEF_BODY).
- 00/6d/CTCWife_OnSerialise_006d2bd0.cpp: GiftsReceived.insert(std::make_pair(..)) -> 0x4E539F; FABLE_RETAIL_CTCWIFE + STLP opt-ins.
- 00/6c/CTCWife_GetAppearanceChangedNoticed_006ced50.cpp: `now - x < PHeroMarriageDef->... * CMainGameComponent::GetConstantFPS()` idiom (same imul-from-global shape).
- 00/6e/CGameScriptInterface_DeregisterTimer_006e7690.cpp: map<long,long>::find -> 0x5937E1 template find.

## 7. Old draft model errors
- Wrong names everywhere (see 3/4): GetDefWorld, GetObjectDef, GetGiftDef, IsNiceGift, GetWorldFrame, GetCurrentPlayer, GetCharacterThing, GetPosition, g_FramesPerSecond, SecondsBetweenNiceGiftReactions.
- Wrong member type: PHeroMarriageDef modelled as raw pointer (real: CDefPointer<const CHeroMarriageDef>, `->` reads [+0xC] then +0x48 -- fine either way bytes-wise but use header).
- `IsCourtingHero() && OpinionAttitude == 0xc` should be the inline IsInLoveWithHero().
- Gift map modelled as fake SGiftNode/CGiftMap with `insert(SGiftPair)`; real is std::map<long,long> find (template, out of line) + insert(make_pair / value_type).
- Residue "block order: retail puts the not-ring branch after the shared block; this in esi, index in ebp": try `if (!ring || FrameGotMarriedToThePlayer <= GetFrame()) { map branch }` inverted / the map branch as the else with the deed post in a shared tail; also note result=true must be stored AFTER the ring bool is computed and the ring arm skips `result=false` (draft gets this). Ring store retail is `mov byte [esi+91h],1` immediate (draft got bl reuse) -- a `bool` member store from the header may fix it.
- Hero position: draft correctly uses hero->GetPos(); keep.

## 0x0083D990

# DOSSIER 0x0083D990 CThingPhysicalSwitch::ConstructFromLevelScript (460 bytes)

## 1. Retail shape (xdis 0083d990 0x1dc)
- thiscall, `ret 4` (both exits 0x83DA6A / 0x83DB59), returns bool in al (`mov al,1` / `xor al,al`).
- NO frame pointer (`sub esp,34h; push ebx; push ebp; push esi`), this in ebp, script in esi. No SEH (no /EHsc frame; temporaries dtor'd inline).
- NOT size-optimised: loop top 0x83D9F0 aligned with `lea ebx,[ebx]` (6-byte nop) -> /O2-style, no optimize("s").
- strcmp intrinsic: null-arm literal compares folded to `repe cmpsb` ("" vs "TriggeredByThing" len 0x11, vs "EndThing" len 9, vs "NULL" len 5) -> needs `strcmp` intrinsic (<string.h> + /Oi or `#pragma intrinsic(strcmp)`).
- Stack slots (base = esp after the 3 pushes): parser @ base+0x18 (0x28 bytes: Filename +0x24, TempCharString +0x28, Comments list +0x2C); token/item @ base+0x0C; ";" temp @ base+0x10; ternary "" temp @ base+0x14; filename temp CCharString("") and the TriggeredByThing VALUE both live in the dead `script` arg slot base+0x44.
- Dead store `mov [esp+14h],ebx(0)` at entry: slot later reused by the ternary "" temp; conditional-temp flag lives in ebx bit0 (`or ebx,1` / `test bl,1; and ebx,-2`).
- Flow: if (!CThingGameObject::ConstructFromLevelScript(script)) return false; Triggered=false (byte [ebp+0D0h], scheduled between the ctor arg pushes); CStringParser parser(script) [pushes 0, CCharString("",-1), 0, script = header default args]; rotated while(NextItemExists) (tested before loop and at bottom); edi pushed only inside loop.
  body: item = ReadAsString(); item.PStringData cached in esi across BOTH compares; if (item=="TriggeredByThing") { value = ReadAsString(); TriggerableObjectScriptName = (value=="NULL") ? CCharString("") : value; ~value } else if (item=="EndThing") -> ~item, pop edi, falls into the shared loop-exit (parser dtor, return true) == `break`; then SkipPastString(";") (temp ";"), ~item.
  Exit after loop: ~list 0x4046B0, ~TempCharString, ~Filename, ~CBaseClass 0x99A2E0 (compiler-generated ~CStringParser expanded), return true.

## 2. Identity (two sources)
- Name: CThingPhysicalSwitch::ConstructFromLevelScript, mangled `?ConstructFromLevelScript@CThingPhysicalSwitch@@UAE_NPBD@Z` (public virtual, bool, `const char*` param).
  Sources: ghidra_out/_fablewin_api.tsv 0x01794433 and naming_stragglers/FableWin.exe.signatures.tsv 0x01794433; rebuild/manifest/rtti-vtables.tsv: CThingPhysicalSwitch vtable 0x01245644 slot 4 = 0x0083D990 (xref: only reference is .rdata 0x01245654), slot 4 = ConstructFromLevelScript in every CThing vtable (CThing.h:191 "slot 4"). Manifest label is RIGHT.
- Draft's param `char*` is WRONG: must be `const char* script`.
- Leaf for row header: `// Leaf: ?ConstructFromLevelScript@CThingPhysicalSwitch@@UAE_NPBD@Z`

## 3. Callees
| addr | landed file | correct name (PDB) | draft spelling |
|---|---|---|---|
| 0x00838930 | 00/83/CThingPhysical_ConstructFromLevelScript_00838930.cpp (misnamed jmp-forwarder stub) | CThingGameObject::ConstructFromLevelScript(const char*) ?ConstructFromLevelScript@CThingGameObject@@UAE_NPBD@Z (CThingGameObject vtable 0x01244F7C slot 4; body = `jmp 0x6A5D90` CThingPhysical::ConstructFromLevelScript). Declared by FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT (CThingGameObject.h:82) | OK name, but param char* -> const char* |
| 0x0099EBF0 | 00/99/CCharString_Constructor_0099ebf0.cpp | CCharString(const char*, long=-1) | OK |
| 0x00414E30 | 00/41/CStringParser_CStringParser_00414e30.cpp | CStringParser(const char*, void(__fastcall*)(const CCharString&)=0, const CCharString& filename=CCharString(""), long=0) (CStringParser.h:48) | draft: plain cdecl fn ptr typedef + char* -> WRONG |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | OK |
| 0x009BA4F0 | 00/9b/CStringParser_NextItemExists_009ba4f0.cpp | ?NextItemExists@CStringParser@@QAE_NXZ | OK |
| 0x009BA6F0 | 00/9b/CStringParser_ReadAsString_009ba6f0.cpp | ?ReadAsString@CStringParser@@QAE?AVCCharString@@XZ | OK |
| 0x004115A0 | 00/41/CBasicStringchar_CBasicStringCharoperatorequals_004115a0.cpp | ??8?$CBasicString@D@@QBE_NPBD@Z (CBasicString<char>::operator==(const char*) const) | draft models a local `CBasicStringChar` struct -> WRONG class spelling; use fable_string.h CBasicString<char> |
| 0x0099EFB0 | 00/99/CCharString_operator_0099efb0.cpp | CCharString::operator=(const CCharString&) | OK |
| 0x009B9BA0 | 00/9b/CStringParser_FindKeyword_009b9ba0.cpp (file is junk/mislabelled) | ?SkipPastString@CStringParser@@QAE_NABVCCharString@@@Z (_fablewin_api 0x01824A38; CStringParser.h:17; TrackNode/AICreature rows call it SkipPastString) | draft `FindKeyword` -> WRONG, rename to SkipPastString |
| 0x004046B0 | 00/40/stdlistCStringParserCommentBoundaries_list_004046b0.cpp | std::list<CStringParserCommentBoundaries>::~list (implicit, via ~CStringParser) | draft local CStringParserCommentList -> WRONG |
| 0x0099A2E0 | 00/99/CBaseClass_CBaseClass_0099a2e0.cpp | ??1CBaseClass@@UAE@XZ | draft calls it ~CBase -> WRONG (CBaseClass) |
Strings: "" 0x0122D70E, "TriggeredByThing" 0x012730B0, "EndThing" 0x01239B8C, "NULL" 0x012393E4, ";" 0x0123542C.

## 4. Members
- this+0xD0 `bool Triggered` (protected) and this+0xD4 `CCharString TriggerableObjectScriptName` (protected): rebuild/include/engine/CThingPhysicalSwitch.h (unconditional), sizeof 0xD8. struct_layouts_egor.tsv: CThingPhysicalSwitch Triggered +232 / TriggerableObjectScriptName +236 (Ego_r debug layout, retail is 0x18 lower, as Save 0x0083D800 notes). Draft names `TriggeredBySet` / `TriggeredByThing` are WRONG.
- Header does NOT declare ConstructFromLevelScript: existing opt-ins are only _PEEKDEFINITION, _TRIGGER, _VSLOTS, _PEEKDEFINITION_SAMETU. Author needs a new nested opt-in (e.g. FABLE_RETAIL_CTHINGPHYSICALSWITCH_LEVELSCRIPT: `virtual bool ConstructFromLevelScript(const char* script); // 0x0083D990`, declaration only, public) -- or CThingGameObject's virtual decl under FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT plus an override decl.
- CStringParser (engine/CStringParser.h, needs FABLE_RETAIL_CSTRINGPARSER): String +4, BaseLineOffset +8, Filename +0xC, TempCharString +0x10, std::list Comments +0x14, ErrorCallback(__fastcall) +0x18, CursorPos +0x1C; base CBaseClass; size 0x28; implicit dtor.
- Locally modelled classes in the old draft that have shared headers: CCharString/CCharStringData/CBasicString (fable_string.h, FABLE_RETAIL_CCHARSTRING), CBase(=CBaseClass, engine/CBaseClass.h), CStringParser + CCursorPos + comment list (engine/CStringParser.h), CThingGameObject (engine/CThingGameObject.h), CThingPhysicalSwitch (engine/CThingPhysicalSwitch.h).

## 5. Callee-body visibility
- CCharString::operator==(const char*) is expanded in place, both arms (null arm = strcmp intrinsic vs "", non-null arm = call 0x004115A0 with ecx = PStringData): opt-in FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ (fable_string.h:404-409), as TrackNode 0x00837E40 uses.
- CBasicString<char>::operator==(const char*) stays OUT OF LINE (call 0x004115A0): keep default declaration (do NOT define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE). TrackNode defines the template body row-locally and VC still calls it out of line -- optional.
- ~CStringParser: compiler-generated, expanded (~list call 0x004046B0 is out of line, the CCharString dtors are calls, ~CBaseClass call). Comes free from the header class.
- CThingGameObject::ConstructFromLevelScript 0x00838930 is in another TU (CThingGameObject), called by qualified name -> declaration only.
- Same-TU neighbours (CThingPhysicalSwitch.cpp): PeekDefinition 0x0083D900, SerialiseGameState 0x0083D940, this row 0x0083D990, Create 0x0083DBE0, TriggerMe 0x0083DDC0; none of them is called here.

## 6. Sibling rows to copy
- BEST: rebuild/src/compiled/00/83/CThingTrackNode_ConstructFromLevelScript_00837e40.cpp (identical idiom: `if (CThingGameObject::ConstructFromLevelScript(script)) { ... CStringParser parser(script); while (parser.NextItemExists()) { CCharString key = parser.ReadAsString(); if (key == "...") {...} else if (key == "EndThing") { break; } parser.SkipPastString(";"); } return true; } return false;`). Defines: FABLE_RETAIL_CCLASS, _DEF_CLASS_BASE, _CCHARSTRING, _CCHARSTRING_FORCEINLINE_TEXT_EQ, _CINTELLIGENTPOINTER, _CTCBASE, _CTHING, _CTHING_LEVELSCRIPT, _CTHINGPHYSICAL, _CTHINGPHYSICAL_LEVELSCRIPT, _CTHINGGAMEOBJECT, _CTHINGGAMEOBJECT_LEVELSCRIPT, _CSTRINGPARSER (+ class-specific). No pragmas.
- rebuild/src/compiled/00/83/CThingAICreature_ConstructFromLevelScript_008315c0.cpp (same parser vocabulary).
- rebuild/src/compiled/00/83/CThingPhysicalSwitch_PeekDefinition_0083d900.cpp (same class, shared-header macro set incl. FABLE_RETAIL_CTHINGPHYSICAL, _CTHINGGAMEOBJECT, _CTHING_MOREACCESS).
- rebuild/src/compiled/00/83/CThingPhysicalSwitch_Save_0083d800.cpp (same TU; `#pragma intrinsic(strcmp)`; same "NULL"/"" conditional-temp idiom in reverse; but uses an older local model -- do not copy that part).

## 7. Old draft model errors
- Param `char*` -> `const char*`; local structs for every class (strict verifier rejects); wrong member names (TriggeredBySet/TriggeredByThing -> Triggered/TriggerableObjectScriptName); wrong callee names (FindKeyword -> SkipPastString, CBase -> CBaseClass, CBasicStringChar -> CBasicString<char>); error callback typedef not __fastcall(const CCharString&).
- `else if (item == "EndThing") return true;` inside the loop: retail exits through the SAME block as the loop-end (~item, pop edi, then the shared parser teardown at 0x83DA3E) -- write `break;` (TrackNode idiom) not return; the parked note blamed "EndThing block layout", which this likely fixes.
- Explicit `CStringParser parser(script, 0, CCharString(""), 0)`: use the header default args `CStringParser parser(script);`.
- Slot packing (value in script slot, token in frame) follows naturally from `CCharString value = parser.ReadAsString();` scoped inside the TriggeredByThing block and `CCharString item` scoped in the loop body; keep both block-scoped. `Triggered = false;` before the parser declaration.

## 0x0072EB40

# DOSSIER 0x0072EB40 -- CTCActionUseBuyHouse::ReceivedConfirmation (463 bytes)

## 1. Retail shape (xdis 0072eb40 0x1df)
- __thiscall, `ret 4` (one stack arg), NO frame pointer (starts `push ecx ; mov eax,[ecx+1Ch]`), no SEH -> plain /O2 TU,
  no `optimize("y",off)`/`("s")` needed. `push ecx` = one 4-byte local (the ETCInterfaceType key copy for LowerBound, `mov [esp+8],4`
  stored AFTER the presence test, i.e. GETTC_EXPANDED shape). esi/edi saved.
- switch on HouseQuestion (+0x1C) compiled as `dec eax` chain: 1 -> 0x72EBE0 (buy/sell), 2 -> 0x72EBC8 (rent toggle), 3 -> 0x72EB57 (dress), default -> epilogue.
- Every arm returns with its own `pop edi/pop esi/pop ecx/ret 4` (tail duplicated); case 3 first in layout.
- Case 2: `SetRented(!house.Rented)` -- `mov dl,[ecx+12h]; test dl,dl; sete dl; push edx; call 0x6C1150`.
- Case 3: hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature(); `if(hero && hero->HasTC(4))` (test byte [eax+20h],10h),
  LowerBound inline-GetTC; then stats; `push 1` (AddMoney's bool), house loaded into edi AFTER the lookup (`mov edi,[esp+10h]`),
  GetPriceForNextDressLevel(), `neg`, AddMoney(-price, true); IncrementDressLevel().
- Case 1 not-owned (house->OwnedByPlayer +0x11 == 0): AddMoney(-GetPrice(), true); SetOwnedByPlayer(true);
  `mov esi,[esi+4]` (house ParentThing) `test dword ptr [esi+28h],80000h` (= UsingComponents bit 83 = TCI_SHOP) -> ++[edi+0E0h] else ++[edi+0E4h].
- Case 1 owned: AddMoney(+GetPrice(), true) (no neg); SetOwnedByPlayer(false); `if (house.Rented) SetRented(false)`; same shop test -> --[+0E0h] / --[+0E4h].
- `this` stays in ecx for every call 0x686D40 (CTCBase::GetPlayerManager is a member call on this, not a free function).

## 2. Identity (two sources)
- Ego_r.pdb (egor_pdb_names.tsv 0x00747160) and FableWin.pdb (_fablewin_api.tsv / naming_stragglers 0x01846476):
  `?ReceivedConfirmation@CTCActionUseBuyHouse@@AAEXAAVCTCBuyableHouse@@@Z` -> **private**, `void ReceivedConfirmation(CTCBuyableHouse& house)` (non-const reference, non-const method). Manifest label is RIGHT.
- Ego_r order: ... OnActionUseRent 0x746E00, OnActionUseMain 0x746F30, ReceivedConfirmation 0x747160, OnActionUse 0x747380, GlobalUpdate 0x7474D0;
  retail same order: OnActionUseRent 0x72E810, OnActionUseMain 0x72E940, **0x72EB40**, OnActionUse 0x72ED10, GlobalUpdate 0x72EE30 (one TU).
- Caller: GlobalUpdate 0x0072EE30 (call at 0x72EF00); its landed comment spells it with a pointer `(CTCBuyableHouse*)` -- PDB says reference (same bytes).
- Class layout (struct_layouts_egor.tsv CTCActionUseBuyHouse, 32 bytes): PDef +0x14 (CDefPointer<CBuyHouseDef const>), WaitingForConfirmation +0x18 (bool),
  HouseQuestion +0x1C (EHouseQuestion). No engine/CTCActionUseBuyHouse.h exists; landed siblings model the class locally deriving CTCBase.

## 3. Callees (landed file / PDB leaf / draft disagreement)
| addr | landed row | PDB leaf | draft |
|---|---|---|---|
| 0x00686D40 | 00/68/CTCBase_GetPlayerManager_00686d40.cpp | ?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ (protected, returns ref) | WRONG: free `GetPlayerManager()` returning ptr |
| 0x00449970 | 00/44/CPlayerManager_GetMainPlayer_00449970.cpp | ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ (non-const, ref) | returns ptr |
| 0x00487DC0 | 00/48/CPlayer_GetPControlledCreature_00487dc0.cpp | ?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ (const) | WRONG name `GetHeroCreature` |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound(const ETCInterfaceType&) non-const | draft local CInterfaceMap |
| 0x006C0120 | 00/6c/CTCBuyableHouse_GetPrice_006c0120.cpp | ?GetPrice@CTCBuyableHouse@@QBEJXZ (const, NO args) | WRONG `GetPrice(bool)` |
| 0x006BF920 | 00/6b/CTCBuyableHouse_GetPriceForNextDressLevel_006bf920.cpp | ?GetPriceForNextDressLevel@CTCBuyableHouse@@QBEJXZ (const, NO args) | WRONG `(bool)` |
| 0x006C04B0 | 00/6c/CTCBuyableHouse_IncrementDressLevel_006c04b0.cpp | ?IncrementDressLevel@CTCBuyableHouse@@QAEXXZ | ok |
| 0x006C1150 | 00/6c/CTCBuyableHouse_SetRented_006c1150.cpp | ?SetRented@CTCBuyableHouse@@QAEX_N@Z | ok |
| 0x006C17B0 | 00/6c/CTCBuyableHouse_SetOwnedByPlayer_006c17b0.cpp | ?SetOwnedByPlayer@CTCBuyableHouse@@QAEX_N@Z | ok |
| 0x0057B327 | 00/57/CTCHeroStats_AddMoney_0057b327.cpp | ?AddMoney@CTCHeroStats@@QAEXJ_N@Z = AddMoney(long, bool) | WRONG `AddMoney(long)`: the `push 1` is AddMoney's bool=true, not a GetPrice arg |
No vtable calls.

## 4. Members
- this+0x1C HouseQuestion (EHouseQuestion) -- local model (no header); siblings: `enum EHouseQuestion { HOUSE_QUESTION_NONE=0, HOUSE_QUESTION_BUY=1, HOUSE_QUESTION_RENT=2 }` (OnActionUseRent/Main); 3 = dress (OnActionUseDress comment).
- house+0x04 ParentThing: engine/CTCBase.h (protected CThing* ParentThing; public inline `GetParentThing()` FableWin ?GetParentThing@CTCBase@@QAEAAVCThing@@XZ, under FABLE_RETAIL_CTCBASE). Must use house.GetParentThing() (protected member of another object).
- house+0x11 OwnedByPlayer, +0x12 Rented: engine/CTCBuyableHouse.h, FABLE_RETAIL_CTCBUYABLEHOUSE (private members, public inline `GetOwnedByPlayer()`/`GetRented()` = FableWin ?GetOwnedByPlayer@..QBE_NXZ). GetPriceForNextDressLevel declared under FABLE_RETAIL_CTCBUYABLEHOUSE_FRAMEUPDATE; GetPrice/SetRented/SetOwnedByPlayer/IncrementDressLevel NOT declared in the header -> need a local declaring helper (e.g. derived-struct methods as row 0x006C04B0 does) or add-on declarations.
- thing+0x20 UsingComponents std::bitset<274>, +0x44 Components CVectorMap: engine/CThing.h (FABLE_RETAIL_CTHING), GetTC(id,&p) under FABLE_RETAIL_CTHING_GETTC_EXPANDED, __forceinline HasTC under **FABLE_RETAIL_CTHING_HASTC_FORCEINLINE**.
- stats+0xE0 NumberOfShopsOwned, +0xE4 NumberOfHousesOwned (Ego_r +0xE8/+0xEC, retail 8 lower): engine/CTCHeroStats.h names them at +0xE0/+0xE4 only in the FABLE_RETAIL_CTCHEROSTATS_SERIALISE branch (line ~228) and the generated default branch (~388); AddMoney(long,bool) declared in BOASTS branch and nested FABLE_RETAIL_CTCHEROSTATS_ADDMONEY (BRIBE branch). No single opt-in gives both AddMoney and the two counters -- check SERIALISE branch for AddMoney or use the landed 0x00895ED0 approach (local HeroStatsRetail{pad[0xE0]; long NumberOfShopsOwned, NumberOfHousesOwned;}).
- Draft models LOCALLY with shared headers available: CThing (CThing.h), CPlayer (CPlayer.h), CPlayerManager (CPlayerManager.h), CTCHeroStats (CTCHeroStats.h), CTCBuyableHouse (CTCBuyableHouse.h), CVectorMap (CVectorMap.h), CTCBase (CTCBase.h).

## 5. Visibility
- All callees are out-of-line, non-inline functions (none COMDAT-inline in PDB). Same TU siblings defined earlier: OnActionUseRent/Main/Dress (none called here).
- Inline only: CThing::HasTC / GetTC (forceinline in retail), CTCBuyableHouse::GetOwnedByPlayer/GetRented, CTCBase::GetParentThing.
- The residue (NOTES hand269: "presence-bit test form of the bool HasTC (shr vs test)") is EXACTLY what CThing.h's FABLE_RETAIL_CTHING_HASTC_FORCEINLINE documents
  (chunk Q23, row 0x006C1250: "plain inline leaves the test() result materialised ... `shr ; and cl,1`"; retail `test dword ptr [eax+28h],80000h` TCI_SHOP).

## 6. Siblings to copy
- 00/6c/CTCBuyableHouse_FrameUpdate_006c1250.cpp (Flags: none): defines FABLE_RETAIL_CCHARSTRING, CCLASS, DEF_CLASS_BASE, CTCBASE, CTHING, CTHING_GETTC_EXPANDED, CTHING_HASTC_FORCEINLINE, CINTELLIGENTPOINTER, CTCBUYABLEHOUSE, CTCBUYABLEHOUSE_FRAMEUPDATE ...; includes stlport/fable_containers.h, engine/CThing.h, engine/CTCBase.h, engine/CTCBuyableHouse.h; uses `ParentThing->HasTC(TCI_SHOP)` -- the same 0x80000 test.
- 00/72/CTCActionUseBuyHouse_OnActionUseMain_0072e940.cpp: FABLE_RETAIL_CTHING_GETTC_EXPANDED, engine/CThing.h + CVectorMap.h + CPlayerManager.h + CTCBuyableHouse.h; `GetPlayerManager().GetMainPlayer().GetPControlledCreature()`, `hero->GetTC(TCI_HERO,&heroTC)`, local `class CTCActionUseBuyHouse : public CTCBase` with HouseQuestion enum.
- 00/72/CTCActionUseBuyHouse_GlobalUpdate_0072ee30.cpp (caller), _OnActionUseRent_0072e810.cpp, _OnActionUseDress_0072e710.cpp (pointer-param style, local models).
- 00/89/CGameScriptInterfaceMethods_SetHouseOwnedByPlayer_00895ed0.cpp: same shop/house counter ++/-- (`UsingComponents[2]&0x80000`, NumberOfShopsOwned/NumberOfHousesOwned at +0xE0).
- 00/6c/CTCBuyableHouse_SetRented_006c1150.cpp: FABLE_RETAIL_STLP_BITSET_INDEX alternative for in-place word tests.

## 7. Draft model errors
- HasTC as `(word & mask) != 0` bool -> materialised shr form (the DIFFER 467v463). Fix: real CThing header + FABLE_RETAIL_CTHING_HASTC_FORCEINLINE (+GETTC_EXPANDED for the hero-stats lookup).
- GetPrice(bool)/GetPriceForNextDressLevel(bool) wrong; AddMoney is (long amount, bool) with true -- write `stats->AddMoney(-house.GetPriceForNextDressLevel(), true)` (bytes same, names/signatures wrong in draft).
- free GetPlayerManager() vs this->GetPlayerManager() (CTCBase protected member); GetHeroCreature -> GetPControlledCreature; pointer returns -> references.
- param should be `CTCBuyableHouse& house`, method private; OwnedByPlayer/Rented read via GetOwnedByPlayer()/GetRented(); thing via house.GetParentThing().
- Control flow otherwise matches (switch 1/2/3, if(!owned) buy first, `if (Rented) SetRented(false)` only in sell arm).

## 0x00BA4A40

# DOSSIER 0x00BA4A40 CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksBlankTextures (463 B)

## 1. Retail shape (xdis 00ba4a40 0x1df)
- __thiscall, `ret 14h` (5 stack args); ecx (`this`) never read. Caller SetupStaticBlocks 0x00BA50C4 leaves its own ecx in place (thiscall chain).
- No frame pointer (ebp used as a general register = primitive) -> default /O2 /Oy, no `optimize("y",off)`. No SEH. No size idioms (/0x2c done by imul 0x2E8BA2E9/sar 3 = CArray size()).
- Regs: esi = texture bank, ebp = &primitive, ebx = first block (spilled into the dead alreadySetUp arg slot [esp+24h]), edi = graphic index; frame spilled into the dead mesh arg slot [esp+10h].
- Order: bank (mesh+0x50 ->+4 ->+0x3d4), frame = bank+0x194, compare frame vs prim+0x7c, then stages[0..2] vs prim+0x6c/+0x70/+0x74 (byte); equal -> *alreadySetUp=1, return 1.
  else *alreadySetUp=0; blocks=prim+0x44; diffuse (stage!=-1): idx=material+0x10, default type 0; bump (stage!=-1): idx=+0x14, default 4.
  Type lookup = PeekGraphic(idx) branchy (`idx<size ? &list[idx] : 0`, test) THEN a 2nd PeekGraphic as cmp/sbb/and select, reloading GraphicList.First after the imul -> ->+0x28 (Frames) ->+0xc (GraphicInfo) ->+0x1c (TextureType); a null first peek gives eax=0 and still derefs (+0xc) = `PeekGraphicInfoFrame(idx,0)->GetTextureType()`.
  Loop: count=prim+0x40 read once, `if count>1`, edx=count-1 down-counter, ecx byte offset +0x20, StaticBlocks RELOADED from [ebp+44h] each iteration, first block fields read via ebx each iteration.
  Tail: RecordedFrame=frame (+0x7c) first, then RecordedTextureStages=stages (12-byte copy via `add ebp,6Ch`), return 1.

## 2. Name / signature (manifest label CORRECT)
- Ego_r.pdb (egor_pdb_names.tsv @Ego_r 0x00a5a8a0) and FableWin.pdb (_fablewin_api.tsv / FableWin.exe.signatures.tsv 0x0181e043, 0x02e9f410):
  `?SetupStaticBlocksBlankTextures@CEnginePrimitiveManagerStaticMeshesBase@@AAE_NAAVC3DPrimitive2@@AAVCEngineInternalPrimitiveStaticMesh@@AAVC3DTextureStages@@PBVC3DMeshMaterial@@PA_N@Z`
  = private, non-static, non-const __thiscall: `bool SetupStaticBlocksBlankTextures(C3DPrimitive2&, CEngineInternalPrimitiveStaticMesh&, C3DTextureStages&, const C3DMeshMaterial*, bool*)`.
  (NB: egor_pdb_names row at 00ba4a40 = HasEvent is an Ego_r-address collision, irrelevant.) Not virtual. Only caller: SetupStaticBlocks 0x00BA50A0 (not landed).
- Header CEnginePrimitiveManagerStaticMeshesBase.h has NO declaration of it in either branch: author must add an opt-in decl (private, thiscall) next to FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_STATICBLOCKS.

## 3. Callees
- 0x009FD150 CGraphicDataBank::GetBlankTexture -- landed rebuild/src/compiled/00/9f/CGraphicDataBank_GetBlankTexture_009fd150.cpp (stub declares `CTexture* GetBlankTexture(ETextureType)` non-const, global enum). PDB: `?GetBlankTexture@CGraphicDataBank@@QBEPBVCTexture@@W4ETextureType@1@@Z` = `const CTexture* GetBlankTexture(CGraphicDataBank::ETextureType) const`. Header CGraphicDataBank.h (TEXTURE_HELPERS branch) has the correct const form. Draft spelling (non-const, `long`, returns CTexture*) DISAGREES with PDB. Landed 009fd150 stub also disagrees (NAME_FIXES candidate).
- Inline (no call) accessors, all FableWin.pdb: C3DMeshInfo::PeekTextureBank QBE (bank+4 ->TextureBank+0x3d4), CMeshDataBank::GetTextureBank QBE, CResourceBank::GetCurrentFrame QBEKXZ (+0x164 base +0x30 = ResourceList.CurrentFrame), CGraphicDataBank::PeekGraphicInfoFrame QBE(KK), CGraphicInfoFrame::GetTextureType QBE, CGraphicInfo::GetTextureType QBE, C3DPrimitive2::GetRecordedFrame QBEJXZ / PeekRecordedTextureStages QBE / SetRecordedFrame QAEXJ / SetRecordedTextureStages QAEXABV, C3DTextureStages::operator== `??8C3DTextureStages@@QBE_NABV0@@Z`, C3DMeshMaterial::GetDiffuseMapBankIndex/GetBumpMapBankIndex.

## 4. Members
- mesh+0x50 CEngineInternalPrimitiveMeshBase::MeshInfo (const C3DMeshInfo*) -- engine/CEngineInternalPrimitiveMeshBase.h (public); retail class branch needs FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH for engine/CEngineInternalPrimitiveStaticMesh.h.
- C3DMeshInfo+4 Bank (CMeshDataBankEntry::PeekBank) -> CMeshDataBank+0x3d4 TextureBank: engine/C3DMeshInfo.h FABLE_RETAIL_C3DMESHINFO_GETMESH (PeekTextureBank) + FABLE_RETAIL_CMESHDATABANKENTRY + engine/CMeshDataBank.h FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK (GetTextureBank). Pattern used by landed 0x00BEC680 CollectStats.
- bank+0x194 CResourceBank(+0x164)::ResourceList.CurrentFrame -> GetCurrentFrame() (engine/CResourceBank.h:30). In CGraphicDataBank.h TEXTURE_HELPERS branch CGraphicDataBank : CBankFileAsync, CResourceBank (public).
- bank+0x1e0 CGraphicDataBank::GraphicList CArray<CGraphicInfo> (private; PeekGraphic/PeekGraphicInfoFrame declared, BODIES defined inline in the landed sibling row 0x00BA48F0, not in the header -- copy them).
- CGraphicInfo (44 B, engine/CGraphicInfo.h): TextureType +0x1c (private, NO GetTextureType accessor yet), Frames +0x28 (PeekInfoFrame). CGraphicInfoFrame (engine/CGraphicInfoFrame.h) GraphicInfo +0xc private, no GetTextureType accessor -> author must add opt-in inline `CGraphicDataBank::ETextureType GetTextureType() const` to both (FableWin QBE, public).
- C3DPrimitive2 (engine/C3DPrimitive2.h): StaticBlockCount +0x40, StaticBlocks +0x44 (GetStaticBlock/GetStaticBlockCount), RecordedTextureStages +0x6c, RecordedFrame +0x7c; SetRecordedFrame under FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS; GetRecordedFrame/PeekRecordedTextureStages/SetRecordedTextureStages NOT declared (add opt-in).
- N3DPrimitive::CStaticBlock (engine/N3DPrimitive_CStaticBlock.h): DiffuseTexture +0x14, BumpTexture +0x18 (const CTexture* under STATICBLOCKS_ORIGINAL_ACCESSORS).
- C3DTextureStages (engine/C3DTextureStages.h): Diffuse/BumpTextureStage long, UseBlankTextures bool; no operator== (add opt-in).
- C3DMeshMaterial (engine/C3DMeshMaterial.h): DiffuseMapBankIndex +0x10, BumpMapBankIndex +0x14; accessors under STATICBLOCKS_TEXTURE_HELPERS or C3DMESHMATERIAL_MAPBANKINDICES.
- Old draft models LOCALLY (all have shared headers): C3DPrimitive2, CStaticBlock, C3DTextureStages, C3DMeshMaterial, CGraphicDataBank, CEngineInternalPrimitiveStaticMesh, CEnginePrimitiveManagerStaticMeshesBase; plus invented CIndexedArray/CBankTextureEntry/CTextureDefHolder/CTextureDef/CGraphicsOwner/CMeshResource/FrameStamp (real: CArray GraphicList / CGraphicInfo / CGraphicInfoFrame / C3DMeshInfo / CMeshDataBank / GetCurrentFrame).

## 5. Visibility
- All accessors above are inline (no Fable.exe copies); only GetBlankTexture is called. Same TU: SetupStaticBlocksNoMorph 0x00BA48F0 (landed, immediately before) and SetupStaticBlocks 0x00BA50A0 (caller) -- same source file. The sibling's inline PeekGraphic / PeekGraphicInfoFrame bodies give exactly the branchy-then-select pair seen here (`PeekGraphic(g) ? PeekGraphic(g)->PeekInfoFrame(f) : 0`).
- Header-branch caution: C3DMeshInfo GETMESH path was used with the non-TEXTURE_HELPERS CGraphicDataBank branch (CollectStats uses FABLE_RETAIL_CGRAPHICDATABANK_BANKFILE); this row needs the TEXTURE_HELPERS branch (CGraphicInfo, GraphicList, ETextureType). Check the two compile together.

## 6. Siblings
- 0x00BA48F0 SetupStaticBlocksNoMorph (rebuild/src/compiled/00/ba/CEnginePrimitiveManagerStaticMeshesBase_SetupStaticBlocksNoMorph_00ba48f0.cpp): `// Flags:` empty, no pragma; defines FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS, _DEF_CLASS_BASE, _CRESOURCELIST, _CIVCOUNTEDPOINTER(_RESET_ADOPT), _CENGINEPRIMITIVERENDERER, _CTEXTURE, _CMOVABLERESOURCE, _C3DPRIMITIVE2_HIERARCHY, _STATICBLOCKS_ORIGINAL_ACCESSORS, _CENGINEPRIMITIVEMANAGERMESHBASE_STATICBLOCKS, _CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_STATICBLOCKS. Idiom: `N3DPrimitive::CStaticBlock& first_block=*mesh_prim.GetStaticBlock(0);` `unsigned long no_blocks=mesh_prim.GetStaticBlockCount(); for(c_block=1;...) ... *mesh_prim.GetStaticBlock(c_block)`; `long graphic_index=material->GetDiffuseMapBankIndex(); if(graphic_index==0) ... GetBlankTexture(CGraphicDataBank::TEXTURE_TYPE_OPAQUE)`.
- 0x00BEC680 CEngineInternalPrimitiveMeshBase::CollectStats: opt-ins C3DMESHINFO_GETMESH, CMESHDATABANK_TEXTUREBANK, C3DMESHMATERIAL_MAPBANKINDICES (texture-bank access idiom).
- 0x00BA1B00 DisplayMirroredTextures (same class): fable_compiler.h + FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_DISPLAYMIRRORED (private decl opt-in pattern to copy).
- 0x00BA9360 / 0x00BA95D0 GetDiffuseTexture/GetBumpTexture (same blank-texture default types 0 / 4).

## 7. Old-draft model errors
- Lookup modelled as GetPtr + operator[] on an invented array; original is the inline PeekGraphic pair inside PeekGraphicInfoFrame(idx,0) -> GetTextureType() (frame->GraphicInfo->TextureType) -- likely the source of the "bank array base reloaded after imul" residue.
- Loop used `blocks[0]` pointer + `primitive.StaticBlocks[i]`; retail = first_block reference (ebx) + GetStaticBlock(c) reload each iteration with no_blocks local; primitive must stay in ebp (the draft's residue was ebx/ebp swapped).
- Bank chain via invented owner structs instead of mesh.MeshInfo->PeekTextureBank(); bank should be `const CGraphicDataBank*`; frame from GetCurrentFrame() (unsigned long) vs GetRecordedFrame() (long).
- GetBlankTexture signature wrong (non-const, long, CTexture*); block textures are const CTexture*.
- Stage comparison: use C3DTextureStages::operator== (PDB-proven), retail loads `stages` as the left/this side (mov eax,[stages]; cmp eax,[prim+6c]); store frame then stages via Set accessors.

## 0x007BBE34

# DOSSIER 0x007BBE34 -- NThingShot::CTCShotDefinitionSpreadShotProjectile::InitMovement (464 B)

## 1. Retail shape (xdis 007bbe34 0x1e0)
- __thiscall, no args, plain `ret` (`leave ; ret`). Frame pointer `push ebp; mov ebp,esp; sub esp,0x5c` -> TU is `#pragma optimize("y",off)` + `optimize("s",on)` (same as every landed shot InitMovement). No SEH.
- Regs: esi=this (also spilled to [ebp-8], because esi is reused for the RandomiseShotDirection result pointer + movsd copy; `this` re-read from [ebp-8] for PShotDef at 0x7BBFC5). edi = target CThing* (kept in register); [ebp-4] = a separate by-reference COPY for IS_THING_ALIVE (lea ecx,[ebp-4]). ebx = ParentThing->PhysicsTC (pushed only after the alive test, inside the taken path).
- Frame slots (no sharing except the two hidden-return temps): [ebp-4] alive temp, [ebp-8] this, [ebp-0x14..-0xc] dir, [ebp-0x20..-0x18] aim, [ebp-0x2c] side, [ebp-0x38] velocity, [ebp-0x44] hidden-result temp (GetFacingVector AND RandomiseShotDirection result), [ebp-0x5c] CRightHandedSet (24 B).
- Flow: GetPItem -> alive test (je to epilogue, early return) -> target->PhysicsTC->vslot70(+0x118, hidden result) * 1.5f (0x0126DF28) into aim -> aim.RotateXY(angle@0x013BAE7C) -> aim += target->GetPos() (OUT-OF-LINE call 0x4C73D0) -> aim.Z = GetWorldMap().GetHeightZBelowPos(aim) -> angle += 0.333f (0x0126DF24); if (angle > 1.0f /*0x0122DED8*/) angle -= 1.0f -> dir = aim - physics->Position (inline, [ebx+0xc..0x14]) -> spread=(long)AccuracyPercentage (fld [esi+0x40]; __ftol2) evaluated BEFORE GetWorld().GetWorldSeed() -> RandomiseShotDirection(dir, spread, seed) result copied by 3x movsd into dir (assignment) -> dir.AccurateNormalise() -> side = C3DVector(dir.Y,-dir.X,0.0f) ^ dir (0.0f at 0x0122DEDC, inline) -> side.AccurateNormalise() -> physics->vslot32(+0x80)(CRightHandedSet(side,dir)) -> velocity = dir * (PShotDef->Speed / (float)ConstantFPS) (fild 0x01375550, fdivr [def+0x3c], scale kept on x87 for 3 comps) -> ApplyAcceleration(velocity) -> physics->vslot38(+0x98)(false).

## 2. Identity (manifest label CORRECT; two sources)
- Leaf: `?InitMovement@CTCShotDefinitionSpreadShotProjectile@NThingShot@@MAEXXZ` (protected virtual void __thiscall, no args).
  Source A: ghidra_out/egor_pdb_names.tsv (Ego_r 0x007BABD4) + FableWin.exe.signatures.tsv (0x01807A5A / 0x02707E98).
  Source B: rebuild/manifest/rtti-vtables.tsv: NThingShot::CTCShotDefinitionSpreadShotProjectile vtable 0x01240D84 slot 33 = 0x007BBE34; slot 33 is InitMovement for every shot class (landed Base 0x007C2CE0, Predictive 0x007BC6FA, TearInGround 0x007BC8FC).
- xref: only data refs (vtable 0x01240E08 and 0x0126E070); no direct callers.
- Class: namespace NThingShot; PDB size 168 (retail 0xA4: Alloc does operator new(0xA4)); RTTI chain like Predictive (SpreadShotProjectile -> Projectile -> Base, assumed: Projectile size 0xA4, this class adds no members per struct_layouts_egor `@STRUCT CTCShotDefinitionSpreadShotProjectile 168 0`). NO shared header engine/NThingShot_CTCShotDefinitionSpreadShotProjectile.h exists -- model it like engine/NThingShot_CTCShotDefinitionProjectilePredictive.h (derive from CTCShotDefinitionProjectile, `protected: virtual void InitMovement();`). The author may need a new header (or declare the class inline deriving from the shared CTCShotDefinitionProjectile).

## 3. Callees (landed file / landed name / PDB) -- draft spelling problems flagged with !!
| addr | landed row | PDB spelling | draft |
|---|---|---|---|
| 0x00A01B50 | 00/a0? CBaseIntelligentPointer_GetPItem_00a01b50.cpp | ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ (returns CBaseObject*) | !! draft returns CThing*; use PTargetThing.Get() (CIntelligentPointer<const CThing>, engine/CIntelligentPointer.h line 60 static_cast) |
| 0x00436380 | global_ISTHINGALIVECThingBuilding_00436380.cpp (leaf there `?IS_THING_ALIVE@@YI_NPBUThingRef@@@Z` -- inconsistent) | ??$IS_THING_ALIVE@PBVCThing@@@@YI_NABQBVCThing@@@Z (template<class T> bool __fastcall IS_THING_ALIVE(const T&), T=const CThing*) per landed TearInGround/Predictive | ok as template |
| vslot 70 +0x118 on target PhysicsTC | CTCPhysicsBase.h line 124 | virtual C3DVector CTCPhysicsBase::GetFacingVector() const | !! draft "GetFacing" on fake CFrameSource |
| 0x00A14040 | C3DVector_RotateXY_00a14040.cpp | ?RotateXY@C3DVector@@QAEXM@Z (void RotateXY(float)) | !! draft "RotateInPlace" |
| 0x004C73D0 | CThing_GetPosition_004c73d0.cpp | CThing::GetPos() const -> const C3DVector& (?GetPos@CThing@@QBEABVC3DVector@@XZ); CThing.h:411 declares it (out of line) | !! draft "GetPosition" |
| 0x00686D30 | CTCBase_GetWorldMap_00686d30.cpp | ?GetWorldMap@CTCBase@@IAEAAVCWorldMap@@XZ (protected, returns CWorldMap&) | !! draft returns CWorldMap* |
| 0x0063E480 | CWorldMap_GetHeightZBelowPos_0063e480.cpp | ?GetHeightZBelowPos@CWorldMap@@QBEMABVC3DVector@@@Z (const) | !! draft non-const |
| 0x00BFEA70 | global_ftol2_00bfea70.cpp | __ftol2 (compiler; from (long)float) | ok |
| 0x00686A70 | CTCBase_GetWorld_00686a70.cpp | ?GetWorld@CTCBase@@IBEAAVCWorld@@XZ (protected const, CWorld&) | !! draft returns CWorld*, non-const |
| 0x0049D810 | CWorld_GetWorldSeed_0049d810.cpp | ?GetWorldSeed@CWorld@@QBEPAKXZ (const, unsigned long*) | !! draft non-const |
| 0x007BEBE0 | 00/7b/NThingShot_RandomiseShotDirection_007bebe0.cpp | ?RandomiseShotDirection@NThingShot@@YI?AVC3DVector@@ABV2@JPAK@Z = C3DVector __fastcall NThingShot::RandomiseShotDirection(const C3DVector&, long, unsigned long*) | !! draft declares it at global scope (must be in namespace NThingShot); no shared header declares it -> declare locally |
| 0x00A14440 | C3DVector_AccurateNormalise_00a14440.cpp | ?AccurateNormalise@C3DVector@@QAEAAV1@XZ (C3DVector&) | !! draft "Normalise" returning void |
| 0x00487050 | CRightHandedSet_CRightHandedSet_00487050.cpp | ??0CRightHandedSet@@QAE@ABVC3DVector@@0@Z ; members Up +0, Forward +0xC (ctor (up, forward)) | !! draft member order Forward/Up |
| vslot 32 +0x80 on physics | CTCPhysicsBase.h:46 | virtual void ResetRHSet(const CRightHandedSet&) (verified: Predictive 0x007BC8B4 same +0x80) | !! draft "SetRHSet" |
| 0x006B0210 | CTCPhysicsBase_ApplyAcceleration_006b0210.cpp | ?ApplyAcceleration@CTCPhysicsBase@@QAEXABVC3DVector@@@Z (non-virtual) | ok |
| vslot 38 +0x98 on physics | CTCPhysicsBase.h:58 | virtual void EnableGravity(bool) (Predictive 0x007BC8F1 same) | !! draft "SetFrozen" |
- Globals: 0x01375550 = protected static CMainGameComponent::ConstantFPS, read via inline `CMainGameComponent::GetConstantFPS()` (engine/CMainGameComponent.h:183; ?GetConstantFPS@CMainGameComponent@@SIJXZ) -- !! draft `g_FramesPerSecond`.
- 0x013BAE7C (the rotating spread angle, float, read/written): NO name found (egor_pdb_names, labels_*, include, src grep for 13bae7c empty). Probably a function-local or file `static float` in the shot TU; a descriptive name is allowed (mark descriptive in NOTES). Initial value unknown here -- check .data vs .bss before choosing `static float x = ...;` vs zero-init (a local static with non-constant init would add a guard; there is none, so it is a constant-initialised static).
- Constants: 1.5f 0x0126DF28, 0.333f 0x0126DF24, 1.0f 0x0122DED8, 0.0f 0x0122DEDC.

## 4. Members (retail = PDB - 4 from PTargetThing on; engine/NThingShot_CTCShotDefinitionBase.h)
- this+0x04 ParentThing (CTCBase, engine/CTCBase.h, FABLE_RETAIL_CTCBASE).
- this+0x20 PTargetThing: CIntelligentPointer<const CThing> (PDB +0x24) -- header line 99 under FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION / MOVEMENT (Predictive uses MOVEMENT + ONHITBYMELEE).
- this+0x38 PShotDef: CDefPointer<const CThingShotDef> (PDB +0x3C) -- header line 104 (MOVEMENT). CThingShotDef::Speed at def+0x3C (engine/CThingShotDef.h).
- this+0x40 AccuracyPercentage: float (PDB +0x44) -- header line 106 (MOVEMENT). Inline accessor `long GetShotAccuracyPercentage() const { return (long)AccuracyPercentage; }` under FABLE_RETAIL_CTCSHOTDEFINITIONBASE_SHOTACCESSORS + MOVEMENT (Ego_r/FableWin ?GetShotAccuracyPercentage@CTCShotDefinitionBase@NThingShot@@IBEJXZ; used by chunk Q20 MultiArrow/DrainLife InitMovement). !! draft name "Spread"; also GetShotDef() exists there.
- CThing+0x60 PhysicsTC: CThing.h:402/410 GetPhysicsTC() inline (FABLE_RETAIL_CTHING). !! draft "Frame".
- CTCPhysicsBase+0x0C Position: GetPosition() inline (CTCPhysicsBase.h:233; FABLE_RETAIL_CTCPHYSICSBASE).
- Locally-modelled classes in the old draft that HAVE shared headers: C3DVector (engine/C3DVector.h, FABLE_RETAIL_C3DVECTOR; operator^ line 170-179 in 4 paren variants, SCALE_X_PAREN variant line 148), CRightHandedSet (engine/CRightHandedSet.h, FABLE_RETAIL_CRIGHTHANDEDSET + _CTOR), CTCPhysicsBase (FABLE_RETAIL_CTCPHYSICSBASE + _APPLYACCELERATION), CThing (CThing.h), CBaseIntelligentPointer (CIntelligentPointer.h, FABLE_RETAIL_CINTELLIGENTPOINTER), CWorldMap (CWorldMap.h, FABLE_RETAIL_CWORLDMAP_HEIGHTZ declares GetHeightZBelowPos -- added by chunk 764 for the twin row 0x007BC004), CWorld (CWorld.h line 143/550 GetWorldSeed), CTCBase (GetWorldMap line 149, GetWorld line 160), CMainGameComponent, CThingShotDef, the shot class itself (CTCFrameSource / CShotDef are invented).

## 5. Callee visibility
- Out of line in retail (do NOT give bodies): GetPItem, IS_THING_ALIVE, GetFacingVector (virtual), RotateXY, CThing::GetPos (out-of-line even though trivial -- CThing.h declares it without body), GetWorldMap, GetHeightZBelowPos, GetWorld, GetWorldSeed, RandomiseShotDirection, AccurateNormalise, CRightHandedSet ctor (0x00487050 called -- do NOT define the ctor inline; CRightHandedSet_CTOR macro should give a declaration), ApplyAcceleration.
- Inline in retail: CThing::GetPhysicsTC ([edi+0x60], [eax+0x60]), CTCPhysicsBase::GetPosition, C3DVector ops (*, -, ^, += componentwise), GetShotAccuracyPercentage ((long) cast -> __ftol2), CMainGameComponent::GetConstantFPS, CIntelligentPointer::Get.
- No STLport helpers involved. Neighbour 0x007BC004 (NymphFun::InitMovement, same TU, immediately after; parked hand253, DIFFER 338v342 with the SAME frame-sharing problem 0x44 v 0x5c) -- whatever fixes slot separation here likely fixes it there too.

## 6. Sibling landed rows (copy idioms)
- rebuild/src/compiled/00/7b/NThingShotCTCShotDefinitionProjectilePredictive_InitMovement_007bc6fa.cpp -- CLOSEST: identical tail (AccurateNormalise, `C3DVector(dir.Y,-dir.X,0.0f) ^ dir`, ResetRHSet(CRightHandedSet(side,dir)), velocity local, ApplyAcceleration, EnableGravity(false)). Pragmas y off / s on, `#pragma intrinsic(sqrt)`; macros FABLE_RETAIL_CTCBASE, _CINTELLIGENTPOINTER, _CTHING, _C3DVECTOR, _C3DVECTOR_SCALE_X_PAREN, _C3DVECTOR_SQUAREDMAGNITUDE_XYZ, _CRIGHTHANDEDSET, _CRIGHTHANDEDSET_CTOR, _CTCPHYSICSBASE, _CTCPHYSICSBASE_APPLYACCELERATION, _CTCPHYSICSBASE_LASTPOSITION, _CTCSHOTDEFINITIONBASE_ONHITBYMELEE, _CTCSHOTDEFINITIONBASE_MOVEMENT; includes ETCInterfaceType, C3DVector, CThing, CRightHandedSet, CTCPhysicsBase, CMainGameComponent, NThingShot_CTCShotDefinitionProjectilePredictive.h; `template<class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T&)` declared locally; `// Flags:` empty.
- rebuild/src/compiled/00/7b/NThingShotCTCShotDefinitionTearInGround_InitMovement_007bc8fc.cpp -- same TU; GetWorldMap().X(...) idiom; `IS_THING_ALIVE<const CThing*>(reinterpret_cast<CThing*>(PTargetThing.GetPItem()))`; `const float speed = PShotDef->Speed / CMainGameComponent::GetConstantFPS();` naming lever (chunk 840). (Still uses local stand-ins -- prefer Predictive's shared-header style.)
- rebuild/src/compiled/00/7c/NThingShotCTCShotDefinitionBase_InitMovement_007c2ce0.cpp, 00/7b/NThingShotCTCShotDefinitionBase_GetShootAtPos_007bedd0.cpp, 00/7b/NThingShot_RandomiseShotDirection_007bebe0.cpp (same TU).

## 7. Old draft model errors vs disassembly
1. Target handling: retail `edi = Get()`, then `[ebp-4] = edi; lea ecx,[ebp-4]` -- a converted temporary, not the variable itself. Spell `CThing* target = PTargetThing.Get()`-ish with T mismatch so IS_THING_ALIVE<const CThing*>(target) binds a temp (e.g. target typed `CThing*`/`const CThing*` vs template arg differing), or pass `PTargetThing.Get()` result through a separate expression; the draft's local was addressed itself -> spill re-read.
2. Frame: retail gives aim, dir, side, velocity, rh each their own slot (0x5c); draft shares (0x44). Name `side`, `velocity` (Predictive names velocity) and the CRightHandedSet temporary; keep aim and dir separate named locals.
3. `dir = RandomiseShotDirection(dir, ...)` is assignment into the existing dir (movsd from result ptr) -- correct form; the argument evaluation order is spread (ftol) first then GetWorld().GetWorldSeed() -- write `GetShotAccuracyPercentage()` / `(long)AccuracyPercentage` as the 2nd arg and seed as 3rd (VC evaluates right-to-left? here ftol happens first, then GetWorld; so check both orders).
4. The target's physics read `mov ecx,[edi+0x60]` is interleaved before `mov ebx,[eax+0x60]` (ParentThing physics): physics local is assigned after the alive test but its load is scheduled with the facing call -- declare `CTCPhysicsBase* physics = ParentThing->GetPhysicsTC();` after the test, as the draft does.
5. aim = facing*1.5f: result written through [ebp-0x20] with fst/fstp pattern: X,Y stored then Z `fst` -- consistent with `operator*(float)` returning a temp copied into aim.
6. All callee names/types per table in section 3 (RotateXY, GetPos, AccurateNormalise, ResetRHSet, EnableGravity, GetFacingVector, CWorldMap&/CWorld& references, const methods, NThingShot::RandomiseShotDirection, CMainGameComponent::GetConstantFPS, AccuracyPercentage, PShotDef, PTargetThing, PhysicsTC).

## 0x00A128E0

# DOSSIER 0x00A128E0 CPhysicsMeshObject::IsInContainmentVolume (464 bytes)

## 1. Retail shape (xdis 00a128e0 0x1e0)
- __thiscall, `ret 0Ch` (3 args), NO frame pointer (/Oy default), no SEH, `sub esp,40h ; push ebx ; push esi`.
- Frame 0x40 = 1/det [esp+0] (S+8 after pushes) + mesh-space point C3DVector [esp+4] (S+0xc) + inverse CMatrix3x4 [esp+10h] (S+0x18..0x44).
  Identical frame to landed GetNavigationLevelAt 0x00A11D10 and IsSpherePartContainedInBoundingBox 0x00A11F20 -> the inverse is a NAMED
  LOCAL filled in place (SetInverse), not a returned temporary (old draft frame was 0x70).
- 1.0f constant at 0x0122DED8 (`fdivr`), as in both siblings.
- Order: inverse of PositionMatrix(+8) inline; E13/E23/E33/E41/E42-ish terms stay on x87; transform of `centre` (eax=[esp+50h]) into
  the local at S+0xc; then `fld radius ; fmul [ecx+3Ch] (OO_Scale) ; fstp [esp+50h]` (scaled radius homed in the dead `centre` arg
  slot) -- computed AFTER the transform; `mov esi,[ecx+4]` (Mesh) hoisted at 0xA129CD; `test esi ; je ; inc [esi+4]` (AddRef) after
  the transform; push scaled radius, &local, name; `mov ecx,esi ; call 0x00A6CA20`; `mov bl,al`; release `test esi ; je ; dec [esi+4] ;
  jne ; mov eax,[esi] ; mov ecx,esi ; call [eax+4]` (slot 1 Delete), NO zero store; `mov al,bl`.
- The counted-copy/release tail is byte-for-byte the landed HasContainmentVolume 0x00A128A0 pattern (xdis 00a128a0 0x31).
- Sibling 0x00A12AB0 (PeekRoughProportionOfSphereContainedInVolume, 468 bytes, unlanded, parked in same pending dir) is the same body
  with a float result (push esi only, calls 0x00A6CA80).

## 2. Name / signature (two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv @Ego_r 0x00B805E0) and FableWin.pdb (naming_stragglers/FableWin.exe.signatures.tsv
  0x0182DF48 thunk / 0x031D2B80 body, 186 bytes / 57 instr):
  `?IsInContainmentVolume@CPhysicsMeshObject@@QBE_NABVCCharString@@ABVC3DVector@@M@Z`
  = `bool CPhysicsMeshObject::IsInContainmentVolume(const CCharString&, const C3DVector&, float) const`.
- Third source: landed callers 0x006A7B20 (CThingFilterPosInContainmentVolume operator(), passes VolumeName) and 0x0082D000
  (CThingBuilding::IsPointInside, passes global CCharString 0x013BAC98) both call 0x00A128E0 with (CCharString&, C3DVector&, 0.0f).
- Manifest label CORRECT. Old draft is WRONG on arg 1: `long volume` -> must be `const CCharString&` (forward-declared class is enough).
- Not virtual (no vtable slot; CPhysicsMeshObject vtable 0x0125D084 has only the dtor).
- Parameter names: no PDB local names available; callers use (name/volume, pos, tolerance). CPhysicsMesh landed row uses (name, position, radius).

## 3. Callees
- 0x00A6CA20 `?IsInContainmentVolume@CPhysicsMesh@@QBE_NABVCCharString@@ABVC3DVector@@M@Z` -- LANDED
  rebuild/src/compiled/00/a6/CPhysicsMesh_IsInContainmentVolume_00a6ca20.cpp, thiscall ret 0Ch, other TU (address far away).
  NOT declared in rebuild/include/engine/CPhysicsMesh.h (no opt-in exists) -> author must add one, e.g.
  FABLE_RETAIL_CPHYSICSMESH_CONTAINMENTVOLUME declaring `bool IsInContainmentVolume(const CCharString& name, const C3DVector& position, float radius) const; // 0x00A6CA20`
  (declaration only; also want 0x00A6CA80 PeekRoughProportionOfSphereContained(const CCharString&, const C3DVector&, float) const for 0x00A12AB0;
  PDB leaf ?PeekRoughProportionOfSphereContained@CPhysicsMesh@@QBEMABVCCharString@@ABVC3DVector@@M@Z -- the old draft spells it
  `PeekRoughProportionOfSphereContainedInVolume` on CPhysicsMesh: WRONG, that name belongs to CPhysicsMeshObject).
- vtable slot 1 (+4) of the pointee = CIVCountedPointeeBase::Delete() const (0x0042D930), reached through the inline CIVCountedPointer dtor.
  Old draft calls it `Release()` on a local CPhysicsMesh with a `RefCount` member -> wrong names (PDB: IVRefCount, Delete).
- Draft's `IsSphereInLocalBoundingBox 0x00A17690` is irrelevant/wrong (0x00A17690 = CPhysicsMesh::HasBoundingBoxIntersectedFromTo); drop.

## 4. Members
- CPhysicsMeshObject (struct_layouts_egor.tsv; shared header rebuild/include/engine/CPhysicsMeshObject.h, base CBaseClass):
  +0x04 `CIVCountedPointer<const CPhysicsMesh> Mesh` -> opt-in FABLE_RETAIL_CPHYSICSMESHOBJECT (also gives inline
        `CIVCountedPointer<const CPhysicsMesh> GetMesh() const { return Mesh; }`, FableWin.pdb inline ?GetMesh@CPhysicsMeshObject@@QBE?AV?$CIVCountedPointer@$$CBVCPhysicsMesh@@@@XZ);
        needs FABLE_RETAIL_CIVCOUNTEDPOINTER (counted copy ctor) and FABLE_RETAIL_DEF_CLASS_BASE (real CIVCountedPointeeBase with
        AddRef/Release/Delete, IVRefCount +4).
  +0x08 `CMatrix3x4 PositionMatrix` -> FABLE_RETAIL_CPHYSICSMESHOBJECT_MATRIX.
  +0x38 `float Scale` (not read); +0x3C `float OO_Scale` (read; always declared, private -> fine inside a member).
- Pointee: CPhysicsMesh (engine/CPhysicsMesh.h, derives CIVCountedPointeeBase): +0x04 IVRefCount (inc/dec), +0 vptr slot 1 Delete.
- C3DVector: engine/C3DVector.h (X,Y,Z). CCharString: no dedicated engine header; forward-declare (CPhysicsMeshObject.h already does).
- Old draft models LOCALLY (all have shared headers): C3DVector, CMatrix3x4, CPhysicsMesh, CIVCountedPointer, CPhysicsMeshObject.

## 5. Inline / visible callee bodies
- CMatrix3x4::SetInverse (FableWin.pdb inline, no retail out-of-line copy) -> engine/CMatrix3x4.h under FABLE_RETAIL_CMATRIX3X4 (FABLE_FORCEINLINE).
- CMatrix3x4::TransformPoint(const C3DVector&, C3DVector&) const -> FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS
  (plain row sums; matched in 0x00A11D10 and 0x00A11F20). Other forms in the header: TRANSFORMPOINT_SUMS, _ZYX_SUMS (in-place one-arg),
  PARTICLE_POINT, default.
- CIVCountedPointer copy ctor / dtor / operator-> (engine/CIVCountedPointer.h): FABLE_RETAIL_CIVCOUNTEDPOINTER; dtor variants
  FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT, _RELEASE_INNER, _RELEASE_MEMBER. Retail release here: `test esi ; je ; dec [esi+4] ;
  jne ; mov eax,[esi] ; mov ecx,esi ; call [eax+4]` with no store (dying local; the default Release()'s `Object=0` should be dead-store
  eliminated -- landed HasContainmentVolume 0x00A128A0 got the same bytes with a local model whose dtor zeroes the pointer).
- No STLport helpers involved.

## 6. Sibling landed rows (same class, same .cpp neighbourhood 0x00A114B0..0x00A12CC0, all `// Flags: none`, no pragmas)
- 0x00A11F20 CPhysicsMeshObject_IsSpherePartContainedInBoundingBox_00a11f20.cpp: FABLE_RETAIL_DEF_CLASS_BASE,
  _CPHYSICSMESHOBJECT_BOUNDINGBOXCONTAINS, _CPHYSICSMESHOBJECT, _CPHYSICSMESHOBJECT_MATRIX, _CIVCOUNTEDPOINTER, _CMATRIX3X4,
  _CMATRIX3X4_TRANSFORMPOINT_SOURCE_DEST_SUMS, _CPHYSICSMESH_BOUNDINGBOXINTERSECTED. Body:
  `float scaled_radius = radius * OO_Scale; CMatrix3x4 inverse; inverse.SetInverse(PositionMatrix); C3DVector local;
   inverse.TransformPoint(pos, local); return Mesh->HasBoundingBoxIntersectedFromTo(scaled_radius, local, local);`
  (there the scaled radius is computed FIRST and kept at [esp+0]; here retail computes it LAST into the arg slot -> write
  `radius * OO_Scale` at the call, or `radius *= OO_Scale` late; note row comment: `radius *= OO_Scale` keeps it in the arg slot).
- 0x00A11D10 CPhysicsMeshObject_GetNavigationLevelAt_00a11d10.cpp: same inverse+TransformPoint idiom (+ FABLE_RETAIL_C3DVECTOR,
  _CPHYSICSMESHOBJECT_NAVIGATION, _CPHYSICSMESH_NAVIGATION).
- 0x00A128A0 CPhysicsMeshObject_HasContainmentVolume_00a128a0.cpp: the counted local copy `CIVCountedPointer<CPhysicsMesh> mesh(Mesh);
  return mesh->HasContainmentVolume(name);` (local models, pre-header) -- same tail bytes as this row.
- 0x00A12890 IsPointContainedInBoundingBox, 0x00A127D0 ~CPhysicsMeshObject also landed in rebuild/src/compiled/00/a1/.

## 7. Old draft's model errors
- `long volume` instead of `const CCharString&` (signature/mangling wrong).
- `GetInverse()` returning a CMatrix3x4 by value + `operator*(C3DVector, CMatrix3x4)` -> frame 0x70 v retail 0x40; use named local
  `CMatrix3x4 inverse; inverse.SetInverse(PositionMatrix); C3DVector local; inverse.TransformPoint(centre, local);`.
- Local struct models instead of shared headers (see 4); wrong member names RefCount/Release (PDB: IVRefCount, Delete).
- Callee declared on local CPhysicsMesh with wrong sibling name (PeekRoughProportionOfSphereContainedInVolume on CPhysicsMesh).
- OPEN RISK (not settled by siblings): product operand order differs from 0x00A11D10. Normalised diff (this=R) shows retail here
  emits e.g. `fld E12 ; fmul E33` (0xA128EB) and `fld E12 ; fmul E23` (0xA128F3) where 0x00A11D10 emits `fld E33 ; fmul E12` and
  `fld E23 ; fmul E12`; many cofactor products in 0xA12934..0xA129B8 are swapped likewise (`fld E31 ; fmul E22` v `fld E22 ; fmul E31`
  etc.). Transform X here = `fld invE11 ; fmul x ; fld invE31 ; fmul z ; faddp ; fld invE21 ; fmul y ; faddp ; fadd E41` (1,3,2
  with inverse operand first) while 0x00A11D10 is `E3k*z, E2k*y, E1k*x`. Hypothesis: register pressure / x87 scheduling from the
  extra live values (Mesh in esi, centre ptr in eax, the AddRef) -- try the SOURCE_DEST_SUMS form first exactly as siblings, then
  vary where the counted copy and scaled radius are written (GetMesh() temporary vs named `CIVCountedPointer<const CPhysicsMesh> mesh(Mesh)`,
  `radius * OO_Scale` inline at the call) before touching header math.

## 0x00B903D0

# DOSSIER 0x00B903D0 CEnginePrimitiveManagerOcclusionBase::RenderUntexturedStaticMeshPrimitive (464 B)

## 1. Retail shape (xdis 00b903d0 0x1e0)
- __thiscall, `ret 8`, NO frame pointer (no push ebp/mov ebp,esp), NO SEH. `sub esp,0x14` first; ebx/ebp/esi/edi pushed LATE
  (0xB90410, after the SetWorldTransform branch merge) -- plain /O2 VC7.1, no optimize("s") idioms seen.
- Args: [esp+0x18] pre-push = arg1 = C3DPrimitive2* primitive (later [esp+0x28], held in ebp across the loop);
  arg2 = const CMatrix3x4& transform ([esp+0x1c] pre-push). After the pushes retail REUSES both arg slots:
  [esp+0x28] (primitive slot) = primitive->IndexBuffer homed each iteration; [esp+0x2c] (transform slot) = byte offset i*0x20.
- Locals: [esp+0x10] count-down block counter, [esp+0x14] mgr ptr (inline `this`), [esp+0x18] PrimitiveCount,
  [esp+0x1c] StartIndex, [esp+0x20] VertexCount (read ONCE before the loop at 0xB904B9, right after StaticBlockCount).
- Body: ApplyVertexShader(VertexShader /*this+0xc*/, false); if (mgr->FixedFunctionVertexMode +0x395) core->SetWorldTransform(t)
  else mgr->SetWorldTransform(t); inline PrepareForRender(1) (tests 4->SetVSConstants, 1, 2, 4->UpdateAmbient, 0x20000, 0x400,
  0x80 via `cmp byte; jns`; flags&1 kills ResetWorldTransform); loop over StaticBlocks.
- Loop body = inline 7-arg CShaderRenderManager::DrawBuffersIndexed(*VertexBuffer, 0, vertexCount, *IndexBuffer, StartIndex,
  PrimitiveCount, block.GetPrimitiveType()): global 0x01436E14 RE-READ each iteration (esi), type = 4+(IsStrip!=0) (setne) in edi
  and IndexBuffer loaded BEFORE SetStreamSource (+0x190, stride byte +0x10, PVertexBuffer +4); device re-read from &mgr->D3DDevice
  for SetIndices (+0x1a0) and DrawIndexedPrimitive (+0x148); `cmp edi,-1` (PT_NULL) -> IndexBuffer->PrimitiveType (+0xc) is
  resolved AFTER SetIndices; RenderManager (+0x1d0) via saved mgr -> RealiseRenderState 0x00A058C0; Draw(type,0,0,vtx,start,count).
  Loop form: guard `count>0` (jbe), byte offset +0x20, `dec counter; jne` (VC strength-reduced `for (i=0;i<count;++i)`).
  StaticBlocks (+0x44), VertexBuffer (+0x30), IndexBuffer (+0x34) re-read every iteration.

## 2. Name / signature (manifest label CORRECT)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv @00a4ffe0) and FableWin.pdb (_fablewin_api.tsv / naming_stragglers @01815218, 02ed8150):
  `?RenderUntexturedStaticMeshPrimitive@CEnginePrimitiveManagerOcclusionBase@@AAEXPAVC3DPrimitive2@@ABVCMatrix3x4@@@Z`
  = PRIVATE, non-virtual, non-const __thiscall `void RenderUntexturedStaticMeshPrimitive(C3DPrimitive2*, const CMatrix3x4&)`.
- Not a vtable slot. Callers (xref.py): 0x00B906DF (DrawVisibilityTestBox 0x00B906C0, landed, models it locally with void* args
  -- made-up spelling there), 0x00B90CB2, 0x00B910F7.

## 3. Callees (landed file / PDB) -- all spellings agree with PDB except the draft helpers
- 0x00988020 CShaderRenderManager_ApplyVertexShader_00988020.cpp  ?ApplyVertexShader@CShaderRenderManager@@QAEXABVCVertexShader@@_N@Z
- 0x00A0B140 CRenderManagerCore_SetWorldTransform_00a0b140.cpp     ?SetWorldTransform@CRenderManagerCore@@QAEXABVCMatrix3x4@@@Z
- 0x009881F0 CShaderRenderManager_SetWorldTransform_009881f0.cpp   ?SetWorldTransform@CShaderRenderManager@@QAEXABVCMatrix3x4@@@Z
- 0x0098B930 SetVSConstants (AAEXJPBXK), 0x00988A20 UpdatePixelShader, 0x00989BF0 UpdatePixelShaderConstants, 0x00989760
  UpdateAmbient, 0x009897C0 UpdateFogColour, 0x0098A760 UpdateLightGlobals, 0x00988A50 UpdateCombinedProjectionTransform
  (all CShaderRenderManager private AAEXXZ, all landed rebuild/src/compiled/00/98/CShaderRenderManager_<Name>_<addr>.cpp)
- 0x00A058C0 CRenderStateManager_RealiseRenderState_00a058c0.cpp  ?RealiseRenderState@CRenderStateManager@@QAEXXZ
- Inline (no call): ?PrepareForRender@CShaderRenderManager@@QAEXK@Z; ?DrawBuffersIndexed@CShaderRenderManager@@QAEXAAVCVertexBufferWin32@@KKAAVCIndexBuffer@@KKW4EPrimitiveType@@@Z
  (out-of-line retail copy landed at 0x00B8EB00: SelectVertexBuffer + 6-arg overload, FABLE_FORCEINLINE);
  ?SelectVertexBuffer@CShaderRenderManager@@QAEXAAVCVertexBufferWin32@@@Z; ?GetPrimitiveType@CBaseBlock@N3DPrimitive@@QBE?AW4EPrimitiveType@@XZ;
  FableWin ?IsFixedFunctionVertexMode@CShaderRenderManager@@QBE_NXZ (accessor for +0x395).
- DRAFT DISAGREES: `DrawIndexed(...)` is a made-up helper -> real is DrawBuffersIndexed (7-arg); `CPrimitiveBlock` -> N3DPrimitive::CBaseBlock;
  `block.IsStrip ? 5 : 4` -> GetPrimitiveType() (PT_TRIANGLE_STRIP/PT_TRIANGLE_LIST, engine/EPrimitiveType.h).
- Globals: 0x01436E14 = labels `g_pShaderRenderManager` (ghidra_out/labels_*.tsv) but landed 0x00BCFB00 spells it
  NEngineInternal::GPShaderRenderManager (header comment cites ?GPShaderRenderManager@NEngineInternal@@). 0x01436E18 =
  `g_pRenderManagerCore` (labels). One-address-one-name conflict for 0x01436E14: pick what the landing coordinator uses;
  RenderRainBox 0x00B54100 / 0x00B9E750 use g_pShaderRenderManager.

## 4. Members
- this +0xC VertexShader (CVertexShader) -- engine/CEnginePrimitiveManagerOcclusionBase.h (gated by FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS,
  needs engine/CEnginePrimitiveManager.h, private members, sizeof 0x20; struct_layouts: VertexShader 12, UnitBox 20, SilhouetteTreshold 28).
  The header does NOT declare RenderUntexturedStaticMeshPrimitive -- draft's "no engine header exists" is stale.
- C3DPrimitive2 (engine/C3DPrimitive2.h): VertexBuffer +0x30 (Ego_r scoped_ptr<CVertexBufferWin32>; header has only
  `unsigned char VertexBuffer[4]` -- no typed opt-in), IndexBuffer +0x34 (scoped_ptr<CIndexBuffer> under
  FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER + FABLE_RETAIL_SCOPED_PTR(+_RESET) + FABLE_RETAIL_CINDEXBUFFER), StaticBlockCount +0x40,
  StaticBlocks +0x44 (CStaticBlock*), VertexCount +0x58 (GetVertexCount() only under FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS);
  GetStaticBlock(i)/GetStaticBlockCount() always available.
- N3DPrimitive::CBaseBlock (engine/N3DPrimitive_CStaticBlock.h): PrimitiveCount +0, StartIndex +4, IsStrip +8; accessors
  GetPrimitiveCount/GetStartIndex/GetPrimitiveType under FABLE_RETAIL_CBASEBLOCK_ACCESSORS (includer supplies EPrimitiveType.h).
- CShaderRenderManager (engine/CShaderRenderManager.h, no class macro): CurrentVSConstantLayout +4, RenderManager +0x1d0 (CRenderManager*),
  D3DDevice +0x1d4, FixedFunctionVertexMode +0x395, UpdateFlags +0x3d8; PrepareForRender is FABLE_FORCEINLINE in the header;
  6-arg DrawBuffersIndexed body under FABLE_RETAIL_CSHADERRENDERMANAGER_INLINE_BLOCKDRAW (needs FABLE_RETAIL_CRENDERMANAGERCORE);
  the 7-arg overload and SelectVertexBuffer are DECLARED only -- the author must give them inline bodies (copy 0x00B8EB00's).
- CVertexBufferWin32 (engine/CVertexBufferWin32.h, FABLE_RETAIL_CVERTEXBUFFERWIN32; PVertexBuffer +4 / Stride +0x10 in CVertexBufferBase).
- CIndexBuffer (engine/CIndexBuffer.h FABLE_RETAIL_CINDEXBUFFER): PD3DIndexBuffer +4, PrimitiveType +0xc stored as unsigned char[4]
  (header 6-arg body reads it via *reinterpret_cast<EPrimitiveType*>).
- CRenderManagerCore.h (FABLE_RETAIL_CRENDERMANAGERCORE) declares SetWorldTransform; CVertexShader.h (FABLE_RETAIL_CVERTEXSHADER).
- Draft LOCALLY models (all have shared headers): IDirect3DDevice9, CVertexShader, CVertexShaderConstantLayout, CRenderStateManager,
  CShaderRenderManager, CRenderManagerCore, CVertexBufferWin32, CIndexBuffer, CStaticBlock, C3DPrimitive2, CEnginePrimitiveManagerOcclusionBase.

## 5. Visibility
- Everything this row inlines (PrepareForRender, SelectVertexBuffer, both DrawBuffersIndexed, GetPrimitiveType,
  IsFixedFunctionVertexMode) is header-inline in retail. Out-of-line callees are ordinary member calls; no STLport helper is called.
- The 7-arg DrawBuffersIndexed needs FABLE_FORCEINLINE per 0x00B8EB00 notes ("plain in-class inline leaves it a call").

## 6. Siblings / idioms
- rebuild/src/compiled/00/bc/CEnginePrimitiveManagerAnimatedMeshesBase_RenderPrimitiveBlocksIndexBuffer_00bcfb00.cpp: shared-header
  model; defines FABLE_RETAIL_SCOPED_PTR, _SCOPED_PTR_RESET, _CINDEXBUFFER, _CRENDERMANAGERCORE, _CSHADERRENDERMANAGER_INLINE_BLOCKDRAW,
  _CBASEBLOCK_ACCESSORS, _C3DPRIMITIVE2_INDEXBUFFER, _C3DPRIMITIVE2_ANIMATEDBLOCKS; calls DrawBuffersIndexed(0, vertex_count,
  *primitive.IndexBuffer, start_index, primitive_count, block->GetPrimitiveType()) with named locals.
- rebuild/src/compiled/00/b9/CEnginePrimitiveManagerStaticMeshesBase_RenderMeshPrimitiveBlocksDegenerateTriangles_00b9e750.cpp: same
  StaticBlocks loop; lesson: block count read once into a const local; draw args as named locals in order primitiveCount,
  startIndex, vertexCount (that order fixed ebx/spill allocation; direct-arg form swapped them).
- rebuild/src/compiled/00/b8/CShaderRenderManager_DrawBuffersIndexed_00b8eb00.cpp: the exact 7-arg body (SelectVertexBuffer + forceinline 6-arg).
- rebuild/src/compiled/00/b5/CEngineWeatherRenderer_RenderRainBox_00b54100.cpp: ApplyVertexShader + SetWorldTransform + PrepareForRender(1) prefix.
- Same-class landed: RenderBoundingBox 0x00B91140, DrawAlphaBoundingBox 0x00B90CD0, IsClippedByNearPlane 0x00B900B0 (local models, no pragmas/Flags lines).

## 7. Draft model errors vs disassembly
- VertexCount must be read into a local BEFORE the loop (retail 0xB904B9); draft does this but block reads/draw args are passed
  directly -> the hand263 residue (ebx/ebp swap). Use 0x00B9E750's named-local order and the 7-arg DrawBuffersIndexed so the
  IndexBuffer/type are evaluated at the inline call (before SetStreamSource) as retail does.
- Draft's DrawIndexed takes `long type` and pointers; real overload takes references + EPrimitiveType; type comes from
  GetPrimitiveType() (PT_NULL=-1 check stays because VC7.1 doesn't fold it).
- primitive should stay a pointer (PDB PAVC3DPrimitive2) -- it lives in ebp; transform is a reference.
- Draft's local headers / FixedFunctionVertexMode direct read are fine codegen-wise but violate the shared-header rule.

