# agentC36 NOTES

## 0x0074AF50 CTCCreatureGenerator::CreateCreaturesUndead (in progress)
- Draft tmp/a1.cpp on shared headers (opt-ins in hdr/README.md). Facts measured:
  * retail calls PeekWorld 0x00686A80 right after HasHitCreationLimit WITHOUT reloading ecx (also in CreateCreatures 0x0074B327): only a
    non-inline DEFINITION of HasHitCreationLimit earlier in the TU reproduces it (an `inline __declspec(noinline)` body does not) ->
    header opt-in FABLE_RETAIL_CTCCREATUREGENERATOR_HASHITCREATIONLIMIT_DEFINITION (decision for the coordinator: a row TU that emits a 2nd function).
  * ~CThingAICreatureInit is out of line (0x004FAD50) -> FABLE_RETAIL_CTHINGAICREATUREINIT_DTOR decl; vector::at expanded -> FABLE_RETAIL_STLP_VECTOR_AT_INLINE.
  * `size() <= 0` gives retail's `sar/test/jbe`.
  * family pointer: retail never keeps it in a register (reloads [esp+18h]) and keeps a zero register ebp; a probe with the family GetDef
    hidden (escape) gives retail's structure, BUT retail CreateCreatures keeps its family in ebp across calls and earlier-instantiation /
    explicit-specialisation probes (p2..p4) change nothing: it is an allocation matter, not visibility. Explicit spec decl = ICE.
- PARKED after 27 grades: differ/0074af50.cpp DIFFER(932v856). Root cause (single): family.Object is CSE'd into ebp from the loop init to the
  loop bottom in our build; retail never caches it (0x0074AFF8 eax, 0x0074B223 ecx, 0x0074B28D ecx) and uses ebp as a zero register
  (`xor ebp,ebp ; xor edx,edx ; cmp edi,ebp` 0x0074AFB2; `xor ebp,ebp ; cmp edi,ebp` 0x0074B211). Retail's family is NOT escaped (short
  fused release without null test, as ours) -- a probe that hides the family GetDef (p1) gets the zero register but the long unfused release.
  Tried without effect: family declared before the random (worse, 39 hunks), nested expression, GFRANDOM_RESULT_LOCAL, unsigned index,
  positive-if wrapper, (*family) / it++, named const CDefIndex&, inline wrapper probes, earlier instantiation of GetDef<Family> (+40 filler
  functions), `continue` form (919, other shape), index loop (878, other shape). Slot order also differs: retail family [esp+18h],
  thing_def [esp+10h], creature_def [esp+14h]; ours 10h/14h/18h -- family is allocated LAST in retail (fewest references / inner scope?):
  the next idea is whatever makes family the lowest-weight handle (e.g. the loop bounds read through a helper of the def, or the
  family handle living inside an inlined helper's scope).
- Names: SetGenerator 0x0074D0D0 = FableWin ?SetGenerator@CTCGeneratedCreature@@QAEXABVCThing@@@Z (+ Ego_r layout PGenerator +0xC; landed as
  unattributed AssignTarget); CCreatureAction_UndeadIdleUnderground (PDB spelling with underscore; ctor takes CThingCreatureBase&);
  IsCreatureGenerationEnabledForGroup 0x0049D910 (Ego_r + FableWin); ~CThingAICreatureInit 0x004FAD50 (FableWin).

## 0x007BC38A NThingShot::CTCShotDefinitionThrowPredictive::InitMovement (near-miss, still DIFFER(880v880), 1 instruction)
- differ/007bc38a.cpp = C34's draft minus the stale FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER define (same grade without it).
- Residue: retail `mov eax,[ebx+4] ; mov esi,[eax+60h] ; fsubp ; mov eax,[esi] ; mov ecx,esi` (0x007BC647..0x007BC651); ours has the vtable
  load before the fsubp (int run of 3 then 1, retail 2 + 2). Measured (5 grades): declaring `physics` before the formula / before sin_squared /
  before `height` makes the ParentThing load use ecx and hoists it to 0x007BC5ED (5 hunks) -- so the pointer statement IS after the formula's
  last use of the GetPos result (`fsub [eax+8]`, 0x007BC644), as in the draft. The difference is the scheduler's choice between the vtable load
  and the fsubp only. Not tried: the formula split in two statements at the `- height`, EnableGravity through an inline forwarder.

## 0x006823D0 CTCCreatureOpinionOfHero::FrameUpdate (near-miss; 8 grades by C36; still DIFFER)
- Frame fact EXPLAINED: retail has four GFFloatAbs parameter homes ([esp+10h] first test, [esp+18h] peak, [esp+14h] RunOut, [esp+1Ch] frames) and
  NO peak slot: the peak's `fld [esp+18h]` (0x00682558) is the helper's return load deferred to the single use. In our build a named
  `float peak = GFFloatAbs(..)` gets its own slot and c2 forwards the masked value into it (frame +4).
- Three spellings give retail's frame, all 962v962 / the SAME 20 register-rotation hunks: (a) C27's pointer read-back GFFloatAbs text (not FableWin's),
  (b) tmp/o6.cpp `float peak = value; *(unsigned long*)&peak &= 0x7fffffff;` (the helper hand-inlined: fitted), (c) tmp/o7.cpp the peak passed BY VALUE to
  a FABLE_FORCEINLINE helper holding the `if (GFFloatAbs(RunOut) > 0.0001f) {...}` block (a plain `inline` helper is called out of line: 864 bytes).
  No such helper is named in either PDB (COpinionTransientOffset has no methods) -> FITTED; the natural spelling is still unknown.
  No effect: peak in its own block, GFFloatAbs returning through the compound-assignment lvalue (`return *(float*)&(*(unsigned long*)&f &= ..)`), cast chains.
- Remaining 20 hunks with the frame right (o7): `push 1` v `mov ecx` order 0x006824A4, clamp address temp ecx v edx/edi (0x0068258D: retail
  `mov ecx,[ebp+48h] ; fld [ecx+edi] ; add ecx,edi`, ours `mov edx,.. ; add edi,edx`), run-out block 0x006825DE, later blocks.
- **MATCHED (30 grades): src/0074af50.cpp RELOCATION_MATCH, test PASS (vt.py), addmatch done, read-only vland WINS 1 (warnings: puts/abort
  import-thunk aliases; RELOC_NAME for SetGenerator 0x0074D0D0 and ~CThingAICreatureInit 0x004FAD50 -> NAME_FIXES.tsv).**
  What closed it: (1) every failure inside the loop is `break`, not `return`: the family handle is then released ONCE (its release has one
  IL reference, so c2 never caches family.Object in a register, the zero register goes to ebp, and the handle gets the LAST slot
  [esp+18h]) -- 932 -> 852 bytes in one step; (2) `CreatedCreatures.push_back(creature)` through the implicit CIntelligentPointer<CThing>
  conversion (retail re-takes `lea eax,[esp+30h]` after the ctor instead of pushing the ctor's result).
  EVIDENCED: break / implicit conversion / `size() <= 0` / at() expanded / out-of-line ~CThingAICreatureInit / IS_THING_ALIVE noinline /
  GetDef + GetSubDef bodies visible (as landed 0x0074E860) / HasHitCreationLimit defined earlier in the TU (missing `mov ecx,esi`).
  FITTED: nothing in the function text. DECISION: the HASHITCREATIONLIMIT_DEFINITION opt-in emits a second (non-inline) function in the
  row's TU -- it is the retail TU's order, but the coordinator must accept the shape (alternative: the definition in the row source).

## 0x00735FD0 CTCCombat::AttemptToStartParticle (near-miss; 6 grades by C36; unchanged DIFFER(993v973))
- Tested the 0x0074AF50 lesson: `break` for both failure exits gives 949 bytes with 4 string dtor calls / 1 ret / 2 def releases; retail has
  6 / 2 / 3 exactly as the `return` draft -> the exits ARE `return` here (first-only / second-only break: 993; `continue`: 929).
  differ/00735fd0.cpp = C29's draft unchanged. Residue as before: frame 0x5C v 0x58, weapon in ebp and the GetSubDef bool in bl (ours spills
  the bool to the parameter slot [esp+70h] and keeps weapon in ebx).

## Untouched by C36 (context): 0x0075D580, 0x006B2CA0, 0x00C271F0, 0x007E15C0, 0x009FDE80, 0x007BFE30
- 0x0075D580: name now has TWO sources: Ego_r 0x0076D6E0 + FableWin 0x025C98E0
  ?UpdateCameraParams@CTCCameraPointScriptedSpline@@QAEMMAAVC3DVector@@0AAM1AA_NPBV2@PBVCRightHandedSet@@@Z (public; float(float, C3DVector&,
  C3DVector&, float&, float&, bool&, const C3DVector*, const CRightHandedSet*); `ret 0x20` fits). No class header exists (siblings use local
  models); retail offsets seen: PTimingSpline +0x60, KeyCameras vector +0x68 (0x38-byte CKeyCamera), PPositionSpline +0x78, PViewDirectionSpline
  +0x80, PScalarSpline +0x88, PAnimationSpeedSpline +0x90, NextCrossInvalid +0xA0, NodeToStartOn +0xB4, NodeToEndOn +0xB8, AnimationSpeed +0xBC.
  Callees: CCardinalSpline evaluate 0x00A567A0 (x5), 0x0048DAE0, C3DVector 0x00A14440, 0x0075C420 / 0x0075C280 (by-value C3DVector helpers
  with the two pointer params), GetKeyCamera 0x0075CFE0 (landed), vector size 0x0075EE70.
- For the near-misses 0x007E15C0 / 0x009FDE80 / 0x007BFE30: check the exit statements first (return v break v continue: count retail's
  destructor copies and rets against the draft's, as done for 0x00735FD0 above) -- one cheap measurement per row.
