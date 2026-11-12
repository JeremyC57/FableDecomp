#ifndef FABLE_ENGINE_CTCTAVERNGAMESHOVEHAPENNY_H
#define FABLE_ENGINE_CTCTAVERNGAMESHOVEHAPENNY_H
// New shared header (w22 agent2007, row 0x007CF9A0 CTCTavernGameShoveHaPenny::ProcessInput): the shove-ha'penny tavern game.
// RTTI .?AVCTCTavernGameShoveHaPenny@@ : CTCTavernGameCoinBase (ghidra_out/rtti_bases.tsv); vtable 0x0126F2F4 (rebuild/manifest/
// rtti-vtables.tsv): slot 0 ??_G 0x007D2240, 6 OnInitialActivate 0x007D04F0, 9 FrameUpdate 0x007D00F0, 22 GetClassTypeName 0x007D2220,
// 28 ProcessInput 0x007CF9A0, 29 EnterGame 0x007CF530, 30 LeaveGame 0x007CF460, 31 NewGame 0x007CF4A0, 32 NewRound 0x007CF7E0,
// 33 EndRound 0x007CF4C0, 34 QuitMidGame 0x007CF520, 40 UpdateValues 0x007D0850, 43 GetBestScore 0x007CF5B0, 44 GetCurrentScore
// 0x007CF690, 45 GetRequiredScore 0x007CF740, 46 GetAdditionalInfo 0x007CFF40. Methods and signatures: the Ego_r.pdb
// `@CTCTavernGameShoveHaPenny@@` set (CalcScores 0x007CF860 public non-virtual; ctor ??0CTCTavernGameShoveHaPenny@@QAE@AAVCThing@@@Z
// 0x007D09C0). Members: Ego_r.pdb RoundsPlayed / AverageJoystickPos at the retail offsets +0x148 / +0x14C (landed EnterGame 0x007CF530
// and GetAdditionalInfo 0x007CFF40 read RoundsPlayed +0x148; ProcessInput 0x007CF9A0 blends AverageJoystickPos +0x14C / +0x150).
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/C2DVector.h"
#include "engine/CTCTavernGameCoinBase.h"
class CInputEvent;
class CProcessedInput;
class CInputProcessTavernGame;

class CTCTavernGameShoveHaPenny : public CTCTavernGameCoinBase {
public:
    CTCTavernGameShoveHaPenny(CThing&);                                  // 0x007D09C0
    virtual void OnInitialActivate();                                    //  6 0x007D04F0
    virtual void FrameUpdate();                                          //  9 0x007D00F0
    virtual CCharString GetClassTypeName() const;                        // 22 0x007D2220
    virtual void ProcessInput(const CInputEvent&, CProcessedInput&, CInputProcessTavernGame*);   // 28 0x007CF9A0
    virtual void EnterGame();                                            // 29 0x007CF530
    virtual void LeaveGame();                                            // 30 0x007CF460
    virtual void NewGame();                                              // 31 0x007CF4A0
    virtual void NewRound();                                             // 32 0x007CF7E0
    virtual void EndRound();                                             // 33 0x007CF4C0
    virtual void QuitMidGame();                                          // 34 0x007CF520
    virtual bool UpdateValues();                                         // 40 0x007D0850
    virtual CWideString GetBestScore();                                  // 43 0x007CF5B0
    virtual CWideString GetCurrentScore();                               // 44 0x007CF690
    virtual CWideString GetRequiredScore();                              // 45 0x007CF740
    virtual CWideString GetAdditionalInfo();                             // 46 0x007CFF40
    void CalcScores();                                                   // ?CalcScores@CTCTavernGameShoveHaPenny@@QAEXXZ 0x007CF860
    long      RoundsPlayed;                                              // +0x148
    C2DVector AverageJoystickPos;                                        // +0x14c
};
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameShoveHaPenny, RoundsPlayed) == 0x148);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameShoveHaPenny, AverageJoystickPos) == 0x14c);
#endif
