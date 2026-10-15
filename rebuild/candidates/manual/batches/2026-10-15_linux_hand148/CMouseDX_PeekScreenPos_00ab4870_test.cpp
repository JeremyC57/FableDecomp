#include <cstdio>
// CMouseDX::PeekScreenPos model: exclusive mode returns Pos, otherwise Pos + the client rectangle's screen origin.
struct C2DVector { float x, y; C2DVector(float ax, float ay) : x(ax), y(ay) { } };
struct POINT { long x; long y; };
struct RECT { long left; long top; long right; long bottom; };
static void ClientToScreen(POINT* p) { p->x += 100; p->y += 50; }
static void GetClientRect(RECT* r) { r->left = 0; r->top = 0; r->right = 640; r->bottom = 480; }
static void OffsetRect(RECT* r, long dx, long dy) { r->left += dx; r->right += dx; r->top += dy; r->bottom += dy; }
struct CMouseDX {
    C2DVector Pos; bool ExclusiveMode;
    CMouseDX() : Pos(3.0f, 4.0f), ExclusiveMode(false) { }
    C2DVector PeekScreenPos() const
    {
        if (ExclusiveMode) return Pos;
        POINT origin; origin.x = 0; origin.y = 0;
        ClientToScreen(&origin);
        RECT client; GetClientRect(&client);
        OffsetRect(&client, origin.x, origin.y);
        return C2DVector((float)client.left + Pos.x, (float)client.top + Pos.y);
    }
};
int main()
{
    CMouseDX m;
    C2DVector a = m.PeekScreenPos();
    m.ExclusiveMode = true;
    C2DVector b = m.PeekScreenPos();
    if (!(a.x == 103.0f && a.y == 54.0f && b.x == 3.0f && b.y == 4.0f)) { std::printf("00ab4870_TEST FAIL\n"); return 1; }
    std::printf("00ab4870_TEST PASS\n");
    return 0;
}
