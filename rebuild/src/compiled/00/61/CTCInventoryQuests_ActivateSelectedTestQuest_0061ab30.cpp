// CTCInventoryQuests::ActivateSelectedTestQuest() -- in test-quest mode, runs the highlighted test quest's ini script when named and present, restarts the quest from scratch (deactivate, clear its persisted script, load its HSP) and activates it, through its quest card when it has one
// Retail 0x0061AB30, 304 bytes (+ int3), esp frame (speed TU); the twin of the landed-here DeactivateSelectedTestQuest
// 0x0061AC60.
// Callees: GetTestQuestsToDisplay(vector<CTestQuest>*) 0x0061A8A0, CQuestManager::DeactivateQuest(const CCharString&,
// unsigned long) 0x004B43D0, ClearQuestPersistScript(const CCharString&) 0x004B39B0, ActivateTestQuestCard(const
// CCharString&, const CCharString&) 0x004B4C50 and ActivateQuest(const CCharString&, bool, bool) 0x004B4A10 (the global
// quest manager 0x013B89FC), CTCBase::GetWorld 0x00686A70 and the world's HSP loader 0x004A0940 (manifest
// "NQuestManager_ActivateQuestWithFilters"; name descriptive), CCharString::operator!=(const char*) 0x0099E960 (with ""), operator+(const char*, const CCharString&)
// 0x0099F690 (__fastcall, hidden result in ecx), CCharString::ToWideString() const 0x0099E480, ~CCharString 0x0099EAE0,
// CAFile::PathExists(const CWideString&) 0x00999230 (__fastcall), CConsole::Get() 0x00414C90 and its config-file
// runner 0x009EC890 (name descriptive), ~CWideString 0x0099B510, the CTestQuest range destructor 0x004ABD90
// (__fastcall first/last + tag), free 0x00BFEA14.
// Shape: as DeactivateSelectedTestQuest: the selected quest is addressed once; the ini path is "Data\Levels\Ini\" +
// IniFile; the quest manager global is loaded once above the card/plain activation branch; the list dies through the range destructor
// (tag homed in a dead frame byte) and free.
// Layout (retail): ActiveTestQuestMode +0x157, TestQuestSelectedIndex +0x158 (Ego_r.pdb names, 0x20 below the PDB);
// CTestQuest QuestName +0x00, IniFile +0x10, OutiFile +0x14.
// Leaf: ActivateSelectedTestQuest
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
    void DeactivateQuest(const CCharString& name, unsigned long reason);        // 0x004B43D0
    void ClearQuestPersistScript(const CCharString& name);                     // 0x004B39B0
    bool ActivateTestQuestCard(const CCharString& name, const CCharString& cardDefName);   // 0x004B4C50
    void ActivateQuest(const CCharString& name, bool a, bool b);               // 0x004B4A10
};
extern CQuestManager* g_pQuestManager;             // 0x013B89FC
class CWorld { public:
    void LoadQuestHSP(const CCharString& hspName);  // 0x004A0940
};
class CConsole {
public:
    static CConsole* Get();                        // 0x00414C90
    void RunScript(const CWideString& path);   // 0x009EC890
};

class CTCBase
{
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};
namespace NInventory {
class CTCInventoryBase : public CTCBase
{
public:
};
}

class CTCInventoryQuests : public NInventory::CTCInventoryBase { public:
    unsigned char _pad_0x000[0x157];
    bool ActiveTestQuestMode;                      // +0x157
    long TestQuestSelectedIndex;                   // +0x158
    void GetTestQuestsToDisplay(CTestQuestVector* quests);   // 0x0061A8A0
    void ActivateSelectedTestQuest();
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

void CTCInventoryQuests::ActivateSelectedTestQuest()
{
    if (!ActiveTestQuestMode)
        return;
    CTestQuestVector quests;
    GetTestQuestsToDisplay(&quests);
    CTestQuest& quest = quests[TestQuestSelectedIndex];
    if (quest.IniFile != "") {
        CWideString path = ("Data\\Levels\\Ini\\" + quest.IniFile).ToWideString();
        if (CAFile::PathExists(path))
            CConsole::Get()->RunScript(path);
    }
    g_pQuestManager->DeactivateQuest(quest.QuestName, 0);
    g_pQuestManager->ClearQuestPersistScript(quest.QuestName);
    GetWorld().LoadQuestHSP(quest.HSPName);
    if (quest.QuestCardDefName != "")
        g_pQuestManager->ActivateTestQuestCard(quest.QuestName, quest.QuestCardDefName);
    else
        g_pQuestManager->ActivateQuest(quest.QuestName, true, true);
}