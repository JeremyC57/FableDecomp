// Behaviour test for CQuestManager::SetQuestCardObjective (0x004B0D30): false without a live card holding a quest-card
// TC or when neither the objective nor the (non-empty) region differs; otherwise the objective is replaced, non-empty
// region / teleporter region names are set, and true is returned; the region temporary is released.
#include <cstdio>
#include <cstring>

static int g_live = 0;
class CCharString { public:
    char Text[32];
    CCharString(const char* t) { std::strcpy(Text, t); ++g_live; }
    CCharString(const CCharString& o) { std::strcpy(Text, o.Text); ++g_live; }
    ~CCharString() { --g_live; }
    bool operator!=(const CCharString& o) const { return std::strcmp(Text, o.Text) != 0; }
    bool operator!=(const char* t) const { return std::strcmp(Text, t) != 0; }
    const CCharString& operator=(const CCharString& o) { std::strcpy(Text, o.Text); return *this; }
};
class CTCBase { public: int dummy; };
struct CTCEntry { long Type; CTCBase* TC; };
struct CTCMap {
    CTCEntry* _M_start; CTCEntry* _M_finish;
    CTCEntry* LowerBound(const unsigned long& type) { CTCEntry* p = _M_start; while (p != _M_finish && (unsigned long)p->Type < type) ++p; return p; }
    CTCBase* Find(long type) { CTCEntry* it = LowerBound((unsigned long)type); if (it == _M_finish || type < it->Type) return 0; return it->TC; }
};
class CThing { public:
    unsigned long UsingComponents[8]; CTCMap Components; unsigned char Dead : 1;
    unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetTC(long type) { return Components.Find(type); }
};
class CTCQuestCard : public CTCBase { public:
    CCharString ObjectiveName; CCharString Region; CCharString Teleporter;
    CTCQuestCard() : ObjectiveName("old"), Region("R1"), Teleporter("T1") {}
    CCharString GetRegionName() { return Region; }
    void SetRegionName(const CCharString& r) { Region = r; }
    void SetTeleporterRegionName(const CCharString& r) { Teleporter = r; }
};
static CThing* g_card = 0;
class CQuestManager { public:
    CThing* GetActiveQuestCardFromScriptName(const CCharString& n) const { return std::strcmp(n.Text, "Q_X") == 0 ? g_card : 0; }
    bool SetQuestCardObjective(const CCharString& scriptName, const CCharString& objective, const CCharString& region, const CCharString& teleporterRegion);
};
bool CQuestManager::SetQuestCardObjective(const CCharString& scriptName, const CCharString& objective, const CCharString& region, const CCharString& teleporterRegion)
{
    CThing* card = GetActiveQuestCardFromScriptName(scriptName);
    if (card != 0 && !card->Dead && card->HasTC(0x6c)) {
        CTCQuestCard* tc = (CTCQuestCard*)card->GetTC(0x6c);
        if (tc != 0) {
            bool changed = tc->ObjectiveName != objective || (region != "" && tc->GetRegionName() != region);
            if (changed) {
                tc->ObjectiveName = objective;
                if (region != "")
                    tc->SetRegionName(region);
                if (teleporterRegion != "")
                    tc->SetTeleporterRegionName(teleporterRegion);
                return true;
            }
        }
    }
    return false;
}
int main()
{
    int bad = 0;
    CTCQuestCard tc;
    CTCEntry entries[2] = { { 0x10, 0 }, { 0x6c, &tc } };
    CThing card; std::memset(card.UsingComponents, 0, sizeof(card.UsingComponents)); card.Dead = 0;
    card.UsingComponents[3] = 1UL << 12; card.Components._M_start = entries; card.Components._M_finish = entries + 2;
    g_card = &card;
    CQuestManager m;
    CCharString q("Q_X"), other("Q_Y"), obj("old"), obj2("new"), empty(""), r1("R1"), r2("R2"), t2("T2");
    int base = g_live;
    if (m.SetQuestCardObjective(other, obj2, empty, empty)) bad = 1;                       // no card
    if (m.SetQuestCardObjective(q, obj, empty, t2) || std::strcmp(tc.Teleporter.Text, "T1")) bad = 1;   // unchanged
    if (m.SetQuestCardObjective(q, obj, r1, empty)) bad = 1;                               // same region
    if (!m.SetQuestCardObjective(q, obj, r2, t2) || std::strcmp(tc.Region.Text, "R2") || std::strcmp(tc.Teleporter.Text, "T2")) bad = 1;
    if (!m.SetQuestCardObjective(q, obj2, empty, empty) || std::strcmp(tc.ObjectiveName.Text, "new") || std::strcmp(tc.Region.Text, "R2")) bad = 1;
    card.Dead = 1;
    if (m.SetQuestCardObjective(q, obj, empty, empty)) bad = 1;
    card.Dead = 0; card.UsingComponents[3] = 0;
    if (m.SetQuestCardObjective(q, obj, empty, empty)) bad = 1;
    if (g_live != base) bad = 1;
    if (bad) { std::printf("004b0d30_TEST FAIL\n"); return 1; }
    std::printf("004b0d30_TEST PASS\n");
    return 0;
}