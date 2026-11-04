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
    unsigned long PlayAttachedCriteria(const CCharString&,bool,bool,bool,long);
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER_VOICENAME
    // Opt-in (chunk 765, row 0x0066A9D0): Ego_r.pdb / FableWin.pdb ?SetVoiceName@CTCSoundPlayer@@QAEXABVCCharString@@@Z (VoiceName +0x14).
    void SetVoiceName(const CCharString&);                               // 0x0066A9D0
#endif
protected:
    CIntelligentPointer<CThingSoundEmitter> PAttachedEmitter;
    CCharString VoiceName,ExtraTag;
    short AnimCriteriaClipDistance;
    long LastMaterialIndex;
};
FABLE_STATIC_ASSERT(sizeof(CTCSoundPlayer)==36);
#endif
