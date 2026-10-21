// Behaviour test for 00b41db0: src/00b41db0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CEngineMap::SetMapVisibility(list<CEngineMap*>& visibleMaps, bool visible)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CEngineMap::SetMapVisibility(list<CEngineMap*>& visibleMaps, bool visible) -- links the map at the front of the manager's visible-map list (or unlinks it), remembers its list position and visibility, then tells its landscape and local-detail parts
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B41DB0, length 145 bytes (160 with padding).
// Callees: malloc @ 0x00BFEA0E (the 12-byte list node), free @ 0x00BFEA14, CEngineLandscapeMap::OnMapVisibilityChanged(bool)
// @ 0x00BDC3C0 (on the map's landscape part at +0), CLocalDetailCacheMap::OnMapVisibilityChanged(bool) @ 0x00BDE160 (on its
// local-detail part at +4). Caller: CEngineMapManager::SetMapVisibility @ 0x00B42390 (landed) passes &m_visibleMaps.
// Shape: the list is the retail STLport-style sentinel-only list (node {next, prev, value}, no size word) with malloc/free
// nodes: insert(begin(), this) builds the node and placement-constructs the value (null-tested `lea ecx,[eax+8]`), then
// links it before `where` (prev read after the construction); erase(pos) unlinks next/prev and frees the node. On becoming
// visible the stored position is begin() re-read from the list, on becoming hidden it is end() (the head). Both part calls
// are null-tested and take the new visibility; `ret 8`.
#include <new>

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14

template<class T> struct CRetailList {              // STLport-style list: the sentinel pointer only, node = {next, prev, value}
    struct node { node* next; node* prev; T value; };
    node* head;
    struct iterator {
        node* p;
        iterator() { }
        explicit iterator(node* q) : p(q) { }
        iterator(const iterator& o) : p(o.p) { }
    };
    iterator begin() { return iterator(head->next); }
    iterator end() { return iterator(head); }
    node* create_node(const T& value)
    {
        node* n = (node*)malloc(sizeof(node));
        new (&n->value) T(value);
        return n;
    }
    iterator insert(iterator where, const T& value)
    {
        node* n = create_node(value);
        node* at = where.p;
        node* prev = at->prev;
        n->next = at;
        n->prev = prev;
        prev->next = n;
        at->prev = n;
        return iterator(n);
    }
    iterator erase(iterator where)
    {
        node* n = where.p;
        node* next = n->next;
        node* prev = n->prev;
        prev->next = next;
        next->prev = prev;
        free(n);
        return iterator(next);
    }
};

struct CEngineLandscapeMap {
    void OnMapVisibilityChanged(bool visible);      // 0x00BDC3C0
};

struct CLocalDetailCacheMap {
    void OnMapVisibilityChanged(bool visible);      // 0x00BDE160
};

struct CEngineMap {
    CEngineLandscapeMap*  m_pLandscape;             // +0x00
    CLocalDetailCacheMap* m_pLocalDetailCache;      // +0x04
    unsigned char         _pad08[0x50];
    CRetailList<CEngineMap*>::iterator m_visibleMapsPos;   // +0x58
    unsigned char         _pad5c[0x8];
    bool                  m_visible;                // +0x64
    void SetMapVisibility(CRetailList<CEngineMap*>& visibleMaps, bool visible);
};

void CEngineMap::SetMapVisibility(CRetailList<CEngineMap*>& visibleMaps, bool visible)
{
    if (visible) {
        if (!m_visible) {
            visibleMaps.insert(visibleMaps.begin(), this);
            m_visibleMapsPos = visibleMaps.begin();
            m_visible = true;
        }
    }
    else if (m_visible) {
        visibleMaps.erase(m_visibleMapsPos);
        m_visibleMapsPos = visibleMaps.end();
        m_visible = false;
    }
    if (m_pLandscape)
        m_pLandscape->OnMapVisibilityChanged(visible);
    if (m_pLocalDetailCache)
        m_pLocalDetailCache->OnMapVisibilityChanged(visible);
}

// ---- stubs and driver ----

static int g_land = 0, g_cache = 0; static bool g_landVis, g_cacheVis;
void CEngineLandscapeMap::OnMapVisibilityChanged(bool visible) { ++g_land; g_landVis = visible; }
void CLocalDetailCacheMap::OnMapVisibilityChanged(bool visible) { ++g_cache; g_cacheVis = visible; }
typedef CRetailList<CEngineMap*> TList;
static int Count(TList::node* h) { int c = 0; for (TList::node* n = h->next; n != h; n = n->next) ++c; return c; }
int main()
{
    TList::node head; head.next = &head; head.prev = &head;
    TList maps; maps.head = &head;
    CEngineLandscapeMap land; CLocalDetailCacheMap cache;
    CEngineMap a; memset(&a, 0, sizeof a); a.m_pLandscape = &land; a.m_pLocalDetailCache = &cache;
    CEngineMap b; memset(&b, 0, sizeof b);
    a.SetMapVisibility(maps, true);
    CHECK(a.m_visible); CHECK(Count(&head) == 1); CHECK(head.next->value == &a); CHECK(a.m_visibleMapsPos.p == head.next);
    CHECK(g_land == 1 && g_landVis); CHECK(g_cache == 1 && g_cacheVis);
    b.SetMapVisibility(maps, true);
    CHECK(Count(&head) == 2); CHECK(head.next->value == &b); CHECK(head.next->next->value == &a); CHECK(g_land == 1);
    a.SetMapVisibility(maps, true);                 // already visible: no insert
    CHECK(Count(&head) == 2); CHECK(g_land == 2);
    a.SetMapVisibility(maps, false);
    CHECK(!a.m_visible); CHECK(Count(&head) == 1); CHECK(head.next->value == &b); CHECK(a.m_visibleMapsPos.p == &head);
    CHECK(g_land == 3 && !g_landVis); CHECK(g_cache == 3 && !g_cacheVis);
    a.SetMapVisibility(maps, false);                // already hidden
    CHECK(Count(&head) == 1); CHECK(g_land == 4);
    if (fails == 0) printf("00b41db0_TEST PASS\n"); else printf("00b41db0_TEST FAIL\n");
    return fails;
}