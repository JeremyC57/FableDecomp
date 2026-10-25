// std::vector<bool>::vector(const vector<bool>& x) -- STLport vector copy constructor for a 1-byte element: allocate x.size() bytes, then copy the elements trivially
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D34F00, 59 bytes, `__fastcall`, one stack argument (`ret 4`), returns this.
//   Callers: the CFireHeartPatternDef element construct @ 0x00D34EC0 (PadActive +0x28, Ego_r.pdb
//   CFireHeartPatternDef::PadActive is vector<bool>) and two copies in 0x00F33Fxx / 0x00F342xx.
// Label correction: the catalog "Map_InsertOrUpdatePair" is descriptive and wrong; 0x00450CCC ("Buffer_InitializeWithSize")
//   is _Vector_base<1-byte>(n, allocator), 0x00D34F40 ("Map_MoveOrCopyValue") the trivial __uninitialized_copy.
// Callees: _Vector_base<bool>::_Vector_base(unsigned int n, const allocator<bool>&) @ 0x00450CCC (null-start triple, malloc
//   @ 0x00BFEA0E when n != 0, finish = start, end = start + n), __uninitialized_copy(const bool*, const bool*, bool*,
//   const __true_type&) @ 0x00D34F40 (__fastcall: first in ecx, last in edx; memmove).
// Shape: speed TU (no pragma). STLport 4.6 `vector(const _Self& x) : _Vector_base(x.size(), x.get_allocator())
//   { _M_finish = __uninitialized_copy(x._M_start, x._M_finish, _M_start, _IsPODType()); }`. Fable's STLport does not
//   specialise vector<bool> (a 12-byte vector of bytes). The allocator temporary (user ctor, no store) and the
//   __true_type tag (value-initialised POD, one byte store of 0) both live in the dead argument slot [esp+0Ch].
namespace std {
struct __true_type { };
template <class T> class allocator {
public:
    allocator() { }
};
template <class T> class _Vector_base {
public:
    T* _M_start;                                        // +0
    T* _M_finish;                                       // +4
    T* _M_end_of_storage;                               // +8 (alloc proxy over the empty allocator)
    _Vector_base(unsigned int n, const allocator<T>& a);    // 0x00450CCC
};
template <class T> T* __fastcall __uninitialized_copy(const T* first, const T* last, T* result, const __true_type&);   // 0x00D34F40
template <class T> class vector : public _Vector_base<T> {
public:
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    allocator<T> get_allocator() const { return allocator<T>(); }
    vector(const vector& x) : _Vector_base<T>(x.size(), x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start, __true_type());
    }
};
template class vector<bool>;
} // namespace std