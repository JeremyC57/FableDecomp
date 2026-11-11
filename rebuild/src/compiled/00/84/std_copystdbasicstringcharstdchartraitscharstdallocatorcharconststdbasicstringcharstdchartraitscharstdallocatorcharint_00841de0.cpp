// _global::Copy_RetailString_00841de0(const CRetailString* first, const CRetailString* last, CRetailString* result, const random_access_iterator_tag&, int*) -- assign [first, last) of STLport strings onto result (self-checked range assign per element), return the advanced result
// Leaf: Copy_RetailString_00841de0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00841DE0, length 81 bytes (`ret 0Ch`).
// Identity: catalog `_Move<CStaticMapGenerationVisibiilityInfo::CMapDesc*>` is propagated: CMapDesc is 16 bytes (a set<long>)
//   in Ego_r.pdb, while this body strides 12 bytes and assigns each element through STLport
//   basic_string::_M_assign_dispatch 0x00569140 (manifest "std::string_InsertRange") behind the inline self-test -- the same
//   inline string operator= as the landed LAddress::operator= 0x00841F60 of this TU. Caller: the vector helper 0x00841FC0.
// Shape: SGI `__copy(first, last, result, random_access_iterator_tag, Distance*)` (the counted form), `__fastcall`: first in
//   ecx, last in edx, result + the tag reference + the distance pointer on the stack (`ret 0Ch`); the signed count is
//   (last - first) / sizeof(T) (reciprocal multiply), counted down; an empty range returns the untouched result.
//   The empty `__false_type` temporary is homed in the dead `result` argument slot (`lea eax,[esp+10h]`).
struct random_access_iterator_tag { random_access_iterator_tag() { } };   // empty tag, passed by const reference

struct __false_type { __false_type() { } };     // STLport empty tag; a user constructor keeps the temporary store-free

struct CRetailString {                          // STLport basic_string<char>, 12 bytes in retail (landed LAddress 0x00841F60)
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    CRetailString& _M_assign_dispatch(const char* first, const char* last, const __false_type&);   // 0x00569140
    CRetailString& operator=(const CRetailString& other)
    {
        if (&other != this)
            _M_assign_dispatch(other._M_start, other._M_finish, __false_type());
        return *this;
    }
};

CRetailString* __fastcall Copy_RetailString_00841de0(const CRetailString* first, const CRetailString* last, CRetailString* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}