# Hand chunk 388 (GFSerialiseVectorBinaryOut<CAttackHistoryCombo> .. vector<CProng>): 36 of 40 byte-exact

All 36 sources in src/ grade MATCH (3) or RELOCATION_MATCH (33) on their `// Leaf:` symbol, which resolves to exactly one
symbol in `objdump -t` of the built object. All 36 tests PASS under g++ (tall.sh) and under the VC lane (tall.py).
Every row ends in a `ret`. There is no noreturn tail (no int3-after-noreturn row), and no row looks cut wrong.
Sizes are the oracle bytes with the trailing 0xCC padding removed.

## What matched (36)

- **CHeroExperienceDef / CAttackHistoryCombo** (006dc550 GFSerialiseVectorBinaryOut, 006dca50
  vector::_M_insert_overflow, 006dd580 GFSerialiseVectorBinaryIn):
  - The landed serialiser templates, without the size pragmas.
  - In _M_insert_overflow, the tag-taking callees carry same-TU noinline bodies that never touch the tag. VC7.1 then
    homes every `__false_type()` temporary on the live `atend` argument slot.
- **NParticleEngine::CParticleEmitter** (006dfda0 SetParticleCreationDisabled): an out-of-line copy of the inline
  bool-to-bit-field setter. It does a 16-bit read-modify-write: `and eax,0FFBFh`, merged with `shl edx,6`.
- **CTCDParticleEmitter** (006e0970 Create(const CCharString&,...), 006e0e80 SendCreationCriteriaSound, 006e0f10
  SendDestructionCriteriaSound):
  - The name is pushed before the database getter: the arguments go first, then the object expression.
  - The driver is a named local, read before the interface test.
  - A chained `GetPItem()->Name.ToUpper()` pushes the ToUpper result slot first, so the live emitter is fetched into a
    named local instead.
  - The sound name is a named local in its own block, so it shares the lookup key's frame word.
- **CTCTalk** (006e2860 GetPLipSyncBank, 006e2940 GetSpeechDurationInFrames, 006e29a0 PlayLipMovement, 006e2d30
  IsSpeechSamplePlaying):
  - A zero-initialised result returned once at the end preloads `xor eax,eax` beside the map test. An early
    `return 0` returns the tested global instead.
  - `p ? p : GetDefault()` keeps `this` in ecx for the getter.
  - `fild ; fmul [eax+4]` for `duration * fps`.
  - The unchecked out-parameter GetInterface: its absent arm reads the lookup key's frame word.
- **NSpeechGainManager** (006e2b70 GetTargetGain, 006e37d0 Update):
  - `float gain = 0` is loaded before the switch, and every case drops it with `fstp st(0)`.
  - `!(gain > 0.0f) || ...` gives `fcom ; test ah,41h ; jne`, where `gain <= 0.0f` gives `jnp`.
  - In Update, a second `next` iterator is stepped before the body. When the element reports finished, it is replaced
    by STLport's inline `erase(it)`: unlink, destroy, free.
- **vector<SPhonemeAnimRecord>** (006e4950 _M_allocate_and_copy, 006e4f30 reserve): STLport 4.x verbatim. The 44-byte
  implicit copy is `rep movsd`, in a speed TU.
- **std::__copy_ptrs** (006e1f40): STLport `__copy_ptrs` → counted `__copy` with a 16-byte stride. The third store is
  strength-reduced to `[esi+edx]`.
- **CScriptConversation / CScriptConversationManager** (006e58a0 AddLineToConversation, 006e60f0 FrameUpdate, 006e6150
  ctor, 006e66e0 std::__find, 006e6d30 vector<CConversationLine>::push_back):
  - AddLineToConversation:
    - A same-TU noinline `__find` stand-in plus a tag with a user ctor. Together they home the `const CThing*`
      temporary in the speaker argument's own slot.
    - The line is an inline ctor temporary.
    - A STLport model is required: retail Conversation is at +0x14, but engine/CScriptConversation.h has +0x18
      (Dinkumware 16-byte vectors).
  - FrameUpdate:
    - Takes `CScriptConversation& c = it->second; ++it; c.FrameUpdate();` (lea before the inline `_M_increment`).
    - The ctor uses the STLport `_Rb_tree_base`/`_Rb_tree()`/`_M_empty_initialize` model.
  - `__find`: the SGI unrolled find. Its `operator==` takes the intelligent pointer by value and calls GetPItem.
- **CGameScriptInterface** (006e7370 StartFadeScreen, 006e75c0 WorldUpdate):
  - StartFadeScreen: the CRGBColour by-value argument is built by its user copy constructor (Set(R,G,B,A)) in the
    argument slot. The floats are passed swapped.
  - WorldUpdate: the fps global is held in a local across the frame call. The virtual slots are 386 (IsInCutscene) and
    395 (IsInMovieSequence). The out-of-line map iterator `++` lives in the dead `active` slot.
- **Combat prong selection** (006e8e10 GetFirstAttackerAlignedWithPlacementVector):
  - `(float)cos(60deg)` through the intrinsic.
  - The dot product is written `a.Z*b.Z + (a.X*b.X + a.Y*b.Y)`. This is the term order retail evaluates.
- **CDefendingCombatantInfo::CCombatWheel** (006e9200 SetAttackerProngAsBlocked): the inline GetRing (signed index test,
  unsigned size test, null-tested element). VC duplicates the trailing `= -1` store into the negative-index exit.
- **CCombatManager** (006e93c0 IsCreatureOnRotatingCombatRing, 006e9680 GetCombatWheelRadiusForAttacker, 006e9700
  IsCreatureOnOuterRing, 006e97a0 IsCreaturePerformingNonInterruptableSequence, 006e9d10 SetAttackerProngAsInvalid,
  006ed3f0 ctor):
  - A same-TU `__declspec(noinline)` body of the combatant `map::find` (0x006F0250) makes the inlined lookups' GetUID()
    key temporaries share one frame slot. The body must be a real algorithm that does not pass the key on.
  - The victim's UID is taken into a local before the ring index is tested.
  - `IsRotating` is `(float)fabs(d) > eps`, which gives `fcomp dword ; test ah,41h`.
  - IsCreatureOnOuterRing:
    - Uses one `if (eligible) {...}` block with `return true` arms and a single final `return false`. Early returns
      shrink-wrap the pushes.
    - Keeps the find's node pointer, not a reference to `->value`, which gives `lea ecx,[ebx+58h]`.
  - IsCreaturePerformingNonInterruptableSequence returns `unsigned char`, which gives the byte `neg al ; sbb al,al ; inc
    al`. A raw pointer local gives the `mov`, not a `lea`.
  - Ctor:
    - Two STLport map members (header nodes 0x18 and 0x88), member initialisers with the zero in ebx, and an
      out-of-line CombatDialogueManager ctor, then RegisterCombatSequences().
    - The dialogue manager model needs 4-byte alignment, or its address is `lea ecx,[esi+35h]`.
- **STLport combat containers** (006eff00 __uninitialized_fill_n<pair<C3DVector,bool>*>, 006f0810 vector<C3DVector> copy
  ctor, 006f1180 __uninitialized_fill_n<CProng*>, 006f1ae0 _Rb_tree<uint64, pair<const uint64, CCombatantInfo>> ctor,
  006f20a0 vector<CProng> copy ctor):
  - STLport 4.x verbatim, with `__fastcall` explicit instantiations for the /Gr helpers.
  - C3DVector is 1-aligned in this module (pack(1), as the landed 0x006F14A0 / 0x006EB8A0). That gives 13-byte
    `pair<C3DVector,bool>` and CProng::Prong at +1.
  - The vector copy ctors use `_Vector_base(n, a)`, then `allocate(n)` (null when n == 0), then `__uninitialized_copy`
    (inline for C3DVector, out of line 0x006F14A0 for CProng).

## Parked for Fable 5.1 (4)

- 006e0880 CTCDParticleEmitter::Create DIFFER(218v228): retail keeps the NULL result in edi and the map number in esi (thing in esi, its own `mov eax,edi` epilogue for the no-map return); ours swaps esi/edi and merges the two returns | tried: plain early return; thing zeroed before the map lookup; `long map; CThing* thing = 0;` + if-null emitter; a separate result variable (constant-folded) | next: the same variable returned inside the if and again at the end (multi-definition, not folded), else lane gap: register allocation
- 006e5990 CScriptConversationManager::RemoveConversation DIFFER(102v98): retail copies the found node into esi before the end() test and keeps it across OnCleanup (this in edi); ours keeps the iterator in find's hidden-result frame slot and reloads it after OnCleanup (esi pushed late) | tried: plain `iterator it = find(k); if (it != end()) { it->second.OnCleanup(f); erase(it); }`; assignment form (96v98, reads via [eax], registers swapped); inline map::find with an iterator copy ctor; a derived base-iterator | next: lane gap: register allocation (iterator lifetime across a call)
- 006e7f00 GetAttackerProngSelectionCompareValue DIFFER(205v207): only the first (placement) normalise differs: retail `fld st(0) ; fmul st(2)` keeps the old X on the x87 stack until `fstp st(1)` after Z, ours `fxch st(1) ; fmul st(1)` | tried: sequential copies; both flattened copies up front; `X = f * X` + the Z-first dot (the dot then matched); `v = v * inv` (263, worse) | next: lane gap: x87 stack scheduling (the prong normalise of the same source shape matches)
- 006e9f60 GetAttackerProngSelectionCurrentWeaponBaseCompareValue DIFFER(236v236): the two result arms are laid out swapped (retail `cmp eax,5 ; je TRUE` / `test al,al ; je FALSE`, ours `jne`/`jne` with the fld constants exchanged); retail reads the combat component via eax (ours ecx) and forms the carrying key address in ecx (ours edx) | tried: non-forced inline helper (31 bytes, not inlined); `__forceinline` helper (221); + HasTC returning the masked word + per-arm returns (233); + a real same-TU LowerBound body + `if (type == 5) return true; return false;` + the not-projectile arm first (236) | next: the projectile arm first with the threaded type test, and the weapon type read without a CTCCombat* temp

## Skipped

None.

## Label corrections

All of these are catalog or manifest labels; the corrected name is on each source's line 1.
- 006dc550 / 006dd580: `GFSerialiseVectorBinaryOut<CMusicEntry>` / `GFSerialiseVectorBinaryIn<CBrainUpdateZone>` are
  propagated. The element is the 16-byte CAttackHistoryCombo (CHeroExperienceDef TU).
- 006dca50 `CFeatDef_Vector_Resize` → `vector<CAttackHistoryCombo>::_M_insert_overflow`.
- 006dfda0 `FrameUpdate_SetAnimationFlag` → `NParticleEngine::CParticleEmitter::SetParticleCreationDisabled`. The
  landed FrameUpdate calls it SetHidden.
- 006e1f40 `_Move<C3DAnimationStateAlpha*,...>` → STLport `__copy_ptrs` over 16-byte param-desc pairs.
- CTCTalk rows:
  - 006e2860 `CAnimateThing_FindMeshSymbol` → `CTCTalk::GetPLipSyncBank`.
  - 006e2940 → `CTCTalk::GetSpeechDurationInFrames`.
  - 006e29a0 → `CTCTalk::PlayLipMovement`.
  - 006e2d30 `CAnimateThing_GetSpeechSourceHeight` → `CTCTalk::IsSpeechSamplePlaying`.
  - Callee 0x00686D70: the manifest's "CAnimateThing::GetMeshBank" is propagated (it is the default speech bank
    getter).
- 006e4950 / 006e4f30 `Array_CopyElements_0x2c` / `AnimInit_Vector_Resize` →
  `vector<SPhonemeAnimRecord>::_M_allocate_and_copy` / `reserve`.
- 006e58a0 `CConversation_AddLineToConversation` → `CScriptConversation::AddLineToConversation`.
- 006e5990 `CScriptConversation_RemoveAndCleanup` → `CScriptConversationManager::RemoveConversation`.
- 006e60f0 `UpdateFrame_IterateConversations` → `CScriptConversationManager::FrameUpdate`.
- 006e6150 `List_Initialize_Empty` → `CScriptConversationManager::CScriptConversationManager()`.
- 006e66e0 `BinarySearch_LinearArray` → `std::__find<CIntelligentPointer<CThing>*, CThing*>`. It is a linear find.
- 006e6d30 `ConversationLine_PushToVector` → `vector<CScriptConversation::CConversationLine>::push_back`.
- 006e7370 `ConstructString` (forge `NGameText::ConstructString`) → `CGameScriptInterface::StartFadeScreen`.
- 006e75c0 `UpdateFrame_ProcessRegions` → `CGameScriptInterface::WorldUpdate`.
- 006e7f00 `Math_DotProductNormalized` → `GetAttackerProngSelectionCompareValue`.
- 006e9200 `CDefendingCombatantInfo_BlockAttackerProng` → `CDefendingCombatantInfo::CCombatWheel::SetAttackerProngAsBlocked`.
- 006e93c0 `C3DAnimationBlendState_ProcessCombatRing` → `CCombatManager::IsCreatureOnRotatingCombatRing`.
- 006e9680 `Combat_ProximityCalculation` → `CCombatManager::GetCombatWheelRadiusForAttacker`.
- 006e9700 `CCombatWheel_ValidateSequencePosition` → `CCombatManager::IsCreatureOnOuterRing`.
- 006e97a0 `CombatSequence_CheckActionPending` → `CCombatManager::IsCreaturePerformingNonInterruptableSequence`.
- 006e9d10 `CombatBlend_SetupDefenderBlock` → `CCombatManager::SetAttackerProngAsInvalid`.
- 006ed3f0 `CombatSequenceManager_Ctor` → `CCombatManager::CCombatManager(CWorld&)` (vtable 0x01261EB4, RTTI
  `.?AVCCombatManager@@`).
- 006eff00 `_Fill_n<unsigned_long*,unsigned_int,unsigned_long>` → `std::__uninitialized_fill_n<pair<C3DVector,bool>*,...>`.
  The stride is 13.
- 006f0810 `Vector_CopyFrom` → `vector<C3DVector>::vector(const vector&)`.
- 006f1180 `fill<C3DAnimationStateAlpha*,C3DAnimationStateAlpha>` → `std::__uninitialized_fill_n<CProng*,...>`.
- 006f1ae0 `TreeHeader_InitSentinel_0x88` → `_Rb_tree<unsigned __int64, pair<const unsigned __int64, CCombatantInfo>, ...>`
  ctor (comp, a).
- 006f20a0 `vector_copy_CTargetPoint` → `vector<CDefendingCombatantInfo::CProng>::vector(const vector&)`.
- Callees:
  - 0x006E8850 is `CCombatWheel::GetRingRadius(long)`. The manifest has "Array_SumFloatsStride"; it was landed as
    "SumPrefixWeights".
  - 0x0059299D is the STLport map iterator `operator++`.
  - 0x0049D870 is GFGetCurrentFrame.
  - 0x00AEAAE0 is `CParticleEmitterDatabase::Get()` (the manifest's GFGetPreMainMemoryUsed is a byte twin).
  - 0x00416225 is the main game component's world-frame getter (the manifest's CWorld::DrawGetEnvironment is a byte
    twin).
  - 0x004FB4A0 is `CWorldMap::IsLoadingAnyMap() const`. The manifest's `_Cons_val<...CParticleMorphs::CEntry...>` is
    propagated.
  - 0x004F963D is the ICF-shared `vector<16-byte>::_M_clear()`. The manifest's `CActiveFile::OnReadFinished` does not fit
    (it destroys and frees a 16-byte-element range).
  - 0x006F0810 is the vector<C3DVector> copy constructor. The landed 0x00764970's anonymous `CElement12_006f0810` is
    therefore vector<C3DVector> (an __uninitialized_copy over vector<vector<C3DVector> >).
