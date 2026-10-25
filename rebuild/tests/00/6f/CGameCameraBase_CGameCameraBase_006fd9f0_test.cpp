// behaviour test for 0x006FD9F0 CGameCameraBase::~CGameCameraBase: LocalThingPosList, then DrawCoordList, then the base
#include <cstdio>

static int g_order[8];
static int g_count = 0;
static void Record(int id) { if (g_count < 8) g_order[g_count] = id; ++g_count; }

class C3DVector {
public:
    float X, Y, Z;
};

namespace _STL {
template <class _Tp> class list {                        // stand-in: an id instead of the sentinel node
public:
    int _M_id;
    list() : _M_id(0) { }
    ~list() { Record(_M_id); }                           // stand-in for 0x0048CA60
};
}

class CAIGameCameraBase {
public:
    virtual ~CAIGameCameraBase() { Record(100); }
};

class CGameCameraBase : public CAIGameCameraBase {
public:
    unsigned char          _camera[0xa4];
    _STL::list<C3DVector>  DrawCoordList;
    _STL::list<C3DVector>  LocalThingPosList;
    CGameCameraBase() { DrawCoordList._M_id = 1; LocalThingPosList._M_id = 2; }
    virtual ~CGameCameraBase();
};

CGameCameraBase::~CGameCameraBase()
{
}

int main()
{
    {
        CGameCameraBase camera;
        g_count = 0;
    }
    if (g_count != 3 || g_order[0] != 2 || g_order[1] != 1 || g_order[2] != 100) { std::printf("006fd9f0_TEST FAIL\n"); return 1; }
    g_count = 0;
    CAIGameCameraBase* base = new CGameCameraBase;
    delete base;                                         // the virtual destructor reaches the derived one
    if (g_count != 3 || g_order[0] != 2 || g_order[1] != 1 || g_order[2] != 100) { std::printf("006fd9f0_TEST FAIL\n"); return 1; }
    std::printf("006fd9f0_TEST PASS\n");
    return 0;
}