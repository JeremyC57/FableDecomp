// CTCDiceGame::SetCurrentPlayer(CThing* player) -- CurrentPlayer (+0x10) = CIntelligentPointer<CThing>(player), through a stack temporary
// Leaf: ?SetCurrentPlayer@CTCDiceGame@@QAEXPAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00929550, oracle length 71 bytes, `ret 4`.
// Identity: Ego_r.pdb / FableWin.pdb ?SetCurrentPlayer@CTCDiceGame@@QAEXPAVCThing@@@Z; Ego_r CTCDiceGame: NumberOfPlayers +0xC,
//   CurrentPlayer +0x10 (CIntelligentPointer<CThing>). Callers 0x009289B0 (CAIStateGroup_BanditPlot::Act, ecx = DiceGame +0x18,
//   creature pushed) and 0x00928E20 (CAIStateGroup_BanditPlayDice::OnReplace) both bind this symbol (rebuild/oracles/reloc_bindings.tsv).
//   The manifest name CVillageTaskCarryCrate::AssignCreatureToTask is a byte-identical folded twin's label.
// Body: the (T*) constructor of a temporary CIntelligentPointer<CThing> (base ctor 0x00A01B10, vptr 0x01238C6C, SetPItem 0x00A01B90),
//   the inline operator= (GetPItem 0x00A01B50 on the temporary, SetPItem on the member), then ~CBaseIntelligentPointer 0x00A01C10.
// Layout: CTCDiceGame modelled as the landed sibling 0x00928E20 models it (no shared header exists).
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "engine/CIntelligentPointer.h"

class CTCDiceGame { public:
    unsigned char _pad[0xc];
    long NumberOfPlayers;                          // +0x0c
    CIntelligentPointer<CThing> CurrentPlayer;     // +0x10
    void SetCurrentPlayer(CThing* player);
};

void CTCDiceGame::SetCurrentPlayer(CThing* player)
{
    CurrentPlayer = CIntelligentPointer<CThing>(player);
}
