## 0x0075A7A0

# DOSSIER 0x0075A7A0 CTCPhysicsRigidBody::ResolveCollisions (726 bytes)

## 1. Retail shape (xdis 0075a7a0 0x2e6)
- __thiscall, `ret 4` (one ref param). NO frame pointer (`sub esp,0x7c` first; no push ebp/mov ebp,esp) -> default /Oy.
- No SEH prologue, no EH state. Frame 0x7c (draft got 0x74 -> retail has ~2 more 12-byte-ish locals / slots).
- Register homes: edi=this whole function; esi=&config; ebx=&config.AngularMomentum (+0x48); ebp=&config.AngularVelocity (+0x9c),
  all hoisted before loop. Loop counter i at [esp+14], count (size() of CollidingVertexIndex, computed ONCE, signed `sar 2`, `jle`/`jl`)
  at [esp+1c], strength-reduced byte offset of CollisionNormal[i] at [esp+10] (+=0xc each pass; begin [edi+40] re-read every use).
- Loop top aligned with `lea ebx,[ebx]` filler after `jmp` (normal VC7.1 loop alignment, /O2 not /Os).
- Body order (per iteration):
  a) vertex = config.BoundingVertices[CollidingVertexIndex[i]]: X,Y fld'd directly, Z moved as INT via ecx to [esp+58] (a named/copied
     C3DVector local; copy ctor -> integer copy of one component), then r = vertex - config.CMPosition stored [20/24/28].
  b) config.AngularVelocity ^ r (cross, components computed X,Y,Z: w.Y*r.Z-w.Z*r.Y ...), + config.CMVelocity -> velocity (X kept in [68], Y on st, Z [64]).
  c) dot(velocity, normal) summed Z then Y then X: (v.Z*n.Z + v.Y*n.Y) + v.X*n.X  -> times (CoefficientOfRestitution + 1.0f [0x0122DED8 = 1.0f literal]) then fchs -> numerator [esp+18].
     i.e. `-( dot * (CoR + 1.0f) )` with CoR+1 evaluated after dot (fld [edi+a4]; fadd 1.0; fmulp; fchs).
  d) r ^ normal computed Z-comp first, then Y, then X (right-to-left ctor args) stored [2c/30/34]; call 0xA56530 (InverseWorldInertiaTensor * that, ecx=esi+0x6c, hidden result [esp+74]).
  e) (result ^ r) dotted with normal (Z,Y,X order again) + Body.OneOverMass [edi+a0]; `fdivr [esp+18]` -> j = numerator/denominator.
  f) impulse = normal * j (X,Y,Z: fld st0; fmul n.X ...), then impulse*OneOverMass added to CMVelocity (+0x3c/40/44) via two stored temps [3c][40].
  g) AngularMomentum += r ^ impulse (via ebx; temps [44][48][4c?]); call 0xA56530 with &AngularMomentum (ebx) -> result [esp+84] copied
     dword-by-dword (mov edx) into AngularVelocity (ebp) => plain struct assignment of the returned C3DVector.
- Epilogue: CollisionNormal.clear() inlined (STLport erase(begin,end) -> __copy random-access loop: n=(finish-finish)/12 via imul 0x2AAAAAAB, element copy, dec/jne; _M_finish=result)
  then CollidingVertexIndex.clear() inlined (__copy_trivial: `cmp eax,eax` last==first -> _M_finish=first; else memmove 0x00BFEAE6). Two separate ret paths.

## 2. Name / signature (two sources)
- Ego_r.pdb: ghidra_out/egor_pdb_names.tsv 00769a70 `?ResolveCollisions@CTCPhysicsRigidBody@@AAEXAAVCRigidBodyConfig@CRigidBody@@@Z`
- FableWin.pdb: ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv (017c00b5 / 025c1cb0) same mangled name.
- Caller: Simulate 0x0075AD50 (call site 0x0075AE68, landed rebuild/src/compiled/00/75/CTCPhysicsRigidBody_Simulate_0075ad50.cpp, declares ResolveCollisions 0x0075A7A0).
- => `private: void __thiscall CTCPhysicsRigidBody::ResolveCollisions(class CRigidBody::CRigidBodyConfig &)`. Manifest label CORRECT.
  NOTE scoping: CRigidBodyConfig is NESTED in CRigidBody (`V...@CRigidBody@@`), a `class` (V), param non-const ref, PRIVATE (A in AAE).
  Draft declares `struct CRigidBodyConfig` at global scope and a public member -> wrong mangled leaf. Copy CalculateVertices' model
  (`struct CRigidBody { class CRigidBodyConfig { public: ... }; ... }`).
- Not virtual (no vtable slot).

## 3. Callees
| target | landed row | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x00A56530 (x2) | rebuild/src/compiled/00/a5/CMatrix3x4_operator_00a56530.cpp | CMatrix3x4::operator*(const C3DVector&) const, leaf ??DCMatrix3x4@@QBE?AVC3DVector@@ABV1@@Z | same (row Leaf) | `C3DVector operator*(const C3DVector&) const` OK; members m_row0.. local (CMatrix3x4.h uses E11..E43 and declares NO operator*; Simulate/CalculateVertices model CMatrix3x4 locally) |
| 0x00BFEAE6 | rebuild/src/compiled/00/bf/global_memmove_00bfeae6.cpp | memmove (CRT) | CRT | draft `#include <string.h>` memmove OK; comes from STLport __copy_trivial |
No other calls / no virtual calls. C3DVector ops (^, -, +, +=, *float, dot) are all expanded inline.

## 4. Members
this (CTCPhysicsRigidBody, header rebuild/include/engine/CTCPhysicsRigidBody.h, opt-in FABLE_RETAIL_CTCPHYSICSRIGIDBODY (+FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CTCPHYSICSBASE)):
- +0x40 CollisionNormal  std::vector<C3DVector> (PDB name; retail branch real std::vector from stlport/fable_containers.h, protected)
- +0x4c CollidingVertexIndex std::vector<long>
- +0xa0 Body (CRigidBody) -- header keeps it OPAQUE `unsigned char Body[0x1b8]`; no CRigidBody header exists anywhere.
  +0xa0 = Body.OneOverMass (float), +0xa4 = Body.CoefficientOfRestitution (float) (struct_layouts_egor.tsv CRigidBody +0/+4).
  Draft models these as direct members OneOverMass/CoefficientOfRestitution of CTCPhysicsRigidBody -> wrong names (should be Body.OneOverMass ...).
config (CRigidBody::CRigidBodyConfig, struct_layouts_egor.tsv, retail offsets = PDB offsets up to +0xa8): CMPosition +0 C3DVector, Orientation +0xc CMatrix3x4,
  CMVelocity +0x3c, AngularMomentum +0x48, CMForce +0x54, Torque +0x60, InverseWorldInertiaTensor +0x6c CMatrix3x4, AngularVelocity +0x9c,
  BoundingVertices +0xa8 vector<C3DVector> (retail size 0xb4).
- Shared header usable: engine/C3DVector.h (X,Y,Z). Ops available: operator- / + (const), operator*(float) (const), operator^ (cross, under FABLE_RETAIL_C3DVECTOR,
  variants FABLE_RETAIL_C3DVECTOR_CROSS_{XYZ,XY,X}_GROUP), Dot() with variants FABLE_RETAIL_C3DVECTOR_DOT_GROUPED / _DOT_ORDERED_YXZ / _ZYX / _XYZ.
  Retail dot order here is Z,Y,X both times -> try FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_ZYX first (or default one-line X+Y+Z which VC often evaluates in reverse).
  FableWin confirms C3DVector has ??T (operator^ cross), ??D(float), ??H, ??G, ??Y(+=), ?Dot@C3DVector@@QBE?BMABV1@@Z, and global ?GFDotProduct@@YIMABVC3DVector@@0@Z.
  No float*C3DVector global in FableWin -> draft's `operator*(float, const C3DVector&)` and CrossProduct/DotProduct free helpers are invented; use `normal * j`, `a ^ b`, `.Dot()`.
- Classes the draft models LOCALLY that have shared headers: C3DVector (engine/C3DVector.h), CMatrix3x4 (engine/CMatrix3x4.h, but it lacks operator* -> landed siblings keep a local CMatrix3x4), vector (stlport/fable_containers.h std::vector), CTCPhysicsRigidBody (engine/CTCPhysicsRigidBody.h).
  CRigidBody / CRigidBodyConfig: no shared header (local model is required; copy CalculateVertices').

## 5. Visibility
- CMatrix3x4::operator* 0xA56530 is far away (other TU, CMatrix3x4 maths); called out of line -> declaration only.
- STLport vector clear()/erase/__copy/__copy_trivial: inline expanded here (fable_containers.h default; do NOT define FABLE_RETAIL_STLP_VECTOR_CLEAR_OUTOFLINE / ERASE_RANGE_OUTOFLINE). size()/operator[] inline.
- Same TU neighbours (CTCPhysicsRigidBody.cpp): SetPhysicsMeshBankIndex 0x0075A530 before, ctor 0x0075AA80, CheckForCollisions 0x0075AAF0, Simulate 0x0075AD50 after;
  out-of-line STLport COMDATs for vector<C3DVector> (fill 0x0075B430, copy 0x0075B560, copy_backward 0x0075B5B0, _M_fill_insert 0x0075BB00, resize 0x0075C040)
  and CMatrix3x4 ops 0x0075B600/0x0075B6C0 exist in this TU but are NOT called here.

## 6. Sibling landed rows (rebuild/src/compiled/00/75/)
- CTCPhysicsRigidBody_CalculateVertices_00759ec0.cpp: best template -- local `struct CRigidBody { class CRigidBodyConfig {...}; ... }`, local CMatrix3x4 with operator* decl, local `template vector`, local CTCPhysicsRigidBody {_pad00[0xa0]; CRigidBody Body;}. No pragmas/flags. Says engine header Body opaque so local model used.
- CTCPhysicsRigidBody_Simulate_0075ad50.cpp: caller; local clear() helpers reproduce the same two inline clears (CopyVectors loop + CopyTrivial memmove) -- note it was byte-matched with these local forms.
- CTCPhysicsRigidBody_ComputeForces_00759d30.cpp: local CRigidBodyConfig, Body.OneOverMass as `float OneOverMass // +0xa0 (Body)`, inline C3DVector ops locally.
- CTCPhysicsRigidBody_GetFacingVector_00759b20.cpp: uses `#define FABLE_RETAIL_CTCBASE / FABLE_RETAIL_CTCPHYSICSBASE / FABLE_RETAIL_CTCPHYSICSRIGIDBODY` + engine/CTCPhysicsRigidBody.h.
- Only one CTCPhysicsRigidBody-family row carries `// Flags: /EHsc`; none use #pragma optimize. Default flags.

## 7. Draft model errors vs retail
- Wrong signature mangling: global struct CRigidBodyConfig and public method (should be CRigidBody::CRigidBodyConfig class, private).
- Body members flattened into this; fine for offsets but names wrong.
- Draft binds `const C3DVector& normal = CollisionNormal[i]` once; retail re-reads CollisionNormal._M_start ([edi+40]) at each use with a separate byte offset -> retail likely indexes CollisionNormal[i] at each use (not a cached reference), so the compiler strength-reduces i*12.
- Retail copies the vertex into a C3DVector (int-move of Z) before subtracting -> `C3DVector vertex = config.BoundingVertices[...]` or r computed from a by-value temporary; draft subtracts straight from the reference.
- Frame 0x7c vs draft 0x74: retail keeps more named locals (vertex copy [50..58], velocity, impulse, the two matrix-result temps [74],[84]).
- Dot order: retail Z,Y,X; draft DotProduct(a,b) a.Z*b.Z + a.Y*b.Y + a.X*b.X ok-ish but use header Dot variant.
- Numerator: retail `-(dot * (CoR + 1.0f))` (fld CoR; fadd 1.0; fmulp; fchs) -> draft matches.
- impulse*OneOverMass: retail computes impulse = normal*j kept on x87 then each component * OneOverMass (`fld OneOverMass; fld st2; fmul st1`), i.e. `impulse * Body.OneOverMass` (member operator*(float)), not `OneOverMass * impulse`.
- AngularVelocity = InverseWorldInertiaTensor * AngularMomentum: result copied via integer moves (struct copy of returned temp) -- matches assignment.
- Use std::vector clear() for both lists (header) instead of hand loops; equivalent bytes expected (Simulate proved the forms).

## 0x00854DD0 (not landed)

# DOSSIER 0x00854DD0 -- CCreatureAction_SpiritAttack::PerformAction (720 bytes)

## 1. Retail shape (xdis 00854dd0 0x2e0)
- __thiscall, plain `ret` (two duplicated epilogues 0x855085 / 0x855098, tail-duplicated OnStrike call), `ret` ends at 0x0085509F; 0x008550A0 is the next row.
- NO frame pointer (`sub esp,7Ch; push ebx/ebp/esi/edi`) -> default /Oy; no SEH prologue even though temporaries with dtors exist
  (CIntelligentPointer temp, CHitParameters) -> no EH state => build without /EHsc unwinding (like siblings, `// Flags: none`).
- Registers: edi = this (spilled to [esp+10h] for the CreateAttackEffect call), ebp = &PCreature (this+8, every GetPItem re-called,
  never cached), esi = target, ebx = 8 constant (GetTC(8) bit, then type-flag test `test [esi+6Ch],bl`), later ebx = CTCCoopSpirit*.
- Frame: [esp+14h] TC out-pointer / CIntelligentPointer temp (vptr 0x01238C6C) / float temps; CHitParameters at [esp+2Ch] (0x60 bytes).
- Normalise: fld X-diff, Y-diff, Z-diff; `fst [esp+28h]; fmul [esp+28h]` (Z spilled) then Y*Y, X*X => sum Z*Z+Y*Y+X*X;
  GFFastOneOverSqrt inline (0x17C, table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C), factor = ((3-r*r*sq)*r)*0.5.

## 2. Identity (manifest label CORRECT)
- vtable CCreatureAction_SpiritAttack 0x01275D74 slot 3 = 0x00854DD0 (rebuild/manifest/rtti-vtables.tsv); slot 3 = PerformAction in
  every CCreatureAction (SpiritReturnToHero 0x00854720 slot 3, SpiritHitResponse 0x008550B0 slot 3, both landed as PerformAction).
- Ego_r.pdb (egor_pdb_names.tsv 0x00844350) and FableWin.pdb (_fablewin_api.tsv 0x01826DEC):
  `?PerformAction@CCreatureAction_SpiritAttack@@UAEXXZ` -> `public: virtual void __thiscall PerformAction(void)`.
- Only xref: the vtable (.rdata 0x01275D80). No direct callers.

## 3. Callees (landed file / landed name / PDB name; DRAFT MISMATCHES flagged)
| addr | landed row | name to use | draft spelling |
|---|---|---|---|
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | CBaseIntelligentPointer::GetPItem() const | ok |
| 0x0040F020 | 00/40/CVectorMap_LowerBound_0040f020.cpp | CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound (via CThing::GetTC<T> forceinline) | draft CInterfaceMap::LowerBound -- WRONG, local |
| vslot 33 (+0x84) on TCI_TARGETING(8) TC | -- | CTCTargetingBase::GetPTargetedThing() const (CTCTargetingBase.h l.34/83; landed 0x00802610 CTCTargetingPlayer::GetPTargetedThing; 007f9b90 / 0064c869 cite slot 33) | draft CTCTargeting::GetTarget -- WRONG |
| 0x00A01B10 / 0x00A01B90 / 0x00A01C10 | CBaseIntelligentPointer ctor / SetPItem / dtor | temp `CIntelligentPointer<CThing>(target)` then `PTarget = temp` | ok in behaviour |
| 0x00670000 | 00/67/CTCPhysicsBase_GetAirResistance_00670000.cpp (MISLABELLED) | CTCCoopSpirit::GetMeleeTargetRange() const (`?GetMeleeTargetRange@CTCCoopSpirit@@QBEMXZ`, Ego_r+FableWin); body `mov eax,[ecx+28h]; fld [eax+38h]` = PCoopSpiritDef(+0x28)->MeleeTargetRange(+0x38, struct_layouts_egor CCoopSpiritDef) | draft GetRange -- WRONG; NAME_FIXES candidate for the landed row |
| 0x004C73D0 | 00/4c/CThing_GetPosition_004c73d0.cpp, Leaf ?GetPos@CThing@@QBEABVC3DVector@@XZ | CThing::GetPos() const, returns const C3DVector& | draft GetPosition returning pointer -- WRONG |
| 0x006A4DF0 | 00/6a/CThingPhysical_IsInRange_006a4df0.cpp | CThingPhysical::IsInRange(const C3DVector&, float) const (bool) | draft non-const pointer arg |
| 0x008548B0 | NOT landed (DIFFER in hand283) | CCreatureAction_SpiritAttack::CreateAttackEffect() (private, PDB AAEXXZ) | ok |
| 0x00789238 | 00/78/CHitParameters_CHitParameters_00789238.cpp | CHitParameters(NHitLocations::CHitLocation*, CThing*, CThing*, const C3DVector&, const C3DVector&, bool knockDown, bool blockable, bool flourish, bool blockCounter), ret 24h | draft `void* location`; arg values (0,creature,0,pos,dir,false,true,false,false) correct |
| 0x00868CA0 | 00/86/CHitParameters_AddDamageWithAttribute_00868ca0.cpp | CHitParameters::AddDamageWithAttribute(long/EDamageAttribute, float) | ok (attr 0, 1.0f) |
| 0x00789358 | 00/78/CHitParameters00789358_..._00789358.cpp | CHitParameters::~CHitParameters() (CHitParameters.h FABLE_RETAIL_CHITPARAMETERS_CTOR declares it) | ok |
| vslot 65 (+0x104) on target | -- | CThingGameObject::OnHit(CHitParameters&) (see 00798770 FinishBeingForcePushed `owner->OnHit(hit)` slot 65) | draft CThing::Hit -- WRONG name |
| 0x0066FB10 | 00/66/CTCCoopSpirit_AddExperience_0066fb10.cpp | CTCCoopSpirit::AddExperience(long) (landed; PDB also has AddScore(long); body adds to +0x10 which Ego_r names `Score` -- ambiguous, keep landed) | ok |
| 0x006707E0 | 00/67/CTCCoopSpirit_OnStrike_006707e0.cpp | CTCCoopSpirit::OnStrike() (reads OnStrikeParticle +0x2c; PDB has both OnHit and OnStrike) | draft OnHit -- WRONG |

## 4. Members
- this+0x08 PCreature: CCreatureActionBase, CIntelligentPointer<CThingCreatureBase> -- engine/CCreatureActionBase.h opt-in
  FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE (+ FABLE_RETAIL_CINTELLIGENTPOINTER first).
- this+0xA8 PTarget (CIntelligentPointer<CThing>), +0xB0 PParticleEmitter: engine/CCreatureAction_SpiritAttack.h (opaque bytes,
  offsets asserted); FrameUpdate 00854b90 redeclares `class CCreatureAction_SpiritAttack : public CCreatureActionBase { CIntelligentPointer<CThing> PTarget, PParticleEmitter; }`.
- thing+0x20 UsingComponents, +0x44 Components (CThing.h, GetTC<T> FABLE_FORCEINLINE; FABLE_RETAIL_CTHING), bit 8 -> TCI_TARGETING, bit 0xC7 -> CTCCoopSpirit.
- thing+0x60 PhysicsTC (CThing.h, GetPhysicsTC() const) -> CTCPhysicsBase::Radius +0x34 (GetRadius(), CTCPhysicsBase.h; FABLE_RETAIL_CTCPHYSICSBASE).
- thing+0x6C ThingTypeFlags, bit 8 = creature bit (CThingCreatureBase mask 15 vs CThingGameObject 7): `GetTypeFlags() & 8` (CThing.h l.423),
  same idiom as landed 008c0c00 / 008c48a0 / 008c92c0.
- thing+0x91 bit0 Dead: `IsDead()` (CThing.h l.280).
- CHitParameters (CHitParameters.h, 0x60): Blocked +0x26 / HitNegated +0x27 -> IsBlocked() / IsHitNegated() (draft "Deflected" wrong).
  The 9-arg ctor and AddDamageWithAttribute are NOT declared in the shared header (no opt-in); landed 00798770 declares them locally.
- CTCCoopSpirit: engine/_quarantine/CTCCoopSpirit.h (fields only: PCoopSpiritDef +0x28, Score +0x10); no method opt-ins -> declare GetMeleeTargetRange/AddExperience/OnStrike + StatGetInterfaceType 0xC7 (FrameUpdate 00854b90 idiom).
- Draft models LOCALLY although shared headers exist: C3DVector, CThing, CTCPhysics(Base), CHitParameters, CBaseIntelligentPointer/CIntelligentPointer,
  CCreatureAction_SpiritAttack, CInterfaceMap (=CVectorMap), CTCTargeting (=CTCTargetingBase).

## 5. Visibility
- GetTC<T>, IsDead, GetTypeFlags, GetPhysicsTC, GetRadius, C3DVector ops, Normalise, GFFastOneOverSqrt: inline (headers).
- Out of line (not visible): GetPItem/SetPItem/ctor/dtor (0xA01Bxx), LowerBound 0x0040F020, GetPos, IsInRange, GetMeleeTargetRange (own TU 0x0067xxxx),
  CHitParameters ctor/dtor/AddDamage, AddExperience, OnStrike.
- Same TU, earlier: CreateAttackEffect 0x008548B0 (address < row; called, not inlined -- fine as an extern member decl).
- C3DVector opt-ins: FABLE_RETAIL_C3DVECTOR + FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_ZYX gives `(Z*Z+Y*Y+X*X)` with the default Normalise
  (= retail's `fst; fmul` Z spill pattern, cited at 0x00A587B2 in the header). Newton variants: GFFASTONEOVERSQRT_NEWTON_{GROUPED,SQUARE_PAREN,STEP_PAREN,PAREN}
  (no-code paren tuples that move the scheduling cut -- the residue here IS an x87 schedule/register-role issue, try these).
  operator-: default or FABLE_RETAIL_C3DVECTOR_SUBTRACT_XY_GROUP.

## 6. Siblings
- 00/85/CCreatureActionSpiritReturnToHero_PerformAction_00854720.cpp (`// Flags: none`, FABLE_RETAIL_BASE_OBJECT, _CCREATUREACTIONBASE,
  _CCREATUREACTIONBASE_PCREATURE, _CTHING, _CINTELLIGENTPOINTER, _CTHINGPHYSICAL, _CTHINGGAMEOBJECT, _CTHINGCREATUREBASE, _CTCPHYSICSBASE;
  `PCreature->GetPos()`, `PCreature->GetPhysicsTC()`).
- 00/85/CCreatureActionSpiritAttack_FrameUpdate_00854b90.cpp (same class; GetTC with `if(i==map.end()||type<i->first) i=map.end()`,
  CTCCoopSpirit wrapper over _quarantine header with StatGetInterfaceType 0xC7).
- 00/79/CTCForcePushed_FinishBeingForcePushed_00798770.cpp (same CHitParameters 9-arg ctor + AddDamageWithAttribute + OnHit slot 65).
- 00/85/CCreatureActionSpiritAttack_FinishAction_008547d0.cpp, _Initialise_008545f0.cpp, _Clone_00855230.cpp.

## 7. Old draft model errors
- Names: GetPosition->GetPos (reference), GetRange->GetMeleeTargetRange, OnHit->OnStrike, Hit->OnHit, GetTarget->GetPTargetedThing,
  Deflected->HitNegated, m_flags6c->ThingTypeFlags/GetTypeFlags()&8, m_flags91->Dead/IsDead().
- Local stand-ins everywhere (verifier warns / strict rejects); FILL10 vtable padding.
- Normalise sum order (Z,Y,X) written by hand; use header ZYX opt-in. The recorded residue (702v720) is the x87 schedule (retail keeps
  X/Y on the stack, only Z spilled) and edi(this)/ebp(&PCreature) swap -- try PCreature->GetPos() through the header's CIntelligentPointer
  `operator->` (Get() -> GetPItem) rather than a GetCreature() helper, and the Newton paren variants.
- Control flow is right: GetTC(8) unconditionally used (uninitialised out-ptr if absent), `PTarget = CIntelligentPointer<CThing>(target)`,
  `if(target && !target->IsDead() && (target->GetTypeFlags()&8))`, IsInRange(PCreature->GetPos(), range + target->GetPhysicsTC()->GetRadius()),
  `if(!hit.IsBlocked() && !hit.IsHitNegated()) spirit->AddExperience(1)`, final fresh GetTC(0xC7)->OnStrike().

