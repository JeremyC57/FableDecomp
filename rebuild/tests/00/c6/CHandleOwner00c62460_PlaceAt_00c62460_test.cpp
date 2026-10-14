#include <cstdio>
#include <cstdlib>
#include <cstring>
struct C3DVector { float X, Y, Z; C3DVector() { } C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } }; static C3DVector Add(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
static long g_handle = 0; static C3DVector g_pos; static void* g_context = 0; struct CTable { virtual void Place(long handle, const C3DVector* p) { g_handle = handle; g_pos = *p; } }; static CTable g_table;
struct STarget { unsigned char _pad[0x14]; C3DVector m_position; }; struct SContext { STarget* m_pTarget; };
struct COwner { unsigned char _pad[8]; long m_handle; C3DVector m_offset; CTable* GetHandleTable(SContext* c) { g_context = c; return &g_table; } void PlaceAt(SContext* context) { if (m_handle >= 0) { C3DVector sum = Add(m_offset, context->m_pTarget->m_position); C3DVector position(sum); GetHandleTable(context)->Place(m_handle, &position); } } };

int main() {
    COwner o; std::memset(&o, 0, sizeof o); o.m_handle = 3; o.m_offset = C3DVector(1, 2, 3); STarget t; t.m_position = C3DVector(10, 20, 30); SContext c; c.m_pTarget = &t;
    o.PlaceAt(&c); if (g_handle != 3 || g_pos.X != 11 || g_pos.Y != 22 || g_pos.Z != 33 || g_context != &c) { std::printf("00c62460_TEST FAIL\n"); return 1; } o.m_handle = -1; g_handle = 0; o.PlaceAt(&c); if (g_handle) { std::printf("00c62460_TEST FAIL\n"); return 1; }
    std::printf("00c62460_TEST PASS\n"); return 0;
}