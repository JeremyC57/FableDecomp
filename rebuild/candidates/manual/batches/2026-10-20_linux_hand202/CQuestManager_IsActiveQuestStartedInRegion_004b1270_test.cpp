// Behaviour test for CQuestManager::IsActiveQuestStartedInRegion (0x004B1270): true when a non-vignette active card's
// definition starts in the named region; an empty name, vignette cards and things without a card never match; the
// definition reference is released after each compare.
#include <cstdio>
#include <cstring>
struct CCharString {
    const char* Text;
    CCharString(const char* t) : Text(t) {}
    bool operator!=(const char* t) const { return std::strcmp(Text, t) != 0; }
};
struct CDefString {
    const char* Name;
    bool operator==(const CCharString& n) const { return std::strcmp(Name, n.Text) == 0; }
};
static int g_releases;
struct CQuestCardDef {
    long RefCount; CDefString TeleporterRegionName;
    void Release() { ++g_releases; }
    void AddRef() { ++RefCount; }
    void DecRef() { if (--RefCount == 0) Release(); }
};
struct CQuestCardDefPointer {
    CQuestCardDef* Object;
    CQuestCardDefPointer(CQuestCardDef* p) : Object(p) {}
    CQuestCardDefPointer(const CQuestCardDefPointer& o) : Object(o.Object) { if (Object) Object->AddRef(); }
    ~CQuestCardDefPointer() { if (Object) Object->DecRef(); }
};
static int g_compares;
struct CTCQuestCard {
    CQuestCardDefPointer PDef; bool Vignette;
    CTCQuestCard(CQuestCardDef* d, bool v) : PDef(d), Vignette(v) {}
    bool IsVignette() const { return Vignette; }
};
struct CThing {
    CTCQuestCard* Card;
    bool HasTC(long type) const { return type == 0x6c && Card != 0; }
    CTCQuestCard* GetTC(long) const { return Card; }
};
static CTCQuestCard* GetQuestCard(CThing* thing) { CTCQuestCard* tc = 0; if (thing->HasTC(0x6c)) tc = thing->GetTC(0x6c); return tc; }
struct CBaseIntelligentPointer { CThing* Item; CThing* GetPItem() const { return Item; } };
struct CCardNode { CCardNode* _M_next; CCardNode* _M_prev; CBaseIntelligentPointer _M_data; };
struct CQuestManager {
    CCardNode* ActiveQuestCards;
    bool IsActiveQuestStartedInRegion(const CCharString& regionName) const;
};
bool CQuestManager::IsActiveQuestStartedInRegion(const CCharString& regionName) const
{
    if (regionName != "") {
        for (CCardNode* node = ActiveQuestCards->_M_next; node != ActiveQuestCards; node = node->_M_next) {
            CTCQuestCard* tc = GetQuestCard(node->_M_data.GetPItem());
            if (tc && !tc->IsVignette()) {
                bool started;
                {
                    CQuestCardDefPointer def(tc->PDef);
                    started = def.Object->TeleporterRegionName == regionName;
                }
                if (started)
                    return true;
            }
        }
    }
    return false;
}
int main()
{
    CQuestCardDef dv = { 1, { "Oakvale" } }, da = { 1, { "Bowerstone" } }, db = { 1, { "" } };
    CTCQuestCard vign(&dv, true), a(&da, false), b(&db, false);
    CThing tv = { &vign }, tn = { 0 }, ta = { &a }, tb = { &b };
    CThing* things[4] = { &tv, &tn, &tb, &ta };
    CCardNode head, n[4];
    CCardNode* prev = &head;
    for (int i = 0; i < 4; ++i) { n[i]._M_data.Item = things[i]; prev->_M_next = &n[i]; n[i]._M_prev = prev; prev = &n[i]; }
    prev->_M_next = &head; head._M_prev = prev;
    CQuestManager qm; qm.ActiveQuestCards = &head;
    char bow[11] = "Bowerstone";
    int bad = 0;
    if (!qm.IsActiveQuestStartedInRegion(CCharString(bow))) bad |= 1;
    if (qm.IsActiveQuestStartedInRegion(CCharString("Oakvale"))) bad |= 2;    // only on the vignette card
    if (qm.IsActiveQuestStartedInRegion(CCharString(""))) bad |= 4;           // empty name: no walk (b would match)
    if (dv.RefCount != 1 || da.RefCount != 1 || db.RefCount != 1 || g_releases) bad |= 8;
    if (bad) { std::printf("004b1270_TEST FAIL\n"); return 1; }
    std::printf("004b1270_TEST PASS\n");
    return 0;
}
