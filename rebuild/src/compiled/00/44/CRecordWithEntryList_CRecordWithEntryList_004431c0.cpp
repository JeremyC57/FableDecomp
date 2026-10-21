// CRecordWithEntryList::~CRecordWithEntryList() -- destroy the entry list at +0x64 (clear, free the sentinel), then the out-of-line base destructor
// Leaf: ??1CRecordWithEntryList
// Retail: Fable.exe @ 0x004431C0, 38 bytes, unreferenced; 0x00443190 and 0x004431C0 are byte-identical. Catalog label
// CTCAssassinRush::FinishMove is wrong: the body is an implicit destructor -- STLport ~_List_base on the member at +0x64
// (out-of-line clear 0x004407B0, which frees each node after ~CWideString on node +0x18 = value +0x10, then
// `if (sentinel) free(sentinel)` through 0x00BFEA14) and a tail jump to the base destructor 0x00442280 (vectors at
// +0x30/+0x3c, ...). The same inline pattern appears in the unreferenced deleting destructors 0x00443BD0..0x00443CA0
// and in 0x00444D90/0x00444DD0 (referenced only from code at 0x004451E0/0x00445250), all over the same base, so this is
// a family of list-owning records; the real class names are not recoverable from the bytes, hence the behavioural name.
// Layout: base 0x64 bytes (destructor out of line), std::list<entry> sentinel pointer at +0x64; entry = 16 bytes then a
// CWideString (+0x10).
// Shape: the destructor is implicit (no vptr store); DestroyRecordWithEntryList's qualified call emits it out of line.
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