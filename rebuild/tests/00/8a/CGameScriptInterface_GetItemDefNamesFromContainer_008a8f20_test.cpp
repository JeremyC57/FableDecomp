// Behaviour test for 008a8f20: body copied from src/008a8f20.cpp (CGameScriptInterface::GetItemDefNamesFromContainer), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
inline void* operator new(unsigned int, void* where) { return where; }
static int g_strCopies = 0, g_strDtors = 0;
struct CCharString {
    void* m_pData;
    CCharString(void* p) : m_pData(p) { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_strCopies; }
    ~CCharString() { ++g_strDtors; }
};
struct __false_type { __false_type() {} };
static int g_erase = 0, g_overflow = 0;
struct CCharStringVector {
    CCharString* _M_start; CCharString* _M_finish; CCharString* _M_end_of_storage;
    CCharString* erase(CCharString* first, CCharString* last) { ++g_erase; CHECK(first == _M_start && last == _M_finish); _M_finish = _M_start; return first; }
    void clear() { erase(_M_start, _M_finish); }
    void _M_insert_overflow(CCharString* position, const CCharString& x, const __false_type&, unsigned long fill_len, bool atend)
    { ++g_overflow; CHECK(position == _M_finish && fill_len == 1 && atend); new (_M_finish) CCharString(x); ++_M_finish; ++_M_end_of_storage; }
    void push_back(const CCharString& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CCharString(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
struct CDefString { long m_index; CDefString(long i) : m_index(i) { } CDefString(const CDefString& other) : m_index(other.m_index) { } };
static int g_defName = 0, g_getString = 0;
struct CDefinitionManager_Retail { CDefString GetDefNameFromGlobalIndex(long index) { ++g_defName; return CDefString(index + 1000); } };
struct CDefStringTable_Retail { CCharString GetString(long index) { ++g_getString; return CCharString((void*)(index * 4)); } };
CDefStringTable_Retail g_defStringTable;
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    const SEntry* LowerBound(const long* type) const
    {
        const SEntry* it = first; int len = int(last - first);
        while (len > 0) { int half = len / 2; const SEntry* middle = it + half; if (middle->type < *type) { it = middle + 1; len = len - half - 1; } else len = half; }
        return it;
    }
};
struct CContainerItem { long DefIndex; long Data; };
struct CTCContainer {
    unsigned char _pad00[0xc];
    CContainerItem* ItemsStart; CContainerItem* ItemsFinish;
    unsigned long ItemCount() const { return ItemsFinish - ItemsStart; }
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    bool HasTC(long type) const { if (m_presentBits[type >> 5] & (1UL << (type & 31))) return true; return false; }
    template <class T> __forceinline const T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (const T*)it->component;
    }
};
static CThing g_thing; static bool g_alive = true; static int g_getThing = 0, g_isAlive = 0;
struct CScriptThing {
    virtual CThing* GetThing() const { ++g_getThing; return &g_thing; }
    virtual bool IsAlive() const { ++g_isAlive; return g_alive; }
    void* PImp[2];
};
struct CGameScriptInterface {
    virtual void GetItemDefNamesFromContainer(const CScriptThing& container, CCharStringVector& names) const;
    void* World; void* Component; void* DisplayEngine; CDefinitionManager_Retail* DefinitionManager;
};
void CGameScriptInterface::GetItemDefNamesFromContainer(const CScriptThing& container, CCharStringVector& names) const
{
    names.clear();
    if (!container.IsAlive())
        return;
    CThing* thing = container.GetThing();
    if (!thing->HasTC(0x3c))
        return;
    const CTCContainer* items = thing->GetTC<CTCContainer>(0x3c);
    for (unsigned long i = 0; i < items->ItemCount(); ++i) {
        CCharString name = g_defStringTable.GetString(DefinitionManager->GetDefNameFromGlobalIndex(items->ItemsStart[i].DefIndex).m_index);
        names.push_back(name);
    }
}
int main()
{
    CDefinitionManager_Retail manager; CGameScriptInterface gsi; gsi.DefinitionManager = &manager;
    CTCContainer container; CContainerItem items[3]; items[0].DefIndex = 1; items[1].DefIndex = 2; items[2].DefIndex = 3;
    container.ItemsStart = items; container.ItemsFinish = items + 3;
    CInterfaceMap::SEntry entries[3]; entries[0].type = 0x10; entries[1].type = 0x3c; entries[1].component = (CTCBase*)&container; entries[2].type = 0x50;
    memset(&g_thing, 0, sizeof g_thing); g_thing.m_interfaces.first = entries; g_thing.m_interfaces.last = entries + 3;
    g_thing.m_presentBits[0x3c >> 5] |= 1UL << (0x3c & 31);
    unsigned char storage[sizeof(CCharString) * 4]; memset(storage, 0, sizeof storage);
    CCharStringVector names; names._M_start = (CCharString*)storage; names._M_finish = names._M_start + 1; names._M_end_of_storage = names._M_start + 2;
    CScriptThing st;
    gsi.GetItemDefNamesFromContainer(st, names);
    CHECK(g_erase == 1 && g_isAlive == 1 && g_getThing == 1 && g_defName == 3 && g_getString == 3 && g_overflow == 1);
    CHECK(names._M_finish - names._M_start == 3);
    CHECK(names._M_start[0].m_pData == (void*)(1001 * 4) && names._M_start[2].m_pData == (void*)(1003 * 4));
    CHECK(g_strDtors >= 3 && g_strDtors <= 6 && g_strCopies >= 3);   // the GetString results and the name locals are destroyed; the vector copies stay
    g_thing.m_presentBits[1] = 0;                            // no container component: cleared only
    gsi.GetItemDefNamesFromContainer(st, names);
    CHECK(g_erase == 2 && g_getThing == 2 && g_defName == 3 && names._M_finish == names._M_start);
    g_alive = false;                                         // dead thing: cleared, GetThing not called
    gsi.GetItemDefNamesFromContainer(st, names);
    CHECK(g_erase == 3 && g_isAlive == 3 && g_getThing == 2);
    if (fails == 0) printf("008a8f20_TEST PASS\n"); else printf("008a8f20_TEST FAIL (%d)\n", fails);
    return fails;
}