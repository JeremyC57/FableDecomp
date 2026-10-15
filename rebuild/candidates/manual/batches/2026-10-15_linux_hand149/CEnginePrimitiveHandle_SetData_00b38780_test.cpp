#include <cstdio>
struct Handle;
static int g_lastReleased = 0, g_destroyed = 0;
struct Prim {
    long RefCount; int id;
    void Destroy() { g_destroyed = id; }
    void OnLastHandleReleased(Handle*) { g_lastReleased = id; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct Handle {
    Prim* Object;
    void Clear() { if (Object && Object->RefCount == 1) Object->OnLastHandleReleased(this); if (Object) { Object->Release(); Object = 0; } Object = 0; }
    void SetData(Prim* p) { Clear(); if (Object) { Object->Release(); Object = 0; } Object = p; }
};
int main()
{
    Prim a = { 1, 1 }, b = { 2, 2 };
    Handle h; h.Object = &a;
    h.SetData(&b);
    bool fail = h.Object != &b || g_lastReleased != 1 || g_destroyed != 1 || a.RefCount != 0;
    g_lastReleased = g_destroyed = 0;
    h.SetData(0);
    if (h.Object || g_lastReleased || g_destroyed || b.RefCount != 1) fail = true;
    if (fail) { std::printf("00b38780_TEST FAIL\n"); return 1; }
    std::printf("00b38780_TEST PASS\n");
    return 0;
}
