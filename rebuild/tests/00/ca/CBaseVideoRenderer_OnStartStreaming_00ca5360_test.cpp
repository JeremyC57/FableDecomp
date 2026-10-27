#include <cstdio>
// CBaseVideoRenderer::OnStartStreaming: ResetStreamingTimes() (virtual), NOERROR.
class Renderer {
public:
    int resets;
    Renderer() : resets(0) {}
    virtual ~Renderer() {}
    virtual int ResetStreamingTimes() { ++resets; return 0x1234; }
    int OnStartStreaming() { ResetStreamingTimes(); return 0; }
};
class Derived : public Renderer {
public:
    int ResetStreamingTimes() { resets += 10; return -1; }
};
int main()
{
    Renderer r; Derived d;
    if (r.OnStartStreaming() != 0 || r.resets != 1) { std::printf("00ca5360_TEST FAIL\n"); return 1; }
    if (d.OnStartStreaming() != 0 || d.resets != 10) { std::printf("00ca5360_TEST FAIL\n"); return 1; }
    std::printf("00ca5360_TEST PASS\n");
    return 0;
}