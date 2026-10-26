// Behaviour test for CWorld::AutoSaveCheckPoint (0x004A0BB0): save types set to 2, each present inventory of a
// live hero incremented once, the checkpoint flag set on success and kept (not cleared) on failure.
#include <cstdio>
class CWideString { public: const char* Text; };
class CUserProfileManager { public:
    static CUserProfileManager& Get() { static CUserProfileManager m; return m; }
    CWideString GetAutoSaveCheckPointFileName() const { CWideString w = { "AutoSave.cp" }; return w; }
};
struct CTCInventoryBase { int Count; void IncrementQuestPersistentRefCountForNonPersistentItems() { ++Count; } };
class CThing { public:
    bool Dead; CTCInventoryBase* Inv[3];   // TCs 0x11, 0x12, 0x13
    bool HasTC(long t) const { return Inv[t - 0x11] != 0; }
    template<class T> T* GetTC(long t) { return Inv[t - 0x11]; }
};
class CPlayer { public: CThing* Hero; CThing* GetHeroThing() { return Hero; } };
class CPlayerManager { public: CPlayer* Main; CPlayer* GetMainPlayer() { return Main; } };
static bool g_saveOk; static int g_saves;
class CWorld { public:
    CPlayerManager* PlayerManager; long MostRecentSaveType, MostRecentSaveTypeBeforeManualSave; bool AutoSaveCheckPointExists;
    bool AutoSaveInternal(const CWideString& n, bool b) { ++g_saves; return g_saveOk && !b && n.Text[0] == 'A'; }
    bool AutoSaveCheckPoint()
    {
        MostRecentSaveType = 2;
        MostRecentSaveTypeBeforeManualSave = 2;
        CThing* hero = PlayerManager->GetMainPlayer()->GetHeroThing();
        if (hero && !hero->Dead)
        {
            if (hero->HasTC(0x11)) hero->GetTC<CTCInventoryBase>(0x11)->IncrementQuestPersistentRefCountForNonPersistentItems();
            if (hero->HasTC(0x13)) hero->GetTC<CTCInventoryBase>(0x13)->IncrementQuestPersistentRefCountForNonPersistentItems();
            if (hero->HasTC(0x12)) hero->GetTC<CTCInventoryBase>(0x12)->IncrementQuestPersistentRefCountForNonPersistentItems();
        }
        bool saved = AutoSaveInternal(CUserProfileManager::Get().GetAutoSaveCheckPointFileName(), false);
        if (saved)
            AutoSaveCheckPointExists = true;
        return AutoSaveCheckPointExists;
    }
};
int main()
{
    CTCInventoryBase a = { 0 }, c = { 0 };
    CThing hero = { false, { &a, 0, &c } }; CPlayer p = { &hero }; CPlayerManager pm = { &p };
    CWorld w = { &pm, 0, 0, false };
    g_saveOk = true;
    bool r1 = w.AutoSaveCheckPoint();
    bool ok = r1 && w.MostRecentSaveType == 2 && w.MostRecentSaveTypeBeforeManualSave == 2 && a.Count == 1 && c.Count == 1;
    hero.Dead = true; g_saveOk = false;
    bool r2 = w.AutoSaveCheckPoint();             // flag stays from before
    ok = ok && r2 && a.Count == 1 && g_saves == 2;
    w.AutoSaveCheckPointExists = false; hero.Dead = false;
    bool r3 = w.AutoSaveCheckPoint();
    ok = ok && !r3 && a.Count == 2 && c.Count == 2;
    if (!ok) { std::printf("004a0bb0_TEST FAIL\n"); return 1; }
    std::printf("004a0bb0_TEST PASS\n");
    return 0;
}