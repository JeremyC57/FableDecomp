// Behaviour test for the implicit CTCHeroPodium::~CTCHeroPodium 0x00771F30: free the CrowdOccupation block (if any), then ~CTCBase
#include <cstdio>
#include <cstdlib>
static int g_freed = 0, g_baseDtors = 0;
static void Free(void* p) { ++g_freed; std::free(p); }
struct LongVector {
    long* _M_start; long* _M_finish; long* _M_end_of_storage;
    LongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~LongVector() { if (_M_start != 0) Free(_M_start); }
};
class CTCBase { public:
    virtual ~CTCBase() { ++g_baseDtors; }
    void* ParentThing;
};
class CTCHeroPodium : public CTCBase { public:
    long NumberOfCrowdOccupants;
    LongVector CrowdOccupation;
    CTCHeroPodium() : NumberOfCrowdOccupants(0) {}
};
int main()
{
    {
        CTCHeroPodium empty;
    }
    if (g_freed != 0 || g_baseDtors != 1) { std::printf("00771f30_TEST FAIL\n"); return 1; }
    {
        CTCHeroPodium full;
        full.CrowdOccupation._M_start = (long*)std::malloc(4 * sizeof(long));
        full.CrowdOccupation._M_finish = full.CrowdOccupation._M_start + 2;
        full.CrowdOccupation._M_end_of_storage = full.CrowdOccupation._M_start + 4;
    }
    if (g_freed != 1 || g_baseDtors != 2) { std::printf("00771f30_TEST FAIL\n"); return 1; }
    CTCBase* p = new CTCHeroPodium;
    delete p;
    if (g_baseDtors != 3) { std::printf("00771f30_TEST FAIL\n"); return 1; }
    std::printf("00771f30_TEST PASS\n");
    return 0;
}