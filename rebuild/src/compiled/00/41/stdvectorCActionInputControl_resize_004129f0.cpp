// std::vector<CActionInputControl>::resize(unsigned int new_size, const CActionInputControl& x) -- STLport resize: erase the tail, or fill-insert copies of x at the end
// Leaf: ?resize@?$vector@VCActionInputControl@@V?$allocator@VCActionInputControl@@@std@@@std@@QAEXIABVCActionInputControl@@@Z
// Retail: Fable.exe @ 0x004129F0, 92 bytes, `ret 8`; callers are the resize(n) wrappers 0x00412AC9 / 0x00412C3C / 0x00412CF9
// (CPersistContext_TransferableVector<CActionInputControl>). Catalog label vector<CBrainBehaviour>::resize is wrong:
// CBrainBehaviour is 12 bytes (Ego_r.pdb), this element is 0x1c (`imul edx,edx,1Ch`, the divide-by-28 reciprocal), as
// the 0x00412AA0 wrapper already showed.
// Body: if (new_size < size()) erase(begin() + new_size, end()) inline: the out-of-line STLport random-access
// __copy(last, end(), first, random_access_iterator_tag(), (ptrdiff_t*)0) (0x0040E2D0, __fastcall: first ecx, last edx,
// the tag by reference into the dead new_size slot), _M_finish = its result (trivial _Destroy); else
// _M_fill_insert(end(), new_size - size(), x) (0x00411DB0).
// Layout: STLport vector (12 bytes); CActionInputControl 0x1c bytes (Ego_r.pdb), copied by assignment (not memmove).
// Shape: /O2 (16-aligned entry). The STLport tag hierarchy matters: a stand-alone empty random_access_iterator_tag gets its
// byte zero-filled by `random_access_iterator_tag()` (97 bytes); the derived (non-aggregate) tag does not.
class CActionInputControl {
public:
    long Fields[7];                                      // GameAction .. ControlDirection (0x1c bytes)
};
namespace std {
struct input_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
template <class T> class allocator {};
template <class RandomAccessIter, class OutputIter, class Distance>
OutputIter __fastcall __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result,
                             const random_access_iterator_tag&, Distance*);   // 0x0040E2D0 for CActionInputControl
template <class T, class A = allocator<T> > class vector {
public:
    typedef T* iterator;
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    iterator erase(iterator first, iterator last)
    {
        iterator i = __copy(last, _M_finish, first, random_access_iterator_tag(), (int*)0);
        _M_finish = i;
        return first;
    }
    void _M_fill_insert(iterator pos, unsigned int n, const T& x);   // 0x00411DB0
    void insert(iterator pos, unsigned int n, const T& x) { _M_fill_insert(pos, n, x); }
    void resize(unsigned int new_size, const T& x)
    {
        if (new_size < size())
            erase(begin() + new_size, end());
        else
            insert(end(), new_size - size(), x);
    }
};
template void vector<CActionInputControl>::resize(unsigned int, const CActionInputControl&);
}