// Behaviour test for 008ed1f0: body copied from src/008ed1f0.cpp (CTCContainer::InsertItem), callees stubbed.
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_strCopies = 0, g_strDtors = 0;
class CCharString { public:
    void* PStringData;
    CCharString(void* p) : PStringData(p) { }
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_strCopies; }
    ~CCharString() { ++g_strDtors; }
};
struct SContainerItem {
    long        first;
    CCharString second;
    SContainerItem(const long& a, const CCharString& b) : first(a), second(b) { }
    SContainerItem(const SContainerItem& other) : first(other.first), second(other.second) { }
};
struct __false_type { __false_type() { } };
static int g_overflow = 0;
struct CContainerItemVector {
    SContainerItem* _M_start;
    SContainerItem* _M_finish;
    SContainerItem* _M_end_of_storage;
    void _M_insert_overflow(SContainerItem* position, const SContainerItem& x, const __false_type&, unsigned long fill_len, bool atend)
    {
        ++g_overflow; CHECK(position == _M_finish && fill_len == 1 && atend);
        static unsigned char storage[8 * sizeof(SContainerItem)];
        SContainerItem* ns = (SContainerItem*)storage; long n = _M_finish - _M_start;
        for (long i = 0; i < n; ++i) new (ns + i) SContainerItem(_M_start[i]);
        new (ns + n) SContainerItem(x);
        _M_start = ns; _M_finish = ns + n + 1; _M_end_of_storage = ns + 8;
    }
    void push_back(const SContainerItem& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) SContainerItem(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
static int g_deleted = 0;
struct CThingDef {
    virtual void v00() {}
    virtual void Delete() { ++g_deleted; }
    long m_refCount;
    unsigned char _pad08[0x42 - 0x08];
    unsigned char Flags42;
    bool PersistsGameState() const { return (Flags42 & 2) != 0; }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
template <class T> struct CDefHolder {
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder() { if (Object && --Object->m_refCount == 0) Object->Delete(); }
    T* operator->() const { return Object; }
};
static CThingDef* g_def = 0; static int g_getDef = 0; static long g_askedIndex = -1;
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CThingDef>* out) const
    {
        ++g_getDef; g_askedIndex = index;
        if (!g_def) return false;
        ++g_def->m_refCount; out->Object = g_def; return true;
    }
};
static CDefinitionManager g_manager;
static bool g_persisted = false; static int g_setPersisted = 0;
class CThing { public:
    bool IsGameStatePersisted() const { return g_persisted; }
    void SetAsGameStatePersisted(bool persisted) { ++g_setPersisted; g_persisted = persisted; }
};
class CTCContainer { public:
    virtual void v00() {}
    CThing* ParentThing;
    unsigned char _pad08[0x0c - 0x08];
    CContainerItemVector ContainerItems;
    const CDefinitionManager* GetDefinitionManager() const { return &g_manager; }
    CThingDef* GetItemDef(long defIndex) const
    {
        CDefPointer<CThingDef> out;
        GetDefinitionManager()->GetDef(defIndex, &out);
        return out.Object;
    }
    void InsertItem(long defIndex, const CCharString& name);
};

void CTCContainer::InsertItem(long defIndex, const CCharString& name)
{
    ContainerItems.push_back(SContainerItem(defIndex, name));
    if (!ParentThing->IsGameStatePersisted()) {
        CDefHolder<CThingDef> def(GetItemDef(defIndex));
        if (def->PersistsGameState())
            ParentThing->SetAsGameStatePersisted(true);
    }
}
int main()
{
    static CThing thing; static CThingDef def; def.m_refCount = 1; def.Flags42 = 0; g_def = &def;
    static unsigned char storage[2 * sizeof(SContainerItem)];
    CTCContainer c; c.ParentThing = &thing; c.ContainerItems._M_start = c.ContainerItems._M_finish = (SContainerItem*)storage; c.ContainerItems._M_end_of_storage = (SContainerItem*)storage + 2;
    CCharString name((void*)0x1111);
    // room available, thing not persisted, def does not ask for persistence
    c.InsertItem(7, name);
    CHECK(c.ContainerItems._M_finish - c.ContainerItems._M_start == 1); CHECK(c.ContainerItems._M_start[0].first == 7 && c.ContainerItems._M_start[0].second.PStringData == (void*)0x1111);
    CHECK(g_getDef == 1 && g_askedIndex == 7 && g_setPersisted == 0); CHECK(def.m_refCount == 1 && g_deleted == 0);
    // def asks for persistence: the thing starts persisting
    def.Flags42 = 2; CCharString name2((void*)0x2222); c.InsertItem(9, name2);
    CHECK(c.ContainerItems._M_finish - c.ContainerItems._M_start == 2 && c.ContainerItems._M_start[1].first == 9); CHECK(g_setPersisted == 1 && g_persisted); CHECK(def.m_refCount == 1);
    // already persisted: no def lookup; vector full, so the overflow path takes the insert
    c.InsertItem(11, name);
    CHECK(g_overflow == 1 && g_getDef == 2); CHECK(c.ContainerItems._M_finish - c.ContainerItems._M_start == 3 && c.ContainerItems._M_start[2].first == 11 && c.ContainerItems._M_start[2].second.PStringData == (void*)0x1111);
    // a def whose only reference is the lookup is deleted by the holder
    g_persisted = false; def.m_refCount = 0; c.InsertItem(13, name); CHECK(g_deleted == 1 && g_setPersisted == 2);
    CHECK(g_strCopies > 0 && g_strDtors > 0);
    if (fails == 0) printf("008ed1f0_TEST PASS\n"); else printf("008ed1f0_TEST FAIL\n");
    return fails;
}