# Hand chunk 204 (CSecretWhisperingMan .. CSystemManager): 24 of 40 byte-exact

1 MATCH + 23 RELOCATION_MATCH, 15 DIFFER, 1 SKIP. Every src/ row re-graded with gall.sh at the end of the run.
Every test passes under g++ (tall.sh) and VC7.1/Wine (tall.py). A relocation-identity pass (reloc_identity.check run
over the q.py objects) found no HARD findings.

## What matched (24)
- CSecretWhisperingMan (0x00EBFC10, OnPredicateFail, entity-script slot 3): `if (Me.MsgIsKilledBy(""))` with an implicit
  "" temporary (the bool rides in bl across its destructor), then four implicit literal temporaries for
  SetQuestCardObjective built right to left. Needed filler virtuals so MsgIsKilledBy sits at slot 18 and
  SetQuestCardObjective at slot 296. ParentClass is CV_ParanoidWhispersScript (ParanoidState +0x48, HideDiggingSpot +0x4C = PDB - 0x14).
- CShaderRenderManager: SetProjectionTransform(16 floats) 0x00988490 calls the private `SetD3DMatrix(D3DMATRIX&, 16 floats)`
  inline with the arguments transposed, which gives the dword-pair copy. Sixteen direct stores get reordered.
  The ctor 0x0098AB00 uses the STLport `_Vector_base` + `_STLP_alloc_proxy` vectors and the red-black tree header (null, malloc 0x18,
  count, colour/links re-read through the member). Retail members sit below the PDB offsets because the containers are 12 bytes.
- CShadowRendererSceneData (0x00B38600): the compiler-generated copy ctor, emitted out of line by a placement-new copy site.
  The matrices are POD block copies and C3DVector has a memberwise user copy ctor.
- CSlider (0x0054ABE0, NUISystem::CSlider::RevertValue): the C2DVector twin of the landed CTextSlider::RevertValue. It is
  `OldValue = StartValue = Value = PreviousValue;` evaluated right to left. The members are unaligned in the PDB, so the
  model uses pack(1).
- CSoundEmitterLH (0x00AAC340, vfunc_8): the sibling of the landed vfunc_9 / StopSoundsContaining. It stops and marks the
  matching sounds, prunes the list, then queries the player again for each match that remains.
- CSpecialAbilitiesDivineWrathDef / PhysicalShieldDef, CSplineRandomOffsetData ctor, CStringParser Error / ReadNextNumber /
  ReadNextString, CSymbolMap::AddSymbolDirty x2: these came from the previous run, re-graded and re-tested
  (0x00633A60 is CPlayer::UninitModeQuickAccessMenu).
- CStartGravekeeperCutsceneMarker (0x00DC7100) and 0x00DC72A0 (really CUndeadSpawnCutscene::Main): a scoped
  CIsThingConscious predicate. After one leading yield, the loop is `while (!IsDistanceBetweenThingsUnder(GetHero(), Me, d))
  { yield; if (IsActiveThreadTerminating()) return; }` with a final terminating test before the parent flag is set.
- CStreamingFontDataBank: InitStreamingEntry(CSymbolMap&) 0x00A5EF50 has an inline counted-pointer `operator=` that reads both
  source words into locals before the block compare, followed by the temporary's release. CloseFonts 0x00A5F2F0 is the
  CloseShaders twin (IsLoaded -> CanUnload/Unload -> delete, then SGI erase(begin, end)). The retail bank base is 0x164.
- CStringAndCageBase (0x00872050, UpdateZVars): the world-height bounds are named float locals of the `if` block, passed to
  an inline `Clamp(float& value, lo, hi)` that rewrites the target in place on the x87 stack. The ease rate is a literal
  ternary. The GetCamTools() result is unused.
- CSubtitleRenderer:
  - GetVisibleText 0x0048DE8A: an implicit "(HC)" temporary in the dead parameter slot. The prefix text and length come
    from one null test written null-case-first, with both values assigned in each arm. The result is
    `strncmp(...) ? false : true`.
  - Update 0x004930D7: `optimize("s")` without the frame pointer. One `MaxDisplayTime != 0` compare is shared by both
    arms, and the profile flag is a ternary accessor.
  - SetAsInCutscene 0x0049319C: the GUI def is a named local fetched first, the "" arguments are implicit temporaries,
    and the skip fade closes through an inline `BeginClose()`, which writes its Closing/Opening bytes.
  - 0x00CBE87F is really the global `__fastcall AddLogbookStoryEntry(long)`: one chained `CCharString(...) + GFIntToCharString(n) + "_"`.
- CShaderResource (0x00A5D510, GetVertexShader(unsigned long) const): CVertexShader's inline copy ctor (vtable, Data,
  add-ref) is a tail shared by both returns. On a cache miss the `new CVertexShader(code, decl, GetEntrySymbol(i),
  entry->GetVSConstantLayout())` arguments are evaluated right to left. The by-value getter's copy lands in the dead
  `index` slot, and the construction-flag word guards the temporaries. The cache is `mutable`.

## Parked for Fable 5.1 (15)
- 00a623d0 CShaderDataBank::NotifyEntryAdded DIFFER(183v174): retail homes the null push_back value and the `__true_type` tag in the one dead `index` slot (one store before the loop); the lane opens a `push ecx` word for the value, stores it on every pass and puts the tag in the index slot | tried: push_back(0), by-value `__forceinline` push_back, `push_back()` -> `push_back(T())` | next: lane gap: empty tag over a live argument slot (24l)
- 00a5f430 CStreamingFontDataBank::NotifyEntryAdded DIFFER(218v213): same push_back value/tag overlay as 00a623d0 (the inline entry ctor lines up) | tried: the 00a623d0 form | next: lane gap: empty tag over a live argument slot
- 00680a40 CSimVoice::Add DIFFER(172v162): retail stores the loop-invariant `pair.first = type` once before the loop, forwards the type argument slot into the in-place copy and keeps this/&Voices in ebx/esi; the lane stores first on every pass and spills `this` | tried: CVectorMap::Add by reference, by-value Add parameters, a hoisted `entry` local (166v162) | next: lane gap: loop-invariant store hoisting
- 0066bd00 CTCSoundPlayer::PlayAttachedCriteria DIFFER(284v231): retail inlines vector<CCharString>::~vector only on the fall-through path and calls 0x00414EA0 on the early return; the lane inlines it on both | tried: in-class inline dtor, out-of-class inline, split `_Destroy` helper | next: lane gap: inline budget per call site
- 009f3470 CSurface::CopyFromMemoryAutoSwizzle DIFFER(153v162): retail pushes ebx/ebp/edi only inside the `rows > 0` guard (shrink-wrapped) and computes width*depth there; the lane saves ebx in the prologue | tried: up-counting for, count-down for, a hoisted rowBytes local, an explicit if/do-while (162v162, frame +4) | next: lane gap: shrink-wrapped callee-saved pushes
- 009f3520 CSurface::CopyToMemoryAutoUnswizzle DIFFER(153v162): the mirror of 009f3470, same gap | tried: the 009f3470 forms | next: lane gap: shrink-wrapped pushes
- 009f3ff0 CSurface::Clear DIFFER(155v161): retail keeps `this` in edi (reused as the column counter and restored from its spill) and re-reads the width at the top of every row; the lane uses esi for `this` and caches the width in edi | tried: row pointer declared after Lock, up-counting loops (153v161) | next: declare `this`-relative locals in another order, or a lane gap: register choice
- 00a44880 CSuspendableProcessMicrothread::RunSuspendableProcess DIFFER(172v172): frame only. In this unoptimised TU retail reserves 0x30 never-touched bytes between the push_back temporaries and the `this` spill; an unused array lands among the named locals instead | tried: unused 48-byte array at function and block scope | next: find the source of the 0x30 bytes (debug-only locals or temporaries)
- 009a6370 CSystemManager::UpdateActive DIFFER(220v227): retail re-reads WindowsQuit at the loop top and right after its own test. Declaring it `volatile` (the window procedure sets it) gives RELOCATION_MATCH (probe/009a6370_volatile_RELOCATION_MATCH.cpp), but this lane bans volatile | tried: plain bool, volatile | next: the landing session decides whether a volatile WindowsQuit is acceptable
- 007610d0 NShape::CSubdivisionTriangle::IsPosInShape DIFFER(237v237): register swap only (point in edx, successor in esi in retail; swapped in the lane) | tried: flat point first, three declaration orders of point/next | next: lane gap: register choice
- 0048f7c0 CSubtitleRenderer::SetButton1OverrideText DIFFER(197v193): the inline `CCharString::operator==("")` materialises `sete al` and re-tests it where retail branches directly (the chunk 188 lane gap) | tried: `__forceinline` operator==, swapped arms with `!(text == "")` | next: lane gap: inline operator== bool materialisation
- 0048f881 CSubtitleRenderer::SetButton2OverrideText DIFFER(197v184): same as 0048f7c0 | tried: the same forms | next: lane gap: inline operator== bool materialisation
- 00a3f250 CSoundManagerLH::vfunc_39 DIFFER(236v236): register naming only (retail edx/eax for the name push and &entry, lane ecx/edx) around the inlined CVectorMap::Add | tried: Add by reference, by-value handle | next: lane gap: register choice
- 00b21090 CSplineRandomOffsetData::GetPointAtIndex DIFFER(132v197) (previous run): the lane cross-jumps the three identical `out = point[index]` tails | tried: plain POD copy, user C3DVector operator=, optimize("g",off) | next: lane gap: cross-jumping of identical tails
- 009b9e00 CStringParser::SkipPastNextItemIfIs DIFFER(219v219) (previous run): only the end-pointer add operand order differs | tried: four forms (see differ/) | next: CalcInQuotes as a by-value helper, or a lane gap: commutative operand order

## Skipped
- 00a3e330 CSoundManagerLH::vfunc_53: the row holds six int3-separated functions (0xA3E330/350/380/3A0/3B0/3C0).

## Label corrections
- 0x00DC72A0 "CStartGravekeeperCutsceneMarker::Main" is CUndeadSpawnCutscene::Main (factory 0x00DC71C0 "UndeadSpawnCutscene", vtable 0x012DA8AC).
- 0x00CBE87F "CSubtitleRenderer::SetText" is the global `?AddLogbookStoryEntry@@YIXJ@Z`.
- 0x009F3520 "CSurface::CopyFromMemoryAutoSwizzle" is CSurface::CopyToMemoryAutoUnswizzle (it copies the surface into the buffer).
- 0x0066BD00 "CSoundInfo::PlaySound" is CTCSoundPlayer::PlayAttachedCriteria(const CCharString&, bool, bool, bool, long).
- 0x00633A60 "~CSpecialAbilitiesDivineWrathDef" is CPlayer::UninitModeQuickAccessMenu (previous run).
- 0x00A5EC00 "CBaseIntelligentPointer::CBaseIntelligentPointer" stores vtable 0x0129A054 = RTTI CPixelShader: it is CPixelShader::CPixelShader().
- 0x009D58D0 is Ego_r.pdb CBankFileAsync::GetAsyncEntry(unsigned long) (two landed rows call it "CreateEntry"; 0x009C8340 already uses GetAsyncEntry).
- 0x00A44690 (landed "YieldMicrothread") is Ego_r.pdb `?InternalSuspend@CSuspendableProcessMicrothread@@KIXXZ`; 0x00A44660 is InternalSwitchToProcess(CSuspendableProcessMicrothread&).
- 0x0099B2C0 (landed "ConvertToCharString") is Ego_r.pdb CWideString::ToCharString; 0x0099BDB0 "CCharString::SetSubstring" is CWideString::GetSubString(unsigned long, unsigned long) const; 0x0099B220 "CCharString::GetLength" is CWideString::GetLength.
- 0x0048F6BD is CSubtitleRenderer::GetTextFromTextTag(const CWideString&, CWideString&); 0x00A61FA0 "CTCInventoryAbilities::GetAbilityLevel" is CShaderDataBank::GetVertexShader(unsigned long) const; 0x00A63020 is the private 4-argument CVertexShader ctor; 0x009CCDC0 is CBankFile::FindIndexBySymbol; 0x004C7A80 "GetMeshBank" is CThing::GetSampleBank() const; 0x0066BC10 / 0x0066B120 are CTCSoundPlayer::GetPotentialCriteriaStrings / InternalPlaySoundEvent; 0x0084D080 "DoSizeof" is NCameraModes::CBase::GetCamTools() const.

Name choices for the landing session (one name per address in these sources):
- 0x00A5EC00: CPixelShader::CPixelShader (RTTI), while the landed family row calls it CBaseIntelligentPointer.
- 0x009D58D0: GetAsyncEntry (PDB) rather than CreateEntry.
- 0x013B8790: g_pPlayerGui (7 landed declarations) rather than g_pGameUI_013b8790 (8). Ghidra labels give no tie-break, so
  the CPlayerGui name was kept; re-check before the sweep.
- 0x00A44C20 push_back is declared as vector<CASuspendableProcess*>::push_back only in a parked row (landed name:
  CRetailVector_dword::PushBack_dword_00a44c20).
