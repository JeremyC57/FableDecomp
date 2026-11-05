# Landing 2026-11-05i: chunk 782 (4 of 6 rows: two engine constructors and two quest-script bodies)

One expert agent authored the rows. All four are plain C++ and grade `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x007941D0 | CTCUnholyPower::CTCUnholyPower(CThing&) | RELOCATION_MATCH PASS (8 grades) |
| 0x00B5F090 | CEngineScreenEffectGlowRenderer::CEngineScreenEffectGlowRenderer(const CEngineInit&) | RELOCATION_MATCH PASS |
| 0x00E00DB0 | NScript::CQ_TraderEscortScript::Main (/Gr) | RELOCATION_MATCH PASS |
| 0x00EB4760 | NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::DoDealWithHero | RELOCATION_MATCH PASS |

- **Names.** The ledger was wrong for two rows:
  - 0x00B5F090 is the glow renderer ctor, not CEnginePrimitiveManagerDecal: vtable 0x012A27CC, `new 0x280` in CEngine::Initialise.
  - 0x00E00DB0 is CQ_TraderEscortScript::Main, not CV_TravellingHeroesScript::Main: slot 2 of vtable 0x012DF16C.
  - DoDealWithHero gains its enclosing CV_MurderTwistFinalScript scope.
  - Labels: `ghidra_out/labels_landing_2026-11-05i.tsv`.
- **How 0x007941D0 matched.** The CDefClassBase::GetSubDef<T> body is in the TU. The same TU also carries an opt-in noinline body
  of CSpecialEffectsDef::GetEffectFromString. The ring indices are scoped to retail's 0x24 frame. See `agent_NOTES.md` and the
  GOTCHAS lines for chunk 782.
- **Headers** (`hdr_README.md`). Every change is a new opt-in branch or a new file. hdrcheck2 reads `default=same` for every header.
  - opt-in branches in CV_MurderTwistFinalScript (ASSASSIN), CEngineScreenEffectGlowRenderer (GLOWRENDERER_MEMBERS),
    CEngineComponent (PROTECTED), CEnvironment (TRANSITIONTOTHEME), NGraphicAppearance_CTCGraphicAppearance (LIGHTINGCHANNEL),
    CTCVolumeContainmentTracker (LIGHTING), CEnginePrimitiveThemeChannelDesc (INDICES), CSpecialEffectsDef (TYPENAME,
    GETEFFECT_BODY) and stlport/fable_containers.h (STLP_VECTOR_ERASE_OUTOFLINE);
  - new headers: CTVertexColTex1.h and CSpecialAbilitiesUnholyPowerDef.h.
- **Landing:** `vland --land` (`logs/land.log`).
  - `bindings_multi_check.py` reports only the known NullCoord member offsets (plan phase 7).
  - 13 queue lines (tag `land0511i`). They include 0x007940F0, landed as CTCDivineWrath::PlaySoundW but really
    CTCUnholyPower::PlaySoundW, and 0x00669D30 (CSpecialEffectsDef::GetEffectFromString), plus the stand-in callee spellings of
    the CTCUnholyPower TU.

## Parked (`pending/2026-11-05_linux_hand782/`)
- **0x00EA1720 CV_KnotholeGladeGatesScript::DoInitialBalverineAttack:** DIFFER(1940v1940), 4 lines, after 10 attempts.
  - Retail hoists `lea ecx,[esp+68h]` above the balverine-wait terminate `jne`; VC7.1 keeps it in the loop latch.
  - This is the same residue as chunk 781's 0x00D6A560.
- **0x00DFD600 CQ_TraderConflictGoodScript::AttackPeople:** DIFFER(2456v2088), 184 lines at best.
  - VC7.1's per-site inline choices for `~CScriptThing` and the merged trader-alive exit blocks do not follow retail.
