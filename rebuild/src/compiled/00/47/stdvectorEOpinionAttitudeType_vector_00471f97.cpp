// std::vector<EOpinionAttitudeType>::vector(const vector& x) -- STLport copy constructor: allocate x.size() slots through _Vector_base, then copy-construct every element and set the finish pointer
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00471F97, 66 bytes, `ret 4` (thiscall: the source vector by reference; returns this).
// Identity: the catalog's "vector_insert_range_copy" is behavioural. First caller: COpinionDeedReactionDef's copy constructor
//   @ 0x00471F60 for the vector at +0x3c (Ego_r.pdb COpinionDeedReactionDef::Attitudes = vector<EOpinionAttitudeType>);
//   the three callers at 0x0067C70D / 0x0067CA37 / 0x0067CAB7 reach the same body (an identical 4-byte-element
//   instantiation folded by the linker).
// Callees: _Vector_base<EOpinionAttitudeType>::_Vector_base(size_t n, const allocator&) @ 0x00471FD9 (out of line:
//   zero the three words, malloc n * 4 unless n is 0, set finish and end of storage).
// Model: STLport 4.x `vector(const _Self& x) : _Vector_base(x.size(), x.get_allocator()) { _M_finish =
//   __uninitialized_copy(x._M_start, x._M_finish, _M_start, _IsPODType()); }` -- an enum is not a POD to STLport's
//   type traits, so the copy is the placement-new loop with its null test. The empty allocator temporary is homed in
//   the dead argument slot's top byte (`lea eax,[ebp+0Bh]`) and never initialised (empty user constructor).
// Shape: size TU with a frame pointer (`optimize("y", off)` + `("s", on)`).
#pragma optimize("y", off)
#pragma optimize("s", on)

inline void* operator new(unsigned int, void* where) { return where; }

enum EOpinionAttitudeType { OPINION_ATTITUDE_NONE };

namespace std {
struct __false_type { };

template <class T1, class T2> __forceinline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class InputIter, class ForwardIter>
__forceinline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}

template <class T> struct allocator {
    allocator() { }
    allocator(const allocator<T>&) { }
};

template <class T, class A> struct _Vector_base {
    T* _M_start;                                    // +0x00
    T* _M_finish;                                   // +0x04
    T* _M_end_of_storage;                           // +0x08 (_STLP_alloc_proxy over the empty allocator)
    _Vector_base(unsigned int n, const A& a);       // 0x00471FD9
};

template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    typedef A allocator_type;
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    allocator_type get_allocator() const { return allocator_type(); }
    vector(const vector<T, A>& x)
        : _Vector_base<T, A>(x.size(), x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start, __false_type());
    }
};

template vector<EOpinionAttitudeType, allocator<EOpinionAttitudeType> >::vector(const vector<EOpinionAttitudeType, allocator<EOpinionAttitudeType> >& x);
}