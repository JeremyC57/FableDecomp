# Hand chunk 634 (PKCPlaybackTemplate .. CScrollingComponent): 1 of 10 byte-exact

Ten parked same-length rows from the 2026-10-30/31 retry sweep. Every best attempt was reproduced first (all ten
DIFFER with the recorded residue), then retested with new levers. Exploration beyond the 4-grade cap was done only as
parameter sweeps of one form family (`bin/sweep.py`, all variants graded in one pass) and is listed per row.

## What matched (1)
- **CEffectFilter** (libpk mixer DSP)
  - 0x00C74AB0 `CEffectFilter::SetLowPass(long, float, unsigned long)` RELOCATION_MATCH: the six-float second-order
    Butterworth low-pass (pass-through set for a zero cutoff; corner `reference * 2^((cutoff+300)/1200)` clamped to
    0.495 * rate; k = tan(pi f / rate)). Shape: the landed sibling SetHighPass 0x00C74BB0's lesson
    `frequency = pow(...); frequency *= reference;` as two statements schedules the unsigned sample rate's
    `mov eax,[esp+10h] ; test eax,eax` before `fmul [reference]` (the parked one-expression product put the fmul first).
    Test PASS under g++ and VC7.1 (pass-through, the analytic Butterworth terms, unity DC gain, the 0.495 clamp).

## Parked for Fable 5.1 (9)
- 00C14F90 PKCPlaybackTemplate::~PKCPlaybackTemplate DIFFER(248v248): `lea ebx,[esi+40h]` before the three flag-patch vtable stores (retail after the +0x90/+0x84 stores) | tried: the real member model (CPKList base + user-dtor holders, CFlagOverride with a compiler-generated dtor over an inline virtual base dtor; this fixed the parked +0x0c zero-store residue), the holders as a PKCTRList<T> template, the QFE-4035 compiler (identical) | next: lane gap: scheduling (one lea slot)
- 006AFAF0 CTCPhysicsBase::ImpulseRespondToCollisonWithImpassableObject DIFFER(249v249): `fld st0 ; fadd st2` / `fadd st1` (retail `fld st1 ; fadd st1` / `fxch ; fadd st1`) and Y*=r as `fxch ; fmul st1 ; fxch` (retail `fld st0 ; fmulp st2`) | tried: inline LengthSquaredXY() condition (fixed the dot spill), X-first and Y-first condition / normalise sums (z,y,x and x,y,z), the PDB reference signature | next: lane gap: x87 canonical operand order (see lesson 4)
- 0076E4D0 CTCWeaponTrail::GetLastWeaponTrailPoints DIFFER(253v253): push_back's in-place copy of the 24-byte pair is load/store paired (retail loads one ahead) | tried: implicit C3DVector copy (worse: block copies), copy ctor as body assignments, std::make_pair temporary | next: model the real STLport 4.6 _M_insert_overflow (the same-TU stand-in body may set the alias facts)
- 0076C8A0 CTCEnemy::CTCEnemy DIFFER(254v254): thing def in edi / enemy def in ebp (retail ebp / edi) | tried: the brief's real fix (engine/CDefPointer.h ENullDef model, CDefPointer<const T> locals, the real GetDef<T> / GetSubDef<CEnemyDef> bodies as same-TU noinline, PDB CThing& / CTCBase(CThing&) / protected const GetDefinitionManager/GetWorld): bytes identical to the parked stand-in form; enemy def declared first (same swap, zero store moves); named `found` bool (cmp al,bl) | next: lane gap: callee-saved colouring
- 00AC7200 QuadricOptimiserInternals::COptimisedPrimitive::MakeOrthogonal DIFFER(255v255): the dot's z term computed second | tried: a 20-form sweep (named products, += chains, operand swaps, parentheses); `float r = X*o.X + Y*o.Y; r += Z*o.Z;` gives retail's x,y,z order but loads n.Z first and changes the final scale (253v255) | next: lane gap: x87 canonical operand order
- 00C72D00 CAudioSource::BindSound DIFFER(256v256): IAT pointer in ebx / zero in ebp / result in ebp (retail ebp / ebx / edi, the SetSourceProperty result with its own epilogue) | tried: six return shapes (separate SetSourceProperty return, else-if chain, inverted test, named item, `return result` on the streamed path); a separate return folds the zero (263v256) | next: lane gap: callee-saved colouring
- 00946190 CAIStateGroup_PrepareMeal::Update DIFFER(259v259): LookAtPos argument slot pointer `mov ecx,esp` before the last fadd (retail `mov ecx,esi` there, slot in eax) | tried: PDB respelling (CAIStateGroupBase protected AIAction_* with the enums, MAEXXZ Update, PeekPos, const operator+) | next: build the vector through a by-value helper with a hidden result pointer
- 00A55F90 CMatrix3x4::SetFromVectorAndAngle DIFFER(259v259): xy / xz products `fld [X] ; fmul [Y|Z]` (retail Y|Z first) and the Position zeroing two slots early | tried: all 16 operand spellings (identical), squares after products, named component locals, `aa + (1-aa)c`, chained Position store | next: lane gap: x87 canonical operand order
- 00547050 NUISystem::CScrollingComponent::UpdatePosition DIFFER(261v261): `fld ParentPosition.X ; fadd RenderPosition.X` (retail RenderPosition.X first) | tried: 16 forms (raw float pointers on either operand, references, +=, temporary, moved velocity declaration); a pointer local flips the zoom multiply instead | next: lane gap: x87 canonical operand order

## Skipped
None (no row holds a second function).

## Names (evidence)
- 0x00C74AB0 CEffectFilter::SetLowPass: class from the landed sibling SetHighPass 0x00C74BB0 and the caller CEffectPart260::Configure
  0x00C783F0 (descriptive libpk, no PDB/RTTI); arguments from the body (fild of a long cutoff, float reference, unsigned rate);
  "LowPass" from the coefficients (symmetric numerator, Butterworth poles). The caller's ?SetShelf@CEffectFilter@@QAEXMMK@Z is wrong
  in both name and first type -> NAME_FIXES.tsv. Descriptive.
- 0x00C14F90: vtable 0x012B29F8's class. Ego_r CTCDiceGame is 24 bytes (this object is 0xA4); the layout matches
  PKCPlaybackTemplate::ApplyTo 0x00C14A80 (same TU). Four landed spellings exist for this class (CTCDiceGame ctor 0x00C14E40,
  CDataOwner_00c14a50 0x00C14A50, CSettingsProxy_012b29f8 0x00C14B50/0x00C14BC0, PKCPlaybackTemplate 0x00C14A80): the parked
  source uses PKCPlaybackTemplate and NAME_FIXES.tsv proposes it for the others (descriptive libpk; coordinator's call).
  Members keep the bound spellings: CDiceRuleHolderA/B (vtables 0x012B29B0/0x012B28D8), CPKList (0x012B1AB0), CPKListImpl
  (dtor 0x00C19250), CFlagOverride (ctor 0x00C2C6B0), CDiceGameState / CDiceController (the ctor's).
- 0x006AFAF0: Ego_r/FableWin `?ImpulseRespondToCollisonWithImpassableObject@CTCPhysicsBase@@IBEXABVCPhysicsCollideResponse@@ABV1@AAVC3DVector@@@Z`
  ("Collison" is the PDB spelling); the parked source had pointer parameters and a public non-const method.
- 0x0076C8A0: Ego_r ??0CTCEnemy@@QAE@AAVCThing@@@Z (parked source: CThing*); callee PDB spellings in NAME_FIXES.tsv.
- 0x00946190: the AIAction_* / Terminate callees are CAIStateGroupBase members in Ego_r (the parked source declared them on the
  derived class with long types and a public Update).
- 0x00C72D00: CAudioSource::BindSound stays descriptive (gap-scan stub; vtable slot 3 of the CAudioSource vtables, the class the
  landed 0x00C72A40/0x00C72800/0x00C72C40 rows use).

## Label corrections
- 0x00C14F90 "CTCDiceGame::~CTCDiceGame" (propagated) -> PKCPlaybackTemplate::~PKCPlaybackTemplate (descriptive libpk).
- 0x00C74AB0 "CAudioChannel_ComputeShelveFilterCoefficients" -> CEffectFilter::SetLowPass.
- 0x0076E4D0 "WeaponTrail_CollectPoints" -> CTCWeaponTrail::GetLastWeaponTrailPoints (Ego_r, private const overload).
- 0x00AC7200 "Vector_ProjectOntoPlane" -> QuadricOptimiserInternals::COptimisedPrimitive::MakeOrthogonal (Ego_r, donor order).
- 0x00A55F90 "Matrix_RotationAroundAxis" -> CMatrix3x4::SetFromVectorAndAngle (both PDBs).
- 0x00C72D00 "sub_00c72d00" -> CAudioSource::BindSound (descriptive).

## Header debt
- engine/CTCBase.h is flattened (vptr as `void*`): 0x0076C8A0 keeps a local polymorphic CTCBase (ctor, protected accessors).
- engine/CEnemyDef.h is flattened: 0x0076C8A0 (like the landed 0x006C7720) declares CEnemyDef : CSubDefClassBase locally with Faction.
