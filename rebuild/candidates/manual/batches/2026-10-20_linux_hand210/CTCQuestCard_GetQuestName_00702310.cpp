// CTCQuestCard::GetQuestName() const -- the card's quest name text: the script-set QuestName symbol looked up in the text bank (or the symbol itself widened when the bank has no entry), else the def's QuestName text entry, else the empty string
// Leaf: GetQuestName
// Retail: Fable.exe (TLC, VC7.1) @ 0x00702310, 232 bytes (`ret 4`, CWideString by hidden pointer; PDB ?GetQuestName@CTCQuestCard@@QBE?AVCWideString@@XZ).
// Callees: CCharString::operator!=(const char*) @ 0x0099E960 ("" @ 0x0122D70E), CDataBank::GetTextBySymbol @ 0x009C95E0 on
//   g_pMainGameComponent @ 0x013B86A0 ->TextBank (+0x14), CWideString::operator!=(const wchar_t*) @ 0x0099B150 (L"" @ 0x0122D70C),
//   CWideString(const CWideString&) @ 0x0099B720, ~CWideString @ 0x0099B510, CCharString::ToWideString() @ 0x0099E480,
//   CDataBank::GetPGroupEntry(unsigned long, bool) @ 0x009C9070, CGroupEntry::GetText() @ 0x009C7A40, g_EmptyWideString @ 0x013BCA24.
// Layout: Ego_r.pdb CTCQuestCard (QuestName 0x18), CQuestCardDef QuestName 0x28 (text index); NGameText::CGroupEntry ref count +0x04.
// Shape: the looked-up text is a named local returned through the copy constructor (no NRVO), the widened symbol passes the
//   hidden pointer straight through, each with its own ~CWideString; the def index is read twice (`PDef->QuestName > 0`, then
//   as the argument) -- one load in ecx copied to eax for the push, where a named index local loads eax directly; the group
//   entry's release is the pointee's long `DecRef(); if (GetRefCount() == 0) Delete();` (`dec edi ; mov edx,edi ; test edx,edx`).
#include <stddef.h>
struct CWideString {
    void* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    bool operator!=(const wchar_t* text) const;         // 0x0099B150
};
extern const CWideString g_EmptyWideString;             // 0x013BCA24
struct CCharString {
    void* PStringData;
    bool operator!=(const char* text) const;            // 0x0099E960
    CWideString ToWideString() const;                   // 0x0099E480
};
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);                    // slot 0
    virtual void Delete();                              // slot 1
    unsigned long IVRefCount;                           // +0x04
    void DecRef() { --IVRefCount; }
    unsigned long GetRefCount() const { return IVRefCount; }
    void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};
template <class T> struct CIVCountedPointer {           // 4 bytes
    T* Object;
    ~CIVCountedPointer()
    {
        if (Object)
            Object->Release();
    }
};
struct CGroupEntry : CIVCountedPointeeBase {            // NGameText::CGroupEntry
    CWideString GetText() const;                        // 0x009C7A40
};
struct CDataBank {                                      // NGameText::CDataBank
    CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(unsigned long index, bool load) const;   // 0x009C9070
};
struct CMainGameComponent {
    unsigned char _pad00[0x14];
    CDataBank*    TextBank;                             // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
struct CQuestCardDef {
    unsigned char _pad00[0x28];
    long QuestName;                                     // +0x28 text index
    long QuestSummary;                                  // +0x2c
    long QuestObjective;                                // +0x30
    long SuccessSummary;                                // +0x34
};
template <class T> struct CDefPointer {
    const T* Object;
    const T* operator->() const { return Object; }
};
struct CThing;
struct CTCQuestCard {
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    bool          ToBeRemoved;                          // +0x08
    bool          IsInGlobalUpdateTCs;                  // +0x09
    unsigned char _pad0a[2];
    CDefPointer<CQuestCardDef> PDef;                    // +0x0c
    bool          Finished;                             // +0x10
    bool          Replayable;                           // +0x11
    long          FeatIndex;                            // +0x14
    CCharString   QuestName;                            // +0x18
    CCharString   ObjectiveName;                        // +0x1c
    CCharString   RegionName;                           // +0x20
    CCharString   TeleporterRegionName;                 // +0x24
    CCharString   ScriptQuestName;                      // +0x28
    long          GoldReward;                           // +0x2c
    long          RenownReward;                         // +0x30
    CWideString GetQuestName() const;
    CWideString GetQuestObjective() const;
};

CWideString CTCQuestCard::GetQuestName() const
{
    if (QuestName != "") {
        CWideString text = g_pMainGameComponent->TextBank->GetTextBySymbol(QuestName);
        if (text != L"")
            return text;
        return QuestName.ToWideString();
    }
    if (PDef->QuestName > 0) {
        CIVCountedPointer<CGroupEntry> entry = g_pMainGameComponent->TextBank->GetPGroupEntry(PDef->QuestName, true);
        if (entry.Object)
            return entry.Object->GetText();
    }
    return g_EmptyWideString;
}
