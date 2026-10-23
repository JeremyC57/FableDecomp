#include <cstdio>
#include <cstring>
// Behaviour test for CLocalDetailCacheMap::CQuadTreeElement::LoadStaticContents (0x00BDE390): already-loaded returns
// false; the first call requests the block; once the block is in memory the collection count is read, that many
// collections are pushed on the list front, each reads its header, then four has-child flags create children (or
// null); the element is marked loaded and the request released; an allocation failure unloads and returns false.
struct Stream {
    const int* data; int pos;
    int ReadLong() { return data[pos++]; }
};
struct Manager {
    bool requested, loaded, released; int requests;
    const int* block;
};
static int g_AllocBudget = 100;
static int g_HeadersRead = 0;
struct Collection { Collection* Next; int id; };
struct Element {
    Collection* Collections;
    Element* Children[4];
    bool StaticContentsLoaded, Loading;
    int unloads;
    Element() : Collections(0), StaticContentsLoaded(false), Loading(false), unloads(0)
    {
        for (int i = 0; i < 4; ++i) Children[i] = 0;
    }
    static bool CanAlloc() { if (g_AllocBudget <= 0) return false; --g_AllocBudget; return true; }
    void Unload() { ++unloads; StaticContentsLoaded = false; }
    bool LoadStaticContents(Manager& m)
    {
        if (StaticContentsLoaded) return false;
        if (!Loading) { m.requested = true; ++m.requests; Loading = true; }
        if (m.loaded) {
            Stream s; s.data = m.block; s.pos = 0;
            int count = s.ReadLong();
            for (int i = 0; i < count; ++i) {
                if (!CanAlloc()) { StaticContentsLoaded = true; Unload(); return false; }
                Collection* c = new Collection; c->id = i; c->Next = Collections; Collections = c;
            }
            for (Collection* c = Collections; c; c = c->Next) { s.ReadLong(); ++g_HeadersRead; }
            for (int child = 0; child < 4; ++child) {
                if (s.ReadLong()) {
                    if (!CanAlloc()) { Children[child] = 0; StaticContentsLoaded = true; Unload(); return false; }
                    Children[child] = new Element;
                    s.ReadLong();
                } else
                    Children[child] = 0;
            }
            StaticContentsLoaded = true;
            if (Loading) { m.released = true; Loading = false; }
        }
        return true;
    }
};
int main()
{
    static const int block[] = { 2, 70, 71, 1, 90, 0, 0, 1, 91 };
    Manager m; m.requested = m.loaded = m.released = false; m.requests = 0; m.block = block;
    Element e;
    bool r1 = e.LoadStaticContents(m);               // requests, block not yet loaded
    bool ok = r1 && m.requested && m.requests == 1 && !e.StaticContentsLoaded && e.Loading;
    m.loaded = true;
    bool r2 = e.LoadStaticContents(m);
    ok = ok && r2 && m.requests == 1 && e.StaticContentsLoaded && !e.Loading && m.released;
    ok = ok && e.Collections && e.Collections->id == 1 && e.Collections->Next->id == 0 && g_HeadersRead == 2;
    ok = ok && e.Children[0] && !e.Children[1] && !e.Children[2] && e.Children[3];
    ok = ok && !e.LoadStaticContents(m);            // already loaded
    g_AllocBudget = 1;
    Element f;
    bool r3 = f.LoadStaticContents(m);
    ok = ok && !r3 && f.unloads == 1;
    if (!ok) { std::printf("00bde390_TEST FAIL\n"); return 1; }
    std::printf("00bde390_TEST PASS\n");
    return 0;
}
