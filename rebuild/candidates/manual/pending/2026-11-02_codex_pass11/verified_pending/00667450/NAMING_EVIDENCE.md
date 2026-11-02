# Identity and naming evidence

`?ReactToGenericStrike@CThingCreatureBase@@IAEXABVCHitParameters@@@Z` is Ego_r00544570 / FableWin01d8fd50. It is a protected member, const CHitParameters reference argument, void return. Native caller006681c9 is the hit-response tail in CThingCreatureBase::OnHit. Donor order and the original action-class argument types independently confirm the identity. The1755-byte span00667450..00667b2b is one complete function, no embedded data or jump table.

## Closest verified shape

Selected grade37:1754 versus1755 bytes. Frame0x184, saved-register roles, all main blocks and cleanup placement now match. Remaining residues are the factory argument setup at00667707 (lea/mov/push ordering), the cyclic eax/ecx/edx choice at006677d8, and the two corresponding cleanup vtable-register choices. The accumulator form and-al is one byte shorter than native and-cl, accounting for the overall size. This row remains PARKED, not byte matched. All44 operand targets are independently verified with positional offsets. Historical1744-byte selected27 remains in its immutable attempt directory.


## Original headers and bodies

All game headers retain original class keys, full hierarchy, original access/const/virtual signatures and native complete size. Existing generated defaults are preserved by opt-in branches. Original raw TPI records are in creatures/types_core.json, types_react.json, types_react_aux.json, types_hit_aux.json, types_effectmap.json and the action/interface/mode enum records. Unused original fields are opaque byte ranges; no shortened fake classes. CHitParameters and GenericStrikeResponseKnockdown preserve unaligned original vector members as opaque12-byte fields, with no new packing directive.

Native sizes: CCreatureActionBase a8, CCombatActionBase114, GenericResponseToDecapitation114, GenericResponseToDrain114, GenericResponseToExplosion138, GenericStrikeResponseBase/GenericStrikeResponse148, GenericStrikeResponseKnockdown13c, CThingCreatureDef14c, CTCDExplosion78, CExplosionDef70, CSpecialEffectsDef34, NHitLocations::CHitLocation18. Original PDB member declarations and native constructors/member accesses prove the fields actually used. IsPushedByExplosions is byte149 bit1, not bit2.

The F1 visible helpers are actual GetSubDef<CSpecialEffectsDef> and actual effect lookup using the original CSpecialEffectsStringMap/std::map/std::_Rb_tree hierarchy, not dummy object layouts. The effect lookup reads its CCharString IN parameter and writes only the long OUT parameter. CCharString::GetCRC is original Ego_r00403c00 and native00404310. The manager specialization uses the same protected nonconst GetPDefFromGlobalIndex API already proved for OnHit. Genuine STLport4.6.2 hierarchy and member spellings come from _tree.h/_map.h; no invented flat map.

Original typed GetTCDriver<CTCDExplosion> has exact publics Ego_r00762cd0/FableWin025a6d40; native tests interface65 then performs mutable component lookup. A const bool reference materializes the presence byte the native body retains. The owner exception docs/EXTRA_RULES.md100–104 authorizes stronger annotation on GetTC/PeekTC and related header lookup family; enum GetTC and GFDebugPDynamicCast declarations are original PDB APIs. IS_THING_ALIVE<CThing*> is the actual visible noinline helper; the three sites where native expands the predicate use direct alive checks, while drain/explosion mode arms preserve the real helper call.

## Every direct relocation

44 operands,31 targets. RELOCATION_REVIEW.json lists exact emitted symbols, original PDB name sets, native sites and older bindings. All40 direct calls and their multiplicities agree; three epsilon operands and one literal are independently checked (44 total includes the full call count recorded by the auditor). The two LowerBound return types reflect retail STLport raw-pair pointers rather than donor Dinkumware iterator classes; original raw CVectorMap TPI plus real STLport4.6 source proves the difference.

- `0x0040dce0`: FableWin018962e0 exact const-reference IS_THING_ALIVE<CThing*> template; native loads pointer then Dead91 bit0.
- `0x0040f020`: Raw CVectorMap TPI mutable LowerBound(const ETCInterfaceType&); native mutable component lookups; original STLport4.6 raw-pair-pointer iterator type replaces donor Dinkumware iterator wrapper.
- `0x004365b0`: Const LowerBound overload; native const component lookups and owner settled overload mapping; genuine STLport4.6 pointer iterator.
- `0x0048c5c0`: FableWin01ad3120 exact CCountedPointer<CCreatureActionBase> destructor; native releases the factory-returned two-word action handle, decrementing its CCPPointerInfo and invoking its fastcall DeleteFunc.
- `0x004c73d0`: Ego_r004c9070/FableWin01cf3c00 exact public const CThing::GetPos returning const C3DVector reference; owner settled symbol and native position-copy/hitter subtraction usage.
- `0x004c9b80`: Ego_r004cb630/FableWin01cf7b10 CThing::Kill(bool); native victim receiver and true argument after successful EXPLODE_ON_KILLING_BLOW lookup.
- `0x0062e900`: Ego_r00545ce0/FableWin01d8ef00 virtual CCombatActionBase destructor; native common cleanup after drain/decapitation, each original derived class has no added members.
- `0x006644f0`: Ego_r00542800/FableWin01d93c40 public SetCurrentAction(const CCreatureActionBase&) returning bool; all native receivers are victim this, with original action subobjects as arguments.
- `0x00669580`: Ego_r00526490/FableWin01da97d0 const CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<const CSpecialEffectsDef>*); native type literal CSpecialEffectsDef and genuine manager/global-index lookup.
- `0x00669d30`: Ego_r00526ca0/FableWin01d89a40 CSpecialEffectsDef::GetEffectFromString(const CCharString&,long&) const bool; native string-key CRC/map search writes long output on success; real CSpecialEffectsStringMap member and original STLport4.6 tree layout.
- `0x0066a510`: FableWin01d915d0 virtual GenericStrikeResponseBase destructor. Raw CodeView attributes0x107 mark both derived and base dtors compiler-generated; derived has no members and its donor19-byte body only wraps the base78-byte teardown. Native22-byte function tears down intelligent pointer114 and tails to CCombatActionBase0062e900. Genuine implicit derived wrapper therefore calls the base dtor. See evidence/naming_support/strike_dtors.json.
- `0x0066a530`: Ego_r00545d90/FableWin01d8ee80 virtual GenericStrikeResponseKnockdown destructor; native paired with008679c0 and teardown of its CCharString114/intelligent-pointer118 members.
- `0x0066a560`: Ego_r00545dc0/FableWin01d91640 GenericResponseToDecapitation constructor(CThingCreatureBase&); native decapitation/killable/damageable arm constructs original size114 stack object.
- `0x0066a5b0`: Ego_r00545ee0/FableWin01d91df0 GenericResponseToExplosion constructor(CThingCreatureBase&,CThing&,const C3DVector&,const C3DVector&); native four arguments and size138 object. Original members PExplosion114, PointOfHit11c, HitDir128, FlipFacing134.
- `0x0066a670`: FableWin01d920a0 virtual GenericResponseToExplosion destructor; native object constructed by0066a5b0 and original intelligent-pointer member teardown.
- `0x0066a690`: Ego_r00545f90/FableWin01d92110 GenericResponseToDrain constructor(CThingCreatureBase&,CThing&); native stack object size114 and two object arguments.
- `0x00692e50`: Ego_r005197f0/FableWin01db2540 static fastcall CCreatureActionBase::GetPActionFromEnum(EActionRegisteredType,CThingCreatureBase&), returning CCountedPointer<CCreatureActionBase>; native hidden return pointer ecx, enum edx, creature stack argument.
- `0x0073a6e0`: Raw nested CModeBase and Ego_r007511e0/FableWin024c2fe0 const GetCurrentMode returning mutable reference; native PModes front load.
- `0x00815290`: Ego_r0080dcc0/FableWin024e6390 GetHitResponseActionType const enum; native ordinary-response branch and original NHitLocations class TPI.
- `0x008152a0`: Ego_r0080dcd0/FableWin024e63b0 GetHitKnockdownActionType const enum; native knockdown branch and three-accessor donor adjacency.
- `0x008152b0`: Ego_r0080dce0/FableWin024e63d0 NHitLocations::CHitLocation::GetHitKnockdownDeathActionType const returning EActionRegisteredType; native victim-death branch, contiguous donor order with other two action accessors.
- `0x00867990`: Ego_r0083d8b0/FableWin0281ab40 GenericStrikeResponse(CThingCreatureBase&,const CThing*,const C3DVector&,const C3DVector&); native object148 and following original setters at134/135.
- `0x008679c0`: Ego_r0083d8e0/FableWin0281ac80 GenericStrikeResponseKnockdown(CThingCreatureBase&,const CThing*,const C3DVector&,const C3DVector&); native object13c and victim/hitter/position/direction arguments.
- `0x008685a0`: Ego_r0083e480/FableWin0280f880 exact public const CHitParameters::IsPlayGetHitResponse(const CThing*); native first receiver is the hit argument, pushed argument is victim this.
- `0x008688f0`: Ego_r0083e860/FableWin0280fb50 IsExplosionStrike const bool; native CHitParameters receiver and adjacent IsDrainStrike match donor order.
- `0x00868920`: Ego_r0083e890/FableWin0280fc30 IsDrainStrike const bool; native adjacent call after IsExplosionStrike.
- `0x0099eae0`: Both PDB dtor names and CCharString lifetime boundaries in caller.
- `0x0099ebf0`: Both PDB ctor signatures and native two arguments (char pointer, length -1).
- `0x00a01b50`: Raw CBaseIntelligentPointer TPI, two PDB sets, native PData+4 dereference; owner settled spelling.
- `0x0125ab88`: Native null-terminated literal EXPLODE_ON_KILLING_BLOW, exact bytes checked against the emitted COFF literal; original GetEffectFromString usage supplies second source.
- `0x0129ba3c`: Owner ESTABLISHED_SPELLINGS raw Ego public+typed data record before GD3DFormats; exact float1e-4 and original assertion literal prove DELTA_NOTIONAL_ZERO.

## Indirect call and field evidence

CCreatureActionBase::SetHitter(const CThing&) is vtable slot40/+a0; SetHitDirection(C3DVector) slot41/+a4; SetHitPosition(C3DVector) slot42/+a8. Full original virtual declaration order, including protected methods and pure declarations, is preserved in its real header. CModeBase::GetType() const is slot1/+4. Def Delete() const is CIVCountedPointeeBase slot1/+4 with RefCount+4. The array/POD/native field stores at strike-response134/135 are original MoveOnHitOverride/CreateDustEffect.

## Behavior fixture and limits

The self-contained C++03 test expands60 complete original headers and instantiates the real original hierarchy. GNU64/-fno-rtti and VC7.1 RTM/GR- pass24 cases. The original15 cases remain, covering co-op early return, disabled response, ordinary reaction, zero direction, world position/null hitter, knockdown, berserk suppression, epic-spell exit, decapitation, threshold equality, dead-victim knockdown, dust/no-movement flags and epic influence movement. Nine new positive registered-action cases combine all3 selectors with null/live/dead hitter states. Actual CCountedPointer ownership allocates a real test-derived CCreatureActionBase and releases it exactly once; virtual direction/position/hitter calls and selector precedence are independently asserted. Receipt: behavior_tests/00667450/3c1b4d8b3b4b/RESULT.json.


Boundary constructors record action kind and vector arguments; the real response object fields are checked via a test-only friend. Active mode components, explosion-component suppression, successful special-effect lookup/Kill, and engine constructor implementations are not dynamically covered; their unsupported stubs abort if reached. The registered factory itself is a controlled external boundary returning a complete genuine test action. Static byte/control-flow and relocation review covers those paths but does not claim engine behavior tests. A missing explosion guard was found and corrected at grade13; earlier attempts intentionally remain in immutable history and are not selected.

Final behavior receipt: behavior_tests/00667450/a926139942c9/RESULT.json.

Function-local names such as hitter, position and action are reconstruction labels, not claimed recovered original local-variable names. Game class/member/function/enum names above come from the original PDB evidence.


## Fresh original local and registry evidence

Raw optimized and unoptimized procedure locals are saved in evidence/naming_support/react_original_locals.json: hit_params, killing_blow, pobject_that_hit_me, direction_of_hit, knock_down, action_response_type, phit_location, nomoveback, decapitate, point_of_hit, explosion, drain, pTCExplosion, paction_response, two ptc_cmm locals, effect_index, pfx_death, and combatresponse. Selected local spellings remain reconstruction labels; original names and exact types are recorded for the handoff. Raw GetTCDriver<CTCDExplosion> records are in react_driver_original_locals.json.

RegisteredActions at013ba8c8 is proved by raw public and DATA32 in evidence/naming_support/RegisteredActions_PROOF.json, protected static owner TPI, and native initializer0121e070/destructor012292e0. The unselected genuine-factory probe33 emits the exact raw PDB public symbol; NAME_FIXES.tsv reports the old Registry and other wrong bindings.

Original GetTCDriver source-line records are in evidence/trajectory_retry/EXPLOSION_GETTCDRIVER_PROVENANCE.json. The thing.tpl91–98 spans support a multiple-statement branch, not an invented one-expression forwarding helper. Attempts38/39 document why replacing the materialized-bool reconstruction with that stronger source-shape evidence currently worsens bytes; no unsupported claim that the named bool local was recovered is made.

## Fresh final retry receipt

Selected grade44 preserves the previous selected37 instruction shape and adds the required parked Grade source header. The source and test hashes above are current. Both fresh C++03 compiler lanes pass; the emitted masked body and all relocation names match the earlier selection (anonymous namespace path hash normalized only). This remains PARKED.

## Current selected54 correction (supersedes earlier parked notes above)

All1755bytes now RELOCATION_MATCH, with34 dual-compiler cases and44 independently reviewed operands. Earlier parked summaries are retained as history. Full proof and limitations are in PROBES.md's independent retry section and identity/. The target remains the same protected original member. Owning-module GetTC<CTCDExplosion> is restored; original local names and scopes are recovered. IsToRespondToHits, IsKillable, IsDamageable, PeekDefinition and IsUnderInfluenceOfEpicSpell declarations match exact public/PDB access/signatures and native corresponding field reads; emitted test symbols are checked in EMITTED_SYMBOLS.json. The const-bool presence reference is reconstructed, not claimed as a PDB local.

Production helpers use actual original class layers; the new guarded Release boundary delegates final deletion to CIVCountedPointeeBase. Supporting helpers themselves are not claimed byte-identical at other addresses. Test-only original constructor declarations/friends preserve full object layout and are absent from production. Active mode transitions and successful effect-map/Kill remain statically reviewed, not dynamically exercised.

## Main independent acceptance

Main reviewed the supplied row and all six neighbors, cut and read the full native1755-byte span, checked original helper/parameter/source-line evidence, then independently recompiled with VC7.1RTM. Review56 confirms the exact Leaf, every unmasked byte and all44 relocation operands. GNU64C++03/noRTTI and VC7.1RTM/GR- fixtures pass34 scenarios using native epsilon bits38d1b717, including equality at its real value. The earlier controlled0.5 fixture is preserved as historical evidence. The existing fixture dependency limitations and reconstructed presence-temporary caveat still apply. The function is now VERIFIED_MATCH; no out-of-scope helper or repository commit was made.
