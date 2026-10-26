// Behaviour test for CQuestManager::IsRegionStartOfRegionLockingMode (0x004B11A0): true only for a card whose
// definition exists and is no vignette, whose FeatIndex is negative, whose quest is running and contains the region;
// every definition reference taken is released again (counts back to 1).
#include <cstdio>
#include <cstring>
// Test doubles: a quest card TC with a counted definition (AddRef/DecRef counted, Release recorded), card things on
// a sentinel ring, a CCharString is its text.
class CCharString { public:
    const char* Text;
    CCharString(const char* t = "") : Text(t) {}
};
static int g_releases;
class CQuestCardDef { public:
    long RefCount; bool IsVignette;
    CQuestCardDef(bool v) : RefCount(1), IsVignette(v) {}
    void Release() { ++g_releases; }
    void AddRef() { ++RefCount; }
    void DecRef() { if (--RefCount == 0) Release(); }
};
struct CQuestCardDefPointer {
    CQuestCardDef* Object;
    CQuestCardDefPointer(CQuestCardDef* p) : Object(p) {}
    CQuestCardDefPointer(const CQuestCardDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CQuestCardDefPointer() { if (Object) Object->DecRef(); }
};
class CTCQuestCard { public:
    CQuestCardDefPointer PDef; long FeatIndex; CCharString ScriptQuestName;
    CTCQuestCard(CQuestCardDef* d, long feat, const char* name) : PDef(d), FeatIndex(feat), ScriptQuestName(name) {}
};
class CThing { public:
    CTCQuestCard* Card; bool Dead;
    bool HasTC(long type) const { return type == 0x6c && Card != 0; }
    CTCQuestCard* GetTC(long) const { return Card; }
};
static CTCQuestCard* GetQuestCard(CThing* thing) { CTCQuestCard* tc = 0; if (thing->HasTC(0x6c)) tc = thing->GetTC(0x6c); return tc; }
struct CBaseIntelligentPointer { CThing* Item; CThing* GetPItem() const { return Item; } };
struct CCardNode { CCardNode* _M_next; CCardNode* _M_prev; CBaseIntelligentPointer _M_data; };
static void Ring(CCardNode* head, CCardNode* n, CThing** t, int count)
{
    CCardNode* prev = head;
    for (int i = 0; i < count; ++i) { n[i]._M_data.Item = t[i]; prev->_M_next = &n[i]; n[i]._M_prev = prev; prev = &n[i]; }
    prev->_M_next = head; head->_M_prev = prev;
}
class CRegion { public: int Id; };
struct CActiveQuest {
    const CRegion* Region;
    bool ContainsRegion(const CRegion& region) const { return Region == &region; }
};
static CActiveQuest g_qa, g_qb;
class CQuestManager { public:
    CCardNode* ActiveQuestCards;
    CActiveQuest* GetActiveQuestFromScriptName(const CCharString& name) const
    {
        if (std::strcmp(name.Text, "QA") == 0) return &g_qa;
        if (std::strcmp(name.Text, "QB") == 0) return &g_qb;
        return 0;
    }
    bool IsRegionStartOfRegionLockingMode(const CRegion& region) const;
};
bool CQuestManager::IsRegionStartOfRegionLockingMode(const CRegion& region) const
{
    for (CCardNode* node = ActiveQuestCards->_M_next; node != ActiveQuestCards; node = node->_M_next) {
        CTCQuestCard* tc = GetQuestCard(node->_M_data.GetPItem());
        if (tc) {
            CQuestCardDefPointer def(tc->PDef);
            if (def.Object && !def.Object->IsVignette && tc->FeatIndex <= -1) {
                CActiveQuest* quest = GetActiveQuestFromScriptName(tc->ScriptQuestName);
                if (quest && quest->ContainsRegion(region))
                    return true;
            }
        }
    }
    return false;
}
int main()
{
    CRegion r1 = { 1 }, r2 = { 2 }, r3 = { 3 }, r4 = { 4 };
    g_qa.Region = &r1; g_qb.Region = &r2;
    CQuestCardDef dv(true), dn(false), dq(false), df(false);
    CTCQuestCard vign(&dv, -1, "QA"), nodef(0, -1, "QA"), feat(&df, 3, "QA"), good(&dq, -1, "QB"), gone(&dn, -1, "QX");
    CThing tv = { &vign, false }, tn = { &nodef, false }, tf = { &feat, false }, tx = { 0, false }, tg = { &good, false }, to = { &gone, false };
    CThing* things[6] = { &tv, &tn, &tf, &tx, &to, &tg };
    CCardNode head, n[6];
    Ring(&head, n, things, 6);
    CQuestManager qm; qm.ActiveQuestCards = &head;
    int bad = 0;
    if (qm.IsRegionStartOfRegionLockingMode(r1)) bad |= 1;      // QA only on a vignette / defless / feat card
    if (!qm.IsRegionStartOfRegionLockingMode(r2)) bad |= 2;     // QB on the good card
    g_qa.Region = &r3;
    if (qm.IsRegionStartOfRegionLockingMode(r3) || qm.IsRegionStartOfRegionLockingMode(r4)) bad |= 4;
    if (dv.RefCount != 1 || dn.RefCount != 1 || dq.RefCount != 1 || df.RefCount != 1 || g_releases) bad |= 8;
    if (bad) { std::printf("004b11a0_TEST FAIL\n"); return 1; }
    std::printf("004b11a0_TEST PASS\n");
    return 0;
}