#include <cstdio>
struct IMediaSample { int draw; };
struct CBaseRenderer {
    virtual int ScheduleSample(IMediaSample* p) { return p != 0 && p->draw; }
    virtual ~CBaseRenderer() { }
};
struct CBaseVideoRenderer : CBaseRenderer {
    int m_cFramesDropped;
    virtual int ScheduleSample(IMediaSample* pMediaSample)
    {
        int bDrawImage = CBaseRenderer::ScheduleSample(pMediaSample);
        if (bDrawImage == 0) { ++m_cFramesDropped; return 0; }
        return 1;
    }
};
int main()
{
    CBaseVideoRenderer r; r.m_cFramesDropped = 0;
    IMediaSample yes = { 1 }, no = { 0 };
    CBaseRenderer* p = &r;
    bool ok = p->ScheduleSample(&yes) == 1 && r.m_cFramesDropped == 0 && p->ScheduleSample(&no) == 0 && p->ScheduleSample(0) == 0 && r.m_cFramesDropped == 2;
    if (!ok) { std::printf("00ca5cf0_TEST FAIL\n"); return 1; }
    std::printf("00ca5cf0_TEST PASS\n");
    return 0;
}