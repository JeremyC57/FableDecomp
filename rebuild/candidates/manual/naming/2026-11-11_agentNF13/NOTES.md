# NF13 notes

57 changed rows in `rows/` (56 RELOCATION_MATCH, 1 MATCH: 0x006BD760), all regraded together at the end (`work/final.sh work/rows.txt`,
`--all`, overlay `hdr/`): no HARD, VC7.1 tests PASS for all 57 (catalog PassPatterns; `work/chk.py` now reads them from
`rebuild/build_candidates.ps1`). g++ tests PASS except ten rows whose landed tests already fail g++ (PREFAIL, unchanged):
005be028 007cce20 008af850 005b3d7a 00a9f1d0 012299e0 012299f0 01229a00 006e2a50 007498e0. Changed tests: `rows/006bd760_test.cpp`
(rewritten for CState, PassPattern `BEHAVIOUR_PASS_006bd760` kept) and `rows/006e2a50_test.cpp` (names respelt only).
Grade counts: one grade per row except 005be028 / 00663f50 / 00694020 / 006e2a50 / 00747480 / 007498e0 / 00a9f2f0 (2),
00aa0c50 (3), 006bd760 (5, of which 3 were cl hangs on an `#error` in an include, see LESSONS), plus the final regrade.
Busy addresses: none of the changed rows is in BUSY.txt.

## Queue (QUEUE.tsv), per entry
- 0x00787F29, 0x00787C35, 0x0083CF20, 0x0066B280, 0x008113C0, 0x00A242C0, 0x00835A20, 0x004C9F60: claims verified (both PDB name
  sets; for 0x0066B280 also the retail pushes: the `push 0 / push 0 / push 1` before the CSoundInit temporary in 0x00694020 are
  PlayAttachedSample's bools and the temporary is the default ctor 0x0041C930, not a (bool, long, long) ctor). Applied to every
  landed binder that compiled wrong (STATUS.tsv), except the binders listed under "Left".
- 0x0083E0D0 / 0x0079BD80 / 0x006D3E80: the self rows were generic `GetDef_<addr>` members; now the FableWin.pdb template
  instantiations (`template bool CDefinitionManager::GetDef<T>(...) const;`) with the landed sibling 0x0074D220 / 0x00593666 shape.
  Row renames in RENAMES.tsv.
- 0x00707930: **claim rejected.** The address is CTCHeroAttachableAppearanceModifiers::UploadChangesToGraphics (Ego_r.pdb QAEXXZ,
  public non-virtual, four direct callers, landed self row already named so); EditFrameUpdate is the CTCBase editor virtual
  (UAEXXZ). The header's `_COVERS` opt-in declared EditFrameUpdate at this address; the overlay adds `_UPLOADCHANGES` (wraps it).
  All four callers now bind UploadChangesToGraphics.
- 0x005DC2E0: nothing to do (every landed binder already binds AnimExists).
- Globals 0x013BAD34 / 0x013BAD38 / 0x013BAD3C (descriptive, no PDB data name on disk): one spelling each, the one the landed users
  already bound: `const CCharString g_DoorOpenAnimName`, `const CCharString g_DoorCloseAnimName`, `CCharString g_ClockHandTurnName`.
  Applied to the dynamic initializers 0x0121ED00 / 0x0121ED20 / 0x0121ED40 (catalog names change: RENAMES.tsv) and to the atexit
  destructor thunks 0x012299E0 / 0x012299F0 / 0x01229A00, which now call `object.~CCharString()` on the real object (they bound an
  invented `g_AutoTinyThunkObject_<addr>` of type AutoTinyThunkTarget with `Invoke()` for ~CCharString). The thunks keep their
  family name GlobalMethodTailThunk_<addr> (compiler-generated `??__F` functions; the family is 297 rows, not renamed here).
- 0x006BD760: renamed to CMusicManager::CState::GetActiveSet() const on engine/CMusicManager.h (opt-in `_GETACTIVESET_OUTOFLINE`).
- 0x00C400D0: the two Animation_ComputeFrameOffset callers now call LookupDirectionFilterPair(long, float, float, const
  SDirectionFilter**, const SDirectionFilter**) (their local CAudioMaster::TapTable is the long table selector).
- 0x006E2A50: claim proven (second source: Ego_r donor order DoesSpeechExist / GetSpeechDurationInFrames / PlayLipMovement /
  PlayRandomLipMovement = retail 0x006E2910 / 0x006E2940 / 0x006E29A0 / 0x006E2A50). Row renamed; its call to 0x006E2940 now binds
  the PDB private const GetSpeechDurationInFrames.
- 0x006BD9B0 and 0x00C43B30: the catalog (auto-re-candidates.tsv) and manifest already carry InternalUpdate /
  AttachConfiguredBackend; only the chunk ROWS.tsv held the old labels. 0x00C43B30 still has two parameter spellings (NAME_FIXES).

## Item 2 (GetDef<CThingBaseDef> 0x0042AF3C / GetDefinitionManager 0x006869D0)
Four rows moved: 007498e0 CTCNoise::OnCreate, 00747480 CTCStealth::UpdateViewability, 007fbc90 CTCKraken::GetCentreMarker,
007ef070 CTCExpression::OnSerialise -- each by giving the row's component a CTCBase base with the protected const
`GetDefinitionManager()` returning `const CGameDefinitionManager&` (CGameDefinitionManager : CDefinitionManager) and calling the
def lookups const through it. Two of them also bind their GetDef callee as the PDB template now (0x0063F3A0, 0x00593666).
The other 48 item-3 rows are left (work/item3_pre.txt): each needs its def-manager callees respelt const as well
(GetPDefFromGlobalIndex returns `CDefPointer<const CDefClassBase>` and is const in the PDB, the rows declare non-const
CCountedPointer / CDefBasePointer stand-ins) or its GetDef stand-in body replaced, i.e. a per-row model rewrite (NF10 did this by
moving rows onto engine/CTC*.h retail branches). The CTCInventoryBase rows (005bf9e3, 005bd82e, 005bdfb1) were not attempted: no
container model was added.

## Left, with reasons
- GetWorld 0x00686A70 binders (15): 006585ee, 00658604, 006cb8f0, 006ceae0, 007236e0, 00811450 are free `__fastcall` stub rows
  calling an invented helper (`helper_prep`, `helperA`, `First_Helper`, `CTCPhysicsStandard_helper`, `baseUpdate`); their own names
  are wrong too (two different addresses are both "CTCHeroExperience_AddSuccessfulBlockParry"), so they need re-identification, not a
  respell. 006af620 is a free `GetDisplayEngine` stub (probably CTCPhysicsBase::GetWorldMap: NAME_FIXES). 006a23e0 lives in an invented
  namespace. 0065c1fc: the flattened CTCVillage header has no CTCBase base (needs FABLE_RETAIL_CTCVILLAGE). 006b0c10 / 007be0f9 /
  007e2790: local polymorphic models without a CTCBase base; 007e2790 also reads a local `CWorld_007e2790` (+0x104).
  006d0180 / 00700940 / 00960760 call `opinion->GetWorld()` on another component: the PDB accessor is protected, so retail reached
  it through something else (an inline of that component) -- not provable from the bytes.
- 0x00835A20: 006c8780 sits in the invented namespace `X_006c8780` with a CTCDNone::Create stand-in body.
- 0x0079BD80: 00833a70 keeps its same-TU GetDef stand-in (NF12-left row: needs the SuspendAI std::set model).
- 008af850 CombatDialogueManager::PlayBattleCry: PlayAttachedSample now binds right (the sound types were hoisted out of the invented
  namespace `ExternalRetry_008af850`), but the row's own symbol and its CCharString dtor are still in that namespace; needs a rewrite.
- 008957c0: AddEvent now right; its own symbol is `CGameScriptInterfaceMethods::SendEntityEvent` and the three event ctors are invented
  `EventPickpocketing` etc. (NAME_FIXES 0x008AB7B0 / 0x008AB840 / 0x008AB8D0).
- 0x00C43B30 callers 00c418c0 / 00c448a0: two descriptive libpk type universes (CUnproven* vs CSoundBuffer / SVoiceCaps / CVoice).
- No fresh idea for NF12's "Left" rows (CTCDoor slot 85, 006a9ef0 OnCreate): not attempted.

## Header debt / naming notes
- engine/CTCHeroAttachableAppearanceModifiers.h `_COVERS` declares EditFrameUpdate at 0x00707930 (wrong); fold `_UPLOADCHANGES`
  into it at landing.
- The flattened CTCEntityEvents branch now has the AddEvent declaration under the retail branch's macro.
- 0x00686D60 is bound under several invented names (GetFrameSource, GetWorld_00686d60, GetGameSubsystem1C ...); the label says
  CTCBase::PeekComponent (Ego_r ?PeekComponent@CTCBase@@IBEABVCMainGameComponent@@XZ is a candidate; not verified here).
- Descriptive names (no original found): g_DoorOpenAnimName, g_DoorCloseAnimName, g_ClockHandTurnName, LookupDirectionFilterPair.
