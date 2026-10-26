// Behaviour test for 0x00B2AA70 std::vector<CCamera>::push_back: with room the camera is copy-constructed in place at
// _M_finish (through the copy constructor) and _M_finish advances; when full, _M_insert_overflow gets
// (_M_finish, x, tag, 1, true).
#include <cstdio>
#include <new>
static int g_copies = 0, g_grows = 0;
static const void* g_growValue = 0;
static unsigned int g_growCount = 0;
static bool g_growAtEnd = false;
class CCamera { public:
    float WorldPosition[3];
    float RHSet[6];
    bool HeightLocked, ViewVecZLocked;
    unsigned long FOVFlags;
    float HorizontalFOV, VerticalFOV, Zoom;
    CCamera() { }
    CCamera(const CCamera& o) { ++g_copies; WorldPosition[0] = o.WorldPosition[0]; Zoom = o.Zoom; }
};
struct __false_type { __false_type() {} };
struct CCameraVector {
    CCamera* _M_start; CCamera* _M_finish; CCamera* _M_end_of_storage;
    void _M_insert_overflow(CCamera*, const CCamera& x, const __false_type&, unsigned int fill_len, bool atend)
    { ++g_grows; g_growValue = &x; g_growCount = fill_len; g_growAtEnd = atend; }
    void push_back(const CCamera& __x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) CCamera(__x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, __x, __false_type(), 1, true);
    }
};
int main()
{
    static CCamera buf[2];
    CCameraVector v; v._M_start = buf; v._M_finish = buf; v._M_end_of_storage = buf + 2;
    CCamera x; x.WorldPosition[0] = 9.0f; x.Zoom = 1.5f;
    v.push_back(x); v.push_back(x); v.push_back(x);
    if (g_copies != 2 || g_grows != 1 || v._M_finish != buf + 2 || buf[1].WorldPosition[0] != 9.0f || buf[1].Zoom != 1.5f
        || g_growValue != &x || g_growCount != 1 || !g_growAtEnd) { std::printf("00b2aa70_TEST FAIL\n"); return 1; }
    std::printf("00b2aa70_TEST PASS\n");
    return 0;
}
