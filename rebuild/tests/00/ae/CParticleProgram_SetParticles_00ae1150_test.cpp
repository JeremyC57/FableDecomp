#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
#define free test_free
#define malloc test_malloc
// Behaviour test for 00ae1150: the reconstructed source below (copied verbatim from src/) runs against stub callees.
// CParticleProgram::SetParticles(const vector<C3DVector>&, CMovableResourceMemoryPool*) -- size the program's non-simulated particle array to the given points, allocate it from the pool and fill it with the points as (x, y, z, 0) vectors; false when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE1150, length 192 bytes (`ret 8`). The manifest's class
// "CParticleSimulation" is propagated: the fields used (+0xbc count, +0xc0 array, and the allocator's +0x138/+0x13c
// handle and pool) are CParticleProgram's NonSimCount / NonSimPositions / MemoryHandle / MemoryPool at their retail
// offsets (PDB +0xcc/+0xd0/+0x168/+0x16c less the 16 bytes of narrower containers).
// Callees: CParticleArrayOwner::AllocateParticles(unsigned long, CMovableResourceMemoryPool*) @ 0x00ADF460 (the landed
//          model of this allocator; the program derives from it here so the call binds to the same symbol).
// Shape: the count is the signed 12-byte element difference stored straight into the member; each point is built as a
// C4DVector temporary (W = 0.0f, hoisted out of the loop as the zero register) and copied as four dwords.
class C3DVector { public: float X, Y, Z; };
class C4DVector { public:
    float X, Y, Z, W;
    C4DVector(const C3DVector& v, float w) : X(v.X), Y(v.Y), Z(v.Z), W(w) {}
};

class CMovableResource { public:
    virtual void v00();
};
class CMovableResourceMemoryPool;

struct CParticleArrayOwner : CMovableResource {
    unsigned char               _pad04[0xb8];
    unsigned long               NonSimCount;         // +0xbc
    C4DVector*                  m_pParticles;        // +0xc0
    unsigned char               _padC4[0x74];
    long                        m_memoryHandle;      // +0x138
    CMovableResourceMemoryPool* m_pMemoryPool;       // +0x13c
    bool AllocateParticles(unsigned long count, CMovableResourceMemoryPool* pool);   // 0x00ADF460
};

template<class T> struct CRetailVector {
    T* m_first;
    T* m_last;
    T* m_end;
    unsigned long size() const { return m_last - m_first; }
    const T& operator[](unsigned long i) const { return m_first[i]; }
};

class CParticleProgram : public CParticleArrayOwner { public:
    bool SetParticles(const CRetailVector<C3DVector>& points, CMovableResourceMemoryPool* pool);
};

bool CParticleProgram::SetParticles(const CRetailVector<C3DVector>& points, CMovableResourceMemoryPool* pool)
{
    NonSimCount = points.size();
    if (!AllocateParticles(NonSimCount, pool))
        return false;
    for (unsigned long i = 0; i < NonSimCount; ++i)
        m_pParticles[i] = C4DVector(points[i], 0.0f);
    return true;
}

// ---- stubs and driver ----
static C4DVector* g_storage = 0;
static bool g_allocOk = true;
static unsigned long g_requested = 0;
static unsigned char g_buf[sizeof(C4DVector) * 8];
void CMovableResource::v00() {}
bool CParticleArrayOwner::AllocateParticles(unsigned long count, CMovableResourceMemoryPool* pool)
{
    g_requested = count; m_pMemoryPool = pool;
    if (!g_allocOk) return false;
    m_pParticles = (C4DVector*)g_buf;
    return true;
}
int main()
{
    C3DVector pts[3] = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
    CRetailVector<C3DVector> v; v.m_first = pts; v.m_last = pts + 3; v.m_end = pts + 3;
    static CParticleProgram prog;
    std::memset(g_buf, 0xcd, sizeof(g_buf));
    CMovableResourceMemoryPool* pool = (CMovableResourceMemoryPool*)0x99;
    bool r = prog.SetParticles(v, pool);
    C4DVector* p = (C4DVector*)g_buf;
    bool ok = r && prog.NonSimCount == 3 && g_requested == 3 && prog.m_pMemoryPool == pool &&
              p[0].X == 1 && p[0].Y == 2 && p[0].Z == 3 && p[0].W == 0.0f && p[2].X == 7 && p[2].Z == 9 && p[2].W == 0.0f;
    g_allocOk = false;
    unsigned char before = g_buf[sizeof(C4DVector) * 3];
    v.m_last = pts + 1;
    ok = ok && !prog.SetParticles(v, pool) && prog.NonSimCount == 1 && g_buf[sizeof(C4DVector) * 3] == before;
    if (!ok) { std::printf("00ae1150_TEST FAIL\n"); return 1; }
    std::printf("00ae1150_TEST PASS\n");
    return 0;
}