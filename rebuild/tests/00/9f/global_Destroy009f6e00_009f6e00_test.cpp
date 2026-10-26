// Behaviour test for 0x009F6E00 Destroy_009f6e00: destroy a range of CInputEventPackage (each package's event vector
// destroys its events and frees its storage).
#include <cstdio>
#include <cstdlib>
#include <new>

struct SAllocatorTag { SAllocatorTag() { } };
static int g_eventDtors = 0;
static int g_frees = 0;
static void TestFree(void* p) { if (p) { ++g_frees; std::free(p); } }

class CInputEvent { public:
    unsigned char _body[0x34];
    ~CInputEvent() { ++g_eventDtors; }
};
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T> inline void _Destroy(T* first, T* last) { for (; first != last; ++first) _Destroy(first); }
template <class T> struct _Vector_base {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    _Vector_base() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~_Vector_base() { if (_M_start) TestFree(_M_start); }
};
template <class T> struct vector : _Vector_base<T> {
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
class CChecksum { public: unsigned long Words[4]; };
class C2DVector { public: float X, Y; };
class CInputEventPackage { public:
    vector<CInputEvent> Events;
    long                TimeStamp;
    CChecksum           Checksum;
    C2DVector           MousePos;
    C2DVector           JoystickPos;
};

static void Destroy(CInputEventPackage* first, CInputEventPackage* last, const SAllocatorTag&)
{
    for (; first != last; ++first)
        _Destroy(first);
}

int main()
{
    static double storage[3 * sizeof(CInputEventPackage) / sizeof(double) + 1];
    CInputEventPackage* packages = (CInputEventPackage*)storage;
    for (int i = 0; i < 3; ++i) {
        new (&packages[i]) CInputEventPackage();
        if (i != 1) {                                   // package 1 keeps an empty (null) vector
            int n = i + 2;
            CInputEvent* events = (CInputEvent*)std::malloc(n * sizeof(CInputEvent));
            packages[i].Events._M_start = events;
            packages[i].Events._M_finish = events + n;
            packages[i].Events._M_end_of_storage = events + n;
        }
    }
    SAllocatorTag tag;
    Destroy(packages, packages + 3, tag);
    bool ok = g_eventDtors == 2 + 4 && g_frees == 2;
    Destroy(packages, packages, tag);
    if (g_eventDtors != 6 || g_frees != 2) ok = false;
    if (!ok) { std::printf("009f6e00_TEST FAIL\n"); return 1; }
    std::printf("009f6e00_TEST PASS\n");
    return 0;
}