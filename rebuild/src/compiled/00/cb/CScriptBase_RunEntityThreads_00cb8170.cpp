// CScriptBase::RunEntityThreads() -- give every active entity script one script frame (over a snapshot of the map) and drop the ones that finished from the live map
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB8170, 173 bytes (`ret`, thiscall).
// Identity: Ego_r.pdb `?RunEntityThreads@CScriptBase@@...`; tail-called from CScriptBase::RunSubThreads 0x00CB8220 (after
//   RunSpawnedFunctions 0x00CB7C40). The manifest label "ScriptEntity_UpdateSpawnedFunctions" is descriptive.
// Callees: vector copy constructor 0x00CBB020 (same-TU body here), CScriptBase::UpdateSpawnedFunction 0x00CB7950,
//   CVectorMap LowerBound 0x00CB9840 / UpperBound 0x00CB98A0, __copy 0x00CBB120, __destroy_aux 0x00CBB1B0, free 0x00BFEA14.
// Shape: the snapshot is a whole CVectorMap copy (16 bytes: that gives retail's 0x18 frame; its dead Dirty copy is
//   dropped), iterated while finished scripts are erased from the live map (Erase = LowerBound, UpperBound, erase).
//   The same-TU vector copy-constructor body keeps the snapshot's start pointer in edi across the final _Destroy.
//   The inlined erase is STLport 4.6's `__copy_ptrs(last, _M_finish, first, _TrivialAss())`; the counted copy
//   0x00CBB120 gets a same-TU stand-in body that never reads its tag, so VC7.1 homes the random_access_iterator_tag
//   temporary on the shared tag byte [esp+17h] with the two __false_type tags (declared only, it took [esp+16h]).
// Leaf: ?RunEntityThreads@CScriptBase@@
void __cdecl operator delete(void* block);                 // 0x00BFE9BC
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
class CCharString {
public:
    void* m_pStringData;
};
struct CCPPointerInfo {                                    // CCountedPointer count block
    long  RefCount;                                        // +0x00
    void  (__fastcall* DeleteFunc)(void* data);            // +0x04
    void* Data;                                            // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                  // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    T* operator->() const { return Data; }
};
class CASuspendableProcess {
public:
    virtual ~CASuspendableProcess();
    bool  Running;                                         // +0x04
    bool  StopRunning;                                     // +0x05
    bool  TerminateUsingExceptions;                        // +0x06
    float TimeProcessSliceStarted;                         // +0x08
    float TimesliceLength;                                 // +0x0c
    unsigned long _pad10;                                  // +0x10
};
class CSpawnedFuncBase : public CASuspendableProcess {
public:
    unsigned long ID;                                      // +0x14
    long          LastScriptFrame;                         // +0x18
    long          CurrentScriptFrame;                      // +0x1c
    CCharString   Name;                                    // +0x20
    CCharString   AssociatedRegionName;                    // +0x24
    bool          FuncPaused;                              // +0x28
    bool          Interrupted;                             // +0x29
    void SetFuncAsInterrupted(bool interrupted) { Interrupted = interrupted; }
};
class CActiveEntityScriptBase : public CSpawnedFuncBase {
};
namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                                    // +0
    _Tp* _M_finish;                                                   // +4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;           // +8
};
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
// Same-TU stand-in body of the out-of-line counted copy 0x00CBB120 (it never reads the tag).
#pragma auto_inline(off)
template <class _InputIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_InputIter __first, _InputIter __last, _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
#pragma auto_inline(on)
// STLport 4.6 vector::erase calls __copy_ptrs directly with its _TrivialAss() tag (the element is not memcpy-able).
template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x00CBB1B0
template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, __first);
}
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&);   // 0x00CBA3E0
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef _Tp* pointer;
    typedef __false_type _TrivialAss;
    vector(const vector<_Tp, _Alloc>& __x);                                 // 0x00CBB020 (out of line)
    ~vector()
    {
        _Destroy(this->_M_start, this->_M_finish);
        if (this->_M_start)
            free(this->_M_start);
    }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};
template <class _Tp> struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp, class _Alloc = allocator<_Tp> >
class list {                                               // STLport 4.6 list: one node pointer (4 bytes)
public:
    _List_node_base<_Tp>* _M_node;
};
}
template <class K, class V> class CKeyPairCompareLess {    // empty
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};
template <class V> class CCompareString {                  // empty
};
template <class K, class V, class C> class CVectorMap {
public:
    typedef _STL::pair<K, V> value_type;
    typedef value_type* iterator;
    _STL::vector<value_type> Vector;                       // +0x00
    C    Compare;                                          // +0x0c
    bool Dirty;                                            // +0x0d
    iterator LowerBound(const K& key);                     // out of line
    iterator UpperBound(const K& key);                     // out of line
    iterator begin() { return Vector.begin(); }
    iterator end() { return Vector.end(); }
    iterator Find(const K& key)
    {
        iterator it = LowerBound(key);
        if (it == end() || key < it->first)
            return end();
        return it;
    }
    void Erase(const K& key)
    {
        iterator lower = LowerBound(key);
        if (lower != end())
            Vector.erase(lower, UpperBound(key));
    }
};
#pragma auto_inline(off)
// Same-TU body of the out-of-line vector copy constructor (0x00CBB020, STLport: allocate size() elements and
// uninitialized-copy them): VC7.1 then sees the snapshot's address is not captured, so its start pointer stays in a
// register across the element destruction at the end, as retail.
namespace _STL {
template <class _Tp, class _Alloc>
__declspec(noinline) vector<_Tp, _Alloc>::vector(const vector<_Tp, _Alloc>& __x)
{
    unsigned int __n = __x._M_finish - __x._M_start;
    this->_M_start = __n ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0;
    this->_M_finish = __uninitialized_copy((const _Tp*)__x._M_start, (const _Tp*)__x._M_finish, this->_M_start, __false_type());
    this->_M_end_of_storage._M_data = this->_M_start + __n;
}
}
// Same-TU bodies of the out-of-line binary searches (landed 0x00CB9840-family / UpperBound 0x00CB98A0): they only read,
// so VC7.1 keeps the map's end pointer loaded before UpperBound in a register for the erase.
template <class K, class V, class C>
__declspec(noinline) typename CVectorMap<K, V, C>::iterator CVectorMap<K, V, C>::LowerBound(const K& key)
{
    iterator first = Vector.begin();
    int count = Vector.end() - Vector.begin();
    while (count > 0) {
        int half = count / 2;
        iterator middle = first + half;
        if (middle->first < key) {
            first = middle + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return first;
}
template <class K, class V, class C>
__declspec(noinline) typename CVectorMap<K, V, C>::iterator CVectorMap<K, V, C>::UpperBound(const K& key)
{
    iterator first = Vector.begin();
    int count = Vector.end() - Vector.begin();
    while (count > 0) {
        int half = count / 2;
        iterator middle = first + half;
        if (!(key < middle->first)) {
            first = middle + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return first;
}
#pragma auto_inline(on)
class CEntityScriptBindingBase;
typedef CVectorMap<unsigned __int64, CCountedPointer<CActiveEntityScriptBase>, CKeyPairCompareLess<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > > CEntityScriptMap;
class CScriptBase {
public:
    virtual ~CScriptBase();
    _STL::list<CCountedPointer<CSpawnedFuncBase> > Threads;                                  // +0x04
    CEntityScriptMap EntityScripts;                                                          // +0x08
    CVectorMap<CCharString, CCountedPointer<CEntityScriptBindingBase>, CCompareString<CCountedPointer<CEntityScriptBindingBase> > > EntityScriptBindings;   // +0x18
    unsigned long     LastWorldUpdateFrame;                                                  // +0x28
    CSpawnedFuncBase* PActiveThread;                                                         // +0x2c
    bool UpdateSpawnedFunction(CSpawnedFuncBase* func);                                      // 0x00CB7950
    void RunEntityThreads();
};

void CScriptBase::RunEntityThreads()
{
    CEntityScriptMap scripts(EntityScripts);
    for (CEntityScriptMap::iterator it = scripts.begin(); it != scripts.end(); ++it)
        if (UpdateSpawnedFunction(it->second.Data))
            EntityScripts.Erase(it->first);
}