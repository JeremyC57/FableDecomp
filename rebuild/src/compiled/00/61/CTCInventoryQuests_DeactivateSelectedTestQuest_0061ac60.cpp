// CTCInventoryQuests::DeactivateSelectedTestQuest() -- in test-quest mode, completes and deactivates the highlighted test quest and runs its "out" ini script when one is named and present
// Retail 0x0061AC60, 256 bytes (+ int3), esp frame (speed TU).
// Callees: GetTestQuestsToDisplay(vector<CTestQuest>*) 0x0061A8A0, CQuestManager::SetQuestAsCompleted(const CCharString&,
// bool, bool, bool) 0x004B1D30, OnQuestFinishScreenDisplayed(const CCharString&) 0x004B0160 (only while the global flag
// 0x01375741 is set), DeactivateQuest(const CCharString&, unsigned long) 0x004B43D0 (the global quest manager
// 0x013B89FC), CCharString::operator!=(const char*) 0x0099E960 (with ""), operator+(const char*, const CCharString&)
// 0x0099F690 (__fastcall, hidden result in ecx), CCharString::ToWideString() const 0x0099E480, ~CCharString 0x0099EAE0,
// CAFile::PathExists(const CWideString&) 0x00999230 (__fastcall), CConsole::Get() 0x00414C90 and its config-file
// runner 0x009EC890 (name descriptive), ~CWideString 0x0099B510, the CTestQuest range destructor 0x004ABD90
// (__fastcall first/last + tag), free 0x00BFEA14.
// Shape: the displayed quest list is a zeroed local; the selected quest (28-byte CTestQuest) is addressed once; the ini
// path is "Data\Levels\Ini\" + OutiFile converted to a wide string; the list dies through the range destructor
// (tag homed in a dead frame byte) and free.
// Layout (retail): ActiveTestQuestMode +0x157, TestQuestSelectedIndex +0x158 (Ego_r.pdb names, 0x20 below the PDB);
// CTestQuest QuestName +0x00, IniFile +0x10, OutiFile +0x14.
// Leaf: DeactivateSelectedTestQuest
typedef unsigned short wchar_t;                    // VC7.1 without /Zc:wchar_t
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

class CWideString { public:
    void* m_pStringData;
    CWideString(const CWideString& other);
    ~CWideString();                                // 0x0099B510
};
class CCharString { public:
    void* m_pStringData;
    CCharString(const CCharString& other);
    ~CCharString();                                // 0x0099EAE0
    bool operator!=(const char* text) const;       // 0x0099E960
    CWideString ToWideString() const;              // 0x0099E480
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
class CAFile { public: static bool __fastcall PathExists(const CWideString& path); };   // 0x00999230

class CTestQuest { public:                                // 28 bytes (Ego_r.pdb)
    CCharString QuestName;                         // +0x00
    CCharString HSPName;                           // +0x04
    long ColourIndex;                              // +0x08
    CCharString DisplayName;                       // +0x0c
    CCharString IniFile;                           // +0x10
    CCharString OutiFile;                          // +0x14
    CCharString QuestCardDefName;                  // +0x18
};
struct __false_type { __false_type() {} };
void __fastcall _Destroy(CTestQuest* first, CTestQuest* last, const __false_type& tag);   // 0x004ABD90
struct CTestQuestVector {                          // vector<CTestQuest> (12 bytes)
    CTestQuest* _M_start;
    CTestQuest* _M_finish;
    CTestQuest* _M_end_of_storage;
    CTestQuestVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CTestQuestVector()
    {
        _Destroy(_M_start, _M_finish, __false_type());
        if (_M_start)
            free(_M_start);
    }
    CTestQuest& operator[](unsigned long n) { return *(_M_start + n); }
};
class CQuestManager { public:
    bool SetQuestAsCompleted(const CCharString& name, bool a, bool b, bool c);   // 0x004B1D30
    bool OnQuestFinishScreenDisplayed(const CCharString& name);                 // 0x004B0160
    void DeactivateQuest(const CCharString& name, unsigned long reason);        // 0x004B43D0
};
extern CQuestManager* g_pQuestManager;             // 0x013B89FC
extern bool g_bQuestFinishScreensEnabled;          // 0x01375741 (descriptive)
class CConsole {
public:
    static CConsole* Get();                        // 0x00414C90
    void RunScript(const CWideString& path);   // 0x009EC890
};

class CTCInventoryQuests { public:
    unsigned char _pad_0x000[0x157];
    bool ActiveTestQuestMode;                      // +0x157
    long TestQuestSelectedIndex;                   // +0x158
    void GetTestQuestsToDisplay(CTestQuestVector* quests);   // 0x0061A8A0
    void DeactivateSelectedTestQuest();
};

CTestQuest* GetTestQuestTable(unsigned long* count);   // stand-in callee of the stand-in below
// Same-TU stand-in for GetTestQuestsToDisplay 0x0061A8A0 (retail compiles it in this TU; not the real body): it reads
// and writes the caller's vector only through the out pointer (the read keeps the caller's zero stores live), so VC7.1 knows the vector does not escape and keeps its start
// in edi across the quest-manager calls -- review at landing.
#pragma auto_inline(off)
void CTCInventoryQuests::GetTestQuestsToDisplay(CTestQuestVector* quests)
{
    unsigned long count;
    CTestQuest* table = GetTestQuestTable(&count);
    if ((unsigned long)(quests->_M_end_of_storage - quests->_M_start) < count) {
        quests->_M_start = table;
        quests->_M_end_of_storage = table + count;
    }
    quests->_M_finish = quests->_M_start + count;
}
#pragma auto_inline(on)

void CTCInventoryQuests::DeactivateSelectedTestQuest()
{
    if (!ActiveTestQuestMode)
        return;
    CTestQuestVector quests;
    GetTestQuestsToDisplay(&quests);
    CTestQuest& quest = quests[TestQuestSelectedIndex];
    g_pQuestManager->SetQuestAsCompleted(quest.QuestName, false, false, false);
    if (g_bQuestFinishScreensEnabled)
        g_pQuestManager->OnQuestFinishScreenDisplayed(quest.QuestName);
    g_pQuestManager->DeactivateQuest(quest.QuestName, 0);
    if (quest.OutiFile != "") {
        CWideString path = ("Data\\Levels\\Ini\\" + quest.OutiFile).ToWideString();
        if (CAFile::PathExists(path))
            CConsole::Get()->RunScript(path);
    }
}