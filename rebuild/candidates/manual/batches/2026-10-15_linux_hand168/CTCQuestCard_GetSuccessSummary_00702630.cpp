// CTCQuestCard::GetSuccessSummary() const -- the game text of the quest card def's SuccessSummary tag (looked up in the game-text data bank), else the empty wide string
// Leaf: GetSuccessSummary
// Label: catalog "CTCQuestCard::GetQuestSummary" (byte twin of 0x00702400); this body reads CQuestCardDef +0x34 = SuccessSummary.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00702630, 112 bytes, `ret 4` (hidden CWideString result).
// Callees: NGameText::CDataBank::GetPGroupEntry(unsigned long, bool) const @ 0x009C9070 (CIVCountedPointer by hidden pointer),
//   NGameText::CGroupEntry::GetText() const @ 0x009C7A40 (CWideString by hidden pointer), CWideString::CWideString(const
//   CWideString&) @ 0x0099B720 from the empty-string global 0x013BCA24, the entry's slot 1 on release. The engine singleton
//   0x013B86A0 (+0x14 = the game-text data bank).
// Layout: PDB CTCQuestCard PDef +0x0c (CDefPointer<CQuestCardDef const>); CQuestCardDef::SuccessSummary +0x34 (PDB).
// Shape: the shape of the CTCHero::GetTitleGuiText row (0x006CA0A0, wave 25): the tag read twice (test, argument) gives the
//   `mov eax,ecx` copy; the counted entry's dtor has a null test and calls the inline ReleaseTemporary (DecRef / GetRefCount on
//   the ref-count base at +4).
class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);      // 0x0099B720
};
extern const CWideString g_emptyWideString_013bca24;    // 0x013BCA24

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CRefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
namespace NGameText {
struct CGroupEntry : CRefVirtual, CRefCounted {
    CWideString GetText() const;                // 0x009C7A40
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
}
template <class T> struct CIVCountedPointer {
    T* Data;
    ~CIVCountedPointer() { if (Data) Data->ReleaseTemporary(); }
    T* operator->() const { return Data; }
};
namespace NGameText {
class CDataBank {
public:
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(unsigned long tag, bool b) const;   // 0x009C9070
};
}
struct CEngine {
    unsigned char _pad00[0x14];
    NGameText::CDataBank* PGameTextDataBank;    // +0x14
};
extern CEngine* g_pEngine;                      // 0x013B86A0
struct CDefPointeeBase { virtual void v00(); long m_refCount; };
struct CQuestCardDef : CDefPointeeBase {
    unsigned char _pad08[0x28 - 0x08];
    long QuestName;                             // +0x28
    long QuestSummary;                          // +0x2c
    long QuestObjective;                        // +0x30
    long SuccessSummary;                        // +0x34
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
class CTCQuestCard {
public:
    virtual ~CTCQuestCard();
    void*         ParentThing;                  // +0x04
    unsigned char _pad08[0x04];
    CDefPointer<const CQuestCardDef> PDef;      // +0x0c
    CWideString GetSuccessSummary() const;
};

CWideString CTCQuestCard::GetSuccessSummary() const
{
    if (PDef->SuccessSummary > 0) {
        CIVCountedPointer<NGameText::CGroupEntry> entry = g_pEngine->PGameTextDataBank->GetPGroupEntry(PDef->SuccessSummary, true);
        if (entry.Data)
            return entry->GetText();
    }
    return g_emptyWideString_013bca24;
}
