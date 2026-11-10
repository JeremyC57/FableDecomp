// CTCDiceGame::~CTCDiceGame() -- the compiler-generated destructor: ~CBaseIntelligentPointer on CurrentPlayer (+0x10), then the out-of-line ~CTCBase 0x00686830 as the tail jump; no vtable store
// Leaf: ??1CTCDiceGame@@UAE@XZ
// Retail: Fable.exe @ 0x004D607C, 19 bytes. Identity: called by the scalar deleting destructors of CTCDiceGame (0x004DB417, slot 0 of
//   vtable 0x0124093C) and of CTCPlot (0x004DB461, vtable 0x012409B4; RTTI CTCPlot : CTCDiceGame, which adds no members, so its
//   destructor is this one); the manifest names it CTCDiceGame::~CTCDiceGame. Ego_r.pdb CTCDiceGame : CTCBase: NumberOfPlayers +0x0C,
//   CurrentPlayer +0x10 (CIntelligentPointer<CThing>); 24 bytes (no STL members: retail keeps the offsets).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (Ego_r.pdb / FableWin.pdb ??1CBaseIntelligentPointer@@UAE@XZ),
//   CTCBase::~CTCBase 0x00686830 (??1CTCBase@@UAE@XZ, tail jump).
// Model: no engine header models CTCDiceGame, so it is declared here over the shared engine/CTCBase.h and engine/CIntelligentPointer.h
//   (retail branches); spellings from Ego_r.pdb. The class declares no destructor.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#include "engine/CTCBase.h"
class CTCDiceGame : public CTCBase {                                    // vtable 0x0124093C
public:
    static CTCBase* FABLE_FASTCALL Alloc(CThing& thing);
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
    void SetCurrentPlayer(CThing* player);
    long NumberOfPlayers;                                               // +0x0C
    CIntelligentPointer<CThing> CurrentPlayer;                          // +0x10
};
FABLE_STATIC_ASSERT(sizeof(CTCDiceGame) == 0x18);

// Emission harness (not a retail function): a qualified call makes VC7.1 emit the implicit destructor's COMDAT, as the deleting
// destructors' calls do in retail.
void DestroyDiceGame(CTCDiceGame* game)
{
    game->CTCDiceGame::~CTCDiceGame();
}
