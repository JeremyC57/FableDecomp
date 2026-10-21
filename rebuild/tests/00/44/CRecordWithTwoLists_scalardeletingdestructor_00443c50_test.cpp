#include <cstdio>
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
class CPodList {                                        // STLport list of plain nodes
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
void DeleteCRecordWithTwoLists(CRecordWithTwoLists* p)                               // emission helper, not a retail function
{
    delete p;
}

#include <new>
static char g_log[8]; static int g_n = 0;
CWideString::~CWideString() {}
template <class T> void std::_List_base<T>::clear() { g_log[g_n++] = 'C'; }
CPodList::~CPodList() { g_log[g_n++] = 'O'; }
CRecordBase::~CRecordBase() { g_log[g_n++] = 'B'; }
int main()
{
    CRecordWithTwoLists* p = new CRecordWithTwoLists;
    p->Entries._M_node = (std::_List_node_base<CListEntry>*)malloc(8);
    DeleteCRecordWithTwoLists(p);
    bool ok = g_n == 3 && g_log[0] == 'O' && g_log[1] == 'C' && g_log[2] == 'B';
    if (ok) std::printf("00443c50_TEST PASS\n");
    return 0;
}