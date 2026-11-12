#ifndef FABLE_ENGINE_NSPEECHGAINMANAGER_H
#define FABLE_ENGINE_NSPEECHGAINMANAGER_H
// New header (chunk 1023, row 0x006E3EC0 NSpeechGainManager::Initialise): the speech gain manager's dialogue-layer def and per-layer
// response rates, and the namespace's statics.
// CDialogueLayerDef: Ego_r.pdb CDialogueLayerDef (64 bytes; ghidra_out/struct_layouts_egor.tsv) derives CDefClassBase (virtual
//   ??1 / Copy / GetSizeofClass / Transfer in both PDB name sets); no container members, so the PDB offsets hold in retail: the
//   packed 0x25-byte CDefClassBase, then six floats from +0x28 (retail 0x006E3F65 / 0x006E3F6E read +0x2C / +0x28 of element 0,
//   elements 0x40 apart).
// CResponseTimes: Ego_r.pdb ??0CResponseTimes@NSpeechGainManager@@QAE@XZ and ??0..QAE@MM@Z (8 bytes; members
//   AttackMaxDeltaPerFrame +0 / ReleaseMaxDeltaPerFrame +4 as the landed UpdateDialogueElement 0x006E3440 reads them). Both
//   constructors are inline in retail: the default one stores nothing (0x006E3F3E passes an unwritten frame temporary to
//   _M_fill_insert); the (MM) one turns the layer's attack / release times in seconds into gain deltas per frame
//   (0x006E3F6E..0x006E3FC3, body reconstructed from those bytes).
// Statics (descriptive names, the established spellings of the landed same-TU rows; no data-symbol name source):
//   g_LastUpdateFrame 0x013BAB8C, g_LayerSpeakerCounts 0x013BAB94 (vector<unsigned short>: its fill insert is the shared COMDAT
//   vector<unsigned short>::_M_fill_insert 0x005149F0, called at 0x006E3F06), g_ActiveDialogue 0x013BABA0, g_ResponseTimes 0x013BABA4.
#include "rebuild_abi.h"
#include "fable_compiler.h"
#include "stlport/fable_containers.h"
#include "engine/CDefClassBase.h"
#include "engine/CMainGameComponent.h"
class CWorld;
class CThing;
namespace NSpeechGainManager {
#ifdef FABLE_RETAIL_NSPEECHGAINMANAGER_ONDIALOGUESTART
    // Opt-in (wf10 row 0x006E3580 CTCTalk::PlaySpeech): Ego_r.pdb / FableWin.pdb
    // ?OnDialogueStart@NSpeechGainManager@@YI_NABVCWorld@@AAVCThing@@KW4EDialogueLayer@1@@Z (retail 0x006E31E0, __fastcall). Declaration only.
enum EDialogueLayer;
bool __fastcall OnDialogueStart(const CWorld& world, CThing& thing, unsigned long sampleID, EDialogueLayer layer);   // 0x006E31E0
#endif

class CDialogueLayerDef : public CDefClassBase {
public:
    float ResponseTimeSecsAttack;                                        // +0x28
    float ResponseTimeSecsRelease;                                       // +0x2c
    float GainLevelHigh;                                                 // +0x30
    float GainLevelMid;                                                  // +0x34
    float GainLevelLow;                                                  // +0x38
    float FocusShiftDistanceThreshold;                                   // +0x3c
#ifdef FABLE_RETAIL_NSPEECHGAINMANAGER_CDIALOGUELAYERDEF_COPY
// agentFAM05 (2026-11-11): the retail Copy override (CDefClassBase slot 19; TLC vtable, agentFAM03 proof, re-verified by agentFAM05) and the
// out-of-line assignment it tail-calls (0x004663AE ??4CDialogueLayerDef@NSpeechGainManager@@QAEAAV01@ABV01@@Z, Ego_r name set and the
// landed binding). Declarations only: no layout change.
    virtual void Copy(const CDefClassBase*);                             // slot 19 0x0046656A
    CDialogueLayerDef& operator=(const CDialogueLayerDef&);              // 0x004663AE
#endif // FABLE_RETAIL_NSPEECHGAINMANAGER_CDIALOGUELAYERDEF_COPY
};
FABLE_STATIC_ASSERT(sizeof(CDialogueLayerDef) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CDialogueLayerDef, ResponseTimeSecsAttack) == 0x28);

class CResponseTimes {
public:
    CResponseTimes() { }
    FABLE_FORCEINLINE CResponseTimes(float attack_secs, float release_secs)
    {
        float frame_secs = 1.0f / CMainGameComponent::GetConstantFPS();
        if (attack_secs > frame_secs)
            AttackMaxDeltaPerFrame = frame_secs / attack_secs;
        else
            AttackMaxDeltaPerFrame = 1.0f;
        if (release_secs > frame_secs)
            ReleaseMaxDeltaPerFrame = frame_secs * (-1.0f / release_secs);
        else
            ReleaseMaxDeltaPerFrame = -1.0f;
    }
    float AttackMaxDeltaPerFrame;                                        // +0x00
    float ReleaseMaxDeltaPerFrame;                                       // +0x04
};

class CActiveDialogueInfo;

extern long g_LastUpdateFrame;                                           // 0x013BAB8C
extern std::vector<unsigned short> g_LayerSpeakerCounts;                 // 0x013BAB94
extern std::list<CActiveDialogueInfo> g_ActiveDialogue;                  // 0x013BABA0
extern std::vector<CResponseTimes> g_ResponseTimes;                      // 0x013BABA4

void __fastcall Initialise(const CWorld& world);                         // 0x006E3EC0

}
#endif // FABLE_ENGINE_NSPEECHGAINMANAGER_H
