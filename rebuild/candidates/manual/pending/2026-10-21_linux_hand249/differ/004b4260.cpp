// CQuestManager::ActivateMultipleQuests(const CArray<CCharString>& questNames, bool p2, bool p3) -- queue every registered quest of the list with its script info, activate them together; true when any was registered
// Grade: DIFFER(ours v retail) -- 336v359: register allocation -- retail: names ebp, this edi (spilled around the info), activated bl, i frame-only, esi a 0 constant re-zeroed per iteration; ours: i edi, this ebp, activated in the frame
// Leaf: ?ActivateMultipleQuests@CQuestManager@@QAE_NABV
extern "C" void __cdecl free(void* p);               // 0x00BFEA14
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
};
namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b);   // 0x009E9F40
}
template<class T> struct CArray {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const T& operator[](unsigned long i) const { return _M_start[i]; }
};
struct CScriptInfo;
struct CScriptInfoManager {
    const CScriptInfo* GetScriptInfo(const CCharString& name);   // 0x00CB5AD0
};
struct CQuestManager {
    struct CActivateQuestInfo {                         // 0x0c bytes
        CCharString        QuestName;
        const CScriptInfo* Info;
        bool               P2;
        bool               P3;
        CActivateQuestInfo(const CCharString& name, const CScriptInfo* info, bool p2, bool p3)
            : QuestName(name), Info(info), P2(p2), P3(p3) { }
    };
    struct CActivateList {                               // STLport vector<CActivateQuestInfo>
        CActivateQuestInfo* _M_start; CActivateQuestInfo* _M_finish; CActivateQuestInfo* _M_end_of_storage;
        CActivateList() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
        ~CActivateList()
        {
            Destroy(_M_start, _M_finish);
            if (_M_start)
                free(_M_start);
        }
        void push_back(const CActivateQuestInfo& x);  // 0x004BB720
        static void Destroy(CActivateQuestInfo* first, CActivateQuestInfo* last)
        {
            for (; first != last; ++first)
                first->~CActivateQuestInfo();
        }
    };
    unsigned char       _pad00[0x78];
    CScriptInfoManager* ScriptInfoManager;              // +0x78
    bool IsQuestRegistered(const CCharString& name);   // 0x004B00C0
    void ActivateQuests(const CActivateList& quests);   // 0x004B3CE0
    bool ActivateMultipleQuests(const CArray<CCharString>& questNames, bool p2, bool p3);
};

bool CQuestManager::ActivateMultipleQuests(const CArray<CCharString>& questNames, bool p2, bool p3)
{
    CActivateList quests;
    bool activated = false;
    for (unsigned long i = 0; i < questNames.size(); ++i) {
        NProgressDisplay::DisplayProgress("QuestManager: Activate Quest", -1.0f, false, false);
        const CCharString& name = questNames[i];
        if (IsQuestRegistered(name)) {
            const CScriptInfo* info = ScriptInfoManager->GetScriptInfo(name);
            if (info)
                quests.push_back(CActivateQuestInfo(name, info, p2, p3));
            else
                quests.push_back(CActivateQuestInfo(name, 0, p2, p3));
            activated = true;
        }
    }
    ActivateQuests(quests);
    return activated;
}
