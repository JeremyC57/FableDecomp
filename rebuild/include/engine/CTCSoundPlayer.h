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
public:
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER
    // Retail opt-in (chunk 751, row 0x007E0D00 CTCEffectOnDie::OnDie): FableWin.pdb ?StatGetInterfaceType@CTCSoundPlayer@@SI?AW4ETCInterfaceType@@XZ
    // for CThing::GetTC<CTCSoundPlayer> (FableWin ??$GetTC@VCTCSoundPlayer@@@CThing@@QAE_NPAPAVCTCSoundPlayer@@@Z; retail looks up
    // interface 0x10 = TCI_SOUND_PLAYER inline). A declaration with its body only: no layout change.
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SOUND_PLAYER; }
#endif
#ifdef FABLE_RETAIL_CTCVOLUMECONTAINMENTTRACKER_UPDATE
    unsigned long PlayAttachedSample(const class CSoundPair&,const class CSoundInit&,bool,bool,bool);
#endif
#ifdef FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE
    void StopAttachedSound(unsigned long); // Both original PDBs; native0066add0.
#endif
    unsigned long PlayAttachedCriteria(const CCharString&,bool,bool,bool,long);
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER_PLAYATTACHEDSAMPLE
    // Opt-in (agentNF12, 2026-11-11, rows 0x0083DDC0 / 0x0083DF20): Ego_r.pdb / FableWin.pdb
    // ?PlayAttachedSample@CTCSoundPlayer@@QAEKABVCSoundPair@@ABVCSoundInit@@_N22@Z (retail 0x0066B280, out of line; the callers push the
    // three bools as 1, 0, 0). Declaration only.
    unsigned long PlayAttachedSample(const class CSoundPair& sound, const class CSoundInit& init, bool, bool, bool);   // 0x0066B280
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
};
FABLE_STATIC_ASSERT(sizeof(CTCSoundPlayer)==36);
#endif
