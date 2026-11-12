// CTCQuestCard::GetQuestObjective() const -- the card's objective text: the script-set ObjectiveName symbol looked up in the text bank (or the symbol itself widened when the bank has no entry), else the def's QuestObjective text entry, else the empty string
// Leaf: GetQuestObjective
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00702470, 232 bytes (`ret 4`, CWideString by hidden pointer; PDB ?GetQuestObjective@CTCQuestCard@@QBE?AVCWideString@@XZ).
//   The catalog labels this body "GetQuestName" (propagated from the 0x00702310 twin): it reads ObjectiveName +0x1c and the def's
//   QuestObjective +0x30, so it is GetQuestObjective.
// Callees: CCharString::operator!=(const char*) @ 0x0099E960 ("" @ 0x0122D70E), CDataBank::GetTextBySymbol @ 0x009C95E0 on
//   CMainGameComponent::Get() @ 0x013B86A0 ->TextBank (+0x14), CWideString::operator!=(const wchar_t*) @ 0x0099B150 (L"" @ 0x0122D70C),
//   CWideString(const CWideString&) @ 0x0099B720, ~CWideString @ 0x0099B510, CCharString::ToWideString() @ 0x0099E480,
//   CDataBank::GetPGroupEntry(unsigned long, bool) @ 0x009C9070, CGroupEntry::GetText() @ 0x009C7A40, CWideString::NullString @ 0x013BCA24.
// Layout: Ego_r.pdb CTCQuestCard (ObjectiveName 0x1c), CQuestCardDef QuestObjective 0x30 (text index); NGameText::CGroupEntry ref count +0x04.
// Shape: the looked-up text is a named local returned through the copy constructor (no NRVO), the widened symbol passes the
//   hidden pointer straight through, each with its own ~CWideString; the def index is read twice (`PDef->QuestObjective > 0`, then
//   as the argument) -- one load in ecx copied to eax for the push, where a named index local loads eax directly; the group
//   entry's release is the pointee's long `DecRef(); if (GetRefCount() == 0) Delete();` (`dec edi ; mov edx,edi ; test edx,edx`).
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): 0x013BCA24 is CWideString::NullString (?NullString@CWideString@@2V1@A, public static;
//   Ego_r.pdb + FableWin.pdb `dynamic initializer for 'CWideString::NullString'', initializer 0x01223DA0 landed by agentFAM09); was g_EmptyWideString.
#include <stddef.h>
class CWideString { public:
    void* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    bool operator!=(const wchar_t* text) const;         // 0x0099B150
public:
    static CWideString NullString;                  // 0x013BCA24 ?NullString@CWideString@@2V1@A
};
class CCharString { public:
    void* PStringData;
    bool operator!=(const char* text) const;            // 0x0099E960
    CWideString ToWideString() const;                   // 0x0099E480
};
class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags) const;                    // slot 0
    virtual void Delete() const;                              // slot 1
    mutable unsigned long IVRefCount;                           // +0x04
    void DecRef() const { --IVRefCount; }
    unsigned long GetRefCount() const { return IVRefCount; }
    void Release() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};
template <class T> class CIVCountedPointer { public:           // 4 bytes
    T* Object;
    ~CIVCountedPointer()
    {
        if (Object)
            Object->Release();
    }
};
namespace NGameText {
class CTextEntry : public CIVCountedPointeeBase { public:            // NGameText::CGroupEntry
    CWideString GetText() const;                        // 0x009C7A40
};
}
using NGameText::CTextEntry;
namespace NGameText {
class CDataBank { public:                                      // NGameText::CDataBank
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
    CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long index, bool load) const;   // 0x009C9070
};
}
using NGameText::CDataBank;
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad00[0x14];
    CDataBank*    TextBank;                             // +0x14
};
class CQuestCardDef { public:
    unsigned char _pad00[0x28];
    long QuestName;                                     // +0x28 text index
    long QuestSummary;                                  // +0x2c
    long QuestObjective;                                // +0x30
    long SuccessSummary;                                // +0x34
};
template <class T> class CDefPointer { public:
    const T* Object;
    const T* operator->() const { return Object; }
};
class CThing;
class CTCQuestCard { public:
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

CWideString CTCQuestCard::GetQuestObjective() const
{
    if (ObjectiveName != "") {
        CWideString text = CMainGameComponent::Get()->TextBank->GetTextBySymbol(ObjectiveName);
        if (text != L"")
            return text;
        return ObjectiveName.ToWideString();
    }
    if (PDef->QuestObjective > 0) {
        CIVCountedPointer<const CTextEntry> entry = CMainGameComponent::Get()->TextBank->GetPTextEntry(PDef->QuestObjective, true);
        if (entry.Object)
            return entry.Object->GetText();
    }
    return CWideString::NullString;
}