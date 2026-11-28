#ifndef FABLE_ENGINE_CTCSOUNDPLAYER_H
#define FABLE_ENGINE_CTCSOUNDPLAYER_H
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
class CThingSoundEmitter;
// Original complete36-byte class and native component16 receiver.
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER
#include "engine/ETCInterfaceType.h"
#endif
class CTCSoundPlayer : public CTCBase {
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
public:
void PlayCollisionSound(const C3DVector&,const CDefPointer<const CMaterialDef>&,const CDefPointer<const CMaterialDef>&);
#endif
public:
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER
    // Retail opt-in (chunk 751, row 0x007E0D00 CTCEffectOnDie::OnDie): FableWin.pdb ?StatGetInterfaceType@CTCSoundPlayer@@SI?AW4ETCInterfaceType@@XZ
    // for CThing::GetTC<CTCSoundPlayer> (FableWin ??$GetTC@VCTCSoundPlayer@@@CThing@@QAE_NPAPAVCTCSoundPlayer@@@Z; retail looks up
    // interface 0x10 = TCI_SOUND_PLAYER inline). A declaration with its body only: no layout change.
#ifdef FABLE_RETAIL_TALK_UPDATE_PASS29
    bool IsAttachedSoundLoadedAndPlaying(unsigned long sample) const;
#endif
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SOUND_PLAYER; }
#endif
#ifdef FABLE_RETAIL_CTCVOLUMECONTAINMENTTRACKER_UPDATE
    unsigned long PlayAttachedSample(const class CSoundPair&,const class CSoundInit&,bool,bool,bool);
#endif
#ifdef FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE
    void StopAttachedSound(unsigned long); // Both original PDBs; native0066add0.
#endif
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER_ATTACHEDDIALOGUE
    // Retail opt-in (wfA1, row 0x006D0830 CTCWife::GlobalUpdate): Ego_r.pdb ?IsAttachedSoundPlaying@CTCSoundPlayer@@QBE_NK@Z (0x00574400;
    // retail 0x0066AE10, reloc_bindings) and ?PlayAttachedDialogue@CTCSoundPlayer@@QAEKABVCCharString@@_N111@Z (0x00575000; retail
    // 0x0066BED0: `push 1 ; push 0 ; push 0 ; push 0 ; push &def+0xC4 (CCharString) ; mov ecx,player ; call`, 0x006D0D8A). Declarations only.
    bool IsAttachedSoundPlaying(unsigned long sample_id) const;          // 0x0066AE10
    unsigned long PlayAttachedDialogue(const CCharString& group, bool b0, bool b1, bool b2, bool b3);   // 0x0066BED0
#endif
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER_CTOR_PASS23
 // Both original PDB PUBLICs: original CThing& constructor.
public: CTCSoundPlayer(CThing&);
#endif
#ifdef FABLE_RETAIL_EXPRESSION_INIT_PASS29
    unsigned long PlayAttachedDialogue(unsigned long,bool,bool,bool,bool);
#endif
#ifdef FABLE_RETAIL_CTCBERSERK_INITABILITY_PASS29
public: void SetExtraTag(const CCharString& tag) { ExtraTag = tag; }
#endif
    unsigned long PlayAttachedCriteria(const CCharString&,bool,bool,bool,long);
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER_PLAYATTACHEDSOUNDEVENT
    // Retail opt-in (hand3c3, row 0x0078A690 CTCHeroRangedAttacks::FrameUpdate): ?PlayAttachedSoundEvent@CTCSoundPlayer@@QAEKABVCCharString@@_N11@Z
    // (Ego_d.pdb CTCSoundPlayer::PlayAttachedSoundEvent; the landed row 0x0066B1D0 carries the name), out of line in retail at
    // 0x0066B1D0 (`ret 10h`; the bow-draw call pushes three false flags, 0x0078A754). Declaration only: no layout change.
    unsigned long PlayAttachedSoundEvent(const CCharString&,bool,bool,bool);   // 0x0066B1D0
#endif
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER_PLAYUNATTACHEDCRITERIA
    // agent wfE10 (row 0x007A1B20): Ego_r.pdb / FableWin.pdb ?PlayUnattachedCriteria@CTCSoundPlayer@@QAEKABVCCharString@@ABVC3DVector@@_N2@Z,
    // out of line in retail at 0x0066BDF0 (reloc_bindings). Declaration only; include engine/C3DVector.h first. Also wfE11, row 0x004A3740
    // CWorld::UpdateRegionLoad (called 0x004A595F with "BIG_DOOR_SLAM_SHUT", the hero's position, false, false).
    unsigned long PlayUnattachedCriteria(const CCharString& criteria, const C3DVector& position, bool loop, bool p3);   // 0x0066BDF0
#endif
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER_RELEASELOOPINGSOUND
    // Nested opt-in (chunk Q25, row 0x00847520): Ego_r.pdb / FableWin.pdb ?ReleaseAttachedLoopingSound@CTCSoundPlayer@@QAEXK@Z, out of
    // line in retail at 0x0066AD90. Declaration only.
    void ReleaseAttachedLoopingSound(unsigned long soundID);             // 0x0066AD90
#endif
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER_STOPALLATTACHED
    // agentFAM13 (2026-11-12, row 0x00773C90 CTCShop::SetAlarm): Ego_r.pdb / FableWin.pdb ?StopAllAttachedSounds@CTCSoundPlayer@@QAEXXZ,
    // out of line in retail at 0x0066AEB0 (the landed caller 0x00773C90 binds it). Declaration only.
    void StopAllAttachedSounds();                                        // 0x0066AEB0
#endif
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER_PLAYATTACHEDSAMPLE
    // Opt-in (agentNF12, 2026-11-11, rows 0x0083DDC0 / 0x0083DF20): Ego_r.pdb / FableWin.pdb
    // ?PlayAttachedSample@CTCSoundPlayer@@QAEKABVCSoundPair@@ABVCSoundInit@@_N22@Z (retail 0x0066B280, out of line; the callers push the
    // three bools as 1, 0, 0). Declaration only.
    unsigned long PlayAttachedSample(const class CSoundPair& sound, const class CSoundInit& init, bool, bool, bool);   // 0x0066B280
#endif
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER_BANKSAMPLE
    // Opt-in (wf10 row 0x006E3580 CTCTalk::PlaySpeech): FableWin.pdb ?PlayAttachedSample@CTCSoundPlayer@@QAEKAAVCASoundBank@@ABVCSoundPair@@ABVCSoundInit@@_N33@Z
    // (retail 0x0066B300, landed; FableWin.pdb 0x01E7FCA0 and Ego_r.pdb 0x00574680 both spell _N33: CASoundBank takes back-reference slot 0, so bool is slot 3) and Ego_r.pdb / FableWin.pdb ?SetSpecificSamplePitch@CTCSoundPlayer@@QAEXKM@Z (retail 0x0066AFA0, landed).
    // Declarations only.
    unsigned long PlayAttachedSample(class CASoundBank& bank, const class CSoundPair& sound, const class CSoundInit& init, bool, bool, bool);   // 0x0066B300
    void SetSpecificSamplePitch(unsigned long sampleID, float pitch);   // 0x0066AFA0
#endif
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER_SETSPECIFICSAMPLEVOLUME
    // Overlay opt-in (wfD4, row 0x006D1E30): Ego_r.pdb 0x00574450 ?SetSpecificSampleVolume@CTCSoundPlayer@@QAEXKM@Z, out of line in retail at
    // 0x0066AFF0 (reloc_bindings; the SetSpecificSamplePitch 0x0066AFA0 twin). Declaration only.
    void SetSpecificSampleVolume(unsigned long sampleID, float volume);  // 0x0066AFF0
#endif
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER_VOICENAME
    // Opt-in (chunk 765, row 0x0066A9D0): Ego_r.pdb / FableWin.pdb ?SetVoiceName@CTCSoundPlayer@@QAEXABVCCharString@@@Z (VoiceName +0x14).
    void SetVoiceName(const CCharString&);                               // 0x0066A9D0
#endif
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER_CRITERIA
    // Retail opt-in (chunk 1021, row 0x0066BC10): Ego_r.pdb
    // ?GetPotentialCriteriaStrings@CTCSoundPlayer@@AAEXABVCCharString@@AAV?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z
    // (private; Ego_r donor order puts it before PlayAttachedCriteria 0x0066BD00 / PlayUnattachedCriteria 0x0066BDF0, its two
    // callers). A declaration only: no layout change. Needs stlport/fable_containers.h and fable_string.h.
private:
    void GetPotentialCriteriaStrings(const CCharString& criteria, std::vector<CCharString>& criteria_strings);   // 0x0066BC10
public:
#endif
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER_TALKFRIEND
    // Opt-in (chunk 1024, row 0x006E2ED0 CTCTalk::GetCurrentLipSyncFrameData): retail TLC CTCTalk reads PAttachedEmitter (+0x0C) in
    // place (`lea edi,[ecx+0Ch] ; call GetPItem`, 0x006E2FEB); no accessor for it is in either PDB name set, so the access is
    // modelled as a friend (unproven: the TLC header may have made the member public instead). No layout change.
    friend class CTCTalk;
#endif
protected:
    CIntelligentPointer<CThingSoundEmitter> PAttachedEmitter;
    CCharString VoiceName,ExtraTag;
#ifdef FABLE_RETAIL_BERSERK_CLEANUP_PASS29
public: void SetExtraTag(const CCharString& tag) { ExtraTag=tag; }
protected:
#endif
    short AnimCriteriaClipDistance;
    long LastMaterialIndex;
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals the declarations above lack, declaration only (no layout change; the
    // vtable already exists). Slots from the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots;
    // spellings from the Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x0066C2C0
public:
#endif
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER_VSLOTS3
// agentFAM05 (2026-11-11): retail virtual(s) this declaration lacks, declaration only (no layout change; the vtable exists). Slots from the TLC
// vtable (agentFAM03 proof, re-verified by agentFAM05); spellings from the Ego_r / FableWin name sets.
public:
    virtual void BulletTimeFrameUpdate();  // slot 10, 0x0066A910
public:
#endif // FABLE_RETAIL_CTCSOUNDPLAYER_VSLOTS3
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER_PLAYALLSOUNDEVENTS
    // Opt-in (chunk 853 text, re-ported by AUTH22 2026-11-22; row 0x0066B370): Ego_r.pdb / FableWin.pdb
    // ?PlayAllSoundEvents@CTCSoundPlayer@@QAEXXZ (Ego_r 0x005749A0, FableWin 0x01E7E870; sole retail caller FrameUpdate 0x0066BBF0, tail
    // jump) and the private ?InternalPlaySoundEvent@CTCSoundPlayer@@AAEKAAVCASoundBank@@ABVCCharString@@_N22J@Z 0x0066B120 (landed).
    // Declarations only: no layout change.
public:
    void PlayAllSoundEvents();                                           // 0x0066B370
private:
    unsigned long InternalPlaySoundEvent(class CASoundBank&,const CCharString&,bool,bool,bool,long);   // 0x0066B120
#endif
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER_INTERNALPLAYSOUNDEVENTATPOS
    // Retail opt-in (Codex pass 24, 0x0066A9E0). The call at 0x0066AA5C
    // and both PDBs identify this private sound-event entry point.
private:
    unsigned long InternalPlaySoundEventAtPos(class CASoundBank&, const C3DVector&, const CCharString&, bool, bool);
#endif
};
FABLE_STATIC_ASSERT_AT(92, sizeof(CTCSoundPlayer)==36);
#endif
