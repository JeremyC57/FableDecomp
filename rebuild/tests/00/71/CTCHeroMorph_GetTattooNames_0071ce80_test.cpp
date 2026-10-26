// Behaviour test for 0071ce80: body copied from src/0071ce80.cpp (CTCHeroMorph::GetTattooNames), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_defDestroyed = 0, g_entryDestroyed = 0;
struct CRefCounted { long m_count; void DecRef() { --m_count; } long GetRefCount() const { return m_count; } };
struct CDefVirtual { virtual void Destroy() = 0; };
struct CDefBase : CDefVirtual, CRefCounted {
    virtual void Destroy() { ++g_defDestroyed; }
    void Release() { if (--m_count == 0) Destroy(); }
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
class CInventoryItemDef : public CDefBase { public: unsigned char _pad_0x08[0x2C]; unsigned long ItemDescription; };
struct CItemDefPointer {
    CInventoryItemDef* m_p;
    CItemDefPointer() : m_p(0) { }
    ~CItemDefPointer() { if (m_p) m_p->Release(); }
    CInventoryItemDef* operator->() const { return m_p; }
};
static CInventoryItemDef g_items[3]; static int g_getSubDef = 0;
class CDefClassBase : public CDefBase { public:
    CInventoryItemDef* m_item;
    bool GetSubDef(CItemDefPointer& out) { ++g_getSubDef; out.m_p = m_item; if (m_item) { ++m_item->m_count; return true; } return false; }
};
class CWideString { public:
    void* PStringData;
    CWideString(void* p) : PStringData(p) { }
    CWideString(const CWideString& other) : PStringData(other.PStringData) { }
    ~CWideString() { PStringData = 0; }
};
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() {} };
static int g_overflow = 0;
struct CWideStringVector {
    CWideString* _M_start; CWideString* _M_finish; CWideString* _M_end_of_storage;
    void _M_insert_overflow(CWideString* pos, const CWideString& x, const __false_type& tag, unsigned long fillLength, bool atEnd)
    { ++g_overflow; CHECK(pos == _M_finish && fillLength == 1 && atEnd); new (_M_finish) CWideString(x); ++_M_finish; ++_M_end_of_storage; }
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
class CGroupEntry : public CDefVirtual, public CRefCounted { public:
    virtual void Destroy() { ++g_entryDestroyed; }
    unsigned long m_tag;
    CWideString GetText() const { return CWideString((void*)(m_tag * 16)); }
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
}
template <class T> class CIVCountedPointer { public:
    T* Data;
    CIVCountedPointer() : Data(0) { }
    CIVCountedPointer(const CIVCountedPointer& o) : Data(o.Data) { if (Data) ++Data->m_count; }
    ~CIVCountedPointer() { if (Data) { Data->ReleaseTemporary(); Data = 0; } }
    T* operator->() const { return Data; }
};
static NGameText::CGroupEntry g_entries[2]; static int g_getEntry = 0; static unsigned long g_lastTag = 0;
namespace NGameText {
class CDataBank {
public:
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(unsigned long tag, bool load) const
    {
        ++g_getEntry; g_lastTag = tag; CHECK(load);
        CIVCountedPointer<CGroupEntry> r; r.Data = 0;
        if (tag == 0x100) { r.Data = &g_entries[0]; ++g_entries[0].m_count; }
        if (tag == 0x200) { r.Data = &g_entries[1]; ++g_entries[1].m_count; }
        return r;
    }
};
}
class CMainGameComponent { public: unsigned char _pad00[0x14]; NGameText::CDataBank* PGameTextDataBank; };
static CMainGameComponent g_main; static NGameText::CDataBank g_bank;
CMainGameComponent* g_pMainGameComponent = &g_main;
struct CThingBaseDefPointer { CDefClassBase* Object; CDefClassBase* operator->() const { return Object; } };
struct CTattooVector {
    CThingBaseDefPointer* _M_start; CThingBaseDefPointer* _M_finish; CThingBaseDefPointer* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const CThingBaseDefPointer& operator[](unsigned long n) const { return _M_start[n]; }
};
class CTCHeroMorph { public:
    void* __vftable; unsigned char _pad_0x04[0x18]; CTattooVector Tattoos;
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
int main()
{
    g_main.PGameTextDataBank = &g_bank;
    for (int i = 0; i < 3; ++i) g_items[i].m_count = 1;
    g_items[0].ItemDescription = 0x100; g_items[1].ItemDescription = 0x999; g_items[2].ItemDescription = 0x200;
    g_entries[0].m_count = 1; g_entries[0].m_tag = 0x100; g_entries[1].m_count = 1; g_entries[1].m_tag = 0x200;
    CDefClassBase classes[3]; CThingBaseDefPointer tattoos[3];
    for (int i = 0; i < 3; ++i) { classes[i].m_count = 1; classes[i].m_item = &g_items[i]; tattoos[i].Object = &classes[i]; }
    CTCHeroMorph morph; memset(&morph, 0, sizeof morph);
    morph.Tattoos._M_start = tattoos; morph.Tattoos._M_finish = tattoos + 3; morph.Tattoos._M_end_of_storage = tattoos + 3;
    unsigned char storage[sizeof(CWideString) * 4]; memset(storage, 0, sizeof storage);
    CWideStringVector names; names._M_start = (CWideString*)storage; names._M_finish = names._M_start; names._M_end_of_storage = names._M_start + 1;
    morph.GetTattooNames(names);
    CHECK(g_getSubDef == 3 && g_getEntry == 3 && g_lastTag == 0x200);
    CHECK(names._M_finish - names._M_start == 2 && g_overflow == 1);      // the unknown description (0x999) adds nothing
    CHECK(names._M_start[0].PStringData == (void*)(0x100 * 16) && names._M_start[1].PStringData == (void*)(0x200 * 16));
    CHECK(g_items[0].m_count == 1 && g_items[1].m_count == 1 && g_items[2].m_count == 1 && g_defDestroyed == 0);   // item defs released back
    CHECK(g_entries[0].m_count == 1 && g_entries[1].m_count == 1 && g_entryDestroyed == 0);                       // entries released back
    CWideStringVector none = names; morph.Tattoos._M_finish = tattoos;                                             // empty tattoo list: nothing happens
    morph.GetTattooNames(none);
    CHECK(g_getSubDef == 3 && none._M_finish == names._M_finish);
    if (fails == 0) printf("0071ce80_TEST PASS\n"); else printf("0071ce80_TEST FAIL (%d)\n", fails);
    return fails;
}