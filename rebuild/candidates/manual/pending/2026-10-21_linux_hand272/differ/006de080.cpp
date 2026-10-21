// CTCTrophy::StartMinigame() -- count the competitors, clear the result list, give every live competitor the opinion-reaction root and (when the def names one) a long-lived emote icon, then start the timer, persist the trophy, play its start sound, show the mini-game HUD with the competitor count and bump the hero's mini-game counter
// Grade: DIFFER(490v497) -- near miss; only gap: retail tests the owner's TC-0x10 presence bit twice (HasTC then the inlined GetInterface, `mov ecx,10000h ; test ecx,edx` x2) where VC7.1 here CSEs the two into one `test [eax+20h],10000h`; tried bool/word HasTC, bool lookup, commuted mask, nested if
// Retail: Fable.exe @ 0x006DE080, 497 bytes (plain `ret`, no EH frame).
// Callees: list clear 0x00510D40, CBaseIntelligentPointer::GetPItem 0x00A01B50, CCharString ctor/dtor 0x0099EBF0/0x0099EAE0,
//   CThing reaction call 0x008302D0, LowerBound 0x0040F020, CTCEmoteIcon::SetEmoteIcon 0x0071AAB0, __ftol2 0x00BFEA70,
//   CThing::SetAsGameStatePersisted 0x004C7880, CSoundInfo::PlaySound 0x0066BD00, CDrawQuestInfo::DisplayMiniGameInfo 0x00654593,
//   CDrawQuestInfo::UpdateMiniGame 0x00644EFD, CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetHeroCreature 0x00487DC0
class CTCBase;
struct CCharString {
    void* m_data;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type);               // 0x0040F020
};
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                             // +0x91 bit0
    void AddOpinionReaction(const CCharString& name, long flags);   // 0x008302D0
    void SetAsGameStatePersisted(bool persisted);       // 0x004C7880
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline void GetInterface(long interfaceType, T** out)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long key = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            *out = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
        }
    }
};
struct CBaseIntelligentPointer {
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CListNode {
    CListNode* Next;
    CListNode* Prev;
    CBaseIntelligentPointer Item;                       // +0x08
};
struct CResultList {
    CListNode* Head;
    void Clear();                                       // 0x00510D40
};
struct CTCEmoteIcon {
    void SetEmoteIcon(long priority, long icon, float time, float scale, unsigned char alpha);   // 0x0071AAB0
};
struct CSoundInfo {
    void PlaySound(void* sound, long a, long b, long c, long d);   // 0x0066BD00
};
struct CTrophyDef {
    unsigned char _pad00[0x28];
    float TimeLimitSeconds;                             // +0x28
    unsigned char _pad2c[0x38 - 0x2c];
    long EmoteIcon;                                     // +0x38
    unsigned char _pad3c[0x48 - 0x3c];
    unsigned char StartSound[4];                        // +0x48
};
struct CDrawQuestInfo {
    void DisplayMiniGameInfo(bool show, long type, CThing* thing);   // 0x00654593
    void UpdateMiniGame(float fraction, long a, long count);         // 0x00644EFD
};
struct CWorldGlobals { unsigned char _pad[0x1c8]; CDrawQuestInfo* QuestInfo; };
extern CWorldGlobals* g_pWorld;                         // 0x013B8790
extern long g_FramesPerSecond;                          // 0x01375550
struct CPlayer { CThing* GetHeroCreature(); };          // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };    // 0x00449970
struct CGameGlobals { unsigned char _pad[0x1c]; CPlayerManager* Players; };
extern CGameGlobals* g_pGame;                           // 0x013B86A0
struct CTCHeroStats { unsigned char _pad[0xd8]; long MiniGamesPlayed; };
struct CTCTrophy {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CTrophyDef* Def;                                    // +0x0c
    unsigned char _pad10[0x14 - 0x10];
    bool Started;                                       // +0x14
    unsigned char _pad15[0x1c - 0x15];
    long TimeLeft;                                      // +0x1c
    long TimeLimit;                                     // +0x20
    long CompetitorsLeft;                               // +0x24
    long CompetitorCount;                               // +0x28
    unsigned char _pad2c[0x34 - 0x2c];
    CListNode* Competitors;                             // +0x34 (STLport list sentinel)
    CResultList Results;                                // +0x38
    void StartMinigame();
};

void CTCTrophy::StartMinigame()
{
    long count = 0;
    for (CListNode* n = Competitors->Next; n != Competitors; n = n->Next)
        ++count;
    CompetitorsLeft = count;
    CompetitorCount = count;
    Results.Clear();
    CListNode* head = Competitors;
    for (CListNode* node = head->Next; node != head; node = node->Next) {
        CBaseIntelligentPointer& competitor = node->Item;
        if (competitor.GetPItem() && !competitor.GetPItem()->Dead) {
            competitor.GetPItem()->AddOpinionReaction("SG_OPINION_REACTION_ROOT", 0);
            CTCEmoteIcon* icon;
            competitor.GetPItem()->GetInterface(0x45, &icon);
            if (Def->EmoteIcon)
                icon->SetEmoteIcon(2, Def->EmoteIcon, 1000000.0f, 1.0f, 0xff);
        }
    }
    Started = true;
    long frames = (long)((float)g_FramesPerSecond * Def->TimeLimitSeconds);
    TimeLeft = frames;
    TimeLimit = frames;
    ParentThing->SetAsGameStatePersisted(true);
    CThing* thing = ParentThing;
    CTrophyDef* def = Def;
    if (thing->HasTC(0x10)) {
        CSoundInfo* sound;
        thing->GetInterface(0x10, &sound);
        sound->PlaySound(def->StartSound, 1, 0, 0, -1);
    }
    g_pWorld->QuestInfo->DisplayMiniGameInfo(true, 6, ParentThing);
    g_pWorld->QuestInfo->UpdateMiniGame(1.0f, 0, CompetitorsLeft);
    CThing* hero = g_pGame->Players->GetMainPlayer()->GetHeroCreature();
    if (hero->HasTC(4)) {
        CTCHeroStats* stats;
        hero->GetInterface(4, &stats);
        ++stats->MiniGamesPlayed;
    }
}
