# Workflow wf3 verdicts (two independent Opus lenses per row: mechanical + evidence; one Opus repair pass)

## 00931200 -- VERIFIED, author attempts 1

- What made it match: The port onto the shared engine headers; the old draft's "register plan" residue was model error from its local CThing / list / intelligent-pointer stand-ins. Source shape: IS_THING_ALIVE(PHome) guard; `CThingAICreature& creature = GetCreature();` taken before the first PHome read; MeInsideHouse = PHome->IsPointInside(creature.GetPos()); HeroInsideHouse as the int-valued `&&`; GuardInsideHouse = false; CThing::GetTC(TCI_VILLAGE_MEMBER, &village_member) && GetPVillage(); `const std::list<CIntelligentPointer<CThingAICreature> >& people = GetPVillage()->PeekVillagePeople()` walked with a const_iterator; IS_THING_ALIVE(person) && person->HasTC(TCI_GUARD) && PHome->IsPointInside(person->GetPos()) sets the flag and breaks; return IsValid(). The one trap: engine/CAIStateGroupBase.h must be included before engine/CAIBrain.h, otherwise GetCreature() is emitted as a call (391v387).

## 009dc6e0 -- VERIFIED, author attempts 1

- What made it match: Statement order plus one inline callee. All eight X/Y stores come first in vertex order, then the eight U1/V1 constants, then per vertex: Z = 0.0f, SetRHWFromZ(), SpecularColour = 0, DiffuseColour = argb. The RHW comes from the header's inline CGenericVertex::SetRHWFromZ() (existing opt-in FABLE_RETAIL_CGENERICVERTEX_SETRHWFROMZ), which folds to RHW = g_ScreenRHW for the known Z of 0. Plain `v[i].RHW = g_ScreenRHW` stores left an extra register copy for vertex 3 and different lea registers at the two Add2DTri calls. Source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf3/009dc6e0/009dc6e0.cpp; test: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf3/009dc6e0/009dc6e0_test.cpp; payload.json and oracle.tsv written by addmatch.py in the same directory.

## 00761980 -- VERIFIED, author attempts 3

- What made it match: One statement added to pass 2's best source: the owner thing is named in a reference before the lifted vector is built, and the call uses it.

`CThing& parent = GetParentThing(); C3DVector top(start.X, start.Y, start.Z + 0.15f); ... parent.GetPos() ...`

`CThing* parent = ParentThing;` and `CThing& parent = *ParentThing;` also grade RELOCATION_MATCH.

Why it works, as far as the graded variants show: retail's scratch registers read as a rotation in evaluation order. The block copy takes eax (source pointer), ecx (X), edx (Y), eax (Z). The parent load (0x00761A39 `mov ecx,[edi+4]`) then takes ecx, start.X takes edx, start.Y takes eax, and info.Engine takes edx. The parent's ecx overwrites the register that still held X, so X is reloaded; X's edx overwrites the register that held Y, so Y is reloaded too. That accounts for the two reloads and the unsplit block copy. With the parent read at the call in the argument list, X and Y are forwarded in registers and the function is 8 bytes short (381).

`GetParentThing` is the FableWin.pdb inline accessor already in the repo's engine/CTCBase.h.

## 00a89740 -- VERIFIED, author attempts 1

- What made it match: Two things. First, the port onto the shared headers in the form the dossier derived: copy the helper position to a local, TransformPoint it in place with the bone's LocalizationMatrix, call CalcBoneTransformation(state, *this, bone, transform, scale), `pos = scale.GetScaled(pos)`, TransformPoint again, `return pos`, with the no-bone return last. That gave the right instructions at once. Second, spelling CMatrix3x4::TransformPoint's rows as plain left-to-right sums in retail's evaluation order (`E3k*zz+E2k*yy+E1k*xx+E4k`) instead of the header's parenthesised default; that moved the epilogue's result-pointer load and `pop esi` to retail's position. It is a new header opt-in, same arithmetic, default path unchanged.

Final source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf3/00a89740/00a89740.cpp
Test (vt.py PASS): /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf3/00a89740/00a89740_test.cpp
addmatch.py wrote payload.json and oracle.tsv in the row directory; NOTES.md, LESSONS.md, LABELS.tsv, STATUS.tsv and hdr/README.md are written there too.

Two things the coordinator should know:
- My first Bash call printed a Python traceback (an assertion in my own file-writing script), which breaks the no-traceback rule once; nothing was compiled or written wrongly because of it.
- A cleanup `rm` of scratch files was blocked by a safety check, so v1..v14.cpp, tA..tF.cpp, *.out, r.txt and o.txt are still in the row directory. They are not part of the hand-back.

Local models in the row file are only for classes with no shared header: C3DHelperPoint, C3DBone2 (Ego_r member names) and C3DAnimationManager (declaration only). No made-up names; no landed name proved wrong.

## 009122a0 -- VERIFIED, author attempts 1

- What made it match: Two source-level facts the parked draft had wrong. (1) The squares go through the engine's inline `GFSqr(float)` (PDB `?GFSqr@@YIMM@Z`, already in the repo's engine/C3DVector.h under FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER): `GFSqr(1.0f - t)` has its argument evaluated before the object expression `Point2 * 3.0f * t`, which reproduces retail's early `fld (1-t)` at 0x00912337 and the square spilled into the dead t argument slot; `GFSqr(t)` is needed as well. (2) The sum is held in a named local and returned by value (`C3DVector result = ...; return result;`), which leaves the four extra component spills in the final additions. Both pow calls use math.h's float overload `pow(x, 3.0f)`. Caveat: the helper being GFSqr specifically is an inference from the bytes (an inline float square with an early-evaluated argument; GFSqr is the only such helper in the PDBs) since an inlined helper leaves no call to prove the name. Final source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf3/009122a0/009122a0.cpp

## 00be7be0 -- VERIFIED, author attempts 1

- What made it match: The tail of the function is three header inlines named in FableWin.pdb, not Render's own code: HasWater() (null test), CEngineWaterRenderer::AddBackgroundWaterPatchToRenderList(patch, alphaed) and, nested in it for sea water (types 3/4/5) and called directly for a water-only patch, AddMidgroundSeaPatchToRenderList(patch, alphaed). Both helpers take the sub-patch pointer by value and pass its address to CArray::push_back(const T&). That accounts for every old residue: two stores of the pointer to the arg0 slot (one per by-value copy), a single PolyCount test (each helper starts with !IsEmpty(); the nested one folds), the cross-jumped sea arm with esi = 4 (the sea helper is if/else with two push_back statements, not a list select), and four out-of-line push_back calls in the background helper. The STLport _M_insert_overflow body must be visible (not declaration-only) for both copies to share the arg0 slot. Retail holds unreferenced out-of-line copies of both helpers at 0x00BE9030 and 0x00BE9080 (not in the manifest) with the same statements in the same order, which is the evidence for the helper bodies. One thing for the coordinator: the two helpers are FABLE_FORCEINLINE in the shared-header opt-in. As plain inline, VC7.1 expands them only at the constant-false water-only site and calls them at the variable-alphaed site (180 v 391); retail expanded both, same bodies either way.

## 0081f240 -- VERIFIED, author attempts 1

- What made it match: Replacing the old draft's local structs and raw member expressions with the shared headers and the PDB inline helpers: `SetEngineThemeAtIncludingExtraCells(to.X, to.Y, source.GetEngineThemeAtIncludingExtraCells(from.X, from.Y, i), i)` in a `for (long i = 0; i < 3; ++i)` loop, then two explicit `SetEngineBlendAtIncludingExtraCells(..., source.GetEngineBlendAtIncludingExtraCells(..., n), n)` calls for n = 0 and 1. The helpers alone reproduce what the old draft called register pressure (this->HeightMap reloaded three times, source cells base spilled to the dead `source` argument slot). The last hunk was the counter's signedness: it is a signed long (`jl`), although the helper parameters are unsigned long. No flags, no pragma.

## 00646580 -- VERIFIED, author attempts 1

- What made it match: Four things, in the order found. (1) Port onto the shared headers with the dossier's identities (const CEnginePrimitivePrepareInfo& parameter, Effects as an STLport vector, ScaleAlpha, the 16-argument sprite constructor, CIEngine::SetPersistentPrimitive on info.Engine, static CDrawBase::ClearEffects). (2) `if (!IsShowSpecialAbilityButtons()) { ... } else FlourishEffect = 0;` -- retail lays the reset arm last. (3) Sprite arm as two named locals before the sprite statement: `C2DVector anchored = GFAnchorToBottomRight(def.AbilityButtonFlourishEffectPos); C2DVector position = def.AbilityButtonFlourishGraphicOffset + anchored;` with the sprite position argument `C2DVector(position)`. The copy is an INFERENCE (marked in the source comment, same idiom as the landed DrawButton 0x00645099): it is the only spelling tried that gives retail's late non-popping x87 stores, and behaviour is identical. (4) The last 3-hunk residue (second uv temporary at [ebp-24h] instead of [ebp-8]) was callee body visibility: GFAnchorToBottomRight 0x00643D0C is defined earlier in the same retail TU; with its real landed body in the row file (`#pragma auto_inline(off)`) the compiler reuses the anchored local's slot. That in-TU body itself grades RELOCATION_MATCH against 0x00643D0C.

## 0052c920 -- VERIFIED, author attempts 1

- What made it match: Two things. First, the port onto the shared headers (engine/NUISystem_CChangingStateComponent.h, engine/NUISystem_CUIState.h, C2DVector::IsNonZero and DELTA_NOTIONAL_ZERO in place of the draft's local classes, GetLengthSquared and g_Epsilon). Second, the colour delta written as a copy of the target colour followed by four channel `-=` against the component's Colour, compiled with the existing FABLE_RETAIL_CRGBCOLOUR opt-in, whose copy constructor reads all four channels through Initialise before any store. That reproduces retail's memory-operand subtracts for R and G, the ebx byte register, and the R,G,A,B store order. The subtraction is spelled on the public channel members in the function body because neither PDB names a CRGBColour operator- or operator-=; no made-up names are used.

## 00848b70 -- VERIFIED, author attempts 1

- What made it match: Two things. (a) The port onto the shared retail headers (expanded CThing GetTC/PeekTC, inlined IS_THING_ALIVE from engine/ThingAlive.h, GetSubDef<CCarryableDef> body visible but out of line, CTCCarryable::PCarryingTC, CCarryableDef::ActiveCarrySlot, CClass::GetType, PutThingInCarrySlot(CThing&, long, bool)) removed the old draft's register and frame residue. (b) The two FinishAction() calls are the ELSE arms of nested ifs: `if (IS_THING_ALIVE(PThingToPickUp)) { ...; if (ptc_carryable->PCarryingTC == 0) { ... } else FinishAction(); } else FinishAction();`. That gives retail's tail placement (carried arm, then dead arm). Source: /tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/wf3/00848b70/00848b70.cpp, leaf `?PerformAction@CCreatureAction_PickUpGenericBox@@UAEXXZ`.

