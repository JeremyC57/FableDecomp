#include <cstdio>
#include <cstring>
static char g_seq[16] = "";
struct IMediaSample { int id; };
struct CBaseRenderer {
    int m_bStreaming;
    virtual void OnRenderStart(IMediaSample*) { std::strcat(g_seq, "S"); }
    virtual void OnRenderEnd(IMediaSample*) { std::strcat(g_seq, "E"); }
    virtual long DoRenderSample(IMediaSample*) { std::strcat(g_seq, "D"); return 0; }
    virtual ~CBaseRenderer() { }
    long Render(IMediaSample* pMediaSample)
    {
        if (pMediaSample == 0) return 1;
        if (m_bStreaming == 0) return 1;
        OnRenderStart(pMediaSample); DoRenderSample(pMediaSample); OnRenderEnd(pMediaSample);
        return 0;
    }
};
int main()
{
    CBaseRenderer r; IMediaSample s; r.m_bStreaming = 0;
    bool ok = r.Render(0) == 1 && r.Render(&s) == 1 && g_seq[0] == 0;
    r.m_bStreaming = 1; ok = ok && r.Render(&s) == 0 && !std::strcmp(g_seq, "SDE");
    if (!ok) { std::printf("00ca4b20_TEST FAIL\n"); return 1; }
    std::printf("00ca4b20_TEST PASS\n");
    return 0;
}
