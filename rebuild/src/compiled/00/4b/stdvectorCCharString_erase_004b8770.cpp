// std::vector<CCharString>::erase(CCharString* position) -- STLport: shift [position+1, finish) down one slot by assignment, destroy the old last element, return position
// Leaf: erase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B8770, length 72 bytes, `ret 4` (thiscall: position).
// Callees: CCharString::operator= @ 0x0099EFB0, CCharString destructor @ 0x0099EAE0.
// Label: the catalog "vector_pop_back_shift" is behavioural; the body is STLport 4.x vector::erase(iterator):
// `if (pos + 1 != end()) __copy_ptrs(pos + 1, _M_finish, pos, __false_type()); --_M_finish; _Destroy(_M_finish);`.
// Shape: the random-access __copy counts `n = last - first` (the `sar 2 ; test ; jle`) and assigns element by
// element; VC folds the source pointer onto the destination (`lea esi,[ecx+4]`), one register walking both.
class CCharStringData;
class CCharString {
public:
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    ~CCharString();                                     // 0x0099EAE0
private:
    CCharStringData* m_pData;
};

namespace std {
struct __false_type { __false_type() {} };

template <class T> __forceinline void _Destroy(T* p) { p->~T(); }

template <class T>
__forceinline T* __copy_ptrs(const T* first, const T* last, T* result, const __false_type&)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* end() { return _M_finish; }
    T* erase(T* position);
};

template <class T> T* vector<T>::erase(T* position)
{
    if (position + 1 != end())
        __copy_ptrs(position + 1, _M_finish, position, __false_type());
    --_M_finish;
    _Destroy(_M_finish);
    return position;
}

template class vector<CCharString>;
}