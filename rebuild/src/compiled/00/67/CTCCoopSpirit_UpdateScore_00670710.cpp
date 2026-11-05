// CTCCoopSpirit::UpdateScore() -- once the spirit's score reaches 20: clear the +0x10 counter of every other player's hero interface-0xc7 component, then swap the spirit back to a hero
// Leaf: ?UpdateScore@CTCCoopSpirit@@QAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
//
// Retail 0x00670710, 135 bytes (plain `ret`). Callees: CTCBase::GetPlayerManager @ 0x00686D40
// (`[0x013B8A1C]+0x30`; manifest "CViewBase::GetDisplayEngine", propagated), CPlayerManager::
// GetMainPlayerNumber @ 0x00449700 (label "CWorld::DrawGetEnvironment" propagated), CPlayerManager::
// GetPlayer(long) @ 0x004498C0, CPlayer::GetPControlledCreature @ 0x00487DC0 (label propagated),
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CTCCoopSpirit::SwapToHero @ 0x0066FF20.
// Layout: Ego_r.pdb CTCCoopSpirit: Score +0x10.
// Shape: the guard `Score >= 20` around a four-player loop (index in edi, `this` in ebp); the main
// player number is re-fetched every pass; the hero's interface-0xc7 bit is a byte test of bitset word 6
// (`mov cl,[eax+38h] ; test cl,cl ; jns`) inside the out-parameter GetInterface (component in eax).
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };

struct CTCCoopScore {                                       // the interface-0xc7 component
    unsigned char _pad[0x10];
    long          m_counter;                                // +0x10
};

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                                 // 0x00487DC0
};

// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"

class CTCBase
{
public:
    virtual void v00();
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
};

class CTCCoopSpirit : public CTCBase { public:
    CThing*       ParentThing;                              // +0x04 (CTCBase)
    unsigned char _pad08[4];
    long          NoMovementTimer;                          // +0x0c
    long          Score;                                    // +0x10
    void UpdateScore();
private:
    void SwapToHero();                                      // 0x0066FF20 ?SwapToHero@CTCCoopSpirit@@AAEXXZ (Ego_r / FableWin, private)
};

void CTCCoopSpirit::UpdateScore()
{
    if (Score >= 20)
    {
        for (long player = 0; player < 4; ++player)
        {
            if (player != GetPlayerManager().GetMainPlayerNumber())
            {
                CThing* hero = GetPlayerManager().GetPlayer(player).GetPControlledCreature();
                CTCCoopScore* score;
                if (hero && hero->GetTC(TCI_COOP_SPIRIT, &score))
                    score->m_counter = 0;
            }
        }
        SwapToHero();
    }
}