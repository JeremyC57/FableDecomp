# 2026-11-11 landing bp -- agentNF13: 57 landed rows respelt to proven callee / own names

Source: `naming/2026-11-11_agentNF13/` (STATUS / NOTES / LESSONS / RENAMES / NAME_FIXES). vland --recheck-landed: 56 RELOCATION_MATCH +
0x006BD760 MATCH; VC7.1 tests PASS (0x006BD760 prints its catalog pattern BEHAVIOUR_PASS_006bd760, checked by running the exe).
The `_$E2` RELOC_CONFLICT warnings on the three static initializers are the TU-local compiler symbol (benign). g++ PREFAIL (pre-existing):
005be028 007cce20 008af850 005b3d7a 00a9f1d0 012299e0 012299f0 01229a00 006e2a50 007498e0.
- AddEvent 0x008113C0 (22 rows: row-local CEventBase moved to NEntityEvents), CEventTriggered(CThing&) (4), PlayAttachedSample (5;
  0x00694020's CSoundInit(bool,long,long) was the call's bool pushes), AddAndCopyTC (5), CTCDNone::Create (3), PeekAnimation / private
  BlendAnimations (4), CTCMeleeTrap / CTCTrapBase self rows + 0x00787CA5, LookupDirectionFilterPair callers, UploadChangesToGraphics callers
  (queue claim EditFrameUpdate rejected), 4 GetDef<CThingBaseDef> / GetDefinitionManager rows.
- Row renames (8): 0x006BD760 CMusicManager::CState::GetActiveSet, 0x006E2A50 CTCTalk::PlayRandomLipMovement, GetDef<CThingPhysicalSwitchDef>
  0x0083E0D0, GetDef<CBrainDef> 0x0079BD80 / 0x006D3E80, static initializers g_DoorOpenAnimName / g_DoorCloseAnimName / g_ClockHandTurnName
  (descriptive data names; atexit thunks now call ~CCharString on the real object).
Headers (base 43674f96e, opt-ins only, hdrcheck2 default=same): CTCEntityEvents (ADDEVENT on the flat branch), CTCHeroAttachableAppearanceModifiers
(_UPLOADCHANGES), CMusicManager (_GETACTIVESET_OUTOFLINE). Bindings: 164 removed, 138 added. Queued land1011bp: 5 unfixed name claims.
