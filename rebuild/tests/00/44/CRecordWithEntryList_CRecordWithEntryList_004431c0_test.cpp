// test for 004431c0 implicit list-owner destructor
#include <stdio.h>
#include <string.h>
#include <new>
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
class CRecordBase {
public:
    ~CRecordBase();                                      // 0x00442280
    unsigned char _body[0x64];
};
class CRecordWithEntryList : public CRecordBase {
public:
    std::list<CListEntry> Entries;                       // +0x64
};
void DestroyRecordWithEntryList(CRecordWithEntryList* p)  // emission helper, not a retail function
{
    p->CRecordWithEntryList::~CRecordWithEntryList();
}

static void* g_cleared; static int g_base; static int g_order;
template <> void std::_List_base<CListEntry>::clear() { g_cleared = this; g_order = g_order * 10 + 1; }
CRecordBase::~CRecordBase() { ++g_base; g_order = g_order * 10 + 2; }
CWideString::~CWideString() {}
int main()
{
    static double buf[0x20];
    CRecordWithEntryList* r = (CRecordWithEntryList*)buf;
    r->Entries._M_node = (std::_List_node_base<CListEntry>*)malloc(8);
    DestroyRecordWithEntryList(r);
    int ok = g_cleared == (char*)buf + 0x64 && g_base == 1 && g_order == 12 && sizeof(CRecordWithEntryList) == 0x68;
    r->Entries._M_node = 0; g_order = 0;
    DestroyRecordWithEntryList(r);
    ok &= g_base == 2 && g_order == 12;
    printf(ok ? "004431c0_TEST PASS\n" : "004431c0_TEST FAIL\n");
    return ok ? 0 : 1;
}