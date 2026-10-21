#include <cstdio>
#include <stdlib.h>
struct CListRecordWithWideStringAt24;
namespace std {
template <class T> class _List_base {
public:
    void* _M_node;                                       // sentinel
    ~_List_base() { clear(); if (_M_node) free(_M_node); }
    void clear();                                        // 0x00440770 for CListRecordWithWideStringAt24
};
template <class T> class list : public _List_base<T> {};
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class T> inline void _Destroy(T* first, T* last) { for (; first != last; ++first) _Destroy(first); }
template <class T> class vector {
public:
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
}
class CPolymorphicElement {
public:
    long Value;                                          // +0x04
    virtual ~CPolymorphicElement();
};
class CRecordWithVectorAndList {
public:
    long                                         Fields[7];   // +0x00
    std::vector<CPolymorphicElement>             Elements;    // +0x1c
    std::list<CListRecordWithWideStringAt24>     Records;     // +0x28
    ~CRecordWithVectorAndList();
};
CRecordWithVectorAndList::~CRecordWithVectorAndList()
{
}


#include <new>
static char g_log[16]; static int g_n;
struct CListRecordWithWideStringAt24 {};
template <> void std::_List_base<CListRecordWithWideStringAt24>::clear() { g_log[g_n++] = 'C'; }
CPolymorphicElement::~CPolymorphicElement() { g_log[g_n++] = (char)('0' + Value); }
int main()
{
    CRecordWithVectorAndList* r = (CRecordWithVectorAndList*)malloc(sizeof(CRecordWithVectorAndList));
    r->Records._M_node = malloc(8);
    CPolymorphicElement* e = (CPolymorphicElement*)malloc(2 * sizeof(CPolymorphicElement));
    new (&e[0]) CPolymorphicElement; e[0].Value = 1;
    new (&e[1]) CPolymorphicElement; e[1].Value = 2;
    r->Elements._M_start = e; r->Elements._M_finish = e + 2;
    r->~CRecordWithVectorAndList();
    bool ok = g_n == 3 && g_log[0] == 'C' && g_log[1] == '1' && g_log[2] == '2';
    free(r);
    if (ok) std::printf("00443140_TEST PASS\n");
    return 0;
}