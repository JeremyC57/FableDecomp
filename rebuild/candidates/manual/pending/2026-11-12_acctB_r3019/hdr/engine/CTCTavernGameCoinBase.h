#ifndef FABLE_ENGINE_CTCTAVERNGAMECOINBASE_H
#define FABLE_ENGINE_CTCTAVERNGAMECOINBASE_H
// New shared header (w22 agent2007, row 0x007CF9A0 CTCTavernGameShoveHaPenny::ProcessInput): the coin tavern games' common base.
// RTTI .?AVCTCTavernGameCoinBase@@ : CTCTavernGame (ghidra_out/rtti_bases.tsv); vtable 0x0126F224 (slot 0 ??_G 0x007D2200, 6
// OnInitialActivate 0x008EF1A0, 21 GetInterfaceType 0x007D21F0, 29 EnterGame 0x008EF390, 30 LeaveGame 0x008EF110, 31 NewGame
// 0x008EF170, 32 NewRound 0x008EF180, 33 EndRound 0x008EF190). Methods: the Ego_r.pdb `@CTCTavernGameCoinBase@@` set (AtRest,
// UpdateMeshes, UpdateCoins, UpdateCoinSounds(bool) 0x008EF910, ClearBoard 0x008EF950, AddNewCoin 0x008EFA00, FindObstacles 0x008EFB00).
// Members: Ego_r.pdb names and order at the retail offsets (Ego_r - 0x18: CTCTavernGame is 0xF0 in retail, Coins is a 12-byte
// STLport vector): Arrow +0xF0 .. Held +0x108 (landed ctor 0x007D09C0 zeroes +0xF0, +0xFC..+0x108), BoardPos +0x10C, BoardRHSet
// +0x118, BoardSize +0x130 (landed NewRound 0x007CF7E0 / CalcScores 0x007CF860), MousePosEffect +0x13C (ProcessInput 0x007CF9A0
// adds the movement to +0x13C / +0x140 and normalises it); sizeof 0x148 (8-aligned by CTCTavernGame's double; the derived
// CTCTavernGameShoveHaPenny::RoundsPlayed is at +0x148, landed EnterGame 0x007CF530). The constructor is inline in retail (the
// landed CTCTavernGameShoveHaPenny ctor 0x007D09C0 stores no CoinBase vtable): declared nowhere here.
// Needs FABLE_RETAIL_CTCTAVERNGAME (+ its chain), FABLE_RETAIL_C3DVECTOR(_PACKED), FABLE_RETAIL_C2DVECTOR, FABLE_RETAIL_CRIGHTHANDEDSET.
#include <stddef.h>
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCTavernGame.h"
#include "engine/Coin.h"
class CThingObject;

class CTCTavernGameCoinBase : public CTCTavernGame {
public:
    virtual void OnInitialActivate();                                    //  6 0x008EF1A0
    virtual ETCInterfaceType GetInterfaceType() const;                   // 21 0x007D21F0
    virtual void EnterGame();                                            // 29 0x008EF390
    virtual void LeaveGame();                                            // 30 0x008EF110
    virtual void NewGame();                                              // 31 0x008EF170
    virtual void NewRound();                                             // 32 0x008EF180
    virtual void EndRound();                                             // 33 0x008EF190
    bool AtRest(float);                                                  // ?AtRest@CTCTavernGameCoinBase@@QAE_NM@Z 0x008EF1B0
    void UpdateMeshes();                                                 // ?UpdateMeshes@CTCTavernGameCoinBase@@QAEXXZ 0x008EF2F0
    void UpdateCoins(float);                                             // ?UpdateCoins@CTCTavernGameCoinBase@@QAEXM@Z
    void UpdateCoinSounds(bool);                                         // ?UpdateCoinSounds@CTCTavernGameCoinBase@@QAEX_N@Z 0x008EF910
    void ClearBoard();                                                   // ?ClearBoard@CTCTavernGameCoinBase@@QAEXXZ 0x008EF950
    void AddNewCoin(const C3DVector&);                                   // ?AddNewCoin@CTCTavernGameCoinBase@@QAEXABVC3DVector@@@Z 0x008EFA00
    void FindObstacles();                                                // ?FindObstacles@CTCTavernGameCoinBase@@QAEXXZ 0x008EFB00
    CThingObject*     Arrow;                                             // +0xf0
    CThingObject*     Arrow1;                                            // +0xf4
    CThingObject*     Arrow2;                                            // +0xf8
    std::vector<Coin> Coins;                                             // +0xfc
    Coin*             Held;                                              // +0x108
    C3DVector         BoardPos;                                          // +0x10c
    CRightHandedSet   BoardRHSet;                                        // +0x118
    C3DVector         BoardSize;                                         // +0x130
    C2DVector         MousePosEffect;                                    // +0x13c
};
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCoinBase, Arrow) == 0xf0);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCoinBase, Coins) == 0xfc);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCoinBase, Held) == 0x108);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCoinBase, BoardRHSet) == 0x118);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCoinBase, BoardSize) == 0x130);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCoinBase, MousePosEffect) == 0x13c);
FABLE_STATIC_ASSERT(sizeof(CTCTavernGameCoinBase) == 0x148);
#endif
