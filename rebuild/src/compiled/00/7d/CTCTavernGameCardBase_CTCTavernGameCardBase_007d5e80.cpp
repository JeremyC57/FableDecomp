// CTCTavernGameCardBase::~CTCTavernGameCardBase() -- card tavern-game base component dtor (compiler-generated): ~ParticleEffects, ~Pack, ~Cards, then ~CTCTavernGame
// Leaf: ??1CTCTavernGameCardBase@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D5E80 (no vptr store: compiler-generated destructor).
// Identity: Ego_r.pdb ??1CTCTavernGameCardBase@@UAE@XZ; members GamePositionRHSet +0xf0, Cards +0x128, Pack +0x134,
//   ParticleEffects vector<CIntelligentPointer<CThing>> +0x148 (retail STLport offsets).
// Shape: emitted through a qualified `game->CTCTavernGameCardBase::~CTCTavernGameCardBase()` call (implicit dtor, no vptr store).
#define FABLE_RETAIL_CTCTAVERNGAME
#include "engine/CTCTavernGame.h"
#include "engine/CRightHandedSet.h"
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"

class CCard;
class CThingObject;
enum ECardGameType { };

class CTCTavernGameCardBase : public CTCTavernGame {
public:
    CTCTavernGameCardBase(CThing&);
    CRightHandedSet                              GamePositionRHSet;          // +0xf0
    C3DVector                                    GamePositionPos;            // +0x108
    long                                         GamePositionActionPointIndex; // +0x114
    float                                        MinX;                       // +0x118
    float                                        MaxX;                       // +0x11c
    float                                        MinY;                       // +0x120
    float                                        MaxY;                       // +0x124
    std::vector<CCard*>                          Cards;                      // +0x128
    std::vector<CCard*>                          Pack;                       // +0x134
    CThingObject*                                Pointer;                    // +0x140
    float                                        PointerScale;               // +0x144
    std::vector<CIntelligentPointer<CThing> >    ParticleEffects;            // +0x148
    float                                        Counter;                    // +0x154
    bool                                         RoundFinished;              // +0x158
    float                                        Times[52];                  // +0x15c
    float                                        BestTimes[52];              // +0x22c
    long                                         NumCards;                   // +0x2fc
    ECardGameType                                GameType;                   // +0x300
    long                                         CardOrder;                  // +0x304
    CCard*                                       SelectedCard;               // +0x308
    CCard*                                       HighlightedCard;            // +0x30c
    long                                         CurrentMovingCard;          // +0x310
};
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCardBase, Cards) == 0x128);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCardBase, ParticleEffects) == 0x148);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCardBase, Counter) == 0x154);

void DestroyCTCTavernGameCardBase(CTCTavernGameCardBase* game)
{
    game->CTCTavernGameCardBase::~CTCTavernGameCardBase();
}