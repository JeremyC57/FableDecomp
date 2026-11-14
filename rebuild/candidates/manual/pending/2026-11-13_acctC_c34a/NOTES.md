- c6: named physics local + C3DVector temp in Init args => q RELOCATION_MATCH (up-store residue closed); remaining: call target site 2
- k1: site 2 = named local 'CCharString secondary_name = def->SecondaryDummyPosName.GetString();' in an inner block with 'bool found' => RELOCATION_MATCH with right call targets (GetString 0x415DD0). default CDefString header. FITTED shape (block+bool), plain C++. Probes: forced op always expands GetString at site2 (any nesting); plain op declined at f<=.25; inline_depth(1) no help; GetString-defined-after-class opt-in no effect (REMOVE from hdr).

## 0x006A1A00 CTCCarrying::GetCarriedThingInterpolatedPosAndOrientation -- RELOCATION_MATCH (vland PASS, test PASS), ~32 grades (C34)
src/006a1a00.cpp (= C32 a8 + two changes). Headers: C32's overlay opt-ins (CVECTORMAP_VALFIND_CONST, CTCCARRYING_INTERPOLATEDPOS, new
CCarrySlotDef.h); default CDefString.h (C32's unused FORCEINLINE_CONVERSION_INLINE_GETSTRING opt-in DROPPED; hdr/engine/CDefString.h removed).
(1) up-vector stores (0x006A1B73): `const CTCPhysicsBase* physics = GetParentThing().GetPhysicsTC(); orientation.Init(C3DVector(0,0,1),
physics->GetFacingVector(time));` -- named physics pointer for the facing call + up as a TEMPORARY in the call (EVIDENCED by the bytes;
named up or no physics local both fail; a physics local shared with the GetPosition call fails).
(2) second string site: `bool found; { CCharString secondary_name = def->SecondaryDummyPosName.GetString(); found = ...GetDummyObject(
secondary_name, ...); } if (found)` -- explicit GetString is CALLED 0x00415DD0 there (static frequency <= .25), the named local builds it
before the pushes, the inner block destroys it before the test. FITTED shape (plain C++, no device). Probes: a FORCEINLINE conversion
always expands GetString inside it (any nesting, inline_depth(1) too); a plain conversion is declined whole (??B) at f <= .25 and expanded
whole at .5 -- there is no setting where the forwarder is expanded and GetString called, so the retail site is NOT the conversion.
vland warning only: RELOC_NAME ?DELTA_NOTIONAL_ZERO@@3MB v label g_NormaliseEpsilon (0x0129BA3C; shared C3DVector.h name, not this row's).
Descriptive name: g_bTurnPrimarySlotCarriedThing 0x0137574C (C32).

## 0x007FC9D0 CTCKraken::FrameUpdate -- RELOCATION_MATCH (vland PASS, test PASS), 5 grades (C34)
src/007fc9d0.cpp = C32 k8 + (a) the three empty helpers moved to hdr/engine/CTCKraken.h FABLE_RETAIL_CTCKRAKEN_EMPTY_UPDATES (inline empty
bodies after the class assert; no assert moved; coordinator decision) and (b) the frame closer: `{ CCharString effect("ON_DIE");
CreateHeadEffect(effect); }` -- a NAMED string local in its own block packs into the slot of the IP temporaries / GetTC key (frame 0x14) and
is destroyed before SetPhase(3). A temporary argument (with or without braces, GetTC in its own block or not) keeps its own slot (0x18).
- 007bc38a: n1 (880v880, 3 hunks): sibling 007bc6fa pragmas; sin((double)THROW_ANGLE)/cos inline at each use; SqrD(double) probe helper for sin^2 (need natural name); 'C3DVector offset = from_shooter; offset.Normalise(); target = GetPos() + offset * 5.0f' fixes slot order; NORMALISE_LOCALS + MAGNITUDE2D_ONE_EXPRESSION + NORMALISE_SQUARE_HELPER

## 0x007BC38A NThingShot::CTCShotDefinitionThrowPredictive::InitMovement -- DIFFER(880v880, 1 hunk = 2 instructions swapped), parked, ~30 grades
Name: Ego_r 0x007BB1D9 ?InitMovement@CTCShotDefinitionThrowPredictive@NThingShot@@MAEXXZ + vtable 0x0123CE9C slot 33 (C30). Extent 0x007BC38A..0x007BC6FA
(ret at 0x007BC6F9; sibling ProjectilePredictive::InitMovement 0x007BC6FA follows). differ/007bc38a.cpp (= tmp/n1.cpp).
New header hdr/engine/NThingShot_CTCShotDefinitionThrowPredictive.h (Throw : Base, ThrowPredictive : Throw; ctor 0x004D54B2 calls Throw's 0x004D545A).
TU idiom from landed sibling 0x007BC6FA: `#pragma optimize("y", off)` + `("s", on)` + intrinsic(sqrt) (here + sin, cos).
Decoded: target = GetShootAtPos(); if shooter alive: from_shooter = target - shooter pos; if GetMagnitude2D() < 5.0f: offset = from_shooter
normalised (fast Normalise, sum z*z+x*x+y*y = NORMALISE_LOCALS) ; target = shooter pos + offset * 5; TargetPos = target; else if target
thing alive: the sibling's lead code (motion * time * accuracy%) ; TargetPos = target. Tail: direction = target - parent pos; height = Z;
Z = 0; distance = GetMagnitude2D(); direction /= distance; q = ((distance / sin A) * cos A - height) * (2 * sin^2 A) / (distance^2 * 0.12f)
(A = (double)(float)(pi/4) 0x01261E50); physics->EnableGravity(true) (vslot +0x98); if q > DELTA_NOTIONAL_ZERO: speed = 1/sqrt(q);
physics->SetVelocity 0x0066F9B0 ((direction * sin A + (0,0,1) * cos A) * speed).
Closers: MAGNITUDE2D_ONE_EXPRESSION + NORMALISE_LOCALS (first block exact); `sin((double)THROW_ANGLE)` written at every use (double CSE
temps spilled as qword; the float overload gives dword spills; named doubles lose retail's `fld st(0)` copy); the sin^2 through a by-value
DOUBLE parameter helper (probe SqrD; GFSqr(float) makes VC7.1 CSE the float conversion and spill it as a dword, retail reloads the qword);
frame order (target above the from_shooter slot group) = packer weight: from_shooter must have fewer IL references than target (15):
`C3DVector offset = from_shooter; offset.Normalise(); target = pos + offset * 5.0f;` (w12) v in-place Normalise + `*=` (w18, slots swapped).
Residue: `mov eax,[esi]` (EnableGravity vtable load) one FP instruction early. Tried (no change or worse): physics declared before the
formula / before height / at top (loads hoist to block top in ecx), GetParentThing(), split formula (drop / range / numerator locals),
const-ref pos + explicit components, named sin/cos doubles. OPEN: the natural spelling of the double squaring (no FableWin symbol found:
only ?GFSqr@@YIMM@Z) -- maybe the whole speed formula is an inline helper with by-value double parameters; that may also fix the schedule.

## 0x006B2CA0 CEnvironment::UpdateSunAndMoonPositions -- NOT AUTHORED (context kept for rows 5-7); outline only
Name: Ego_r 0x006EE710 / FableWin 0x0244D960 ?UpdateSunAndMoonPositions@CEnvironment@@AAEXXZ (private, void()). Extent 0x006B2CA0..0x006B302E (ret).
Outline (from the memory operands): `if (ForcedNonColourValues +0x3D) return;` three angles * a double deg->rad constant 0x0124F2B8 (DayTime
+0xC0C; theme floats +0x1A8 negated, +0x1AC), fcos/fsin pairs stored as floats, products with 0.0f 0x0122DEDC (rotation composition with
constant zero components -- NOT factorised by the compiler, so the source has `(a + b) * 0.0f`-like inline helper bodies), + theme float
+0x1B0, AccurateNormalise 0x00A14440 -> sun; the same with +0x1B4 / +0x1B8 / +0x1BC -> moon; stores into InterpolatedParams.SunVector
+0xC30 / MoonVector +0xC3C; a blend by the theme float +0x19C (and 1 - it), AccurateNormalise; DiffuseClampAngle +0x40 * deg->rad, an
fcom branch (0x006B2FBF) with a second blend + AccurateNormalise; final 12-byte store through esi. 205 FP instructions, frame 0x84.

## 0x0074AF50 CTCCreatureGenerator::CreateCreaturesUndead -- NOT AUTHORED; FULL DECODE (C34) for the next agent
Name: Ego_r 0x0075DD00 + FableWin ?CreateCreaturesUndead@CTCCreatureGenerator@@IAEXPAVCThingCreatureBase@@@Z (protected, `ret 4` matches) -- (FableWin hit confirmed;
still needed (FableWin grep not run). Extent 0x0074AF50..0x0074B2A8 (ret 4 at 0x0074B2A5). No EH frame, frame 0x158.
Landed same-class rows (UpdateUndead 0x0074ACE0 etc.) use LOCAL models, so the shared CTCCreatureGenerator.h needs a typed opt-in:
CreatureFamilies +0x0C as a 12-byte STLport vector<long>, CreatedCreatures +0x18 list<CIntelligentPointer<CThing> > (push_back 0x004BA4E0),
a CCharString at +0x48 (inside GenParams +0x28), CreationCount +0x84, UndeadTriggered +0x88, PExclusionArea +0xA4 (CTCShapeManager*).
New headers needed: CTCGeneratedCreature (class name string 0x0123AC80; method 0x0074D0D0(CThing*) landed as unattributed AssignTarget),
CCreatureActionUndeadIdleUnderground (ctor 0x0074DD20(CThingCreatureBase*), base dtor 0x0062E900, ~0x118 bytes at [esp+0x54]).
Body:
  if (CreatureFamilies.size() == 0 || HasHitCreationLimit() 0x0074A190) return;
  UndeadTriggered = true;
  index = random(PeekWorld() 0x00686A80 .GetWorldSeed() 0x0049D810: *seed = *seed * 0x24A1 + 0x24DF; *seed = GFROR13(*seed) 0x00497890;
          n ? value % n : 0) with n = CreatureFamilies.size();
  CDefPointer<const CCreatureGenerationFamilyDef> family ([esp+18h]);  CreatureFamilies.at(index) EXPANDED (puts("vector") 0x00BFEAF2 + abort
  0x00BFEAEC); GetDefinitionManager() 0x006869D0 .GetDef(id, &family) 0x0074D220 (result unused);
  for (it = family->[+0x3C]; it != family->[+0x40]; ++it) {            // vector<long> of creature def ids
     CDefPointer<const CThingCreatureDef> thing_def;  if (ParentThing->GetDefinitionManager() 0x004C7990 ->GetDef(*it, &thing_def) 0x00513160) {
        CDefPointer<const CCreatureDef> creature_def;  if (thing_def->GetSubDef(&creature_def) 0x00485815 &&
            PeekWorld().IsCreatureGenerationEnabledForGroup(creature_def->[+0xAC]) 0x0049D910) {
           C3DVector pos ([esp+24h]);
           if (triggerer /*param*/) { if (!GetUndeadGenerationPoint(*triggerer, pos) 0x0074A260) return; }          // releases both defs + family
           else { pos = ParentThing->GetPos() 0x004C73D0; if (PExclusionArea && PExclusionArea->IsPosInAnyShape(pos) 0x00760870) return; }
           CThingAICreatureInit init(0, 0, false, <CCharString member +0x48>, CCharString("") 0x0122D70E, true) 0x004FACD0 ([esp+38h]);
           CThingAICreature* creature = CThingAICreature::Create(*it, pos, (long)(signed char)ParentThing->[+0x90], &init) 0x00833800 (fastcall);
           if (!creature || creature->IsDead (+0x91 bit 0)) return;                                                   // ~init 0x004FAD50 + releases
           CTCGeneratedCreature* tc = creature->AddTC(CCharString("CTCGeneratedCreature") 0x0123AC80, false, 0) 0x004C9D60;
           tc->0x0074D0D0(ParentThing);
           if (IS_THING_ALIVE(triggerer) 0x005C05C0) creature->SetCurrentAction(CCreatureActionUndeadIdleUnderground(creature) 0x0074DD20) 0x006644F0;
           CreatedCreatures.push_back(CIntelligentPointer<CThing>(creature) 0x004A96F0) 0x004BA4E0;
           ++g_GeneratedCreatureCount 0x013BAD54; ++CreationCount;
           if (HasHitCreationLimit()) return;                                                                         // 0x0074B253 path
        }
     }
  }
Loop reads family->[+0x40] again each iteration; `xor ebp,ebp` zero register reused for the per-iteration def pointer resets.

## 0x0075D580 / 0x00C271F0 -- not started (see prev_C32/NOTES.md "Untouched rows" and prev_style/NOTES_C30.md for what is established)
- 0x007BC38A update: the fitted SqrD helper is GONE: `double sin_squared = sin((double)THROW_ANGLE); sin_squared *= sin_squared;` gives the
  same bytes (retail's `fld st(0)` copy) in plain C++; differ/007bc38a.cpp uses it. Remaining residue unchanged (1 hunk, vtable-load schedule).
  Also tried: SqrD * 2 operand order, physics as a reference, the formula as an inline helper with by-value params (887, worse),
  `float sin_angle` variable (frame reshuffle, 49 hunks).
