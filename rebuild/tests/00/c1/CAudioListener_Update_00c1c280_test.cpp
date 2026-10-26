// Behaviour test for 00c1c280: CAudioListener::Update -- under the audio lock, hand the listener the position, front and top vectors with Y and Z swapped
#include <cstdio>

static int g_lockDepth = 0, g_order = 0, g_posOrder = 0, g_oriOrder = 0;
struct CScopedLock {
    CScopedLock(void*) { ++g_lockDepth; }
    ~CScopedLock() { --g_lockDepth; }
};
static int g_audioLock;

class C3DVector { public:
    float x, y, z;
    C3DVector SwapYZ() const { C3DVector r; r.x = x; r.y = z; r.z = y; return r; }
};

static void* g_handle; static C3DVector g_pos, g_front, g_top; static int g_lockedCalls = 0;
static void SetListenerPosition(void* handle, const C3DVector* position)
{ g_handle = handle; g_pos = *position; g_posOrder = ++g_order; if (g_lockDepth == 1) ++g_lockedCalls; }
static void SetListenerOrientation(void* handle, const C3DVector* front, const C3DVector* top)
{ if (handle != g_handle) g_handle = 0; g_front = *front; g_top = *top; g_oriOrder = ++g_order; if (g_lockDepth == 1) ++g_lockedCalls; }

struct CAudioListener {
    void* m_hListener;
    void Update(const C3DVector& position, const C3DVector& front, const C3DVector& top)
    {
        CScopedLock lock(&g_audioLock);
        C3DVector pos, fwd, up;
        pos = position.SwapYZ();
        fwd = front.SwapYZ();
        up = top.SwapYZ();
        SetListenerPosition(m_hListener, &pos);
        SetListenerOrientation(m_hListener, &fwd, &up);
    }
};

static bool Same(const C3DVector& v, float x, float y, float z)
{
    float a = v.x - x, b = v.y - y, c = v.z - z;
    return a < 0.0001f && a > -0.0001f && b < 0.0001f && b > -0.0001f && c < 0.0001f && c > -0.0001f;
}

int main()
{
    int token;
    CAudioListener l; l.m_hListener = &token;
    C3DVector p = { 1.0f, 2.0f, 3.0f }, f = { 4.0f, 5.0f, 6.0f }, t = { 7.0f, 8.0f, 9.0f };
    l.Update(p, f, t);
    if (g_handle != &token || !Same(g_pos, 1, 3, 2) || !Same(g_front, 4, 6, 5) || !Same(g_top, 7, 9, 8)) {
        std::printf("00c1c280_TEST FAIL\n"); return 1;
    }
    if (g_posOrder != 1 || g_oriOrder != 2 || g_lockedCalls != 2 || g_lockDepth != 0) { std::printf("00c1c280_TEST FAIL\n"); return 1; }
    std::printf("00c1c280_TEST PASS\n");
    return 0;
}