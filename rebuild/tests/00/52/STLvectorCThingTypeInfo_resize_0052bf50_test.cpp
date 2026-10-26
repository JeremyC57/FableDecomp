// Behaviour test for 0052bf50 vector<CThingTypeInfo>::resize(n) (copy of the model; the two-argument resize is a
// recording stand-in, the string counts constructions/destructions).
#include <cstdio>
static int g_ctor = 0, g_dtor = 0;
class CCharString { public:
    void* PStringData;
    CCharString() : PStringData(0) { ++g_ctor; }
    CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_ctor; }
    ~CCharString() { ++g_dtor; }
};
class CThingTypeInfo {
public:
    long          DefType;
    unsigned long ThingTypeFlags;
    CCharString   DefClassType;
    CThingTypeInfo() {}
};
static unsigned int g_n = 0;
static const CThingTypeInfo* g_x = 0;
static int g_liveDuring = 0;
template <class T> class vector {
public:
    typedef unsigned int size_type;
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    void resize(size_type new_size, const T& x) { g_n = new_size; g_x = &x; g_liveDuring = g_ctor - g_dtor; }
    void resize(size_type new_size) { resize(new_size, T()); }
};
int main()
{
    vector<CThingTypeInfo> v; v._M_start = v._M_finish = v._M_end_of_storage = 0;
    v.resize(16);
    int ok = 1;
    if (g_n != 16 || g_x == 0 || g_liveDuring != 1) ok = 0;     // a temporary is alive during the call
    if (g_ctor != 1 || g_dtor != 1) ok = 0;                     // and destroyed afterwards
    if (!ok) { std::printf("0052bf50_TEST FAIL\n"); return 1; }
    std::printf("0052bf50_TEST PASS\n");
    return 0;
}