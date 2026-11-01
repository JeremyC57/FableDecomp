// Behaviour test for 0x0087B660 NCameraModes::CProjectileWeapon::~CProjectileWeapon: the implicit destructor frees the
// Attractors vector storage (when allocated) and then runs NCameraModes::CBase's destructor.
#include <cstdio>
#include <cstdlib>
static int g_freed = 0, g_baseDtors = 0;
static void* g_lastFreed = 0;
static void TrackedFree(void* p) { ++g_freed; g_lastFreed = p; std::free(p); }
class C3DVector { public: float X, Y, Z; };
template <class T> struct CMallocVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CMallocVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CMallocVector() { if (_M_start != 0) TrackedFree(_M_start); }
};
namespace NCameraModes {
class CBase { public: virtual ~CBase() { ++g_baseDtors; } };
class CProjectileWeapon : public CBase { public:
    long ShotFiredTimer; bool Finished;
    CMallocVector<C3DVector> Attractors;
    float MaxZoomInFOV, MinZoomInFOV, ZoomInFOV;
};
}
int main()
{
    void* storage = std::malloc(3 * sizeof(C3DVector));
    {
        NCameraModes::CProjectileWeapon mode;
        mode.Attractors._M_start = (C3DVector*)storage;
        mode.Attractors._M_finish = mode.Attractors._M_start + 1;
        mode.Attractors._M_end_of_storage = mode.Attractors._M_start + 3;
    }
    if (g_freed != 1 || g_lastFreed != storage || g_baseDtors != 1) { std::printf("0087b660_TEST FAIL\n"); return 1; }
    {
        NCameraModes::CProjectileWeapon mode;              // no attractors: nothing freed
    }
    if (g_freed != 1 || g_baseDtors != 2) { std::printf("0087b660_TEST FAIL\n"); return 1; }
    std::printf("0087b660_TEST PASS\n");
    return 0;
}