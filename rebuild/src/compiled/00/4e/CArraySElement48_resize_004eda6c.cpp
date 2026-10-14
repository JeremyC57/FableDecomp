#pragma optimize("s", on)
// CArray<SElement48>::resize(unsigned long n, const SElement48& value) -- shrink by erasing [begin()+n, end()) or grow
// by inserting n - size() copies of `value` at the end.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EDA6C, length 75 bytes.
// Callees: CArray<SElement48>::Erase(first, last) 0x004EDAB7, CArray<SElement48>::Insert(where, count, value)
//          0x004EDB3E (both __fastcall). The manifest label (push_back) is propagated: `ret 8` with an index compared
//          against size() (`jae`, unsigned) and two calls is the VC6-era vector resize.
// Retail CArray<T> is {first, last, end_of_storage} (12 bytes); size() is the signed pointer difference divided by
// the 0x30-byte stride (`cdq ; idiv`), compared unsigned with n. Size-optimised TU (`push 30h ; pop esi`); `last`
// and size() are re-read for the insert branch.
struct SElement48 {
    unsigned char _body[0x30];
};

template<class T> struct CArray {
    T* first; T* last; T* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    void Erase(T* f, T* l);                              // 0x004EDAB7
    void Insert(T* where, unsigned long count, const T& value);   // 0x004EDB3E
    void resize(unsigned long n, const T& value);
};

template<class T> void CArray<T>::resize(unsigned long n, const T& value)
{
    if (n < size())
        Erase(first + n, last);
    else
        Insert(last, n - size(), value);
}

template struct CArray<SElement48>;