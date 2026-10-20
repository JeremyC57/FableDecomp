// Behaviour test for CTCInventoryBase::GetInventoryText @ 0x005BD273 (model copy; stubbed text bank).
#include <cstdio>
struct CWideString {
    const unsigned short* PStringData;
    CWideString() : PStringData(0) {}
    CWideString(const CWideString& other) : PStringData(other.PStringData) {}
};
static const unsigned short g_emptyChars[1] = { 0 };
static CWideString g_emptyWideString;
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags) {}
    virtual void Delete() { ++deletes; }
    unsigned long IVRefCount;
    static long deletes;
    CIVCountedPointeeBase() : IVRefCount(0) {}
};
long CIVCountedPointeeBase::deletes = 0;
template <class T> struct CIVCountedPointer {
    T* Object;
    CIVCountedPointer(T* p) : Object(p) { if (p) ++p->IVRefCount; }
    CIVCountedPointer(const CIVCountedPointer& o) : Object(o.Object) { if (Object) ++Object->IVRefCount; }
    ~CIVCountedPointer()
    {
        T* old = Object;
        if (old) {
            T* object = old;
            --old->IVRefCount;
            if (old->IVRefCount == 0)
                object->Delete();
        }
    }
};
struct CDataBankEntry { int type; };
static const unsigned short g_helloChars[6] = { 'h', 'e', 'l', 'l', 'o', 0 };
struct CGroupEntry : CIVCountedPointeeBase {
    CWideString GetText() const { CWideString s; s.PStringData = g_helloChars; return s; }
};
struct CDataBank {
    CDataBankEntry entry;
    CGroupEntry group;
    long lookups;
    CDataBank() : lookups(0) {}
    CDataBankEntry* GetTextInfo(unsigned long index) const { return index == 7 || index == 9 ? const_cast<CDataBankEntry*>(&entry) : 0; }
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(unsigned long index, bool load) const
    {
        const_cast<CDataBank*>(this)->lookups++;
        return CIVCountedPointer<CGroupEntry>(index == 7 && load ? const_cast<CGroupEntry*>(&group) : 0);
    }
};
struct CMainGameComponent {
    unsigned char _pad00[0x14];
    CDataBank*    TextBank;
};
static CMainGameComponent g_mainGame;
static CMainGameComponent* g_pMainGameComponent = &g_mainGame;

struct CTCInventoryBase {
    CWideString GetInventoryText(long textIndex);
};
CWideString CTCInventoryBase::GetInventoryText(long textIndex)
{
    if (textIndex > 0) {
        CDataBank* bank = g_pMainGameComponent->TextBank;
        bool found = bank->GetTextInfo(textIndex) != 0 && bank->GetPGroupEntry(textIndex, true).Object != 0;
        if (found)
            return bank->GetPGroupEntry(textIndex, true).Object->GetText();
    }
    return g_emptyWideString;
}
int main()
{
    CDataBank bank;
    g_mainGame.TextBank = &bank;
    g_emptyWideString.PStringData = g_emptyChars;
    CTCInventoryBase inv;
    bool ok = true;
    CWideString a = inv.GetInventoryText(7);                     // text entry with a group entry
    if (a.PStringData != g_helloChars || bank.lookups != 2) ok = false;
    if (bank.group.IVRefCount != 0) ok = false;                  // both temporaries released
    CWideString b = inv.GetInventoryText(9);                     // text entry, no group entry -> empty
    if (b.PStringData != g_emptyChars || bank.lookups != 3) ok = false;
    CWideString c = inv.GetInventoryText(3);                     // not a text entry -> no group lookup
    if (c.PStringData != g_emptyChars || bank.lookups != 3) ok = false;
    CWideString d = inv.GetInventoryText(0);                     // non-positive index -> no bank access
    CWideString e = inv.GetInventoryText(-5);
    if (d.PStringData != g_emptyChars || e.PStringData != g_emptyChars || bank.lookups != 3) ok = false;
    if (!ok) { std::printf("005bd273_TEST FAIL\n"); return 1; }
    std::printf("005bd273_TEST PASS\n");
    return 0;
}