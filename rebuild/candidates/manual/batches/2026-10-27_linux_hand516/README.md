# Chunk 516 (agent516, 2026-10-28): 39 of 40 re-queued single functions landed (under 128 bytes)

Third re-queue chunk: 40 chunk_history orphans under 128 bytes (address order, 0x00899000..0x008D03E0), almost all
CGameScriptInterface virtuals. One Opus 5.5 agent at high effort.

- **Landed:** 39 rows with `vland.py --land`, all RELOCATION_MATCH, every test PASS under g++ and VC7.1; no
  RELOC_IDENTITY, RELOC_CONFLICT or RELOC_DUPLICATE. 35 CGameScriptInterface component rows matched first grade on the
  batch hand955 model (PDB spelling of the interface-map LowerBound 0x0040F020, `__forceinline`
  `CThing::GetInterface(T** out, type)`), in five guard forms; both SetIsGossipForPlayer slots (0x008A96F0, 0x008A97D0;
  Ego_r's static CTCAIScratchpad::SetIsGossipForPlayer(CCharString, bool) inlined); the CCombatSequenceBase constructor
  (0x008B0AB0, real STLport `_Vector_base` with the `_STLP_alloc_proxy` member); CGameScriptThing::GetFocalPos (0x008D03E0).
- **Parked (1, pending/2026-10-27_linux_hand516/differ/):** 0x008C6160 CCreatureAction_RockTrollFallToDeath::Initialise
  (87v87, `this` / `&PCreature` in esi / edi swapped).
- **Identities:** all 40 proven (vtable 0x01260F0C slots, ghidra_out/cgsi_slots_aligned.tsv, Ego_r mangled names; the
  overload pairs settled by slot and by body: slot 618 the `long` EntitySetPersonalityOverride, 619 the CCharString one,
  723 the by-value SetIsGossipForPlayer, 724 the const-ref one). Probable callees: 0x004C1060 CTCGraphicAppearance::
  GetFocalPos(float) const, 0x008B25C0 `std::__copy<CCombatAction*, CCombatAction*, int>`.

Label corrections (NOTES.md): 0x00681E60 CTCCreatureOpinionOfHero::SetPersonalityOverride(long, bool), 0x008ABB50
CTCTargetingBase::SetPScriptOverrideRangedTarget, 0x007EA270 CTCTargeted::SetTargetingFlags, 0x00728460
CTCRandomAppearanceMorph::SetAppearanceSeed, 0x008B3010 `__destroy_aux<CCombatAction*>`, 0x00833A10
CThingAICreature::SuspendAI, 0x0099EAE0 CCharString::~CCharString, 0x008574D0 CCombatActionBase::SetCombatAnimation.

Why earlier attempts stalled: hand44 (QFE 4035) parked 31 rows of this family as a register-choice gap; under RTM with
the out-pointer GetInterface form the shape comes out. The remaining hand44 / hand45 parks of the family should be
regraded on this model. agent20's constructor used an invented stand-in vector; hand45's GetFocalPos park was source shape.

Review item: 67 distinct invented spellings in rebuild/oracles/reloc_bindings.tsv bind 0x00487DC0 (the settled
`?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ`), e.g. landed 0x00899060 declares it
CPlayer::GetPPlayerCharacter.
