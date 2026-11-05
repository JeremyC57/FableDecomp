# Hand chunk 782 (CTCUnholyPower .. NScript::CQ_TraderEscortScript): 4 of 6 byte-exact

All 4 matched rows are RELOCATION_MATCH. vland (read-only, with the overlay) finds no RELOC_IDENTITY. It warns STANDIN_LEAF on
0x00B5F090 and 0x00E00DB0 only because the catalog names are wrong (see Label corrections). Every test passes under g++ and
VC7.1, the differ/ tests included.

## What matched (4)
- **0x007941D0 CTCUnholyPower::CTCUnholyPower(CThing&)**: the unholy-power spell component ctor (8 grades).
  - Shape: the init list in member order, with SpellExclusionChannel built by the EEE ctor. Then the body:
    - `HasTC(167)` with the forced-inline `GetTC(type)` straight on Components (folded `cmp [eax],0A7h`), then `LockAbilitiyMutex(19)`
      (Ego_r spelling, with its typo).
    - GetGlobalIndexFromClassIndex / GetDef<CSpecialAbilitiesUnholyPowerDef>, then PeekTC(111) GetAbilityLevel.
    - Three `(long)(ConstantFPS * def[level])` conversions.
    - The theme loop over channels 0..5 that skips 5.
    - PeekLightingChannel / SetLightingChannel.
    - `GetTC(117)` SetUpdateLightingChannels(false).
    - The stamina split, then the effects table. The table is a function-scope CDefPointer plus **function-scope small / large ring
      indices zeroed before GetSubDef**. The hand-effect and dark-faces indices are **block locals**: they reuse the large index's
      slot, and the hand temporaries reuse the small one's.
    - The start and looping sounds through PlaySoundW.
  - Two same-TU bodies were needed:
    - The GetSubDef<T> member template body (the landed 0x00669580 one). Without it the effects def is re-read from its slot at
      every use.
    - The GetEffectFromString noinline body (overlay). Without it the hand index is re-read from memory between the two
      CreateParticleEffect calls.
  - The slot layout came from scoping: all-function-scope locals give a frame of 0x28, every index in a block gives 0x20, and the
    mix above gives retail's 0x24.
- **0x00B5F090 CEngineScreenEffectGlowRenderer::CEngineScreenEffectGlowRenderer(const CEngineInit&)** (first grade):
  - shared headers with the new GLOWRENDERER_MEMBERS opt-in;
  - `init.InitFlags & 0x80` decides EffectSupported;
  - nine shader assignments, each through `g_pResourceManager->GetVShaderResource((EShaderBankNo)13)->GetVertexShader(..)` or
    GetPShaderResource()->GetPixelShader(..), in retail's order (Copy, Blur, GlowNeg, Glow, Blur, ApplyGlow x2, ShowDebug x2);
  - InitialiseTextures, then the four texture pointers cleared;
  - per-field vertex stores quad by quad (half-texel offsets 1/256 and 1/64);
  - a `for (long i...)` loop for the white colours;
  - the thresholds / intensities / motion blur after the if.
- **0x00E00DB0 NScript::CQ_TraderEscortScript::Main** (first grade): the SecretPassage quest-Main model on
  engine/CQ_TraderEscortScript.h + fable_script_def_global.h, /Gr.
- **0x00EB4760 NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::DoDealWithHero** (2 grades): the sheriff dialogue model
  from chunk 746 on the ASSASSIN opt-in. `inline CScriptThing::~CScriptThing() { }` after the include lets VC7.1 expand only the
  first (hot) GetHealth temporary, as retail does.

## Parked for Fable 5.1 (2)
- 0x00EA1720 NScript::CV_KnotholeGladeGatesScript::DoInitialBalverineAttack DIFFER(1940v1940): retail hoists `lea ecx,[esp+68h]` above the balverine-wait terminate `jne` at 0x00EA1CB2 / 0x00EA1E41 (target 0x00EA1E60, past the exit block's lea); VC7.1 jumps to 0x00EA1E5C and keeps the lea in the latch (4 diff lines, same residue as chunk 781's 0x00D6A560) | tried: while, for(;;)+break, if+do-while, `== true`, by-value + const-ref alias, non-const ref, scoped alias, if(!term) Unset tail, no /Gr (10 grades) | next: a different exit shape for the pre-balverine returns, or a block-count change earlier in the function, so that the latch fall-through starts with the lea.
- 0x00DFD600 NScript::CQ_TraderConflictGoodScript::AttackPeople DIFFER(2456v2088): VC7.1 makes different per-site inline / exit-merge decisions. Retail calls ~CScriptThing 0x004AA840 out of line at the null-temporary and loop-end sites, expands it only at the break site, and merges the four trader-alive terminate returns into one exit block (0x00DFDC54) and the two timer-block returns into another (0x00DFDC8A). VC7.1 expands the dtor at the temporary and at the first exits and emits one cleanup per return (+368 bytes); the hero arm also differs in register choice | tried: all-inline headers, the out-of-line erase opt-in, __forceinline CTimer::operator= / GetTime, a named null local, out-of-line ~CScriptThing (body matches to 0x00DFDABB), plain-inline operator= [best, 184 lines], while+break head, const-ref enemy alias, __forceinline ~CScriptThing (10 grades) | next: a retail spelling where the break-site object is a distinct inner-scope local, or where the erase loop sits in its own scope, so that the inline budget lands on that site only; otherwise "lane gap: inline-budget site choice".

## Skipped
None.

## Label corrections
- 0x00B5F090: catalog `CEnginePrimitiveManagerDecal::CEnginePrimitiveManagerDecal` -> CEngineScreenEffectGlowRenderer ctor
  (vtable 0x012A27CC, CEngine::Initialise 0x00B269FD after new 0x280, glow shader literals, PDB).
- 0x00E00DB0: catalog `NScript::CV_TravellingHeroesScript::Main` -> `NScript::CQ_TraderEscortScript::Main` (slot 2 of the
  vtable 0x012DF16C, whose other slots are landed as CQ_TraderEscortScript).
- 0x00EB4760: catalog `NScript::CMurderTwistAssassin::DoDealWithHero` lacks the enclosing `CV_MurderTwistFinalScript`.
- 0x007940F0: landed `CTCDivineWrath::PlaySoundW(unsigned long, unsigned long)` is CTCUnholyPower::PlaySoundW(const CCharString&,
  bool). All six callers are in the CTCUnholyPower TU; CTCDivineWrath's own copy is the byte twin 0x0078EBF0.
- 0x004D4ACC: catalog `CTCUnholyPower::Construct` -> `CTCUnholyPower::Alloc` (Ego_r).
- 0x00669D30: landed file `CCRC_CalcAndLookup` -> CSpecialEffectsDef::GetEffectFromString (Ego_r).
- 0x00B5ECE0 and 0x00DFDE30: placeholder spellings (see NAME_FIXES.tsv).
- Full list with evidence: LABELS.tsv and NAME_FIXES.tsv (13 symbol fixes for the callees these rows bind).

## Names: proven vs descriptive
- Proven (Ego_r / FableWin PDB name sets plus a second source: vtable/RTTI, the caller, literals or donor order): all six row names
  and every callee symbol in the matched rows, plus the CTCSpecialAbilities members (AbilityMutexLock +0x0C, CurrentAbility +0x10,
  LockAbilitiyMutex) and the CSpecialAbilitiesUnholyPowerDef members.
- Descriptive:
  - g_RightHandAttachmentName 0x013BAC30 / g_LeftHandAttachmentName 0x013BAC34: the landed bound spellings, kept; not in either PDB
    name set.
  - Local variable and parameter names.
  - The CTCDParticleEmitter and CTCSpecialAbilities local models: Ego_r member functions, but no engine header exists for either.

## Header debt
- CTCUnholyPower has no engine header. Both the landed dtor 0x00794AC0 and this ctor model it locally with the same Ego_r layout,
  so an engine/CTCUnholyPower.h (FABLE_RETAIL_CTCUNHOLYPOWER) would serve both.
- CTCSpecialAbilities and CTCDParticleEmitter have no engine header either. This row models them in place with the members and
  methods it needs; a header should take: CTCSpecialAbilities (Ego_r 0x38, with list / vector members at retail sizes) and
  CTCDParticleEmitter (0x48, PEmitter +0x14, as the landed 0x006E0590 notes).
- NGraphicAppearance::CTCGraphicAppearance still keeps +0x0C..+0x3F opaque in the new opt-in.
