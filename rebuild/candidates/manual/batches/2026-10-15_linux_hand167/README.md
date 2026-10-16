# Hand chunk 167 (CTCKraken .. CTCOwnedEntity): 27 of 40 byte-exact

All 27 src/ files grade RELOCATION_MATCH (grades.txt); 27/27 tests PASS under g++ (tall.sh) and VC7.1 (tall.py). 11 of the 40 catalog labels were propagated byte-twin labels (see Label corrections).

## What matched (27)
- CTCKrakenTentacle: 0x004DB2E7 ctor (size TU, `optimize("s", on)`: `push [esp+14h]`, the vtable kept in ebp); 0x007F0810 RemoveObstruction (IS_THING_ALIVE + Kill(true); the clearing CIntelligentPointer temporary goes in its own `{}` block so the tick store comes after its dtor).
- CModeDragonStrafeBurninate 0x008D7BB0 EndBeamEffect (catalog CTCKraken::EndHeadEffects): the landed 0x008C99C0 kill-and-clear shape.
- CModeHoverBase 0x008EAC40 ctor (catalog CTCKrakenTentacle): the vector is the STLport `_Vector_base` + `_STLP_alloc_proxy` model (puts `mov ecx,edi` between the 2nd and 3rd zero); the list mallocs its sentinel; `CurrentName = ""` in the body.
- CTCLight: 0x00640BB0 SetColour (member CRGBColour ==/!= compared R,G,B,A); 0x00640DF0 SetOverridden (inline CCountedPointer Release zeroing Info then Data); 0x00640E60 SetLightDef (SetOverridden(true) inlined; members copied through re-read pointers); 0x00640F00 ctor (vectors as `_Vector_base`/alloc proxy for the `lea edi` placement; the secondary appearance base's inline ctor stores its vtable first).
- CCombatActionBase 0x0062E900 dtor (catalog ~CTCLightningOrb): `__declspec(novtable)`, empty body, members in reverse; the strike-event list calls its out-of-line clear 0x0062E2B0 then frees the sentinel.
- Entity scripts 0x00CDFEB0 CEndGameBarrier / 0x00EE02C0 CTH_SnowTroll ctors (catalog CTCLightningOrb): the landed CDarkwoodTrader shape with two default CScriptThings.
- CTCLook: 0x007528A0 CentreHead (`(float)fabs(x) < 1e-4f`; the -1 store written after the change test, not via inlined SetHeadTurnState); 0x007530D0 FrameUpdateHeadLookAtPos (plain source; VC itself keeps the eyes bool in the angle's frame word and the speed in the dead pos slot); 0x00753AD0 IsLookTimeOver (out-pointer GetInterface `&&` flag test; `return frame >= LookTimer`).
- CTCMeshAutomaticEntityCreator 0x0072E4C0 CatchMessage: HasTC written `if (bits & mask) return true; return false;` (a `!= 0` form gives `shr ; test cl,1`).
- CTCMultiArrow 0x007CC120 RemoveEffectOnWeapon, 0x007CC190 CancelForCutscene, 0x007CC230 ResumeForCutscene (signed `(long)size()` loop; `T* e = 0;` + untested GetInterface; Asleep bool bit field at +0x45); CTCMultiStrike 0x007CC9E0 RemoveEffectOnWeapon (IS_THING_ALIVE on `*(begin()+i)`, re-indexed for Kill).
- CTCNymph: 0x004E3325 ctor (size TU; byte-aligned intelligent pointers under pack(1); the spline's end-of-storage word as an alloc proxy); 0x0080F190 IsDecentAttackRange (delta via member operator-, nested float ifs); 0x0080FD90 OnInitialActivate (owner def copied into a pointer local with hand add-ref/release around GetSubDef; resize(5) with a default C3DVector).
- CTCHeroRangedAttacks 0x00789BB0 ctor (catalog CTCOpinionSource): init list in declaration order.
- __linear_insert<CWideString> 0x009B58B0 (catalog CTCOpinionSource::PushNewSourceDef): the landed 0x00A70FF0 /Gr template.
- CTCOracleMinigame: 0x007E61B0 FireOutput (landed CRandomGenerator::Next inline; the sound name a named CCharString local, an unnamed temporary passes the ctor's eax); 0x007E6230 SetText (`T* p = 0;` preset, unnamed CWideString temporary). The engine header is data-only, so both use a local model with its offsets.
- CTCOwnedEntity 0x007E83B0 OnInitialActivate: CTCBase must be a real base (a flat vptr + __int64 class pads the vptr to 8); owner and ParentThing named locals for the drawable copy.

## Parked for Fable 5.1 (13)
- 007fb610 CTCKraken::RaiseSunkenTentacles DIFFER(137v133): `sub esp,8` and the fps copy in its own frame word; retail `push ecx` shares one slot for the GetInterface key, the fps copy and the spilled world frame | tried: direct global, fps+def named locals, own `{}` block, `__forceinline` GetTC returning the out pointer | next: lane gap: frame-slot sharing of a short-lived local
- 00752120 CTCLook::ChangeHeadAngleXY DIFFER(91v112): ours folds `* (1/360) * 0.5f` into one constant and CSEs the halved range for both limits; retail two fmuls, twice, bool in dl | tried: def local, inline GetHalfHeadRangeXY(), `/ 2.0f` | next: an inline DegreesToTurns(float) helper returning by value, or a non-literal 0.5 member
- 00752190 CTCLook::ChangeHeadAngleYZ DIFFER(100v100): `fmul [esp+14h]` (speed*time) before the second `sub esp,8` in retail, after it in ours | tried: plain, def named local | next: lane gap: scheduling only
- 00752480 CTCLook::IsAbleToTurnViaAnimation DIFFER(133v137): retail loads PParentCreature into eax, tests it there and `mov ecx,eax` before each call; ours loads ecx directly | tried: IS_ALIVE on CThingCreatureBase*/CThing*, inline accessor, Dead bit field + IsDead() | next: lane gap: looked-up pointer used as `this` loaded via eax
- 007525c0 CTCLook::GetTurningAnglesAndCheckForTurningBody DIFFER(115v115): the negation tests `eax` where retail does `test al,al ; sete dl` after a full-register `mov eax,1 / xor eax,eax` | tried: bool local, bool helper, long helper + (bool), unsigned char helper | next: long helper with `? false : true`
- 0082fd90 CThingAICreature::WorldUpdateBulletTime DIFFER(108v108): sum of squares z*z + x*x + y*y vs retail z*z + y*y + x*x | tried: X+Y+Z, Y+X+Z, X+Z+Y | next: lane gap: x87 sum-of-squares term order
- 007498e0 CTCNoise::OnCreate DIFFER(162v160): the zeroed CDefPointer store before the GetDef pushes (retail after them) and the def read via eax then `mov ebp,eax` | tried: CDefPointer dtor, pointer local + hand release, `__forceinline` GetThingNoiseDef(), raw pointer out | next: lane gap: temporary CDefPointer zero store scheduled after the pushes
- 0074a100 CTCCreatureGenerator::GetNumGeneratedCreatures DIFFER(147v137): CDefPointer dtor decrement compiles to `mov edx,[eax+4] ; add eax,4 ; dec edx ; mov [eax],edx` (or reloads Object), retail keeps the def in ecx with `dec dword ptr [ecx+4] ; jne` | tried: member dtor, pointer-local dtor (two forms), const/mutable vs plain | next: pointer local read once after GetDef with a hand release (the 0x007498E0 attempt-2 form gave `dec [ebp+4]`)
- 00766050 CTCObjectAugmentations::HasCombinationOfAugmentations DIFFER(116v110): sub-def release `mov ecx,eax ; add eax,4 ; dec edx ; test edx,edx ; mov [eax],edx` vs retail `mov eax,[ecx+4] ; dec eax ; mov [ecx+4],eax ; jne`, plus this/index in edi/ebx (retail esi/edi) | tried: dtor, scoped holder | next: lane gap: def release register shape (shared by the next three)
- 007660c0 CTCObjectAugmentations::HasAugmentationOfType DIFFER(131v116): same release gap on both exits; type argument in ebp (retail ebx) | tried: dtor | next: lane gap: def release register shape
- 00766140 CTCObjectAugmentations::GetDamageMultiplier DIFFER(119v113): same release gap | tried: member dtor, pointer-local dtor, member ReleaseReference(), optimize("s") | next: lane gap: def release register shape
- 00766400 CTCObjectAugmentations::GetExperienceMultiplier DIFFER(119v113): same release gap | tried: hand release on a pointer local, member accessor + member release | next: lane gap: def release register shape
- 00771770 CTCOpinionSource::CleanupSource DIFFER(150v151): register choice only, this in ebp and the next-node cursor in ebx (retail ebx/ebp) | tried: it/node loop forms, owner local, declaration order | next: lane gap: register choice (scheduling-only)

## Skipped
None.

## Label corrections
- 0x008D7BB0 "CTCKraken::EndHeadEffects" = NCreatureMode::CModeDragonStrafeBurninate::EndBeamEffect (works on +0x98, PDB PBeamWeaponEffect +0x9C; callers 0x008DCB4F / 0x008DE132 in CModeDragonStrafeBurninate).
- 0x008EAC40 "CTCKrakenTentacle::CTCKrakenTentacle" = NCreatureMode::CModeHoverBase::CModeHoverBase(CThing*) (RTTI vtable 0x01287D14).
- 0x0062E900 "~CTCLightningOrb" = CCombatActionBase::~CCombatActionBase (already named so in landed callers).
- 0x00CDFEB0 / 0x00EE02C0 "CTCLightningOrb::CTCLightningOrb" = CEndGameBarrier / CTH_SnowTroll entity-script ctors (vtables 0x012C3434 / 0x012F42B0).
- 0x00752120 "CTCLook::ChangeHeadAngleYZ" = ChangeHeadAngleXY (XY members; CentreHead calls it first).
- 0x0082FD90 "CTCLook::UpdateHeadPos" = CThingAICreature::WorldUpdateBulletTime (vtable 0x0127293C slot 20).
- 0x0074A100 "CTCObjectAugmentations::GetDamageMultiplier" = CTCCreatureGenerator::GetNumGeneratedCreatures (caller CGameScriptInterface::CreatureGeneratorGetNumGeneratedCreatures).
- 0x00766400 "CTCObjectAugmentations::GetDamageMultiplier" = GetExperienceMultiplier (+0x38).
- 0x00789BB0 "CTCOpinionSource::CTCOpinionSource" = CTCHeroRangedAttacks::CTCHeroRangedAttacks (RTTI vtable 0x0126BAF4).
- 0x009B58B0 "CTCOpinionSource::PushNewSourceDef" = STL __linear_insert<CWideString*, CWideString, less<CWideString>> (/Gr).
- 0x007498E0 "CTCNoise::vfunc_4" = CTCNoise::OnCreate.
- Callee labels: 0x00686800 is CTCBase::CTCBase; 0x0059346A / 0x0080FE60 / 0x0042B248 are GetSubDef<CLightDef> / <CNymphDef> / <CAugmentationDef>; 0x00749AC0 GetDef<CThingNoiseDef>; 0x004BD390 CTCGraphicAppearance::RegisterComponent; 0x00661E80 CThingCreatureBase combat-state query; 0x004C7AA0 the world-camera accessor; 0x00497460 copy_backward<CWideString>; 0x009B1870 __unguarded_linear_insert<CWideString>.
