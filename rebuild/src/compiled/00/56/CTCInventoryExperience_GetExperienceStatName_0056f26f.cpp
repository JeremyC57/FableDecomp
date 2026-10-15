// CTCInventoryExperience::GetExperienceStatName(long stat) -- the inventory text for a stat's label id, or the empty string
// Retail 0x0056F26F, 78 bytes, ret 8 (hidden CWideString result + stat). It indexes the def's
// ExperienceItemLabelStat vector (retail +0x55C, the twin 0x0056F2BD reads ExperienceItemLabelAbility
// at +0x568, 12 bytes later -- the PDB order), so the manifest label GetExperienceAbilityName is
// propagated from the twin; callers include the CTCInventoryStats screen (0x005D6A48..).
// Shape: PDef (the base's CDefPointer<CInventoryDef>, retail +0x68) is copied into a local -- inline
// copy constructor `if (p) ++p->refCount` (+4) -- the def's
// ExperienceItemLabelStat start is indexed with `stat`, and the copy dies right there: `if (--refCount == 0) Destroy()` (vtable
// slot 1) with no null test (the counted release lives on the pointee). A non-zero text id is
// resolved by CTCInventoryBase::GetInventoryText 0x005BD273 (thiscall, hidden result); otherwise the
// result is copy-constructed (CWideString(const CWideString&) 0x0099B720) from the global empty
// wide string at 0x013BCA24.
// Frame-pointer body without spills: `#pragma optimize("y", off)` + `("s", on)`.
// Leaf: GetExperienceStatName
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CWideString {
    void* m_pStringData;
    CWideString(const CWideString& other);         // 0x0099B720
    ~CWideString();                                // 0x0099B510
};
extern CWideString g_EmptyWideString;              // 0x013BCA24

struct SLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
};

struct CInventoryDef {
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long refCount;                                 // +0x004
    unsigned char _pad_008[0x554];
    SLongVector ExperienceItemLabelStat;           // +0x55C (PDB +0x578)
    SLongVector ExperienceItemLabelAbility;        // +0x568 (PDB +0x588)
    void AddRef() { ++refCount; }
    void Release() { if (--refCount == 0) Destroy(); }
};

struct CInventoryDefPointer {
    CInventoryDef* m_p;
    CInventoryDefPointer(const CInventoryDefPointer& other) : m_p(other.m_p) { if (m_p) m_p->AddRef(); }
    ~CInventoryDefPointer() { m_p->Release(); }
    CInventoryDef* operator->() const { return m_p; }
};

struct CTCInventoryExperience {
    unsigned char _pad_000[0x68];
    CInventoryDefPointer PDef;                     // +0x068
    CWideString GetInventoryText(long textId);     // 0x005BD273 (CTCInventoryBase)
    CWideString GetExperienceStatName(long stat);
};

CWideString CTCInventoryExperience::GetExperienceStatName(long stat)
{
    long textId;
    {
        CInventoryDefPointer def(PDef);
        textId = def->ExperienceItemLabelStat._M_start[stat];
    }
    if (textId)
        return GetInventoryText(textId);
    return g_EmptyWideString;
}