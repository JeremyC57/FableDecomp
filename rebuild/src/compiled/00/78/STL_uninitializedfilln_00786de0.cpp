// _STL::__uninitialized_fill_n(CSkeletalMorph* first, unsigned long n, const CSkeletalMorph& x, const __false_type&) -- copy-construct n copies of a skeletal-morph entry {long, CCharString, float} into raw storage and return the end
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00786DE0, 56 bytes, `ret 8` (/Gr algorithm TU: first in ecx, n in edx,
//   the element reference and the empty tag on the stack).
// Instantiation: the fill path of CArray<CTCSkeletalMorph::CSkeletalMorph> (the 12-byte entry of
//   CTCSkeletalMorph::AddSkeletalMorph 0x00786670 / CArray::push_back 0x00787500: +0 long, +4 CCharString, +8 float).
// Callees: CCharString copy constructor @ 0x0099EC30.
// Shape: STLport `for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;` (no EH: exceptions off) -- the unsigned
//   `n > 0` is a `test ; jbe` entry and a `dec ebx ; jne` loop; the placement copy keeps VC's null test and copies the
//   entry memberwise (long through eax, the string's copy constructor, the float as a dword).
// Label correction: catalogued as `_Fill_n<unsigned_long*,unsigned_int,unsigned_long>` (propagated); the body is
//   STLport `__uninitialized_fill_n`.
struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);               // 0x0099EC30
};

struct CSkeletalMorph {                                  // CTCSkeletalMorph::CSkeletalMorph, 12 bytes
    long        Type;                                    // +0x00
    CCharString Name;                                    // +0x04
    float       Value;                                   // +0x08
    CSkeletalMorph(const CSkeletalMorph& o) : Type(o.Type), Name(o.Name), Value(o.Value) { }
};

struct __false_type { __false_type() { } };
inline void* operator new(unsigned int, void* where) { return where; }

template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }

CSkeletalMorph* __fastcall __uninitialized_fill_n(CSkeletalMorph* first, unsigned long n, const CSkeletalMorph& x,
                                                  const __false_type&)
{
    CSkeletalMorph* cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}