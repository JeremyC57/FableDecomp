// CTCHeroGuide::OpenQuestSelectInterface() -- find the hero centre this guide belongs to; when the world allows it and the
// centre has quest cards on offer, clear the hero's quest-select flag and queue game event 0x1a (payload 0xd) for the main
// player, otherwise show "TEXT_GUI_QUESTS_NO_AVAILABLE" as a subtitle
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007800B0, 466 bytes (plain `ret`, /GS cookie 0x0139C8A8).
// Callees: CTCBase::GetPlayerManager const 0x00686D50, CPlayerManager::GetMainPlayer 0x00449960, CPlayer::GetHeroThing
// 0x00487DC0, the const interface LowerBound 0x004365B0 and the non-const one 0x0040F020, CBaseIntelligentPointer::GetPItem
// 0x00A01B50, CTCBase::GetWorld 0x00686A80 (+0xDA a bool), CTCHeroCentre::GetQuestCards 0x0077DBC0, CMemoryBuffer::Copy
// 0x009F1760, the second player-manager getter 0x00686D40, CPlayerManager::GetMainPlayerNumber 0x00449700,
// CMainGameComponent::AddGameEvent 0x0041649C, CCharString 0x0099EBF0 / 0x0099EAE0, CTCBase::GetGameSubsystem 0x00686D60
// (+0x1614C the font bank), CSubtitleRenderer::SetText 0x00492ED1 on the player GUI 0x013B8790 (+0x18),
// ~vector<CIntelligentPointer<CThing> > 0x004406D0, __security_check_cookie 0x00BFE9F9.
// Layout: CGameEvent as the landed CTCShop::OpenShoppingInterface 0x00773B20; CThing interface bitset +0x20, map +0x44,
// the centre pointer +0x170; the 0x33 component's centre pointer +0x0D; the 0x19 component's flag +0x14D (retail).
// Shape (LowerBound): the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation is defined in this TU (noinline),
//   as retail compiled it in the caller's TU; with its body visible VC7.1 packs the lookup keys and temporaries as retail.
class CThing;
class CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

struct CBaseIntelligentPointer {
    CThing* GetPItem() const;                           // 0x00A01B50
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x170 - 0x4c];
    CBaseIntelligentPointer HeroCentre;                 // +0x170
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline const T* FindTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        if (it == map->last || type < it->type)
            it = map->last;
        return (const T*)it->component;
    }
    template<class T> __forceinline T* GetTC(long type)
    {
        T* result;
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            result = (T*)it->component;
        }
        return result;
    }
};
struct CTCCentreOwner {                                 // interface 0x33
    unsigned char _pad00[0x0d];
    CBaseIntelligentPointer HeroCentre;                 // +0x0D
};
struct CThingVector {                                   // vector<CIntelligentPointer<CThing> >
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CThingVector();                                    // 0x004406D0
};
class CTCHeroCentre { public:                                  // interface 0x6A
    bool GetQuestCards(CThingVector* cards) const;      // 0x0077DBC0
};
struct CTCQuestSelect {                                 // the hero's interface 0x19 component
    unsigned char _pad00[0x14d];
    bool QuestSelectPending;                            // +0x14D
};
class CPlayer { public:
    CThing* GetHeroThing();                             // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer() const;                     // 0x00449960
    long GetMainPlayerNumber();                         // 0x00449700
};
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);  // 0x009F1760
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                                 // +0x00
    signed char   Player;                               // +0x04
    unsigned char Data[32];                             // +0x05
    unsigned char EndPos;                               // +0x25
    bool          Valid;                                // +0x26
    bool          Replacement;                          // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddULONG(unsigned long value) { Copy(4, &value); }
};
class CMainGameComponent { public:
    void AddGameEvent(const CGameEvent& event);         // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CFontBank;
class CSubtitleRenderer { public:
    void SetText(CFontBank* font, const CCharString& text, unsigned long colour, float time, bool a, bool b);   // 0x00492ED1
};
class CPlayerGui { public:
    unsigned char _pad00[0x18];
    CSubtitleRenderer Subtitles;                        // +0x18
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790
struct CGameSubsystem {
    unsigned char _pad00[0x1614c];
    CFontBank* SubtitleFont;                            // +0x1614C
};
class CWorld { public:
    unsigned char _pad00[0xda];
    bool QuestCardsEnabled;                             // +0xDA
};
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    CWorld* GetWorld() const;                           // 0x00686A80
    CPlayerManager* GetPlayerManager2() const;          // 0x00686D40
    CPlayerManager* GetPlayerManager() const;           // 0x00686D50
    CGameSubsystem* GetGameSubsystem() const;           // 0x00686D60
};
class CTCHeroGuide : public CTCBase { public:
    void OpenQuestSelectInterface();
};

void CTCHeroGuide::OpenQuestSelectInterface()
{
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
    const CThing* parent = ParentThing;
    const CThing* centre;
    if (parent->HasTC(0x33))
        centre = parent->FindTC<CTCCentreOwner>(0x33)->HeroCentre.GetPItem();
    else
        centre = parent->HeroCentre.GetPItem();
    if (centre && GetWorld()->QuestCardsEnabled && centre->HasTC(0x6a)) {
        const CTCHeroCentre* heroCentre = centre->FindTC<CTCHeroCentre>(0x6a);
        CThingVector cards;
        if (heroCentre->GetQuestCards(&cards)) {
            hero->GetTC<CTCQuestSelect>(0x19)->QuestSelectPending = false;
            CGameEvent event(0x1a);
            event.AddULONG(0xd);
            event.Player = (signed char)GetPlayerManager2()->GetMainPlayerNumber();
            g_pMainGameComponent->AddGameEvent(event);
        } else {
            CCharString text("TEXT_GUI_QUESTS_NO_AVAILABLE", -1);
            CGameSubsystem* subsystem = GetGameSubsystem();
            g_pPlayerGui->Subtitles.SetText(subsystem->SubtitleFont, text, 0x122, 4.0f, false, true);
        }
    }
}