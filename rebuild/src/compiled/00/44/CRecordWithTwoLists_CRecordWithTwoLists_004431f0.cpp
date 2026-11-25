// CRecordWithTwoLists::~CRecordWithTwoLists() -- destroy the second list (+0x68), then the entry list (+0x64), then the out-of-line base destructor
// Leaf: ??1CRecordWithTwoLists
// Retail: Fable.exe @ 0x004431F0, 46 bytes, unreferenced. Catalog label CTCAssassinRush::FinishMove is wrong: the body
// is an implicit destructor of the 0x00443190 family -- members in reverse order: the list at +0x68 through its
// out-of-line STLport ~_List_base (0x0043FD40: free every node and the sentinel through 0x00BFEA14; catalog-labelled
// CGuiControlTreePane::SortTreeRecursively), then the CWideString-entry list at +0x64 with its inlined ~_List_base
// (out-of-line clear 0x004407B0, `if (sentinel) free(sentinel)`), then a tail jump to the base destructor 0x00442280.
// The real class name is not recoverable from the bytes, hence the behavioural name (as 0x00443190).
// Layout: base 0x64 bytes, list<entry> sentinel pointer +0x64 (entry = 16 bytes + CWideString), second list +0x68
// (its element type does not reach the bytes; its destructor is spelled as a declared out-of-line one).
// Shape: the destructor is implicit (no vptr store); DestroyRecordWithTwoLists's qualified call emits it out of line.
// Naming (NF451, 2026-11-25; r450b done through the shared header): this record derives from NPlayerGui::CStatBase
//   (engine/NPlayerGui_CStatBase.h, add-only no-containers view: +0x00..+0x30, the vptr and the scalar members), so the implicit
//   base-destructor call names 0x00442280 ??1CStatBase@NPlayerGui@@QAE@XZ (FableWin.pdb only, r449c best guess); was ??1CRecordBase@@QAE@XZ.
//   The derived model pads +0x30..+0x64 itself (the base's two STLport vectors +0x30/+0x3C, left out of the view, and the
//   derived scalar members).
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_GUIMODE
#define FABLE_RETAIL_NPLAYERGUI_CSTATBASE_NO_CONTAINERS
#include "engine/NPlayerGui_CStatBase.h"                // ~CStatBase 0x00442280 (FableWin.pdb ??1CStatBase@NPlayerGui@@QAE@XZ)
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
class CRecordWithTwoLists : public NPlayerGui::CStatBase {
public:
    unsigned char _body[0x64 - 0x30];                    // +0x30 (base vectors, derived scalars)
    std::list<CListEntry> Entries;                       // +0x64
    CPodList              Others;                        // +0x68
};
void DestroyRecordWithTwoLists(CRecordWithTwoLists* p)  // emission helper, not a retail function
{
    p->CRecordWithTwoLists::~CRecordWithTwoLists();
}