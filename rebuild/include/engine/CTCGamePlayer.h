#ifdef FABLE_RETAIL_CDRAWBETTINGHUD_PASS30
#ifndef FABLE_ENGINE_CTCGAMEPLAYER_H
#define FABLE_ENGINE_CTCGAMEPLAYER_H

#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"

class CTCTavernGame;
class CThing;

// Ego_r original declaration and member layout, also confirmed by FableWin.
class CTCGamePlayer : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_GAME_PLAYER; }
    CTCTavernGame* GetGame() const; // 0x005EDC90

private:
    CTCTavernGame* Game;
    CIntelligentPointer<CThing> GameTable;
};

#endif
#endif
