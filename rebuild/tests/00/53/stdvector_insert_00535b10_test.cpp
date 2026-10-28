// Behaviour test for 0x00535B10 std::vector<CCountedPointer<NUISystem::CComponent> >::insert(iterator, const T&)
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_deleted = 0;
class CCPPointerInfo { public:
    unsigned int RefCount;
    void (*DeleteFunc)(void* data);
    void* Data;
};
static void DeleteComponent(void*) { ++g_deleted; }
struct CComponent { int Id; };

struct CCountedPointer {
    CComponent* Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
    void ReleaseData()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    void ShareData(CComponent* d, CCPPointerInfo* i)
    {
        if (Info != i) {
            ReleaseData();
            Data = d;
            Info = i;
            if (i)
                ++i->RefCount;
        }
    }
    CCountedPointer& operator=(const CCountedPointer& o) { ShareData(o.Data, o.Info); return *this; }
};

struct Vector {
    CCountedPointer* _M_start;
    CCountedPointer* _M_finish;
    CCountedPointer* _M_end_of_storage;
    CCountedPointer* begin() { return _M_start; }
    CCountedPointer* end() { return _M_finish; }
    void _M_insert_overflow(CCountedPointer* pos, const CCountedPointer& x)
    {
        unsigned int old = (unsigned int)(_M_finish - _M_start);
        unsigned int len = old + (old > 1 ? old : 1);
        CCountedPointer* ns = (CCountedPointer*)std::malloc(len * sizeof(CCountedPointer));
        CCountedPointer* nf = ns;
        for (CCountedPointer* p = _M_start; p != pos; ++p, ++nf) new (nf) CCountedPointer(*p);
        new (nf) CCountedPointer(x); ++nf;
        for (CCountedPointer* p = pos; p != _M_finish; ++p, ++nf) new (nf) CCountedPointer(*p);
        for (CCountedPointer* p = _M_start; p != _M_finish; ++p) p->~CCountedPointer();
        std::free(_M_start);
        _M_start = ns; _M_finish = nf; _M_end_of_storage = ns + len;
    }
    // the function under test, as written in the source
    CCountedPointer* insert(CCountedPointer* position, const CCountedPointer& x)
    {
        unsigned int n = (unsigned int)(position - begin());
        if (_M_finish != _M_end_of_storage) {
            if (position == end()) {
                new (_M_finish) CCountedPointer(x);
                ++_M_finish;
            } else {
                new (_M_finish) CCountedPointer(*(_M_finish - 1));
                ++_M_finish;
                CCountedPointer x_copy = x;
                CCountedPointer* last = _M_finish - 2;
                CCountedPointer* result = _M_finish - 1;
                for (int k = (int)(last - position); k > 0; --k)
                    *--result = *--last;
                *position = x_copy;
            }
        } else
            _M_insert_overflow(position, x);
        return begin() + n;
    }
};

int main()
{
    CComponent a = { 1 }, b = { 2 }, c = { 3 };
    CCPPointerInfo* ia = new CCPPointerInfo; ia->RefCount = 1; ia->DeleteFunc = DeleteComponent; ia->Data = &a;
    CCPPointerInfo* ib = new CCPPointerInfo; ib->RefCount = 1; ib->DeleteFunc = DeleteComponent; ib->Data = &b;
    CCPPointerInfo* ic = new CCPPointerInfo; ic->RefCount = 1; ic->DeleteFunc = DeleteComponent; ic->Data = &c;
    Vector v;
    v._M_start = (CCountedPointer*)std::malloc(4 * sizeof(CCountedPointer));
    v._M_finish = v._M_start; v._M_end_of_storage = v._M_start + 4;
    CCountedPointer* raw = (CCountedPointer*)std::malloc(3 * sizeof(CCountedPointer));
    raw[0].Data = &a; raw[0].Info = ia;
    raw[1].Data = &b; raw[1].Info = ib;
    raw[2].Data = &c; raw[2].Info = ic;
    CCountedPointer* r = v.insert(v.end(), raw[0]);           // append
    if (r != v._M_start || v._M_finish - v._M_start != 1 || ia->RefCount != 2) { std::printf("00535b10_TEST FAIL\n"); return 1; }
    r = v.insert(v.begin(), raw[1]);                           // shift: b, a
    if (r != v._M_start || v._M_start[0].Data != &b || v._M_start[1].Data != &a) { std::printf("00535b10_TEST FAIL\n"); return 1; }
    if (ia->RefCount != 2 || ib->RefCount != 2) { std::printf("00535b10_TEST FAIL\n"); return 1; }
    r = v.insert(v.begin() + 1, raw[2]);                       // b, c, a
    if (r != v._M_start + 1 || v._M_start[1].Data != &c || v._M_start[2].Data != &a || ic->RefCount != 2) { std::printf("00535b10_TEST FAIL\n"); return 1; }
    v.insert(v.end(), raw[2]);                                 // b, c, a, c (full)
    r = v.insert(v.begin(), raw[0]);                           // overflow: a, b, c, a, c
    if (v._M_finish - v._M_start != 5 || r != v._M_start || v._M_start[0].Data != &a || v._M_start[4].Data != &c) { std::printf("00535b10_TEST FAIL\n"); return 1; }
    if (ia->RefCount != 3 || ic->RefCount != 3 || g_deleted != 0) { std::printf("00535b10_TEST FAIL\n"); return 1; }
    std::printf("00535b10_TEST PASS\n");
    return 0;
}