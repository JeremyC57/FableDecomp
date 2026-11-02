// Behaviour test for CBaseVideoRenderer::~CBaseVideoRenderer (0x00CA7310): the empty derived body runs, then the base
// CBaseRenderer destructor (deleting through the base pointer reaches both, derived first).
#include <cstdio>
static int order[4]; static int n = 0;
struct INonDelegatingUnknown { virtual long NonDelegatingQueryInterface() = 0; };
struct IQualProp { virtual long get_FramesDrawn(int*) = 0; };
struct IQualityControl { virtual long Notify(void*, int) = 0; };
class CUnknown : public INonDelegatingUnknown {
public:
    virtual long NonDelegatingQueryInterface() { return 0; }
    virtual ~CUnknown() { }
};
class CBaseRenderer : public CUnknown {
public:
    virtual ~CBaseRenderer() { order[n++] = 2; }
};
class CBaseVideoRenderer : public CBaseRenderer, public IQualProp, public IQualityControl {
public:
    int m_cFramesDrawn;
    CBaseVideoRenderer() : m_cFramesDrawn(3) { }
    virtual long get_FramesDrawn(int* p) { *p = m_cFramesDrawn; return 0; }
    virtual long Notify(void*, int) { return 0; }
    virtual ~CBaseVideoRenderer();
};
CBaseVideoRenderer::~CBaseVideoRenderer()
{
    order[n++] = 1;
}
int main()
{
    CUnknown* u = new CBaseVideoRenderer;
    IQualProp* q = static_cast<CBaseVideoRenderer*>(u);
    int drawn = 0; q->get_FramesDrawn(&drawn);
    delete u;
    if (drawn != 3 || n != 2 || order[0] != 1 || order[1] != 2) { std::printf("00ca7310_TEST FAIL\n"); return 1; }
    std::printf("00ca7310_TEST PASS\n");
    return 0;
}