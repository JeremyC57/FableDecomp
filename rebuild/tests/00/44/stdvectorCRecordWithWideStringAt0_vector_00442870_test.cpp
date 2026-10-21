// test for 00442870 vector dtor
#include <stdio.h>
#include <new>
#include <stdlib.h>
class CWideString {
public:
    void* PData;
    ~CWideString();                                      // 0x0099B510
};
struct CRecordWithWideStringAt0 {
    CWideString Text;                                    // +0x00
    long        Data[4];                                 // +0x04
};
namespace std {
struct __false_type {};
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class ForwardIter>
inline void __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template <class T> inline void _Destroy(T* first, T* last) { __destroy_aux(first, last, __false_type()); }
template <class T> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~_Vector_base() { if (_M_start) free(_M_start); }
};
template <class T> class vector : public _Vector_base<T> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
template class vector<CRecordWithWideStringAt0>;
}

static int g_n; static void* g_seen[8];
CWideString::~CWideString() { g_seen[g_n++] = this; }
int main()
{
    {
        std::vector<CRecordWithWideStringAt0>* v = (std::vector<CRecordWithWideStringAt0>*)malloc(sizeof *v);
        v->_M_start = (CRecordWithWideStringAt0*)malloc(3 * sizeof(CRecordWithWideStringAt0));
        v->_M_finish = v->_M_start + 3; v->_M_end_of_storage = v->_M_finish;
        CRecordWithWideStringAt0* s = v->_M_start;
        v->~vector();
        int ok = g_n == 3 && g_seen[0] == &s[0] && g_seen[2] == &s[2] && sizeof(CRecordWithWideStringAt0) == 0x14;
        v->_M_start = 0; v->_M_finish = 0; v->~vector();
        ok = ok && g_n == 3;
        printf(ok ? "00442870_TEST PASS\n" : "00442870_TEST FAIL\n");
    }
    return 0;
}