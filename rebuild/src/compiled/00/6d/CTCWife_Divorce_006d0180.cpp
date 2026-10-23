// CTCWife::Divorce() -- once married, divorce the hero: unmarry, mark divorced, set the wife's divorced opinion masks and personality, reset the hero's marriage state and morality, sour the opinion of whoever is at the marital home and refresh the player GUI
// Leaf: ?Divorce@CTCWife
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D0180, 598 bytes (+ int3 padding), plain `ret`.
// Callees: CWorld::GetWorldFrame() 0x0049D870 (static); CTCWife::Unmarry(bool) 0x006CFF10; LowerBound 0x0040F020;
//   CTCCreatureOpinionOfHero::SetOpinionReactionMask 0x00681E30 / SetOpinionDeedMask 0x006822E0 / SetPersonalityOverride
//   0x006820E0 / ClearAllKnownDeeds 0x00681A70 / ApplyOpinionTransientOffset 0x00682140; CTCBase world/player-manager
//   getters 0x00686A70 / 0x00686D40; CPlayerManager::GetMainPlayer 0x00449970; CPlayer::GetHeroThing 0x00487DC0;
//   CTCHeroStats::SetMarriageState 0x0057BC6E / GetMoralityMax 0x0057A67E / SetMorality 0x0057C2E6; the home's
//   occupant getter 0x007EE540; the player GUI refresh 0x004378C0 on g_pPlayerGui 0x013B8790; CCharString 0x0099EBF0 /
//   ~CCharString 0x0099EAE0; frame rate 0x01375550.
// Layout (retail CTCWife = Ego_r.pdb less 4 after the 12-byte GiftsReceived map): ParentThing +0x04, PHeroMarriageDef +0x0c,
//   FrameGotMarriedToThePlayer +0x20, DivorcedHero +0x24, LoveAttitudeValue +0x8c; CHeroMarriageDef
//   HeroStatMoralityInOpinionFormChangePerDivorce +0xac; CTCCreatureOpinionOfHero LastOpinionReactionFrame +0x0c,
//   HeroOpinionEnemy +0x342; CTCHeroStats Morality +0x28.
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
// same-TU stand-in for the interface-map LowerBound (VC7.1 then knows the entry is not derived from the key)
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* lo = first;
    long count = last - first;
    while (count > 0) {
        long half = count >> 1;
        SEntry* mid = lo + half;
        if (mid->type < *type) {
            lo = mid + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return lo;
}
#pragma auto_inline(on)
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    unsigned char _pad92[0x1c9 - 0x92];
    unsigned char Flags1C9;                     // +0x1c9
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
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CWorld {
    static long GetWorldFrame();                // 0x0049D870
};
struct CPlayer {
    CThing* GetHeroThing() const;               // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
};
struct CTCCreatureOpinionOfHero {
    unsigned char _pad00[0x0c];
    long LastOpinionReactionFrame;              // +0x0c
    unsigned char _pad10[0x342 - 0x10];
    bool HeroOpinionEnemy;                      // +0x342
    CWorld* GetWorld();                         // 0x00686A70
    void SetOpinionReactionMask(const CCharString& name);   // 0x00681E30
    void SetOpinionDeedMask(const CCharString& name);       // 0x006822E0
    void SetPersonalityOverride(const CCharString& name, bool force);   // 0x006820E0
    void ClearAllKnownDeeds();                  // 0x00681A70
    void ApplyOpinionTransientOffset(long type, float amount, long a, long frames, float decay, long b);   // 0x00682140
};
struct CTCHeroStats {
    unsigned char _pad00[0x28];
    long Morality;                              // +0x28
    void SetMarriageState(CThing* spouse, long state);   // 0x0057BC6E
    long GetMoralityMax();                      // 0x0057A67E
    void SetMorality(long morality);            // 0x0057C2E6
};
struct CHomeOccupant {
    unsigned char _pad00[4];
    CThing* Thing;                              // +0x04
};
struct CTCHome {
    CHomeOccupant* GetOccupant();               // 0x007EE540
};
struct CPlayerGui {
    void Refresh(long a);                       // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                // 0x013B8790
extern long g_FrameRate;                        // 0x01375550
struct CHeroMarriageDef {
    unsigned char _pad00[0xac];
    float HeroStatMoralityInOpinionFormChangePerDivorce;   // +0xac
};
struct CTCWife {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CHeroMarriageDef* PHeroMarriageDef;         // +0x0c
    unsigned char _pad10[0x20 - 0x10];
    long FrameGotMarriedToThePlayer;            // +0x20
    bool DivorcedHero;                          // +0x24
    unsigned char _pad25[0x8c - 0x25];
    float LoveAttitudeValue;                    // +0x8c
    CPlayerManager* GetPlayerManager();         // 0x00686D40
    void Unmarry(bool notify);                  // 0x006CFF10
    void Divorce();
};

void CTCWife::Divorce()
{
    if (FrameGotMarriedToThePlayer > CWorld::GetWorldFrame())
        return;
    Unmarry(false);
    DivorcedHero = true;
    CTCCreatureOpinionOfHero* opinion;
    if (ParentThing->GetInterface(&opinion, 0x4f)) {
        {
            CCharString mask("OPINION_REACTION_MASK_WIFE_DIVORCED");
            opinion->SetOpinionReactionMask(mask);
        }
        {
            CCharString mask("OPINION_DEED_MASK_WIFE_DIVORCED");
            opinion->SetOpinionDeedMask(mask);
        }
        {
            CCharString personality("OPINION_PERSONALITY_WIFE_DIVORCED");
            opinion->SetPersonalityOverride(personality, true);
        }
        opinion->LastOpinionReactionFrame = opinion->GetWorld()->GetWorldFrame();
        opinion->ClearAllKnownDeeds();
        opinion->HeroOpinionEnemy = false;
    }
    ParentThing->Flags1C9 &= ~8;
    LoveAttitudeValue = 0.0f;
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
    if (hero && !(hero->Flags91 & 1)) {
        CTCHeroStats* stats;
        hero->GetInterface(&stats, 4);
        stats->SetMarriageState(ParentThing, 5);
        long morality = stats->Morality;
        const CHeroMarriageDef* def = PHeroMarriageDef;
        stats->SetMorality((long)(stats->GetMoralityMax() * def->HeroStatMoralityInOpinionFormChangePerDivorce + morality));
    }
    CTCHome* home;
    if (ParentThing->GetInterface(&home, 0x23)) {
        CHomeOccupant* occupant = home->GetOccupant();
        if (occupant) {
            CTCCreatureOpinionOfHero* other;
            if (occupant->Thing->GetInterface(&other, 0x4f)) {
                other->ApplyOpinionTransientOffset(0, -1.0f, 0, g_FrameRate * 300, 0.0f, 0);
                other->ApplyOpinionTransientOffset(3, -2.0f, 0, g_FrameRate * 300, -0.5f, 0);
            }
        }
    }
    g_pPlayerGui->Refresh(0);
}