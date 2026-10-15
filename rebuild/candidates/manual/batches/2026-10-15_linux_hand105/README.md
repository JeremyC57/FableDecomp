# Hand chunk 105 (CTCVolumeContainmentTracker .. CTargetPoint): 37 of 40 byte-exact

All 37 matched sources grade MATCH / RELOCATION_MATCH with the leaf the lander derives from line 1 (re-checked
explicitly), and every test prints PASS under g++ and the lane's VC7.1 cl.exe. Three rows are parked in differ/.

## What matched (37)

- CTCVolumeContainmentTracker
  - 00785c80 OnThingCarried(CThing*) (Ego_r takes `CThing&`; same code) -- run Update once, push CurrentThemeChannel onto the carrier's graphic appearance
    (CThing +0x64) and SetAsChanged(4). Shape: the TC pointer is a plain member re-read after the first call.
- CTCWallMountEffects
  - 007df610 constructor -- CTCBase(thing), null def / creature pointer, FrameEffectLastUsed = 0xC0000000, then
    `ParentThing->PDef->GetSubDef(&PDef)`. Shape: derived vtable first, member pointer's base call + vtable inline.
  - 007df3e0 OnPutOnMount -- teleport effect adds "CTCTeleporter" (AddTC by name), any effect persists the object.
    Shape: the CCharString is an implicit conversion temporary (re-addressed with `lea`).
  - 007df440 OnTakenOffMount -- RemoveTC(0xDD) and restore persistence. Plain `&&`.
- CTCWaterWader
  - 007b5b40 constructor -- two CIntelligentPointer<CThing> members built from 0 (`SetPItem(0)` inline), OldWade = false.
- CTCWeapon (engine/CTCWeapon.h through a derived helper struct for methods; local model only for the constructor)
  - 005da4f0 FrameUpdate (vtable slot 9) -- `++TimeWielded` while wielded, `++AimingTime` unless -1.
  - 005da6a0 constructor -- AimingTime = -1 in the BODY (its immediate store is last; init-list zeros share eax).
  - 005da850 GetAnimationSpeedValue, 005da950 IsProjectileWeaponAutoLoading, 005da980 IsProjectileWeaponLoaded,
    005daa00 IsMeleeWeapon, 005dad10 IsBlockBrokenOnBlock, 005dad60 IsCapableOfStealthAttackBy -- all read the weapon
    def through a by-value CDefPointer copy built from the raw object (`if (p) AddRef`) whose destructor releases WITHOUT
    a null test; a def field read into a local in its own `{}` scope releases before the tests. `||` on a bool gives
    full-register `mov eax,1`/`xor eax,eax` tails; IsCapableOfStealthAttackBy needs `if (type == 0) return
    IsUsableBy(t); return false;` (the early-return polarity swaps the tails).
  - 005da540 GetUnsheatheAnim / 005da5a0 GetSheatheAnim -- `(GetPosition() - rhset->last).GetLengthSquared() > e*e`
    picks the "normal" or "combat" string of the def; the C3DVector temporary stays on the x87 stack (three
    `fstp st(0)`), `e` is a global so `fld e ; fmul e` is not folded.
- NUISystem::CRedefinerList
  - 00556350 constructor -- CScrollableList(parent) out of line, six zero members (FadeOutTime 0.0f shares the integer
    zero register), three derived vtables (+0, +4, +0x18) stored after the members.
- NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar
  - 00656caa constructor -- CQuestInfoElement(2.0f), Type, Percentage 0, two white CRGBColour(r,g,b,a) (B,G,R,A memory),
    Updatable, Graphic 0, empty vector. Needs `#pragma optimize("s", on)`: only the size TU passes a float literal as
    `fld ; push ecx ; fstp [esp]` and stores 0.0f as `fldz ; fstp`.
- CVtable_012B8778 (intrusive list node of the combat-action pools)
  - 00c4cf40 destructor -- inline Unlink() then the root's user-declared empty virtual destructor stores 0x012B5868.
- CTCWeaponTrail
  - 0076de60 IsTrailActiveForThing -- the out-parameter local of GetWeaponTrailUsedInStrikeAction(long&) is homed in
    the dead `thing` slot; `result == this && index == trailIndex`.
  - 0076e4a0 HasWeaponTrailPoints -- if/else returning two compares gives the hoisted `xor eax,eax` + `sete`/`setg`.
  - 0076e5f0 GetLastWeaponTrailPoints(vector&, long) -- `if (i < NoTrails) { ...two tail calls... } return false;`;
    the retail STLport list is 4 bytes (vector stride 4).
- CTCWeatherVane
  - 00732d00 OnInitialActivate -- placement pointer as a named local and one store per arm (`value + 0.5f`), the absent
    case `g_zero + 0.5f` with an extern 0.0f; the single ternary picks eax for the vptr where retail uses edx.
- CAnimateThing
  - 00661720 IsFinished -- `IsAnimationFinished() || m_markedFinished` (bit 0 of +0x94, `test byte ptr ..,1`).
- CTCWife (engine/CTCWife.h where the offsets agree; a local model past the GiftsReceived map, which is 4 bytes
  smaller in retail)
  - 006cec90 GetNumHoursMarriedFor -- zero-initialised float result local; `(float)(now - wed) / (fps * 3600.0f)`.
  - 006ceea0 IsOfferingSex -- fps (via the unused GetWorld() call) and the def pointer taken into locals before the
    frame call.
  - 006cefe0 OnCreate -- the definitions holder is a named local (call before the argument push); inline world-random
    `if (range != 0) return seed % range; return 0;` into FrameLastCulledGiftsReceived (+0x88).
  - 006cfe80 IsAttractedToHero -- `IsCourtingHero() && (Love > g_zero || IsInLoveWithHero())` with the inline
    IsInLoveWithHero (out-of-line copy landed at 0x006CF900).
  - 006cfec0 IsWillingToMarry -- five-term `&&` in `if (...) return true; return false;` (byte tails).
  - 006d03e0 InitWifeNonpersistentState, 006d0800 OnInitialActivate (the same body inlined) -- inline
    IsMarriedToThePlayer, SetAttractionIcon(true, 0.5f, 255), NeedsToChangeBrain = true.
  - 006d1de0 ReceivedConfirmation(bool) -- nested ifs; the marriage test is evaluated twice.
- CVtable_012B5A80 (pooled character-morph action)
  - 00c40410 Init -- `X = Y = Z = 0.0f` chained assignments for the first two vectors (Z stored first), Set(x,y,z) for
    the three axes; one zero and one 1.0f register; returns true.
- CTCWillEnflame
  - 007c6a40 CancelMove -- the landed out-parameter GetInterface template (type 0xA7, bit test folds to `test cl,cl ;
    jns`), early return when the component state is not 5, common `m_moveState = 3` tail.
- NScript::CV_TravellingHeroesScript::CTH_Trigger_Bowerstone
  - 00ee13b0 IsNightTime -- `if (t > 1800 || t < 700) { if (!IsAborted()) return true; } else { IsAborted(); } return
    false;` (the abort test runs in both arms).
- CTargetPoint / CShaderConstantRange / CVolumeTexture
  - 004d7a76 CTargetPoint() -- `or dword ptr [..],-1` stores need `#pragma optimize("s", on)`.
  - 00987d50 CShaderConstantRange(), 00a695c0 CVolumeTexture() -- CBaseClass base call, plain zero initialisers before
    the derived vtable store.

## Parked for Fable 5.1 (3)

- 0076e290 CTCWeaponTrail::SetLastProjectileWeaponTrailPoint DIFFER(84v84): pair copy scheduling -- retail `mov ecx,[esp+4] ; mov edx,[esp+8] ; mov eax,esi ; mov [eax],ecx ...`, lane `mov edx,[esp+4] ; mov ecx,esi ; mov [ecx],edx ; mov eax,[esp+8] ...` | tried: `back() = Get(point)` implicit copy (73v84), + memberwise operator= (83v84), named result local (87v84), destination reference taken before the call + named local + memberwise operator= (84v84) | next: lane gap: register scheduling of a 24-byte memberwise copy (try C3DVector with its own user operator= / copy constructor)
- 0076ef40 CTCWeaponTrail::OnCreate DIFFER(92v91): retail `movzx ecx,byte ptr [eax+6Ch]`, lane `xor ecx,ecx ; mov cl,byte ptr [eax+6Ch]` (rest identical) | tried: `(b & 8) ? 2 : 0` on an unsigned char, a 1-bit bitfield (94v91), /G7 (90v91), /G6 (92v91) | next: the byte read through a `bool`/enum-returning inline accessor widened to long (wave 24p: a bool forwarded to a long is `movzx`), else lane gap: movzx selection
- 007e2da0 CTCWobble::SetWobbleParameters DIFFER(92v79): retail inlines an inline-assembly ceil helper (`fadd [ebp-4] ; fistp dword [ebp+0Ch] ; fild ; fsub [ebp+8] ; fstp [ebp-0Ch] ; mov eax,[ebp-8] ; cmp eax,[ebp-0Ch] ; jne ; dec [ebp+0Ch]`) | tried: volatile locals + `(long)` under /QIfist /Op (`fistp qword`, 92v79) | next: lane gap: inline-assembly helper (asm_bake oracle only)

## Skipped

None: no row in this chunk holds a second function.

## Label corrections

- 00556350 "CTCWeapon::CTCWeapon" -> NUISystem::CRedefinerList::CRedefinerList (vtables 0x0124A994/0x0124A970/0x0124A968).
- 00656caa "CTCWeapon::CTCWeapon" -> NPlayerGui::CDrawQuestInfo::CQuestInfoElementMiniGameBar ctor (vtable 0x0125A46C).
- 00c4cf40 "CTCWeapon::FrameUpdate" -> CVtable_012B8778::~CVtable_012B8778 (the intrusive pool node; ctor 0x00C4CF20).
- 005da4f0 "CTCWeapon::vfunc_9" -> CTCWeapon::FrameUpdate (slot 9).
- 005da540 "CTCWeapon::GetSheatheAnim" -> CTCWeapon::GetUnsheatheAnim (returns the Unsheathe strings +0xd4/+0xd8);
  005da5a0 is GetSheatheAnim.
- 005da950 "CTCWeapon::IsAnalogueLoading" -> CTCWeapon::IsProjectileWeaponAutoLoading (def +0xc4 = PDB +0xd4); the
  real IsAnalogueLoading is 0x005DA8F0 (def +0x90), by the getter order of the Ego_r declaration.
- 0076ef40 "CTCWeaponTrail::vfunc_4" -> CTCWeaponTrail::OnCreate (slot 4).
- 00661720 "CTCWife::IsInLoveWithHero" -> CAnimateThing::IsFinished (descriptive; out-of-line inline copy, no callers).
- 006d0800 "CTCWife::InitWifeNonpersistentState" (duplicate) -> CTCWife::OnInitialActivate (vtable 0x0125ED5C slot 6).
- 00c40410 "CTCWife::ClearAllAppearanceChangedNoticed" -> CVtable_012B5A80::Init (character-morph action reset).
- 00987d50 / 00a695c0 "CTargetPoint::CTargetPoint" -> CShaderConstantRange / CVolumeTexture constructors.
- Callee labels confirmed propagated: 0x00686800 ("CEngineSubPrimitive2DClockSprite" = CTCBase(CThing*)), 0x004C9840
  ("CBaseObject::OnPropertyChanged" = CThing::RemoveTC), 0x004C73D0 ("CVertexBufferWin32::DoSizeof" =
  CThing::GetPosition), 0x0099EAE0 ("_Cons_val<...>" = ~CCharString), 0x00F35B30 (= the script abort test),
  0x00555180 ("CTCHeroPodium" = CScrollableList ctor), 0x00656795 ("CVsyncCallbackTimerInfo" = CQuestInfoElement(float)).
