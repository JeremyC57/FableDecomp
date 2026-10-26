#include <cstdio>
#include <cstdlib>
#include <cstring>
class C2DVector { public: float X, Y; }; static void* g_owner = 0; static const C2DVector* g_pos = 0; static long g_flags = 0;
struct CHitShape { virtual C2DVector GetClosestPointToPos2D(void* owner, const C2DVector& position, long flags) = 0; };
struct CShape : CHitShape { virtual C2DVector GetClosestPointToPos2D(void* owner, const C2DVector& position, long flags) { g_owner = owner; g_pos = &position; g_flags = flags; C2DVector r = { position.X + 2, position.Y + 2 }; return r; } };
struct CHolder { unsigned char _pad[0x3c]; CHitShape* m_pShape; };
struct CHitLocation { void* m_pOwner; CHolder* m_pHolder; C2DVector GetClosestPointToPos2D(const C2DVector& position, long flags) { return m_pHolder->m_pShape->GetClosestPointToPos2D(m_pOwner, position, flags); } };

int main() {
    CShape shape; CHolder holder; holder.m_pShape = &shape; CHitLocation l; int owner; l.m_pOwner = &owner; l.m_pHolder = &holder; C2DVector p = { 1, 2 }; C2DVector r = l.GetClosestPointToPos2D(p, 7);
    if (r.X != 3 || r.Y != 4 || g_owner != &owner || g_pos != &p || g_flags != 7) { std::printf("00815370_TEST FAIL\n"); return 1; }
    std::printf("00815370_TEST PASS\n"); return 0;
}