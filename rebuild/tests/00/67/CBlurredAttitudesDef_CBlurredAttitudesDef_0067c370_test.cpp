// Behaviour test for 0x0067C370 (implicit CBlurredAttitudesDef::~CBlurredAttitudesDef): frees the Attitudes storage, then
// the parent def's SubDefInfoMap storage (a null storage is not freed), then the CResource base runs last.
#include <cstdio>
#include <cstdlib>

static void* g_freed[8];
static int g_frees = 0;
static int g_resourceDtors = 0;
static int g_order = 0, g_resourceOrder = 0;
static void test_free(void* p) { if (p) { if (g_frees < 8) g_freed[g_frees] = p; ++g_frees; ++g_order; std::free(p); } }

class CResource {
public:
    CResource() {}
    virtual ~CResource() { ++g_resourceDtors; g_resourceOrder = ++g_order; }
};
class CDefPointeeBase : public CResource { public: virtual ~CDefPointeeBase() {} };
class CDefClassBase : public CDefPointeeBase { public: virtual ~CDefClassBase() {} };

template <class T> class vector {                        // model of the STLport vector storage release
public:
    vector() : Start(0), Finish(0), EndOfStorage(0) {}
    ~vector() { if (Start != 0) test_free(Start); }
    void Allocate(int n) { Start = (T*)std::malloc(sizeof(T) * n); Finish = Start; EndOfStorage = Start + n; }
    T* Start; T* Finish; T* EndOfStorage;
};
struct SubDefInfoPair { unsigned int Key; long DefIndex; long OriginalParentDefIndex; };
class CParentDefClassBase : public CDefClassBase {
public:
    long InstantiationName;
    vector<SubDefInfoPair> SubDefInfoMap;
};
enum EOpinionAttitudeType { ATTITUDE_A, ATTITUDE_B };
class CBlurredAttitudesDef : public CParentDefClassBase {
public:
    vector<EOpinionAttitudeType> Attitudes;
};

int main()
{
    void* attitudes = 0;
    void* subDefs = 0;
    {
        CBlurredAttitudesDef def;
        def.Attitudes.Allocate(4); attitudes = def.Attitudes.Start;
        def.SubDefInfoMap.Allocate(2); subDefs = def.SubDefInfoMap.Start;
    }
    if (g_frees != 2 || g_freed[0] != attitudes || g_freed[1] != subDefs || g_resourceDtors != 1 || g_resourceOrder != 3) {
        std::printf("0067c370_TEST FAIL\n"); return 1;
    }
    {
        CBlurredAttitudesDef empty;                      // null storages: nothing freed, base still destroyed
    }
    if (g_frees != 2 || g_resourceDtors != 2) { std::printf("0067c370_TEST FAIL\n"); return 1; }
    std::printf("0067c370_TEST PASS\n");
    return 0;
}