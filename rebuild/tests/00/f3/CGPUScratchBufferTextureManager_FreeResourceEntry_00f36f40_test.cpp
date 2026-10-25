// Behaviour test for 0x00F36F40 CGPUScratchBufferTextureManager::FreeResourceEntry: push the index on the free-slot
// list, clear the entry (type 0, texture and surface deleted and nulled), then erase the first occurrence of the index
// from its memory page's resource list (memmove of the tail, unless it was the last element).
#include <cstdio>
#include <cstring>

static int g_texturesFreed = 0;
static int g_surfacesFreed = 0;

struct CTexture { int id; void Uninitialise() { ++g_texturesFreed; } ~CTexture() { Uninitialise(); } };
struct CSurface { virtual ~CSurface() { ++g_surfacesFreed; } };

struct LongVec {
    long items[16];
    unsigned int count;
    void push_back(long v) { items[count++] = v; }
    void erase(unsigned int pos)
    {
        long* position = items + pos;
        long* finish = items + count;
        if (position + 1 != finish)
            std::memmove(position, position + 1, (const char*)finish - (const char*)(position + 1));
        --count;
    }
};

struct Entry { long ResourceType; CTexture* Texture; CSurface* Surface; long PageIndex; };
struct Page { LongVec ResourceList; };

struct CManager {
    Entry   Resources[8];
    Page    MemoryPages[4];
    LongVec FreeTextureSlots;
    void FreeResourceEntry(long index)
    {
        FreeTextureSlots.push_back(index);
        long pageIndex = Resources[index].PageIndex;
        Resources[index].ResourceType = 0;
        delete Resources[index].Texture;
        delete Resources[index].Surface;
        Resources[index].Texture = 0;
        Resources[index].Surface = 0;
        Page& page = MemoryPages[pageIndex];
        for (unsigned long i = 0; i < page.ResourceList.count; ++i) {
            if (page.ResourceList.items[i] == index) {
                page.ResourceList.erase(i);
                return;
            }
        }
    }
};

int main()
{
    static CManager m;
    m.FreeTextureSlots.count = 0;
    for (unsigned int i = 0; i < 8; ++i) { m.Resources[i].ResourceType = 3; m.Resources[i].Texture = 0; m.Resources[i].Surface = 0; m.Resources[i].PageIndex = i % 2; }
    for (unsigned int p = 0; p < 4; ++p) m.MemoryPages[p].ResourceList.count = 0;
    m.MemoryPages[1].ResourceList.push_back(1);
    m.MemoryPages[1].ResourceList.push_back(3);
    m.MemoryPages[1].ResourceList.push_back(5);
    m.Resources[3].Texture = new CTexture;
    m.Resources[3].Surface = new CSurface;
    int ok = 1;
    m.FreeResourceEntry(3);                                   // middle element: tail moved down
    if (m.FreeTextureSlots.count != 1 || m.FreeTextureSlots.items[0] != 3) ok = 0;
    if (m.Resources[3].ResourceType != 0 || m.Resources[3].Texture != 0 || m.Resources[3].Surface != 0) ok = 0;
    if (g_texturesFreed != 1 || g_surfacesFreed != 1) ok = 0;
    if (m.MemoryPages[1].ResourceList.count != 2 || m.MemoryPages[1].ResourceList.items[0] != 1 || m.MemoryPages[1].ResourceList.items[1] != 5) ok = 0;
    m.FreeResourceEntry(5);                                   // last element: no move
    if (m.MemoryPages[1].ResourceList.count != 1 || m.MemoryPages[1].ResourceList.items[0] != 1) ok = 0;
    m.FreeResourceEntry(7);                                   // not listed: list untouched, slot still freed
    if (m.MemoryPages[1].ResourceList.count != 1 || m.FreeTextureSlots.count != 3 || m.FreeTextureSlots.items[2] != 7) ok = 0;
    if (g_texturesFreed != 1 || g_surfacesFreed != 1) ok = 0;  // null pointers are not deleted
    std::printf(ok ? "00f36f40_TEST PASS\n" : "00f36f40_TEST FAIL\n");
    return ok ? 0 : 1;
}