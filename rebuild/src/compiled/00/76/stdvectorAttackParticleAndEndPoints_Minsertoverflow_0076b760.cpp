// std::vector<AttackParticleAndEndPoints>::_M_insert_overflow(AttackParticleAndEndPoints* position, const AttackParticleAndEndPoints& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport growth path: reallocate to old_size + max(old_size, fill_len), copy the prefix, construct fill_len copies of x, copy the suffix, destroy and free the old block
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076B760, 227 bytes (+int3 pad to 240), `ret 14h`.
// Callers: 0x00767ABA / 0x00767CC5 (CTCObjectAugmentations::AttachAugmentationParticles 0x007677E0, the inline
//   push_back) and 0x0076BCA2 (the out-of-line push_back 0x0076BC70).
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (STLport malloc allocator), __uninitialized_copy 0x007696E0 (twice),
//   AttackParticleAndEndPoints copy constructor 0x00769040 (`_Construct` for fill_len == 1), __uninitialized_fill_n
//   0x00769750, __destroy_aux 0x0076A2A0 (the old elements, from `_M_clear`).
// Identity: Ego_r.pdb CTCObjectAugmentations::AttackParticleEffects is a vector<vector<AttackParticleAndEndPoints> >;
//   the 0x18 stride (`lea eax,[ebx+ebx*2] ; shl eax,3`) and the callees' element (two CCharStrings, two longs,
//   CIntelligentPointer<CThing>) are AttackParticleAndEndPoints. The catalog's "Vector_InsertRange_MoveAndFill" is
//   a descriptive auto label.
// Shape: STLport 4.x `_M_insert_overflow(pos, x, __false_type, fill_len, atend)` verbatim (exceptions off):
//   `(max)(old_size, fill_len)` takes references, so old_size gets the `push ecx` frame word; every empty
//   __false_type temporary is homed on the `atend` argument slot (`lea ..,[esp+28h]`); `_M_clear()` destroys the
//   old range and frees it (`if (p) free(p)`), `_M_set` stores finish, start, end of storage. The temporaries land on
//   the atend slot only when the tag-taking callees have same-TU auto_inline(off) bodies that never touch the tag
//   (GOTCHAS 0x4ADB50); a declared-only set gives an extra 4-byte tag frame word.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CThing;

class CCharString {
public:
    void* PStringData;                                      // +0x00
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                        // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
};

struct AttackParticleAndEndPoints {                         // 0x18 (Ego_r.pdb)
    CCharString StartDummy;                                 // +0x00 (names as the landed 0x00769040)
    CCharString EndDummy;                                   // +0x04
    long        ParticleType;                               // +0x08
    long        PeriodicFrames;                             // +0x0c
    CIntelligentPointer<CThing> ParticleEffect;             // +0x10
    AttackParticleAndEndPoints(const AttackParticleAndEndPoints& other);   // 0x00769040
    ~AttackParticleAndEndPoints();                          // inlined into 0x0076A2A0 (strings + intelligent pointer)
};

namespace std {

struct __false_type { __false_type() { } };

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p) free(p); }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
};

template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

// Same-TU stand-ins of the three tag-taking COMDATs (0x007696E0, 0x00769750, 0x0076A2A0; `__declspec(noinline)`, since
// auto_inline(off) alone does not stop a speed TU inlining templates): with their bodies visible and the tag never touched, VC7.1 homes every empty __false_type temporary on the atend slot as retail does.
#pragma auto_inline(off)
template <class InputIter, class ForwardIter>
__declspec(noinline) ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)   // 0x007696E0
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template <class ForwardIter, class Size, class T>
__declspec(noinline) ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)   // 0x00769750
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

template <class ForwardIter>
__declspec(noinline) void __fastcall __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&)   // 0x0076A2A0
{
    for (; first != last; ++first)
        first->~AttackParticleAndEndPoints();
}
#pragma auto_inline(on)

template <class ForwardIter, class T>
inline void __destroy(ForwardIter first, ForwardIter last, T*) { __destroy_aux(first, last, __false_type()); }

template <class ForwardIter>
inline void _Destroy(ForwardIter first, ForwardIter last) { __destroy(first, last, &*first); }

template <class T, class A> class _Vector_base {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;             // +0x08
};

template <class T, class A = allocator<T> > class vector : protected _Vector_base<T, A> {
public:
    unsigned int size() const { return this->_M_finish - this->_M_start; }
    protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    public:
    protected:
    void _M_set(T* s, T* f, T* e)
    {
        this->_M_start = s;
        this->_M_finish = f;
        this->_M_end_of_storage._M_data = e;
    }
    public:
    protected:
    void _M_insert_overflow(T* __position, const T& __x, const __false_type&, unsigned int __fill_len, bool __atend);
    public:
};

template <class T, class A>
void vector<T, A>::_M_insert_overflow(T* __position, const T& __x, const __false_type&, unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    T* __new_start = this->_M_end_of_storage.allocate(__len);
    T* __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template class vector<AttackParticleAndEndPoints>;

}