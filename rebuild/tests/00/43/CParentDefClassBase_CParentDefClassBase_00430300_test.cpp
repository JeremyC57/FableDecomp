// Behaviour test for 0x00430300 CParentDefClassBase::~CParentDefClassBase(): the SubDefInfoMap vector storage is freed when there
// is one, then the CDefClassBase / CDefPointeeBase / CResource destructors run.
#include <cstdio>
#include <cstdlib>
static int g_resource = 0;
class CResource { public: virtual ~CResource() { ++g_resource; } };
class CDefPointeeBase : public CResource { public: virtual ~CDefPointeeBase() { } };
class CDefClassBase : public CDefPointeeBase { };
template <class T> struct CVector { T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CVector() { if (_M_start) std::free(_M_start); } };
class CParentDefClassBase : public CDefClassBase { public:
    long InstantiationName;
    CVector<long> SubDefInfoMap;
};
int main()
{
    { CParentDefClassBase a; a.SubDefInfoMap._M_start = (long*)std::malloc(16); }
    { CParentDefClassBase b; }
    if (g_resource != 2) { std::printf("00430300_TEST FAIL\n"); return 1; }
    std::printf("00430300_TEST PASS\n");
    return 0;
}
