// vector<CDefPointer<const CThingBaseDef> >::operator=(const vector& x) -- STLport vector assignment for the hero-morph tattoo defs: reallocate and copy-construct when the source outgrows the capacity, else assign over the live def pointers and destroy the surplus, or assign the overlap and copy-construct the rest
// Leaf: ??4CDefPointerVector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00721150, 228 bytes (240 with padding), `ret 4`.
// Label correction: the catalog "Vector_CopyOrMove_CountedPtr" is descriptive; its caller CTCHeroMorph::Copy (landed
//   0x0071CD90) assigns the Tattoos member, vector<CDefPointer<const CThingBaseDef> > (Ego_r.pdb).
// Callees: vector::_M_allocate_and_copy @ 0x0071F2E0 (this chunk; thiscall, `ret 0Ch`), __destroy_aux(first, last,
//   const __false_type&) @ 0x005B26E9 (__fastcall), free @ 0x00BFEA14, __copy(first, last, result,
//   const random_access_iterator_tag&, int*) @ 0x0071F430 (__fastcall), __uninitialized_copy(first, last, result,
//   const __false_type&) @ 0x0071EA10 (landed, __fastcall).
// Element: CDefPointer<T> = { CDefPointeeBase* Object } (4 bytes; the pointee keeps its count at +4).
// Shape: the STLport 4.x operator= text (same as the landed vector<SSlot>::operator= 0x006F3C20) with
//   _M_allocate_and_copy out of line and _M_clear inline (`_Destroy(start, finish)` then the allocator's
//   `if (p) free(p)`); the empty tag temporaries share the dead `x` argument slot and __copy's null distance
//   pointer is pushed once ahead of the size() >= xlen test for both remaining arms.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };

struct CDefPointeeBase {
    virtual void v00();
    virtual void Release();                             // slot 1 (+0x04)
    long RefCount;                                      // +0x04
};
struct CThingBaseDef;
template <class T> struct CDefPointer {                 // 4 bytes
    CDefPointeeBase* Object;
};
typedef CDefPointer<const CThingBaseDef> TDefPtr;

TDefPtr* __fastcall __uninitialized_copy(const TDefPtr* first, const TDefPtr* last, TDefPtr* result, const __false_type&);   // 0x0071EA10
TDefPtr* __fastcall __copy(const TDefPtr* first, const TDefPtr* last, TDefPtr* result, const random_access_iterator_tag&, int*);   // 0x0071F430
void __fastcall __destroy_aux(TDefPtr* first, TDefPtr* last, const __false_type&);   // 0x005B26E9

inline TDefPtr* __copy_ptrs(const TDefPtr* first, const TDefPtr* last, TDefPtr* result, const __false_type&)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}
inline void _Destroy(TDefPtr* first, TDefPtr* last)
{
    __destroy_aux(first, last, __false_type());
}

struct CDefPointerVector {                              // STLport vector<CDefPointer<const CThingBaseDef> > (12 bytes)
    typedef unsigned int size_type;
    TDefPtr* _M_start;                                  // +0x00
    TDefPtr* _M_finish;                                 // +0x04
    TDefPtr* _M_end_of_storage;                         // +0x08
    size_type size() const { return size_type(_M_finish - _M_start); }
    size_type capacity() const { return size_type(_M_end_of_storage - _M_start); }
    TDefPtr* _M_allocate_and_copy(size_type n, const TDefPtr* first, const TDefPtr* last);   // 0x0071F2E0
    void _M_clear()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    CDefPointerVector& operator=(const CDefPointerVector& x);
};

CDefPointerVector& CDefPointerVector::operator=(const CDefPointerVector& x)
{
    if (&x != this) {
        const size_type xlen = x.size();
        if (xlen > capacity()) {
            TDefPtr* tmp = _M_allocate_and_copy(xlen, x._M_start, x._M_finish);
            _M_clear();
            _M_start = tmp;
            _M_end_of_storage = _M_start + xlen;
        } else if (size() >= xlen) {
            TDefPtr* i = __copy_ptrs(x._M_start, x._M_finish, _M_start, __false_type());
            _Destroy(i, _M_finish);
        } else {
            __copy_ptrs(x._M_start, x._M_start + size(), _M_start, __false_type());
            __uninitialized_copy(x._M_start + size(), x._M_finish, _M_finish, __false_type());
        }
        _M_finish = _M_start + xlen;
    }
    return *this;
}