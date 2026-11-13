#ifndef FABLE_ENGINE_CTCTAVERNGAMESPOTTHEADDITION_H
#define FABLE_ENGINE_CTCTAVERNGAMESPOTTHEADDITION_H
// New shared header (w22 agent2002, row 0x007D8570 CTCTavernGameSpotTheAddition::ProcessInput): the spot-the-addition tavern game.
// RTTI .?AVCTCTavernGameSpotTheAddition@@ : CTCTavernGame; vtable 0x0126F814 (slot 28 ProcessInput 0x007D8570). Members: Ego_r.pdb
// (ghidra_out/struct_layouts_egor.tsv) names and order, at the retail offsets the landed FrameUpdate 0x007D8E20 uses (the PDB's
// Dinkumware vector KnickKnacks is 12 bytes in retail, so everything after it sits 4 lower): CurrentRound +0xf0 .. MaxY +0x184.
// GamePositionRHSet (+0x14d) / GamePositionPos (+0x165) follow the bool FirstRound with no padding: C3DVector is 1-aligned in retail
// (EXTRA_RULES), so this header needs FABLE_RETAIL_C3DVECTOR_PACKED (the asserts below fail without it). ProcessInput reads
// GamePositionRHSet's Forward in place (+0x159..+0x161) and passes &GamePositionRHSet / &GamePositionPos to PositionMesh.
// Methods: the Ego_r.pdb / FableWin.pdb `@CTCTavernGameSpotTheAddition@@` set with the PDB access (FindTarget / PositionCover /
// UpdatePointerScale private, UpdateHighlightedObject public). Needs FABLE_RETAIL_CTCTAVERNGAME (+ its chain).
#include <stddef.h>
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCTavernGame.h"
class CThingObject;
class CInputEvent;
class CProcessedInput;
class CInputProcessTavernGame;
class CEnginePrimitivePrepareInfo;

struct KnickKnack {                                                 // 16 bytes (Ego_r.pdb KnickKnack, key U)
    C3DVector     Pos;                                              // +0x00
    CThingObject* Thing;                                            // +0x0C
    KnickKnack() { }
};

class CTCTavernGameSpotTheAddition : public CTCTavernGame {
public:
    CTCTavernGameSpotTheAddition(CThing&);                                          // ??0CTCTavernGameSpotTheAddition@@QAE@AAVCThing@@@Z
    virtual void OnInitialActivate();                                               // ?OnInitialActivate@CTCTavernGameSpotTheAddition@@UAEXXZ
    virtual void FrameUpdate();                                                     // 0x007D8E20
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
    virtual CCharString GetClassTypeName() const;
    virtual void ProcessInput(const CInputEvent&, CProcessedInput&, CInputProcessTavernGame*);   // slot 28, 0x007D8570
    virtual void EnterGame();                                                       // 0x007D7FD0
    virtual void LeaveGame();
    virtual void NewGame();                                                         // 0x007D80C0
    virtual void NewRound();
    virtual void EndRound();
    virtual void QuitMidGame();
    virtual bool UpdateValues();                                                    // 0x007D8AB0
    virtual CWideString GetWinPhrase();
    virtual CWideString GetBestScore();
    virtual CWideString GetCurrentScore();
    virtual CWideString GetRequiredScore();
    virtual CWideString GetAdditionalInfo();                                        // 0x007D8160
    virtual void DrawBetAndMoney(const CEnginePrimitivePrepareInfo&, float, float, float, float, float, float);
    void UpdateHighlightedObject();                                                 // 0x007D8290 ?UpdateHighlightedObject@CTCTavernGameSpotTheAddition@@QAEXXZ
private:
    CThingObject* FindTarget(float range);                                          // 0x007D7C40 ?FindTarget@CTCTavernGameSpotTheAddition@@AAEPAVCThingObject@@M@Z
    void PositionCover(float amount);                                               // 0x007D7F10 ?PositionCover@CTCTavernGameSpotTheAddition@@AAEXM@Z
    void UpdatePointerScale();                                                      // 0x007D8C20 ?UpdatePointerScale@CTCTavernGameSpotTheAddition@@AAEXXZ
public:
    long            CurrentRound;                 // +0xf0
    float           CurrentTime;                  // +0xf4
    CThing*         Cover;                        // +0xf8
    float           CoverTimer;                   // +0xfc
    bool            UpdatedKnickKnacks;           // +0x100
    CThingObject*   Pointer;                      // +0x104
    float           PointerScale;                 // +0x108
    float           DistFromObject;               // +0x10c
    std::vector<KnickKnack> KnickKnacks;          // +0x110
    C3DVector       BoardPos;                     // +0x11c
    CRightHandedSet BoardRHSet;                   // +0x128
    long            HeldTime;                     // +0x140
    long            Selected;                     // +0x144
    CThingObject*   NewKnickKnack;                // +0x148
    bool            FirstRound;                   // +0x14c
    CRightHandedSet GamePositionRHSet;            // +0x14d
    C3DVector       GamePositionPos;              // +0x165
    long            GamePositionActionPointIndex; // +0x174
    float           MinX;                         // +0x178
    float           MaxX;                         // +0x17c
    float           MinY;                         // +0x180
    float           MaxY;                         // +0x184
};
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameSpotTheAddition, CurrentRound) == 0xf0);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameSpotTheAddition, Pointer) == 0x104);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameSpotTheAddition, KnickKnacks) == 0x110);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameSpotTheAddition, NewKnickKnack) == 0x148);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameSpotTheAddition, GamePositionRHSet) == 0x14d);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameSpotTheAddition, GamePositionPos) == 0x165);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameSpotTheAddition, MinX) == 0x178);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameSpotTheAddition, MaxY) == 0x184);
#endif
