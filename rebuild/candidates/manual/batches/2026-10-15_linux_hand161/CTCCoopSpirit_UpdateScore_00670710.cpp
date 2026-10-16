// CTCCoopSpirit::UpdateScore() -- once the spirit's score reaches 20: clear the +0x10 counter of every other player's hero interface-0xc7 component, then swap the spirit back to a hero
//
// Retail 0x00670710, 135 bytes (plain `ret`). Callees: CTCBase::GetPlayerManager @ 0x00686D40
// (`[0x013B8A1C]+0x30`; manifest "CViewBase::GetDisplayEngine", propagated), CPlayerManager::
// GetMainPlayerNumber @ 0x00449700 (label "CWorld::DrawGetEnvironment" propagated), CPlayerManager::
// GetPlayer(long) @ 0x004498C0, CPlayer::GetHeroThing @ 0x00487DC0 (label propagated),
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CTCCoopSpirit::SwapToHero @ 0x0066FF20.
// Layout: Ego_r.pdb CTCCoopSpirit: Score +0x10.
// Shape: the guard `Score >= 20` around a four-player loop (index in edi, `this` in ebp); the main
// player number is re-fetched every pass; the hero's interface-0xc7 bit is a byte test of bitset word 6
// (`mov cl,[eax+38h] ; test cl,cl ; jns`) inside the out-parameter GetInterface (component in eax).
struct CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCCoopScore {                                       // the interface-0xc7 component
    unsigned char _pad[0x10];
    long          m_counter;                                // +0x10
};

struct CPlayer {
    CThing* GetHeroThing();                                 // 0x00487DC0
};

struct CPlayerManager {
    long GetMainPlayerNumber();                             // 0x00449700
    CPlayer* GetPlayer(long number);                        // 0x004498C0
};

struct CTCCoopSpirit {
    virtual void v00();
    CThing*       ParentThing;                              // +0x04 (CTCBase)
    unsigned char _pad08[4];
    long          NoMovementTimer;                          // +0x0c
    long          Score;                                    // +0x10
    CPlayerManager* GetPlayerManager();                     // 0x00686D40 (CTCBase)
    void SwapToHero();                                      // 0x0066FF20
    void UpdateScore();
};

void CTCCoopSpirit::UpdateScore()
{
    if (Score >= 20)
    {
        for (long player = 0; player < 4; ++player)
        {
            if (player != GetPlayerManager()->GetMainPlayerNumber())
            {
                CThing* hero = GetPlayerManager()->GetPlayer(player)->GetHeroThing();
                CTCCoopScore* score;
                if (hero && hero->GetInterface(&score, 0xc7))
                    score->m_counter = 0;
            }
        }
        SwapToHero();
    }
}
