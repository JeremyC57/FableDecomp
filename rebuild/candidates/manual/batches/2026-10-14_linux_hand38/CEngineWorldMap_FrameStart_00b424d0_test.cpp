#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

static int g_order = 0, g_auto = 0, g_erase = 0, g_dispose = 0; static void* g_first = 0; static void* g_last = 0;
struct CEngineComponentParams { int x; };
struct CEngineMap { int x; };
struct C2DBoxI { long x0, y0, x1, y1; };
struct CRetailBoxVector { C2DBoxI* m_pFirst; C2DBoxI* m_pLast; C2DBoxI* m_pEnd; void erase(C2DBoxI* first, C2DBoxI* last) { g_erase = ++g_order; g_first = first; g_last = last; m_pLast = m_pFirst; } };
struct CRetailMapArray { CEngineMap** m_pBegin; CEngineMap** m_pEnd; CEngineMap** m_pCapacity; unsigned long Size() const { return (unsigned long)(m_pEnd - m_pBegin); } };
struct CBankFileAsync { unsigned char _body[0x171]; void ProcessDisposalList() { g_dispose = ++g_order; } };
struct CEngineWorldMap { unsigned char _pad0[0x1c]; CRetailMapArray MapList; unsigned char _pad1[0x34 - 0x2c]; CBankFileAsync StaticMapFile; bool FirstWorldUpdate; unsigned char _pad3[0x1b4 - 0x1a6]; CRetailBoxVector ChangedAreas;
    void AutoBuildWorldMapGrid() { g_auto = ++g_order; }
    virtual void FrameStart(const CEngineComponentParams& params) { AutoBuildWorldMapGrid(); for (unsigned long i = 1; i < MapList.Size(); ++i) { } ChangedAreas.erase(ChangedAreas.m_pFirst, ChangedAreas.m_pLast); StaticMapFile.ProcessDisposalList(); FirstWorldUpdate = false; } };
int main()
{
    CEngineWorldMap* w = (CEngineWorldMap*)std::malloc(sizeof(CEngineWorldMap)); std::memset(w, 0, sizeof(*w)); new (w) CEngineWorldMap();
    CEngineMap* maps[3] = { 0, 0, 0 }; w->MapList.m_pBegin = maps; w->MapList.m_pEnd = maps + 3; w->MapList.m_pCapacity = maps + 3;
    C2DBoxI boxes[4]; w->ChangedAreas.m_pFirst = boxes; w->ChangedAreas.m_pLast = boxes + 2; w->ChangedAreas.m_pEnd = boxes + 4; w->FirstWorldUpdate = true;
    CEngineComponentParams p; w->FrameStart(p);
    bool ok = g_auto == 1 && g_erase == 2 && g_dispose == 3 && g_first == boxes && g_last == boxes + 2 && !w->FirstWorldUpdate;
    std::free(w);
    if (!ok) { std::printf("00b424d0_TEST FAIL\n"); return 1; }
    std::printf("00b424d0_TEST PASS\n"); return 0;
}
