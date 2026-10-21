// CTCHasName::GetNPCShortName() -- the thing manager's stored name for the owner if it has one; else the def's default name tag
// from the text bank (a plain entry, or a random member of a group entry), remembered in the thing manager; else empty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F4990, 444 bytes (+ int3 padding), `ret 4` (hidden CWideString result).
// Leaf: GetNPCShortName
// Grade: DIFFER(448v449) -- (1) the 64-bit owner UID argument: retail `mov ecx,[ebx+4] ; mov eax,[ecx+18h] ; mov ecx,[ecx+1Ch]`
// (low word first, ParentThing in ecx), ours loads the parent into eax and the high word first; (2) the stored-name local
// sits at a different frame word (retail shares the `name` slot), so the first branch is one byte longer/shifted.
// Callees: GetThingManager 0x00686AE0, HasNameForThing 0x0051F010, GetNameForThing 0x0051F040, SetNameForThing 0x00521A90, the
// text bank's EntryExists 0x009CC190 / GetEntryDataType 0x009CC210 / GetPGroupEntry 0x009C9070 / group-list getter 0x009C8890,
// CGroupEntry::GetText 0x009C7A40, rand 0x00BFEB16, CWideString 0x0099AED0 / 0x0099B720 / = 0x0099B7D0 / ~ 0x0099B510.
// Layout: landed sibling CTCHasName_GetNPCNameAndFullTitle_007f4b60 model (PDef +0x0c, CHasNameDef DefaultNameTag +0x28).
typedef unsigned short wchar16;

extern "C" int __cdecl rand();                              // 0x00BFEB16

struct CWideString {
    void* PStringData;
    CWideString();                                          // 0x0099AED0
    CWideString(const CWideString& other);                  // 0x0099B720
    ~CWideString();                                         // 0x0099B510
    CWideString& operator=(const CWideString& other);       // 0x0099B7D0
};

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy() const;                           // slot 1
};
struct CRefCounted {
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
template <class T> struct CIVCountedPointer {
    T* Object;
    ~CIVCountedPointer() { if (Object) Object->ReleaseTemporary(); }
    T* operator->() const { return Object; }
};

template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long i) const { return _M_start[i]; }
};

namespace NGameText {
struct CGroupEntry : CIVCountedPointee {
    CWideString GetText() const;                            // 0x009C7A40
};
struct CGroupList : CIVCountedPointee {                     // a group of text entries
    unsigned char _pad08[0x1c - 0x08];
    CRetailVector<unsigned long> Entries;                   // +0x1c
};
struct CDataBank {
    bool EntryExists(unsigned long index);                  // 0x009CC190
    unsigned long GetEntryDataType(unsigned long index);    // 0x009CC210
    CIVCountedPointer<const CGroupEntry> GetPGroupEntry(unsigned long index, bool load) const;   // 0x009C9070
    CIVCountedPointer<const CGroupList> GetPGroupList(unsigned long index, bool load) const;     // 0x009C8890
};
}

struct CMainGameComponent {
    unsigned char        _pad00[0x14];
    NGameText::CDataBank* TextBank;                         // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0

struct CHasNameDef {
    unsigned char _pad00[0x28];
    unsigned long DefaultNameTag;                           // +0x28
};

struct CThingUID { unsigned long Low, High; };
struct CThingManager {
    bool HasNameForThing(CThingUID uid);                         // 0x0051F010
    CWideString GetNameForThing(CThingUID uid);                  // 0x0051F040
    void SetNameForThing(CThingUID uid, const CWideString& name);   // 0x00521A90
};

class CThing {
public:
    unsigned char _pad00[0x18];
    CThingUID UID;                                   // +0x18
    CThingUID GetUID() const { return UID; }
};
class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CThingManager* GetThingManager() const;                 // 0x00686AE0
};

class CTCHasName : public CTCBase {
public:
    const CHasNameDef* PDef;                                // +0x0c CDefPointer<CHasNameDef const>
    const CHasNameDef* GetDef() const { return PDef; }
    CWideString GetNPCShortName();
};

CWideString CTCHasName::GetNPCShortName()
{
    if (GetThingManager()->HasNameForThing(ParentThing->GetUID())) {
        CWideString stored = GetThingManager()->GetNameForThing(ParentThing->GetUID());
        return stored;
    }
    NGameText::CDataBank* bank = g_pMainGameComponent->TextBank;
    CWideString name;
    if (bank->EntryExists(GetDef()->DefaultNameTag)) {
        if (bank->GetEntryDataType(GetDef()->DefaultNameTag) == 0) {
            CIVCountedPointer<const NGameText::CGroupEntry> entry = bank->GetPGroupEntry(GetDef()->DefaultNameTag, true);
            name = entry->GetText();
        } else {
            CIVCountedPointer<const NGameText::CGroupList> group = bank->GetPGroupList(GetDef()->DefaultNameTag, true);
            unsigned long count = group->Entries.size();
            name = bank->GetPGroupEntry(group->Entries[rand() % count], true)->GetText();
        }
        GetThingManager()->SetNameForThing(ParentThing->GetUID(), name);
    }
    return name;
}
