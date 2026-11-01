// CStreamingFontDataBank::~CStreamingFontDataBank() -- streaming font bank destructor: restore the vtable, CloseFonts(), then the members (streaming entry counted pointer, file name, font array, state block) and the CBankFileAsync base
// Leaf: ??1CStreamingFontDataBank@@UAE@XZ
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> struct __type_traits<_Tp*> { typedef __true_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy(__first, __last, __first); }
template <class _Tp> class allocator { public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base { public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> { public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}
template <class T> class CArray : public std::vector<T, std::allocator<T> > { };   // Fable's CArray over the STLport vector
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                                          // 0x0099EAE0
};
class CBankFile : public CBaseClassNonCopyable {             // RTTI CBankFile : CBaseClassNonCopyable : CBaseClass; retail sizeof 0x110 (engine/CBankFile.h)
public:
    unsigned char _body[0x110 - 0x04];                       // +0x04 .. +0x10f (engine/CBankFile.h members, flattened there)
};
class CBankFileAsync : public CBankFile {                    // RTTI CBankFileAsync : CBankFile; retail sizeof 0x164
public:
    CBankFileAsync();                                        // 0x009D5F80 ??0CBankFileAsync@@QAE@XZ
    virtual ~CBankFileAsync();                               // 0x009D6490 ??1CBankFileAsync@@UAE@XZ
    unsigned char _body[0x164 - 0x110];                      // +0x110 .. +0x163 (ThreadedFile, LoadingMemoryPool, ... CriticalSection)
};
class CBankStateBlock {
public:
    CBankStateBlock() { }
    virtual ~CBankStateBlock() { }                           // ??1CBankStateBlock@@UAE@XZ (vtable 0x0129C82C store)
    virtual void GetDefault(CBankStateBlock& block) = 0;     // slot 1
    virtual unsigned long CreateCRC() = 0;                   // slot 2
protected:
    virtual void Set(const CBankStateBlock* block) = 0;      // slot 3
};
class CStreamingFontBankStateBlock : public CBankStateBlock {
public:
    struct CStateBlock {
        long FontHeight;                                     // +0x00
        long FontWeight;                                     // +0x04
        long MinSpacing;                                     // +0x08
        bool Italics;                                        // +0x0c
    };
    CStreamingFontBankStateBlock();
    virtual void GetDefault(CBankStateBlock& block);
    virtual unsigned long CreateCRC();
    CStateBlock State;                                       // +0x04
protected:
    virtual void Set(const CBankStateBlock* block);
};
class CStreamingFontDataBankEntry;
class CBankFileAsyncEntry;
class CStreamingFontDataBank : public CBankFileAsync {
public:
    virtual ~CStreamingFontDataBank();
    CStreamingFontBankStateBlock         StateBlock;         // +0x164
    CArray<CStreamingFontDataBankEntry*> Fonts;              // +0x178
    CCharString                          Filename;           // +0x184
    CCountedPointer<CBankFileAsyncEntry> streamingEntry;     // +0x188
private:
    void CloseFonts();                                       // 0x00A5F2F0
};
CStreamingFontDataBank::~CStreamingFontDataBank()
{
    CloseFonts();
}
