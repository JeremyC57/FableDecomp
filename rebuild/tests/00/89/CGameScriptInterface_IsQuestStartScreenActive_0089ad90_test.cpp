#include <cstdio>
class CCharStringData { public:
    const char*   Chars;                                 // +0x00
    unsigned long Length;                                // +0x04
};
long __fastcall Compare(const char* a, const char* b);   // 0x00411570
class CCharString {
public:
    CCharStringData* PStringData;
    __forceinline bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->Length == other.PStringData->Length)
            return Compare(PStringData->Chars, other.PStringData->Chars) == 0;
        return false;
    }
};
class CTCBase;
class CInterfaceMap {                                    // CThing::Components, CVectorMap<ETCInterfaceType, CTCBase*> (retail 8 bytes)
public:
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    CInterfaceMap Components;                            // +0x44
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTC()
    {
        long type = T::INTERFACE_TYPE;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || T::INTERFACE_TYPE < it->type) ? map->last : it)->component;
    }
};
class CTCQuestCompletionUI {
public:
    enum { INTERFACE_TYPE = 0xfc };
    unsigned char _pad00[0x0c];
    bool          Active;                                // +0x0c
    bool          ReallyActive;                          // +0x0d
    unsigned char _pad0e[0x1c - 0x0e];
    bool          Quitting;                              // +0x1c
    unsigned char _pad1d[3];
    long          Completion;                            // +0x20 CompletionType
    unsigned char _pad24[0x50 - 0x24];
    CCharString   ScriptName;                            // +0x50
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPPlayerCharacter() const;   // 0x00487DC0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                            // 0x00449970
};
class CMainGameComponent {
public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PPlayerManager;                      // +0x1c
    CPlayerManager* GetPlayerManager() const { return PPlayerManager; }
};
extern CMainGameComponent* g_pMainGameComponent;         // 0x013B86A0
class CGameScriptInterface {
public:
    virtual bool IsQuestStartScreenActive(CCharString& questName) const;
};

bool CGameScriptInterface::IsQuestStartScreenActive(CCharString& questName) const
{
    CThingPlayerCreature* hero = g_pMainGameComponent->GetPlayerManager()->GetMainPlayer().GetPPlayerCharacter();
    if (hero->HasTC(CTCQuestCompletionUI::INTERFACE_TYPE))
    {
        CTCQuestCompletionUI* ui = hero->GetTC<CTCQuestCompletionUI>();
        if (ui->Active && !ui->Quitting && ui->Completion == 0 && ui->ScriptName == questName)
            return true;
    }
    return false;
}

#include <cstring>
long __fastcall Compare(const char* a, const char* b) { return std::strcmp(a, b); }
static CInterfaceMap::SEntry g_entries[3];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
static CThingPlayerCreature g_hero;
CThingPlayerCreature* CPlayer::GetPPlayerCharacter() const { return &g_hero; }
static CPlayer g_player;
CPlayer& CPlayerManager::GetMainPlayer() { return g_player; }
static unsigned char g_mgcRaw[sizeof(CMainGameComponent)];
CMainGameComponent* g_pMainGameComponent = (CMainGameComponent*)g_mgcRaw;
int main()
{
    CPlayerManager pm; g_pMainGameComponent->PPlayerManager = &pm;
    static CTCQuestCompletionUI ui;
    static CCharStringData d1 = { "Q_Start", 7 }, d2 = { "Q_Start", 7 }, d3 = { "Q_Other", 7 }, d4 = { "Q_Sta", 5 };
    ui.Active = true; ui.Quitting = false; ui.Completion = 0; ui.ScriptName.PStringData = &d1;
    for (int i = 0; i < 9; ++i) g_hero.UsingComponents[i] = 0;
    g_hero.UsingComponents[7] = 1u << 28;
    g_entries[0].type = 4; g_entries[0].component = 0;
    g_entries[1].type = 0xfc; g_entries[1].component = (CTCBase*)&ui;
    g_hero.Components.first = g_entries; g_hero.Components.last = g_entries + 2;
    CGameScriptInterface gsi; CCharString q; q.PStringData = &d2;
    bool ok = gsi.IsQuestStartScreenActive(q);
    q.PStringData = &d1; ok = ok && gsi.IsQuestStartScreenActive(q);          // same block
    q.PStringData = &d3; ok = ok && !gsi.IsQuestStartScreenActive(q);         // other name
    q.PStringData = &d4; ok = ok && !gsi.IsQuestStartScreenActive(q);         // other length
    q.PStringData = 0;   ok = ok && !gsi.IsQuestStartScreenActive(q);         // null name
    q.PStringData = &d2;
    ui.Active = false; ok = ok && !gsi.IsQuestStartScreenActive(q); ui.Active = true;
    ui.Quitting = true; ok = ok && !gsi.IsQuestStartScreenActive(q); ui.Quitting = false;
    ui.Completion = 1; ok = ok && !gsi.IsQuestStartScreenActive(q); ui.Completion = 0;
    g_hero.UsingComponents[7] = 0; ok = ok && !gsi.IsQuestStartScreenActive(q); g_hero.UsingComponents[7] = 1u << 28;
    ok = ok && gsi.IsQuestStartScreenActive(q);
    std::printf(ok ? "0089ad90_TEST PASS\n" : "0089ad90_TEST FAIL\n");
    return ok ? 0 : 1;
}