// CTCShop::GetDaysTillRestock(long) -- days until the given stock entry restocks: looks the key up in the restock map (+0x34) and returns `interval - day % interval` (0 when the key is unknown or its interval is 0)
// Grade: DIFFER(78v80) -- retail keeps `this` in esi and the node in ebx read back from the iterator's frame slot (`mov ebx,[esp+0Ch]`, push ebx in the prologue); ours colours this=ebx, node=esi loaded through the returned pointer (`mov esi,[eax]`). Tried: plain expression (76), `day` local (81), node local from the call (78, best), named iterator + node local (77: the node becomes `lea edi,[eax+20h]`).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00772790, length 80 bytes.
// Callees: the restock map's find @ 0x00777170 (VC6-era tree: head pointer at +0 of the map, key at node +0x10 compared
// signed, iterator returned through the hidden pointer, `ret 8`), CTCBase::GetWorld @ 0x00686A70 (the manifest label
// "CViewBase::GetDisplayEngine" is propagated); the world's game-time manager (+0x1c, engine/CWorld.h
// PGameTimeManager_ptr) holds the day counter at +0x14.
// Shape: the iterator lives in the `push ecx` slot (a 4-byte class with a user constructor, returned by hidden pointer);
// the key's own argument slot is passed by address (`const long&`); end() is the map's head word (`cmp ebx,[edi]`);
// the interval is re-read for the divide; `idiv` remainder subtracted from the interval.
struct CGameTimeManager {
    unsigned char _pad[0x14];
    long          m_day;                     // +0x14
};

struct CWorld {
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

struct CTCShop {
    unsigned char _pad[0x34];
    CRestockMap   m_restock;                 // +0x34
    CWorld* GetWorld();                      // 0x00686A70 (CTCBase)
    long GetDaysTillRestock(long key);
};

long CTCShop::GetDaysTillRestock(long key)
{
    SRestockNode* node = m_restock.find(key).m_pNode;
    if (node != m_restock.m_pHead && node->m_restockInterval != 0) {
        long day = GetWorld()->m_pGameTimeManager->m_day;
        return node->m_restockInterval - day % node->m_restockInterval;
    }
    return 0;
}
