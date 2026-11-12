// CTCHero::GetTitleGuiText() const -- the GUI text of the hero's current title (the title sub-def's HeroTitleGuiTag looked up in the game-text data bank), else the empty wide string
// Naming wave 18: the global stand-in g_pEngine (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CA0A0, length 114 bytes (+14 int3 padding), `ret 4` (hidden CWideString result).
// Ego_r.pdb: ?GetTitleGuiText@CTCHero@@QBE?AVCWideString@@XZ
// Callees: NGameText::CDataBank::GetPGroupEntry(unsigned long, bool) const @ 0x009C9070 (CIVCountedPointer by hidden pointer),
// the group entry's text getter @ 0x009C7A40 (manifest "CBankFile::GetBankHandle", propagated; CWideString by hidden pointer),
// CWideString::CWideString(const CWideString&) @ 0x0099B720 from the empty-string global 0x013BCA24, the entry's slot 1 on release.
// Globals: the engine singleton 0x013B86A0 (+0x14 = the game-text data bank).
// Shape: the counted entry pointer's dtor has a null test and calls the entry's inline ReleaseTemporary(), whose DecRef /
// GetRefCount sit on the ref-count base at +4 (`mov edx,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edx ; test ; mov [eax],edx`).
// The tag is read twice from the def (test, then argument): the CSE gives retail's `mov eax,ecx` copy; a named local does not.
// Layout: Ego_r.pdb CTCHero: PHeroTitleSubDef +0x18 (CDefPointer<const CHeroTitleDef>); CHeroTitleDef::HeroTitleGuiTag +0x2c.
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
class CHeroTitleDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x2c - 0x08];
    long HeroTitleGuiTag;                       // +0x2c
};
template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};
class CThing;
class CTCHero {
public:
    virtual ~CTCHero();
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad08[0x18 - 0x08];
    CDefPointer<const CHeroTitleDef> PHeroTitleSubDef;   // +0x18
    CWideString GetTitleGuiText() const;
};

CWideString CTCHero::GetTitleGuiText() const
{
    if (PHeroTitleSubDef.Object && PHeroTitleSubDef->HeroTitleGuiTag > 0) {
        CIVCountedPointer<const NGameText::CTextEntry> entry =
            CMainGameComponent::Get()->PGameTextDataBank->GetPTextEntry(PHeroTitleSubDef->HeroTitleGuiTag, true);
        if (entry.Data)
            return entry->GetText();
    }
    return CWideString::NullString;
}