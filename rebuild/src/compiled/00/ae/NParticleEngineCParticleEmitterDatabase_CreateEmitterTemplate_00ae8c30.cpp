// NParticleEngine::CParticleEmitterDatabase::CreateEmitterTemplate(long& handle) -- news a default CParticleEmitter, files it in Emitters under the next template handle (insert then re-sort the vector map), hands the handle back and returns the emitter
// Leaf: ?CreateEmitterTemplate@CParticleEmitterDatabase@NParticleEngine@@QAEPAVCParticleEmitter@2@AAJ@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE8C30, 275 bytes, thiscall `ret 4`, returns the new emitter.
// Identity (proven): Ego_r.pdb ?CreateEmitterTemplate@CParticleEmitterDatabase@NParticleEngine@@QAEPAVCParticleEmitter@2@AAJ@Z
//   (donor 0x00AAE5F0, 0x110 bytes, between DeleteAllEmitterTemplates and Load -- the same order and size); the body news
//   a 0x16C-byte CParticleEmitter (constructor 0x00AF0380), wraps it in a CCountedPointer whose delete function is
//   CCountedPointer<NParticleEngine::CParticleEmitter>::DeleteData 0x00AE91D0 (FableWin.pdb name; the body is the virtual
//   deleting-destructor call), inserts it under this+0x40 (Ego_r CurrentHandle, rebased) into this+0x28 (Ego_r Emitters,
//   CVectorMap<long, CCountedPointer<NParticleEngine::CParticleEmitter>, CKeyPairCompareLess<...> >), writes the handle
//   through the long& argument and post-increments it. Called from 0x00AE8E0E (Load). The catalog's
//   "CParticleEmitterManager_EmplaceEmitter" is descriptive and names the wrong class.
// Layout (retail): Emitters +0x28 (Ego_r +0x30: retail CSymbolMap is 8 bytes shorter), EmitterBank +0x38, CurrentHandle +0x40.
// Callees: operator new 0x00BFEA1A, CParticleEmitter::CParticleEmitter 0x00AF0380 (Ego_r ??0CParticleEmitter@NParticleEngine@@QAE@XZ;
//   bound without the namespace -- NAME_FIXES), vector<pair<long, CCountedPointer<CParticleEmitter> > >::_M_insert_overflow
//   0x00AEA440 (landed spelling), std::sort<pair*, CKeyPairCompareLess<...> > 0x00AEA880 (/Gr __fastcall; landed under the
//   invented Sort_SElement12_00aea880 -- NAME_FIXES), operator delete 0x00BFE9BC.
// Shape: CVectorMap::Insert (push_back of a pair temporary, Dirty = true) and CVectorMap::Update (if Dirty: Sort = sort +
//   Dirty = false) inline, as the landed out-of-line COMDATs 0x00AEA6F0 / 0x00AEA8E0 show their bodies. Insert is
//   __forceinline (VC7.1 kept it out of line otherwise; grade 1, 181v275), and the STLport 4.6 _M_insert_overflow body
//   with its __uninitialized_copy / __uninitialized_fill_n / __destroy_aux helpers is defined in the TU (retail emits them
//   here: 0x00AEA440 calls 0x00AE9860 / 0x00AE98A0 / 0x00AE9D50), else the escaped pair temporary is reloaded after every
//   reference-count write (grade 2, 301v275) -- the landed Insert 0x00AEA6F0 needed the same.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
void* __cdecl operator new(unsigned int size);             // 0x00BFEA1A
void __cdecl operator delete(void* p);                     // 0x00BFE9BC
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CCPPointerInfo {
public:
    unsigned long RefCount;                                // +0
    void (__fastcall* DeleteFunc)(void* data);             // +4
    void* Data;                                            // +8
    CCPPointerInfo(void (__fastcall* deleteFunc)(void*), void* data) : RefCount(1), DeleteFunc(deleteFunc), Data(data) { }
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                  // +0
    CCPPointerInfo* Info;                                  // +4
    static void __fastcall DeleteData(void* data);         // 0x00AE91D0 (CParticleEmitter)
    CCountedPointer(T* data) : Data(data) { Info = data ? new CCPPointerInfo(DeleteData, data) : 0; }
    CCountedPointer(const CCountedPointer<T>& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
};

namespace NParticleEngine {
class CParticleEmitter {
public:
    CParticleEmitter();                                    // 0x00AF0380
    char Body[0x16C];
};
}

namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __p) { __p->~_Tp(); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)   // 0x00AE9860
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)   // 0x00AE98A0
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(__cur, __x);
    return __cur;
}
template <class _ForwardIter>
void __destroy_aux(_ForwardIter __first, _ForwardIter __last, const __false_type&)   // 0x00AE9D50
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x0
    _Tp* _M_finish;                                        // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x8
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    _Tp* begin() { return this->_M_start; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp* end() { return this->_M_finish; }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);         // 0x00AEA440
    void _M_clear()
    {
        __destroy_aux(this->_M_start, this->_M_finish, __false_type());
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
};
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                                             unsigned int __fill_len, bool __atend)   // STLport 4.6 _vector.c
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);
    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
template <class _RandomAccessIter, class _Compare>
void sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00AEA880
}   // namespace std

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
template <class K, class V, class C = CKeyPairCompareLess<K, V> > class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    std::vector<value_type> Data;                          // +0x0
    C    Compare;                                          // +0xC
    bool Dirty;                                            // +0xD
    __forceinline void Insert(const K& key, const V& value)
    {
        Data.push_back(value_type(key, value));
        Dirty = true;
    }
    void Sort()
    {
        std::sort(Data.begin(), Data.end(), Compare);
        Dirty = false;
    }
    void Update()
    {
        if (Dirty)
            Sort();
    }
};

namespace NParticleEngine {
class CParticleEmitterDatabase {
public:
    CParticleEmitter* CreateEmitterTemplate(long& handle);
    char SymbolMapEtc[0x28];                               // +0x00 SymbolMap, UsingSymbolMap
    CVectorMap<long, CCountedPointer<CParticleEmitter> > Emitters;   // +0x28
    CCountedPointer<CParticleEmitter> EmitterBankStandIn;  // +0x38
    long CurrentHandle;                                    // +0x40
};

CParticleEmitter* CParticleEmitterDatabase::CreateEmitterTemplate(long& handle)
{
    CParticleEmitter* emitter = new CParticleEmitter;
    CCountedPointer<CParticleEmitter> pointer(emitter);
    Emitters.Insert(CurrentHandle, pointer);
    Emitters.Update();
    handle = CurrentHandle;
    ++CurrentHandle;
    return emitter;
}
}