## 0x00959BF0

# DOSSIER 0x00959BF0 -- CAIStateGroup_HoverWander::Update (739 bytes)

## 1. Retail shape (xdis 00959bf0 0x2f3)
- thiscall virtual, plain `ret` (0x00959EC3 / 0x00959ED2), no args. NO frame pointer (`sub esp,70h; push ebx,ebp,esi,edi`) -> default /Oy, no SEH.
- Two epilogues: loop-exit (counter==3) and the shared Terminate tail at 0x00959EC4/0x00959EC6 (`mov ecx,esi; call 0x008FCF40`).
- Order of evaluation at entry: GetWorld() 0x008FCFC0 -> GetWorldSeed 0x0049D810 FIRST (seed ptr kept in edi for the whole function),
  then the GetTC(7) on GetCreature() (bitset byte bit 7 of +0x20 stored to [esp+13h]; on a miss `nav` stays the uninitialised [esp+28h]).
  => source: `unsigned long* seed = GetWorld().GetWorldSeed();` as the first statement, GetTC hoisted ABOVE the loop (unlike 0x00959A60).
- Loop counter [esp+1Ch] (also the GetTC key slot) 0..3, `inc; cmp 3; jne` at the bottom. Wander-position miss -> `continue`.
- [esp+13h] (the GetTC presence byte) is reused as the `bool& reached` argument of slot 22 (lea [esp+17h] after one push).
- Constants: 4294967296.0f 0x0122DCB4 (unsigned->float fix-up), 0.003f 0x01294EAC, 0.1f 0x0122EE58, -1.0f 0x0122DEE0 (`fmul`, so `*= -1.0f`
  not unary minus), 0.02f 0x01294EA8, 3.0f 0x01294EA4, 0.0f 0x0122DEDC, double 6.2831854820251465 0x0124F2B8 = ?TWO_PI@@3NB
  (= (double)6.2831855f, the GFSin/GFCos scale), 5.0f pushed imm 0x40A00000, 0.05f imm 0x3D4CCCCD, 0x11.

## 2. Identity
- Name: CAIStateGroup_HoverWander::Update, leaf `?Update@CAIStateGroup_HoverWander@@UAEXXZ` (void, no params, public virtual).
  Sources: (a) rebuild/manifest/rtti-vtables.tsv: CAIStateGroup_HoverWander vtable 0x0128C5D4 slot 12 = 0x00959BF0 (xref: only ref is
  .rdata 0x0128C604 = 0x0128C5D4+0x30); (b) CAIStateGroupBase.h slot 12 = `virtual void Update() = 0`; (c) Ego_r.pdb (egor_pdb_names
  0x0097E390) and FableWin (_fablewin_api 0x02C09EB0) both have ?Update@CAIStateGroup_HoverWander@@UAEXXZ. Manifest label CORRECT.
- Class: CAIStateGroup_HoverWander : CAIStateGroup_HoverMovementBase : CAIStateGroup_EnemyCreatureBase : ... CAIStateGroupBase; sizeof 0x1C (no own data).

## 3. Callees (landed file / landed name / PDB) -- draft spellings that are WRONG marked [FIX]
- 0x008FCFC0 rebuild/src/compiled/00/8f/CAIStateGroupBase_GetWorld_008fcfc0.cpp  CAIStateGroupBase::GetWorld() const -> CWorld&
  (?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ, protected). [FIX] draft `GetPWorld()` returning CWorld*.
- 0x0049D810 .../00/49/CWorld_GetWorldSeed_0049d810.cpp  CWorld::GetWorldSeed() const -> unsigned long* (?GetWorldSeed@CWorld@@QBEPAKXZ).
  [FIX] draft `GetRandomSeed()` returning unsigned long&.
- 0x0040F020 .../00/40/CVectorMap_LowerBound_0040f020.cpp  CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound (via CThing::GetTC,
  FABLE_RETAIL_CTHING_GETTC_EXPANDED). [FIX] draft's local CInterfaceMap/SEntry/m_presentBits/m_interfaces model.
- 0x004C73D0 .../00/4c/CThing_GetPosition_004c73d0.cpp  CThing::GetPos() const (?GetPos@CThing@@QBEABVC3DVector@@XZ). [FIX] draft `GetPosition`.
- 0x006CB6E0 .../00/6c/CTCCreatureNavigation_GetNearbyWanderPosition_006cb6e0.cpp  ?GetNearbyWanderPosition@CTCCreatureNavigation@@QAE_NABVC3DVector@@MPAV2@_N@Z (OK; flag arg here = true).
- 0x00497890 .../00/49/global_GFROR13_00497890.cpp  GFROR13 (?GFROR13@@YIJK@Z, returns long) -- declared in engine/GFRandom.h; draft returned unsigned long.
- 0x00A14440 .../00/a1/C3DVector_AccurateNormalise_00a14440.cpp  ?AccurateNormalise@C3DVector@@QAEAAV1@XZ (returns C3DVector&). [FIX] draft `Normalise`.
  Opt-in FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE (used by sibling 0x0091DDD0).
- 0x0097D580 .../00/97/CAIStateGroupBase_AIActionLookAtPos_0097d580.cpp  ?AIAction_LookAtPos@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@JM@Z
  (`ret 18h`; enums EAIActionRepeat/EAIActionReturn, engine/EAIAction.h). Declared in CAIStateGroupBase.h under FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS (line 81).
- vtable slot 22 (+0x58) = 0x0091D7F0 .../00/91/CAIStateGroupHoverMovementBase_AIActionMoveToPosAndFacePos_0091d7f0.cpp
  ?AIAction_MoveToPosAndFacePos@CAIStateGroup_HoverMovementBase@@MAE?AW4EAIActionReturn@@W4EAIActionRepeat@@ABVC3DVector@@1AA_NW4EMovementType@@@Z
  (protected virtual; (EAIActionRepeat, const C3DVector& pos, const C3DVector& facePos, bool&, EMovementType)).
  [FIX] draft calls it AIAction_MoveToPosProximityAndFacePos -- that is the NON-virtual 0x009412b0-in-Ego_r one with an extra float (`M1`).
  NOTE engine/CAIStateGroup_HoverMovementBase.h declares NO members (empty class) -- the virtual must be modelled (sibling 0x00959A60 does it locally).
- 0x008FCF40 .../00/8f/CAIStateGroupBase_Terminate_008fcf40.cpp  ?Terminate@CAIStateGroupBase@@QAEXXZ (OK).
- Inline (no call): GFRandom (?GFRandom@@YIKKPAK@Z, engine/GFRandom.h: `*seed*=0x24a1; *seed+=0x24df; *seed=GFROR13(*seed); if(range) return *seed%range; return 0;`)
  x4: GFRandom(100,seed), GFRandom(2,seed) (`and 1`), GFRandom(100,seed), GFRandom(3, GetWorld().GetWorldSeed())+8.
  GFSin/GFCos (?GFSin@@YINM@Z / ?GFCos@@YINM@Z, engine/GFSin.h / engine/GFCos.h: `sin(angle*(double)6.2831855f)`, needs #pragma intrinsic(sin,cos)).

## 4. Members
- this+0x04 CAIStateGroupBase::PBrain (CAIBrain*) -- engine/CAIStateGroupBase.h (FABLE_RETAIL_CAISTATEGROUPBASE[_MEMBERS]).
- PBrain+0x18 CAIBrain creature (CThingAICreature) -- engine/CAIBrain.h GetCreature() under FABLE_RETAIL_CAIBRAIN_GETCREATURE;
  CAIStateGroupBase::GetCreature() const -> CThingAICreature& under FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE (inline, ?GetCreature@CAIStateGroupBase@@IBEAAVCThingAICreature@@XZ).
- creature+0x20 bitset (UsingComponents) bit 7 and +0x44 Components CVectorMap -- engine/CThing.h, FABLE_RETAIL_CTHING_GETTC_EXPANDED (GetTC(TCI_NAVIGATION,&nav)), engine/CVectorMap.h.
- C3DVector X/Y/Z -- engine/C3DVector.h (operator+, operator*(float) const, FABLE_VECTOR_INLINE = __forceinline).
- Draft models LOCALLY (all have shared headers): C3DVector, CInterfaceMap/CVectorMap, CThing, CThingCreatureBase, CTCCreatureNavigation,
  CWorld, CAIBrain, CAIStateGroupBase, CAIStateGroup_HoverMovementBase (header empty -> virtual slot 22 needs a model).

## 5. Callee visibility
- GetWorld / GetWorldSeed / GetPos / AccurateNormalise / LookAtPos / Terminate / GetNearbyWanderPosition / LowerBound / GFROR13: out of line (calls).
- Inline in retail (bodies must be visible): GFRandom (engine/GFRandom.h), GFSin/GFCos (engine/GFSin.h, GFCos.h), CThing::GetTC
  (FABLE_RETAIL_CTHING_GETTC_EXPANDED), CAIStateGroupBase::GetCreature + CAIBrain::GetCreature (both _GETCREATURE macros), C3DVector ops.
- Do NOT use CAIStateGroupBase::GetPWorldSeed (0x008FCFD0, out of line) -- retail calls GetWorld+GetWorldSeed explicitly here.

## 6. Siblings (copy idioms)
- 0x0091DDD0 rebuild/src/compiled/00/91/CAIStateGroupHoverAllAroundTest_Update_0091ddd0.cpp (RELOCATION_MATCH) -- THE closest idiom:
  `C3DVector dir(tp.X-mp.X, tp.Y-mp.Y, 0.0f); dir.AccurateNormalise(); C3DVector perp(dir.Y, -dir.X, 0.0f);`
  `target = start + A*(float)GFSin(frac)*dist + perp*(float)GFCos(frac)*dist;` Macros: FABLE_RETAIL_CTHING, _CINTELLIGENTPOINTER,
  _CAISTATEGROUPBASE, _CAISTATEGROUPBASE_MEMBERS, _GETCREATURE, _AIACTIONS, _SPEECH, _MEALACTIONS, FABLE_RETAIL_CAIBRAIN_GETCREATURE,
  FABLE_RETAIL_C3DVECTOR_ACCURATENORMALISE, _CWORLD, _CTHINGPHYSICAL/_CTHINGGAMEOBJECT/_CTHINGCREATUREBASE/_CTHINGAICREATURE; `#pragma intrinsic(fmod, sin, cos)`;
  includes engine/GFSin.h, engine/GFCos.h, engine/EAIAction.h, engine/EMovementType.h.
- 0x00959A60 .../00/95/CAIStateGroupHoverWanderAroundThing_Update_00959a60.cpp (same 3-iteration loop, same slot-22 call, GetTC via
  FABLE_RETAIL_CTHING_GETTC_EXPANDED + CVectorMap.h, Random(3)+8 before the do/while; models HoverMovementBase locally with slot 22).
- 0x0096E830 .../00/96/CAIStateGroupMeleeCombatNearby_IsEnemyCreatureNearCombatPosition_0096e830.cpp (TWO_PI 0x0124F2B8 = ?TWO_PI@@3NB, GFCos).
- 0x00959800 HoverWanderAroundThing::IsRunnable, 0x00959F30 HoverKeepInArea::IsRunnable (`// Flags: none`, no pragmas).
- Neighbour rows use no #pragma optimize; Flags none.

## 7. Draft model errors vs disassembly
1. SWAPPED randoms: retail v1 = GFRandom(100)*0.003f+0.1f, `*= -1.0f` when GFRandom(2)==0 -> stored [esp+18h] is the ANGLE in turns
   (fld [esp+20h] after 8 pushed bytes, `fmul qword TWO_PI`); v2 = GFRandom(100)*0.02f+3.0f [esp+14h] is the DISTANCE (fmul [esp+28h]
   after +0x14). Draft computes angle from the 3..5 value and scales by the ±0.1..0.4 one.
2. Rotation formula wrong. Retail (decoded x87): x = (T.X + dir.Y*s*d) + dir.X*c*d; y = (T.Y + (-dir.X)*s*d) + dir.Y*c*d;
   z = (T.Z + 0.0f*s*d) + dir.Z*c*d, cos term evaluated first (VC right-to-left operator+ arg), `-dir.X` (fchs) computed before the angle.
   => `C3DVector perp(dir.Y, -dir.X, 0.0f); C3DVector look = target + perp*(float)GFSin(v1)*v2 + dir*(float)GFCos(v1)*v2;`
   (exact sibling 0x0091DDD0 form; one `angle*TWO_PI` shared by fcos/fsin).
3. Movement type: retail computes GFRandom(3, GetWorld().GetWorldSeed())+8 ONCE after LookAtPos succeeds, before the do/while (loop
   head 0x00959E87 re-pushes ebx); draft recomputes it inside the loop.
4. Seed: `unsigned long* seed = GetWorld().GetWorldSeed();` first statement; then GFRandom(100,seed) etc. (dead store of the first rotate result is retail's own).
5. dir = C3DVector(T.X-here.X, T.Y-here.Y, 0.0f) with here = GetCreature().GetPos() re-fetched (Y subtracted first = right-to-left ctor args) then AccurateNormalise().
6. Names/types per section 3 (GetWorld, GetWorldSeed, GetPos, AccurateNormalise, AIAction_MoveToPosAndFacePos, EAIActionRepeat/EMovementType enums).

