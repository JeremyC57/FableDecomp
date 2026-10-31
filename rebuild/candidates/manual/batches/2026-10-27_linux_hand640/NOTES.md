# Hand chunk 640 (CWorldMap .. CHeteroTerrainFractal): 2 of 10 byte-exact

Chunk 640 is the 10-row parked-retry chunk (CHUNK_BRIEF.md). Every row started from its recorded best attempt, which I
regraded first (all reproduced the recorded residue). Grades were tagged `QTAG=a640*`.

## What matched (2)
- **C3DKeyframe** 0x00AA3AF0 `C3DKeyframe::ToMatrix(CMatrix4x4&, bool) const` (catalog "Quaternion_ToMatrix4x4"),
  RELOCATION_MATCH. This is the keyframe quaternion plus position as a 4x4 matrix (the transposed rotation, the position
  column, and a 0,0,0,1 last row).
  - Lever: every element is written through the FableWin inline accessor `??RCMatrix4x4@@QAEAAMHH@Z`
    (`float& operator()(int row, int col)` over a `float[4][4]` view of the union).
  - With named members E11..E44, VC7.1 hoists the six integer stores (E34, the zero row, E44) to just after the E11 store.
    Through the returned `float&` they stay after E33, as in retail.
  - The `M[4][4]` member name of the union's second view is not in struct_layouts (the tsv lists only the E11..E44 struct),
    so it is descriptive.
- **NScript::CQ_TentacleKrakenBossFightScript::CM_Experience** 0x00DF48F0 `Main()` (private virtual, Ego_r EAEXXZ),
  RELOCATION_MATCH. The script sets a CIsThingConscious(Me) predicate, waits for the master data's MotherDodgingKraken flag,
  waits while "TheKraken" is alive, then creates an experience orb at Me + (0,0,5) worth pGlobalScriptDef's
  PrisonExperienceForKillingKraken.
  - Lever: the shared FABLE_RETAIL_NSCRIPT headers: engine/CEntityScriptBase.h, CScriptThing.h, CGameScriptInterface.h and
    CIsThingConscious.h (: CPredicateBase). With them VC7.1 keeps &Me in edi, as retail does. The old local models used ebx.
  - The predicate ctor and dtor are defined inline after the include. `CScriptThing::IsAlive` is defined inline (the
    0x004AB130 body). `~CScriptThing` must stay out of line.
  - The script name converts implicitly (`GetThingWithScriptName("TheKraken")`). An explicit `CCharString(...)` temporary
    loads the vtable before building the string.

## Parked for Fable 5.1 (8)
- 004fec80 CWorldMap::UpdateDestroyedThemeAreas DIFFER(337v337): count/index register swap, a colouring tie (retail count=ebx, index=ebp, ours the reverse, with the matching push order) | tried (this session): PDB respell (FableWin C2DBoxI ctors and copy ctor, GetWidth/GetHeight, STLport clear), separate first/pos locals (321), pos declared before count (same), extended box copied before pos (308) | next: lane gap: register colouring tie (6 forms here, 4 in the sweep)
- 00aa3af0 -- matched (above).
- 00697cc0 CGameCameraManager::UpdateRightStickLookAround DIFFER(346v346): in the side vector's Y term, the first Rotate call's `lea ecx,[esp+14h]`/`push ecx` come one x87 instruction late | tried: PDB respell (private AAE, C3DVector::Cross, AccurateNormalise, NCameraModes::CBase::IsUseRightStickLookAround), CCamera::PeekViewDirection/PeekUpDirection accessors (same), CRightHandedSet local copy (worse: copy order), inline CRightHandedSet::Rotate/Normalise (worse) | next: lane gap: scheduling window (the sweep's 25 flag combos and 3 local-order shapes also failed)
- 00c1d1c0 CMatrix3x4::SetInverse DIFFER(347v347): x87 operand order of 7 commutative fmul pairs (det E23*E12 and E32*E23, cofactors E11*E23, E11*E22/E21*E12, E23*E12, translation E32*m.E43 and E13*m.E41) | tried: inline GetDeterminant() (det identical), `this` members (translation worse), `CMatrix3x4& r = *this` (back to the 7); probes show VC7.1 picks the fld operand from the function context, not the source order (an isolated `a*b` and `b*a` give the same code) | next: lane gap: x87 canonical operand order (park fast, agent634/635)
- 00df48f0 -- matched (above).
- 00986ed0 CThingFilter_IsAICreatureMatchingTypeAndFactionNearby ctor DIFFER(353v353): x87 operand order of three cross-product fmuls | tried: inline CRightHandedSet::GetRight() through the landed CrossProduct helper and through C3DVector::Cross (both worse: 4 pairs) | next: lane gap: x87 canonical operand order (16 sweep grades + 2)
- 00be1c10 CEngineInternalPrimitive2DViewport::Update DIFFER(353v353): retail loads the factor X/Y from the C4DVector temporary and starts the Z term before storing them; VC7.1 finishes Z first | tried: a named default-constructed C4DVector with four member stores (same) | next: lane gap: scheduling (with the sweep's user copy-assign, user copy ctor, body-assign ctor)
- 00a22e10 CRightHandedSet::MakePerpendicular DIFFER(358v358): one `mov esi,17Ch` scheduled after `fmul st(4)` (retail before it) | tried: PDB inline GFFastOneOverSqrt + `C3DVector& Normalise()` (same) | next: lane gap: scheduling
- 00688d40 CInputProcessZTarget::AddToInterface DIFFER(362v360): this in edi and the def temporary's pointer in ebp; retail has this=ebp, def=edi (the two `[ebp]` derefs are the 2 bytes) | tried: shared engine/CDefPointer.h (this=ebp but &PCreature=edi, 358v360: its Assign reads other.Object once, retail twice), local CDefPointer re-reading other.Object (the differ: all else matches, including the shrink-wrapped def register and both release shapes), named player local (360v360, wrong call order), PCreature-> (same) | next: lane gap: register colouring tie
- 00f36a30 CHeteroTerrainFractal::GetFractalHeight DIFFER(361v361): in the octave loop, retail loads the counter before the first `fmul qword [edi]` | tried: QFE 13.10.4035 compiler (same), i++ inside the ExponentArray index (same) | next: lane gap: scheduling (with the sweep's inline +=, stepwise *=, split product, early ++i)

## Skipped
None. No row spans two functions.

## Label corrections
- 0x00AA3AF0: catalog `global::Quaternion_ToMatrix4x4` -> `C3DKeyframe::ToMatrix(CMatrix4x4&, bool) const`. Evidence:
  - FableWin.pdb has exactly two 4x4 conversions, and this is the out-of-line one;
  - the 48-byte keyframe layout is read through `this`;
  - the body is the 4x4 twin of the landed 3x4 C3DKeyframe::ToMatrix 0x00A52450.
  - No callers in the image.
- 0x00C1D1C0: catalog `C3DMeshObject::SetInversePositionMatrixFromPositionMatrix` (IBEXXZ, `this` only) ->
  `CMatrix3x4::SetInverse(const CMatrix3x4&)` (FableWin ?SetInverse@CMatrix3x4@@QAEXABV1@@Z). Evidence:
  - the function takes two matrix pointers, destination in ecx and source in edx (the LTCG convention);
  - it is called from two classes' code (the quadric update 0x00C1BF00 and CMatrix::UpdateInversePerspective 0x00C0B570);
  - the body inverts with translation.
  - Probable (the parked differ).
- 0x00697CC0: the bound spelling is QAE; Ego_r.pdb says private AAE (NAME_FIXES.tsv).
- 0x00663340: bound as `CDefPointer<CHeroCombatDef>`; the PDB returns `CDefPointer<const CHeroCombatDef>` (NAME_FIXES.tsv).
- 0x00A14440: one binding each spells it `?Normalise@C3DVector@@QAEAAV1@XZ` / `QAEXXZ`; the body and the Ego_r
  out-of-line set say AccurateNormalise (NAME_FIXES.tsv). Note: GOTCHAS chunks 160/181 list "0xA14440
  C3DVector::Normalise", which conflicts.

## Names and evidence (matched rows)
- 00aa3af0:
  - ?ToMatrix@C3DKeyframe@@QBEXAAVCMatrix4x4@@_N@Z: FableWin name, plus the landed 3x4 twin's layout.
  - The inline ??RCMatrix4x4@@QAEAAMHH@Z: FableWin.
  - Globals are float-pool constants (masked).
- 00df48f0:
  - Main / CM_Experience / CQ_TentacleKrakenBossFightScript: Ego_r.
  - SetPredicate / IsActiveThreadTerminating: engine header, Ego_r.
  - CIsThingConscious vtable 0x012C2FE8: engine header, RTTI-less predicate.
  - pGlobalScriptDef 0x0143E90C: the established bound spelling, 100+ landed uses. Ego_r calls it
    NGlobalScriptDefs::pGlobalScriptDef, a CDefPointer, so the lander may want the namespaced spelling.
  - PrisonExperienceForKillingKraken at retail +0xDE0: Ego_r field name, plus the landed CScriptDef ctor 0x00F28F90
    retail offset.
  - MotherDodgingKraken +0x84: Ego_r CQ_SunnyvaleMasterData +132.

## Header debt
- 00df48f0: CScriptDef and CQ_SunnyvaleMasterData are local partial models; no shared headers exist.
- 00688d40 (differ): engine/CDefPointer.h's `Assign(CDefPointeeBase*)` loads `other.Object` once. Retail 0x00688DB9 re-reads
  it after the release (`mov eax,[edi]` after `mov [ebp+30h],0`). The differ uses a local CDefPointer whose operator=
  reads `other.Object` directly. A header variant with `if (Object != other.Object) { Release(); Object = other.Object;
  ... }` would serve this site.
- 004fec80 (differ): local CWorldMap next to the flattened engine/CWorldMap.h, the same model as the landed 0x004FF920.
- 00697cc0 (differ): engine/CGameCameraManager.h offsets are right (+0xC4), but it flattens the vptr and Camera, so the
  differ uses a local model.
