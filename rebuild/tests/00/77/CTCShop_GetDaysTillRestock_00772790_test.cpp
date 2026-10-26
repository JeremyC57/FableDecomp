// Behaviour test for 00772790: src/00772790.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCShop::GetDaysTillRestock(long)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCShop::GetDaysTillRestock(long) -- days until the given stock entry restocks: looks the key up in the restock map (+0x34) and returns `interval - day % interval` (0 when the key is unknown or its interval is 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00772790, length 80 bytes.
// Callees: the restock map's find @ 0x00777170 (VC6-era tree: head pointer at +0 of the map, key at node +0x10 compared
// signed, iterator returned through the hidden pointer, `ret 8`), CTCBase::GetWorld @ 0x00686A70 (the manifest label
// "CViewBase::GetDisplayEngine" is propagated); the world's game-time manager (+0x1c, engine/CWorld.h
// PGameTimeManager_ptr) holds the day counter at +0x14.
// Shape: the iterator lives in the `push ecx` slot (a 4-byte class with a user constructor, returned by hidden pointer);
// the key's own argument slot is passed by address (`const long&`); end() is the map's head word (`cmp ebx,[edi]`);
// the interval is re-read for the divide; `idiv` remainder subtracted from the interval.
class CGameTimeManager { public:
    unsigned char _pad[0x14];
    long          m_day;                     // +0x14
};

class CWorld { public:
    unsigned char     _pad[0x1c];
    CGameTimeManager* m_pGameTimeManager;    // +0x1c
};

struct SRestockNode {
    unsigned char _pad[0x10];
    long          m_key;                     // +0x10
    unsigned char _pad14[0xc];
    long          m_restockInterval;         // +0x20 (days)
};

struct CRestockMap {
    SRestockNode* m_pHead;                   // +0 (end() sentinel)
    struct iterator {
        SRestockNode* m_pNode;
        iterator() { }
        iterator(SRestockNode* node) : m_pNode(node) { }
    };
    iterator find(const long& key);          // 0x00777170
    iterator end() { return iterator(m_pHead); }
};
// Same-TU body of the STLport map::find instantiation 0x00777170: lower_bound walk from the root (header +4; left +8,
// right +0xc) reading the key through its const reference, then the end() test.
#pragma auto_inline(off)
__declspec(noinline) CRestockMap::iterator CRestockMap::find(const long& key)
{
    SRestockNode* y = m_pHead;
    SRestockNode* x = *(SRestockNode**)((unsigned char*)m_pHead + 4);
    while (x != 0) {
        if (!(x->m_key < key)) {
            y = x;
            x = *(SRestockNode**)((unsigned char*)x + 8);
        } else
            x = *(SRestockNode**)((unsigned char*)x + 0xc);
    }
    if (y == m_pHead || key < y->m_key)
        return iterator(m_pHead);
    return iterator(y);
}
#pragma auto_inline(on)

class CTCShop { public:
    unsigned char _pad[0x34];
    CRestockMap   m_restock;                 // +0x34
    CWorld* GetWorld();                      // 0x00686A70 (CTCBase)
    long GetDaysTillRestock(long key);
};

long CTCShop::GetDaysTillRestock(long key)
{
    CRestockMap::iterator it = m_restock.find(key);
    SRestockNode* node = it.m_pNode;
    if (node != m_restock.m_pHead && node->m_restockInterval != 0) {
        long day = GetWorld()->m_pGameTimeManager->m_day;
        return node->m_restockInterval - day % node->m_restockInterval;
    }
    return 0;
}

// ---- stubs and driver ----
static int g_world = 0; static CGameTimeManager g_time; static CWorld g_worldObj;
CWorld* CTCShop::GetWorld() { ++g_world; return &g_worldObj; }
static SRestockNode* mk(SRestockNode* n, long key, long interval, SRestockNode* l, SRestockNode* r)
{ memset(n, 0, sizeof(*n)); n->m_key = key; n->m_restockInterval = interval; *(SRestockNode**)((unsigned char*)n + 8) = l; *(SRestockNode**)((unsigned char*)n + 0xc) = r; return n; }
int main()
{
    g_worldObj.m_pGameTimeManager = &g_time; g_time.m_day = 23;
    static SRestockNode header, a, b, c;
    mk(&a, 1, 7, 0, 0); mk(&c, 9, 0, 0, 0); mk(&b, 5, 10, &a, &c); memset(&header, 0, sizeof(header)); *(SRestockNode**)((unsigned char*)&header + 4) = &b;
    static CTCShop shop; shop.m_restock.m_pHead = &header;
    CHECK(shop.GetDaysTillRestock(1) == 7 - 23 % 7); CHECK(shop.GetDaysTillRestock(5) == 10 - 23 % 10); CHECK(g_world == 2);
    CHECK(shop.GetDaysTillRestock(9) == 0 && g_world == 2);   // zero interval
    CHECK(shop.GetDaysTillRestock(4) == 0 && shop.GetDaysTillRestock(100) == 0 && g_world == 2);   // unknown keys
    if (fails == 0) printf("00772790_TEST PASS\n"); else printf("00772790_TEST FAIL\n");
    return fails;
}