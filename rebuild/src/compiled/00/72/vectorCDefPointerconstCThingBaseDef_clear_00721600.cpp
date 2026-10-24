// vector<CDefPointer<const CThingBaseDef> >::clear() -- STLport clear of the hero-morph tattoo vector: erase(begin(), end()) -- copy the (empty) tail down, release every def pointer, reset the finish pointer
// Leaf: ?clear@CDefPointerVector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00721600, 49 bytes (64 with padding), plain `ret`.
// Label correction: the catalog "Vector_Erase_MoveNext" is descriptive; the body is STLport `clear() { erase(begin(),
//   end()); }` with `erase(first, last) { pointer i = __copy_ptrs(last, _M_finish, first, _TrivialAss());
//   _Destroy(i, _M_finish); _M_finish = i; return first; }` inlined; its caller is CTCHeroMorph::AddTattoo (landed
//   0x0071DA70, call site 0x0071DAD2) on the Tattoos member.
// Callees: __copy(first, last, result, const random_access_iterator_tag&, int*) @ 0x0071FA60 and __destroy_aux(first,
//   last, const __false_type&) @ 0x005B26E9 (both __fastcall).
// Shape: one `push ecx` frame word whose high byte homes both empty tag temporaries (`lea edx,[esp+0Fh]`,
//   `lea eax,[esp+0Bh]`); the copied-to pointer is kept in edi for the final _M_finish store.
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

TDefPtr* __fastcall __copy(const TDefPtr* first, const TDefPtr* last, TDefPtr* result, const random_access_iterator_tag&, int*);   // 0x0071FA60
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
    TDefPtr* _M_start;                                  // +0x00
    TDefPtr* _M_finish;                                 // +0x04
    TDefPtr* _M_end_of_storage;                         // +0x08
    TDefPtr* begin() { return _M_start; }
    TDefPtr* end() { return _M_finish; }
    TDefPtr* erase(TDefPtr* first, TDefPtr* last)
    {
        TDefPtr* i = __copy_ptrs(last, _M_finish, first, __false_type());
        _Destroy(i, _M_finish);
        _M_finish = i;
        return first;
    }
    void clear();
};

void CDefPointerVector::clear()
{
    erase(begin(), end());
}