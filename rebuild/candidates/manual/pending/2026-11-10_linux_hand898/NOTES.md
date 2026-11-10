# Chunk 898 notes (22 parked near misses)

Result: 9 RELOCATION_MATCH in src/ (each with a passing C++03 test that also compiles clean under VC7.1 /Od /W3), 13 parked in differ/
with measured residues (re-graded at the end, 4-6 masked lines each). The header overlay is documented in hdr/README.md (4 opt-ins
plus a copy of LHNetworkTypes.h). gall.sh must run with FABLE_HDR_OVERLAY=$A/hdr.

## Identities proven (see LABELS.tsv)
- 0x006B83F0 = CEnvironmentSpellEffectInterpolator::ScreenFilterFadeOut(float, void*) (Ego_r/FableWin name; the PDB-named
  CGameScriptInterface::ScreenFilterFadeOut 0x00890200 tail-jumps there off CEnvironment+0x1964). Landed code has it wrong in three
  places (NAME_FIXES.tsv): the 0x006B83F0 row itself (_Cons_val / Alloc::ConsVal) and the callers 0x0079D020 / 0x0079D110
  (CEnvironmentSpellEffects::RemoveFilter_006b83f0(long, void*)). The landed 0x0079D110 also passes PDef->FilterRemoveMode as the
  first argument: under the PDB signature that is the fade time (a float).
- 0x00A0BE30 CCamera::SetView(const C3DVector&, const C3DVector&); 0x0079CEE0 CTCBulletTime::StopSound(unsigned long).
- Catalog names corrected by the drafts' identities: 0x00732D40 (not CViewBase_UpdateLookDirection), 0x0080DB70 (not
  CTCPhysicsBase_ApplyExplosiveImpulseFromPoint), 0x00807A90 (not std_copy_if_WithDistance), 0x00826C90 (not
  CAFile_ConstructNavConnectorVertex), 0x00712900 (Update, not vfunc_1), 0x007E4530 (static member of CTCWillResponse).

## Header debt (shared-header pass after the coordinator's review)
- All nine src/ rows now take every class that has a header from rebuild/include (re-graded RELOCATION_MATCH after the move):
  0x009E2020 C2DBoxF / C2DExtentsI / CDisplayManager / CSystemManager (existing opt-ins only); 0x00732D40 CEnvironment (new
  _VIRTUALS) / CTCPhysicsBase (existing retail branch); 0x0079CF30 CTCBase / CTCBulletTime (new _MEMBERS) / CTCCarrying (new
  _BYPASSFILTERS) / NParticleEngine::CParticleEmitter (new _KILL) / CIntelligentPointer (retail branch + ASSIGN); 0x00654438
  CPlayerGuiDef (new _QUESTINFO) / CPlayerGui (new _MINIMAPDRAWING, _GUIMODE) / CWideString (fable_string.h); 0x00A3BF10
  CIVCountedPointeeBase / CIVCountedPointer (new _CONVERT) / C3DAnimationState (new _DEFAULT) / C3DAnimationTransitionState (new
  _INTERPOLATE) / C3DAnimationBlendState (new _CREATEINTERPOLATE); 0x00D9EB30 CQ_MinionCampScript (new _MINIONGUARD) / CCharString
  (fable_string.h, FABLE_RETAIL_CCHARSTRING). Every opt-in is documented in hdr/README.md; default paths unchanged.
- Classes still defined in a row because no shared header exists: CTCWeatherVane (0x00732D40), CDrawQuestInfo and its
  CQuestInfoElement (0x00654438), CTCWillResponse (0x007E4530), winsock timeval / fd_set (0x008F8540).
- Header spelling: engine/CQ_MinionCampScript.h forward-declares the guard as `CMCMinionGuard`; Ego_r / FableWin and all landed rows
  spell it CMC_MinionGuard (the overlay's opt-in uses the PDB name; the default line should be respelled at landing).
- FABLE_RETAIL_CPLAYERGUI_MEMBERS could not be used in 0x00654438: with it cl spun to the 240 s timeout (no diagnostic), hence the
  small _GUIMODE opt-in for the enum alone.

## What each parked row needs next
- 0x0080DB70: impulse Y x87 operand order (`fld [Y] ; fmul st(2)`); the `*=` form fixes it but keeps X alive past the join.
- 0x006E8CE0: the first inlined Normalise: retail `fmul [esp+38h]`, ours `fld [esp+38h] ; fmul st(1)` + an extra `fstp st(0)`; the
  shared C3DVector GetMagnitude squares X before Y (retail Y first), so a shared-header rebase needs a stepwise GetMagnitude opt-in.
- 0x00A22F80: one commuted fmul (E11 * tx) in the Up X row; insensitive to term order and parentheses -- candidate-id order, try
  where the matrix temporary is built (ToMatrix member vs free function, declaration order).
- 0x00712900: forwarded-temp register rotation: retail's played-arm read is one rotation step ahead (ecx vs eax); making the played
  arm the then-arm gives the registers but lays the blocks out wrong.
- 0x00A65720: minSize temporary in ebp (retail edi after `push edi`); size spellings inert.
- 0x00826C90: `mov ebx,[esp+10h]` scheduled two instructions early; zero-store order, held ctor and accessors inert.
- 0x006509F8: `lea ecx,[ebp-14h]` before `fstp [ebp+8]` at the join; return forms inert.
- 0x00B396E0: &PrimitiveTransform in ebp vs ebx; c2prio: temp53 (prio 22) is coloured before child (12) and the zero constant (-25)
  and takes the already-used ebp; retail needs ebx used first. Heavy local models.
- 0x00AC7200: dot-product term order; the shared C3DVector diverges (305 bytes).
- 0x008884D0: esi/ecx copy direction in the blended vector's teardown; the draft is stand-ins (CBlendedThemeArray, CFloatArray),
  and dtor spellings (no local, both locals, deallocate helper, base/derived dtor split) are inert or worse: rebase onto the real
  stlport vector<CBlendedEnvironmentTheme> with out-of-line reserve/resize opt-ins.
- 0x00941FC0: `add eax,edi` vs retail `add edi,eax` for y+r. The shared C2DBoxI(const C2DCoordI&, long radius) ctor fixes y but
  computes x+r with `lea` and shifts later registers; the draft also carries a noinline same-TU search stand-in.
- 0x00807A90: *F3 copied at the predicate top vs retail just before the third call; moving the copy enregisters it (ebp).
- 0x00BEEA00: val-copy load scheduling before `push comp` (retail preloads the key's two words); the draft uses a local vector stand-in.
