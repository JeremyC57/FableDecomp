#include <cstdio>
#include <cstring>
#include <new>

class C2DVector { public: float X; float Y; C2DVector() {} C2DVector(float x, float y) : X(x), Y(y) {} C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {} };
static int g_instances;
namespace NUISystem { class CManager { public: static CManager* GetInstance() { static CManager m; ++g_instances; return &m; } C2DVector GetUIScale() { return C2DVector(0.5f, 2.0f); } }; }
struct CSprite { C2DVector GetSize() { return C2DVector(2.0f, 3.0f); } };
struct CMouseCursor : CSprite {
    C2DVector GetScaledSize() { C2DVector size = GetSize();
        size.X = NUISystem::CManager::GetInstance()->GetUIScale().X * size.X;
        size.Y = NUISystem::CManager::GetInstance()->GetUIScale().Y * size.Y; return size; }
};
int main()
{
    CMouseCursor cursor; C2DVector r = cursor.GetScaledSize();
    if (r.X != 1.0f || r.Y != 6.0f || g_instances != 2) { std::printf("0055c600_TEST FAIL\n"); return 1; }
    std::printf("0055c600_TEST PASS\n");
    return 0;
}