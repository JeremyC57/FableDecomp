// CSoundManagerLH::~CSoundManagerLH() -- Lionhead-audio sound manager destructor: restore the CSoundManagerLH vtable, clear the base bank map, shut the sound system down (its slot 13, pointer cleared), then the members (name map, name-pair map, sound list) and the CASoundManager base
// Leaf: ??1CSoundManagerLH@@UAE@XZ
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
extern "C" void __cdecl free(void* block);      // 0x00BFEA14
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                             // 0x0099EAE0
};
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> struct __type_traits<_Tp*> { typedef __true_type has_trivial_destructor; };
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
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy(__first, __last, __first); }
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
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc>
class _List_base { public:
    _STLP_alloc_proxy<_List_node_base*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;
    ~_List_base() { clear(); _M_node.deallocate((_List_node<_Tp>*)_M_node._M_data, 1); }
    void clear();
};
template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : protected _List_base<_Tp, _Alloc> { public:
};
}
template <class K, class V> class CKeyPairCompareLess { public: };
template <class K, class V, class C = CKeyPairCompareLess<K, V> > class CVectorMap { public:
    std::vector<std::pair<K, V> > Data;         // +0x00
    C    Compare;                               // +0x0c
    bool Dirty;                                 // +0x0d
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
    virtual ~CASoundManager();                  // 0x00991880
    void ClearBanks_009918f0();                 // 0x009918F0
};
class C3DVector { public:
    float x, y, z;
};
class CSoundSystem {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual void Shutdown_00a3edf7();           // slot 13 (+0x34)
};
class CSoundSystemReset_00a3eb20;
class CSoundObjectLH;
class CSoundBankLH;
class CSoundManagerLH : public CASoundManager { public:
    CSoundSystem*               m_pSoundSystem; // +0x40
    CSoundSystemReset_00a3eb20* m_pMain;        // +0x44
    long                        m_48;           // +0x48
    std::list<CSoundObjectLH*>  Sounds;         // +0x4c
    long                        m_nextEmitterID;   // +0x50
    CVectorMap<std::pair<CCharString, CCharString>, CSoundBankLH*> NamePairMap;   // +0x54
    CVectorMap<CCharString, CSoundBankLH*>      NameMap;        // +0x64
    C3DVector                   m_vector;       // +0x74
    virtual ~CSoundManagerLH();
};
CSoundManagerLH::~CSoundManagerLH()
{
    ClearBanks_009918f0();
    if (m_pSoundSystem) {
        m_pSoundSystem->Shutdown_00a3edf7();
        m_pSoundSystem = 0;
    }
}
