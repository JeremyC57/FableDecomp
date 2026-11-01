// CASoundManager::~CASoundManager() -- sound manager base destructor
// Leaf: ??1CASoundManager@@UAE@XZ
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
extern "C" void __cdecl free(void* block);      // 0x00BFEA14
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                             // 0x0099EAE0
};
class CSoundBank;
class CSoundBankData;
class CSoundBankRecord { public:
    CCountedPointer<CSoundBank>     Bank;       // +0x00
    CCountedPointer<CSoundBankData> BankData;   // +0x08
    CCharString Name0;                          // +0x10
    CCharString Name1;                          // +0x14
    CCharString Name2;                          // +0x18
    CCharString Name3;                          // +0x1c
    ~CSoundBankRecord();                        // 0x00992510
};
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
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
template <class K, class V> class CKeyPairCompareLess { public: };
template <class K, class V, class C = CKeyPairCompareLess<K, V> > class CVectorMap { public:
    std::vector<std::pair<K, V> > Data;         // +0x00
    C    Compare;                               // +0x0c
    bool Dirty;                                 // +0x0d
};
class CResourceBank;
class CASoundManager : public CBaseClassNonCopyable { public:
    long          Setting08;                    // +0x04
    long          Setting0C;                    // +0x08
    float         Scale;                        // +0x0c
    float         InverseScale;                 // +0x10
    long          Setting04;                    // +0x14
    long          Setting00;                    // +0x18
    float         Volume;                       // +0x1c
    bool          Flag;                         // +0x20
    bool          Unknown21[3];                 // +0x21
    bool          Enabled24;                    // +0x24
    bool          Enabled25;                    // +0x25
    bool          Enabled26;                    // +0x26
    CCountedPointer<CResourceBank> ResourceBank;   // +0x28
    CVectorMap<long, CSoundBankRecord> Banks;   // +0x30
    virtual ~CASoundManager();
};
CASoundManager::~CASoundManager()
{
}
