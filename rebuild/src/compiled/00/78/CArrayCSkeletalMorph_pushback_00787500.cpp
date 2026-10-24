// CArray<CSkeletalMorph>::push_back(const CSkeletalMorph& x) -- STLport vector push_back of a skeletal-morph entry {long, CCharString, float}: copy-construct at the finish when there is room, else the overflow path
// Leaf: push_back
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00787500, 80 bytes, __fastcall `ret 4`.
// Identity: Ego_r.pdb `?push_back@?$CArray@VCSkeletalMorph@CTCSkeletalMorph@@@@QAEXABVCSkeletalMorph@CTCSkeletalMorph@@@Z`
//   (CArray<CTCSkeletalMorph::CSkeletalMorph>::push_back); the out-of-line copy of the body that
//   CTCSkeletalMorph::AddSkeletalMorph @ 0x00786670 inlines (same growth callee). No direct callers.
// Callees: CCharString copy constructor @ 0x0099EC30, _M_insert_overflow(pos, x, __false_type, 1, true) @ 0x00787380
//   (catalogued as StdVector_ReallocateWithCopy).
// Layout: retail CArray<T> is the 12-byte STLport vector (start +0, finish +4, end of storage +8); the 12-byte entry is
//   CSkeletalMorph(long, const CCharString&, float) (PDB ctor signature): +0 long, +4 name, +8 float (copied as a dword).
// Shape: STLport `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, __false_type(), 1, true);` -- placement new under VC's null test with the entry's
//   memberwise copy constructor, `++_M_finish` as `add dword ptr [edi+4],0Ch`; the empty __false_type temporary is homed
//   in the dead `x` argument slot.
// Label correction: catalogued as `Vector_Insert_CCharString`; the body is CArray<CSkeletalMorph>::push_back.
struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
};

struct CSkeletalMorph {                                  // CTCSkeletalMorph::CSkeletalMorph, 12 bytes
    long        Type;                                    // +0x00
    CCharString Name;                                    // +0x04
    float       Value;                                   // +0x08
    CSkeletalMorph(const CSkeletalMorph& o) : Type(o.Type), Name(o.Name), Value(o.Value) { }
};

struct __false_type { __false_type() { } };
inline void* operator new(unsigned int, void* where) { return where; }

template <class T> struct CArray {                       // retail: the 12-byte STLport vector
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned long fill_len, bool atend);
    void push_back(const T& x);
};

template <> void CArray<CSkeletalMorph>::_M_insert_overflow(CSkeletalMorph* position, const CSkeletalMorph& x,
                                                            const __false_type&, unsigned long fill_len,
                                                            bool atend);   // 0x00787380

template <class T> void CArray<T>::push_back(const T& x)
{
    if (_M_finish != _M_end_of_storage) {
        new (_M_finish) T(x);
        ++_M_finish;
    }
    else
        _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
}

template void CArray<CSkeletalMorph>::push_back(const CSkeletalMorph& x);