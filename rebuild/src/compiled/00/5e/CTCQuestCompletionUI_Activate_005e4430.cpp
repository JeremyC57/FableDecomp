// CTCQuestCompletionUI::Activate(CompletionType type, CTCQuestCard* card, bool arg) -- open the quest-completion screen: set the wait flags and completion type; with a card copy its script name, def, rewards and boasts (dropping boasts for other scripts), pick the rendered object (an augmentation reward on a type-1 completion of a reward card), fetch the summary / success / name texts; without one clear them; then create the screen-filter component, close the player GUI and stamp the render time
// Leaf: Activate
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E4430, 778 bytes, `ret 0Ch`.
// Layout: Ego_r.pdb CTCQuestCompletionUI with retail containers (Components 12 bytes, Boasts list 4 bytes): Active +0xc,
//   WaitingForRegionLoad +0x1d, WaitingForPlayerControl +0x1e, WaitingForBoastProcessing +0x1f, Completion +0x20, Frame +0x24,
//   FrameStartedBoastProcessing +0x28, PCardDef +0x2c, Boasts +0x30, RenderedObjectDef +0x34, QuestSummary +0x38,
//   SuccessSummary +0x3c, QuestObjective +0x40, LastRenderTime +0x48, ScriptName +0x50, GoldReward +0x58, RenownReward +0x5c,
//   PBlack +0xbc and QuestCardDef +0xc4 (PDB -0x14 after the three maps).
extern "C" void __cdecl free(void*);
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
    CCharString& operator=(const char* text);              // 0x0099EFE0
    bool operator!=(const CCharString& other) const;       // 0x0099E900
};
class CWideString { public:
    void* m_pData;
    ~CWideString();                                        // 0x0099B510
    CWideString& operator=(const CWideString& other);      // 0x0099B7D0
};
struct CDef { virtual void v00(); virtual void DeleteThis(); long RefCount;
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
class CQuestCardDef : public CDef { public:
    unsigned char _pad08[0x4c - 0x08];
    vector<long> RewardObjects;                            // +0x4c
};
struct CObjectDef : CDef {
    unsigned char _pad08[0x7d - 0x08];
    unsigned char Type;                                    // +0x7d
};
class CDefinitionManager { public:
    static CDefinitionManager* __fastcall Get();           // 0x0044C6B0
    bool GetDef(long index, CDefPointer<CObjectDef>* out); // 0x0042B040 (same-TU stand-in below)
};
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CObjectDef>* out)
{
    out->P = (CObjectDef*)index;
    return true;
}
#pragma auto_inline(on)
bool __fastcall IsAugmentation_005d9930(long index);                 // 0x005D9930
class CThing { public: unsigned char _pad00[0x8c]; unsigned short DefIndex; };
class CTCQuestCard { public:
    void*  vftable;
    CThing* Owner;                                         // +0x04
    CThing* GetOwner() const { return Owner; }
    unsigned char _pad08[0x0c - 0x08];
    CDefPointer<CQuestCardDef> PDef;                       // +0x0c
    unsigned char _pad10[0x28 - 0x10];
    CCharString ScriptName;                                // +0x28
    long GoldReward;                                       // +0x2c
    long RenownReward;                                     // +0x30
    CWideString GetQuestSummary();                         // 0x00702400
    CWideString GetSuccessSummary();                       // 0x00702630
    CWideString GetQuestName();                            // 0x00702470
};
struct CBoast { long a, b, c; CCharString ScriptName; CWideString Text0; CWideString Text1; };
struct CBoastNode { CBoastNode* next; CBoastNode* prev; CBoast data; };
struct CBoastList {
    CBoastNode* node;
    CBoastList& operator=(const CBoastList& o);            // 0x005E9630
    void clear();                                          // 0x004B8C00
};
class CQuestManager { public: unsigned char _pad00[0x6c]; CBoastList Boasts; };
extern CQuestManager* g_pQuestManager;                     // 0x013B89FC
struct CComponent { virtual void v00(); };
struct CFrontEndManager {
    static CFrontEndManager* __fastcall GetInstance();     // 0x0041E5F2
    CComponent* CreateComponent(const CCharString& name, bool b);   // 0x0041DB1D
};
struct CScreenFilter {
    void* vfp;
};
class CPlayerGui { public: void Close(bool b); };                 // 0x00438400
extern CPlayerGui* g_pPlayerGui;                           // 0x013B8790
double __fastcall GFGetTime();                             // 0x009E1BC0
enum CompletionType { COMPLETION_0, COMPLETION_1, COMPLETION_2, COMPLETION_3 };
struct CFilterComponent {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14();
    virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c(); virtual void v40(); virtual void v44();
    virtual void v48(); virtual void v4c(); virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c(); virtual void v70(); virtual void v74();
    virtual void v78(); virtual void v7c(); virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9c(); virtual void va0(); virtual void va4();
    virtual void va8(); virtual void Show();                                  // +0xac
    virtual void vb0(); virtual void vb4(); virtual void vb8(); virtual void vbc();
    virtual void SetFocus(bool b);                                            // +0xc0
};
class CTCQuestCompletionUI { public:
    void* vftable;                                         // +0x00 (a real vptr pads to 8 beside the double)
    unsigned char _pad04[0x0c - 0x04];
    bool Active;                                           // +0x0c
    unsigned char _pad0d[0x1d - 0x0d];
    bool WaitingForRegionLoad;                             // +0x1d
    bool WaitingForPlayerControl;                          // +0x1e
    bool WaitingForBoastProcessing;                        // +0x1f
    CompletionType Completion;                             // +0x20
    unsigned long Frame;                                   // +0x24
    long FrameStartedBoastProcessing;                      // +0x28
    CDefPointer<CQuestCardDef> PCardDef;                   // +0x2c
    CBoastList Boasts;                                     // +0x30
    unsigned long RenderedObjectDef;                       // +0x34
    CWideString QuestSummary;                              // +0x38
    CWideString SuccessSummary;                            // +0x3c
    CWideString QuestObjective;                            // +0x40
    unsigned char _pad44[0x48 - 0x44];
    double LastRenderTime;                                 // +0x48
    CCharString ScriptName;                                // +0x50
    unsigned char _pad54[0x58 - 0x54];
    long GoldReward;                                       // +0x58
    long RenownReward;                                     // +0x5c
    unsigned char _pad60[0xbc - 0x60];
    CFilterComponent* PBlack;                              // +0xbc
    unsigned char _padc0[0xc4 - 0xc0];
    long QuestCardDef;                                     // +0xc4
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
    PBlack = (CFilterComponent*)CFrontEndManager::GetInstance()->CreateComponent("PC_SCREEN_FILTER", false);
    PBlack->Show();
    PBlack->SetFocus(true);
    g_pPlayerGui->Close(true);
    LastRenderTime = GFGetTime();
}