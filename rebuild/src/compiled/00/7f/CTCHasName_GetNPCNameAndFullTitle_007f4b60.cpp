// CTCHasName::GetNPCNameAndFullTitle() -- the NPC's short name followed by " - " and its occupation text when the text bank has that entry; else the short name, or "Missing text tag" when the name is empty
// Retry 10b (accessor sweep, FIXES B-4/B-5): the def is read through an inline accessor; the plain member read gives the parked register difference.
// Leaf: GetNPCNameAndFullTitle
//
// Retail 0x007F4B60, 288 bytes incl. padding (`ret 4`: hidden CWideString result). Ego_r.pdb
// `?GetNPCNameAndFullTitle@CTCHasName@@QAE?AVCWideString@@XZ`.
// Callees: CTCHasName::GetNPCShortName() @ 0x007F4990, the text bank's EntryExists(unsigned long) @ 0x009CC190 (label
// "CBankFile::EntryExists"), NGameText::CDataBank::GetPGroupEntry(unsigned long, bool) const @ 0x009C9070
// (CIVCountedPointer<const CGroupEntry> by value), CGroupEntry's text getter @ 0x009C7A40 (label "CBankFile::GetBankHandle"
// is propagated; landed as GetText in CItem::GetItemDescription 0x005BD8A0), the entry's virtual slot 1 (release to zero),
// operator+(const CWideString&, const wchar_t*) @ 0x0099BF30 and operator+(const CWideString&, const CWideString&) @ 0x0099BE70
// (both __fastcall, hidden result in ecx), CWideString::CWideString(const CWideString&) @ 0x0099B720, CWideString::GetLength
// @ 0x0099B220 (label says CCharString), CWideString::CWideString(const wchar_t*) @ 0x0099B6B0, ~CWideString @ 0x0099B510.
// Globals: g_pMainGameComponent @ 0x013B86A0 (text bank +0x14), L" - " @ 0x0122E070, L"Missing text tag" @ 0x01271DA8.
// Layout: Ego_r.pdb CTCHasName: PDef +0x0c; CHasNameDef Occupation +0x30 (PDB offset, no shift for this def).
// Shape: the bank is a local read before the def field; the counted group entry is released inline through the pointee's
// ReleaseTemporary (DecRef/GetRefCount on a refcount base: the long `mov esi,[eax+4] ; ... ; dec esi ; mov edx,esi ; test`
// release); the concatenation is one `return name + L" - " + title;` (VC7.1 has no NRVO, so it copies into the result).
typedef unsigned short wchar16;

class CWideString { public:
    void* PStringData;
    CWideString(const wchar16* text);                       // 0x0099B6B0
    CWideString(const CWideString& other);                  // 0x0099B720
    ~CWideString();                                         // 0x0099B510
    long GetLength() const;                                 // 0x0099B220
};
CWideString __fastcall operator+(const CWideString& left, const wchar16* right);       // 0x0099BF30
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy() const;                           // slot 1
};
struct CRefCounted {                                        // the count sits at +4 behind the vptr
    mutable long m_count;
    void DecRef() const { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CIVCountedPointee : CDefVirtual, CRefCounted {
    void ReleaseTemporary() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CIVCountedPointer { public:
    T* Object;
    ~CIVCountedPointer() { if (Object) Object->ReleaseTemporary(); }
    T* operator->() const { return Object; }
};

namespace NGameText {
class CTextEntry : public CIVCountedPointee { public:
    CWideString GetText() const;                            // 0x009C7A40
};
class CDataBank { public:
    bool EntryExists(unsigned long index);                  // 0x009CC190
    CIVCountedPointer<const CTextEntry> GetPTextEntry(unsigned long index, bool load) const;   // 0x009C9070
};
}

class CMainGameComponent { public:
    unsigned char        _pad00[0x14];
    NGameText::CDataBank* TextBank;                         // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0

extern const wchar16 g_textSpacedDash[];                    // 0x0122E070 L" - "
extern const wchar16 g_textMissingTextTag[];                // 0x01271DA8 L"Missing text tag"

class CHasNameDef { public:
    unsigned char _pad00[0x28];
    unsigned long DefaultNameTag;                           // +0x28
    unsigned long Home;                                     // +0x2c
    unsigned long Occupation;                               // +0x30
};

class CThing;
class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCHasName : public CTCBase {
public:
    const CHasNameDef* PDef;                                // +0x0c CDefPointer<CHasNameDef const>
    const CHasNameDef* GetDef() const { return PDef; }
    CWideString GetNPCShortName();                          // 0x007F4990
    CWideString GetNPCNameAndFullTitle();
};

CWideString CTCHasName::GetNPCNameAndFullTitle()
{
    CWideString name = GetNPCShortName();
    NGameText::CDataBank* bank = g_pMainGameComponent->TextBank;
    if (bank->EntryExists(GetDef()->Occupation)) {
        CWideString title = bank->GetPTextEntry(GetDef()->Occupation, true)->GetText();
        CWideString fullTitle = name + g_textSpacedDash + title;
        return fullTitle;
    }
    if (name.GetLength() > 0)
        return name;
    return CWideString(g_textMissingTextTag);
}