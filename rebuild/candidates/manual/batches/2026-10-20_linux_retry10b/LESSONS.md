# Retry 10 -- lessons (fold into GOTCHAS at landing)

- The "inline-once destructor band" is not a cost band: VC7.1 (RTM 3077 and QFE alike) expands an inline destructor at
  EVERY site while the object has two destructor sites, and at exactly ONE site once it has three or more live sites
  (unreachable sites do not count). For a by-value parameter the expanded site is the first one in source order (the
  `return` inside the wait loop) and the later sites call the out-of-line copy and tail-merge; for a block-local (the
  vector in 0066bd00) the expanded site is the block end and the earlier returns call out of line. So a script thread
  with "both exits inlined" (00d17090, 00e32200) is written with a THIRD exit: `if (IsActiveThreadTerminating())
  return;` after the loop instead of folding that test into the final `&&` / `if (!...)`. Retail's tail (`jne` past the
  flag store straight onto `call ~CScriptThing`) is the merged sites 2 and 3.
- Per-callee, not per-caller: inline accessors, extra sibling functions in the TU, and wrappers around the other callees
  never changed the destructor decision; a dead member store in the callee can (vector `_M_start = 0` -> once,
  CCountedPointer `Info = 0` -> never), but it perturbs the register colouring (zero register kept for the null test).
- A bool function whose result is a comparison: `return a >= b;` emits `xor eax,eax ; cmp ; setge al` (int-width);
  retail's bare `setge al` is a NAMED bool (`bool ok = a >= b; return ok;`), `if (..) return true; return false;` or
  the `? true : false` ternary (0059da10).
- A long-returning inline helper (`return 1; / return 0;`) negated into a bool result: `!(unsigned char)Helper()`
  gives retail's `mov eax,1 ; ... ; test al,al ; sete dl` (byte test of the full-register constant); `!(bool)Helper()`
  tests eax, a bool helper emits `mov al,1` (007525c0).
- The inline `CCharString == ""` family (sete materialised): write the null-block `strcmp("", "")` and the block compare as
  two tests joined by `goto` into the SAME arm (the landed 0x0048F7C0 idiom), keep the non-empty arm in line and put the
  empty arm at the label; a default literal passed to a `const CCharString&` is an implicit conversion
  (`SetAnimationName("LIT")`), not a named local -- the named local is homed in a different dead argument slot
  (008bb130, 008bb200, 008bb6f0, 00892cf0).

## CAnimInit passed as a temporary (008d6780, 008d6cb0, 008d6e80, 008d6f20 -- NCreatureMode::*::AnimUpdate)
`anim.SetMainAnim(CAnimInit(type, 0, entry->BankIndex, 1, looping, flags, 100), blendIn, 0);` -- the record is a
temporary bound to the `const CAnimInit&` parameter, not a named local. With a named `CAnimInit init(...)` VC7.1 treats
the inline ctor's `anim` parameter as a variable: the BankIndex load is hoisted and coloured into a callee-saved register
(esi/ecx) and `&init` goes through the leftover scratch register; as a temporary the load is a scratch temp (edx, or eax
loaded lazily after the flags push) and `lea eax,[esp+14h]` follows. Same instructions, different registers -- the
"register colouring only" family note for these rows was this idiom. Also: the slot-4 virtual `GetAnimFilter()` of
CModeBase takes NO argument; the `push 0` before `call [eax+10h]` is the third (long) argument of
CThingCreatureBase::GetAnimFromName(const CCharString&, const CAnimFilterBase*, long) @ 0x00662B10.

## CDefPointer release through a `long&` (00591e8c, and 5-12 bytes closer on 20 more release-family rows)
`--Object->RefCount == 0` written directly makes VC7.1 either re-read the pointer slot after the `dec` (member form:
`dec [eax+4] ; mov ecx,[slot] ; mov eax,[ecx+4] ; test`) or materialise the count in a register (local-copy form:
`mov edx,[eax+4] ; add eax,4 ; dec edx ; test edx,edx ; mov [eax],edx`). Decrementing through a reference,
`static __forceinline bool DecRefToZero(long& count) { return --count == 0; }` (or `long& count = p->RefCount; if (--count == 0)`),
gives retail's `dec dword ptr [reg+4] ; jne`. With a local copy of the pointer in the destructor the remaining gap on the
other rows is only `mov eax,[slot] ; test eax,eax ; mov ecx,eax` versus retail's `mov ecx,[slot] ; test ecx,ecx`.

## Call result used directly as the object of a virtual call (00543d30, 00543df0 -- CParticleEffect::CreateEmitter)
`CFrontEndManager::GetInstance()->GetLayer(Depth)` inside the argument list, not `CFrontEndManager* frontEnd = GetInstance(); ... frontEnd->GetLayer(Depth)`.
With the named local VC7.1 hoists the vtable load (`mov edx,[eax]`) right after the call, which lets the temporary
CRightHandedSet's constructor stores sink past the next push; written inline the vtable load stays at the call site
and the stores land between `push 1` and the GetLayer call as in retail. Same family as the CAnimInit lesson: a named
local for a value used once changes scheduling/colouring, a temporary or inline expression does not.

## STLport vector model: the end-of-storage pointer is an _STLP_alloc_proxy (00458c19, 00457da9, 004e00bc, 004570d3)
A 12-byte vector modelled as three plain pointers zeroed in one ctor (`first(0), last(0), end(0)`) makes VC7.1 schedule
the next member constructor's `lea ecx,[this+N]` one or two zero stores too early ("24i lea-scheduling lane gap").
Model it as STLport does: `T* _M_start; T* _M_finish; CAllocProxy<T> _M_end_of_storage;` with
`template<class T> struct CAllocProxy { T* p; CAllocProxy(T* v) : p(v) {} };` -- the third store then comes from a
member-object constructor and the lea lands between the second and third store exactly as retail.

## Zeroing a vector member through an inline member function (00724f80 -- CTCPhysicsControlled::OnCreate)
`ImpulseVelocity.Set(0.0f, 0.0f, 0.0f)` with `void Set(float x, float y, float z) { Z = z; Y = y; X = x; }` reproduces
retail's Z,Y,X store order AND keeps the following bit-field loads (`mov cl,[esi+3Ch]`) after the stores: stores through
the member function's `this` pointer may alias, so the loads are not hoisted. Direct member stores
(`ImpulseVelocity.X = ... = 0.0f`) let the lane hoist the first flag load above them.

## Def-constructor store order: a -1 member is a record with an inline ctor, the scalar band goes in a novtable base (00f29d00)
In `CCutsceneDef::CCutsceneDef` the lane sank the bit-field clear (`and cl,0F8h ; mov [esi+24h],cl`) and the `-1` store below the
vtable store, retail keeps declaration order. A plain `long m_index(-1)` is the trigger: modelling it as `struct CDefIndex { long
Index; CDefIndex() : Index(-1) {} }` (the chunk-212 CDefIndex lesson) restores the -1 and bit stores, and putting the scalar band
(+0x25..+0x36) in an intermediate `__declspec(novtable)` base with its own inline ctor puts the vtable store after them, right before
the vectors, as retail. Alloc-proxy vectors alone did nothing here (all stores are `mov [esi+N],eax` without a lea).

## A retail `CArray<T>` member in a constructor is the STLport _Vector_base shape too (007437c0, 007438a0, 008ddfb0)
`CCardinalSpline::ControlPoints` (CArray, 12 bytes) modelled as three plain words zeroed in one init list lets the `push -1` of
the next member's string literal float above the three zero stores; with the alloc-proxy third member (`CArrayAllocProxy<T>
_M_end_of_storage` with its own ctor) the push lands after them as retail. Same rule as the def vectors: any 12-byte
start/finish/end record retail built from the STL gets the proxy, whatever the project called it.

## Constructor store order: every member that retail built with an inline ctor is a scheduling anchor (007445e0, 00f29d00)
VC7.1's scheduler sinks immediate stores (`mov [esi+1Ch],-1`) and RMW bit stores below the zero-register stores that follow
them, but never across the inline constructor of a member object. So a `CDefPointer<T>` member must be a record with an inline
ctor (`Object(0)`), not a plain pointer initialised with 0: the -1 store then stays just above the CDefPointer store as in retail.
A derived value computed from a base member (`UpdateOffset = ParentThing->m_uidLow & 3`) that retail computes after the last
init-list store is a body assignment (in the init list its `and` is computed right after the load).
- CScriptThing (12 bytes) is `CScriptBase` + `CCountedPointer<CScriptThing> PImp` (Ego_r.pdb): model PImp as ONE record with an inline
  ctor, not two raw `void*` words -- the record keeps the member's vtable store above its two zero stores (007e71d0; the first of
  two CScriptThing members happened to match either way, the second did not).

## "component loaded into eax then `mov ecx,eax`" = the GetInterface out-pointer model (008955c0, 00896120)
When retail has `mov ecx,[eax+4] ; test ecx,ecx ; je ; call` after the interface-map join (`mov eax,esi`) and the lane gives
`mov eax,[eax+4] ; test eax,eax ; je ; mov ecx,eax ; call`, the source used `GetInterface(&out, type)` (out-pointer, two joins).
Write it as the landed 0x00584157 idiom: `if (thing->HasInterface(type)) { T* tc = thing->GetTC<T>(type); if (tc) tc->M(); }`
with `GetTC` = `(T*)((it == map->last || wanted < it->type) ? map->last : it)->component` (a __forceinline template member).
A probe shows every other spelling of the null-tested call (named local, if/else, early return, && / ternary return without
the model) keeps eax; the ternary GetTC with HasInterface outside is what retail inlined.
- The same model when the component is used without a null test: `T* tc = 0; if (HasTC) tc = GetTC();` when retail preloads the
  register (`xor ecx,ecx`), and an UNINITIALISED `T* tc; if (HasTC) tc = GetTC();` when retail's miss arm reads the slot
  (`mov ecx,[esp+34h]`, 00749bf0). `cvt.py <src> <dst>` in the retry10 workspace adds the GetTC/HasTC members to any parked
  source with the GetInterface template; only the call site needs rewriting by hand.
- When the thing comes from an out-of-line GetPItem(), read it ONCE into a local (`CThing* t = P.GetPItem(); if (t->HasTC(x)) tc = t->GetTC<T>(x);`): HasTC and GetTC on separate GetPItem() calls add a third call (00784600, 00847fb0, 00848fd0, 009492a0).
- Retail mixes the two lookup spellings per call site: a call with several pushed arguments can keep the eax + `mov ecx,eax` out-pointer form (0093c420 SetState with 5 args, 008c4a80) while its neighbours use the ternary GetTC; convert one call site at a time and keep whichever matches.

## A call result's member used as `this`: name the CALL result, chain the member (00972110, 00954a20, 0093e240)
`CCombatManager* combat = GetPWorld()->m_pCombatManager; combat->Deregister(creature);` gives `mov eax,[eax+4Ch] ; mov ecx,eax`;
the direct chain `GetPWorld()->m_pCombatManager->Deregister(creature)` pushes the creature before the world call; retail
(`call GetPWorld ; mov ecx,[eax+4Ch] ; push edi ; call`) is `CWorld* world = GetPWorld(); world->m_pCombatManager->Deregister(creature);`
-- the named local is the call result, the member read stays in the call expression. Complements the CAnimInit/GetInstance lessons:
what gets a name decides which load is hoisted.

## The def-release `mov ecx,[slot] ; test ecx,ecx` register (probe result, still open on ~20 release rows)
Probing 30 spellings of the CDefPointer destructor (named/unnamed local, pointee inline Release/DecRef members, free helpers,
base-class reference, && / early-return forms): the loaded pointer only lands in ecx when the slot is WRITTEN after the load
(`T* o = p; p = 0; if (o) ...` or the clear inside the if), which also emits the store retail does not have (the slot escaped
to GetDef, so VC7.1 keeps a store that precedes the Destroy call; a clear after the last call is dead-store-eliminated but
then eax comes back). So retail's shape needs the pointer held in a register without a visible store -- not found yet.
- CDefPointer zero-store placement (0069a920, 00730630 family): an inline GetDef wrapper `out.m_pObject = 0; return GetDefRaw(idx, &out.m_pObject);` with an EMPTY CDefPointer ctor puts the zero store after the argument pushes as retail; it also changes which of this/index is loaded first (open).
- A `return false` early exit that retail places AFTER the success path (shared epilogue at the end) is one nested `if (ok) { ...; return true; } return false;` (00ab6180 under /GS; the chunk-209 lesson, confirmed on the RTM compiler).
- Mechanical ternary sweep (retry 10): every remaining parked row whose source still used the out-pointer `GetInterface(&tc, t)` form was rewritten with `$W/cvt.py` to `if (thing->HasTC(t)) { T* tc = thing->GetTC<T>(t); ... }` with `GetTC` = `(T*)((it == map->last || t < it->type) ? map->last : it)->component`. 13 of 36 regraded RELOCATION_MATCH on the RTM compiler with no other change (list in `$W/var/tern_rows.txt`: 0091f050 0090fb60 00933470 00683650 005ee418 0065bffa 004c8fb0 00663ba0 006a8560 00839080 0064b5c1 0064e0da 006a8370). The 23 others differ only in where the component lands (eax form kept, or the component loaded before/after pushes) -- those keep their old sources. A missing-entry lookup dereferences `last`, so behaviour tests must keep the presence bit and the entry consistent (004c8fb0: an out-of-range bitset position must map to a clear word).
- 00b1b4d0 (SDecalRequest ctor): an immediate store (`mov [eax],1Eh`) that retail keeps between two zero stores is pinned by making the NEXT member (the +8 byte) a record with an inline zeroing constructor: VC7.1 sinks immediates below zero-register stores but never across a member-object constructor, so the anchor goes on the member after the immediate, not on the immediate itself (MATCH).
- 00a5c570 (CSimpleMemoryPool ctor): the opposite case -- when retail sets the zero register BEFORE an unrelated init-list store, the zeroed sub-object (CArea Root) has NO inline constructor; its words and the other zero members are plain body assignments after the init list, which lets VC hoist `xor ecx,ecx` above the init-list store (RELOCATION_MATCH).
- 00b8ff00 (CEngineInternalPrimitiveBase ctor): a bit-field byte whose retail load sits early and whose merged store sits late is several bit-field assignments SPREAD through the body (the load belongs to the first one, the store to the last); VC7.1 merges them into one and/or pair across intervening plain stores, but NOT across a for-loop (00bddf30: a `for` over Children splits the merge into two `and`s; hand-unrolled stores keep it merged).
- 004016f0 (CDisplayManagerInit ctor): when retail materialises two constants at the top (`xor eax,eax ; mov ecx,1`) and stores the zero group BEFORE the one group, the SOURCE assigned the `true`/1 members first: VC7.1 floats the zero-register stores above the 1-register stores, and the +0x18 immediate then stays before the +0x19 register store.
- 00779ca0 (CTCDExperienceOrb ctor): `xor ecx,ecx` before `or eax,-1` with the zero stored first = the -1 members modelled as records with an inline (-1) constructor (or the zero member as a zero record); both plain in the init list gives `or` first.
- 00def6a0 / 00c0a790 (open): a `mov ecx,edi` (this of a member ctor) interleaved between zero stores, and a float constant materialised lazily before its first store, do not move with init-list/body/record spellings (3 each tried).
- 00a5f290 / 00def6a0 (ctors): a `mov ecx,edi` (the this of the NEXT member's out-of-line constructor) that retail places between the second and third of three zero stores means the three words are an STLport vector (start, finish, alloc-proxy end-of-storage with its own inline constructor), not a flat 3-word record: the proxy's constructor is a member-object boundary and the scheduler slides the `mov` across it. Any unexplained 12-byte triple-zero group in a constructor should be tried as `CArray`/vector with the alloc proxy first.
- 0072ac40 / 00584b42: the ternary-GetTC sweep (cvt.py) only caught `GetInterface(T** out, long type)`; the same fix applies to the other spellings (`GetInterface(long type, T** out)`, a `__fastcall` static caller). Any remaining row whose source still writes an out-pointer interface lookup should be converted by hand.
- 00853260 (IsFinished): when retail places the shared `mov al,1` exit right after the second test (`je A ; ... ; je A` with A in the middle), the source has TWO separate early `return true;` statements (`if (!p) return true; if (p->dead) return true;`), not one `||` condition (which puts the exit last, `jne`).
- 006e2cf0 / 007fe670 (STLport find): when retail loads the `first` iterator straight into ecx AFTER the tag/value pushes (`lea ecx,[esp+10h] ; push ecx ; mov ecx,[esi+28h] ; mov edx,edi ; call`), write the call to the tagged four-argument `find(first, last, value, random_access_iterator_tag())` directly on the raw vector words (`_M_start`/`_M_finish`); an inline by-value wrapper (`find(first,last,val)` forwarding) or an inline `begin()` accessor makes VC7.1 evaluate `first` early into eax and copy it to ecx (`mov eax,[..] ; ... ; mov ecx,eax`). A wrapper taking its iterators by const reference also matches.
- Ternary sweep 2 (retry 10): the same rewrite applied to the OTHER if-form spelling, a map `Find(kind)` method (`if (it == last || kind < it->kind) it = last; return it->tc;` -> `return ((it == last || kind < it->kind) ? last : it)->tc;`), regraded 43 remaining rows (`$W/var/tern2_rows.txt`): 5 more RELOCATION_MATCH (008a0720 0089bbc0 0089fe20 00691560 00438ff0). The remaining 38 keep their other gaps (component-before-argument, eax form, frame slots); the ternary itself never made a row worse by more than the register choice, so it is safe as the default spelling of every interface lookup.
- 0051e4f0 (GetPhysicsDebugInfo): a virtual call whose vtable retail loads into eax but the lane into edx (`mov edx,[ecx] ; call [edx+N]`) means the object pointer was NOT held in a named local: write the member expression twice (`CThing* thing = it->PThing; if (thing->PPhysics) thing->PPhysics->Call(..)`); VC7.1 CSEs the second load and eax is free for the vtable (MATCH). A named `physics` local keeps eax reserved.
- 0048dc3c (GetTextOffsetFromEmbeddedTagGraphic): same rule as 00853260 generalised -- a return that retail places right after a PAIR of equality tests (`je A ; cmp ; jne next ; A: add eax,.. ; jmp`) is two separate `if (x == a) return r; if (x == b) return r;` statements; the `||` form moves that return below the default return.
- 006621c0 (IsFreeToFinishAction): the mirror of the 00853260/0048dc3c rule -- when retail's `mov al,1` tail sits AFTER the `xor al,al` tail and every test jumps to it (`je true ... jl true`), the early `return true`s are ONE `||` chain; separate `if (..) return true;` statements inline the last `mov al,1 ; pop ; ret` before the false tail. (Separate statements = the shared exit placed right after its own tests; a chain = the exit at the end.)
- 00927df0 (CAIStateGroup_SetupWares::IsRunnable): when two call arms each end in `return false` in retail with one shared `xor al,al` tail after the SECOND arm, write them as `if (c) A(); else B(); return false;` inside the guard; two separate `return false;` statements put the shared tail after the first arm.
- 0063ef50 (CThingFilter_OverlappingCircle::operator()): x87 square order follows statement order, not operand order. `dx*dx + dy*dy` in one expression squares the RIGHT term first; `float d2 = dx*dx; d2 += dy*dy;` squares dx first. Pick the accumulator form whose statement order equals retail's fmul order.
- 00bb6ab0 (CEnginePrimitiveManagerWeatherMask::RenderPrimitive): three-term squared distance with retail order Y,Z,X = `float d2 = dy*dy; d2 += dz*dz; d2 += dx*dx;` (`d2 = dy*dy; d2 += dz*dz + dx*dx;` also matches). The single-expression sum always squares the last term first.
- 007483e0 (CCompare_FurthestFromInitPos2D::operator()): a frame 4 bytes larger than the live locals with a "2D" vector temporary means retail's temporary was 12 bytes (C3DVector-sized, Z never written); model it as `struct { float X, Y, Z; ctor(x, y) : X(x), Y(y) {} }`. Pair with the accumulator form (`r = Y*Y; r += X*X`) for the x87 square order.
- 006cb620 / 006cb6e0 / 006cb840 (CTCCreatureNavigation distance tests): every "squared-distance term order" park is fixed by the accumulator form: `float r = a*a; r += b*b; r += c*c;` in retail's fmul order. The single-expression sum (any grouping, any operand order) is reassociated by VC7.1 so the written order never reaches the x87 code.
- 00987bf0 / 00addfe0 (CMatrix34 Transform): `Dot3(t) + T * w` reassociates to z,y,w,x; `float y = Dot3(t); y += T * w;` keeps w last (z,y,x,w). A `+=` statement is a sequence point the reassociation does not cross.
- 0082fd90 (CThingAICreature::WorldUpdateBulletTime) / 0075c530 (CTCCameraPointDefinitionBase::IsAngleValid): same rule as 006cb620: any parked "x87 sum-of-squares / dot-product term order" row is an accumulator (`s = t1; s += t2; s += t3;`) in retail's fmul order; works inside an inline GetLength()/dot helper too.
- 00733020 (CThingFilter_HasBuildingAsHome::operator()): for `cmp <call result>,<member>` (retail `cmp eax,edx`) hold the call result in a named local and write `local == member`; `member == f()` gives `cmp edx,eax` and `f() == member` inline folds into `sub ; neg ; sbb ; inc`.
- 00702c60 (CThingObject::DrawIsDrawable): a bool return of the low bit that retail computes as `mov al,[m] ; and al,1` is `return (m & 1) ? true : false;`; `(bool)(m & 1)`, `(m & 1) != 0` and byte casts all go through `mov eax ; and eax,1` or `xor eax,eax ; mov al`.
- 00c6adb0 (reverb SetDefaults, 22 constant stores): when retail's store sequence is in ADDRESS order except for a hoisted shared-register constant (1.0f in eax, 0 in edx), write the body assignments in address order and let the scheduler hoist; writing the shared-constant members first makes the lane also CSE another repeated constant (0.04f) into a register that retail keeps as immediates.
- 00c915a0 (CBitstream::ReadBits): when retail materialises the default result (`or edi,-1`) BETWEEN two compares of an `A && B` guard, the assignment sits inside the first test in the source: `if (A) { value = -1; if (B) goto advance; }` (or `if (!A || (value = -1, !B))`); `if (A && B) value = -1; else {...}` gives a separate `mov edi,-1 ; jmp` block.
- 004b64e0 (CQuestManager::SaveGameState): when retail lays the then-arm of an `a || b` test first (`cmp 2 ; je THEN ; cmp 3 ; jne ELSE ; THEN:`) but `if (a || b) {then} else {else}` builds `je THEN ; je THEN ; ELSE ; jmp ; THEN:`, put the condition in a named bool (`bool writing = a || b; if (writing)`); the same change let the compiler duplicate the call tail of a pair of null-tested string accessors (two WriteBuffer calls) instead of merging the arms.
- 00639930 (CTCRegionDisplay::FinishShowingRegionName): an x87 compare whose `fild`/`fld` order is swapped against retail with the SAME `test ah,41h ; jne` is a sign the parked condition is inverted, not a scheduling gap: `fild elapsed ; fld d ; fsub ; fcompp ; test ah,41h ; jne` skips when d <= elapsed, i.e. the source is `elapsed < d`. Check the semantics before shuffling operands.
