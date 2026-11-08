# hdr/ overlay -- chunk 868 (agent868)

Every file here is the repo header plus an additive `#ifdef FABLE_RETAIL_*` opt-in (declarations only, no layout change for
any existing includer), or a new header. Carried over from hand-back 863 unchanged: `engine/CQ_OrchardFarmRaidScript.h`
(new) and the `FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_ASSASSINMARKER` block of `engine/CQ_BanditCampBossBattleScript.h`
(used by the parked differ/00d11930.cpp and differ/00dcfa60.cpp). Neither matched row (0x00D18DF0, 0x00ECBDF0) needs any
overlay header: both build on the repo tree as it is.

New this chunk, all for the 0x007A1B20 CTCWillLightning::CLightningBeam::FrameUpdate shared-header rebuild
(differ/007a1b20.cpp, parked 2712v2840):

| header | opt-in macro | what it adds | evidence |
|---|---|---|---|
| engine/CThing.h | FABLE_RETAIL_CTHING_VIRTUALS | vtable slots 17..45 (IsToBePlacedInMap .. SetPositionUsingBrain) + forward decls of their parameter types | PDB-named local model in landed rebuild/src/compiled/00/83/CThingDeadCreature_OnCreate_00835fd0.cpp; CThingGameObject.h STANDINGON slots 38/39 agree |
| engine/CThingPhysical.h | FABLE_RETAIL_CTHINGPHYSICAL_VIRTUALS | slots 46..59 (IsSolidToThing .. ModifyHealth; PeekPMaterial returns CDefPointer<const CMaterialDef>) | same local model; needs the CThing chain |
| engine/CThingGameObject.h | FABLE_RETAIL_CTHINGGAMEOBJECT_VIRTUALS | slots 60..64 (DrawIsTargeted .. IsCover) so `OnHit` is slot 65 = +0x104 | retail `call [edx+104h]` at 0x007A228C / 0x007A2436 |
| engine/CTCWillLightning.h | FABLE_RETAIL_CTCWILLLIGHTNING_BEAM | `class CLightningBeam;` + `StartLoopingSound()` (0x007A0400) in the class; the naturally aligned `CTCWillLightning::CLightningBeam` (sizeof 0x7c, STLport list at +0x4c) after it | struct_layouts_egor.tsv (PDB 0x84 with the 8-byte-larger Dinkumware list); every offset read by 0x007A1B20 (m_fTime +0x44, m_lChildren +0x4c, m_bTemporary +0x50, m_fDelay +0x68, m_nFrameCount +0x70, PDef +0x74, SuccessfulHit +0x78) |
| engine/CHitParameters.h | FABLE_RETAIL_CHITPARAMETERS_LIGHTNING | 9-arg ctor (0x00789238), `AddDamageWithAttribute(EDamageAttribute,float)` (0x00868CA0), inline `SetCreateDustParticleEffectOnHit(bool)`; `enum EDamageAttribute;` forward | FableWin.pdb ctor name; labels; landed 0x007E2600 sibling models |
| engine/CTCEntityEvents.h | FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT | `AddEvent(NEntityEvents::CEventBase*)` (0x008113C0) | Ego_r.pdb / FableWin.pdb name; the row's call after `new CEventHitBySpecialAbility` |
| engine/CTCSoundPlayer.h | FABLE_RETAIL_CTCSOUNDPLAYER_UNATTACHED | `PlayUnattachedCriteria(const CCharString&,const C3DVector&,bool,bool)` (0x0066BDF0) | labels + landed will-lightning rows |
| engine/NCreatureMode.h | FABLE_RETAIL_NCREATUREMODE_QUERIES | `__fastcall IsUnderground(const CThing&)` 0x0085F120, `IsLightningable` 0x0085FAA0 | labels_*.tsv + PDB names |
| engine/CSpecialAbilitiesLightningSpellDef.h | (new, always on) | `: public CDefPointeeBase` (so CDefPointer<const T>::Get compiles), pad to +0x110, CDefString InitSound/LoopingSound/EndHeroSound/EndAttackeeSound +0x110..+0x11c | landed ctor / StartLoopingSound 0x007A0400 read +0x110/+0x114; the row reads +0x11c; PDB order |
| engine/NEntityEvents_CEventHitBySpecialAbility.h | (new) | ctor `(const CHitParameters&, EHeroAbility)` 0x00811830, sizeof 0x18 | struct_layouts_egor.tsv sizeof; `push 18h ; new` in the row |
| engine/CTCElectrocutedEffect.h | (new) | `void Reset(); // 0x007CE7A0` | labels_*.tsv + PDB |

Pack: CLightningBeam is deliberately NOT `#pragma pack(1)` -- the PDB layout has no odd offsets and a packed declaration makes
VC7.1 spill the float members through a frame temporary (r1: 2728 bytes, frame 0xc4; r2 proved pack irrelevant to the remaining
residue). `CCRC::Calc` is declared only inside CSpecialEffectsDef.h's GETEFFECT_BODY opt-in, so the row declares it locally.
