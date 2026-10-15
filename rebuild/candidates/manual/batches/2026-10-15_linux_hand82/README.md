# Hand chunk 82 (CTCBoastingPosition .. CTCCameraPointScripted): 33 of 40 byte-exact

All 33 matched rows grade MATCH/RELOCATION_MATCH from the exact file in `src/` (gall.sh, see
`grades.txt`), and every behaviour test prints PASS under g++ (tall.sh) and the lane's VC7.1 cl.exe (tall.py).
No row needed a pragma or flag. Rows whose catalog label is wrong keep the catalog `Module::Name` on
line 1 (so the payload lines up with the catalog) and choose the real symbol with `// Leaf:`.

## What matched (33)

- **CTCBoastingPosition** (2)
  - 00781da0 constructor: CTCBase(owner), the derived vtable store, then members in declaration order:
    PBoastingPodium (+0x0c, inline `CIntelligentPointer<CThing>` ctor = out-of-line base ctor + its own vtable),
    three bools sharing one `xor al,al`, PParticleEffect at the unaligned +0x2e (byte-packed TC hierarchy).
  - 00781df0 destructor: `if (fx.Get() && !fx.Get()->Dead) fx.Get()->Kill(true);` (three GetPItem calls),
    member pointer destructors in reverse order with no derived-pointer vtable store (the pointer template
    declares no destructor), tail jump to ~CTCBase.
- **CTCBob** (2)
  - 00787750 constructor: a body call `ParentThing->SetCutscenePauseBehaviour(2)`; VC schedules the vtable store
    between the argument setup and the call.
  - 00787780 OnInitialActivate: the physics component in a named local (edi across the virtual call),
    `Rhset = physics->GetRHSet()` (slot 72), `Pos = physics->Position`, `ForceVisible = false`.
- **CTCBoss** (6)
  - 007aeda0 SetNextBossPhase: `EnablePhaseChanges || force`, phase stored before the GetPosition call, the vector
    copied to the unaligned PhaseStartPos +0x22.
  - 007aede0 IsOnFinalPhase: nested `if (def) { if (phase < size()) return false; } return true;`, unsigned compare.
  - 007aee40 GetMinHealthForPhase: `MaxHealth * ((float)percent * 0.01f)` -- parenthesised so the literal multiply
    stays first (the left-to-right product moves the literal to the end).
  - 007aee90 IsReadyForPhaseChange: the parent thing in a local, `GetMinHealthForPhase(phase) > Health`.
  - 007aeec0 / 007aef10 (both labelled CountEffects): living-entry counters over HelperCreatures (+0x30) and Effects
    (+0x3c); unsigned index, size() re-read, the element bound to a reference local once per iteration.
- **CTCBulletTime** (2)
  - 0079ce80 PlaySoundW and its byte twin 0079de00: the landed out-parameter `GetInterface(type, &out)` template
    (type 16) feeding `PlaySound(sound, 0, flags, 0, -1)`, else the invalid-handle global.
- **CTCBuyableHouse** (9 matched under this label)
  - 006bf810 GetRentsDue: nested ifs; the clamp is `if (rents > 0) return rents;` (the ternary goes branch-free).
  - 006bf860 MovedInNotification: `DayNextRentIsDue = min(DayNextRentIsDue, ++day)` with the Dinkumware
    const-reference min (argument written back to its slot, self-store arm).
  - 006bf920 GetPriceForNextDressLevel: `Price[level+1] - Price[level]` behind `level >= 0 && level+1 < size()`.
  - 006bf960 IsForSale: owned early-out, then the sign post's Dead bit read through an inline `bool IsDead()`.
  - 006c0270 GetMaxCollectableRentValue: the Rent first pointer in a named local before the trophy call,
    `(long)(trophies * 0.1f + rents[level])`, then `PDef->MaxRentBags` through the scoped AddRef/Release
    CDefRef (landed 0x005BD1CF shape).
  - 006c0f70 CreateDressingSignPost: `DressingSignPost = CIntelligentPointer<CThing>(CreateSignPost(g_name, false))`.
  - 00747450 / 008ed010 / 00a77540 (labelled MovedInNotification, really three `find(...) != last` wrappers): the
    empty tag (user ctor) homed in a dead argument slot; 00747450 needs the key passed through an inline front end
    by `const&` (tag lea before the key read); 008ed010 `!= last ? true : false` (bare `setne al`) with `last` in a
    local; 00a77540 `return ... != last;` (full-register bool) with `last` in a local.
- **CTCCameraPointBuilding** (1)
  - 0075d940 GetOwnerBuilding: GetInterface(51) (word 1 bit 19, second LowerBound copy 0x004365B0), the owner
    pointer at +0x0d, building flag 0x40, `*out = building; return true;`.
- **CTCCameraPointDefinitionBase** (7)
  - 005ecfd0 / 00850780 / 0087b7e0 / 0087b9f0 / 00737160 GetPointType<T>: one member template, instantiated with type
    ids 6 / 0 / 1 / 3 (slot 28) and 1 (slot 3, a different hierarchy). Id 0 gives `test eax,eax`; id 1 gives `mov al,al`.
  - 00878a10 GetActiveCameraPosition: static `__fastcall` returning C3DVector (user ctors) through the hidden pointer
    in ecx, thing in edx; named local `(X, Y, Z + 1.6f)` returned through the memberwise copy constructor.
  - 00942230 (label wrong) constructor of a thing bounding sphere: three separate float members copied off `this`
    plus the physics radius (a struct member copy materialises `mov ecx,esi`).
- **CTCCameraPointScripted** (4 matched under this label)
  - 0052e0a0 / 00535320 / 00567a40 (labelled GetLookDirection): SGI `find(first, last, value)` front ends compiled /Gr:
    hidden result in ecx, the `const&` value passed through in edx, iterator classes on the stack, the tag in
    `last`'s slot; the tree iterator needs a user constructor (else it returns in eax), the list iterator has a
    user copy constructor (built in its argument slot).
  - 006e1b10 (labelled EditSetThingForEditorSetThing): `return C3DVector(P.X, P.Y, P.Z);` -- the value constructor
    gives retail's `fld Z ; fld Y ; mov X` mix; `return P;` is three integer moves.

## Parked for Fable 5.1 (7)

- 004fa9a0 CTCBulletTime::GetBulletTimeTicksFromAbilityLevel DIFFER(62v58): ebp frame with `mov [ebp-0Ch],0.5f ; mov [ebp-8],1.0f ; fld x ; fadd [ebp-0Ch] ; fistp dword [ebp-4] ; fild [ebp-4] ; fsub [ebp-8] ; fstp [ebp-10h] ; mov eax,x ; cmp eax,[ebp-10h] ; dec dword [ebp-4]` | tried: volatile locals + /QIfist + optimize("y",off) (74v58), plain C ceiling + /QIfist (62v58) | next: lane gap: inline __asm (asm_bake only)
- 0079c5f0 CTCBulletTime::GetBulletTimeTicksFromAbilityLevel DIFFER(61v68): the same inline-asm ceiling inlined after `fild [ticksPerSecond] ; fmul [ecx+2Ch] ; fstp [ebp-10h]` | tried: plain C ceiling + /QIfist | next: lane gap: inline __asm
- 0079c680 CTCBulletTime::GetBulletTimeRemainingPercentage DIFFER(80v77): the same inline-asm ceiling, then `fild [ebp-18h] ; fidiv [ebp-4]` | tried: plain C ceiling + /QIfist | next: lane gap: inline __asm
- 0079c6d0 CTCBulletTime::GetBulletTimeElapsedTicks DIFFER(64v71): the same inline-asm ceiling, then `mov eax,[ebp-4] ; sub eax,[ecx+0Ch]` | tried: plain C ceiling + /QIfist | next: lane gap: inline __asm
- 006c0120 CTCBuyableHouse::GetPrice DIFFER(57v57): ours `mov edi,[eax+28h] ; mov eax,[eax+2Ch] ; sub eax,edi`, retail `mov edx,[eax+2Ch] ; mov edi,[eax+28h] ; sub edx,edi` | tried: `GetValueOfTrophies() + def->Price[level]` (48v57), call result in a named local (60v57), `prices` first-pointer local + explicit `last - prices` (57v57), `prices` local + size() (61v57) | next: lane gap: register/load order of the size() difference (maybe a `const vector&` local plus the `prices` local)
- 007e3040 CTCCameraCollision::vfunc_4 DIFFER(93v81): retail keeps the local def pointer's object in esi from GetSubDef's `this` through the release (`test esi,esi ; dec [esi+4]`) and stores the local's null after the argument pushes; the lane re-reads the frame slot after the call, stores the null first and expands the release (`mov esi,[eax+4] ; add eax,4 ; dec esi`) | tried: CDefPointer local (93v81; member-template specialisation declarations ICE the compiler first), explicit release on a raw copy (119v81), raw out-pointer local (89v81) | next: lane gap: CDefPointer local cached in esi across a call (GOTCHAS 24k)
- 0075cd30 CTCCameraPointScripted::GetEndPos DIFFER(81v81): X sum `fld [esi+40h] ; fadd [eax]` stored after two pushes (retail) vs `fld [eax] ; fadd [esi+40h]` stored after three | tried: free `operator+(pos, EndPos)`, member `pos + EndPos` (X and store order right, Z/Y reversed), member `EndPos + pos` | next: lane gap: x87 operand order / store scheduling of the last component

## Skipped

None. No row spans two functions (every int3 tail is plain padding).

## Label corrections

- 007aeec0 "CTCBoss::CountEffects" counts HelperCreatures (+0x30); only 007aef10 counts Effects (+0x3c).
- 0079de00 "CTCBulletTime::PlaySoundW" is called from CTCPhysicalShieldSpell::CancelMove (0x0079E300) and its
  neighbours: most likely CTCPhysicalShieldSpell::PlaySound (byte-identical body, propagated label).
- 00747450 / 008ed010 / 00a77540 "CTCBuyableHouse::MovedInNotification" are unrelated find wrappers: a thing list on
  the interface-54 component (caller CombatDialogueManager::IsSpeechAllowed), a possession list on the interface-60
  component (caller CGameScriptInterface::IsObjectInThingsPossession), a position list (caller
  CNavigatorManager::IsActionPoint). Only 006bf860 is the CTCBuyableHouse method (caller CTCDummyVillager::CreateVillager).
- 00942230 "CTCCameraPointDefinitionBase::GetActiveCameraPosition" is a constructor (position + physics radius),
  called from CAIStateGroup_KidReactToBlockage::IsRunnable.
- 0052e0a0 / 00535320 / 00567a40 "CTCCameraPointScripted::GetLookDirection" are SGI `find` front ends (tree, tree,
  list iterators); 00567a40's caller is CTCHeroOnlineScoreboard::DisplayScoreboardGUI.
- 006e1b10 "CTCCameraPointScripted::EditSetThingForEditorSetThing" is a C3DVector-by-value getter of +0xd4 used by
  CParticleEmitter::IsVisible.
- GetPointType<CTCCameraPointFixedPoint> (005ecfd0, 00737160, 00850780, 0087b7e0, 0087b9f0) are different
  instantiations (type ids 6, 1, 0, 1, 3); 00737160 uses vtable slot 3, so it is not even the camera-point hierarchy.
- 004fa9a0 "CTCBulletTime::GetBulletTimeTicksFromAbilityLevel" is the standalone copy of a float ceiling helper.
- Callees: 0x00686800 is CTCBase::CTCBase (label CEngineSubPrimitive2DClockSprite), 0x00686830 ~CTCBase
  (~CChunkedFileChunk), 0x004C73D0 CThing::GetPosition (CVertexBufferWin32::DoSizeof), 0x00686A80 a CTCBase world
  accessor called as a __thiscall member (CViewBase::GetDisplayEngine), 0x006869D0 CTCBase::GetDefinitionManager
  (a _Cons_val<...> label), 0x004C7990 the thing's definition-manager accessor (CViewBase::GetDisplayEngine),
  0x00686850 CTCBase's empty slot-4 body ($E2).
