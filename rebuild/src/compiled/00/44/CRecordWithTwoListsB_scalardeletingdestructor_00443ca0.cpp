// CRecordWithTwoListsB::`scalar deleting destructor'(unsigned int flags) -- implicit destructor of the 0x00443190 record family, then free the object when bit 0 is set
// Leaf: ??_GCRecordWithTwoListsB@@QAEPAXI@Z
// Retail: Fable.exe @ 0x00443CA0, 67 bytes, `ret 4`; no code or data references (orphaned COMDAT). Catalog label
// CEngineSubPrimitiveText::`scalar_deleting_destructor' is wrong: the body is the inlined implicit destructor of the
// two-list record (as 0x004431F0): the +0x68 list through its out-of-line destructor (0x0043FD80), the CWideString-entry
// list at +0x64 (out-of-line clear 0x004407B0, `if (sentinel) free(sentinel)`), the base destructor 0x00442280; then
// `if (flags & 1) operator delete(this)` (0x00BFE9BC). the 0x00443C50 twin uses POD-list destructor 0x0043FD40; class names are behavioural, the real ones are not
// recoverable from the bytes.
// Layout: base 0x64 bytes, list<entry> sentinel +0x64 (entry = 16 bytes + CWideString), second list +0x68.
// Shape: non-virtual destructor; the `delete p` helper makes VC7.1 emit ??_G.
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
class CPodListB {                                        // STLport list of plain nodes
public:
    void* _M_node;                                       // sentinel
    ~CPodListB();                                         // 0x0043FD80
};
class CRecordBase {
public:
    ~CRecordBase();                                      // 0x00442280
    unsigned char _body[0x64];
};
class CRecordWithTwoListsB : public CRecordBase {
public:
    std::list<CListEntry> Entries;                       // +0x64
    CPodListB              Others;                        // +0x68
};
void DeleteCRecordWithTwoListsB(CRecordWithTwoListsB* p)                               // emission helper, not a retail function
{
    delete p;
}