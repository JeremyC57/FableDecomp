// std::vector<CWideString>::resize(unsigned int new_size, const CWideString& x) -- STLport resize: erase the tail, or fill-insert copies of x at the end
// Leaf: ?resize@?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@QAEXIABVCWideString@@@Z
// Retail: Fable.exe @ 0x00412B20, 64 bytes, `ret 8`; one caller (0x00E68DD3: `resize(8, CWideString())` on a member vector
// at +0x1c). Catalog label vector<pair<CCharString,long> >::resize is wrong: the element is 4 bytes and the callees are the
// CWideString range erase 0x00412450 (CWideString::operator= 0x0099B7D0 shifting) and _M_fill_insert 0x004126E0.
// Body: if (new_size < size()) erase(begin() + new_size, end()); else insert(end(), new_size - size(), x) (size re-read).
// Shape: /O2 (16-aligned entry).
class CWideString {
public:
    void* PData;
};
namespace std {
template <class T> class allocator {};
template <class T, class A = allocator<T> > class vector {
public:
    typedef T* iterator;
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    iterator erase(iterator first, iterator last);       // 0x00412450 for CWideString
    void _M_fill_insert(iterator pos, unsigned int n, const T& x);   // 0x004126E0 for CWideString
    void insert(iterator pos, unsigned int n, const T& x) { _M_fill_insert(pos, n, x); }
    void resize(unsigned int new_size, const T& x)
    {
        if (new_size < size())
            erase(begin() + new_size, end());
        else
            insert(end(), new_size - size(), x);
    }
};
template void vector<CWideString>::resize(unsigned int, const CWideString&);
}