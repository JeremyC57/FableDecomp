// test for 004431f0 two-list record destructor
#include <stdio.h>
#include <stdlib.h>
class CWideString {
public:
    void* PData;
    ~CWideString();                                      // 0x0099B510
};
struct CListEntry {
    long        Key[4];                                  // +0x00
    CWideString Name;                                    // +0x10
};
namespace std {
template <class T> struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class T> class _List_base {
public:
    _List_node_base<T>* _M_node;                         // sentinel
    ~_List_base() { clear(); if (_M_node) free(_M_node); }
    void clear();                                        // 0x004407B0
};
template <class T> class list : public _List_base<T> {};
}
class CPodList {                                         // STLport list of plain nodes
public:
    void* _M_node;                                       // sentinel
    ~CPodList();                                         // 0x0043FD40
};
class CRecordBase {
public:
    ~CRecordBase();                                      // 0x00442280
    unsigned char _body[0x64];
};
class CRecordWithTwoLists : public CRecordBase {
public:
    std::list<CListEntry> Entries;                       // +0x64
    CPodList              Others;                        // +0x68
};
void DestroyRecordWithTwoLists(CRecordWithTwoLists* p)  // emission helper, not a retail function
{
    p->CRecordWithTwoLists::~CRecordWithTwoLists();
}

static void* g_cleared; static void* g_pod; static int g_base; static int g_order;
template <> void std::_List_base<CListEntry>::clear() { g_cleared = this; g_order = g_order * 10 + 2; }
CPodList::~CPodList() { g_pod = this; g_order = g_order * 10 + 1; }
CRecordBase::~CRecordBase() { ++g_base; g_order = g_order * 10 + 3; }
CWideString::~CWideString() {}
int main()
{
    static double buf[0x20];
    CRecordWithTwoLists* r = (CRecordWithTwoLists*)buf;
    r->Entries._M_node = (std::_List_node_base<CListEntry>*)malloc(8);
    DestroyRecordWithTwoLists(r);
    int ok = g_pod == (char*)buf + 0x68 && g_cleared == (char*)buf + 0x64 && g_base == 1 && g_order == 123 && sizeof(CRecordWithTwoLists) == 0x6c;
    r->Entries._M_node = 0; g_order = 0;
    DestroyRecordWithTwoLists(r);
    ok &= g_base == 2 && g_order == 123;
    printf(ok ? "004431f0_TEST PASS\n" : "004431f0_TEST FAIL\n");
    return ok ? 0 : 1;
}