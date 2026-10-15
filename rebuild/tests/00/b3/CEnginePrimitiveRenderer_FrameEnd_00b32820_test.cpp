// Behaviour test for 0x00B32820 (see src/00b32820.cpp): FrameEnd over recording scenes and render entries.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("00b32820_TEST FAIL\n"); return 1; } } while (0)

struct CEngineScene { int frameEnds; int resets; int order; void FrameEnd() { ++frameEnds; } void ResetRenderLists() { ++resets; } };
struct CRenderable { int ended; void EndFrame() { ++ended; } };
struct SRenderEntry { CRenderable* m_pObject; int m_key; };
struct CEnginePrimitiveRenderer {
    bool m_frameFlag;
    SRenderEntry* nsFirst; SRenderEntry* nsLast;
    CEngineScene** scFirst; CEngineScene** scLast;
    unsigned int NsCount() const { return (unsigned int)(nsLast - nsFirst); }
    unsigned int ScCount() const { return (unsigned int)(scLast - scFirst); }
    void FrameEnd()
    {
        for (CEngineScene** scene = scFirst; scene != scLast; ++scene) (*scene)->FrameEnd();
        for (unsigned int i = 0; i < NsCount(); ++i) nsFirst[i].m_pObject->EndFrame();
        for (unsigned int j = 0; j < ScCount(); ++j) scFirst[j]->ResetRenderLists();
        m_frameFlag = false;
    }
};
int main()
{
    CEngineScene s1 = { 0, 0, 0 }, s2 = { 0, 0, 0 }; CEngineScene* scenes[2] = { &s1, &s2 };
    CRenderable r1 = { 0 }, r2 = { 0 }, r3 = { 0 }; SRenderEntry entries[3] = { { &r1, 0 }, { &r2, 1 }, { &r3, 2 } };
    CEnginePrimitiveRenderer r; r.m_frameFlag = true; r.nsFirst = entries; r.nsLast = entries + 3; r.scFirst = scenes; r.scLast = scenes + 2;
    r.FrameEnd();
    CHECK(s1.frameEnds == 1 && s2.frameEnds == 1 && s1.resets == 1 && s2.resets == 1);
    CHECK(r1.ended == 1 && r2.ended == 1 && r3.ended == 1 && !r.m_frameFlag);
    r.scLast = scenes; r.nsLast = entries; r.m_frameFlag = true; r.FrameEnd();
    CHECK(s1.frameEnds == 1 && r1.ended == 1 && !r.m_frameFlag);
    std::printf("00b32820_TEST PASS\n");
    return 0;
}