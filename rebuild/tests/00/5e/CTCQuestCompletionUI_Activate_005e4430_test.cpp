// 005e4430 behaviour test: CTCQuestCompletionUI::Activate (control flow + data of the final.cpp body)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
class CCharString { public:
    char Text[32];
    CCharString(const char* text = "", long = -1) { strcpy(Text, text); }
    ~CCharString() {}
    const CCharString& operator=(const CCharString& o) { strcpy(Text, o.Text); return *this; }
    const CCharString& operator=(const char* t) { strcpy(Text, t); return *this; }
    bool operator!=(const CCharString& o) const { return strcmp(Text, o.Text) != 0; }
};
class CWideString { public:
    int Id;
    CWideString(int id = 0) : Id(id) {}
    ~CWideString() {}
    const CWideString& operator=(const CWideString& o) { Id = o.Id; return *this; }
};
static int g_deleted = 0;
struct CDef { virtual void v00() {} virtual void DeleteThis() { ++g_deleted; } long RefCount;
    CDef() : RefCount(1) {}
    __forceinline void Release() { if (--RefCount == 0) DeleteThis(); } };
template <class T> class CDefPointer { public:
    T* P;
    __forceinline CDefPointer() : P(0) { }
    __forceinline T* operator->() const { return P; }
    __forceinline CDefPointer(const CDefPointer& o) : P(o.P) { if (P) ++P->RefCount; }
    __forceinline ~CDefPointer() { if (P) P->Release(); }
    __forceinline CDefPointer& operator=(const CDefPointer& o)
    {
        if (P != o.P) {
            if (P) { P->Release(); P = 0; }
            P = o.P;
            if (P) ++P->RefCount;
        }
        return *this;
    }
};
template <class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const T& operator[](unsigned long i) const { return *(begin() + i); }
    const T* begin() const { return _M_start; } };
class CQuestCardDef : public CDef { public: vector<long> RewardObjects; };
struct CObjectDef : CDef { unsigned char Type; };
static CObjectDef g_defs[8];
static int g_getdef_calls = 0;
class CDefinitionManager { public:
    static CDefinitionManager* __fastcall Get() { static CDefinitionManager m; return &m; }
    bool GetDef(long index, CDefPointer<CObjectDef>* out) { ++g_getdef_calls; out->P = &g_defs[index]; ++out->P->RefCount; return true; }
};
bool __fastcall IsAugmentation_005d9930(long index) { return index == 6; }
class CThing { public: unsigned short DefIndex; };
class CTCQuestCard { public:
    CThing* Owner;
    CThing* GetOwner() const { return Owner; }
    CDefPointer<CQuestCardDef> PDef;
    CCharString ScriptName;
    long GoldReward;
    long RenownReward;
    CWideString GetQuestSummary() { return CWideString(11); }
    CWideString GetSuccessSummary() { return CWideString(12); }
    CWideString GetQuestName() { return CWideString(13); }
};
struct CBoast { long a, b, c; CCharString ScriptName; CWideString Text0; CWideString Text1; };
struct CBoastNode { CBoastNode* next; CBoastNode* prev; CBoast data; };
static CBoastNode* NewNode() { CBoastNode* n = (CBoastNode*)malloc(sizeof(CBoastNode)); new (&n->data) CBoast(); n->next = n->prev = n; return n; }
struct CBoastList {
    CBoastNode* node;
    CBoastList() : node(NewNode()) {}
    void push_back(const char* s, long a) { CBoastNode* n = NewNode(); n->data.ScriptName = s; n->data.a = a;
        n->prev = node->prev; n->next = node; node->prev->next = n; node->prev = n; }
    void clear() { CBoastNode* it = node->next; while (it != node) { CBoastNode* nx = it->next; it->data.~CBoast(); free(it); it = nx; } node->next = node->prev = node; }
    CBoastList& operator=(const CBoastList& o) { clear(); for (CBoastNode* it = o.node->next; it != o.node; it = it->next) push_back(it->data.ScriptName.Text, it->data.a); return *this; }
    int count() const { int c = 0; for (CBoastNode* it = node->next; it != node; it = it->next) ++c; return c; }
};
class CQuestManager { public: CBoastList Boasts; };
CQuestManager* g_pQuestManager;
struct CComponent { virtual void v00() {} };
struct CFilterComponent : CComponent {
    int shown, focus;
    CFilterComponent() : shown(0), focus(0) {}
    virtual void Show() { ++shown; }
    virtual void SetFocus(bool b) { focus = b ? 1 : 2; }
};
static CFilterComponent g_filter;
static char g_compname[64];
namespace NUISystem { class CManager { public:
    static CManager* __fastcall GetInstance() { static CManager m; return &m; }
    CComponent* CreateComponent(const CCharString& name, bool) { strcpy(g_compname, name.Text); return &g_filter; }
}; }
static int g_closed = 0;
class CPlayerGui { public: void Close(bool b) { g_closed = b ? 1 : 2; } };
CPlayerGui* g_pPlayerGui;
double __fastcall GFGetTime() { return 42.5; }
enum CompletionType { COMPLETION_0, COMPLETION_1, COMPLETION_2, COMPLETION_3 };
class CTCQuestCompletionUI { public:
    bool Active;
    bool WaitingForRegionLoad;
    bool WaitingForPlayerControl;
    bool WaitingForBoastProcessing;
    CompletionType Completion;
    unsigned long Frame;
    long FrameStartedBoastProcessing;
    CDefPointer<CQuestCardDef> PCardDef;
    CBoastList Boasts;
    unsigned long RenderedObjectDef;
    CWideString QuestSummary;
    CWideString SuccessSummary;
    CWideString QuestObjective;
    double LastRenderTime;
    CCharString ScriptName;
    long GoldReward;
    long RenownReward;
    CFilterComponent* PBlack;
    long QuestCardDef;
    void Activate(CompletionType type, CTCQuestCard* card, bool arg);
};
void CTCQuestCompletionUI::Activate(CompletionType type, CTCQuestCard* card, bool arg)
{
    if (Active) return;
    WaitingForRegionLoad = !arg;
    WaitingForPlayerControl = false;
    Completion = type;
    if (type == COMPLETION_1 || type == COMPLETION_2) WaitingForBoastProcessing = true;
    else WaitingForBoastProcessing = false;
    Active = true;
    Frame = 0;
    FrameStartedBoastProcessing = -1;
    if (card) {
        ScriptName = card->ScriptName;
        PCardDef = CDefPointer<CQuestCardDef>(card->PDef);
        QuestCardDef = card->Owner->DefIndex;
        GoldReward = card->GoldReward;
        RenownReward = card->RenownReward;
        Boasts = g_pQuestManager->Boasts;
        CBoastNode* it = Boasts.node->next;
        while (it != Boasts.node) {
            if (ScriptName != it->data.ScriptName) {
                CBoastNode* next = it->next;
                CBoastNode* prev = it->prev;
                prev->next = next;
                next->prev = prev;
                it->data.~CBoast();
                free(it);
                it = next;
            } else {
                it = it->next;
            }
        }
        CDefPointer<CObjectDef> def;
        CDefinitionManager::Get()->GetDef(card->GetOwner()->DefIndex, &def);
        if (def.P->Type == 5) {
            RenderedObjectDef = card->Owner->DefIndex;
            if (PCardDef.P && Completion == COMPLETION_1) {
                bool found = false;
                for (unsigned long i = 0; i < PCardDef->RewardObjects.size() && !found; ++i) {
                    if (IsAugmentation_005d9930(PCardDef->RewardObjects[i])) {
                        CDefPointer<CObjectDef> reward;
                        CDefinitionManager::Get()->GetDef(PCardDef->RewardObjects[i], &reward);
                        RenderedObjectDef = PCardDef->RewardObjects[i];
                        found = true;
                    }
                }
            }
        } else {
            RenderedObjectDef = 0;
        }
        QuestSummary = card->GetQuestSummary();
        SuccessSummary = card->GetSuccessSummary();
        QuestObjective = card->GetQuestName();
        def.P->Release();
        def.P = 0;
    } else {
        ScriptName = "";
        if (PCardDef.P) { PCardDef.P->Release(); PCardDef.P = 0; }
        PCardDef.P = 0;
        Boasts.clear();
        RenderedObjectDef = 0;
        QuestCardDef = 0;
    }
    PBlack = (CFilterComponent*)NUISystem::CManager::GetInstance()->CreateComponent("PC_SCREEN_FILTER", false);
    PBlack->Show();
    PBlack->SetFocus(true);
    g_pPlayerGui->Close(true);
    LastRenderTime = GFGetTime();
}
static void Reset(CTCQuestCompletionUI& ui)
{
    ui.Active = false; ui.WaitingForRegionLoad = ui.WaitingForPlayerControl = true; ui.WaitingForBoastProcessing = false;
    ui.Completion = COMPLETION_0; ui.Frame = 99; ui.FrameStartedBoastProcessing = 7; ui.PCardDef.P = 0;
    ui.RenderedObjectDef = 77; ui.LastRenderTime = 0; ui.ScriptName = "old"; ui.GoldReward = ui.RenownReward = 0;
    ui.PBlack = 0; ui.QuestCardDef = 55; g_filter.shown = g_filter.focus = 0; g_closed = 0; g_compname[0] = 0;
}
int main()
{
    CQuestManager qm; g_pQuestManager = &qm;
    CPlayerGui gui; g_pPlayerGui = &gui;
    qm.Boasts.push_back("Q1", 1); qm.Boasts.push_back("Q2", 2); qm.Boasts.push_back("Q1", 3); qm.Boasts.push_back("Q3", 4);
    g_defs[2].Type = 5; g_defs[3].Type = 1; g_defs[6].Type = 9;
    long rewards[3] = { 4, 6, 6 };
    CQuestCardDef cardDef; cardDef.RewardObjects._M_start = rewards; cardDef.RewardObjects._M_finish = rewards + 3; cardDef.RewardObjects._M_end_of_storage = rewards + 3;
    CThing owner; owner.DefIndex = 2;
    CTCQuestCard card; card.Owner = &owner; card.PDef.P = &cardDef; ++cardDef.RefCount; card.ScriptName = "Q1"; card.GoldReward = 100; card.RenownReward = 20;

    CTCQuestCompletionUI ui; Reset(ui);
    // already active: nothing changes
    ui.Active = true; ui.Activate(COMPLETION_1, &card, true);
    CHECK(ui.Frame == 99 && ui.PBlack == 0 && g_closed == 0);

    // type 1, reward card, owner def type 5: first augmentation reward (6) chosen, loop stops after it
    Reset(ui); g_getdef_calls = 0;
    ui.Activate(COMPLETION_1, &card, false);
    CHECK(ui.Active && ui.WaitingForRegionLoad && !ui.WaitingForPlayerControl && ui.WaitingForBoastProcessing);
    CHECK(ui.Completion == COMPLETION_1 && ui.Frame == 0 && ui.FrameStartedBoastProcessing == -1);
    CHECK(strcmp(ui.ScriptName.Text, "Q1") == 0 && ui.PCardDef.P == &cardDef && ui.QuestCardDef == 2);
    CHECK(ui.GoldReward == 100 && ui.RenownReward == 20);
    CHECK(ui.Boasts.count() == 2 && ui.Boasts.node->next->data.a == 1 && ui.Boasts.node->next->next->data.a == 3);
    CHECK(ui.RenderedObjectDef == 6 && g_getdef_calls == 2);
    CHECK(ui.QuestSummary.Id == 11 && ui.SuccessSummary.Id == 12 && ui.QuestObjective.Id == 13);
    CHECK(ui.PBlack == &g_filter && g_filter.shown == 1 && g_filter.focus == 1 && g_closed == 1);
    CHECK(strcmp(g_compname, "PC_SCREEN_FILTER") == 0 && ui.LastRenderTime == 42.5);

    // type 2: boast wait, but no augmentation pick (owner def kept)
    Reset(ui); g_getdef_calls = 0;
    ui.Activate(COMPLETION_2, &card, true);
    CHECK(!ui.WaitingForRegionLoad && ui.WaitingForBoastProcessing && ui.RenderedObjectDef == 2 && g_getdef_calls == 1);

    // owner def not type 5: rendered object cleared
    Reset(ui); owner.DefIndex = 3; g_getdef_calls = 0;
    ui.Activate(COMPLETION_0, &card, false);
    CHECK(!ui.WaitingForBoastProcessing && ui.RenderedObjectDef == 0 && ui.QuestCardDef == 3 && g_getdef_calls == 1);

    // no card: everything cleared
    Reset(ui); ui.PCardDef.P = &cardDef; ++cardDef.RefCount;
    long rc = cardDef.RefCount;
    ui.Activate(COMPLETION_3, 0, false);
    CHECK(ui.Active && ui.PCardDef.P == 0 && cardDef.RefCount == rc - 1 && ui.Boasts.count() == 0);
    CHECK(ui.RenderedObjectDef == 0 && ui.QuestCardDef == 0 && ui.ScriptName.Text[0] == 0);
    CHECK(ui.PBlack == &g_filter && g_closed == 1);

    if (!g_fail) printf("005e4430_TEST PASS\n");
    return g_fail;
}