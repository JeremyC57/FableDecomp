# Hand chunk 85 (CTCCreatureGroup .. CTCDHeroSuit): 31 of 40 byte-exact

All 31 src/ rows grade MATCH/RELOCATION_MATCH (grades.txt, gall.sh) and every test prints PASS under g++ (tall.sh) and the
lane's VC7.1 cl.exe (tall.py). 9 rows parked in differ/, 0 skipped.

## What matched (31)
- **CTCCreatureGroup** -- 00812f70 IsActiveMember: null/Dead (+0x91 bit 0) test, then a map find on the 64-bit UID.
  Shape: the UID comes from an inline by-value accessor and find takes `const unsigned __int64&`, so the key is copied to an
  8-byte frame temporary; the found node is compared with the map header (STLport `_M_header`).
- **CTCCreatureGroupTag** -- 004e5f18 ctor (`#pragma optimize("s")`: TaggerUID/TaggedFrame zeroed from one register before the
  vtable store); 008137b0 SetTagger (constant `false` result: both exits `xor al,al`); 00813860 RemoveMember / 008138a0
  DeactivateMember (vtable slots 29/30: base call, 64-bit `TaggerUID == member->UniqueID` low dword first, two bool exits);
  008138e0 GetTagger (`TaggerUID > 0` on an unsigned 64-bit member = `test edx,edx ; ja` + `cmp [eax],0 ; jbe`; GetPItem tail call).
- **CTCCreatureItemLevitationBase/Hero** -- 007f8390 DropItems (walk re-reads the header after the virtual call, then STLport
  `_M_clear` written out: `tmp = cur; cur = cur->next; free(tmp)`, header self-linked); 007f8490 PopItem (front node in a
  local reused after Kill for the inline erase); 007f8540 LeafUpdate (NumItems in a local cached in edi across the virtual
  GetMaxNumItems; an inline HasTimedOut reads LastAddFrame into a local BEFORE the frame call and adds 30 after it).
- **CTCCreatureNavigation** -- 006cb6c0 FindPosZAt (inline navigator member forwarding to the external interface slot 4 with
  `true`); 006cba80 SetNavigatorToType (byte NavigatorType `movzx`, inline find(first,last) returning `last`, result
  dereferenced unconditionally).
- **CModeStandBase** -- 00861e00 ctor (catalogued as a CTCCreatureNavigation ctor): base ctor, vtable, scalars from one zero
  register (bool as bl), CCharString ctor, twelve counted-pointer words.
- **CVtable_012b32a0** -- 00c1d850 GetTotalTimeSeconds (catalogued as IsPosNavLineOfSightTo): named local for the dividend so
  it is computed before the divisor's virtual call; unsigned `xor edx,edx ; div`.
- **CTCCreatureOpinionOfHero** (local retail-offset model: 4-byte STLport lists shift everything past +0x54) -- 006818f0
  IsWillingToContinueFollowingHero (switch 0..16 -> `cmp eax,10h ; ja` + byte-indexed two-entry jump table; ParentThing and
  the manager as locals so the GetWorld call precedes the push); 00681940 GetToleranceToBeingHitRemaining (`>= 0.0f` =
  `fcomp ; test ah,1 ; jne`, personality bonus only when the def is set); 00681ae0 KnowsOfDeed (two inline list finds sharing
  the `mov al,1` exit); 00681b30 OnKill (landed IS_THING_ALIVE idiom, GetPItem fetched three times); 00681e30
  SetOpinionReactionMask(const CCharString&) and 006822e0 SetOpinionDeedMask(const CCharString&) (definition manager as a local,
  zero index ignored).
- **CTCCreatureTintEffect** -- 007cede0 Reset: `(long)(def secs * g_fps)` = `fild [g] ; fmul [edi+3Ch] ; call __ftol2`; the
  four-word colour copy as a 4-trip loop the compiler unrolls with PDef re-read after every store.
- **CTCCreditsUI** -- 007e4a50 ctor: vtable after base ctor, members share one zero register, double from the pool.
- **CTCDExperienceOrb** -- 0077ace0 scalar deleting destructor (Leaf `??_G`): body `PDef = 0` + member destructor release,
  everything `__forceinline`, a `new` factory pulls vtable and ??_G into the TU.
- **Destructors (catalogued as ~CTCDExplosion)** -- 004dcd2b ~CTCKrakenTentacle, 004e1437 ~CTCKraken, 004e3ee1 ~CTCDExplosion
  (all `#pragma optimize("s")`), 006c1be0 ~CTCBuyableHouse: empty body, members destroyed in reverse order with RETAIL offsets
  (4-byte lists, 12-byte vectors), the forceinline CDefPointer release last, then the jump to ~CTCBase.
- **CTCDExplosion** -- 0080c210 GetDistanceFromMeAsRatioOfDamageRadius (right-to-left args: GetPosition first; __fastcall
  segment-distance callee; `fdiv [esi+0Ch]`); 0080c3c0 OnInitialActivate (thing def +0x70 used in place, GetSubDef result
  dropped, def read once, ftol life timer, ring reset to 0/-1); 0080c410 OnKill (iterator loop over the 12-byte-element
  Emitters vector, end re-read every trip, IS_THING_ALIVE body).
- **CTCDExplosiveTrail** -- 008100a0 OnInitialActivate (catalogued vfunc_6): the landed CTCAISummon hook without the base call --
  a counted local copy of the thing def (add-ref, release, tail `jmp [edx+4]` destroy).
- **CTCParticleAttacher** -- 004de7fd ctor (catalogued CTCDHeroSuit): `#pragma optimize("s")`, member-object zeroing ctors
  sharing one zero register after the vtable store.

## Parked for Fable 5.1 (9)
- 007714b0 CTCOpinionSource::PushNewSourceDef DIFFER(95v87): the inlined by-value push_back copy gets its own frame slot (`push ecx ; mov [esp+8],edi`), retail homes it in the parameter slot | tried: by-value push_back (95v87, best), const& push_back (84v87, loses the edi cache), two further push_back variants (4 grades) | next: a push_back that takes `const long&` but is passed the parameter itself (`DefStack.push_back(defIndex)` with the insert taking the parameter's address), or lane gap: frame-slot homing
- 0073ae10 CTCCreatureModeManager::AddMode DIFFER(99v92): the by-value CCountedPointer local's count block is re-read from its address-taken frame slot after the AddMode call (`mov eax,[esp+0Ch] ; mov esi,eax ; dec [eax]`), retail keeps it in esi | tried: temporary argument `AddMode(AllocateMode(t))`, named local with count-block-cached copy ctor/release (3 grades) | next: lane gap: address-taken local re-read (same family as 0073af60)
- 0073af60 CTCCreatureModeManager::SetupDefaultMode DIFFER(104v95): the GetSubDef out-parameter local is re-read from its frame slot at every use; retail loads it once into edi | tried: local CDefPointer copy + zero-constructed out-parameter (1 grade) | next: lane gap: address-taken out-parameter re-read (shares the AddMode cause)
- 006cba50 CTCCreatureNavigation::IsPosNavigable DIFFER(38v38): scheduling only -- retail loads pos (edx) before `push esi` and pushes the `false` before `mov esi,ecx` | tried: nested call expression `PNavigator->CanNavigateToPos(ParentThing->GetPosition(), pos)` and variants (3 grades) | next: lane gap: scheduling
- 00681790 CTCCreatureOpinionOfHero::TransformForcedOpinion DIFFER(40v43): retail tests PPersonalityDef through eax (`mov eax,[ecx+20h] ; test eax,eax`) and re-reads it into ecx; the lane CSEs both reads into ecx | tried: plain member, inline def member, early return, CDefPointer IsNull/operator-> (4 grades) | next: a volatile-free second read through a different inline accessor (e.g. `GetPersonalityDef()` returning the CDefPointer by const&), or lane gap: CSE
- 006817c0 CTCCreatureOpinionOfHero::TransformForcedOpinion DIFFER(40v43): byte-identical twin of 00681790, same diff | tried: same source as 00681790 | next: follows 00681790
- 006817f0 CTCCreatureOpinionOfHero::IsWillingToAttackHeroOnOpinion DIFFER(84v84): switch normalisation `sub eax,2` (83 e8 02) retail vs `add eax,-2` (83 c0 fe) lane | tried: long, unsigned long, enum, inline-helper switch forms (4+ grades) | next: lane gap: compiler build (both retail `sub` sites are in this TU)
- 006820e0 CTCCreatureOpinionOfHero::SetPersonalityOverride DIFFER(93v96): retail threads the inline strcmp (`repe cmpsb`) flags straight into `je <call>` with its own `pop ; ret 8` exit; the lane materialises `sete al` and joins at one `test al,al` | tried: plain, `? true : false`, `if (!idx && !(name == "")) return;` (4 grades) | next: write the empty-name test as a separate early-return statement before the index test, or lane gap: flag threading
- 0077a130 CTCDExperienceOrb::CombineWithExperienceOrb DIFFER(74v72): PDef (+0x0c) is held in ebx (push/pop ebx) instead of reusing esi once `this` is dead | tried: ternary GetTCDriver (76v72), `p = 0; if (bit) p = driver;` (74v72, best), position in a named local (74), raw CExperienceDef* PDef (74) (4 grades) | next: a real CDefPointer with a non-trivial `operator->`/`Get()` or SetExperiencePoints argument as a named local; else lane gap: register allocation

## Skipped
None (no row holds two functions).

## Label corrections
- 007714b0 "CTCCreatureGroupTag::DeactivateMember" -> CTCOpinionSource::PushNewSourceDef(long) (between SetupFromDef 0x007713C0 and AppliesToWitness 0x00771540; called by the by-name overload 0x00771620).
- 00813860 "vfunc_29" -> CTCCreatureGroupTag::RemoveMember; 008138a0 "vfunc_30" -> CTCCreatureGroupTag::DeactivateMember.
- 00861e00 "CTCCreatureNavigation::CTCCreatureNavigation" -> CModeStandBase::CModeStandBase.
- 00c1d850 "CTCCreatureNavigation::IsPosNavLineOfSightTo" -> CVtable_012b32a0::GetTotalTimeSeconds.
- 00681b30 "vfunc_14" -> CTCCreatureOpinionOfHero::OnKill (slot 14).
- 00681e30 "SetOpinionDeedMask" -> CTCCreatureOpinionOfHero::SetOpinionReactionMask(const CCharString&).
- 0077ace0 "`vector_deleting_destructor'" -> CTCDExperienceOrb `scalar deleting destructor' (`test [esp+8],1` only).
- 004dcd2b "~CTCDExplosion" -> CTCKrakenTentacle::~CTCKrakenTentacle; 004e1437 -> CTCKraken::~CTCKraken; 006c1be0 -> CTCBuyableHouse::~CTCBuyableHouse.
- 004de7fd "CTCDHeroSuit::CTCDHeroSuit" -> CTCParticleAttacher::CTCParticleAttacher.
- 008100a0 "CTCDExplosiveTrail::vfunc_6" -> CTCDExplosiveTrail::OnInitialActivate (vtable 0x01242B4C slot 6).
- Callee labels: 0x004C73D0 is CThing::GetPosition (manifest "CVertexBufferWin32::DoSizeof"); 0x00751190 / 0x00810C80 are
  GetSubDef<CExplosionDef> / GetSubDef<CExplosiveTrailDef> (manifest "GetSubDef<CAICreatureWillPowerIndicatorDef>").
