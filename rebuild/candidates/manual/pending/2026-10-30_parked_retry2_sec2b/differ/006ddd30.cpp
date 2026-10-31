// Grade: DIFFER(842v841) 194 difflines (was 202): __forceinline HasTC restores retail's inline presence test (VC7.1 had stopped inlining it in this large body); left: frame 0x38 v 0x30 (retail packs the five witness filters into the frame and the dead argument slot), dialog/string temporaries (retry wave 2 sec2b)
// Was: DIFFER(848v841) -- first drafts: frame 0x38 v 0x30 (retail packs the five witness filters into the frame and the dead argument slot), dialog/string temporaries laid out differently
// CTCTrophy::OnTrophyUseAction(CThingCreatureBase*) -- a creature uses the trophy: during the trophy minigame it queues a show-trophy action; otherwise (outside other minigames, in a normal player mode) it either records the trophy on the hero (when the GUI asks for that) or gathers the witnesses able to see it and offers the play-game dialog, or the no-people dialog when there are none
// Leaf: ?OnTrophyUseAction@CTCTrophy
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DDD30, 839 bytes (+ int3 padding), `ret 4`.
// Callees: CDrawQuestInfo::GetMiniGameType 0x00644D54; CTCBase GetPlayerManager 0x00686D40 / GetWorld 0x00686A70;
//   CPlayerManager::GetMainPlayer 0x00449970; CPlayer::GetCurrentMode 0x00633BE0 / GetHeroThing 0x00487DC0; the GUI state
//   getter 0x00437CD0; LowerBound 0x0040F020 (interface 0x17); witness list clear 0x00510D40 / size 0x0065F17B;
//   CThingSearchTools::GetAllThings<...> 0x006DFA50; CTCTrophy::PlaySound 0x006DDCD0 / StartDialogBox 0x006DD7E0;
//   CDataBank::GetTextBySymbol 0x009C95E0; ConstructString 0x009C7B80 (`__fastcall`); CWideString(const wchar_t*)
//   0x0099B6B0 / (const CWideString&) 0x0099B720 / ~ 0x0099B510; CCharString 0x0099EBF0 / ~ 0x0099EAE0; operator new
//   0x00BFEA1A; CDefStringTable::GetString 0x009D49B0 on g_DefStringTable 0x013CA828; CCreatureAction_ShowTrophy ctor
//   0x006DF730; CCountedPointer::Reset 0x00668D90; the creature's action push 0x00664670; operator delete 0x00BFE9BC.
// Layout (retail CTCTrophy, one-pointer lists, as the landed OnCreate 0x006DDA20): PDef +0x0c, MinigameInProgress +0x14,
//   BestWitnessesAheadToDate +0x18, AvailableWitnesses +0x34; CTrophyDef name index +0x3c, sound +0x48.
struct CThing;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    CWideString(const unsigned short* text);                   // 0x0099B6B0
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
};
bool __fastcall ConstructString(CWideString& text, long value0, long value1);   // 0x009C7B80
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol);   // 0x009C95E0
};
struct CGameComponent {
    unsigned char _pad00[0x14];
    CDataBank* TextBank;                                // +0x14
};
extern CGameComponent* g_pMainGameComponent;            // 0x013B86A0
struct CDefStringTable {
    CCharString GetString(long index);                  // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x8c - 0x4c];
    unsigned short DefIndex;                            // +0x8c
    unsigned char _pad8e[0x91 - 0x8e];
    unsigned char m_flags;                              // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long t) const { return m_presentBits[t >> 5] & (1UL << (t & 31)); }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
struct CCreatureAction { };
struct CCountedPointerAction {
    struct CInfo { long Count; void (__fastcall* Destroy)(void*); void* Object; };
    CCreatureAction* Data;
    CInfo* Info;
    CCountedPointerAction() : Data(0), Info(0) { }
    void Reset(CCreatureAction* action);                // 0x00668D90
    ~CCountedPointerAction()
    {
        if (Info) {
            --Info->Count;
            if (Info->Count == 0) {
                Info->Destroy(Info->Object);
                delete Info;
            }
        }
    }
};
struct CThingCreatureBase : CThing {
    void AddAction(const CCountedPointerAction& action);   // 0x00664670
};
struct CCreatureAction_ShowTrophy : CCreatureAction {
    unsigned char _body[0xc0];
    CCreatureAction_ShowTrophy(CThingCreatureBase* creature, CThing* trophy, const CCharString& name, long a);   // 0x006DF730
};
struct CTCHeroTrophies {                                // interface 0x17 (descriptive)
    unsigned char _pad00[0x17c];
    long TrophyBeingUsed;                               // +0x17c
};
struct CPlayer {
    long GetCurrentMode();                              // 0x00633BE0
    CThing* GetHeroThing() const;                       // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                           // 0x00449970
};
struct CDrawQuestInfo {
    long GetMiniGameType();                             // 0x00644D54
};
struct CGuiState {
    unsigned char _pad00[0x34];
    bool RecordTrophy;                                  // +0x34 (descriptive)
};
struct CPlayerGui {
    unsigned char _pad00[0x1c8];
    CDrawQuestInfo* QuestInfo;                          // +0x1c8
    CGuiState* GetState();                              // 0x00437CD0
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790

struct CThingFilter_HasTC { long Type; CThingFilter_HasTC(long t) : Type(t) { } };
struct CThingFilter_HasScriptSupervision { bool Supervised; CThingFilter_HasScriptSupervision(bool s) : Supervised(s) { } };
struct CThingFilter_AICreatureHasProperty { long Property; CThingFilter_AICreatureHasProperty(long p) : Property(p) { } };
struct CThingFilter_IsLyingDown { };
template<class F> struct CFilter_Not { const F& Filter; CFilter_Not(const F& f) : Filter(f) { } };
template<class A, class B, class C, class D, class E> struct CFilter_Bind5 {
    const A& FA; const B& FB; const C& FC; const D& FD; const E& FE;
    CFilter_Bind5(const A& a, const B& b, const C& c, const D& d, const E& e) : FA(a), FB(b), FC(c), FD(d), FE(e) { }
};
typedef CFilter_Bind5<CThingFilter_HasTC, CThingFilter_HasTC, CThingFilter_HasScriptSupervision,
                      CFilter_Not<CThingFilter_AICreatureHasProperty>, CFilter_Not<CThingFilter_IsLyingDown> > CWitnessFilter;
struct CWitnessList {
    void* _M_head;
    void clear();                                       // 0x00510D40
    long size() const;                                  // 0x0065F17B
    bool empty() const { return *(void**)_M_head == _M_head; }
};
struct CThingSearchTools {
    void GetAllThings(const CWitnessFilter& filter, CWitnessList* out);   // 0x006DFA50
};
struct CWorld {
    unsigned char _pad00[0x20];
    CThingSearchTools* SearchTools;                     // +0x20
};
struct CTrophyDef {
    unsigned char _pad00[0x3c];
    long NameIndex;                                     // +0x3c
    unsigned char _pad40[0x48 - 0x40];
    CCharString UseSound;                               // +0x48
};

struct CTCTrophy {
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[4];
    CTrophyDef* PDef;                                   // +0x0c
    long NonMinigameDeedHandle;                         // +0x10
    bool MinigameInProgress;                            // +0x14
    long BestWitnessesAheadToDate;                      // +0x18
    unsigned char _pad1c[0x34 - 0x1c];
    CWitnessList AvailableWitnesses;                    // +0x34
    CWorld* GetWorld();                                 // 0x00686A70
    CPlayerManager* GetPlayerManager();                 // 0x00686D40
    void PlaySound(const CCharString& name);            // 0x006DDCD0
    void StartDialogBox(CWideString text, unsigned long buttons, CWideString a, CWideString b, CWideString c);   // 0x006DD7E0
    void OnTrophyUseAction(CThingCreatureBase* user);
};

void CTCTrophy::OnTrophyUseAction(CThingCreatureBase* user)
{
    if (!MinigameInProgress) {
        if (g_pPlayerGui->QuestInfo->GetMiniGameType())
            return;
        long mode = GetPlayerManager()->GetMainPlayer()->GetCurrentMode();
        if (mode != 1 && mode != 2 && mode != 0x24 && mode != 0x2b)
            return;
        if (g_pPlayerGui->GetState()->RecordTrophy) {
            CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
            if (!hero || (hero->m_flags & 1) || !hero->HasTC(0x17))
                return;
            hero->GetTC<CTCHeroTrophies>(0x17)->TrophyBeingUsed = ParentThing->DefIndex;
            return;
        }
        AvailableWitnesses.clear();
        GetWorld()->SearchTools->GetAllThings(CWitnessFilter(CThingFilter_HasTC(0x4f), CThingFilter_HasTC(0x23),
                                                             CThingFilter_HasScriptSupervision(false),
                                                             CFilter_Not<CThingFilter_AICreatureHasProperty>(CThingFilter_AICreatureHasProperty(3)),
                                                             CFilter_Not<CThingFilter_IsLyingDown>(CThingFilter_IsLyingDown())),
                                              &AvailableWitnesses);
        if (AvailableWitnesses.empty()) {
            CWideString text = g_pMainGameComponent->TextBank->GetTextBySymbol("TEXT_GUI_TROPHY_NO_PEOPLE");
            StartDialogBox(text, 10, L"", L"", L"");
            return;
        }
        PlaySound(PDef->UseSound);
        CWideString text = g_pMainGameComponent->TextBank->GetTextBySymbol("TEXT_GUI_TROPHY_PLAY_GAME");
        ConstructString(text, AvailableWitnesses.size(), BestWitnessesAheadToDate);
        CWideString title(L"TEXT_GUI_HELP");
        StartDialogBox(text, 4, L"", L"", title);
    } else {
        CCountedPointerAction action;
        action.Reset(new CCreatureAction_ShowTrophy(user, ParentThing, g_DefStringTable.GetString(PDef->NameIndex), 0));
        user->AddAction(action);
    }
}
