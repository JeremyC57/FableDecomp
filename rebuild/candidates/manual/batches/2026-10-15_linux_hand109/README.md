# Hand chunk 109 (CThingCreatureBase .. CThingFilter_HasFreeActionPoint): 32 of 40 byte-exact

All 32 src/ rows grade MATCH/RELOCATION_MATCH (grades.txt) and their behaviour tests print `<addr>_TEST PASS` under
g++ (tall.sh) and the lane's VC7.1 cl.exe (tall.py). Names follow Ego_r.pdb (`ghidra_out/egor_pdb_names.tsv`) where the
body could be tied to a PDB symbol; propagated catalog labels are listed under "Label corrections".

## What matched (32)
- CThingCreatureBase (18)
  - 00662620 IsPerformingASneakingIncompatibleAction: current action (+0x100) not finished (+0x61) and its slot-55 query; the PDB says `bool` but retail returns a full-eax 1/0, so the source returns `long`.
  - 00662650 FinishCurrentAction: the counted action pointer is COPIED into an inner-scope local (inc/dec of the count block, `dec [edi] ; jne` destroy + delete), the one-bit "finishing" flag set before and cleared after the scope.
  - 006626a0 SetHiddenOnMiniMap: `bool m : 1` bitfield compared with the bool argument byte-wise; the void refresh call on the global manager as the last statement becomes the tail jump with the argument slot reused (`mov [esp+4],0`).
  - 00662880 ConstructFromParams / 006628e0 SerialiseGameState / 00662930 ConstructFromLevelScript: base call, then `GetDefinitionManager()->GetDef(m_defIndex, &m_def)` with the arguments pushed before the accessor (chained call); the success path must be the `if` body (`if (!base) return false;` reorders the exits).
  - 00662970 NewCreatureInteraction(ECreatureInteractionType): `m_interaction.Reset(new CCreatureInteraction(type, *this))`, VC duplicates the out-of-line Reset into both arms of the allocation null test.
  - 006629b0 RemoveFromCreatureInteraction: `RemoveParticipant(*this)` then an inline `Reset(0)`-style release that re-reads the block member after the decrement and zeroes block then object.
  - 00664370 OnKill: `if (HasTC(0x31)) GetTC(0x31)->f(); CThing::OnKill();` with the pointer-returning inline find (component straight into ecx).
  - 006649c0 UnsheatheMeleeWeapon / 00664a10 UnsheatheRangedWeapon: two early returns (slot 77, the __fastcall carrying test), SheatheAllWeapons, UnsheatheWeapon(def->weapon, primary) with the def re-read per call.
  - 00664c60 IsAbleToRecoverFromStunned: the out-parameter GetInterface template (component comes back in eax, `mov ecx,eax`).
  - 00664cb0 GetMaxSneakingSpeed(const C3DVector&) (catalog "IsBlocking"): pointer-returning find, `return 0.0f` = `fld [0x0122DEDC]`.
  - 00664d00 IsBlocking: pointer-returning find + bool query.
  - 006a8880, 006a89e0, 006a8a30, 006c8c50 (catalog IsBlocking / IsAbleToRecoverFromStunned): these are `__thiscall` filter functors on `const CThing&` (callers load ecx with the filter). Out-parameter template where retail has `mov eax,[eax+4] ; mov ecx,eax`, pointer-returning find where it has `mov ecx,[eax+4]`; a call result returned as the last statement with a filter word as its argument tail-jumps with the argument slot reused. The out-of-line COMDAT copy of the GetInterface template is larger than the functor: the `// Leaf: ??RCThingFilter_<Name>` line is required.
- CThingDeadCreature (5)
  - 0052c6a0 (catalog WorldUpdateBulletTime) = STLport `vector<T>::resize(n) { resize(n, T()); }` for a 24-byte record {CCharString, vector, long, bool}: matched first try with an out-of-line CCharString ctor/dtor and a malloc-based inner vector destructor (`free` when non-null).
  - 006a80e0 (catalog WorldUpdateBulletTime) = CThingFilter_IsPerceivingThing: out-parameter template with the uninitialised component local (stale argument slot on the absent path), `f() == m_thing` as `sub ; neg ; sbb ; inc`.
  - 00835aa0 CreateFromDef (vtable slot 15): `m_flag = def.m_flag` between two one-bit `unsigned char` bitfields (`shl ; xor ; and 40h ; xor`).
  - 00835ac0 DrawPreparePrimitivesForRendering (slot 36): qualified base call, then the cached graphic-appearance component's prepare, `true` from the `if` body.
  - 00835c20 CThingDeadCreature(): inline CThingGameObject layer (vtable store, CIntelligentPointer<CThing> member = base ctor + vtable), then the derived init list in declaration order. The scoped_ptr must be a member CLASS with an inline constructor: a plain pointer initialiser sinks its zero store below `pop edi`. CThingGameObject is 0xd0 in retail (PDB 0xe8): its one flag byte at +0xc8 is followed by 7 bytes of tail padding.
- CThingDrawInformation (1)
  - 006bb810 SetBarPercentage: store, then clamp through an inline helper taking the MEMBER by reference (`ClampFloat(BarPercentage, 0, 1)`); a direct `if` compares the argument and loses the `fld [ecx+8]` re-reads.
- CThingFilter_* (8)
  - 006a70b0 HasDefGlobalIndex, 006a75d0 / 006c4560 AICreatureHasProperty, 00982db0 HasBuildingAsHome: `return f() == m_value;` with a bool return = `sub eax,[esi] ; neg ; sbb eax,eax ; inc eax` (the member is folded as a memory operand).
  - 006a74f0 HasAnyScriptName: named pointer to the +0x74 name (`add esi,74h`), `if (*name != "" && *name != "NULL") return true; return false;`.
  - 006a78d0 CanSeeEachOther: the creature down-casts are null-initialised locals assigned in an `if` (a ternary turns into branchless `neg ; sbb ; and`), the inline `see(a,b) && see(b,a)` helper gives the full-register 1/0 next to the caller's `xor al,al`.
  - 006a8a80 HasFreeActionPoint, 006a8d30 CanBeHitByEpicSpell: pointer-returning find + query.

## Parked for Fable 5.1 (8)
- 00663260 CThingCreatureBase::GetCombatAnimationSet DIFFER(59v49): `mov eax,[esp+4] ; ... ; mov edx,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edx ; test edx,edx ; mov [eax],edx` where retail has `mov ecx,[esp+4] ; ... ; dec [ecx+4] ; jne` | tried: CDefPointer destructor, local-copy release, release as a member of the counted object, the landed CDefBase (CDefVirtual + CRefCounted) model | next: the hand-written release on a pointer local (it fixed the release in 00662c70/00662c10), else lane gap: address-taken def-pointer release
- 00835b90 CThingDeadCreature::PeekDefinition DIFFER(70v60): the filled def pointer loaded into eax then copied to esi, release forms `&m_refCount` (`add eax,4 ; dec edx ; test edx,edx`) where retail loads esi and does `dec [esi+4] ; jne` | tried: template CDefPointer destructor, optimize("s"), a non-virtual function-pointer model, the landed CDefBase model, a free inline release | next: lane gap: address-taken def-pointer release (same as agent47's 006CDB50)
- 0083d900 CThingDeadCreature::PeekDefinition DIFFER(72v60): same as 00835b90 even with the hand-written release (`mov eax,[esp+4] ; test ; mov esi,eax ; ... add eax,4 ; dec edx ; mov ecx,edx`) | tried: raw release on a pointer local, the returned reference taken before the release | next: lane gap: address-taken def-pointer release
- 00662c70 CThingCreatureBase::HasAnim DIFFER(72v72): `mov ecx,[esp+14h] ; mov esi,[esp+8] ; mov edx,[esp+10h]` where retail loads the def into esi after both arguments | tried: CDefPointer destructor (re-read + split release), /Oa, hand release on a pointer local (scheduling only left), an inline const-reference accessor | next: lane gap: scheduling (argument loads vs the pointer local)
- 00662c10 CThingCreatureBase::GetAnimsWithNameStartingWith DIFFER(83v83): `mov edx,[esp+14h] ; mov esi,[esp+4] ; mov eax,[esp+10h]` where retail loads esi after both arguments | tried: hand release on a pointer local, the pointer captured inside the call's object expression | next: lane gap: scheduling (same as 00662c70)
- 006e3780 CTCTalk::GetPhonemeAnim DIFFER(79v77): the def pointer's zero store scheduled before `mov ecx,[ecx+70h]` and the filled pointer loaded into eax then `mov edi,eax` (retail `mov edi,[esp+8]`) | tried: hand release on a pointer local (no null test, matching retail's), the base pointer as a named local (fixes the prologue, breaks the registers), an inline GetBaseDef() accessor | next: lane gap: def-pointer load register (eax then copy)
- 00835bd0 CThingDeadCreature::PreloadDeadAnimation DIFFER(75v75): `sub esp,8` (a second frame word for the for_each functor result) and no `test esi,esi ; je` before the release (the lane drops it after the dereference) | tried: hand release on a pointer local, CDefPointer destructor (93 bytes: re-read + split release) | next: lane gap: hidden-result temporary sharing the dead def slot (QFE/RTM frame-slot sharing)
- 00733020 CThingFilter_HasBuildingAsHome::operator() DIFFER(27v27): `cmp edx,eax` where retail has `cmp eax,edx` (written `m_building == f()`) | tried: `f() == m_building` (folds to `sub eax,[esi] ; neg ; sbb ; inc`, 23 bytes), the reversed compare, a base-class pointer member, a reference member | next: a `bool` local or an inline compare helper taking both pointers by value, else lane gap: compare operand order

## Skipped
- none (no row holds two functions).

## Label corrections
- 00662c10 "CThingCreatureBase::HasAnim" -> CThingCreatureBase::GetAnimsWithNameStartingWith(const CCharString&, vector<const CAnimationEntry*>&) const (Ego_r.pdb order + the CAnimFilterNoGroup temporary).
- 00664cb0 "CThingCreatureBase::IsBlocking" -> CThingCreatureBase::GetMaxSneakingSpeed(const C3DVector&) const (Ego_r.pdb order, float result, the only caller passes a C3DVector).
- 006e3780 "CThingCreatureBase::GetPhonemeAnim" -> CTCTalk::GetPhonemeAnim(const CCharString&) const (reads this+4 = ParentThing; called from CTCTalk speech code).
- 006a8a30 "CThingCreatureBase::IsBlocking" -> CThingFilter_IsOccupiableOccupied::operator()(const CThing&) const.
- 006c8c50 "CThingCreatureBase::IsBlocking" -> CThingFilter_IsAlliedWithFaction::operator()(const CThing&) const (caller GetPBestThing<CFilter_Bind2<CThingFilter_IsNot, CThingFilter_IsAlliedWithFaction>>).
- 006a80e0 "CThingDeadCreature::WorldUpdateBulletTime" -> CThingFilter_IsPerceivingThing::operator()(const CThing&) const (callers GetAllTCsInRoughArea<CTCPerceiveThing, Bind2<IsPerceivingThing, IsNot>>).
- 006a8880 "CThingCreatureBase::IsBlocking" -> a filter functor (kind 0x57 membership query with the filter's word), named CThingFilter_IsInFaction (descriptive; real name unknown).
- 006a89e0 "CThingCreatureBase::IsAbleToRecoverFromStunned" -> a filter functor (occupiable kind 0xac IsOfType(4)), named CThingFilter_IsOccupiableOfType (descriptive).
- 0052c6a0 "CThingDeadCreature::WorldUpdateBulletTime" -> std::vector<24-byte record>::resize(size_type) (named CMarkerRecordVector::resize; the element is only known by layout).
- 00835aa0 "CThingDeadCreature::vfunc_15" -> CThingDeadCreature::CreateFromDef(const CThingBaseDef&); 00835ac0 "vfunc_36" -> DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&).
- 006c4560 "CThingFilter_AICreatureHasProperty" and 00982db0 "CThingFilter_HasBuildingAsHome" are second bodies of those shapes (slot 1 query / creature-definition word) with no direct callers: kept as catalogued, probably other filters.
- Callee labels seen propagated: 0x008631B0 is CCreatureInteraction::RemoveParticipant (not CTCInventoryAbilities::AddLearnedExpression); 0x006A1620 is the ranged twin of IsThingCarryingMeleeWeapon; 0x0052C630 is vector::resize(n, const T&) of 24-byte records; 0x008365F0 is a /Gr for_each (not CTCInventoryAbilities_PreloadDeadAnimation); 0x005296B0 is CThingPhysical's constructor (not CTCDPhysicalObstruction).
