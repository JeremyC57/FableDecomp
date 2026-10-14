#include <cstdio>
#include <cstring>
#include <new>

struct C2DVector { float X; float Y; C2DVector() {} C2DVector(float x, float y) : X(x), Y(y) {} C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {} };
static int g_instances;
struct CFrontEndManager { static CFrontEndManager* GetInstance() { static CFrontEndManager m; ++g_instances; return &m; } C2DVector GetUIScale() { return C2DVector(0.5f, 2.0f); } };
struct CSprite { C2DVector GetSize() { return C2DVector(2.0f, 3.0f); } };
struct CMouseCursor : CSprite {
    C2DVector GetScaledSize() { C2DVector size = GetSize();
        size.X = CFrontEndManager::GetInstance()->GetUIScale().X * size.X;
        size.Y = CFrontEndManager::GetInstance()->GetUIScale().Y * size.Y; return size; }
};
int main()
{
    CMouseCursor cursor; C2DVector r = cursor.GetScaledSize();
    if (r.X != 1.0f || r.Y != 6.0f || g_instances != 2) { std::printf("0055c600_TEST FAIL\n"); return 1; }
    std::printf("0055c600_TEST PASS\n");
    return 0;
}