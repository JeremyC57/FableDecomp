// COpinionReactionManager::GetHeroStatsInOpinionFormat(EOpinion opinion) const -- the controlled hero's stat that feeds an opinion type, as a float: 0 morality (scaled by the morality maximum, plus appearance goodness, limited to -1..1), 1 renown level fraction, 2 scariness, 3 agreeableness, 4 attractiveness; 0 without a hero or hero stats
// Leaf: GetHeroStatsInOpinionFormat
// Retail: Fable.exe (TLC, VC7.1) @ 0x007009D0, 249 bytes + a 5-entry jump table (0x00700ACC), `ret 4`.
// Callees: CWorld::PeekMyPlayer 0x0049D9D0, CPlayer::GetControlledCreature 0x00487DD0, CVectorMap<ETCInterfaceType,
//   CTCBase*>::LowerBound 0x004365B0 (const; inlined HasTC + GetTC for CTCHeroStats, type 4), CTCHeroStats::GetMoralityMax
//   0x0057A67E, `__fastcall` GFLimitReturn<float>(const float&, const float&, const float&) 0x0057EF86 (returns a reference).
// Globals: the 0.0f constant 0x0122DEDC.
// Layout: COpinionReactionManager World +0x04; CTCHeroStats retail (Ego_r.pdb - 8): Morality +0x28, RenownLevel +0x70,
//   RenownMaxLevel +0x74, Agreeableness +0x80, Attractiveness +0x9c, Scariness +0xa0, AppearanceGoodstrength +0xa4.
// Shape: dense switch -> jump table. Case 0 names the -1..1 limits first (retail stores both before the maximum call) and reads
//   Morality/AppearanceGoodstrength through inline getters (the getter return copies Morality through edx into a stack slot
//   before `fild`).
class CTCBase;
struct CInterfaceMap {                     // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;  // 0x004365B0 (the const instantiation)
};
class CThing { public:
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];       // +0x20 std::bitset of interface types
    CInterfaceMap  m_interfaces;           // +0x44
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline const T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        const CInterfaceMap::SEntry* sel = (it == map->last || interfaceType < it->type) ? map->last : it;
        return (const T*)sel->component;
    }
};
class CPlayer { public:
    CThing* GetControlledCreature() const; // 0x00487DD0
};
class CWorld { public:
public:
    const CPlayer* PeekMyPlayer() const;   // 0x0049D9D0
};
class CTCHeroStats { public:
    unsigned char _pad00[0x28];
    long          Morality;                // +0x28
    unsigned char _pad2c[0x70 - 0x2c];
    long          RenownLevel;             // +0x70
    long          RenownMaxLevel;          // +0x74
    unsigned char _pad78[0x80 - 0x78];
    float         Agreeableness;           // +0x80
    unsigned char _pad84[0x9c - 0x84];
    float         Attractiveness;          // +0x9c
    float         Scariness;               // +0xa0
    float         AppearanceGoodstrength;  // +0xa4
    long GetMoralityMax() const;           // 0x0057A67E
    long GetMorality() const { return Morality; }
    float GetAppearanceGoodness() const { return AppearanceGoodstrength; }
};
template <class T> const T& __fastcall GFLimitReturn(const T& value, const T& low, const T& high);   // 0x0057EF86 (float)

enum EOpinion { OPINION_MORALITY, OPINION_RENOWN, OPINION_SCARINESS, OPINION_AGREEABLENESS, OPINION_ATTRACTIVENESS };

class COpinionReactionManager { public:
    void*   __vftable;
    CWorld* World;                         // +0x04
    float GetHeroStatsInOpinionFormat(EOpinion opinion) const;
};

float COpinionReactionManager::GetHeroStatsInOpinionFormat(EOpinion opinion) const
{
    CThing* hero = World->PeekMyPlayer()->GetControlledCreature();
    if (hero && hero->HasTC(4)) {
        const CTCHeroStats* stats = hero->GetTC<CTCHeroStats>(4);
        switch (opinion) {
        case OPINION_MORALITY: {
            float high = 1.0f;
            float low = -1.0f;
            float morality = (float)stats->GetMorality();
            float goodness = stats->GetAppearanceGoodness();
            float value = morality / stats->GetMoralityMax() + goodness;
            return GFLimitReturn(value, low, high);
        }
        case OPINION_RENOWN:
            return (float)stats->RenownLevel / stats->RenownMaxLevel;
        case OPINION_SCARINESS:
            return stats->Scariness;
        case OPINION_AGREEABLENESS:
            return stats->Agreeableness;
        case OPINION_ATTRACTIVENESS:
            return stats->Attractiveness;
        }
    }
    return 0.0f;
}