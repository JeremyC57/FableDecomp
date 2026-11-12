// CTCQuestCard::GetSuccessSummary() const -- the game text of the quest card def's SuccessSummary tag (looked up in the game-text data bank), else the empty wide string
// Leaf: GetSuccessSummary
// Naming wave 18: the global stand-in g_pEngine (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
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
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): 0x013BCA24 is CWideString::NullString (?NullString@CWideString@@2V1@A, public static;
//   Ego_r.pdb + FableWin.pdb `dynamic initializer for 'CWideString::NullString'', initializer 0x01223DA0 landed by agentFAM09); was g_emptyWideString_013bca24.
class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);      // 0x0099B720
public:
    static CWideString NullString;                  // 0x013BCA24 ?NullString@CWideString@@2V1@A
};

struct CRefCounted {
    mutable long m_count;
    void DecRef() const { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CRefVirtual {
    virtual void v00();
    virtual void Destroy() const;                     // slot 1
};
namespace NGameText {
class CTextEntry : public CRefVirtual, public CRefCounted { public:
    CWideString GetText() const;                // 0x009C7A40
    void ReleaseTemporary() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
}
template <class T> class CIVCountedPointer { public:
    T* Data;
    ~CIVCountedPointer() { if (Data) Data->ReleaseTemporary(); }
    T* operator->() const { return Data; }
};
namespace NGameText {
class CDataBank {
public:
    CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long tag, bool b) const;   // 0x009C9070
};
}
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad00[0x14];
    NGameText::CDataBank* PGameTextDataBank;    // +0x14
};
class CDefPointeeBase { public: virtual void v00(); long m_refCount; };
class CQuestCardDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x28 - 0x08];
    long QuestName;                             // +0x28
    long QuestSummary;                          // +0x2c
    long QuestObjective;                        // +0x30
    long SuccessSummary;                        // +0x34
};
template <class T> class CDefPointer { public:
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
        CIVCountedPointer<const NGameText::CTextEntry> entry = CMainGameComponent::Get()->PGameTextDataBank->GetPTextEntry(PDef->SuccessSummary, true);
        if (entry.Data)
            return entry->GetText();
    }
    return CWideString::NullString;
}