// std::vector<CCharString>::resize(unsigned int) -- STLport resize of a CCharString vector with a default-constructed fill value: erase the surplus [begin + n, end) when shrinking, else insert n - size() copies of an empty string at the end
// Leaf: ?resize@CCharStringVector@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049B760, 101 bytes, `ret 4` (__fastcall: this -> esi, the new size on the
//   stack; the `push ecx` frame word holds the default CCharString).
// Callees: 0x0099E4B0 CCharString default ctor (its returned `this` is the pushed `const T&` of the insert),
//   0x0043336A vector<CCharString>::erase(iterator, iterator), 0x004333B3 vector<CCharString>::insert(iterator,
//   size_type, const T&) (both out of line, __fastcall), 0x0099EAE0 ~CCharString (each arm destroys the fill value).
// Callers: seven sites (0x009C8D73, 0x009CE79D, 0x009CEB2C, 0x00CC0177..0x00CD1819).
// Shape: STLport `void resize(size_type __new_size) { resize(__new_size, _Tp()); }` with the two-argument
//   `resize(size_type, const _Tp& __x)` inlined: `if (__new_size < size()) erase(begin() + __new_size, end());
//   else insert(end(), __new_size - size(), __x);` (finish re-read for the second size()). The fill value binds BY
//   REFERENCE: the temporary's address is the constructor's returned `this` (`push eax` in the insert arm); a by-value
//   `_Tp __x` is not inlined by the lane and, forced inline, re-forms the address with a `lea`. Both tail-duplicated arms
//   destroy the temporary.
// Label correction: catalogued as `Vector_InsertElement`; the body is STLport `vector<CCharString>::resize`.
class CCharString { public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

struct CCharStringVector {
    CCharString* _M_start;                              // +0x00
    CCharString* _M_finish;                             // +0x04
    CCharString* _M_end_of_storage;                     // +0x08

    CCharString* begin() { return _M_start; }
    CCharString* end() { return _M_finish; }
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    CCharString* erase(CCharString* __first, CCharString* __last);                          // 0x0043336A
    void insert(CCharString* __pos, unsigned int __n, const CCharString& __x);              // 0x004333B3
    void resize(unsigned int __new_size, const CCharString& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(unsigned int __new_size);
};

void CCharStringVector::resize(unsigned int __new_size)
{
    resize(__new_size, CCharString());
}