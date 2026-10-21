// CRecordWithVectorAndList::~CRecordWithVectorAndList() -- implicit destructor: free the record list, destroy and free the polymorphic-element vector
// Leaf: ??1CRecordWithVectorAndList@@QAE@XZ
// Retail: Fable.exe @ 0x00443140, 75 bytes, plain `ret`; callers 0x00443BB3 (its ??_G, 0x00443BB0) and 0x00444D77 (the
// CCountedPointer delete function 0x00444D70, referenced from data at 0x00445170). Catalog label
// CEngineScreenEffectDisplacementRenderer::FinishWithDynamicTextures is wrong: the body is a member-wise destructor --
// the list at +0x28 (out-of-line _List_base::clear 0x00440770 over the CWideString-at-+0x24 record, then
// `if (sentinel) free(sentinel)`), then the vector at +0x1c (each 8-byte element destroyed through its virtual destructor,
// `call [vtbl](0)`, then `if (_M_start) free(_M_start)`). Class and element names are behavioural; the real ones are not
// recoverable from the bytes.
// Layout: plain members +0x00..+0x1b, STLport vector<element> +0x1c (12 bytes), list<record> sentinel +0x28.
// Shape: /O2, non-virtual destructor; the element range destroyed through a by-value (first, last) helper so the end stays in a register.
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