## 0x007F0F40

# DOSSIER 0x007F0F40 CTCKrakenTentacle::ReturnToDesiredPos (559 B)

## 1. Retail shape (xdis 007f0f40 0x23f)
- __thiscall, `ret 4` (one bool arg at [esp+0x70] after the 3 pushes). No frame pointer (`sub esp,0x60` first, then push esi/edi; ebx pushed only AFTER the alive test, so the early-out pops edi/esi only). No SEH. Plain /O2 (no /Os idioms).
- Frame (esp offsets after push ebx; esi/edi/ebx at 0..0xb): dist 0x0c, krakenAngle->angle 0x10, facing 0x14, dir 0x18/1c/20, current 0x24/28/2c, desired 0x30/34/38, temp 0x3c/40/44 (first the `desired-current` result, later the SetPosition argument), scaled.Z spill 0x50, CRightHandedSet orientation 0x54..0x6b.
- Flow:
  1. GetPItem(PKraken) twice: `!=0` then `test byte [eax+0x91],1` (CThing::Dead bit) -> IS_THING_ALIVE(PKraken) expanded; false -> return.
  2. ParentThing->GetPos() (0x4C73D0) copied into BOTH current and desired (X,Y loaded once, fst'd twice; Z through eax/ecx dwords).
  3. a THIRD GetPItem whose result is unused (same idiom as landed QueueStrikeDamage 0x007F1170: an unused `CThing* kraken = PKraken...` local), then `ebx = PTentacleDef` (def local), then a 4th GetPItem -> +0x64 PTCGraphicAppearance -> GetDummyObject(DummyName(+0x48), 0, 0, desired, orientation, 1.0f).
  4. true -> desired.Z += def->TentacleHeightOffset (+0x3c).
  5. temp = desired - current materialised at 0x3c.. and each component dword-copied into dir (0x18..): `C3DVector dir = desired - current;` style copy (not RVO into dir).
  6. sum = dir.Z*dir.Z + dir.Y*dir.Y + dir.X*dir.X (Z first!) vs DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO (0x0129BA3C, fld;fmul same) -> dist = dir.GetMagnitudeAndNormalise() (0xA14480, float in st0) else dist = 0.
  7. kraken physics (GetPItem -> +0x60 PhysicsTC) ->GetFacingAngleXY (vtbl +0xFC) -> [0x10]; ParentThing->PhysicsTC->GetFacingAngleXY -> [0x14]; [0x10] += FacingAngle(+0x44) stored back then reloaded; push [0x10], push facing -> GFGetSignedAngleDifference(facing, angle) -> stored to [0x10] (same slot = same variable `angle`).
  8. if (!immediate): turn = def->TentacleTurnSpeed(+0x2c) * 1/360 (0x01238E00) kept on x87; `if (angle < 0.0f /*0x0122DEDC*/) { if (-turn > angle) angle = -turn; } else if (turn < angle) angle = turn;` (fchs on turn; fcom angle; test ah,41 jne / test ah,5 jp); then `if (!(dist < def->TentacleMoveSpeed(+0x30))) dist = def->TentacleMoveSpeed;` (fcomp; test ah,5; jnp; dword move).
  9. physics = ParentThing->PhysicsTC loaded BEFORE the arithmetic (eax=[esi+4], ecx=[eax+0x60]); dir.X*dist, dir.Y*dist on stack, push &temp, dir.Z*dist spilled to 0x50, then X+current.X, Y+current.Y, Z(reloaded)+current.Z -> temp; vtbl +0x78 SetPosition(temp). => `dir * dist + current` (scaled is left operand of fadd).
  10. SetFacingAngleXY (vtbl +0x108) with `angle + facing` computed straight into the push slot (NOT stored back to [0x10]) -> spell `angle + facing`, not `angle += facing`.

## 2. Name / signature
- Ego_r.pdb `?ReturnToDesiredPos@CTCKrakenTentacle@@AAEX_N@Z` (egor_pdb_names.tsv 0x007EAA70, egor_signatures.tsv) and FableWin.pdb same mangling (_fablewin_api.tsv 0x0182A4A6 / 0x0277F170; naming_stragglers FableWin.exe.signatures.tsv). Ego_r neighbourhood order TrackTarget 0x7EA700 -> ReturnToDesiredPos 0x7EAA70 -> QueueStrikeDamage 0x7EACE0 matches retail TrackTarget 0x7F0C70 -> 0x7F0F40 -> QueueStrikeDamage 0x7F1170. Manifest label is CORRECT.
- PRIVATE (`AAE`), `void ReturnToDesiredPos(bool)`, non-const, non-virtual. Param name: landed callers spell it `immediately` (SetFacingAngleAndDummy 0x7F15D0, FrameUpdate 0x7F14F0); old draft spells `immediate` (no PDB param name evidence either way).
- Callers (xref): 0x7F15A8 in CTCKrakenTentacle::FrameUpdate 0x007F14F0 (`ReturnToDesiredPos(false)`), 0x7F15EB in SetFacingAngleAndDummy 0x007F15D0 (`ReturnToDesiredPos(true)`).

## 3. Callees
| addr | landed row | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x00A01B50 (x5) | rebuild/src/compiled/00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | CBaseIntelligentPointer::GetPItem | same | OK |
| 0x004C73D0 | rebuild/src/compiled/00/4c/CThing_GetPosition_004c73d0.cpp | CThing::GetPos() const | ?GetPos@CThing@@QBEABVC3DVector@@XZ (FableWin) | WRONG: draft `GetPosition`; use CThing.h `const C3DVector& GetPos() const` |
| 0x004C08C0 | not landed | NGraphicAppearance::CTCGraphicAppearance::GetDummyObject | ?GetDummyObject@CTCGraphicAppearance@NGraphicAppearance@@QBE_NABVCCharString@@JKAAVC3DVector@@AAVCRightHandedSet@@M@Z | WRONG: draft uses pointers + non-const; PDB is const method, C3DVector& / CRightHandedSet& refs (header FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT) |
| 0x00A14480 | rebuild/src/compiled/00/a1/C3DVector_GetMagnitudeAndNormalise_00a14480.cpp | C3DVector::GetMagnitudeAndNormalise | ?GetMagnitudeAndNormalise@C3DVector@@QAEMXZ | WRONG: draft `Normalise` (that PDB name is the fast inline returning C3DVector&) |
| 0x00A441E0 | rebuild/src/compiled/00/a4/global_GFGetSignedAngleDifference_00a441e0.cpp | GFGetSignedAngleDifference | ?GFGetSignedAngleDifference@@YIMMM@Z (__fastcall, floats on stack, ret 8) | WRONG: draft `AngleDifference` __stdcall; declared in engine/GFAngles.h `float FABLE_FASTCALL GFGetSignedAngleDifference(float,float)` |
| vtbl +0xFC | - | CTCPhysicsBase::GetFacingAngleXY() const | PDB | draft non-const local CPhysicsBody |
| vtbl +0x78 | - | CTCPhysicsBase::SetPosition(const C3DVector&) | PDB | draft local |
| vtbl +0x108 | - | CTCPhysicsBase::SetFacingAngleXY(float) | PDB | draft local |
Slot check: engine/CTCPhysicsBase.h (FABLE_RETAIL_CTCPHYSICSBASE) has GetFacingAngleXY 33 decls after SetPosition and SetFacingAngleXY 36 after = 0xFC-0x78 /4 and 0x108-0x78 /4. Landed CTCPhysicsRigidBody_SetFacingAngleXY_0075bf40.cpp uses `FABLE_RETAIL_CTCBASE` + `FABLE_RETAIL_CTCPHYSICSBASE`.
Data: 0x0129BA3C DELTA_NOTIONAL_ZERO (C3DVector.h, under FABLE_RETAIL_C3DVECTOR); 0x01238E00 = 1/360 literal (0.0027777778f; PerformStrikeDamage cites it); 0x0122DEDC = 0.0f; 1.0f immediate push.

## 4. Members
- CTCKrakenTentacle (struct_layouts_egor.tsv, size 80): PTentacleDef +0x0C CDefPointer<const CKrakenTentacleDef>, PKraken +0x10 CIntelligentPointer<CThing>, PObstruction +0x18, PRippleEffect +0x20, CanStrike +0x28, Tracking +0x29, IsStriking +0x2A, QueuedStrikeTick +0x2C, LastStrikeTick +0x30, TickToRise +0x34, DesiredPos +0x38 C3DVector, FacingAngle +0x44 float, DummyName +0x48 CCharString, DoneDeathStuff +0x4C bool. NO shared header engine/CTCKrakenTentacle.h exists -> local class `: public CTCBase` is legitimate (all landed siblings do it). Reads: +0x04 (CTCBase::ParentThing), +0x0C, +0x10, +0x44, +0x48.
- CKrakenTentacleDef: shared header rebuild/include/engine/CKrakenTentacleDef.h (generated, no FABLE_RETAIL_* macros, pack(1)): TentacleTurnSpeed +0x2C, TentacleMoveSpeed +0x30, TentacleHeightOffset +0x3C (all float). Draft models it locally -> should include the header.
- CThing (engine/CThing.h): PhysicsTC +0x60 (CTCPhysicsBase*), PTCGraphicAppearance +0x64 (NGraphicAppearance::CTCGraphicAppearance*), `Dead:1` bitfield at +0x91 bit0 with `bool IsDead() const`, `GetPos()`, GetPhysicsTC(), GetTCGraphicAppearance(). Draft models CThing locally (Flags91, m_pPhysics, GraphicAppearance, GetPosition) -> shared header exists.
- CTCBase.h: ParentThing +0x04 (FABLE_RETAIL_CTCBASE gives the virtual model; GetParentThing() inline).
- C3DVector.h (FABLE_RETAIL_C3DVECTOR): operator-, operator*, operator+ return `const C3DVector`, IsNonZero variants: FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX gives `Z*Z+Y*Y+X*X > DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO` = retail order here (Z,Y,X). GetMagnitudeAndNormalise is NOT declared in C3DVector.h (landed callers 0x006701A0 / 0x008444A0 declare it in local C3DVector models) -- author must declare or check for a macro.
- CRightHandedSet.h (FABLE_RETAIL_CRIGHTHANDEDSET; _CTOR / _CTOR_INLINE / _DEFAULT_IDENTITY options) -- retail does NOT construct/initialise `orientation` (no stores before the call), so a trivial/no-op default ctor is needed.
- CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER, _COMPARE_NULL_INT, _DEREFERENCE, ...) + engine/ThingAlive.h (FABLE_RETAIL_IS_THING_ALIVE: `if(thing!=0){if(!thing->IsDead())return true;}return false;`) -> matches step 1.
- Draft-local classes that have shared headers: C3DVector, CThing, CTCGraphicAppearance, CRightHandedSet, CKrakenTentacleDef, CBaseIntelligentPointer/CIntelligentPointer, CPhysicsBody(=CTCPhysicsBase), CCharString (fable_string.h), CInterfaceMap (=engine/CVectorMap.h, and unused here).

## 5. Callee visibility
- All calls are out of line and non-inline in retail (GetPItem, GetPos, GetDummyObject, GetMagnitudeAndNormalise, GFGetSignedAngleDifference, three virtuals). No STLport helpers, no LowerBound/GetInterface in this function at all.
- Inline-expanded: IS_THING_ALIVE, IsDead, IsNonZero (ZYX), C3DVector operator-/*/+, copy ctor, CDefPointer->.
- Same-TU neighbours (CTCKrakenTentacle.cpp): TrackTarget 0x7F0C70 (not landed) defined just before; DoDieFX 0x7F0B10, OnInitialActivate 0x7F08B0 earlier. GFGetSignedAngleDifference (0xA441E0) / GetMagnitudeAndNormalise (0xA14480) are in other TUs -> declarations only.
- The draft's same-TU noinline LowerBound body (copied from DoDieFX) is irrelevant to this function (retail never calls 0x40F020 here) -- drop it.

## 6. Sibling rows (rebuild/src/compiled/00/7f/)
- CTCKrakenTentacle_QueueStrikeDamage_007f1170.cpp (next fn): local CTCKrakenTentacle model, includes engine/CThing.h, `PKraken.Get() && !PKraken.Get()->IsDead()` then unused `CThing* kraken = PKraken.Get();` -> same 3-GetPItem idiom; def local read before call. No pragmas.
- CTCKrakenTentacle_PerformStrikeDamage_007f11d0.cpp: `#define CTCBase CTCBaseLayout` trick, FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_CTHING_GETTC_EXPANDED, engine/CThing.h, engine/CVectorMap.h, local IS_THING_ALIVE, pack(1) CBaseIntelligentPointer (vtable 0x01238C6C), local CKrakenTentacleDef.
- CTCKrakenTentacle_FrameUpdate_007f14f0.cpp: FABLE_RETAIL_CTCBASE + engine/CTCBase.h, local CThing/GetInterface, CVectorMap.h; calls ReturnToDesiredPos(false).
- CTCKrakenTentacle_OnInitialActivate_007f08b0.cpp / DoDieFX_007f0b10.cpp: FABLE_RETAIL_CTCBASE, engine/C3DVector.h, `#pragma auto_inline(off/on)` around same-TU LowerBound.
- Shared-header donor using GetDummyObject + IS_THING_ALIVE: rebuild/src/compiled/00/8e/NCreatureModeCModeScorpionKingVulnerable_FrameUpdate_008e0a10.cpp (defines FABLE_RETAIL_CCHARSTRING, _CTBASE, _CTHING, _CINTELLIGENTPOINTER, _IS_THING_ALIVE, _CRIGHTHANDEDSET, _C3DVECTOR, _CTCGRAPHICAPPEARANCE_DUMMYOBJECT; includes fable_string.h, C3DVector.h, CRightHandedSet.h, CThing.h, ThingAlive.h, NGraphicAppearance_CTCGraphicAppearance.h).
- None of the kraken siblings carry a `// Flags:` line (default flags).

## 7. Old draft model errors (vs disasm)
- Old grade DIFFER 561v559, 55 difflines: dist/angle frame slots swapped, kraken GetFacingAngleXY reg choice, Z operand order in SetPosition sum, vtable-load scheduling (STATUS.tsv line 16, NOTES.md line 26).
- Squared-length order: draft LengthSquared `Y*Y+Z*Z+X*X`; retail is Z*Z, +Y*Y, +X*X -> use IsNonZero ZYX (`Z*Z+Y*Y+X*X`).
- Retail uses an IsNonZero test then GetMagnitudeAndNormalise / 0 -- draft wraps it in a local NormaliseSafe; fine semantically but use the header IsNonZero.
- Final call: retail passes `angle + facing` without storing to angle; draft `angle += facing` -- spell `angle + facing`.
- `dir` should be initialised from the subtraction (`C3DVector dir = desired - current;` with the header's copy ctor) -- draft declares then assigns.
- desired is a separate copy of GetPos (`C3DVector desired = current;` or from GetPos()) -- matches.
- The unused third GetPItem must stay (unused kraken local); draft has it.
- Draft's operator*= / operator+= / `Z = s*Z` are dead code; operator* local returns non-const by default-construct; header versions are `const C3DVector(X*s,...)`.
- GetDummyObject: draft passes pointers; retail ABI identical but use PDB refs + const.
- GFGetSignedAngleDifference is __fastcall (`YI`), draft __stdcall: same bytes for 2 floats but wrong name/convention; use GFAngles.h.
- Dead bit: draft `Flags91 & 1`; PDB `Dead:1` bitfield + IsDead() in CThing.h.

## 0x006B08F0

# DOSSIER 0x006B08F0  CTCPhysicsBase::OnSerialise (565 bytes)

## 1. Retail shape (xdis 006b08f0 0x245)
- __thiscall, `ret 8` (CPersistContext&, bool). No frame pointer (`sub esp,18h; push ebx/esi/edi`), no SEH prologue, no EH.
- Frame-less /O2 shape: esi=this, edi=&context, then edi=&Position (this+0xc) on the load arm. ebp pushed/popped locally on the
  save arm only (0x6b0918/0x6b0959).
- Save arm (Mode 1|3 at context+0x18, cmp 1 / cmp 3 = inline IsSaving): pos = Position copy (3 dwords to [esp+18..20]); TWO calls
  to 0x686D20 (first -> ebx = object of GetMapNumberAt, second -> ebp = object of GetMapPos; i.e.
  `PeekWorldMap().GetMapPos(PeekWorldMap().GetMapNumberAt(pos))`, outer object evaluated first); vslot16 (+0x40) then vslot23 (+0x5c,
  C2DCoordI returned through hidden ptr [esp+10]); `pos.X -= origin.X; pos.Y -= origin.Y` (fild/fsubr/fstp).
- Three Transfer<float> calls "PositionX/Y/Z" (0x0125D460/54/48) with the default 0.0f stored AFTER the pushes into the dead
  `context` arg slot ([esp+28] -> `mov [esp+34h],0`) = the GetDefaultVal<T>() two-arg Transfer pattern
  (FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS, `Transfer(name,value)` -> `Transfer(name,value,GetDefaultVal<T>())`; needs
  `template<> inline float FABLE_FASTCALL CPersistContext::GetDefaultVal<float>() { return 0; }` like CTCShapeManager 0x00762BA0).
- IsSaving re-tested (`mov edi,[edi+18h]`; 1/3 -> return). Load arm: `Position = pos` (copy via eax=edi=this+0xc);
  `if (GetSerialiseVersion() > 0)` (call 0x686A90, `test eax,eax; jle`) { PeekWorldMap(), ParentThing->CurrentMapNumber (movsx word
  +0x8e = GetCurrentMapNumber()), vslot23 GetMapPos; Position.X += origin.X ([edi]); Position.Y += origin.Y ([esi+10h]) }.
- `if (ParentThing->ThingTypeFlags & 0x20)` (test byte [+0x6c],20h). Then `PeekWorld()` (0x686A80) -> loads [+0x149] Z, [+0x145] Y,
  then `add eax,141h ; fld [eax]` X; squares X*X + Y*Y (faddp) + Z*Z (faddp); `fld DELTA_NOTIONAL_ZERO ; fmul DELTA_NOTIONAL_ZERO ;
  fcompp ; test ah,5 ; jp` = `C3DVector::IsNonZero()` on `PeekWorld().GetSaveGameMarkerPos()` (const C3DVector& - read in place,
  NO copy). Then 2nd PeekWorld(), marker copied to local [esp+0c..14] (`C3DVector marker = PeekWorld().GetSaveGameMarkerPos();`).
- Region test: left = GetRegionNumberMapIsIn(PeekWorldMap().GetMapNumberAt(marker)) (PeekWorldMap; vslot16 via the wm object; then
  2nd PeekWorldMap -> ecx -> call 0x4FC190); `mov ebx,eax`; then ParentThing->CurrentMapNumber is loaded+pushed BEFORE the 3rd
  PeekWorldMap call; call 0x4FC190; `cmp ebx,eax; jne`.
  NB: the GetRegionNumberMapIsIn object is a SEPARATE PeekWorldMap() call made after the arg's PeekWorldMap()+vcall -> source is
  `PeekWorldMap().GetRegionNumberMapIsIn(PeekWorldMap().GetMapNumberAt(marker))`; in the save arm the outer object call came FIRST
  (GetMapPos is virtual; GetRegionNumberMapIsIn non-virtual -> the compiler evaluates args first for the non-virtual call).
- Then `Position = marker` (via eax=edi), PeekWorldMap(); `mov esi,[esi+4]` (ParentThing) loaded before `push edi` (&Position);
  vslot16; `push eax ; mov ecx,esi ; call 0x4C7580` = `ParentThing->SetCurrentMapNumber(PeekWorldMap().GetMapNumberAt(Position))`.

## 2. Name / signature (two sources) - manifest label CORRECT
- Ego_r.pdb (egor_pdb_names.tsv 0x006ED890, 549 bytes) `?OnSerialise@CTCPhysicsBase@@UAEXAAVCPersistContext@@_N@Z`; FableWin
  (naming_stragglers 0x0246E620) same leaf; rtti-vtables.tsv CTCPhysicsBase vtbl 0x0125D2C4 slot 1 = 0x006B08F0; header
  engine/CTCPhysicsBase.h:174 `virtual void OnSerialise(CPersistContext &,bool);`.
- `void CTCPhysicsBase::OnSerialise(CPersistContext& context, bool)` public virtual, void, non-const.
- Callers: only vtable refs (xref: 0x0125D2C8 data) + calls from 0x007242A2 / 0x0075A362 (derived OnSerialise, e.g.
  CTCPhysicsStandard vtbl 0x01265F84 slot1 = 0x00724290 calling the base).

## 3. Callees (landed file / landed name / PDB name)
| addr | landed row | PDB name | draft spelling |
|---|---|---|---|
| 0x00686D20 | 00/68/CTCBase_PeekWorldMap_00686d20.cpp | ?PeekWorldMap@CTCBase@@IBEABVCWorldMap@@XZ (Ego_r 0x006B2110), `const CWorldMap&` | WRONG: draft "GetWorldMap" (CTCBase::GetWorldMap is 0x00686D30; CTCPhysicsBase::GetWorldMap is 0x006AF620/630) |
| 0x00686A80 | 00/68/CTCBase_PeekWorld_00686a80.cpp | ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ, `const CWorld&` | WRONG: draft "GetWorld" (CTCBase::GetWorld is 0x00686A70) |
| 0x00686A90 | 00/68/CTCBase_GetSerialiseVersion_00686a90.cpp | ?GetSerialiseVersion@CTCBase@@IBEJXZ | ok (needs FABLE_RETAIL_CTCBASE_SERIALISEVERSION) |
| 0x00410620 | 00/41/CPersistContext_Transferfloat_00410620.cpp | ??$Transfer@M@CPersistContext@@QAEXPBDAAMABM@Z | ok, but call via the 2-arg inline Transfer(name,value) |
| 0x004FC190 | 00/4f/CWorldMap_GetRegionNumberMapIsIn_004fc190.cpp | ?GetRegionNumberMapIsIn@CWorldMap@@QBEJJ@Z | ok |
| 0x004C7580 | 00/4c/CThing_SetCurrentMapNumber_004c7580.cpp (old fastcall stub `CThing_SetCurrentMapNumber`) | ?SetCurrentMapNumber@CThing@@QAEXJ@Z (arg long) | draft param `unsigned short` WRONG -> `long`; landed row spelling is a NAME_FIXES candidate |
| vslot16 +0x40 | CWorldMap 0x004FC0C0 | ?GetMapNumberAt@CWorldMap@@UBEJABVC3DVector@@@Z (const) | draft "GetMapNumberAt(const C3DVector&)" non-const - fix const |
| vslot23 +0x5c | 00/4f/CWorldMap_GetMapPos_004fc690.cpp | ?GetMapPos@CWorldMap@@UBE?AVC2DCoordI@@J@Z | ok |
| global 0x0129BA3C | - | ?DELTA_NOTIONAL_ZERO@@3MB (C3DVector.h:26) | WRONG: draft "g_FloatEpsilon" |
| inline | - | ?GetSaveGameMarkerPos@CWorld@@QBEABVC3DVector@@XZ (FableWin 0x0184F602 thunk; returns const&) | draft returned by value - WRONG |
| inline | - | ?IsNonZero@C3DVector@@QBE_NXZ (FableWin 0x01B4F100) | draft "LengthSquared() > eps*eps" - use IsNonZero |
| inline | - | ?IsSaving@CPersistContext@@QBE_NXZ (Ego_r 0x00466130) | ok (header inline, Mode==1||Mode==3) |

## 4. Members
- CTCPhysicsBase (engine/CTCPhysicsBase.h, FABLE_RETAIL_CTCBASE + FABLE_RETAIL_CTCPHYSICSBASE): `C3DVector Position` +0x0c
  (struct_layouts_egor: CTCPhysicsBase 12 C3DVector Position). Default (non-retail) branch flattens Position_X/Y/Z - use the retail branch.
- CTCBase (engine/CTCBase.h): `CThing* ParentThing` +0x04 (public), PeekWorld/PeekWorldMap protected decls at lines 150-151 (no macro
  beyond FABLE_RETAIL_CTCBASE); GetSerialiseVersion under FABLE_RETAIL_CTCBASE_SERIALISEVERSION.
- CThing (engine/CThing.h, FABLE_RETAIL_CTHING): `unsigned long ThingTypeFlags` (retail +0x6c; PDB +0x7c), inline `GetTypeFlags()`
  line 423; `short CurrentMapNumber` (retail +0x8e) with inline `long GetCurrentMapNumber() const` line 278 (movsx = the long return);
  `void SetCurrentMapNumber(long)` line 463 under FABLE_RETAIL_CTHING_STATEACCESS. Bit 0x20 written as plain 0x20 in landed rows.
- CWorld (engine/CWorld.h, FABLE_RETAIL_CWORLD): SaveGameMarkerPos at +0x141 modelled as `unsigned char SaveGameMarkerPos[0xf]`
  (unaligned C3DVector; Ego_r layout CWorld 337=0x151 C3DVector SaveGameMarkerPos). NO GetSaveGameMarkerPos accessor/opt-in exists:
  the author must add one (e.g. a new nested FABLE_RETAIL_CWORLD_SAVEGAMEMARKERPOS: `const C3DVector& GetSaveGameMarkerPos() const
  { return *(const C3DVector*)SaveGameMarkerPos; }` or a pack(1) C3DVector member; FABLE_RETAIL_C3DVECTOR_PACKED exists in C3DVector.h:72).
- CPersistContext (engine/CPersistContext.h, FABLE_RETAIL_CPERSISTCONTEXT): Mode +0x18; IsSaving inline (line 34);
  FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS (line 92) gives the 2-arg Transfer.
- CWorldMap (engine/CWorldMap.h, FABLE_RETAIL_CWORLDMAP): GetMapNumberAt(const C3DVector&) const = slot16, GetMapPos(long) const = slot23.
- C3DVector IsNonZero variants (C3DVector.h:385-401): retail sums X*X+Y*Y+Z*Z with Z,Y,X loaded first -> try
  FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ (`X*X+Y*Y+Z*Z>D*D`, line 395; per C3DVector.h:189 the XYZ one-line sum loads Z,Y,X) ;
  alternatives ISNONZERO_ZYX (line 389) / default (X,Z,Y).
- Locally-modelled classes in the draft that HAVE shared headers: C3DVector, C2DCoordI (engine/C2DCoordI.h), CPersistContext,
  CWorldMap, CWorld, CThing, CTCPhysicsBase (all must come from engine/ headers).

## 5. Visibility
- PeekWorldMap/PeekWorld/GetSerialiseVersion are out-of-line protected CTCBase members (different TU, 0x00686xxx) - declarations only.
- CWorldMap::GetRegionNumberMapIsIn 0x004FC190 out of line (other TU). CThing::SetCurrentMapNumber 0x004C7580 out of line.
- Transfer<float> 0x00410620 out-of-line template instance; the 2-arg Transfer and GetDefaultVal are inline.
- Inline in this TU: IsSaving, GetSaveGameMarkerPos, IsNonZero, GetCurrentMapNumber, GetTypeFlags, C3DVector copy.
- No STLport helpers called.

## 6. Siblings
- 00/76/CTCShapeManager_OnSerialise_00762ba0.cpp (closest idiom: PeekWorldMap().GetMapPos(PeekWorldMap().GetMapNumberAt(pos)),
  2-arg Transfer with GetDefaultVal specialisation; defines FABLE_RETAIL_CCHARSTRING, _CCLASS, _DEF_CLASS_BASE, _CTCBASE, _CWORLD,
  _CTHING, _CPERSISTCONTEXT, _CPERSISTCONTEXT_NAMED_DEFAULTS, _CWORLDMAP; `// Flags: none`).
- 00/6b/CTCPhysicsBase_InitialActivate_006b0b30.cpp (next function, same TU): FABLE_RETAIL_CTCBASE, _CTCPHYSICSBASE,
  _CTCPHYSICSBASE_MAPWHOPOSITION, _CTHING, _C3DVECTOR, ...
- 00/6b/CTCPhysicsBase_SetPhysicsMeshInfoFromGraphic_006b08e0.cpp (previous function): FABLE_RETAIL_CTCBASE, _CTCPHYSICSBASE, ...
- 00/6b/CTCPhysicsBase_OnCreate_006b0780.cpp, _SetPosition_006b0c10.cpp (older local-model style).
- No #pragma optimize needed (no frame pointer).

## 7. Old draft model errors
- Called "GetWorldMap"/"GetWorld" instead of CTCBase::PeekWorldMap / PeekWorld (bytes same; names wrong).
- GetSaveGameMarkerPos returned BY VALUE + LengthSquared(): retail reads the marker in place through the const& (+0x141 base via
  `add eax,141h`) and uses IsNonZero against DELTA_NOTIONAL_ZERO^2 - the residue "marker LengthSquared loads X,Y,Z" is this.
- Second marker read: `C3DVector marker = PeekWorld().GetSaveGameMarkerPos();` (copy from the const&).
- Region compare: use `PeekWorldMap().GetRegionNumberMapIsIn(...)` for both sides (no reordering); right side
  `ParentThing->GetCurrentMapNumber()` (long, inline).
- SetCurrentMapNumber param is long; GetMapNumberAt is const; all classes local (must use shared headers).

## 0x00731EA0

# DOSSIER 0x00731EA0 -- CTCSpotLight::OnAppearanceDraw (575 bytes)

## 1. Retail shape (xdis 00731ea0 0x24f)
- thiscall, `ret 8` (2 args), NO frame pointer (`sub esp,68h ; push ebx,ebp,esi,edi`), NO SEH prologue even though a
  CPrimitive temporary with a dtor (0x004C4160) is built -> same as landed sibling CTCLight::DrawFlare 0x00641AE0 (no SEH,
  `sub esp,58h`, default flags). Frame 0x68. Siblings carry no `// Flags:` / pragmas -> default TU flags.
- ecx = `this` of the NGraphicAppearance::CAGraphicAppearanceComponent subobject (+0x0C): edi=ecx, esi=`lea [edi-0Ch]`
  = full CTCSpotLight. Only xref: vtable 0x01266FA4 slot 0 (rtti-vtables.tsv line 5143; slot 1 = OnAppearanceClear 0x00731AA0).
  => must be a real override in a class `CTCSpotLight : CTCBase, NGraphicAppearance::CAGraphicAppearanceComponent`.
- ebx = 0 register (xor ebx,ebx) used for `LightPrimitive == 0` (cmp eax,ebx), the CPrimitive temp zero stores, and
  `cmp al,bl` on Overridden in the colour select (later selects use `test al,al`).
- Flow: (1) if !LightPrimitive: CPrimitive temp {Type=3 @+30, CBaseClass ctor 0x99A2D0 on +34, vptr 0x0122F598
  (CEnginePrimitiveHandle), Data=0 @+38, BankIndex=0 @+3C}; LightPrimitive = appearance.AddPersistentPrimitive(temp) [0x4C0050,
  ecx=appearance arg]; temp.~CPrimitive() [0x4C4160]. (2) colour = Overridden ? &PLightInfo->Colour(+0x14) : &PLightDef->Colour(+0x28);
  bytes read +2,+1,+0,+3 (R,G,B,A) each `movzx;fild;fmul 1/255 [0x01231724]` = inline CRGBFloatColour(const CRGBColour&).
  (3) flicker = (1.0f[0x0122DED8] - info.Interpolate) * OldAccumilativeFlickerFactor + AccumilativeFlickerFactor * info.Interpolate,
  stored into the `info` param slot [esp+80h]. (4) R,G,B,A *= flicker (operator*(float)), then each *255.0f [0x01230014] -> __ftol2
  0xBFEA70, order R->byte+2, G->+1, B->+0, A->+3 (inline GetRGBColour). (5) three selects into locals in order:
  [E+14]=Overridden?Info->Width(+4):Def->Width(+0x34); [E+18]=Info->InnerRadius(+8):Def->InnerRadius(+0x2C);
  [E+1C]=Info->OuterRadius(+0xC):Def->OuterRadius(+0x30) (= inline GetWidth/GetInnerRadius/GetOuterRadius, each re-reads Overridden).
  (6) ParentThing([edi-8]=+4)->GetPos() 0x4C73D0, result COPIED to a local C3DVector (inline CTCSpotLight::GetPosition() returns by value).
  (7) ParentThing re-read, UniqueID +0x18/+0x1C -> ebx:ebp. (8) push -1, push 0, [esp+4]=Width*0.5f[0x0122F59C],
  [esp]=InnerRadius*flicker, push OuterRadius(dword), push &colourBytes, GetDirection(hidden ret) 0x731CA0 (this=esi, ret 4),
  push &dir, push &pos, push id hi/lo, ecx=&local(0x38 bytes) call 0x7324A0. (9) appearance.SetPrimitiveGraphics(LightPrimitive,
  prim, 0) 0x4BD620. return true (`mov al,1`).
- Call = `CEnginePrimitiveShadowedSpotLight(id, pos, dir, colour, OuterRadius, InnerRadius*flicker, Width*0.5f, (EEngineInterpolationSet)0, 0xFFFFFFFF)`.

## 2. Name / signature (two sources)
- Ego_r.pdb egor_pdb_names.tsv 0x00749280 and FableWin _fablewin_api.tsv 0x017C866B / 0x02581B40:
  `?OnAppearanceDraw@CTCSpotLight@@UAE_NAAVCTCGraphicAppearance@NGraphicAppearance@@ABVCGraphicAppearanceDrawInfo@3@@Z`
  = `virtual bool CTCSpotLight::OnAppearanceDraw(NGraphicAppearance::CTCGraphicAppearance&, const NGraphicAppearance::CGraphicAppearanceDrawInfo&)` (non-const).
  Vtable 0x01266FA4 slot 0 = CAGraphicAppearanceComponent::OnAppearanceDraw (header slot 0). Manifest label is CORRECT. Leaf: OnAppearanceDraw.
- Inline helpers that exist in both PDBs (FableWin out-of-line, Ego_r 0x748CE0..0x748F10; NOT called in retail -> inlined):
  `?GetColour@CTCSpotLight@@QAE?AVCRGBColour@@XZ` (non-const, returns CRGBColour BY VALUE), `?GetWidth@..QAEMXZ`,
  `?GetInnerRadius@..QAEMXZ`, `?GetOuterRadius@..QAEMXZ`, `?GetPosition@CTCSpotLight@@QAE?AVC3DVector@@XZ` (by value),
  `?GetDirection@CTCSpotLight@@QAE?AVC3DVector@@XZ` (out of line 0x731CA0). Also `??C?$CCountedPointer@VCSpotLightInfo..QBE` (operator->)
  and `??C?$CDefPointer@$$CBVCSpotLightDef@@@@QBEPBVCSpotLightDef@@XZ` (operator->).

## 3. Callees
| addr | landed file | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x0099A2D0 | 00/99/CBaseClass_CBaseClass_0099a2d0.cpp | CBaseClass::CBaseClass | (CBaseClass ctor; base of CEnginePrimitiveHandle) | draft `CBase()` WRONG |
| 0x004C0050 | 00/4c/NGraphicAppearanceCTCGraphicAppearance_AddPersistentPrimitive_004c0050.cpp | CTCGraphicAppearance::AddPersistentPrimitive(const CPrimitive&) | ?AddPersistentPrimitive@CTCGraphicAppearance@NGraphicAppearance@@QAEPAVCPrimitive@2@ABV32@@Z | draft `InsertAppearanceNode(CAppearanceNodeSpec)` WRONG |
| 0x004C4160 | 00/4c/SPrimitiveHandleHolder004c4160_..._004c4160.cpp | made-up `SPrimitiveHandleHolder_004c4160::~` | real: ??1CPrimitive@NGraphicAppearance@@QAE@XZ (FableWin 0x017C5E2F; body = destroy handle member at +4 of 16-byte non-polymorphic CPrimitive) -> NAME_FIXES candidate | draft `~CAppearanceNodeSpec` WRONG |
| 0x00BFEA70 | 00/bf/global_ftol2_00bfea70.cpp | __ftol2 | CRT | compiler-generated |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp | CThing::GetPos() const (const C3DVector&) | inline, no PDB out-of-line | draft `GetPosition` -> use `GetPos` (engine/CThing.h line ~411, under FABLE_RETAIL_CTHING) |
| 0x00731CA0 | 00/73/CTCSpotLight_GetDirection_00731ca0.cpp | CTCSpotLight::GetDirection() const | ?GetDirection@CTCSpotLight@@QAE?AVC3DVector@@XZ (non-const in PDB; landed declares const) | ok |
| 0x007324A0 | 00/73/CEnginePrimitiveLine_CEnginePrimitiveLine_007324a0.cpp | CEnginePrimitiveLine::CEnginePrimitiveLine (10 args incl BankIndex/BlendMode/BlendOp) | REAL: ??0CEnginePrimitiveShadowedSpotLight@@QAE@_KABVC3DVector@@1ABVCRGBColour@@MMMW4EEngineInterpolationSet@@K@Z (Ego_r 0x0074A620 right after CTCSpotLight fns; FableWin 0x02581D70). Evidence: PDB ShadowedSpotLight size 56=0x38 (retail object 0x38; PDB Line is 60), members WorldPos+0C Direction+18 Colour+24 Radius+28 SpotRadius+2C ConeRadius+30 LightingChannelMask+34 match the stores; this caller passes 3 floats where the "Line" model has float,ulong,long. -> NAME_FIXES (landed name wrong) | draft `CEnginePrimitiveParticleLine` WRONG (that ctor is 0x00AFE220) |
| 0x004BD620 | 00/4b/NGraphicAppearanceCTCGraphicAppearance_SetPrimitiveGraphics_004bd620.cpp | CTCGraphicAppearance::SetPrimitiveGraphics(CPrimitive*, const CEnginePrimitive&, unsigned short) | ?SetPrimitiveGraphics@...QAEXPAVCPrimitive@2@ABVCEnginePrimitive@@G@Z | draft passes CEnginePrimitive* (wrong: const ref) |

## 4. Members
- CTCSpotLight (struct_layouts_egor, size 0x2C; retail = PDB): PLightInfo CCountedPointer<CTCSpotLight::CSpotLightInfo> +0x10 (Data)/+0x14,
  PLightDef CDefPointer<const CSpotLightDef> +0x18, Overridden bool +0x1C, LightPrimitive CPrimitive* +0x20,
  AccumilativeFlickerFactor float +0x24, OldAccumilativeFlickerFactor float +0x28. CTCBase::ParentThing +0x04.
  Shared header engine/CTCSpotLight.h is FLATTENED (no bases, pads at +4..+0xC, members PLightInfo_Data/PLightInfo_Info/PLightDef_Object;
  only opt-in FABLE_RETAIL_CTCSPOTLIGHT_VSLOTS which just adds 3 virtual decls). It cannot carry CTCBase + CAGraphicAppearanceComponent bases,
  which this row needs (this = +0xC subobject). Landed precedent: CTCSpotLight ctor 0x00731D40 states "The header's flattened pads cannot carry
  the two bases, so this is a local model with the same offsets" and declares `class CTCSpotLight : public CTCBase, public NGraphicAppearance::CAGraphicAppearanceComponent`.
  Better: model like engine/CTCLight.h (`class CTCLight:public CTCBase,public NGraphicAppearance::CAGraphicAppearanceComponent`) including
  engine/CTCBase.h (FABLE_RETAIL_CTCBASE) + engine/NGraphicAppearance_CAGraphicAppearanceComponent.h.
- CSpotLightInfo (PDB 0x18): Angle+0, Width+4, InnerRadius+8, OuterRadius+0xC, Flicker+0x10, Colour CRGBColour+0x14 (no shared header; local in OnCreate/OnSerialise).
- CSpotLightDef (PDB 0x44, engine/CSpotLightDef.h exists): Colour+0x28, InnerRadius+0x2C, OuterRadius+0x30, Width+0x34, Flicker+0x38, FlickerSpeed+0x3C, Angle+0x40.
- CGraphicAppearanceDrawInfo (PDB 36 bytes): Interpolate float +0 (draft calls it Interpolation). No shared header (only fwd decls).
- CThing: UniqueID unsigned __int64 +0x18 (engine/CThing.h, `GetUniqueID()` inline, static assert), GetPos() under FABLE_RETAIL_CTHING; CTCBase::GetParentThing() inline (engine/CTCBase.h:143).
- NGraphicAppearance::CPrimitive: engine/NGraphicAppearance_CPrimitive.h (Type+0, CEnginePrimitiveHandle+4, BankIndex+0xC, size 16;
  ctor CPrimitive(EPrimitiveType, long bank_index=0), ~CPrimitive() declared) -- exactly the retail temp.
- CTCGraphicAppearance: engine/NGraphicAppearance_CTCGraphicAppearance.h, FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FLARE gives AddPersistentPrimitive / SetPrimitiveGraphics.
- CRGBFloatColour: engine/CRGBFloatColour.h (forceinline ctor from CRGBColour; operator*(float); FABLE_RETAIL_CRGBFLOATCOLOUR -> forceinline GetRGBColour()).
  CRGBColour: engine/CRGBColour.h (FABLE_RETAIL_CRGBCOLOUR / _DWORD_COPY variants of copy ctor). C3DVector: engine/C3DVector.h (FABLE_RETAIL_C3DVECTOR).
- CEnginePrimitive: engine/CEnginePrimitive.h (FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR). No header for CEnginePrimitiveShadowedSpotLight
  (engine/CEngineInternalPrimitiveShadowedSpotLight.h is the internal class); engine/CEnginePrimitiveLine.h is the real Line (different ctor).
- Draft LOCAL models with shared headers: C3DVector, CRGBColour, CRGBFloatColour, CThing, CTCBase, CBase(=CBaseClass), CTCSpotLight,
  CTCGraphicAppearance, CAGraphicAppearanceComponent, CPrimitive (as CAppearanceNodeSpec/Data), CEnginePrimitive, CSpotLightDef.

## 5. Callee visibility
- Out of line in retail (call targets): AddPersistentPrimitive, ~CPrimitive (0x4C4160), CBaseClass ctor, GetPos (inline accessor emitted
  out of line -> declare only, as landed callers do), GetDirection (same TU, defined earlier at 0x731CA0 but NOT inlined -> keep a
  declaration only / it is too big), ShadowedSpotLight ctor (0x7324A0, same TU neighbourhood, out of line -> declaration only, or
  __declspec(noinline) as engine/CEnginePrimitiveLine.h line 24 does), SetPrimitiveGraphics.
- Expanded inline (no call): GetColour, GetWidth, GetInnerRadius, GetOuterRadius, GetPosition (CTCSpotLight, by value: copies GetPos()),
  CRGBFloatColour(const CRGBColour&), operator*(float), GetRGBColour, CCountedPointer/CDefPointer operator->, GetParentThing, GetUniqueID,
  CEnginePrimitiveHandle ctor (vptr store 0x0122F598 + Data=0 after out-of-line CBaseClass ctor).
- No STLport helpers involved.

## 6. Sibling rows (same class / neighbourhood)
- 00/64/CTCLight_DrawFlare_00641ae0.cpp -- best idiom donor (AddPersistentPrimitive(CPrimitive((EPrimitiveType)3)), SetPrimitiveGraphics(prim, CEnginePrimitiveX(GetParentThing().GetUniqueID(),...),0)).
  Macros: FABLE_RETAIL_BASE_OBJECT, _CCLASS, _DEF_CLASS_BASE, _CTCBASE, _CTHING, _CLIGHTDEF, _C3DVECTOR, _CENGINEPERSISTENTPRIMITIVEBASE,
  _CIVCOUNTEDPOINTER, _CTCGRAPHICAPPEARANCE_MOVEMENT, _CTCGRAPHICAPPEARANCE_FLARE, _CENGINEPRIMITIVE_ID_CTOR; includes CThing.h, CTCLight.h,
  NGraphicAppearance_CTCGraphicAppearance.h. Flags: none.
- 00/73/CTCSpotLight_CTCSpotLight_00731d40.cpp (ctor; local CTCSpotLight with both bases, explains why).
- 00/73/CTCSpotLight_GetDirection_00731ca0.cpp (local CTCSpotLight flat model; `Overridden ? PLightInfo_Data->Angle : PLightDef_Object->Angle`).
- 00/73/CTCSpotLight_FrameUpdate_00731bf0.cpp, _OnCreate_00731e20.cpp, _OnSerialise_007320e0.cpp, _EditorDrawPreparePrimitivesForRendering_00731ab0.cpp
  (uses #define FABLE_RETAIL_CTCSPOTLIGHT_VSLOTS + engine/CTCSpotLight.h). None has Flags/pragma lines.
- Untouched: CTCLight::OnAppearanceDraw 0x00641C30 is NOT landed (manifest-gaps).

## 7. Old draft model errors
- Wrong callee names/types: CBase, CAppearanceNodeSpec/Data, InsertAppearanceNode, CEnginePrimitiveParticleLine; SetPrimitiveGraphics takes const CEnginePrimitive&.
- Flat local structs instead of shared headers; CTCBase lacks proper layout.
- GetColour returns a reference; PDB returns CRGBColour by value (non-const QAE accessors). Draft names GetA/GetB/GetC = GetWidth/GetInnerRadius/GetOuterRadius.
- Draft `C3DVector pos = ParentThing->GetPosition()` - retail is consistent with inline CTCSpotLight::GetPosition() by value (copy into a local); id via GetParentThing().GetUniqueID() re-reading ParentThing after the call.
- Residue (STATUS.tsv: DIFFER 553v575, frame 0x78 vs 0x68, ebp kept for appearance param, colour bytes via movzx instead of mov cl/dl/bl/al then movzx):
  retail colour conversion loads all four bytes to 8-bit regs first then movzx -> the CRGBColour is read as a by-value copy (GetColour() returns by value);
  the draft builds an extra named CRGBFloatColour + scaled copy + named drawColour (frame bloat). Likely source:
  `CRGBColour colour = (CRGBFloatColour(GetColour()) * flicker).GetRGBColour();` with flicker a named float, then width/inner/outer named floats.
- Order of ftol R,G,B,A (not A first as in AddAbilityToList) -- check GetRGBColour vs explicit CRGBColour(r,g,b,a) form when compiling.

## 0x009341C0

# DOSSIER 0x009341C0 CAIStateGroup_MinionKidGroupTag::TagTarget (580 bytes)

## 1. Retail shape (xdis 009341c0 0x254)
- __thiscall, no stack args, plain `ret` (0x00934403). NO frame pointer (sub esp,0xC4 then push ebx/esi/edi) -> default /Oy, no
  optimize("y",off). NO SEH prologue (no EH state: the CCharString / action temporaries are destroyed without unwind table ->
  either /EHs-c region or no throwing calls between; siblings in this TU have no Flags line). Not size-optimised.
- Locals (relative to esp after the 3 pushes): +0x00 dx spill / later fps copy / CCharString "TAG"; +0x07 bool reached;
  +0x08/+0x0C target X,Y (ONLY 8 bytes copied -> a C2DVector built from target->PeekPos(), not a C3DVector copy) / later CSoundPair;
  +0x10..+0xC4 CCreatureAction_PlayAnimation (0xB4 bytes).
- Flow: target = GetNearestOtherActiveMember(creature) -> return if null or dead (+0x91 bit0) -> MoveToThingProximity(1, IP(target),
  0.5f, reached, 6, true,false,true,true, 0.0f) != 2 -> return -> reach test #1 (out of reach -> return) -> LookAtThing(1, IP(target),
  1, 0.013888889f /0x3c638e39/) == 1 -> jump to Terminate tail (0x009343F3) -> reach test #2 -> fps/frame test -> SetTagger(target) ->
  CCharString("TAG",-1) -> CSoundPair zeroed inline AFTER the string -> PlayAnimation(creature,"TAG",sound,false,true,false,0,0)
  -> PerformActionAndWait(action,false) -> bl=(ret==1) via dec/neg/sbb/inc -> ~CCreatureActionBase 0x693EF0 -> ~CCharString
  -> if bl Terminate(). Single epilogue at 0x009343FA; Terminate block at 0x009343F3 shared by both Terminate paths (LookAt==1 jumps
  straight there), i.e. one Terminate() call site: `if (LookAt(..)==1 || done) Terminate();` style or goto-merged by the optimizer.
- The `push 0` at 0x00934399 is PerformActionAndWait's `wait=false` (pushed before the ctor args); ctor itself takes 8 args (ret 0x20).
- Reach test (twice, identical): target X,Y copied (C2DVector from PeekPos), creature PeekPos via GetCreature() re-read each time
  ([esi+4]->[+0x18]); fld here.X, here.Y; dx=t.X-h.X spilled to [esp+0], dy=t.Y-h.Y kept in st; THEN radius:
  GetCreature().PhysicsTC(+0x60)->Radius(+0x34) + 0.5f (0x01292294); then dy*dy, then dx*dx, faddp (=> Y squared first = 
  C2DVector GetSquaredMagnitude _YX ordering, macro FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX); then reach*reach; fcompp
  st0=reach^2 vs st1=lenSq; `test ah,1 ; jne return` => continue iff reach^2 >= lenSq, i.e. `lenSq <= reach*reach` (operands
  swapped by fcompp). C source candidates: `if (!(delta.GetSquaredMagnitude() <= reach*reach)) return;` or positive nested
  `if (lenSq <= reach*reach) { ... }`. The draft's `lenSq > reach*reach` gave test ah,5/jnp (wrong).
  Note radius is loaded AFTER the delta (dx spill) but BEFORE the squares: `C2DVector delta = ...; float reach = radius+0.5f;`
  then compare.
- Time test: fps (0x01375550 = CMainGameComponent::ConstantFPS, read via inline static GetConstantFPS()) copied to a stack int
  BEFORE the call; TaggedFrame ([PTCGroupTag+0x30]) into ebx BEFORE the call; CWorld::GetFrame() 0x0049D870 (direct call, no
  PeekWorld); eax-ebx -> unsigned->float (fild + 2^32 0x0122DCB4 fix); fild fps; fadd st,st (2.0f*fps); fcompp st0=2fps st1=elapsed;
  `test ah,5 ; jp return` => continue iff elapsed > 2*fps. Source e.g.
  `if ((float)(unsigned long)(CWorld::GetFrame() - tag.TaggedFrame) > 2.0f * CMainGameComponent::GetConstantFPS())` -- the
  early fps load suggests fps evaluated first (local `long fps = GetConstantFPS();` before the subtraction, as draft did).

## 2. Identity
- Name CONFIRMED: Ego_r.pdb `?TagTarget@CAIStateGroup_MinionKidGroupTag@@AAEXXZ` (Ego_r 0x009578A0) and FableWin.pdb same leaf
  (0x02B682E0). PRIVATE, void, no params, non-const. Manifest label correct.
- Mangled leaf: ?TagTarget@CAIStateGroup_MinionKidGroupTag@@AAEXXZ
- Second source: TU order. Retail class run 0x00934090 InitStateGroup, 0x009340F0 OnReplace, 0x00934100 IsStillValid, 0x00934120
  OnActivate, 0x009341C0 TagTarget, 0x00934410 EvadeTarget, 0x009347B0 Cleanup, 0x00934870 Update, 0x00934920 IsRunnable (all
  landed except TagTarget/EvadeTarget); landed Update 0x00934870 calls 0x009341C0 as TagTarget (xref: only caller 0x009348E5).
  Not virtual (no vtable slot).
- Related inline accessor: FableWin `?GetTCGroupTag@CAIStateGroup_MinionKidGroupTag@@AAEAAVCTCCreatureGroupTag@@XZ` (private,
  returns reference; absent from Ego_r => inlined in release). Retail reads [esi+0x18] directly; using GetTCGroupTag() or
  PTCGroupTag-> is codegen-equivalent.

## 3. Callees (landed file / landed name / PDB)
| addr | landed row | PDB leaf | draft spelling issue |
|---|---|---|---|
| 0x00812CA0 | 00/81/CTCCreatureGroup_GetNearestOtherActiveMember_00812ca0.cpp | ?GetNearestOtherActiveMember@CTCCreatureGroup@@QBEPBVCThingAICreature@@PBV2@@Z (const, returns const CThingAICreature*, param const CThingAICreature*) | draft: member of CTCCreatureGroupTag, non-const, non-const types -> WRONG |
| 0x00A01B10 | 00/a0/CBaseIntelligentPointer_CBaseIntelligentPointer_00a01b10.cpp | ??0CBaseIntelligentPointer@@QAE@XZ | ok |
| 0x00A01B90 | 00/a0/CBaseIntelligentPointer_SetPItem_00a01b90.cpp | ?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z | draft param const CThing* -> CBaseObject* |
| vtbl 0x01237F1C | CIntelligentPointer<const CThing> | inline ctor `CIntelligentPointer(T*){SetPointer(p);}` in engine/CIntelligentPointer.h | draft local struct |
| 0x0097E380 | NOT landed | ?AIAction_MoveToThingProximity@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@$$CBVCThing@@@@MAA_NW4EMovementType@@_N444M@Z | draft: long returns/args, bool* reached -> must be EAIActionReturn(EAIActionRepeat, CIntelligentPointer<const CThing>, float, bool&, EMovementType, bool,bool,bool,bool, float) |
| 0x006A4D00 | 00/6a/CThingPhysical_PeekPos_006a4d00.cpp | ?PeekPos@CThingPhysical@@QBEABVC3DVector@@XZ | draft `CThingCreatureBase::GetPosition` -> WRONG name/class |
| 0x0097D6F0 | 00/97/CAIStateGroupBase_AIActionLookAtThing_0097d6f0.cpp | ?AIAction_LookAtThing@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@$$CBVCThing@@@@JM@Z | draft long types |
| 0x0049D870 | 00/49/CWorld_GetFrame_0049d870.cpp | ?GetFrame@CWorld@@SIJXZ (static __fastcall, returns long) | draft `CWorld::GetWorldFrame` unsigned -> WRONG |
| 0x008137B0 | 00/81/CTCCreatureGroupTag_SetTagger_008137b0.cpp (landed leaf `?SetTagger@CTCCreatureGroupTag@@QAE_NPBVCThing@@@Z`) | ?SetTagger@CTCCreatureGroupTag@@QAE_NPBVCThingAICreature@@@Z (Ego_r + FableWin) | landed param type CThing* disagrees with both PDBs -> NAME_FIXES candidate; draft non-const CThingAICreature* |
| 0x0099EBF0 | 00/99/CCharString_Constructor_0099ebf0.cpp | ??0CCharString@@QAE@PBDJ@Z | ok (use fable_string.h FABLE_RETAIL_CCHARSTRING) |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ??1CCharString@@QAE@XZ | ok |
| 0x008424F0 | 00/84/CCreatureActionPlayAnimation_CCreatureActionPlayAnimation_008424f0.cpp (ret 0x20) | ??0CCreatureAction_PlayAnimation@@QAE@AAVCThingCreatureBase@@ABVCCharString@@ABVCSoundPair@@_N33JJ@Z | draft first param pointer -> CThingCreatureBase& |
| 0x00693EF0 | 00/69/CCreatureActionBase_CCreatureActionBase_00693ef0.cpp | ??1CCreatureActionBase@@UAE@XZ (called directly = PlayAnimation dtor implicit/inline -> FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR) | ok |
| 0x0097DB70 | 00/97/CAIStateGroupBase_AIActionPerformActionAndWait_0097db70.cpp | ?AIAction_PerformActionAndWait@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@ABVCCreatureActionBase@@_N@Z | draft took pointer -> const CCreatureActionBase& |
| 0x008FCF40 | 00/8f/CAIStateGroupBase_Terminate_008fcf40.cpp | ?Terminate@CAIStateGroupBase@@QAEXXZ | ok |
| data 0x01375550 | -- | ?ConstantFPS@CMainGameComponent@@1JA via inline ?GetConstantFPS@CMainGameComponent@@SIJXZ (engine/CMainGameComponent.h:183) | draft `g_framesPerSecond` -> WRONG |
| inline | -- | ?IsDead@CThing@@QBE_NXZ (CThing.h:280, Dead +0x91 bit) ; ?GetPhysicsTC@CThing@@QBEPBVCTCPhysicsBase@@XZ (CThing.h:410) ; ?GetRadius@CTCPhysicsBase@@QBEMXZ (CTCPhysicsBase.h:244) ; ??0C2DVector@@QAE@ABVC3DVector@@@Z ; ??GC2DVector@@QBE?AV0@ABV0@@Z ; ?GetSquaredMagnitude@C2DVector@@QBEMXZ ; ?GetCreature@CAIStateGroupBase@@IBEAAVCThingAICreature@@XZ | draft modelled all locally |

## 4. Members
- this+0x04 CAIStateGroupBase::PBrain (CAIBrain*) -- engine/CAIStateGroupBase.h (FABLE_RETAIL_CAISTATEGROUPBASE).
- PBrain+0x18 CAIBrain Creature (reference via GetCreature) -- FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE + FABLE_RETAIL_CAIBRAIN_GETCREATURE
  (+ FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE as in 0x0091B3A0).
- this+0x18 CAIStateGroup_MinionKidGroupTag::PTCGroupTag (CTCCreatureGroupTag*) -- struct_layouts_egor.tsv; engine/CAIStateGroup_MinionKidGroupTag.h
  is a flattened generated header (no base, no methods, no opt-in macro) -> class must be modelled locally deriving from the retail
  CAIStateGroupBase, as ALL landed siblings (Update/OnActivate) do. Other members: InitiatedTag +0x14, InitialTagPos C3DVector +0x1c,
  StandModeAddedToCreature +0x28, FrameLastPlayedIn +0x2c (sizeof 0x30).
- PTCGroupTag+0x30 CTCCreatureGroupTag::TaggedFrame (ulong) (PDB; TaggerUID ulong64 +0x28, sizeof 0x38). No engine/CTCCreatureGroupTag.h;
  the base CTCCreatureGroup is in engine/CTCCreatureGroup.h under FABLE_RETAIL_CTCBOASTINGCROWDCONTROL (sizeof 36; declares
  GetNearestOtherActiveMember const). Landed SetTagger row models `class CTCCreatureGroupTag : public CTCCreatureGroup {TaggerUID; TaggedFrame;}` locally.
  Draft name `LastTagFrame` -> WRONG, use TaggedFrame.
- target+0x91 bit0 CThing::Dead -> IsDead() (engine/CThing.h, retail branch).
- creature+0x60 CThing::PhysicsTC (CTCPhysicsBase*) -> GetPhysicsTC() (CThing.h:402/410, static_assert 0x60 retail).
- PhysicsTC+0x34 CTCPhysicsBase::Radius (float) -> GetRadius() (engine/CTCPhysicsBase.h:244, FABLE_RETAIL_CTCPHYSICSBASE).
- C2DVector X/Y (engine/C2DVector.h, FABLE_RETAIL_C2DVECTOR + _FROM3D for the C3DVector ctor, _SQUAREDMAGNITUDE_YX for the Y-first sum).
- C3DVector engine/C3DVector.h (FABLE_RETAIL_C3DVECTOR). CSoundPair engine/CSoundPair.h. CCreatureAction_PlayAnimation engine/CCreatureAction_PlayAnimation.h
  (FABLE_RETAIL_CCREATUREACTIONBASE, FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION, _IMPLICIT_DTOR).
- Locally-modelled classes in draft that HAVE shared headers: C3DVector, C2DVector, CCharString (fable_string.h), CSoundPair, CThing,
  CThingCreatureBase, CThingAICreature, CBaseIntelligentPointer/CIntelligentPointer, CCreatureActionBase, CCreatureAction_PlayAnimation,
  CWorld, CAIBrain, CAIStateGroupBase, CTCPhysicsBase (as CPhysicsBody), CTCCreatureGroup (+ CMainGameComponent for fps).

## 5. Callee-body visibility
- Inline in retail (bodies must be visible): CIntelligentPointer<const CThing>(T*) (base ctor + vtable store 0x01237F1C + SetPItem;
  header default inline branch, NOT _CTOR_OUTOFLINE); C2DVector(const C3DVector&), operator-, GetSquaredMagnitude; GetCreature;
  GetPhysicsTC; GetRadius; IsDead; GetConstantFPS; CSoundPair(); GetTCGroupTag (if used). Draft note: the 2nd by-value IP ctor was
  not inlined in its build (needed __forceinline) -- check whether the shared header ctor inlines both; if not, a FORCEINLINE ctor
  opt-in does not exist in CIntelligentPointer.h (grep FORCEINLINE: none) -> would need a new nested opt-in.
- Out-of-line callees: all 14 calls above are other classes/TUs (CTCCreatureGroup, CThingPhysical, CWorld, CCharString, actions,
  CAIStateGroupBase AIAction_*). Same-TU functions defined earlier: InitStateGroup/OnReplace/IsStillValid/OnActivate (not called).
  No STLport helpers called.
- ~CCreatureAction_PlayAnimation is not called: dtor resolves to direct ~CCreatureActionBase 0x00693EF0 call (implicit dtor of a
  local whose static type is known) -> FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR.

## 6. Sibling landed rows (idioms)
- 00/93/CAIStateGroupMinionKidGroupTag_Update_00934870.cpp: same class; local model of CAIStateGroupBase : CBaseClass with
  reference GetCreature(); `#pragma intrinsic(fmod)`; no Flags line; CWorld::GetFrame static __fastcall long.
- 00/93/CAIStateGroupMinionKidGroupTag_OnActivate_00934120.cpp: same class, FABLE_RETAIL_CTHING_GETTC_EXPANDED, local
  CTCCreatureGroupTag model, `PTCGroupTag->SetTagger(PBrain->PCreature)`.
- 00/90/CAIStateGroupMinionGuardFollowPlayer_Update_0090c540.cpp ("Flags: none"): the shared-header recipe for MoveToThingProximity/
  LookAtThing with IP by value: defines FABLE_RETAIL_CCHARSTRING, CCLASS, DEF_CLASS_BASE, CTCBASE, CTHING, CINTELLIGENTPOINTER,
  CTHINGPHYSICAL, CTHINGGAMEOBJECT, CTHINGCREATUREBASE, CTHINGAICREATURE, CAISTATEGROUPBASE, _MEMBERS, _AIACTIONS, _GETCREATURE,
  CAIBRAIN_GETCREATURE, CMAINGAMECOMPONENT(+_WORLD), CWORLD; enums `EAIActionReturn{..=2}`, `EAIActionRepeat{..=1}`, EMovementType;
  call `AIAction_MoveToThingProximity(AI_ACTION_REPEAT_ONCE, hero, 3.0f, reached, MOVEMENT_WALK, true,false,true,true, 0.0f)` with
  a raw `const CThing*` converting to the by-value IP; `long oneSecond = CMainGameComponent::GetConstantFPS();` read first.
- 00/91/CAIStateGroupSeekPerceivedThing_RunToPerceivedThingPosition_0091b3a0.cpp: PlayAnimation + PerformActionAndWait recipe:
  + FABLE_RETAIL_C3DVECTOR, CCREATUREACTIONBASE, CCREATUREACTION_PLAYANIMATION(_IMPLICIT_DTOR), CAISTATEGROUPBASE_PERFORMACTION,
  CAIBRAIN_CREATURE_REFERENCE; includes engine/fable_ai_action_enums.h, CSoundPair.h, CCreatureAction_PlayAnimation.h.
- 00/a1/CNavigatorAStar_CullWayPoints_00a1c010.cpp: user of FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX.
- 00/96/CAIStateGroupMeleeCombatNearby_IsEnemyCreatureInCombatPosition_0096e700.cpp: another physics-radius reach test.

## 7. Old draft model errors
1. Reach compare sense: `lenSq > reach*reach` -> retail is `!(lenSq <= reach*reach)` / positive `lenSq <= reach*reach` block (test ah,1;jne).
2. Target position copied as C3DVector (12 bytes) -> retail copies only X,Y: `C2DVector targetPos(target->PeekPos())` (FROM3D),
   explains the frame slot shift; creature position used through the reference (not copied).
3. Squared length order: retail Y*Y first -> GetSquaredMagnitude _YX opt-in (or `float sq = d.Y*d.Y; sq += d.X*d.X;`).
4. Wrong names: GetPosition->PeekPos (CThingPhysical), GetWorldFrame->CWorld::GetFrame (long, __fastcall static), g_framesPerSecond->
   CMainGameComponent::GetConstantFPS(), LastTagFrame->TaggedFrame, GetNearestOtherActiveMember belongs to CTCCreatureGroup and is
   const with const pointer types; SetTagger takes const CThingAICreature*; AIAction_* take/return enums and bool&; PerformActionAndWait
   takes const CCreatureActionBase&; PlayAnimation ctor takes CThingCreatureBase&.
5. Terminate: one shared call site at 0x009343F3 (LookAt==1 path jumps into the final `if (done) Terminate();` block) -- the draft's
   two separate `Terminate(); return;` may or may not merge; keep in mind if size is off by ~7 bytes.
6. All engine classes modelled locally (see section 4) -> switch to shared headers (verifier strict mode).
7. Draft's private inline IsOutOfReach helper with __forceinline: fine conceptually, but the retail evaluation order is delta first,
   then radius, then squares, then reach^2 and compare.

## 0x00754050

# DOSSIER 0x00754050 CTCLook::UpdateTurningAnims (588 bytes)

## 1. Retail shape (xdis 00754050 0x25c)
- __thiscall, no args, plain `ret` (0x0075429B). No frame pointer (`sub esp,20h; push esi` -> /Oy default, NO optimize("y",off)).
- NO SEH/EH prologue although a CCharString temp and a local vector with dtor exist -> TU built without /EHsc (no unwind funclets).
- Frame exactly 0x20 (draft got 0x24). Slot map (offsets from esp after the 4 pushes esi,edi,ebp,ebx):
  +0x10 GetTC key (0x31) / later the __false_type tag byte (+0x13);
  +0x14 CCharString temp #1/#2/#3 (shared) / CAnimFilterNoGroup temp / `rotation` float (and the final sign*clamped rotation is stored BACK into this slot);
  +0x18 CCharString temp in the fallback / the clamp limit temp (0.5f-DELTA) / the `sign` int (-1/1, `fild`);
  +0x1c..+0x20 pair<float,const CAnimationEntry*> temp; +0x24..+0x2c the local vector<const CAnimationEntry*> (begin/end/eos zeroed with ebp=0).
- Register homes: esi=this; edi=map ptr then mode manager (`mov edi,[eax+4]`), later loop's entry pointer; ebp=0 (zero reg for vector init and the PAppearanceDef null test `cmp eax,ebp`), then PAppearanceDef copy (loaded `mov ebp,[esi+18h]` between GetCurrentMode and the slot-4 vcall), then the loop iterator; ebx=&TurningAnims (`lea ebx,[esi+8Ch]`, set right before clear and kept for the inline push_back).
- Order: GetTC(0x31) expanded (bit 17 of word1 `test [eax+24h],20000h`, LowerBound 0x0040F020 non-const, end read through map ptr `mov edi,[edi+4]`, end-or-greater `cmp [eax],31h; jle`); early-out; `if PAppearanceDef null` return; GetCurrentMode().slot1 (result discarded); TurningAnims.clear() (CALL 0x00755320, out of line); TurningViaAnimMinAngle=0.5f (0x3F000000 immediate store); vector ctor zeroing; GetAnims("TURNING_", anims, GetCurrentMode().GetAnimFilter()); GetAnims("TURNING_", anims, GetCurrentMode().GetDefaultAnimFilter()) (mode+8); if anims.empty(): GetAnims("TURNING_", anims, CAnimFilterNoGroup()) (vptr 0x0122FFDC stored, after call vptr 0x0122FFCC stored = inline ~CAnimFilterNoGroup -> ~CAnimFilterBase, no call); for-loop with end() RE-READ each iteration (`mov eax,[esp+28h]` at the bottom), iterator in ebp, first test before loop (`cmp eax,[esp+28h]; mov ebp,eax; je`); vector dtor inline `if(start) free(start)` (0x00BFEA14).
- The CCharString("TURNING_") is built fresh for each call (ctor 0x0099EBF0 with (str,-1), dtor 0x0099EAE0 after each call) -> three separate `CCharString("TURNING_")` temporaries / implicit conversions, not one named local.
- Loop body: `const CAnimationEntry* anim=*it; float rotation; anim->GetRotationInAnimation(rotation);`
  `if (fabs(rotation) > DELTA_NOTIONAL_ZERO)` (fcomp; test ah,41h; jne skip) {
  TurningAnims.push_back(pair(rotation, anim)) -- pair temp stores (+0x1c/+0x20) interleave with the `_M_finish==_M_end_of_storage` test (ebx+4 vs ebx+8); fast path is placement-new null test `test eax,eax; je` then two dword stores FROM REGISTERS (edx/ecx, edi), `add [ebx+4],8`; slow path `_M_insert_overflow(_M_finish, x, __false_type(), 1, true)` 0x00755350 (push 1; push 1; tag addr; &x; pos) -- note __false_type, NOT __true_type;
  clamp: size=fabs(rotation); limit=0.5f-DELTA_NOTIONAL_ZERO (computed at runtime into a stack temp: fld [0x0122F59C]; fsub [DELTA]; fstp tmp); `if (size > limit) size=limit` (fcom tmp; test ah,41h; jne keep);
  sign: `long sign=-1; ... if (!(rotation < 0.0f)) sign=1` shape: store -1 FIRST (mov [esp+18h],-1 before the fcomp), fcomp 0.0f [0x0122DEDC]; test ah,5; jnp keep -1; else store 1 -> `rotation < 0.0f ? -1 : 1` with -1 pre-stored;
  rotation = sign*size (fild sign; fmul st(1); fstp [rotation slot]) -- stored back to memory and reloaded;
  `if (fabs(rotation) < TurningViaAnimMinAngle) TurningViaAnimMinAngle = fabs(rotation)` (fcom [esi+98h]; test ah,5; jp skip; fstp [esi+98h]). }
- Note pair stores the UNCLAMPED rotation; the signed clamp only feeds the min angle.

## 2. Name / signature (manifest label is RIGHT)
- `?UpdateTurningAnims@CTCLook@@QAEXXZ` -- public, non-virtual, non-const, void(void). Sources: Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 0x00766460; egor_signatures.tsv 0x00766460/0x007666d0) and FableWin.pdb (_fablewin_api.tsv / FableWin.exe.signatures.tsv 0x025AD320, 824 bytes debug). Declared already in rebuild/include/engine/CTCLook.h line 40 (`void UpdateTurningAnims();`).
- Callers (xref.py): 0x0073AC21, 0x0073ADB8 (CTCCreatureModeManager code, mode change), and the tail `jmp` at 0x007542A0 = CTCLook::OnInitialActivate (landed rebuild/src/compiled/00/75/CTCLook_OnInitialActivate_007542a0.cpp, which includes engine/CTCLook.h and calls UpdateTurningAnims()).

## 3. Callees
| addr | landed row | landed/PDB name | draft spelling problem |
|---|---|---|---|
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound (non-const) | draft: local CInterfaceMap/CThing::GetInterface -> use CThing::GetTC under FABLE_RETAIL_CTHING_GETTC_EXPANDED (Components +0x44, UsingComponents bitset +0x20) |
| 0x0073A6E0 | 00/73/CTCCreatureModeManager_GetCurrentMode_0073a6e0.cpp | ?GetCurrentMode@CTCCreatureModeManager@@QBEAAVCModeBase@1@XZ: `CModeBase& GetCurrentMode() const` | draft returned CCreatureMode* (wrong: const member, returns reference to CTCCreatureModeManager::CModeBase) |
| vslot 1 (+4) | - | CModeBase::GetType() const (pure; derived ?GetType@CModeSad@NCreatureMode@@UBEKXZ -> unsigned long, const) | draft `GetModeType` non-const long -- wrong |
| vslot 4 (+0x10) | - | ?GetAnimFilter@CModeBase@CTCCreatureModeManager@@UBEABVCAnimFilterBase@@XZ `virtual const CAnimFilterBase& GetAnimFilter() const` | draft non-const |
| mode+8 (inline) | - | ?GetDefaultAnimFilter@CModeBase@CTCCreatureModeManager@@QBEABVCAnimFilterBase@@XZ (FableWin only -> inline in retail; returns WeaponBasedFilter, CAnimFilterOfGroup at +8) | draft `SecondFilter` member -- wrong; NOT in the shared header yet |
| 0x00755320 | 00/75/stdvectorstdpairfloatconstCAnimationEntry_clear_00755320.cpp | vector<pair<float,const CAnimationEntry*>>::clear (out of line COMDAT) | ok |
| 0x00755350 | 00/75/stdvector_Minsertoverflow_00755350.cpp | vector<..>::_M_insert_overflow(pos,const T&,const __false_type&,size_type,bool) `ret 14h` | landed leaf uses placeholder element `CUnprovenElement_00755350` -> NAME_FIXES: element is std::pair<float,const CAnimationEntry*> (this caller proves it). Draft used __true_type (wrong tag) |
| 0x0099EBF0 | 00/99/CCharString_Constructor_0099ebf0.cpp | CCharString::CCharString(const char*, long=-1) | ok (use fable_string.h, not a local struct) |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | CCharString::~CCharString | ok |
| 0x005DCB00 | 00/5d/CAnimationSet_GetAnimsWithNameStartingWith_005dcb00.cpp | ?GetAnimsWithNameStartingWith@CAnimationSet@@QBEXABVCCharString@@AAV?$vector@PBVCAnimationEntry@@..@std@@ABVCAnimFilterBase@@@Z | ok; header decl under FABLE_RETAIL_CANIMATIONSET_QUERIES (CAnimationSet.h:32-37) |
| 0x005DBFD0 | 00/5d/CAnimationEntry_GetRotationInAnimation_005dbfd0.cpp | ?GetRotationInAnimation@CAnimationEntry@@QBEXAAM@Z `void (float&) const` | draft takes float* -- wrong (same bytes); CAnimationEntry.h has NO declaration -> author must add (or local decl) |
| 0x00BFEA14 | 00/bf/global_free_00bfea14.cpp | free | via STLport allocator in fable_containers.h |
- Globals: 0x0129BA3C = `DELTA_NOTIONAL_ZERO` (?DELTA_NOTIONAL_ZERO@@3MB, const float 1e-4; declared in engine/C2DVector.h:15, CActionInputControl.h:10, 59 landed rows) -- draft's `g_FloatEpsilon` is WRONG. 0x0122F59C = 0.5f literal, 0x0122DEDC = 0.0f literal (compiler constants). 0x0125AB08 "TURNING_" literal. Vtables 0x0122FFDC CAnimFilterNoGroup, 0x0122FFCC CAnimFilterBase.

## 4. Members
- CTCLook (rebuild/include/engine/CTCLook.h, sizeof 208 = retail; PDB is 212 because the PDB vector is 16 bytes; header uses 12-byte STLport vector): PAppearanceDef `CDefPointer<CAppearanceDef const>` +0x18 (null test = IsNull()/Get()); TurningAnims `std::vector<std::pair<float,CAnimationEntry const*> >` +0x8C; TurningViaAnimMinAngle float +0x98. struct_layouts_egor.tsv lines 82672/82698/82699 (PDB offsets 24/140/156). Base CTCBase: ParentThing +0x04 (GetParentThing() inline under FABLE_RETAIL_CTCBASE).
- Sibling opt-in set for CTCLook.h (from CTCLook_OnInitialActivate_007542a0.cpp / CTCLook_HasFrameUpdate_00755310.cpp): FABLE_RETAIL_C3DVECTOR_PACKED, _CLOOKDEF, _DEF_CLASS_BASE, _CTCPHYSICSBASE, _C3DVECTOR, _C3DVECTOR_NOT_EQUAL, _CTCBASE, _CTHING, _CTHINGPHYSICAL, _CTHINGGAMEOBJECT, _CTHINGCREATUREBASE, _LOOK_HEAD_ANIM; add FABLE_RETAIL_CTHING_GETTC_EXPANDED (used by siblings 0x00753BA0, 0x00753840).
- CThing (engine/CThing.h): Components CVectorMap +0x44; UsingComponents bitset +0x20; GetTC<T>(ETCInterfaceType,T**) body at CThing.h:340 under FABLE_RETAIL_CTHING_GETTC_EXPANDED (conditional-expression end-or-greater, key local inside the test, map via pointer local -- exactly retail's `lea edi,[eax+44h]` ... `mov edi,[edi+4]`). Interface 0x31 = TCI_ENTITY_MODE_MANAGER (CTCCreatureModeManager::StatGetInterfaceType()).
- CTCCreatureModeManager::CModeBase (engine/CTCCreatureModeManager.h): vptr; ParentThing CThing& +4; WeaponBasedFilter +8 (header: `unsigned char[12]`, protected; Ego_r: CAnimFilterOfGroup, struct_layouts_egor.tsv 23294); FramesBeforeRemovingSelf +0x14; Active +0x18. Opt-ins: FABLE_RETAIL_CMODEBASE_VIRTUALS (slot 4 GetAnimFilter decl), FABLE_RETAIL_CMODEBASE_PARENTTHING, _NOFRAMES, FABLE_RETAIL_CTCCREATUREMODEMANAGER(_VSLOTS/_FRAMEUPDATE/_ADDMODEPOINTER). MISSING: GetDefaultAnimFilter() inline (needs a new opt-in returning `*(const CAnimFilterBase*)WeaponBasedFilter` / a typed CAnimFilterOfGroup member).
- CAppearanceDef (engine/CAppearanceDef.h): member is `Animation` (CAnimationSet) at +0x34 -- draft's `AnimationSet` is WRONG. Needs FABLE_RETAIL_DEF_CLASS_BASE.
- CAnimFilterNoGroup / CAnimFilterBase: engine/CAnimFilterNoGroup.h under FABLE_RETAIL_CANIMFILTER (inline ctor, inline virtual dtor -> matches retail's two vptr stores); CAnimFilterBase.h.
- Classes the draft models LOCALLY that have shared headers: CCharString (fable_string.h), CAnimationEntry, CAnimFilterBase, CAnimFilterNoGroup, CAnimationSet, CAppearanceDef, CTCCreatureModeManager (+CModeBase), CThing (+CVectorMap interface map), CTCLook, both std::vectors (stlport/fable_containers.h), std::pair.

## 5. Callee-body visibility
- Retail calls vector<pair>::clear OUT OF LINE (0x00755320, COMDAT after the CTCLook TU at 0x00753020..0x007542A0) -> `#define FABLE_RETAIL_STLP_VECTOR_CLEAR_OUTOFLINE` (fable_containers.h:531, declaration-only clear()).
- push_back is EXPANDED inline (default STLport body fable_containers.h:478, `_M_insert_overflow(_M_finish,x,_IsPODType(),1,true)`); _M_insert_overflow called out of line (COMDAT 0x00755350) -> FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY (fable_containers.h:609) to keep its body out of the TU (else it might inline / be instantiated here). pair<float,ptr> is non-POD to STLport -> __false_type overload.
- LowerBound 0x0040F020 out of line (CVectorMap.h, as GetTC_EXPANDED expects). GetCurrentMode 0x0073A6E0 is called (3x), not inlined -> plain declaration from the header (it is not visible in this TU). GetTC/PeekTC are __forceinline in CThing.h.
- CAnimFilterNoGroup ctor/dtor and CAnimFilterBase dtor inline (visible). vector<const CAnimationEntry*> ctor/dtor/empty inline.

## 6. Sibling landed rows (same TU, 0x00753020..0x007542A0)
- CTCLook_OnInitialActivate_007542a0.cpp: shared CTCLook.h with the 12 opt-ins listed above (copy them).
- CTCLook_ImplementHeadSettings_00753ba0.cpp and CTCLook_TrackNearestInterestingThing_00753840.cpp: `#define FABLE_RETAIL_CTHING_GETTC_EXPANDED` + engine/CThing.h + CVectorMap.h (local CTCLook model though).
- CTCLook_FrameUpdate_00753c70.cpp: fable_compiler.h, C3DVector.h, CVectorMap.h, `#pragma auto_inline(off/on)` around helpers.
- CTCLook_IsCurrentlyTurningViaAnimation_00753b40.cpp, CTCLook_SetBodyMovingViaAnimation_00753930.cpp: no Flags lines, no optimize pragmas -> default /O2 TU, no /EHsc evidence.
- None carries a `// Flags:` line; no optimize("s"/"y") in the TU.

## 7. Old draft model errors (vs disassembly)
1. Wrong names: g_FloatEpsilon (-> DELTA_NOTIONAL_ZERO), AnimationSet (-> Animation), GetModeType (-> GetType const, unsigned long), SecondFilter (-> GetDefaultAnimFilter()), GetRotationInAnimation(float*) (-> float&), CCreatureMode (-> CTCCreatureModeManager::CModeBase&), GetCurrentMode returns pointer (-> reference, const fn), _M_insert_overflow tag __true_type (-> __false_type).
2. Local struct models for every class that has a shared header (the verifier warns / strict rejects).
3. Frame 0x24 vs 0x20 and ebx/ebp swap: draft's local `push_back` + CTurningAnim ctor and the local GetInterface (if-form, `long type` copy) differ from the shared STLport push_back (_Construct with placement-new null test) and GetTC_EXPANDED (conditional-expression form). Retail keeps &TurningAnims in ebx from the clear() call (so `TurningAnims.clear()` then later `TurningAnims.push_back` with this->TurningAnims CSE'd into ebx).
4. Clamp/sign: retail stores sign*size back to the rotation slot and reloads it; the 0.5f-DELTA limit lives in a stack temp; sign int pre-stored -1 before the compare. The draft's `float maxRotation=..; float size=..; long sign=-1; if(!(rotation<0)) sign=1; rotation=sign*size;` is close; try `rotation < 0.0f ? -1 : 1` and in-place compare forms only after fixing the header models.
5. Draft's GetInterface returned the mode manager pointer and never tested it -- matches retail (no null test of the component); the bool return gates the early out.
6. Loop must re-read anims.end() each iteration (address of `anims` escapes into GetAnims; draft's `it != anims._M_finish` is fine) and test before entry.

## 0x00951D60

# DOSSIER 0x00951D60 CAIStateGroup_JackDragon::IntoHover (588 B)

## 1. Retail shape (xdis.py 00951d60 0x25c)
- thiscall, `ret` (no args), no frame pointer (`sub esp,30h; push ebx/ebp/esi/edi`), no SEH (no /EHsc frame), /O2-style (no `optimize("s")` idioms).
- `xor ebx,ebx` at +0x16: ebx is the zero register for the whole body (def init, `cmp [eax+60h],ebx`, push ebx as the 0.0f arg, vector zeroing, null tests).
- Frame (after push edi): +0x10 = def CDefPointer home (zeroed, &passed to GetSubDef) -- the SAME slot is later reused for the sum temp (+0x10..0x18) and the CCountedPointer action (+0x10/+0x14); vector route +0x1c/+0x20/+0x24; offset vec +0x28/+0x2c/+0x30; second vec +0x34/+0x38/+0x3c.
- def.Object is loaded into ebp only at first use (0x951e02, after the marker GetPItem), not right after GetSubDef.
- Constants: [0x012944B8] = -10.07981f; offset = (0, -10.07981f - def->HoverBackOffOffset, 30.148378f (0x41F12FE1)); second = (0, -80.0f, 30.0f) -- rotated then never used (dead but its RotateXY call stays).
- Exit paths: guard-fail -> 0x951f88 (reload ebp from slot) -> `CurrentState=3` + checked release; runnable -> ~action, free(route) -> same join; Terminate path: Terminate(), ~action, free(route), release WITHOUT null test (`dec [ebp+4]` directly) -- VC7.1 drops the dtor's null test on a path that already dereferenced def (documented in landed 0x00950A30 header).

## 2. Name (3 sources agree; manifest label is RIGHT)
- Ego_r.pdb `?IntoHover@CAIStateGroup_JackDragon@@AAEXXZ` (egor_signatures.tsv 009766f0, 605 B; ego_r.exe.signatures.tsv); FableWin `_fablewin_api.tsv` 02bdfed0 same mangling; landed caller Update 0x00952E80 `case 2: IntoHover();` (xref: only caller 0x00952FA4).
- Leaf: `?IntoHover@CAIStateGroup_JackDragon@@AAEXXZ` -- PRIVATE, `void IntoHover()`, non-const, no args.
- Ego_r neighbour order StartFlying..FlyOutOfGround, IntoHover, UpdateHover matches retail (0x950C30 .. 0x951B20, 0x951D60): same TU (CAIStateGroup_JackDragon.cpp).

## 3. Callees (landed file / landed name / PDB name; X = draft disagrees)
- 0x008E3300 rebuild/src/compiled/00/8e/CDefClassBase_GetSubDefCJackDragonDef_008e3300.cpp: `CDefClassBase::GetSubDef<CJackDragonDef>(CDefPointer<const CJackDragonDef>*) const`, PDB `??$GetSubDef@VCJackDragonDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCJackDragonDef@@@@@Z`, ret 4. X draft: non-const, `CJackDragonDef**` out (should be CDefPointer<const CJackDragonDef>*). Called on creature+0x70 (PBaseDef; siblings: `GetCreature()->PeekBaseDef().GetSubDef(&def)`).
- 0x00A01B50 CBaseIntelligentPointer::GetPItem `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` (reach via CIntelligentPointer<CThing>::Get()/operator->).
- 0x00A14040 C3DVector_RotateXY_00a14040.cpp `?RotateXY@C3DVector@@QAEXM@Z`. X draft "RotateAboutZ".
- 0x004C73D0 CThing_GetPosition_004c73d0.cpp leaf `?GetPos@CThing@@QBEABVC3DVector@@XZ`. X draft "GetPosition". (egor_pdb_names hit at that address is another binary; ignore.)
- 0x0066E630 stdvector_pushback_0066e630.cpp `std::vector<C3DVector>::push_back(const C3DVector&)`.
- 0x00BFEA1A operator new (0x114 bytes) ; 0x00BFEA14 free (inline vector dtor).
- 0x009534D0 `??0CCreatureAction_DragonHoverInto@@QAE@AAVCThingCreatureBase@@@Z` -- takes CThingCreatureBase& . X draft: class name "CCreatureAction_DragonIntoHover" and pointer param. Correct: `new CCreatureAction_DragonHoverInto(*GetCreature())`.
- 0x00695880 `??0?$CCountedPointer@VCCreatureActionBase@@@@QAE@PAVCCreatureActionBase@@@Z` (called out of line).
- 0x0048C5C0 `??1?$CCountedPointer@VCCreatureActionBase@@@@QAE@XZ` (file name is a propagated label, leaf correct; called out of line).
- 0x00950C30 StartFlying. PDB (Ego_r): `?StartFlying@CAIStateGroup_JackDragon@@AAEXAAVCThingCreatureBase@@AAV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@AAVC3DVector@@V?$CCountedPointer@VCCreatureActionBase@@@@@Z` = private, (CThingCreatureBase&, vector<C3DVector>&, C3DVector&, CCountedPointer<CCreatureActionBase> by value). X both the landed row (leaf `QAE...PAVCThingCreatureBase...ABUCC3DVectorVector...` stand-in) and the draft (pointer + const refs) disagree with PDB -> NAME_FIXES candidate. By-value copy is built inline (`sub esp,8`, copy Data/Info, `inc [Info]` if non-null).
- 0x0094FDF0 IsStillRunnable (Ego_r `?IsStillRunnable@CAIStateGroup_JackDragon@@AAE_NXZ`, private; landed leaf says QAE).
- 0x008FCF40 `?Terminate@CAIStateGroupBase@@QAEXXZ` (5 B, called out of line).
- vcall [phys]+0xFC slot 63 = CTCPhysicsBase::GetFacingAngleXY() const (landed OnActivate 0x009500E0). X draft "GetHeading".
- vcall [creature phys]+0x10C slot 67 = SetFacingAngleYZ(float) (landed identity proof row "slot 67 ... SetFacingAngleYZ(float)" + rows calling `GetPhysicsTC()->SetFacingAngleYZ(0.0f)`). X draft "SetGravity(false)" -- WRONG; EnableGravity is slot 38 (+0x98). Arg pushed as `push ebx` = 0.0f.
- def release: def vtable slot 1 (+4) = Delete, count at def+4.

## 4. Members
- this: PBrain +0x04 (CAIStateGroupBase.h, FABLE_RETAIL_CAISTATEGROUPBASE[_MEMBERS], GetCreature -> FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE); CAIBrain::PCreature +0x18.
- CAIStateGroup_JackDragon (struct_layouts_egor, packed 0x140): PHoverMarker1 +0x24 / PHoverMarker2 +0x2c CIntelligentPointer<CThing>; CurrentState +0x5c EDragonState (store 3); GoLeft +0x60 long; PreviousFlightPos +0x65 C3DVector. Shared header engine/CAIStateGroup_JackDragon.h is a byte-blob layout (PHoverMarker as unsigned char[8], PreviousFlightPos_X/Y/Z) -- siblings all model the class locally (pack(1)); do the same, using shared CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER) as FlyOutOfGround does.
- CThing: PhysicsTC +0x60 (CThing.h GetPhysicsTC(), static-asserted), PBaseDef +0x70 (PeekBaseDef), dead bit +0x91 bit0 (IsDead()). CThing.h + FABLE_RETAIL_CTHING* (FlyOutOfGround includes engine/CThing.h).
- CJackDragonDef +0x54 = **HoverBackOffOffset** (float) (X draft "HoverHeight"); +0x04 refcount.
- Locally-modelled in draft but shared header exists: C3DVector (engine/C3DVector.h, FABLE_RETAIL_C3DVECTOR, inline operator+ line 144/146, RotateXY decl), CThing, CBaseIntelligentPointer/CIntelligentPointer, CDefClassBase, CDefPointer (engine/CDefPointer.h needs FABLE_RETAIL_DEF_CLASS_BASE), CCountedPointer (engine/CCountedPointer.h), CAIStateGroupBase, CAIBrain, CTCPhysicsBase, CThingCreatureBase, CJackDragonDef.

## 5. Visibility
- GetSubDef<CJackDragonDef> 0x8E3300 is a template COMDAT of this TU: landed OnActivate (0x009500E0) and ChooseIntoGroundState (0x00950A30) / ChooseHoverOrStrafeState compile its body in-TU as `__declspec(noinline)` / `#pragma auto_inline(off)` -- "with it VC7.1 knows the def pointer's frame word is not captured by the call". This is exactly hand278's residue ("def gets a second frame home", "next: GetSubDef noinline stand-in (fixed the def/ebx shape in 0095bcf0)"). Copy OnActivate lines 35-47 model.
- CCountedPointer ctor/dtor called out of line but frame slot +0x10 is shared by def, sum temp and action -> use FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR + FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_DTOR (pair; precedent 0x008A8C60 / 0x0041E6D3), not the decl-only CALLED_CTOR.
- operator+ must be INLINE (draft's grade says it went out of line): use C3DVector.h FABLE_VECTOR_INLINE operator+ (FABLE_RETAIL_C3DVECTOR). Order: `offset + marker->GetPos()` (fld offset.X; fadd pos.X).
- vector<C3DVector> ctor/dtor inline (zero 3 words; `if(start) free(start)`), push_back out of line.
- Terminate/IsStillRunnable/StartFlying are earlier in the same TU but called (not inlined).

## 6. Siblings (same class/TU, landed)
- 0x00951B20 FlyOutOfGround (immediate predecessor): pack(1) local class, engine/CThing.h, `#define FABLE_RETAIL_CINTELLIGENTPOINTER` + engine/CIntelligentPointer.h, IS_THING_ALIVE(const CIntelligentPointer<CThing>&) via Get()/IsDead(), local CVector3DList, GetPhysicsTC()->GetHeading slot 63.
- 0x009500E0 OnActivate: FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_C3DVECTOR, FABLE_RETAIL_CTHING_GETTC_EXPANDED, engine/CSubDefClassBase.h + CDefPointer.h + C3DVector.h; noinline GetSubDef<CJackDragonDef> specialization; `CDefPointer<const CJackDragonDef> def; GetCreature()->PeekBaseDef().GetSubDef(&def);`; `x.RotateXY(m->GetPhysicsTC()->GetFacingAngleXY())`; `C3DVector + PLandedMarker->GetPos()`.
- 0x00950A30 ChooseIntoGroundState: auto_inline(off) GetSubDef body; local CDefPointer whose dtor null-test VC drops after deref.
- 0x00950C30 StartFlying, 0x00952E80 Update (caller), 0x0094FDF0 IsStillRunnable (FABLE_RETAIL_CINTELLIGENTPOINTER). No `// Flags:` lines in these rows (default flags).

## 7. Old draft model errors
- def as raw `CJackDragonDef*` via inline GetDragonDef() + ReleaseDef helper instead of `CDefPointer<const CJackDragonDef>` local with same-TU noinline GetSubDef -> the "second frame home"/ebx residue.
- Wrong vslot meaning: SetGravity(false) -> SetFacingAngleYZ(0.0f); GetHeading -> GetFacingAngleXY.
- Wrong names: RotateAboutZ->RotateXY, GetPosition->GetPos, HoverHeight->HoverBackOffOffset, CCreatureAction_DragonIntoHover->CCreatureAction_DragonHoverInto (ctor takes CThingCreatureBase&), StartFlying param types (refs, non-const, private).
- Local operator+ went out of line -> use shared inline operator+.
- Local stand-in structs everywhere (C3DVector, CThing, CBaseIntelligentPointer, CCountedPointer, CDefClassBase) where shared headers exist.
- Control flow otherwise right: 4-way && guard, `(GoLeft ? PHoverMarker1 : PHoverMarker2)` selected BEFORE one GetPItem call (ecx select then single call), Terminate+return inside scope, else fall to CurrentState=3.

## 0x009CCEF0

# DOSSIER 0x009CCEF0 CBankFile::FindIndicesByFilename (592 bytes)

## 1. Retail shape (xdis 009ccef0 0x260)
- thiscall, `ret 0x10` = hidden result ptr + path + type + bool. `sub esp,0x20; push ebx,ebp,esi,edi` -- NO frame pointer (/Oy on),
  NO SEH prologue (no EH state despite CCharString temps -> TU built without /EHsc unwinding here, like the sibling rows), normal /O2
  (16-aligned entry, `nop` loop alignment at 0x9ccf5f / 0x9cd09f => not optimize("s")).
- Frame (esp after 4 pushes): [0x10] cond-dtor flag word for the "UNASSIGNED" temp (bit0); [0x14] lowered name; [0x18] "UNASSIGNED"
  temp; [0x1c] homed node index (push_back's const& arg); [0x20] end node; [0x24..0x2c] local result vector start/finish/eos;
  [0x34] hidden result; [0x38] path slot REUSED for the ConvertPathToFilename temp and later the entry-symbol temp; [0x3c] type
  (re-read from its slot every iteration, never cached); [0x40] bool slot also passed as the __true_type tag address to _M_insert_overflow.
- Registers: ebp=this all function; esi=node; edi=end then index; ebx=0 (start) then skip flag (bl) then reloaded `mov ebx,[esp+0x24]` (_M_start) after push_back.
- Order: ConvertPathToFilename(path) -> GetAsLowerCase() into name -> dtor temp -> _M_upper_bound(name) FIRST (end) then _M_lower_bound(name)
  (= STLport `equal_range` = pair(lower_bound(k), upper_bound(k)) with VC7.1 right-to-left argument evaluation).
- Loop: guarded `cmp esi,edi; je` then bottom-tested do-loop; increment is the inlined STLport _Rb_tree_base_iterator::_M_increment
  (right child -> leftmost via +8; else climb parents via +4 / +0xc, final `cmp [eax+0xc],ecx`).
- Body: `if (type == -1 || GetEntryDataType(idx) == type)`; GetEntryDataType inline: `PackedDataType.Size ([ebp+0x5c]) ? PackedDataType[idx-1]
  (call 0xA629F0, ecx=ebp+0x58) : RuntimeData.start[idx].DataType (byte at +8, stride 12)`.
  Symbol = inline GetEntrySymbol(idx) (Symbols empty -> CCharString("(Unknown)",-1) 0x129A03C, else copy of Symbols[idx]) into [esp+0x38].
  skip(bl) = excludeArg && symbol.FindCharPos('[') == -1 && strncmp(symbol.text, tmp.text, tmp.len) != 0 where tmp = CCharString("UNASSIGNED",-1)
  (0x129B538) is a CONDITIONAL temporary (flag |=1, dtor only if built) => an implicit conversion inside a short-circuit &&:
  `!symbol.StartsWith("UNASSIGNED")` with StartsWith(const CCharString&) inline (NullChar 0x129AAF4 selects for both strings).
  `if (!skip) indices.push_back(index)` -- push_back fully inline (finish!=eos: `if(finish) *finish=x; ++finish` else _M_insert_overflow(finish,x,tag,1,true)).
  Symbol dtor then next.
- Return: `return indices;` -- STLport vector copy ctor INLINE into *result (malloc n*4 / memmove / finish), then ~name, then free(local start). No NRVO.

## 2. Name / signature (two sources agree; manifest label CORRECT)
- Ego_r ghidra_out/egor_pdb_names.tsv 00b1aca0 and FableWin _fablewin_api.tsv / naming_stragglers 0182e98e, 02f97f20:
  `?FindIndicesByFilename@CBankFile@@QBE?AV?$CArray@K@@ABVCCharString@@J_N@Z`
  = public const: `CArray<unsigned long> CBankFile::FindIndicesByFilename(const CCharString&, long, bool) const`.
- Caller: only 0x009CD190 inside CBankFile::FindIndexByPathname 0x009CD140 (xref.py; landed rebuild/src/compiled/00/9c/CBankFile_FindIndexByPathname_009cd140.cpp,
  calls `FindIndicesByFilename(filename, -1, false)`; it names params "maxCount, exact" -- made up; draft names "type, excludeUnassigned" fit the bytes better).
  Param names are not in any PDB dump grep'd (descriptive).

## 3. Callees (landed file under rebuild/src/compiled/aa/bb/)
| addr | landed file | PDB name | note |
|---|---|---|---|
| 0x009CBE10 | 00/9c/CBankFile_ConvertPathToFilename_009cbe10.cpp | ?ConvertPathToFilename@CBankFile@@ABE?AVCCharString@@ABV2@@Z (PRIVATE const) | same TU, defined earlier, called out of line |
| 0x0099ED10 | 00/99/CCharString_ToLower_0099ed10.cpp | ?GetAsLowerCase@CCharString@@QBE?AV1@XZ (Ego_r 00afa690, FableWin 02f61580) | **draft/landed spell "ToLower" -- WRONG, PDB = GetAsLowerCase** (NAME_FIXES candidate) |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | |
| 0x009D2B50 | 00/9d/stdRbtree_Mupperbound_009d2b50.cpp | _Rb_tree<CCharString,pair<const CCharString,ulong>,...>::_M_upper_bound(const CCharString&) const (QBE) | draft "UpperBound" on a local CFilenameMap |
| 0x0042AD34 | 00/42/stdRbtree_Mlowerbound_0042ad34.cpp | same tree ::_M_lower_bound (ABE), size-TU COMDAT | draft "LowerBound" |
| 0x00A629F0 | 00/a6/CPackedUIntArray_operator_00a629f0.cpp | ??ACPackedUIntArray@@QBEKK@Z | |
| 0x0099EBF0 | 00/99/CCharString_Constructor_0099ebf0.cpp | CCharString(const char*, long = -1) | |
| 0x0099EC30 | 00/99/CCharString_CCharStringcopy_0099ec30.cpp | CCharString(const CCharString&) | |
| 0x0099E5A0 | 00/99/CCharString_FindCharPos_0099e5a0.cpp | ?FindCharPos@CCharString@@QBEJD@Z (Ego_r 00af9e60) | |
| 0x00BFEAF8 | 00/bf/global_strncmp_00bfeaf8.cpp | strncmp import thunk (toolchain_library) | |
| 0x00464ACE | 00/46/stdvectorunsignedlong_Minsertoverflow_00464ace.cpp | vector<unsigned long>::_M_insert_overflow(ulong*,const ulong&,const __true_type&,uint,bool) | size/Oy- COMDAT |
| 0x00BFEA0E / 0x00BFEAE6 / 0x00BFEA14 | 00/bf/global_malloc / global_memmove / global_free | CRT | inline vector copy ctor + dtor |
Inlined (no call): GetEntrySymbol 0x009CC1D0 (?GetEntrySymbol@CBankFile@@QBE?AVCCharString@@K@Z), GetEntryDataType 0x009CC210 (?GetEntryDataType@CBankFile@@QBEKK@Z),
StartsWith (FableWin 0187b1f0 ?StartsWith@CCharString@@QBE_NABV1@@Z), push_back, vector copy ctor, iterator ++, equal_range.

## 4. Members (ghidra_out/struct_layouts_egor.tsv names; retail offsets per rebuild/include/engine/CBankFile.h)
- +0x08 Symbols CArray<CCharString> (start +8, finish +0xc) -- header has it OPAQUE `unsigned char Symbols[0xc]`.
- +0x20 RuntimeData CArray<CBankFile::CRuntimeData> (12-byte elems, DataType byte at +8) -- opaque in header.
- +0x58 PackedDataType CPackedUIntArray; tested field +0x5c is **Size** (rebuild/include/engine/CPackedUIntArray.h: PackedInts +0, Size +4,
  Bits +8, Bias +0xc; GetSize() inline). Draft calls +4 `m_pData`/IsPacked -- wrong name (PDB: Size).
- +0xc4 FilenameIndices multimap<CCharString,unsigned long> -- opaque `unsigned char FilenameIndices[0xc]` in header.
- Node: _Rb_tree_node_base color/parent/left/right at 0/4/8/0xc, value pair at +0x10 (key CCharString), mapped ulong at +0x14.
- CBankFile.h offers member opt-ins only FABLE_RETAIL_CBANKFILE_ENTRYQUERY (EntryExists/GetEntryDataType declared OUT of line -- not what
  this row needs: retail inlines GetEntryDataType here), _ENTRYDATASIZE, _READENTRYDATA, FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS (virtuals,
  base). No typed Symbols/RuntimeData/FilenameIndices, no GetEntrySymbol / FindIndicesByFilename decl -> the sibling rows all use a local
  CBankFile_Retail model (GetEntrySymbol_009cc1d0, GetEntryDataType_009cc210, FindIndexByFilename_009ccdf0).
- Classes the draft models locally that have shared headers: CCharString (rebuild/include/fable_string.h), CArray / vector
  (engine/CArray.h + stlport/fable_containers.h), CPackedUIntArray (engine/CPackedUIntArray.h), the multimap/_Rb_tree (stlport/fable_map.h),
  CBankFile (engine/CBankFile.h, opaque members), __true_type (fable_containers.h, already has the empty user ctor).

## 5. Callee visibility / opt-in macros
- fable_string.h: FABLE_RETAIL_CCHARSTRING (base), FABLE_RETAIL_CCHARSTRING_STARTSWITH (FABLE_FORCEINLINE StartsWith via strncmp, prefix
  data/len from one null test -- exactly 0x9ccfe9..0x9cd01e), FABLE_RETAIL_CCHARSTRING_SUBSTRING (declares FindCharPos 0x0099E5A0).
  GetAsLowerCase is NOT declared in fable_string.h (only mentioned in the GETASUPPERCASE comment, line ~447) -> needs a declaration (or a
  method-only view like FindIndexByFilename_009ccdf0's CCharStringLowerView -- but spell it GetAsLowerCase).
- fable_map.h: FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE (tree _M_lower_bound/_M_upper_bound declared out of line + inline lower_bound /
  upper_bound / equal_range = pair(lower_bound(k), upper_bound(k)) -- gives the upper-first call order). Used by landed
  00/4b/CQuestManager_ActivateQuests_004b3ce0.cpp. std::multimap (line 542) has NO equal_range / lower_bound / upper_bound / begin/end
  accessors yet (only insert/clear/erase/CONSTFIND find/end) -- a multimap equal_range const forwarder under that macro would be needed.
- fable_containers.h vector: push_back inline by default (matches); copy ctor inline by default (matches: malloc/memmove inline);
  do NOT define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE / _PUSHBACK_OUTOFLINE / _PUSHBACK_NOINLINE. _M_insert_overflow(__true_type) is
  declared (line 572) -> out-of-line COMDAT 0x464ACE as retail. CArray.h: FABLE_RETAIL_CARRAY*, PUSHBACK opt-ins exist but retail here is
  plain inline STLport push_back.
- Same TU earlier definitions: GetEntrySymbol 0x9CC1D0, GetEntryDataType 0x9CC210 (both inlined here -> give them inline bodies in the
  class), ConvertPathToFilename 0x9CBE10 (called; keep declaration-only or visible-noinline), FindIndexByFilename 0x9CCDF0 (immediately
  before; same equal_range walk). FindIndexByPathname 0x9CD140 (the caller) is immediately after.

## 6. Siblings
- 00/9c/CBankFile_FindIndexByFilename_009ccdf0.cpp (242 B, landed): same ConvertPathToFilename->lower->UpperBound/LowerBound walk with
  local CFilenameNode/CFilenameMap, CCharStringLowerView, GetDataType inline (`PackedDataType.Size ? [i-1] : RuntimeData.begin()[i].DataType`),
  includes fable_string.h + engine/CArray.h, no pragmas.
- 00/9c/CBankFile_GetEntrySymbol_009cc1d0.cpp: `if (Symbols.IsEmpty()) return CCharString("(Unknown)", -1); return CCharString(*(Symbols.begin() + index));`
  (iterator-object begin()+index so first is reloaded -- retail here also reloads [ebp+8] at 0x9ccfb2).
- 00/9c/CBankFile_GetEntryDataType_009cc210.cpp: `if (PackedDataType.IsPacked()) return PackedDataType[index-1]; return RuntimeData[index].DataType;`
- 00/9c/CBankFile_FindIndexByPathname_009cd140.cpp: the caller; uses a scalar stand-in for this function.
- 00/9c/CBankFile_WriteSymbolHeaderFile_009cf3a0.cpp: shared-header idiom (#define FABLE_RETAIL_CCHARSTRING, _SUBSTRING, FABLE_RETAIL_CARRAY,
  includes stlport/fable_containers.h, fable_map.h, fable_string.h, engine/CArray.h, engine/CPackedUIntArray.h).
- No CBankFile row in 00/9c carries a // Flags: line except AllocateBuffers (/O2 /Oy /W3) and WriteRecoveryFile (/GS); this row needs none.

## 7. Draft model errors (vs disasm)
- Uses separate UpperBound/LowerBound on a local map; retail is consistent with multimap equal_range (pair temporaries, end homed to
  [esp+0x20]) -- try `pair<const_iterator,const_iterator> r = FilenameIndices.equal_range(name); for (it = r.first; it != r.second; ++it)`
  with real STLport iterators (++ = _M_increment inline).
- Caches `type` in ebp / `this` in edi (pending NOTES: 620v592, extra frame word); retail re-reads type from its slot every iteration and
  keeps `this` in ebp -- likely caused by the hand-written NextFilenameNode / local structs changing liveness; real iterator objects may fix.
- Names: ToLower -> GetAsLowerCase; CPackedUIntArray +4 m_pData -> Size (GetSize()); local CULongVector -> CArray<unsigned long>
  (engine/CArray.h); CompareStart -> StartsWith (FABLE_RETAIL_CCHARSTRING_STARTSWITH) with polarity `!symbol.StartsWith("UNASSIGNED")`.
- Draft's local CCharString::c_str/Length should be fable_string.h GetAsCharArray / GetLength (NullChar 0x129AAF4, `PStringData==0 ? 0 : ...`).
- Local struct models of CCharString / CArray / CPackedUIntArray / tree violate the shared-header rule.

## 0x00492973 (not landed)

# DOSSIER 0x00492973 -- CSubtitleRenderer::SetText (full 4-caption wide overload), 568 bytes

## 1. Retail shape (xdis 00492973 0x248)
- thiscall, `ret 28h` = 10 dword args (font, text, name, b1, b2, b3, flags, time, bool, bool).
- `push ebp; mov ebp,esp; sub esp,1Ch; push esi` ... `leave; ret 28h` => `#pragma optimize("y", off)` + `#pragma optimize("s", on)`
  (same as every landed CSubtitleRenderer row: 0x00492E4B, 0x00492BAB, 0x0048F6BD, 0x0048F939, 0x0048DDF6). No SEH, no /GS cookie
  (Clear 0x00492BAB and Pause 0x0048DD94 carry `// Flags: /GS`, this body has no cookie read -> no /GS flag needed).
- Frame 0x1C = the CSetTextInfo local only ([ebp-1Ch], sizeof 0x1C: 4 strings + flags + time + 2 bools). Everything else is homed
  in DEAD ARGUMENT SLOTS: CCharString temps at [ebp+0Ch]; hero pointer local at [ebp+0Ch] (passed by ref to IS_THING_ALIVE);
  GetTC key 0xCB at [ebp+0Ch]; found list at [ebp+0Ch] with its default allocator temp at [ebp+0Fh]; filter (0x11) at [ebp+14h].
- Early branch: `if (DisplayingText) { CSetTextInfo info(text,b1,b2,b3,flags,time,bool1,bool2); TextInfoQueue.push(info); }`
  then `jmp` to the common epilogue (pop esi only; ebx/edi pushed only on the main path) -> source is if/else or early return.
- ebx = &Text (+0x5C) is cached and reused for the Preload vcall; edi = 0x0122D70E ("" char literal) reused for both CCharString temps.
- Tail loop is UNROTATED: `mov edi,[ebp+0Ch] (sentinel); mov esi,[edi]; jmp test; body: push [esi+8]; mov ecx,ebx; call RemoveMessage;
  mov esi,[esi]; test: cmp esi,edi; jne body`. Manager in ebx (loaded BEFORE the list ctor), sentinel in edi.

## 2. Identity (label OK modulo doubled class)
- Mangled leaf: `?SetText@CSubtitleRenderer@@QAEXABVCFontBank@@ABVCWideString@@1111KM_N2@Z`
  Sources: ghidra_out/egor_pdb_names.tsv (Ego_r 0x0049BBF8) + egor_signatures.tsv; the landed caller 0x00492E4B forwards its 10 args
  here (`call 0x00492973`), and landed Clear 0x00492BAB calls it with (`*PFont, info.Text, L"", info.Button1Text, ...`).
  Manifest label `CSubtitleRenderer::CSubtitleRenderer::SetText` has a doubled class prefix; real name CSubtitleRenderer::SetText.
- Signature: `void SetText(const CFontBank&, const CWideString&, const CWideString&, const CWideString&, const CWideString&,
  const CWideString&, unsigned long, float, bool, bool)` -- public, non-const, returns void.
- Callers (xref.py): 0x00492E27(.. ->0x00492E4B landed), 0x00492EA6/0x00492ED1, 0x00492F36/0x00492F69, 0x00492FA2/0x00492FB4,
  0x00493017/0x00493049, 0x004930A5/0x004930D7 (Update), 0x005EAE56, 0x006DD853, 0x00713392, 0x00892AF2.
- Parameter -> member mapping from bytes (IMPORTANT, not the obvious one):
  [ebp+10h] (arg3) -> NameText +0x60 ; [ebp+14h] (arg4) -> Button2Text +0x68 ; [ebp+18h] (arg5) -> Button1Text +0x64 ;
  [ebp+1Ch] (arg6) -> Button3Text +0x6C. Arg3 ("name") is NOT passed to CSetTextInfo (ctor gets text, arg4, arg5, arg6).
  Clear passes L"" as arg3 and info.Button1Text/2/3 as arg4/5/6. Header opt-in SETTEXT names params button0_text/button1_text/...
  (no PDB param names found). Draft already maps arg4->Button2Text, arg5->Button1Text: keep it.

## 3. Callees (landed file / landed name / PDB name; FLAG = draft spelling wrong)
- 0x0048DDF6 CSetTextInfo ctor: 00/48/CSubtitleRendererCSetTextInfo_CSetTextInfo_0048ddf6.cpp; PDB
  `??0CSetTextInfo@CSubtitleRenderer@@QAE@ABVCWideString@@000KM_N1@Z`. FLAG: draft models CSetTextInfo as a top-level struct; it is
  the NESTED class CSubtitleRenderer::CSetTextInfo (Clear 0x00492BAB models it nested).
- 0x004941C0: 00/49/stddequeCSubtitleRendererCSetTextInfo_pushback_004941c0.cpp = std::deque<CSetTextInfo>::push_back (out of line);
  source spelling is the inline `queue::push(x)` -> `c.push_back(x)`. FLAG: draft's `CSetTextInfoQueue::push` stand-in; use
  std::queue<CSetTextInfo, std::deque<...> > as Clear does (TextInfoQueue opaque [0x28] in header -> reinterpret_cast like Clear).
  fable_deque.h offers FABLE_STLPORT_DEQUE_PUSH_BACK (+ _AUX_NOINLINE).
- 0x0043E3D0 ~CSetTextInfo: 00/43/CSubtitleRendererCSetTextInfo_CSetTextInfo_0043e3d0.cpp, `??1CSetTextInfo@CSubtitleRenderer@@QAE@XZ`.
- 0x0099B800 CWideString::operator=(const wchar_t*) (00/99/CWideString_operatorwchartconst_0099b800.cpp); Clear declares it
  `const CWideString& operator=(const wchar_t*)` -- FLAG draft returns `CWideString&` and takes `const unsigned short*` with a cast.
  Literal L"" = 0x0122D70C.
- 0x0048F6BD `?GetTextFromTextTag@CSubtitleRenderer@@AAE_NABVCWideString@@AAV2@@Z` (private, returns bool) -- landed 00/48/..._0048f6bd.cpp.
- 0x00492381 `?ProcessEmbeddedGraphicTags@CSubtitleRenderer@@AAEXXZ` (private) -- NOT landed.
- 0x00491AD2 `?ProcessEmbeddedHighlightTags@CSubtitleRenderer@@AAEXXZ` (private) -- NOT landed.
- 0x0099EBF0 CCharString(const char*, long = -1) / 0x0099EAE0 ~CCharString (landed 00/99). Literal "" = 0x0122D70E. Args are temps:
  `SetButton1OverrideText("")`.
- 0x0048F7C0 / 0x0048F881 `?SetButton{1,2}OverrideText@CSubtitleRenderer@@QAEXABVCCharString@@@Z` (landed; header opt-in
  FABLE_RETAIL_CSUBTITLERENDERER_BUTTONOVERRIDE declares Button2 only).
- 0x00449970 `?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ` returns CPlayer& -- FLAG draft returns CPlayer*.
- 0x00487DC0 `?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ` (const) -- FLAG draft `GetHeroThing`/CThingCreatureBase.
- 0x0043DBD0 `??$IS_THING_ALIVE@PAVCThingPlayerCreature@@@@YI_NABQAVCThingPlayerCreature@@@Z` (__fastcall, ptr by const ref) --
  FLAG draft instantiates IS_THING_ALIVE<CThing*>; must be <CThingPlayerCreature*>.
- 0x00410DE0 std::bitset<274>::test (CThing::UsingComponents +0x20) -- FLAG draft `CBitset Present`.
- 0x0040F020 CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>>::LowerBound(const ETCInterfaceType&) via the inline
  CThing::GetTC(ETCInterfaceType) -- FLAG draft `CInterfaceMap`/`GetTC<T>(long)`; copy Clear's CThing::GetTC model verbatim.
- 0x0048DD94 `?Pause@CSubtitleRenderer@@QAEX_N@Z` (header opt-in FABLE_RETAIL_CSUBTITLERENDERER_PAUSE).
- 0x0048F939 `?InitPlayerMode@CSubtitleRenderer@@AAEXXZ` (private) -- FLAG draft `ProcessPlayerModes`.
- 0x00493BC2 std::list<const CMessageEvent*>::list(const allocator&) out of line (landed as a placeholder
  global_ListNodeCreateCircularSentinel_00493bc2.cpp, `ret 4`, arg = &allocator temp [ebp+0Fh]).
- 0x00493EAB `??$FindAllMessages@VCMessageEventFilter_IsOfType@@@CMessageEventManager@@QBE?BJABVCMessageEventFilter_IsOfType@@AAV?$list@PBVCMessageEvent@@...@Z`
  -- CONST member, returns `const long`, output is std::list<const CMessageEvent*>&. FLAG draft non-const, returns long, custom list.
- 0x00493B5B `?RemoveMessage@CMessageEventManager@@QAEXPBVCMessageEvent@@@Z` (opt-in FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE, decl only).
- 0x00493BDD list<...>::~list out of line (std_list_T_00493bdd_dtor_00493bdd.cpp; placeholder name). fable_containers.h has
  FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE.
- vcall [PFont vtbl+0x10] = slot 4 `CFontBank::Preload(const CWideString&, bool) const` (engine/CFontBank.h line 41, 0x009A0B20) --
  FLAG draft `LayoutText`. Called as `PFont->Preload(Text, true)` (push 1; push &Text).
- Globals: 0x013B86A0 = CMainGameComponent::PMainGameComponent via inline static Get() (naming wave 18; draft's g_pMainGameComponent
  is the old stand-in) ; +0x1C PPlayerManager (scoped_ptr<CPlayerManager>), +0x24 PWorld; CWorld +0x60 PMessageEventManager_Data
  (engine/CWorld.h, inline GetMessageEventManager() under FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER).
- Message type 0x11 = MESSAGE_EVENT_TEXT_FINISHED_2 (Clear's enum) -- filter CMessageEventFilter_IsOfType(type).

## 4. Members touched (engine/CSubtitleRenderer.h, retail offsets, struct_layouts_egor.tsv names)
+0x24 TextCharactersShowing long (`and [esi+24h],0`) ; +0x28 TextGroupHasEntriesRemaining bool ; +0x2C TextGroupIndex long (`or -1`);
+0x30 TextGroupTag_PStringData (CWideString) ; +0x34 TextInfoQueue opaque [0x28] (queue<CSetTextInfo,deque>) ; +0x5C Text_PStringData;
+0x60 NameText ; +0x64 Button1Text ; +0x68 Button2Text ; +0x6C Button3Text (all `X_PStringData`, reinterpret_cast<CWideString&>
as Clear does) ; +0x78/+0x79 Button1Alpha/Button2Alpha =0xFF ; +0x7C DisplayFlags ; +0x80 PFont (CFontBank*) ; +0x88 MaxDisplayTime ;
+0x8C TextTimer (fldz) ; +0xDD AllCharactersDisplayed ; +0xE5 DisplayingText ; +0xE6 UpdateWhilePaused ; +0xE7 UpdateTimer.
Header opt-ins available: FABLE_RETAIL_CSUBTITLERENDERER (IsDisplayingText inline), _PAUSE, _BUTTONOVERRIDE, _CLEAR, _SETTEXT,
_SETTEXT_CHAR (declares this 10-arg overload), _INTERACTIVE, _INCUTSCENE, _GRAPHICTAG, _CTOR, _DTOR. Clear uses the
`#define CSubtitleRenderer CSubtitleRendererLayout` + derived-class trick to add the nested CSetTextInfo + member decls.
Other headers: engine/CWorld.h, engine/CPlayer.h, engine/CPlayerManager.h, engine/CThing.h (UsingComponents +0x20, Components +0x44),
engine/CMessageEventManager.h, engine/CFontBank.h, engine/CTCHeroReceiveItems.h (no members past PickedUpItems modelled).
- Hero component byte: `mov byte ptr [GetTC(0xCB)+0x26],1` = CTCHeroReceiveItems::DisplayingItems (Ego_r +0x32; retail is Ego_r-0xC,
  same shift as Clear's ForceClear Ego_r +0x33 -> retail +0x27). FLAG draft `CHeroControls::Frozen`. Interface 0xCB =
  TCI_HERO_RECEIVE_ITEMS (Clear).
- Classes the draft models LOCALLY that have shared headers: CSubtitleRenderer, CThing, CPlayer, CPlayerManager, CWorld,
  CMainGameComponent, CMessageEventManager, CFontBank, CVectorMap (draft's CInterfaceMap), CTCHeroReceiveItems (draft CHeroControls).

## 5. Callee visibility
- Same TU (CSubtitleRenderer.cpp, defined EARLIER): Pause 0x0048DD94, CSetTextInfo ctor 0x0048DDF6, GetTextFromTextTag 0x0048F6BD,
  SetButton1/2OverrideText 0x0048F7C0/0x0048F881, InitPlayerMode 0x0048F939, ProcessEmbeddedHighlightTags 0x00491AD2,
  ProcessEmbeddedGraphicTags 0x00492381. All called out of line in retail.
- COMDATs emitted after the TU (0x00493xxx/0x00494xxx, unaligned odd addresses = template/inline instantiations of this TU):
  list ctor 0x00493BC2, list dtor 0x00493BDD, RemoveMessage 0x00493B5B (out of line here! inline in 0x0089AF30's TU),
  FindAllMessages 0x00493EAB, deque::push_back 0x004941C0. Sibling 0x0089AF30 note: "the sentinel stays in edi across
  FindAllMessages only when that template's body (and push_back's) is compiled earlier in the TU" -- they gave those bodies
  under `#pragma auto_inline(off)` before the row. Consider the same for FindAllMessages / list ctor / dtor / RemoveMessage here.
- Opt-ins: FABLE_RETAIL_STLP_LIST, FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE, FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE (fable_containers.h);
  FABLE_STLPORT_DEQUE_PUSH_BACK (fable_deque.h); FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE; FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER.

## 6. Sibling landed rows (copy idioms)
- 00/49/CSubtitleRenderer_Clear_00492BAB.cpp -- next function in retail; THE template: Layout-rename include trick, nested CSetTextInfo,
  std::queue/deque stand-ins, CThing::GetTC FABLE_FORCEINLINE over CVectorMap::LowerBound, CPlayer::GetPControlledCreature,
  IS_THING_ALIVE(hero), CMainGameComponent::Get(), `// Flags: /GS` (this row needs none), pragmas y-off + s-on.
- 00/49/CSubtitleRenderer_SetText_00492E4B.cpp, _00492F69.cpp -- forwarding overloads; y-off + s-on.
- 00/48/CSubtitleRenderer_Pause_0048DD94.cpp -- includes engine/CSubtitleRenderer.h directly.
- 00/89/CGameScriptInterface_RemoveAllCutSceneSkippedMessages_0089AF30.cpp -- the FindAllMessages(IsOfType)+remove idiom, with
  list<const CMessageEvent*> model and auto_inline(off) callee bodies.

## 7. Draft model errors vs disassembly
- Body control flow is right (DIFFER 572v568, tail only). Residue: retail homes the found list in the dead `text` slot [ebp+0Ch]
  (allocator temp at [ebp+0Fh]) and the filter in [ebp+14h]; the draft gets list +8/filter +0xC; and retail's removal loop is
  unrotated (jmp-to-test) with sentinel in edi, manager in ebx.
- Draft list/ctor: retail calls an out-of-line list ctor taking `const allocator&` (default-arg temp) -- draft's CAllocatorTag stand-in
  should be the real STLport `std::list<const CMessageEvent*>` with `explicit list(const _Alloc& a = _Alloc())`.
- Loop: retail iterates begin()..end() via iterator (`it = head->_M_next` loaded via `mov esi,[edi]`), calling RemoveMessage(*it);
  write `for (std::list<const CMessageEvent*>::iterator it = found.begin(); it != found.end(); ++it)` with a real STLport iterator
  (or a while loop) -- draft's raw-node for loop rotates.
- FindAllMessages is const and returns `const long`; manager pointer is read as `Get()->PWorld->PMessageEventManager_Data` (ebx)
  before the list is constructed -- keep a named manager local declared before the list.
- Names wrong in draft: GetHeroThing->GetPControlledCreature, ProcessPlayerModes->InitPlayerMode, LayoutText->Preload,
  CHeroControls::Frozen->CTCHeroReceiveItems::DisplayingItems, IS_THING_ALIVE<CThing*> -> <CThingPlayerCreature*>,
  CBitset Present -> std::bitset<274> UsingComponents, CInterfaceMap -> CVectorMap Components, g_pMainGameComponent ->
  CMainGameComponent::Get(), top-level CSetTextInfo -> CSubtitleRenderer::CSetTextInfo, local CSubtitleRenderer -> shared header.

## 0x00918640 (not landed)

# DOSSIER 0x00918640 CAIStateGroup_Gaze::FindFreeParticularGazePos (570 bytes, ret 8)

## 1. Retail shape (xdis 00918640 0x24a)
- __thiscall, `ret 8` (long, bool). No frame pointer (`sub esp,18h ; push ebx,ebp,esi ; mov ebx,ecx ; push edi`), no SEH -> plain /O2 /Oy, no pragma needed.
- Frame 0x18 locals: [esp+14h] = saved `this` (ebx spill, reloaded at 0x918807 for GetPWorldSeed), [esp+18h] = key temp (0x21) AND push_back value temp, [esp+1Ch..24h] = std::vector<CThingGameObject*> {start,finish,eos} zeroed via esi=0, [esp+13h] byte = filter functor object (branch 1).
- Branch 2 reuses PARAM slots: key 0x23 stored into the bool param slot [esp+30h]; filter functor ecx=&bool-param slot; gazeType cached in ebx and push_back temp stored into the gazeType param slot [esp+2Ch]. Branch 1 instead re-reads gazeType from [esp+2Ch] and the bool from [esp+30h] each iteration.
- `this` lives in ebx (draft had esi). Building in ebp; list node in edi; &node->data in esi.
- Early failures (no building TC 0x21 bit / no village-member bit / GetPVillage()==0) jump straight to 0x91886E: `pop; xor eax,eax; ret 8` with NO vector free (VC proved start==0). Success tail: count = (finish-start)>>2, `test/jbe` (unsigned size()==0) -> free(start) if non-null, return 0; else random index, load start[idx] into esi BEFORE free(start), return esi. Vector dtor = inline `if(p) free(p)` (STLport allocator deallocate, call 0x00BFEA14 free thunk), emitted twice (once per return).
- Random: GetPWorldSeed() (ecx=ebx) -> `seed=seed*0x24A1+0x24DF; seed=GFROR13(seed); xor edx,edx; test edi,edi; je; div edi` = `n ? seed % n : 0` (same WorldRandomRange inline as sibling 0x00918AC0).
- Loops: STLport list walk `for(it=begin; it!=end; ++it)`; branch 1 compares `edi` against `[ebx+0Ch]` (list header re-read via positions ptr each iteration -> list accessed through a pointer/reference, end() re-evaluated); branch 2 compares against `[ebp]` where ebp = &village->VillageMarkers (+0x70) (end re-read through a reference to the list). Loop top 16-aligned (`lea esp,[esp]` pad) in branch 1.
- Condition chain per element (both branches): GetPItem()!=0, !(item+0x91 &1) [IsThingAlive / IS_THING_ALIVE nested-if], `movzx word [item+8Ch]` == gazeType (GetDefGlobalIndex()), filter(*item) (CThingFilter_IsNotBeingExclusivelyUsed::operator(), push eax = CThing&), branch 1 only: `if(bool) building->IsPointInside(item->GetPos())`, then push_back(item) through a temp (`mov [esp+18h],eax ; lea eax,[esp+18h]; push eax`). GetPItem called 4-6 times (no caching -> pointer.Get() each time).
- Interface lookups are the GetTC expansion: HasTC bit test (`test byte [ebp+24h],2` = bit 0x21; `test byte [eax+24h],8` = bit 0x23), key store AFTER the test, `lea esi,[x+44h]` map pointer, out-of-line LowerBound 0x0040F020, end read as `[esi+4]`, `cmp [eax],key ; jle` conditional-expression form, `->second` at +4. This is exactly FABLE_RETAIL_CTHING_GETTC_EXPANDED's GetTC(id,&ptc).
- Else branch creature: `PBrain(+4)->+0x18` (CAIBrain creature), not a virtual.

## 2. Name / signature (two sources agree; manifest label CORRECT)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv @0093bcc0) and FableWin.pdb (_fablewin_api.tsv 017b4035/02bb4910; naming_stragglers FableWin.exe.signatures.tsv):
  `?FindFreeParticularGazePos@CAIStateGroup_Gaze@@IAEPAVCThingGameObject@@J_N@Z`
  = protected: `CThingGameObject* CAIStateGroup_Gaze::FindFreeParticularGazePos(long, bool)` (non-const, non-virtual, __thiscall).
- Caller: only CAIStateGroup_Gaze::FindFreeGazePos 0x00918880 (xref 0x009188D4, 0x009188E9), landed rebuild/src/compiled/00/91/CAIStateGroupGaze_FindFreeGazePos_00918880.cpp, passes (PrimaryMarkerDef / SecondaryMarkerDef, inSleepTime). Param names in that row: markerDef, inSleepTime (descriptive).
- NAME_FIX for the coordinator: FindFreeGazePos row calls slot 15 (+0x3C) `IsStateGroupStillValid` -- FableWin.pdb has `?InsideAtNight@CAIStateGroup_Gaze@@MAE_NXZ` and the vtable 0x0128FDEC slot 15 = 0x00918ED0 (landed CAIStateGroupGaze_InsideAtNight_00918ed0.cpp). Proper spelling: InsideAtNight.

## 3. Callees
| addr | role | landed file | landed name | PDB name | draft spelling |
|---|---|---|---|---|---|
| vslot 14 [eax+38h] | building | CAIStateGroupGaze_GetOwner_00918ec0.cpp (slot 14 of 0x0128FDEC) | GetOwner | `?GetOwner@CAIStateGroup_Gaze@@MAEPAVCThingBuilding@@XZ` (FableWin, protected virtual) | **GetPBuilding -- WRONG** |
| 0x0040F020 | map lower bound | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound(const ETCInterfaceType&) | same | local CInterfaceMap::LowerBound defined in TU -- use engine/CVectorMap.h |
| 0x00A01B50 | | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | CBaseIntelligentPointer::GetPItem() const | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` | returns CThing* in draft (should be CBaseObject*; use CIntelligentPointer<T>::Get()) |
| 0x006A8AF0 | free test | 00/6a/CThingFilterIsNotBeingExclusivelyUsed_operator_006a8af0.cpp | CThingFilter_IsNotBeingExclusivelyUsed::operator()(const CThing&) const | `??RCThingFilter_IsNotBeingExclusivelyUsed@@QBE_NABVCThing@@@Z` | **CGazePosFree / pointer arg -- WRONG** |
| 0x004C73D0 | | 00/4c/CThing_GetPosition_004c73d0.cpp | CThing::GetPos() const | `?GetPos@CThing@@QBEABVC3DVector@@XZ` | **GetPosition -- WRONG** (engine/CThing.h:411 declares GetPos) |
| 0x0082D000 | | 00/82/CThingBuilding_IsPointInside_0082d000.cpp | CThingBuilding::IsPointInside | `?IsPointInside@CThingBuilding@@QBE_NABVC3DVector@@@Z` (CONST) | draft non-const |
| 0x00436C80 | vector push_back | 00/43/global_PushBackdword00436c80_00436c80.cpp (COMDAT-folded 4-byte vector push_back; callers model it as `vector<CThing*>::push_back`, e.g. 00/90/CAddFreeBenchToList_operator_0090c060.cpp line 90) | out of line | std::vector<T*>::push_back(const T*&) | ok as out-of-line decl |
| 0x007EE540 | village | 00/7e/CTCVillageMember_GetPVillage_007ee540.cpp | CTCVillageMember::GetPVillage() const | `?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ` | **CTCRegionLink::GetPRegion -- WRONG** |
| 0x008FCFD0 | | 00/8f/CAIStateGroupBase_GetPWorldSeed_008fcfd0.cpp | GetPWorldSeed | `?GetPWorldSeed@CAIStateGroupBase@@IBEPAKXZ` (protected CONST) | draft non-const |
| 0x00497890 | | 00/49/global_GFROR13_00497890.cpp | GFROR13 (__fastcall) | GFROR13 | ok |
| 0x00BFEA14 | | 00/bf/global_free_00bfea14.cpp | free import thunk | msvcrt _free | ok |
Interface ids: 0x21 = TCI_THING_OWNER (engine/ETCInterfaceType.h:38) -> CTCThingOwner (draft "CTCGazePositions" WRONG); 0x23 = TCI_VILLAGE_MEMBER (:40) -> CTCVillageMember (draft "CTCRegionLink" WRONG).

## 4. Members
- this: CAIStateGroupBase::PBrain +0x04 (engine/CAIStateGroupBase.h; retail branch FABLE_RETAIL_CAISTATEGROUPBASE, members FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS declares `unsigned long* GetPWorldSeed() const`; FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE declares inline `CThingAICreature& GetCreature() const` with body in engine/CAIBrain.h under FABLE_RETAIL_CAIBRAIN_GETCREATURE). CAIStateGroup_Gaze.h is a flat data-only generated header (no vptr chain / no methods); all landed Gaze siblings model the class locally (see 6).
- CAIBrain +0x18 creature (CThingAICreature*), via GetCreature.
- CThing (engine/CThing.h): UsingComponents bitset<274> +0x20 (HasTC), Components CVectorMap +0x44, DefGlobalIndex `unsigned short` +0x8C protected, accessor `unsigned long GetDefGlobalIndex() const` (CThing.h:267; FableWin `?GetDefGlobalIndex@CThing@@QBEKXZ`) -- the draft's `GazeType` is wrong; the bit +0x91 bit0 = `Dead` bitfield, `IsDead()` (CThing.h:280). Opt-ins: FABLE_RETAIL_CTHING_GETTC_EXPANDED (GetTC(id,T**) with HasTC+out-of-line LowerBound+cond-expr), FABLE_RETAIL_CTHING_HASTC_FORCEINLINE.
- CTCThingOwner (engine/CTCThingOwner.h): `std::list<CIntelligentPointer<CThing> > ObjectList` +0x0C (private), accessor `GetObjectList()` (both const/non-const; FableWin `?GetObjectList@CTCThingOwner@@QAEAAV?$list...`), FABLE_RETAIL_CTCTHINGOWNER gives StatGetInterfaceType()=TCI_THING_OWNER.
- CTCVillage (engine/CTCVillage.h, FABLE_RETAIL_CTCVILLAGE branch): `std::list<CIntelligentPointer<CThingMarker> > VillageMarkers` +0x70 (Ego_r name). FableWin accessor `?PeekVillageMarkers@CTCVillage@@QBEABV?$list@V?$CIntelligentPointer@VCThingMarker@@@@...@XZ` (inline; NOT yet in the header -- only PeekVillagePeople under FABLE_RETAIL_CTCVILLAGE_PEEKVILLAGEPEOPLE exists; a new nested opt-in mirroring that one is needed).
- CTCVillageMember (engine/CTCVillageMember.h:39 `CTCVillage* GetPVillage() const;` 0x007EE540).
- CIntelligentPointer (engine/CIntelligentPointer.h, FABLE_RETAIL_CINTELLIGENTPOINTER): Get() = static_cast<T*>(GetPItem()) inline.
- Locally modelled in draft though a shared header exists: CThing, C3DVector, CThingBuilding, CBaseIntelligentPointer, CAIBrain, CAIStateGroupBase, CAIStateGroup_Gaze, the interface map (CVectorMap), list nodes (std::list in stlport/fable_containers.h), vector (std::vector in fable_containers.h, deallocate = `if(p) free(p)` line ~330), CTCThingOwner, CTCVillage, CTCVillageMember.

## 5. Callee visibility
- LowerBound 0x0040F020, GetPItem 0x00A01B50, GetPos 0x004C73D0, IsPointInside, GetPVillage, GetPWorldSeed, filter op(), push_back 0x00436C80: all out of line in retail and in other TUs -> declare only (the draft's in-TU LowerBound with auto_inline(off) is a stand-in; header CVectorMap.h declares it).
- Same TU (Gaze .cpp, retail 0x00918050..0x00919040): CIsFreeBench::operator() 0x00918050, std::find_if list 0x00918110, ctors 0x00918190.., InitStateGroup 0x00918350, OnReplace 0x009183A0, FindFreeGazePos 0x00918880 (defined AFTER this row), IsThingAlive 0x00919040 (out-of-line IS_THING_ALIVE<CIntelligentPointer<CThing>> instance, emitted late) -- here it is inlined (GetPItem twice + test byte [+91h],1), matching its landed body (00/91/global_IsThingAlive00919040_00919040.cpp: `if(p.Get()!=0){ if(!p.Get()->IsDead()) return true;} return false;`).
- No STLport map/tree helpers. Vector dtor/size inline.

## 6. Siblings (same TU)
- 00/91/CAIStateGroupGaze_FindFreeGazePos_00918880.cpp -- local model, no pragma/flags.
- 00/91/CAIStateGroupGaze_InitCreaturePosition_00918ac0.cpp -- local model + `#include "engine/CThing.h"`, `engine/C3DVector.h`; inline `WorldRandomRange(unsigned long& seed, unsigned long range){seed=seed*0x24A1+0x24DF; seed=GFROR13(seed); if(range==0) return 0; return seed%range;}` called as `WorldRandomRange(*GetPWorldSeed(), n)` -- copy for this row's random.
- 00/91/CAIStateGroupGazeAtHome_IsRunnable_009189c0.cpp -- `#define FABLE_RETAIL_CTHING_GETTC_EXPANDED`, engine/CThing.h + CVectorMap.h, GetTC(TCI_..., &ptr), local CAIStateGroupBase with slot 14 `virtual CThingBuilding* GetOwner();`.
- 00/91/global_IsThingAlive00919040_00919040.cpp -- FABLE_RETAIL_CINTELLIGENTPOINTER + engine/CThing.h IsDead().
- 00/91/CAIStateGroupGaze_scalardeletingdestructor_00918ee0.cpp uses `#pragma optimize("t", on)` (only pragma in the class).
- 00/90/CAddFreeBenchToList_operator_0090c060.cpp -- push_back 0x00436C80 modelled as out-of-line `vector<CThing*>::push_back`.

## 7. Old draft model errors
- Wrong callee/class names (GetPBuilding->GetOwner, GetPosition->GetPos, CTCRegionLink::GetPRegion->CTCVillageMember::GetPVillage, CGazePosFree->CThingFilter_IsNotBeingExclusivelyUsed taking const CThing&, GazeType->GetDefGlobalIndex(), CTCGazePositions->CTCThingOwner::GetObjectList(), m_gazeList->VillageMarkers/PeekVillageMarkers(), return CThing*->CThingGameObject*).
- Early `return 0` exits: retail elides the vector free on those paths, the draft kept a free test (+11 bytes). NOTES (hand277 line 143) suggests: single exit / nested ifs without early returns (`if(building){ if(GetTC(0x21,&owner)) {loop} } else { if(GetTC(0x23,&member)) { if(member->GetPVillage()) {loop} } }` then `if(!v.empty()) return v[rand]; return 0;`) -- note retail's success path frees AFTER loading the element, so the element is read into a local, then dtor runs at scope exit; the zero-size path also frees.
- `this` in esi vs retail ebx: retail spills this to [esp+14h] because ebx is reused for gazeType (branch 2) and the list owner pointer (branch 1). Using GetCreature()/PBrain only in the else branch and calling GetPWorldSeed() on this at the end matches.
- Random zero test: retail keeps `test edi,edi ; je` inside (the `n ? %n : 0` form) though count>0 is already known -> use the sibling's WorldRandomRange inline verbatim.
- Draft's `HasTC(...) return 0; GetTC(...)` split should be the single GetTC(id,&ptc) form (key store after bit test).
- push_back argument: retail stores GetPItem() result into a temp then passes its address -> `v.push_back(pos.Get())` with `const T&` param (not a named local).
- Branch-1 element ptr: retail calls GetPItem anew for each use (no local `item`).

## 0x00881A40 (not landed)

# DOSSIER 0x00881A40 NScreenEffect::CScreenEffectPriorityStack::ScreenFilterSetFlags (581 bytes)

## 1. Retail shape (xdis 00881a40 0x255)
- __thiscall, `ret 8` (2 stack args), NO frame pointer (`sub esp,14h ; push ebx ; push esi`, then `push ebp ; push edi` only after the
  early-out test), no SEH frame, no `optimize("s")` idioms (imul 38E38E39h size divide inlined 3x, `jl` loop back-edges, `mov edi,edi`
  2-byte alignment pad before loop 1 at 0x00881A9E). => plain speed TU, /Oy on, no pragmas needed.
- Early out: `ScreenFilterKeys.find(entry)` (call 0x008839E0, ecx=this+0xA0, hidden iterator result local, key local = handle stored
  `mov [esp+10h],ebx`) ; `cmp edx,[esi]` vs header ; `je` straight to the pop/ret epilogue (ebp/edi not yet pushed).
- Register plan retail: esi = &args (arg1 reloaded into esi after find), ebx = handle, then ebx = &entry->reset (`add ebx,30h`) for all of
  loop 1; loop 1: ebp = counter i, arg1's stack slot [esp+28h] reused as the byte offset i*9, edi = args._M_start (reloaded every
  iteration as part of `size()`); loop 2: [esp+28h] (arg1 slot) = counter j, edi = byte offset j*9, ebp = &args[j] then the graphic.
  arg2 (handle) re-read from [esp+2Ch] in loop 2 to form entry->reset (`add ecx,30h`).
- size() = (finish-start)/9 recomputed at loop entry and at each back-edge (`for (i=0; i<args.size(); ++i)` with signed compare `jl`/`jle`
  => int counter compared against `(int)size()` or size() returning a signed difference -- STLport vector::size() returns size_type,
  but retail compares signed: counter is `int`, size cast or the comparison is int vs ... check: `cmp ebp,ecx ; jl` = signed).
- Loop 1 body (per element): key = args[i].thing.GetPItem() (0xA01B50, result stored to key local [esp+14h]); it = reset.find(key)
  (0x00883860, hidden result [esp+10h]); `if (it != reset.end())` (cmp vs [ebx] header): edi=&node->second(+0x14);
  GetPItem x3 on it (non-null, `test [eax+91h],1` Dead, `mov ecx,[eax+64h]` PTCGraphicAppearance, null test) then
  SetByPassColorFilter(bypassReset @ node+0x1C = second+8, zero-extended `xor edx,edx ; mov dl,..`);
  then INLINE erase(iterator): `_Rebalance_for_erase(node, header->_M_parent(+4), header->_M_left(+8), header->_M_right(+0xC))` cdecl
  `add esp,10h`; `~CBaseIntelligentPointer` on node+0x14 (0xA01C10, the value's destructor = ~CThingGraphicByPassFlagState);
  `if (node) free(node)` (0xBFEA14); `dec [ebx+4]` (--_M_node_count).
- Loop 2 body: GetPItem(args[j].thing) x2 on ebp (non-null, Dead bit), third GetPItem re-forms `[esi]+edi` (args[j] re-indexed, not a
  cached ref) -> graphic=[eax+64h] in ebp, null test; then local CThingGraphicByPassFlagState state at [esp+18h]: base ctor 0xA01B10,
  vptr 0x01238C6C, SetPItem(0) (i.e. the default ctor's thing pointer is set through SetPItem(0)), `state.bypassReset=false` scheduled
  late; `state.thing = args[j].thing` (GetPItem on args[j] -> SetPItem on state: the ASSIGN form); `state.bypassReset =
  graphic->ByPassColorFilter > 0` (`mov al,[ebp+5Bh] ; test al,al ; setg cl` = inline GetByPassColorFilter() const);
  key = state.thing.GetPItem() into [esp+14h]; `entry->reset[key]` (0x00884C70, ret 4) -> ebx; `*ebx = state` as
  SetPItem(state.thing.GetPItem()) + byte copy of bypassReset (CThingGraphicByPassFlagState::operator= = the PDB's ??4, inline);
  graphic->SetByPassColorFilter(args[j].bypassSet) (byte re-read `[edi+ecx+8]`); state destroyed via 0xA01C10.

## 2. Identity (two sources -- CONFIRMED, manifest label right)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv, Ego_r addr 0x008C27D0) and FableWin.pdb (ghidra_out/_fablewin_api.tsv 0x02A59710 /
  naming_stragglers FableWin.exe.signatures.tsv): 
  `?ScreenFilterSetFlags@CScreenEffectPriorityStack@NScreenEffect@@QAEXABV?$vector@UCThingGraphicArgPasser@CScreenEffectPriorityStack@NScreenEffect@@V?$allocator@UCThingGraphicArgPasser@CScreenEffectPriorityStack@NScreenEffect@@@std@@@std@@PAX@Z`
- Signature: `void NScreenEffect::CScreenEffectPriorityStack::ScreenFilterSetFlags(const std::vector<CThingGraphicArgPasser>& , void*)`,
  public non-const non-virtual (QAE). Shared header already declares it as `ScreenFilterSetFlags(const ThingGraphicArgPassers& things,
  void* handle)` (rebuild/include/engine/NScreenEffect_CScreenEffectPriorityStack.h). PDB param names not on disk.
- Caller: only the tail-jump in landed CEnvironmentSpellEffectInterpolator::ScreenFilterSetFlags 0x006B83E0
  (`add ecx,90h ; jmp 0x00881A40`; xref.py shows 006b83e6), rebuild/src/compiled/00/6b/CEnvironmentSpellEffectInterpolator_ScreenFilterSetFlags_006b83e0.cpp.
- Not virtual (no vtable slot).

## 3. Callees
| addr | landed file | landed name | real (PDB/STLport) name | draft spelling |
|---|---|---|---|---|
| 0x008839E0 | 00/88/CRetailTreeulongkey_find_008839e0.cpp | `CRetailTree_ulongkey::find` (invented, "one of 12 bodies") | STLport `_Rb_tree<CStackEntry*,...>::find` = `std::set<CStackEntry*>::find` (out-of-line COMDAT; body shared by 12 instantiations) | `CKeySet::find` local -- fine as model, but use std::set |
| 0x00883860 | 00/88/stdRbtreeCUnprovenKey00883860..._find_00883860.cpp | `_Rb_tree<CUnprovenKey_00883860,...>::find` (placeholder types) | `_Rb_tree<CThing*, pair<CThing* const, CThingGraphicByPassFlagState>, _Select1st, less<CThing*> >::find` (map::find, out of line). Hand back NAME_FIXES: the key/value are now proven by this row | `CResetMap::find` |
| 0x00884C70 | 00/88/STLmap_operator_00884c70.cpp | std::map::operator[] | `map<CThing*, CThingGraphicByPassFlagState>::operator[]` (Ego_r ??A?$map@PAVCThing@@VCThingGraphicByPassFlagState@...), ret 4, out of line | ok |
| 0x0042A34E | 00/42/stdRbglobalbool_Rebalanceforerase_0042a34e.cpp | `std::_Rb_global<bool>::_Rebalance_for_erase` (static cdecl) | same | WRONG: draft `_Rb_tree_rebalance_for_erase` free function (SGI name, label-propagated) |
| 0x00BFEA14 | 00/bf/global_free_00bfea14.cpp | free | CRT free (via STLport malloc allocator deallocate) | ok |
| 0x00A01B10 | 00/a0/CBaseIntelligentPointer_CBaseIntelligentPointer_00a01b10.cpp | CBaseIntelligentPointer() | same | ok |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | GetPItem() const | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` -- returns CBaseObject* | draft returns CThing* (wrong type) |
| 0x00A01B90 | 00/a0/CBaseIntelligentPointer_SetPItem_00a01b90.cpp | SetPItem | `?SetPItem@..@@QAEXPAVCBaseObject@@@Z` | draft takes CThing* (wrong) |
| 0x00A01C10 | 00/a0/CBaseIntelligentPointer_CBaseIntelligentPointer_00a01c10.cpp | ~CBaseIntelligentPointer (virtual) | same | draft non-virtual dtor + fake v000 |
| 0x004BFB00 | 00/4b/NGraphicAppearanceCTCGraphicAppearance_SetByPassColorFilter_004bfb00.cpp | NGraphicAppearance::CTCGraphicAppearance::SetByPassColorFilter(bool) | `?SetByPassColorFilter@CTCGraphicAppearance@NGraphicAppearance@@QAEX_N@Z` | draft drops the NGraphicAppearance namespace (mangling irrelevant for a direct call but name must be exact) |

Inline (no call) but named in PDBs: `?GetByPassColorFilter@CTCGraphicAppearance@NGraphicAppearance@@QBE_NXZ` (the `setg` on +0x5B),
`??4CThingGraphicByPassFlagState@...@QAEAAV0123@ABV0123@@Z`, `??0CThingGraphicByPassFlagState@...@QAE@XZ`, `??1...`, and
`?Reset@CThingGraphicByPassFlagState@CScreenFilterEntry@CScreenEffectPriorityStack@NScreenEffect@@QAEXXZ` (FableWin/Ego_r) -- almost
certainly the loop-1 restore block (IS_THING_ALIVE(thing) && graphic -> SetByPassColorFilter(bypassReset)), inline here; the same
block is in landed CScreenFilterEntry::Reset 0x00883AE0.

## 4. Members
- this+0xA0: `ScreenFilterKeys` (std::set<CStackEntry*>, 12-byte STLport). Ego_r offset 240 (debug containers bigger); retail +0xA0
  per landed DeleteDisplacementEntry 0x00881860 layout notes (+0x6C DisplacementParamStack, +0x78 DisplacementKeys, +0x84 colour ...).
  Shared header: NScreenEffect_CScreenEffectPriorityStack.h FULL branch (`FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_FULL`) declares
  RadialParamStack[3] .. ScreenFilterKeys in PDB order; verify its sizeof math gives +0xA0 (FULL needs fable_containers.h/fable_map.h with
  FABLE_RETAIL_STLP_LIST / FABLE_RETAIL_STLP_MAP like landed InsertRadialBlurEntry 0x00881690). Default branch is member-less.
- CScreenFilterEntry (handle cast): `reset` map<CThing*, CThingGraphicByPassFlagState> at +0x30 (struct_layouts_egor.tsv:
  CScreenFilterEntry 64 bytes, +48 `reset`; landed Reset 0x00883AE0 uses +0x30, CStackEntry base +0..+0xF, CScreenFilterParams +0x10).
  Header: FULL branch has only a forward decl `struct CScreenFilterEntry;`; PROCESSES branch has CScreenFilterEntry : CStackEntry with only
  Reset(). No shared header models `reset` -- header debt (landed 0x00883AE0 / 0x00884C70 model it locally).
- CThingGraphicByPassFlagState (Ego_r 9 bytes): `thing` CIntelligentPointer<CThing> +0, `bypassReset` bool +8 (draft calls it `bypass`:
  WRONG spelling). Map node: key +0x10, value +0x14 (thing), +0x1C bypassReset.
- CThingGraphicArgPasser (9 bytes, pack 1): `thing` +0, `bypassSet` +8. Shared header opt-in `FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_ARGPASSER`
  (needs engine/CIntelligentPointer.h with FABLE_RETAIL_CINTELLIGENTPOINTER (+_COPY) and complete CThing first).
- CThing +0x64 `PTCGraphicAppearance` (NGraphicAppearance::CTCGraphicAppearance*), +0x91 bit0 `Dead` bitfield: engine/CThing.h
  (IsDead(), GetTCGraphicAppearance() inline accessors exist). IS_THING_ALIVE<T>: engine/ThingAlive.h, opt-in FABLE_RETAIL_IS_THING_ALIVE
  (`if(thing!=0){ if(!thing->IsDead()) return true;} return false;` -- on a CIntelligentPointer needs operator->/compare; landed
  Reset 0x00883AE0 instead spells it as nested ifs on GetPItem()).
- CTCGraphicAppearance +0x5B `ByPassColorFilter` signed char: engine/NGraphicAppearance_CTCGraphicAppearance.h line 98 (private-ish
  block); neither GetByPassColorFilter nor SetByPassColorFilter is declared in that header (grep: no hits) -- add or declare locally.
- CIntelligentPointer<CThing> vtable 0x01238C6C: engine/CIntelligentPointer.h, FABLE_RETAIL_CINTELLIGENTPOINTER (+ _ASSIGN: `item=
  other.GetPItem(); SetPItem(item)`, _COPY). Its default `CIntelligentPointer()` is EMPTY in the shared header, but retail state ctor
  calls SetPItem(0): CThingGraphicByPassFlagState() must init `thing(0)` (CIntelligentPointer(T*) -> SetPointer -> SetPItem) or the
  landed 0x00884C70 local model `CIntelligentPointer() { SetPItem(0); }`.
- Classes the old draft models LOCALLY that have shared headers: CThing (engine/CThing.h), CTCGraphicAppearance
  (engine/NGraphicAppearance_CTCGraphicAppearance.h), CBaseIntelligentPointer/CIntelligentPointer (engine/CIntelligentPointer.h),
  NScreenEffect::CScreenEffectPriorityStack (+CStackEntry, CThingGraphicArgPasser) (engine/NScreenEffect_CScreenEffectPriorityStack.h),
  STLport set/map/vector (stlport/fable_map.h, fable_containers.h).

## 5. Callee visibility
- set::find 0x008839E0, map::find 0x00883860, map::operator[] 0x00884C70: STLport inline members emitted out of line (COMDATs) in
  this TU. fable_map.h: plain `find` is inline-bodied (`iterator(_M_find(key))`); FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE gives the
  template form VC7.1 called out of line (chunk 890). operator[] under FABLE_RETAIL_STLP_MAP has its body visible (out of line in retail
  anyway; the visible body lets VC see the key only read -- relevant to the key-temp slot [esp+14h] shared by both loops).
- erase(iterator): EXPANDED inline here (rebalance+destroy+free+--count). fable_map.h FABLE_RETAIL_STLP_TREE_ERASE +
  FABLE_RETAIL_STLP_TREE_ERASE_BODY gives exactly that body (`_Rb_global_inst::_Rebalance_for_erase(..)`, `_Destroy`, `deallocate`,
  `--_M_node_count`). Check deallocate emits `if(p) free(p)` (retail `test edi,edi ; je`).
- Same TU neighbours (CScreenEffectPriorityStack.cpp, 0x00880A40..0x00884FE0): InsertRadialBlurEntry 0x00881690, InsertDisplacementEntry
  0x00881780, DeleteRadialBlurEntry 0x00881820 / DeleteDisplacementEntry 0x00881860 just before; InsertScreenFilterEntry /
  DeleteScreenFilterEntry (PDB names) probably right after 0x00881C90 (next function, `sub esp,60h`). CScreenFilterEntry::Reset
  0x00883AE0 and CThingGraphicByPassFlagState ctor/??4/Reset are inline (not called).
- GetPItem/SetPItem/ctor/dtor are in another TU (0x00A01Bxx) -- not visible.

## 6. Sibling landed rows (copy idioms)
- 00/88/NScreenEffectCScreenEffectPriorityStack_InsertRadialBlurEntry_00881690.cpp: `#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_FULL`,
  FABLE_RETAIL_STLP_LIST, FABLE_RETAIL_STLP_MAP, FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY; includes stlport/fable_containers.h,
  stlport/fable_map.h, engine/CMainGameComponent.h, engine/NScreenEffect_CScreenEffectPriorityStack.h; `#pragma auto_inline(off/on)`
  around a copy ctor only. No Flags line.
- 00/88/NScreenEffectCScreenEffectPriorityStackCScreenFilterEntry_Reset_00883ae0.cpp: same `reset` map walk with the restore block --
  `#pragma pack(push,1)` + `#define FABLE_RETAIL_CINTELLIGENTPOINTER` + engine/CIntelligentPointer.h; local STLport tree; nested
  `if (state.thing.GetPItem() != 0) { if (!(((CThing*)state.thing.GetPItem())->m_flags91 & 1)) { CThing* thing = (CThing*)
  state.thing.GetPItem(); if (thing->m_pGraphicAppearance) thing->m_pGraphicAppearance->SetByPassColorFilter(state.bypassReset); } }`
  (three GetPItem calls = retail loop-1 shape).
- 00/88/STLmap_operator_00884c70.cpp: the map<CThing*,CThingGraphicByPassFlagState> model, pack(1), `CIntelligentPointer(){SetPItem(0);}`,
  `CThingGraphicByPassFlagState() : bypassReset(false) {}`.
- 00/88/NScreenEffectCScreenEffectPriorityStack_DeleteDisplacementEntry_00881860.cpp: the keys-set find 0x008839E0 with a derived-entry
  key temporary homed in the arg slot; retail stack layout comments.
- 00/88/NScreenEffectCScreenEffectPriorityStackCScreenFilterProcess_OnErase_00880f50.cpp: FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_PROCESSES.
- 00/6a/NScreenEffectCScreenEffectPriorityStackCThingGraphicArgPasser_CThingGraphicArgPasser_006a2f00.cpp, 00/6a/stdvector_Minsertoverflow_006a3950.cpp:
  ARGPASSER users.

## 7. Old draft's likely model errors
- Parked DIFFER(591v581) on register allocation (ebx/ebp swap). Probable roots: (a) draft's loop-1 restore is open-coded with a
  draft-local IS_THING_ALIVE taking a reference; retail likely calls inline `it->second.Reset()` (?Reset@CThingGraphicByPassFlagState,
  PDB) -- try that member; (b) loop 2 state is built by default ctor `CThingGraphicByPassFlagState state;` then `state.thing =
  args[j].thing` -- matches; but the bypass is `graphic->GetByPassColorFilter()` (inline const accessor, returns bool) -- draft spells
  `> 0` directly (same code, but accessor inlining can shift regalloc); (c) map assign via the PDB's ??4 (user-defined/implicit
  operator= doing ASSIGN form then bool copy) -- draft relies on CIntelligentPointer::operator= only; fine; (d) the draft casts
  `void* entry` to CScreenFilterEntry* three times; retail forms &entry->reset once in ebx for loop 1 but re-reads the arg in loop 2 --
  consistent with casting per use; (e) the find key: retail stores the handle into a separate key local ([esp+10h]) before the find
  (conversion temp void* -> CStackEntry* const&), draft passes `(CStackEntry*)entry` -- matches.
- Wrong types/names: GetPItem/SetPItem take/return CBaseObject* (draft CThing*); dtor is virtual (draft non-virtual + fake v000);
  `bypass` -> `bypassReset`; `_Rb_tree_rebalance_for_erase` -> `_Rb_global<bool>::_Rebalance_for_erase`; SetByPassColorFilter lives in
  NGraphicAppearance::CTCGraphicAppearance; vector/args should be std::vector<CThingGraphicArgPasser> (ThingGraphicArgPassers typedef)
  not a fake CArgVector; local CThing/CTCGraphicAppearance/CIntelligentPointer/CScreenEffectPriorityStack stand-ins must become shared
  headers.
- Loop form is right: two `for (int i=0; i<(int)args.size(); ++i)` loops indexing `args[i]` (size recomputed each test; elements
  re-indexed, no cached element reference in loop 2's third GetPItem / bypassSet read).

