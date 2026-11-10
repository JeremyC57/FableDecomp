// Behaviour test for 0x004F9CFC CCardPositionsDef::~CCardPositionsDef(): the Offset vector's storage is freed (no element dtors:
// C2DVector), then the CParentDefClassBase destructor runs.
#include <cstdio>
#include <cstdlib>
static int g_base = 0;
class CParentDefClassBase { public: virtual ~CParentDefClassBase() { ++g_base; } };
struct C2DVector { float X, Y; };
template <class T> struct CVector { T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CVector() { if (_M_start) std::free(_M_start); } };
class CCardPositionsDef : public CParentDefClassBase { public:
    CVector<C2DVector> Offset;
    ~CCardPositionsDef() { }
};
int main()
{
    {
        CCardPositionsDef d;
        d.Offset._M_start = (C2DVector*)std::malloc(4 * sizeof(C2DVector));
        d.Offset._M_finish = d.Offset._M_start + 2; d.Offset._M_end_of_storage = d.Offset._M_start + 4;
    }
    { CCardPositionsDef e; }
    if (g_base != 2) { std::printf("004f9cfc_TEST FAIL\n"); return 1; }
    std::printf("004f9cfc_TEST PASS\n");
    return 0;
}
