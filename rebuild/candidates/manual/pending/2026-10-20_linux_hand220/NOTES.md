# Hand chunk 220 (CAIStateGroup_VillageTaskTorch .. CCombatRing): 22 of 40 byte-exact

## What matched (22)
- **STLport vector::operator= (4 rows, all labelled "CActiveFile::*")** -- 0x00454467 vector<CDegradableInfo> (CDegradableDef),
  0x00461017 vector<CReverbEnvironmentEntry> (CSoundDef), 0x004E5F70 vector<CEngineGraphic> (CReplaceableMeshDef::Meshes),
  0x00471CC5 vector<COpinionDeedReactionDef> (COpinionDeedEffectsDef, non-POD element). Shape: the STLport 4.x body verbatim
  (`__xlen > capacity()` -> _M_allocate_and_copy + _M_clear; `size() >= __xlen` -> __copy; else __copy + __uninitialized_copy)
  with the helpers `__forceinline` (the size TU emits them out of line otherwise); the non-POD `_Destroy` tag must be a NAMED
  `__false_type` local (a `__false_type()` temporary is zero-filled with `stosb`).
- **CActiveFile copy ctor 0x009C72B0** (CFileInstaller flavour) -- memberwise copy; STLport list copy = `_List_base(alloc)` +
  `insert(begin(), x.begin(), x.end())` -> `_M_insert_dispatch`; `iterator` has a user copy ctor (built in its slot),
  `const_iterator` only the converting one; CCompressorZlib member at +0x38 (retail 0x58).
- **CBitstream::WriteBits 0x00C90FC0** (libpk, `__fastcall`) -- the count PARAMETER is the running bit total
  (`count += m_bitPos`), byte advance computed before the bit-position store.
- **CCharString::ToLower 0x0099ED10** -- copy + MakeOwnCopyOfData + loop + return copy; same-TU UnassignString body under
  `auto_inline(off)`; the loop condition `i < result.GetLength()` (inline null-tested getter) keeps retail's non-rotated loop.
- **CLight::SetSlotEnabled 0x00C630D0** (label "CCharacter::SetSkillPropertyEx") -- EAX 3.0 slot toggle; `(Flags & 1) == 1`
  gives the 8-bit `dec/neg/sbb/inc`; direct `m_slots[slot]` indexing; own `return 0` in the enabling arm; store order
  Room, HFReference, AirAbsorptionHF.
- **CCS_OakValeRevisitedScript ctor 0x00EE8100** -- the landed CV_ChickenKickingScript model (two STLport vectors, a map, 7
  scripted-thing handles).
- **CCombatActionBase ctors 0x00857F30 / 0x00858030** -- the landed 0x00857E40 sibling model with the 0x00693C70 base call;
  PTarget assigned from a CIntelligentPointer temporary.
- **CCombatAbilityData::TransferBinaryIn 0x0044FADC** -- three inline 4-byte persist reads through a `__forceinline` template
  `TransferIn<T>(stream, member)` (local in the dead stream slot, floats through eax) + CDefString::TransferBinaryIn.
- **CAS_Trickster::Init 0x00E2BD80** -- CGameScriptInterface slots from cgsi_slots_aligned.tsv; by-value CScriptThing via the
  IMPLICIT copy ctor over a CCountedPointer member (a user CScriptThing copy ctor reads through the cached &Me); C3DVector by
  value through a memberwise USER copy ctor.
- **NUISystem::CChangingStateComponent::ForceState 0x0052CAF0** -- inline STLport list::clear; `CurrentState = TargetState =
  RequestedState` stores TargetState first.
- **CBankFileManager::ReadQuotedString 0x009A77D0** -- landed ReadUntilCharacter stream model; nested success path.
- **CBriarRose::DoOnHitSpeech 0x00DA26B0** -- the landed DoOnSpeakToSpeech model + `rand() % 4 == 0`.
- **CBankFile::OpenFileReadWrite 0x009D1160** -- the landed OpenFileReadOnly model; stores written ReadOnly, OpenFlags, Alignment.
- **CCombatAction_GenericStrikeResponseKnockdownRecoverBase::FinishAction 0x00866E80** -- ScorpionRecover model + the ONE-ternary
  GetInterface (component straight into ecx) + an inline `GetBody()` accessor for the virtual calls.
- **CAIStateGroup_WorkAtSchool::IsRunnable 0x00942A00** -- StayNearSchool model; `HasTC` must be `__forceinline` (two lookups).
- **CAIStateGroup_WanderAroundShops::InitCreaturePosition 0x009473E0** -- `&&` chain ending in `(Shop = SelectRandomShop())
  .GetPItem() != 0` (IMPLICIT conversion temporary: construction flag in ebx after the ctor); named creature locals per arm.
- **CSequencePlayer_00c36ea0::ExecuteAttackSequence 0x00C36EA0** (class unidentified) -- inline getters returning a NAMED local
  (no NRVO: temp + copy), `__fastcall` key builder with an out pointer, 32-bit restart flag.
- **CBankFile::PrepareDataForInclusion 0x009CC9F0** -- local CDiskFileWin32 over a POLYMORPHIC CBase (else the base sits at +4);
  STLport vector<unsigned char>(n) = allocate + memset intrinsic; the source through a counted pointer's inline `operator->`.

## Parked for Fable 5.1 (18)
- 0099ee20 CCharString::ToUpper DIFFER(277v278): `mov eax,[esi] ; add eax,edi ; mov al,[eax]` vs ours `mov ecx,[esi] ; mov al,[edi+ecx]` (+ ecx/edx swap) | tried: char local, `*(p+i)`, GetChar reference, unsigned-char cast | next: an inline helper returning `const char*` to the character (pointer value CSE), or lane gap: address form
- 0099ba70 CWideString::Format DIFFER(265v265): vector zero stores placed before wcslen but knowledge kept (pushes 0 / no size()) vs ours after the call / reloads | tried: vector before/after wcslen, CRT prototypes | next: vector declared first with a `long size` copy of `wcslen` held in a different statement order, or lane gap: store hoisting
- 009cd140 CBankFile::FindIndexByPathname DIFFER(267v268): retail keeps vector _M_start in ebp across _stricmp, spills count | tried: count local, size() in condition, cached begin pointer | next: `const CArray<unsigned long>& indices = ...` (reference-bound temporary) or lane gap: regalloc
- 008c4b80 CCombatAction_StrikeResponseScorpionKing::Initialise DIFFER(304v302): two-word frame (facing/effectType/strings share words), def slot straight into ebp | tried: function scope, block-scoped facing, nested scopes | next: effect lookup inside an inline helper returning the raw def (slot block-local), or lane gap: frame packing
- 009a7f80 CBankFileManager::FindContainedBank DIFFER(271v270): one `mov eax,ebx` before the shared copy-ctor push (retail `push ebx` in the arm) | tried: both ternary orders, inline ResolveAlias, prvalue ternary | next: pass `(alias != end) ? &alias->second : &name` and copy `*p`, or lane gap: ternary arm push
- 009d5bf0 CBankFileAsync::HandleFailedAllocation DIFFER(305v314): `this` in esi/spilled vs ebp, count ebp vs edi, lock frame order, `mov cl ; test cl` flag reads | tried: member and accessor flag reads | next: lane gap: regalloc / frame order
- 009f05a0 CCodeSectionManager::RegisterSection DIFFER(300v300): `this` edi / handle ebp swapped | tried: address-taken handle (315), `long` handle (temp at call), const handle, separate key local (this file) | next: lane gap: register swap
- 005dcb00 CAnimationSet::GetAnimsWithNameStartingWith DIFFER(253v255): GetString temporary not homed in the dead `out` slot (4th frame word), strncmp arg registers | tried: StartsWith helper (kept), cached out local, unnamed overflow tag | next: lane gap: temp-into-dead-arg-slot choice
- 0093e8c0 CAIStateGroup_VillageTaskTorch::IsRunnable DIFFER(320v318): `lea ecx,[eax+8]` vs `mov ecx,eax ; add ecx,8` for the task's torch pointer | tried: named appearance local, uninitialised orientation, GetCreature accessor, base-pointer member | next: make GetPItem return CVillageTaskTorch* directly (no void* cast through GetTask)
- 0096a7a0 CAIStateGroup_WhisperRetreat::IsRunnableLeaf DIFFER(263v270): 0x10 frame with no zero register in retail vs ours 0x14 + `xor ebx,ebx` zero register | tried: function-scope def, inline GetRetreatDef helper | next: lane gap: frame packing / zero-register heuristic
- 0096ae50 CAIStateGroup_WhisperRetreat::UseWill DIFFER(262v258): counted-pointer copy fields loaded before the outer `push 0`, target/creature evaluated before the constant pushes | tried: copy ctor testing o.Info | next: build the follow-up pointer argument through an inline helper returning by value, or lane gap: argument scheduling
- 00c71f80 CAudioSource::ComputePitchScale DIFFER(311v311): final flags/state reload order (eax first vs ecx first) | tried: both operand orders | next: lane gap: scheduling
- 00a5b260 CCachedFile::Open DIFFER(265v264): bytes identical + the int3 VC emits after the final _CxxThrowException (oracle strips trailing CC) | tried: n/a | next: lane gap: grader trims retail int3 but not the build -- land by hand
- 00859660 CCombatActionBase::ResetMeleeCombatAbilityEffects DIFFER(317v311): effect def kept in edi (pushed at entry) and re-read once after the speed block vs ours eax re-reads | tried: local, slot-> everywhere, releasing CDefPointer dtor | next: lane gap: regalloc of a memory CSE
- 00858cc0 CCombatActionBase::OnBlowBlocked DIFFER(321v317): `this` in ebp / &PTarget esi / &PCreature edi vs ours this spilled | tried: explicit tests, IS_THING_ALIVE helper | next: lane gap: regalloc
- 006ed8c0 CCombatManager::DeregisterAttackedCombatant DIFFER(242v242): pointer temporary not sharing the key slot (0x10 vs 8 frame) + ebx/ebp swap | tried: reference/pointer record, pointee Release (fixed the release) | next: lane gap: temporary slot sharing
- 006eb8a0 CCombatRing::DebugGetAllProngVectors DIFFER(247v253): retail materialises the scaled vector (fst/fstp Y,Z) in a 0x34 frame, ours forwards all three on the FPU (0x28) | tried: inline return, named result, user copy ctor, named scaled local | next: lane gap: x87 temporary materialisation
- 0070d780 CAnimateThing::AddCombinationAnim DIFFER(291v305): InternalAlphas begin kept in ebp through the final free (pushed at entry) | tried: front() reference, cached pointer, forceinline push_back | next: same-TU CAnimSet copy ctor body under auto_inline(off) (callee knowledge), else lane gap: regalloc

## Skipped
- none (no row in this chunk holds two functions).

## Label corrections
- 0x00454467 / 0x00461017 / 0x004E5F70 / 0x00471CC5 ("CActiveFile::operator= / CopyAssign_84 / operator_assign / AssignOpinionDeeds"):
  vector<CDegradableInfo> / vector<CReverbEnvironmentEntry> / vector<CEngineGraphic> / vector<COpinionDeedReactionDef>::operator=
  (callers: the slot-19 Copy of CDegradableDef 0x00454434, CSoundDef 0x00460A61, CReplaceableMeshDef 0x004E5F51,
  COpinionDeedEffectsDef 0x00471A77).
- 0x009C69C0 / 0x009C6A10 ("CActiveFile::CopyList_20byte/_12byte"): list<CPendingWrite> / list<CActiveRead*>::_M_insert_dispatch.
- 0x00471DF4 (landed as "global_OnReadFinished", described as a reader notify): vector<COpinionDeedReactionDef>::_M_clear
  (`__destroy_aux` 0x00450AEA on [start, finish) then free).
- 0x0099BA70 ("CCharString::SetFromFormatV"): CWideString::Format(CWideString*, const wchar_t*, ...) (wide twin of 0x0099F1F0).
- 0x00C630D0 ("CCharacter::SetSkillPropertyEx"): the EAX slot enable setter of the "CLight" EAX block (landed 0x00C631E0 family).
- 0x00C36EA0 ("CActionSummonerLightningAttackTarget::ExecuteAttackSequence"): propagated; all its callee labels are unrelated
  (class unidentified, landed under a placeholder).
- 0x0044FC00 ("CPersistTraits<float>::TransferBinaryIn"): CDefString::TransferBinaryIn(CMemoryDataInputStream*).
- 0x009F1470 ("CCodeSectionManager_RegisterSection"): map<unsigned long, CSectionDesc>::operator[].
- 0x00946F90 ("CAIStateGroup_WanderAroundShops_SelectRandomEnemy"): the shop picker of WanderAroundShops.
- 0x006F3260 ("Vector_InsertRepeatElements") / 0x005E1420 ("VectorInsertSequence"): STLport `_M_insert_overflow`.
- 0x00956590 (GetSubDef<CAICreatureWillPowerIndicatorDef>): the def read at +0x3c is beyond that 0x2c-byte def -- the template
  argument is doubtful.
