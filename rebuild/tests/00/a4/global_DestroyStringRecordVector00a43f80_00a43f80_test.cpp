// Behaviour test for 0x00A43F80 Destroy_StringRecordVector_00a43f80: destroy a range of vectors of string records
// (each record's CCharString destructor runs, each storage block is freed).
#include <cstdio>
#include <cstdlib>
#include <new>

struct __false_type { __false_type() { } };
static int g_stringDtors = 0;
static int g_frees = 0;
static void TestFree(void* p) { if (p) { ++g_frees; std::free(p); } }

class CCharString { public:
    const char* m_pStringData;
    ~CCharString() { ++g_stringDtors; }
};
struct SStringRecord12 { CCharString m_name; long m_4; long m_8; };
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T> inline void _Destroy(T* first, T* last) { for (; first != last; ++first) _Destroy(first); }
template <class T> struct _Vector_base {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    _Vector_base() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~_Vector_base() { if (_M_start) TestFree(_M_start); }
};
struct SStringRecordVector : _Vector_base<SStringRecord12> {
    ~SStringRecordVector() { _Destroy(this->_M_start, this->_M_finish); }
};

static void Destroy(SStringRecordVector* first, SStringRecordVector* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(first);
}

int main()
{
    static double storage[4 * sizeof(SStringRecordVector) / sizeof(double) + 1];
    SStringRecordVector* vectors = (SStringRecordVector*)storage;
    for (int i = 0; i < 4; ++i) {
        new (&vectors[i]) SStringRecordVector();
        if (i != 2) {
            int n = i + 1;
            SStringRecord12* records = (SStringRecord12*)std::malloc(n * sizeof(SStringRecord12));
            vectors[i]._M_start = records;
            vectors[i]._M_finish = records + n;
            vectors[i]._M_end_of_storage = records + n;
        }
    }
    __false_type tag;
    Destroy(vectors, vectors + 4, tag);
    bool ok = g_stringDtors == 1 + 2 + 4 && g_frees == 3;
    Destroy(vectors + 1, vectors + 1, tag);
    if (g_stringDtors != 7 || g_frees != 3) ok = false;
    if (!ok) { std::printf("00a43f80_TEST FAIL\n"); return 1; }
    std::printf("00a43f80_TEST PASS\n");
    return 0;
}