// CTCInventoryQuests::vfunc_0() -- slot 0 of the screen's NHeroInformationScreens::CBase secondary vtable: the inventory text for the def's title index, returned by value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0061A220, 57 bytes (+7 int3). rtti-vtables: CTCInventoryQuests vtable 0x012585A0 slot 0.
// Callee: CTCInventoryBase::GetInventoryText(long) @ 0x005BD273 (CWideString by hidden pointer); def slot 1 (+4) on release.
//
// Shape: the override of a virtual first declared in the SECONDARY base, so `this` arrives as the CBase sub-object at
// +0x130 and every primary member is a negative offset (`mov esi,[ecx-0C8h]` = PDef at +0x68, `add ecx,-130h`
// before the base call). The def pointer is copied into a LOCAL CDefPointer: the copy constructor's null-tested
// `inc [esi+4]`, the destructor's untested `dec [esi+4] ; jne ; call [edx+4]` after the call (a release on the
// object: `if (--m_refCount == 0) Destroy();`, GOTCHAS 24f/24b). The hidden result pointer (edi) is handed straight to
// GetInventoryText and returned (`mov eax,edi ; ret 4`): a by-value CWideString with a user copy constructor and
// destructor. PDef is CTCInventoryBase::PDef (PDB +0x70, retail +0x68: the two retail vectors before it are 12 bytes,
// not 16). Local model: no engine header for CTCInventoryBase / CTCInventoryQuests / CInventoryDef; the def word at +0x1EC is named
// from use. The method's real name is unknown (catalog label kept).
struct CWideString {
    void* m_pData;
    CWideString(const CWideString& other);
    ~CWideString();
};

struct CInventoryDef {
    virtual void v00();
    virtual void Destroy();                   // slot 1 (+4)
    long m_refCount;                          // +0x04
    unsigned char _pad_0x08[0x1ec - 8];
    long m_titleTextIndex;                    // +0x1EC (name from use)
    void AddRef() { ++m_refCount; }
    void Release() { if (--m_refCount == 0) Destroy(); }
};

struct CDefPointer {                          // CDefPointer<const CInventoryDef>
    CInventoryDef* m_pDef;
    CDefPointer(const CDefPointer& other) : m_pDef(other.m_pDef) { if (m_pDef) m_pDef->AddRef(); }
    ~CDefPointer() { m_pDef->Release(); }
    CInventoryDef* operator->() const { return m_pDef; }
};

struct CTCInventoryBase_Retail {              // primary base, retail size 0x130
    virtual void v00();
    unsigned char _pad_0x04[0x68 - 4];
    CDefPointer PDef;                         // +0x68
    unsigned char _pad_0x6c[0x130 - 0x6c];
    CWideString GetInventoryText(long index); // 0x005BD273
};

namespace NHeroInformationScreens {
struct CBase {                                // secondary base at +0x130
    virtual CWideString vfunc_0();            // slot 0
};
}

struct CTCInventoryQuests : CTCInventoryBase_Retail, NHeroInformationScreens::CBase {
    virtual CWideString vfunc_0();
};

CWideString CTCInventoryQuests::vfunc_0()
{
    CDefPointer def = PDef;
    return GetInventoryText(def->m_titleTextIndex);
}
