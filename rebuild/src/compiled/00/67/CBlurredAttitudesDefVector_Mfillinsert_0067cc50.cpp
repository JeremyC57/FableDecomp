// CBlurredAttitudesDefVector::_M_fill_insert(CBlurredAttitudesDef* position, unsigned long n, const CBlurredAttitudesDef& x)
// -- STLport vector<CBlurredAttitudesDef> fill-insert: with room, shift the tail up (uninitialized copy of the last n
// or fill of the overhang) and fill the gap from a local copy of x; without room, regrow through _M_insert_overflow
// Leaf: _M_fill_insert
// Retail: Fable.exe (TLC, VC7.1) @ 0x0067CC50, 342 bytes (padded to 352), `ret 0xc`.
// Identity: the manifest row "CVectorBase::resize_with_copy" is descriptive; 0x48-byte elements (`imul 38E38E39h ;
//   sar 4`), the element copy ctor 0x0067C6D0 (CBlurredAttitudesDef, vtable 0x0125B8AC) and the overflow call to the
//   landed CBlurredAttitudesDefVector::_M_insert_overflow 0x0067CB20 make it this vector's _M_fill_insert.
// Callees: CBlurredAttitudesDef(const CBlurredAttitudesDef&) 0x0067C6D0, __fastcall __uninitialized_copy 0x0067C9F0,
//   __fastcall __copy_backward 0x0067C8B0, __fastcall fill 0x0067C720, __fastcall __uninitialized_fill_n 0x0067CA70,
//   _M_insert_overflow 0x0067CB20, free 0x00BFEA14 (the two vector members of the local copy), the root base dtor
//   0x009FC550 (CDefPointeeBase vtable 0x01230BA0 stored first).
// Layout: the def base chain as the landed CUIIconsDef/CFrontEndDef operator= (pack(1) CParentDefClassBase: SubDefInfoMap
//   vector at +0x29), CBlurredAttitudesDef::Attitudes (vector<EOpinionAttitudeType>) at +0x3c (Ego_r.pdb +0x40).
// Shape: the four helpers need same-TU stand-in bodies (declared-only externs move the `old_finish - n` spill from the
//   dead n slot into the dead x slot).
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };
template <class T> struct CRetailVector {              // STLport vector storage: begin/end/capacity
    T* m_first; T* m_last; T* m_end;
    ~CRetailVector() { if (m_first) free(m_first); }
};
struct CBase_009fc550 {                                // polymorphic root (its dtor stores vtables 0x012354A4/0x012354D0)
    virtual void b00();
    unsigned long m_words[6];                          // +0x04..+0x18
    ~CBase_009fc550();                                 // 0x009FC550 (labelled "~CMemoryBlock")
};
#pragma pack(push, 1)
class CDefPointeeBase : public CBase_009fc550 { public:              // vtable 0x01230BA0, 28 bytes
    ~CDefPointeeBase() { }
};
class CDefinitionManager;
struct __declspec(novtable) CDefClassBase : CDefPointeeBase {
    CDefinitionManager* PDefManager;                   // +0x1c
    unsigned long GlobalIndex;                         // +0x20
    unsigned char Flags;                               // +0x24 (Setup/Template/DefaultValsApplied bits)
};
struct CSubDefInfo;
struct CKeyPairCompareLess {};
template<class K, class V> class CVectorMap { public:         // retail: 14 bytes
    CRetailVector<V>    m_vector;                      // +0x00
    CKeyPairCompareLess m_compare;                     // +0x0c (empty)
    bool                m_sorted;                      // +0x0d
};
class CDefString { public: long TablePos; };
struct __declspec(novtable) CParentDefClassBase : CDefClassBase {
    CDefString                             InstantiationName;   // +0x25
    CVectorMap<unsigned long, CSubDefInfo> SubDefInfoMap;       // +0x29
};
#pragma pack(pop)
enum EOpinionAttitudeType { OPINION_ATTITUDE_NONE };
struct __declspec(novtable) CBlurredAttitudesDef : CParentDefClassBase {   // 0x48 bytes (vtable 0x0125B8AC)
    unsigned char                        _pad37[5];
    CRetailVector<EOpinionAttitudeType>  Attitudes;    // +0x3c
    CBlurredAttitudesDef(const CBlurredAttitudesDef& other);   // 0x0067C6D0
};
void* FindRef_0067c9f0();                               // stand-in callees of the stand-in bodies below
void* FindRef_0067c8b0();
void* FindRef_0067c720();
void* FindRef_0067ca70();
// Same-TU stand-in bodies (the real ones are out of line in retail): they only write a word through their pointer
// parameters, so VC7.1 knows the caller's address-taken locals do not escape (precedent: _M_insert_overflow 0x0067CB20).
#pragma auto_inline(off)
__declspec(noinline) CBlurredAttitudesDef* __fastcall __uninitialized_copy(CBlurredAttitudesDef* first, CBlurredAttitudesDef* last,
                                                                           CBlurredAttitudesDef* result, const __false_type& p3)   // 0x0067C9F0
{
    void* p = FindRef_0067c9f0();
    if (!p)
        return 0;
    *(void**)first = p;
    *(void**)last = p;
    *(void**)result = p;
    return 0;
}
__declspec(noinline) CBlurredAttitudesDef* __fastcall __copy_backward(CBlurredAttitudesDef* first, CBlurredAttitudesDef* last,
                                                                      CBlurredAttitudesDef* result, const random_access_iterator_tag& p3, int* p4)   // 0x0067C8B0
{
    void* p = FindRef_0067c8b0();
    if (!p)
        return 0;
    *(void**)first = p;
    *(void**)last = p;
    *(void**)result = p;
    return 0;
}
__declspec(noinline) void __fastcall fill(CBlurredAttitudesDef* first, CBlurredAttitudesDef* last, const CBlurredAttitudesDef& x)   // 0x0067C720
{
    void* p = FindRef_0067c720();
    if (!p)
        return;
    *(void**)first = p;
    *(void**)last = p;
}
__declspec(noinline) CBlurredAttitudesDef* __fastcall __uninitialized_fill_n(CBlurredAttitudesDef* first, unsigned long n,
                                                                             const CBlurredAttitudesDef& x, const __false_type& p3)   // 0x0067CA70
{
    void* p = FindRef_0067ca70();
    if (!p)
        return 0;
    *(void**)first = p;
    *(void**)&x = p;
    return 0;
}
#pragma auto_inline(on)
inline CBlurredAttitudesDef* __copy_backward_ptrs(CBlurredAttitudesDef* first, CBlurredAttitudesDef* last,
                                                  CBlurredAttitudesDef* result, const __false_type&)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}
struct CBlurredAttitudesDefVector {                    // STLport vector (12 bytes in retail)
    CBlurredAttitudesDef* _M_start;
    CBlurredAttitudesDef* _M_finish;
    CBlurredAttitudesDef* _M_end_of_storage;
    void _M_insert_overflow(CBlurredAttitudesDef* position, const CBlurredAttitudesDef& x, const __false_type&,
                            unsigned long fill_len, bool atend = false);   // 0x0067CB20
    void _M_fill_insert(CBlurredAttitudesDef* position, unsigned long n, const CBlurredAttitudesDef& x);
};

void CBlurredAttitudesDefVector::_M_fill_insert(CBlurredAttitudesDef* position, unsigned long n, const CBlurredAttitudesDef& x)
{
    if (n != 0) {
        if ((unsigned long)(_M_end_of_storage - _M_finish) >= n) {
            CBlurredAttitudesDef x_copy = x;
            const unsigned long elems_after = _M_finish - position;
            CBlurredAttitudesDef* old_finish = _M_finish;
            if (elems_after > n) {
                __uninitialized_copy(_M_finish - n, _M_finish, _M_finish, __false_type());
                _M_finish += n;
                __copy_backward_ptrs(position, old_finish - n, old_finish, __false_type());
                fill(position, position + n, x_copy);
            }
            else {
                __uninitialized_fill_n(_M_finish, n - elems_after, x_copy, __false_type());
                _M_finish += n - elems_after;
                __uninitialized_copy(position, old_finish, _M_finish, __false_type());
                _M_finish += elems_after;
                fill(position, old_finish, x_copy);
            }
        }
        else
            _M_insert_overflow(position, x, __false_type(), n);
    }
}