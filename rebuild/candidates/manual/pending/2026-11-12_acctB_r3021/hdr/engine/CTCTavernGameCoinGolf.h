#ifndef FABLE_ENGINE_CTCTAVERNGAMECOINGOLF_H
#define FABLE_ENGINE_CTCTAVERNGAMECOINGOLF_H
// New shared header (w22 agent2022, row 0x007D3820 CTCTavernGameCoinGolf::ProcessInput): the coin-golf tavern game.
// RTTI .?AVCTCTavernGameCoinGolf@@ : CTCTavernGameCoinBase (ghidra_out/rtti_bases.tsv); vtable 0x0126F49C (rebuild/manifest/
// rtti-vtables.tsv): slot 0 ??_G 0x007D4D90, 6 OnInitialActivate 0x007D3F80, 9 FrameUpdate 0x007D2FF0, 11
// DrawPreparePrimitivesForRendering 0x007D4A60, 22 GetClassTypeName 0x007D4D70, 28 ProcessInput 0x007D3820, 29 EnterGame 0x007D34A0,
// 30 LeaveGame 0x007D4A90, 31 NewGame 0x007D2900, 32 NewRound 0x007D3510, 33 EndRound 0x007D2920, 34 QuitMidGame 0x007D29F0, 40
// UpdateValues 0x007D4410, 43 GetBestScore 0x007D2A00, 44 GetCurrentScore 0x007D2B70, 45 GetRequiredScore 0x007D2C20, 46
// GetAdditionalInfo 0x007D3700. Methods and signatures: the Ego_r.pdb / FableWin.pdb `@CTCTavernGameCoinGolf@@` set (EnableCourse(long)
// public, SetupForNewRound / FadeOutTableOccupants(bool) private, GetCamera(C3DVector&, C3DVector&) public 0x007D3560, ctor
// ??0CTCTavernGameCoinGolf@@QAE@AAVCThing@@@Z 0x007D49E0). Members: Ego_r.pdb names and order at the retail offsets (Ego_r - 0x18, the
// retail CTCTavernGameCoinBase being 0x148): ZAngle +0x148, XAngle +0x14C (landed GetCamera 0x007D3560; ProcessInput 0x007D3911 /
// 0x007D3927), CameraDistance +0x150, CameraFocus +0x154, AverageJoystickPos +0x160 (ProcessInput 0x007D39C1 / 0x007D3C92), CoinStartPos
// +0x168, GoalPos +0x170, GoalRadius +0x178, CurrentCourse +0x17C; sizeof 0x180 (Alloc 0x004D59DF `push 180h`).
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CTCTavernGameCoinBase.h"
class CInputEvent;
class CProcessedInput;
class CInputProcessTavernGame;
class CEnginePrimitivePrepareInfo;

class CTCTavernGameCoinGolf : public CTCTavernGameCoinBase {
public:
    CTCTavernGameCoinGolf(CThing&);                                      // 0x007D49E0
    virtual void OnInitialActivate();                                    //  6 0x007D3F80
    virtual void FrameUpdate();                                          //  9 0x007D2FF0
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);   // 11 0x007D4A60
    virtual CCharString GetClassTypeName() const;                        // 22 0x007D4D70
    virtual void ProcessInput(const CInputEvent&, CProcessedInput&, CInputProcessTavernGame*);   // 28 0x007D3820
    virtual void EnterGame();                                            // 29 0x007D34A0
    virtual void LeaveGame();                                            // 30 0x007D4A90
    virtual void NewGame();                                              // 31 0x007D2900
    virtual void NewRound();                                             // 32 0x007D3510
    virtual void EndRound();                                             // 33 0x007D2920
    virtual void QuitMidGame();                                          // 34 0x007D29F0
    virtual bool UpdateValues();                                         // 40 0x007D4410
    virtual CWideString GetBestScore();                                  // 43 0x007D2A00
    virtual CWideString GetCurrentScore();                               // 44 0x007D2B70
    virtual CWideString GetRequiredScore();                              // 45 0x007D2C20
    virtual CWideString GetAdditionalInfo();                             // 46 0x007D3700
    void EnableCourse(long);                                             // ?EnableCourse@CTCTavernGameCoinGolf@@QAEXJ@Z
    void GetCamera(C3DVector& camPos, C3DVector& lookAt);                // ?GetCamera@CTCTavernGameCoinGolf@@QAEXAAVC3DVector@@0@Z 0x007D3560
private:
    void SetupForNewRound();                                             // ?SetupForNewRound@CTCTavernGameCoinGolf@@AAEXXZ 0x007D2EF0
    void FadeOutTableOccupants(bool);                                    // ?FadeOutTableOccupants@CTCTavernGameCoinGolf@@AAEX_N@Z
public:
    float     ZAngle;                                                    // +0x148
    float     XAngle;                                                    // +0x14c
    float     CameraDistance;                                            // +0x150
    C3DVector CameraFocus;                                               // +0x154
    C2DVector AverageJoystickPos;                                        // +0x160
    C2DVector CoinStartPos;                                              // +0x168
    C2DVector GoalPos;                                                   // +0x170
    float     GoalRadius;                                                // +0x178
    long      CurrentCourse;                                             // +0x17c
};
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCoinGolf, ZAngle) == 0x148);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCoinGolf, AverageJoystickPos) == 0x160);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCoinGolf, CurrentCourse) == 0x17c);
FABLE_STATIC_ASSERT(sizeof(CTCTavernGameCoinGolf) == 0x180);
#endif
