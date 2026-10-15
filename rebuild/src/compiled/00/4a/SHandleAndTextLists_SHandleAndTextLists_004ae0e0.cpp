// SHandleAndTextLists::~SHandleAndTextLists() -- destructor of a non-polymorphic record holding two STLport vectors: 16-byte text entries (+0x14) and 8-byte polymorphic handles (+8)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AE0E0, 99 bytes.
// Leaf: ??1SHandleAndTextLists
// Label correction: the catalog's "CMiniMapDisplay::ClearPrimitiveHandles" is propagated; this body is
// the out-of-line destructor of a stack record (callers 0x006E5990 / 0x006E6280 / 0x006E6DD0 destroy
// three of them, 0x38 bytes apart, in conversation code); the real class name is not known, so the
// record is named descriptively.
// Shape: members destroyed in reverse order: the text-entry vector (+0x14: each entry's CCharString at
// +8 destroyed by 0x0099EAE0, then the storage freed with free 0x00BFEA14 when non-null), then the
// handle vector (+8: each element's virtual destructor called explicitly, `push 0 ; call [eax]`, then
// the storage freed). STLport _Destroy(first, last) takes its bounds by value (end cached in edi).
extern "C" void __cdecl free(void* p);              // 0x00BFEA14

struct CCharString {
    void* m_pData;
    ~CCharString();                                 // 0x0099EAE0
};

struct CPolyHandle {                                // 8 bytes: vptr + item
    virtual ~CPolyHandle();
    void* m_pItem;
};

struct STextEntry {                                 // 16 bytes
    long        m_a;                                // +0
    long        m_b;                                // +4
    CCharString m_text;                             // +8
    long        m_c;                                // +0xc
};

template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}

template <class T> struct CVector {                 // STLport vector: start / finish / end_of_storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CVector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};

struct SHandleAndTextLists {
    long                 m_owner;                   // +0x00
    long                 m_id;                      // +0x04
    CVector<CPolyHandle> m_handles;                 // +0x08
    CVector<STextEntry>  m_texts;                   // +0x14
    ~SHandleAndTextLists();
};

SHandleAndTextLists::~SHandleAndTextLists()
{
}