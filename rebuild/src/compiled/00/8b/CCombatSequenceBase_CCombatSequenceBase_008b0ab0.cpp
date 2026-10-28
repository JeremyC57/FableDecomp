// CCombatSequenceBase::CCombatSequenceBase(CThingCreatureBase& creature, CThingCreatureBase& target) -- store the creature and target, clear the action vector and reset the sequence state
// Leaf: ??0CCombatSequenceBase@@QAE@AAVCThingCreatureBase@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008B0AB0, 96 bytes, thiscall, `ret 8`.
// Identity (proven): Ego_r.pdb ??0CCombatSequenceBase@@QAE@AAVCThingCreatureBase@@0@Z (the class's only constructor; donor
//   0x008EC040, after ??1 0x008EBFF0 as retail 0x008B0AB0 follows the class's other members); it stores vtable 0x01278FEC
//   (RTTI .?AVCCombatSequenceBase@@, whose slot 0 is the landed ??_G 0x008B3430) and the two references as the Creature /
//   Target pointers (+0x1C / +0x20, Ego_r +0x20 / +0x24 minus 4 for the STLport vector).
// Callees: std::__copy<CCombatAction*, CCombatAction*, int> 0x008B25C0 (random-access copy loop; landed as
//   Move_CountedPointer) and std::__destroy_aux<CCombatAction*> 0x008B3010 (landed with the ??_G 0x008B3430 model) -- the
//   inlined STLport vector::clear() -> erase(begin(), end()) -> __copy_ptrs + _Destroy.
// Layout: the landed ??_G 0x008B3430 model (Ego_r.pdb CCombatSequenceBase with the retail 12-byte STLport vector).
// Shape: both out-of-line helpers have their real bodies in the TU under auto_inline(off) + noinline, so VC7.1 homes the
//   empty tag temporaries in the dead `creature` argument slot as retail does (GOTCHAS 24l); the vector is STLport's real
//   _Vector_base with the _STLP_alloc_proxy end-of-storage member built from a default allocator argument -- with a plain
//   pointer third member VC7.1 schedules `push edi` before the three zero stores instead of between the second and third.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

class CCPPointerInfo { public:                          // Ego_r.pdb, 12 bytes
    unsigned long RefCount;                             // +0x00
    void (__fastcall* DeleteFunc)(void*);               // +0x04
    void* Data;                                         // +0x08
};

template <class T>
class CCountedPointer {                                 // Ego_r.pdb, 8 bytes
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        if (Info != other.Info) {
            Release();
            Data = other.Data;
            Info = other.Info;
            if (Info)
                ++Info->RefCount;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        Release();
        Data = 0;
        Info = 0;
    }
};

class CCombatActionBase;
class CThingCreatureBase;

class CCombatAction {                                   // Ego_r.pdb, 8 bytes
public:
    CCountedPointer<CCombatActionBase> CreatureAction;  // +0x00
};

namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp>
inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

#pragma auto_inline(off)
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)   // 0x008B3010
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                   const random_access_iterator_tag&, _Distance*)                            // 0x008B25C0
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
#pragma auto_inline(on)

template <class _Tp>
inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator { public: allocator() { } allocator(const allocator&) { } };
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;  // +0x08
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    __forceinline ~_Vector_base() { if (_M_start != 0) free(_M_start); }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    __forceinline ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};
}

class CCombatSequenceBase {                             // vtable 0x01278FEC
public:
    CCombatSequenceBase(CThingCreatureBase& creature, CThingCreatureBase& target);
    virtual __forceinline ~CCombatSequenceBase() { }
    virtual void PureSlot1() = 0;                       // _purecall in the retail vtable
    virtual void PureSlot2() = 0;                       // _purecall in the retail vtable

    bool                         Running;               // +0x04
    long                         NoAvailableActions;    // +0x08
    long                         CurrentActionIndex;    // +0x0C
    std::vector<CCombatAction>   CombatActions;         // +0x10
    CThingCreatureBase*          Creature;              // +0x1C
    CThingCreatureBase*          Target;                // +0x20
    bool                         CreateCalled;          // +0x24
};

CCombatSequenceBase::CCombatSequenceBase(CThingCreatureBase& creature, CThingCreatureBase& target)
    : Creature(&creature), Target(&target)
{
    CombatActions.clear();
    NoAvailableActions = 0;
    CurrentActionIndex = 0;
    Running = false;
    CreateCalled = false;
}