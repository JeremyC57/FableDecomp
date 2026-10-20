// CTCHeroMorph::GetTattooNames(std::vector<CWideString>&) const -- appends the display name (inventory-item description text) of every tattoo the hero wears
// Grade: DIFFER(238v238) -- frame only: retail puts the GetText() string AND the `__false_type` tag of _M_insert_overflow in the dead `names` slot (the tag overlays the live string), ours gives the string its own word (sub esp,0Ch) -- the known tag-over-live-slot lane gap
// Leaf: GetTattooNames
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071CE80, 238 bytes (+ int3 padding), `ret 4`.
// Callees: CDefClassBase::GetSubDef<CInventoryItemDef>(CDefPointer<const CInventoryItemDef>&) @ 0x005BADE1 (the
//          template is instantiated in this TU: see the stand-in below), NGameText::CDataBank::GetPGroupEntry(ulong,
//          bool) const @ 0x009C9070 (on the main game component's text bank, +0x14), NGameText::CGroupEntry::GetText()
//          const @ 0x009C7A40, CWideString::CWideString(const CWideString&) @ 0x0099B720 (push_back's placement copy),
//          vector<CWideString>::_M_insert_overflow(pos, x, __false_type, n, atEnd) @ 0x00412330, ~CWideString
//          @ 0x0099B510; the defs' / entries' slot 1 on release. Global g_pMainGameComponent @ 0x013B86A0.
// Shape: per tattoo the item def is read once from the zeroed out slot into ebx (kept: the GetSubDef body is known),
// the group entry is a counted temporary in the frame (null-tested, released through the ref-count base's inline
// accessors and nulled), the text a CWideString in the dead `names` slot pushed with the STLport inline push_back
// (null-checked placement copy and `add [edi+4],4`, else _M_insert_overflow with the `__false_type` tag); the item
// def is released without a null test.
// Layout: retail CTCHeroMorph Tattoos vector<CDefPointer<const CThingBaseDef>> at +0x1C (PDB +0x2C); CInventoryItemDef
// ItemDescription +0x34.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
};

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                          // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    void Release() { if (--m_count == 0) Destroy(); }
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
struct CDefPointerBase {                             // CDefPointer<CDefBase>: the manager's return type
    CDefBase* m_p;
    CDefPointerBase() { }
    ~CDefPointerBase() { if (m_p) { m_p->ReleaseTemporary(); m_p = 0; } }
};
struct CInventoryItemDef : CDefBase {
    unsigned char _pad_0x08[0x2C];
    unsigned long ItemDescription;                   // +0x34
};
struct CItemDefPointer {                             // CDefPointer<const CInventoryItemDef>
    CInventoryItemDef* m_p;
    CItemDefPointer() : m_p(0) { }
    CItemDefPointer(const CDefPointerBase& other) : m_p((CInventoryItemDef*)other.m_p) { if (m_p) ++m_p->m_count; }
    ~CItemDefPointer() { if (m_p) m_p->Release(); }
    CInventoryItemDef* operator->() const { return m_p; }
    CItemDefPointer& operator=(const CItemDefPointer& other)
    {
        if (m_p != other.m_p) {
            if (m_p) { m_p->Release(); m_p = 0; }
            m_p = other.m_p;
            if (m_p) ++m_p->m_count;
        }
        return *this;
    }
};
struct CDefinitionManager {
    CDefPointerBase GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
struct CDefClassBase : CDefBase {
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    unsigned char _pad[0x1c - 8];
    CDefinitionManager* m_pManager;                  // +0x1C
    bool GetSubDef(CItemDefPointer& out);            // 0x005BADE1
};

// Stand-in body of GetSubDef<CInventoryItemDef> (0x005BADE1, the landed 0x0077E430 shape): compiled above the
// caller with auto_inline off so VC7.1 knows the out pointer does not escape. Only GetTattooNames is graded.
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CItemDefPointer& out)
{
    long* pIndex;
    {
        CCharString name("CInventoryItemDef", -1);
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CItemDefPointer result(m_pManager->GetPDefFromGlobalIndex(*pIndex));
        out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)

struct CWideString {
    void* PStringData;
    CWideString(const CWideString& other);           // 0x0099B720
    ~CWideString();                                  // 0x0099B510
};
inline void* operator new(unsigned int, void* where) { return where; }

struct __false_type { __false_type() {} };
struct CWideStringVector {                           // retail STLport vector<CWideString> (12 bytes)
    CWideString* _M_start;
    CWideString* _M_finish;
    CWideString* _M_end_of_storage;
    void _M_insert_overflow(CWideString* pos, const CWideString& x, const __false_type& tag,
                            unsigned long fillLength, bool atEnd);   // 0x00412330
    void push_back(const CWideString& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                new (_M_finish) CWideString(x);
            ++_M_finish;
        } else {
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
        }
    }
};

namespace NGameText {
struct CGroupEntry : CDefVirtual, CRefCounted {
    CWideString GetText() const;                     // 0x009C7A40
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
}
template <class T> struct CIVCountedPointer {
    T* Data;
    ~CIVCountedPointer() { if (Data) { Data->ReleaseTemporary(); Data = 0; } }
    T* operator->() const { return Data; }
};
namespace NGameText {
class CDataBank {
public:
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(unsigned long tag, bool load) const;   // 0x009C9070
};
}
struct CMainGameComponent {
    unsigned char _pad00[0x14];
    NGameText::CDataBank* PGameTextDataBank;         // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;     // 0x013B86A0

struct CThingBaseDefPointer {                        // CDefPointer<const CThingBaseDef>
    CDefClassBase* Object;
    CDefClassBase* operator->() const { return Object; }
};
struct CTattooVector {                               // retail STLport vector (12 bytes)
    CThingBaseDefPointer* _M_start;
    CThingBaseDefPointer* _M_finish;
    CThingBaseDefPointer* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const CThingBaseDefPointer& operator[](unsigned long n) const { return _M_start[n]; }
};

struct CTCHeroMorph {
    void*         __vftable;                         // +0x00
    unsigned char _pad_0x04[0x18];
    CTattooVector Tattoos;                           // +0x1C (PDB +0x2C)
    void GetTattooNames(CWideStringVector& names) const;
};

void CTCHeroMorph::GetTattooNames(CWideStringVector& names) const
{
    for (unsigned long i = 0; i < Tattoos.size(); ++i) {
        CItemDefPointer item;
        Tattoos[i]->GetSubDef(item);
        CInventoryItemDef* itemDef = item.m_p;
        CIVCountedPointer<NGameText::CGroupEntry> entry =
            g_pMainGameComponent->PGameTextDataBank->GetPGroupEntry(itemDef->ItemDescription, true);
        if (entry.Data) {
            CWideString text = entry->GetText();
            names.push_back(text);
        }
    }
}
