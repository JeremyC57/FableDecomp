// NUISystem::CComponent::RemoveChild(unsigned long index) -- retire the child at index: find its counted pointer in Children, tell it to Die, move it onto ChildrenToDelete and erase it from Children, then drop index from ShapeChildren and renumber the shape indices above it
// Leaf: ?RemoveChild@CComponent@NUISystem@@UAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00533BC0, 255 bytes, virtual thiscall, `ret 4`.
// Identity (proven): CComponent vtable slot 64 (0x0122F5D4 NUISystem::CSprite and 40 other component vtables, all
//   inherit it); Ego_r.pdb ?RemoveChild@CComponent@NUISystem@@UAEXK@Z is the only unsigned-long child member, sits
//   between AddChild and AddChildDynamically in the donor TU with a 0x100-byte body (this one is 0xFF), and its
//   neighbours here are AddChild 0x00533B50 (slot 59), AddChildFront 0x00533BA0 (slot 61), AddChildDynamically
//   0x00533CC0 (slot 60). The landed STLport vector::erase 0x005354E0 already documents this row as RemoveChild.
// Label: catalogued "CScriptBase_RemoveChildAt" (gap scan); the landed CDynamicList callers 0x00541220 / 0x0054133C
//   bind it as RemoveBaseChild / CChangingStateComponent::RemoveChild(long) (NAME_FIXES.tsv).
// Callees: std::__find<CCountedPointer<CComponent>*, CCountedPointer<CComponent> > 0x00534EB0 and
//   std::__find<unsigned long*, unsigned long> 0x005348E0 (STLport random-access finds, __fastcall, tag by reference),
//   the child's virtual Die (slot 81, +0x144; Ego_r.pdb ?Die@CComponent@NUISystem@@UAEXXZ, landed 0x00530720),
//   vector<CCountedPointer<CComponent> >::_M_insert_overflow 0x005359D0 (push_back), ::erase(iterator) 0x005354E0,
//   memmove 0x00BFEAE6 (vector<unsigned long>::erase's trivial copy).
// Layout (retail): Children +0xB0, ChildrenToDelete +0xBC (STLport vectors of 8-byte counted pointers), ShapeChildren
//   +0xE4 (vector<unsigned long>) -- Ego_r.pdb +0xB8 / +0xC8 / +0x104 less the Dinkumware container growth. No
//   engine/ header exists for NUISystem::CComponent. Vtable slots this body does not call are layout-only fillers.
// Shape: STLport find() = __find(first, last, val, iterator_category(first)) with the empty tag homed in the live
//   `index` argument slot (both finds): that needs same-TU bodies of the two __find instantiations (the real
//   STLport 4.6 _algobase.c loop), which never touch the tag.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

class CCPPointerInfo {
public:
    unsigned long RefCount;                                 // +0
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0
    CCPPointerInfo* Info;                                   // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer();
    T* operator->() const { return Data; }
    bool operator==(const CCountedPointer& other) const { return Data == other.Data; }
};

namespace std {
struct __false_type { __false_type() { } };
struct __true_type { __true_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&);                       // 0x00534760
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                               const __false_type&);                     // 0x005347A0
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x00535420
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _Tp> inline random_access_iterator_tag iterator_category(_Tp*) { return random_access_iterator_tag(); }

#pragma auto_inline(off)
template <class _RandomAccessIter, class _Tp>
__declspec(noinline) _RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last,
                                                         const _Tp& __val, const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;
    for ( ; __trip_count > 0 ; --__trip_count) {
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (*__first == __val) return __first;
        ++__first;
    case 2:
        if (*__first == __val) return __first;
        ++__first;
    case 1:
        if (*__first == __val) return __first;
        ++__first;
    case 0:
    default:
        return __last;
    }
}
#pragma auto_inline(on)
template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return __find(__first, __last, __val, iterator_category(__first));
}

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);          // 0x005359D0 (CCountedPointer<CComponent>), body below
public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
    iterator erase(iterator __position);                    // 0x005354E0 (CCountedPointer<CComponent>)
};
// The real STLport 4.6 _vector.c body of the retail COMDAT 0x005359D0 (landed byte-exact): same-TU and noinline, so VC
// sees it never reads its __false_type tag and homes the push_back tag in the live `index` slot, as retail does.
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                                                  size_type __fill_len, bool __atend)
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
#pragma auto_inline(on)
// vector<unsigned long>::erase(iterator): the trivially assignable element copies with memmove (STLport __copy_ptrs
// over __true_type), drops the last slot.
template <> class vector<unsigned long, allocator<unsigned long> > : public _Vector_base<unsigned long, allocator<unsigned long> > {
public:
    typedef unsigned long* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    unsigned long& operator[](size_type __n) { return *(begin() + __n); }
    iterator erase(iterator __position)
    {
        if (__position + 1 != end())
            (iterator)__copy_trivial(__position + 1, this->_M_finish, __position);
        --this->_M_finish;
        return __position;
    }
};
}   // namespace std

namespace NUISystem {
class CComponent {
public:
    virtual void Slot000(); virtual void Slot001(); virtual void Slot002(); virtual void Slot003(); virtual void Slot004();
    virtual void Slot005(); virtual void Slot006(); virtual void Slot007(); virtual void Slot008(); virtual void Slot009();
    virtual void Slot010(); virtual void Slot011(); virtual void Slot012(); virtual void Slot013(); virtual void Slot014();
    virtual void Slot015(); virtual void Slot016(); virtual void Slot017(); virtual void Slot018(); virtual void Slot019();
    virtual void Slot020(); virtual void Slot021(); virtual void Slot022(); virtual void Slot023(); virtual void Slot024();
    virtual void Slot025(); virtual void Slot026(); virtual void Slot027(); virtual void Slot028(); virtual void Slot029();
    virtual void Slot030(); virtual void Slot031(); virtual void Slot032(); virtual void Slot033(); virtual void Slot034();
    virtual void Slot035(); virtual void Slot036(); virtual void Slot037(); virtual void Slot038(); virtual void Slot039();
    virtual void Slot040(); virtual void Slot041(); virtual void Slot042(); virtual void Slot043(); virtual void Slot044();
    virtual void Slot045(); virtual void Slot046(); virtual void Slot047(); virtual void Slot048(); virtual void Slot049();
    virtual void Slot050(); virtual void Slot051(); virtual void Slot052(); virtual void Slot053(); virtual void Slot054();
    virtual void Slot055(); virtual void Slot056(); virtual void Slot057(); virtual void Slot058();
    virtual void AddChild(CCountedPointer<CComponent>& child);              // slot 59, 0x00533B50
    virtual void AddChildDynamically(CCountedPointer<CComponent>& child);   // slot 60, 0x00533CC0
    virtual void AddChildFront(CCountedPointer<CComponent>& child);         // slot 61, 0x00533BA0
    virtual void Slot062(); virtual void Slot063();
    virtual void RemoveChild(unsigned long index);                          // slot 64
    virtual void Slot065(); virtual void Slot066(); virtual void Slot067(); virtual void Slot068(); virtual void Slot069();
    virtual void Slot070(); virtual void Slot071(); virtual void Slot072(); virtual void Slot073(); virtual void Slot074();
    virtual void Slot075(); virtual void Slot076(); virtual void Slot077(); virtual void Slot078(); virtual void Slot079();
    virtual void Slot080();
    virtual void Die();                                                     // slot 81 (+0x144), 0x00530720
    unsigned char                                 _pad04[0xb0 - 4];
    std::vector<CCountedPointer<CComponent> >     Children;                 // +0xB0
    std::vector<CCountedPointer<CComponent> >     ChildrenToDelete;         // +0xBC
    unsigned char                                 _padc8[0xe4 - 0xc8];
    std::vector<unsigned long>                    ShapeChildren;            // +0xE4
};

void CComponent::RemoveChild(unsigned long index)
{
    std::vector<CCountedPointer<CComponent> >::iterator child = std::find(Children.begin(), Children.end(), Children[index]);
    (*child)->Die();
    ChildrenToDelete.push_back(*child);
    Children.erase(child);
    std::vector<unsigned long>::iterator shape = std::find(ShapeChildren.begin(), ShapeChildren.end(), index);
    if (shape != ShapeChildren.end())
        ShapeChildren.erase(shape);
    for (unsigned long i = 0; i < ShapeChildren.size(); ++i)
        if (ShapeChildren[i] > index)
            --ShapeChildren[i];
}
}   // namespace NUISystem