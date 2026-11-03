#ifndef FABLE_ENGINE_CTCSOUNDPLAYER_H
#define FABLE_ENGINE_CTCSOUNDPLAYER_H
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER
#include "engine/ETCInterfaceType.h"
#endif
class CThingSoundEmitter;
// Original complete36-byte class and native component16 receiver.
class CTCSoundPlayer : public CTCBase {
public:
    unsigned long PlayAttachedCriteria(const CCharString&,bool,bool,bool,long);
#ifdef FABLE_RETAIL_CTCSOUNDPLAYER
    // Chunk 728 round 2 (row 0x006D0830 CTCWife::GlobalUpdate): TCI_SOUND_PLAYER (16; GlobalUpdate tests bit 16 of UsingComponents
    // +0x20, `test ecx,10000h`) and the Ego_r.pdb / FableWin.pdb members it calls out of line:
    // ?PlayAttachedDialogue@CTCSoundPlayer@@QAEKABVCCharString@@_N111@Z 0x0066BED0, ?IsAttachedSoundPlaying@CTCSoundPlayer@@QBE_NK@Z
    // 0x0066AE10, ?StopAttachedSound@CTCSoundPlayer@@QAEXK@Z 0x0066ADD0. Declarations only: no layout change.
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SOUND_PLAYER; }
    unsigned long PlayAttachedDialogue(const CCharString& group, bool, bool, bool, bool);   // 0x0066BED0
    bool IsAttachedSoundPlaying(unsigned long sampleID) const;           // 0x0066AE10
    void StopAttachedSound(unsigned long sampleID);                      // 0x0066ADD0
    // Chunk 728 round 2 (row 0x006D1E30): Ego_r.pdb / FableWin.pdb ?SetSpecificSampleVolume@CTCSoundPlayer@@QAEXKM@Z (0x0066AFF0). Declaration only.
    void SetSpecificSampleVolume(unsigned long sampleID, float volume);  // 0x0066AFF0
#endif
protected:
    CIntelligentPointer<CThingSoundEmitter> PAttachedEmitter;
    CCharString VoiceName,ExtraTag;
    short AnimCriteriaClipDistance;
    long LastMaterialIndex;
};
FABLE_STATIC_ASSERT(sizeof(CTCSoundPlayer)==36);
#endif
