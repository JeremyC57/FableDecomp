// Grade: DIFFER(135v135) -- scheduling only: retail loads the old start and runs the free's null compare BEFORE loading the temporary's start word; ours loads the temporary's start first (one mov moved by one slot). Exchange / plain-assign forms identical; swap + inline dtor is worse (171v153).
// CArray<CInstantiatedDefInfo*>::DeallocateUnusedMemory() -- shrink the array to fit: when it has spare capacity, copy the elements into an exactly reserved temporary, release the old storage and adopt the temporary's
// Leaf: ?DeallocateUnusedMemory@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B6EB0, length 135 bytes (+ int3 padding); called by the landed CDefinitionManager::RemoveUnusedMemory 0x009AE5F0 on InstantiatedDefs (+0xa4);
// the manifest's CArray<std::pair<CCharString,...>> label is propagated.
// Callees: vector::reserve 0x009B4F30 (on the temporary), vector::_M_range_insert(pos, first, last, forward_iterator_tag)
// 0x009B4430, CArray::DeallocateMemory 0x009B4B80, free 0x00BFEA14.
// Shape: the CArray<CMapInfo>::DeallocateUnusedMemory 0x0051B560 form for a trivially destructible element (no range destroy):
// `capacity() > size()` compares unsigned (`jbe`); the temporary's inline constructor zeroes its three words; the hand-over
// detaches the old start through `Exchange(slot, new)`, copies the finish / end words and frees the old block. The insert's
// empty tag is a one-byte temporary at frame byte 3.
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

struct CInstantiatedDefInfo;
typedef CInstantiatedDefInfo* CInstantiatedDefInfoPtr;   // 4-byte element (`sar 2`)
struct forward_iterator_tag { forward_iterator_tag() { } };

template<class T> struct CArray {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    unsigned long capacity() const { return (unsigned long)(_M_end_of_storage - _M_start); }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    void reserve(unsigned long n);                                                         // 0x009B4F30
    void _M_range_insert(T* pos, T* first, T* last, const forward_iterator_tag& tag);      // 0x009B4430
    void insert(T* pos, T* first, T* last) { _M_range_insert(pos, first, last, forward_iterator_tag()); }
    void DeallocateMemory();                                                               // 0x009B4B80
    void DeallocateUnusedMemory();
};

template<class P> P Exchange(P& slot, P value) { P old = slot; slot = value; return old; }

template<> void CArray<CInstantiatedDefInfoPtr>::DeallocateUnusedMemory()
{
    if (capacity() > size()) {
        CArray<CInstantiatedDefInfoPtr> temp;
        temp.reserve(size());
        temp.insert(temp.begin(), begin(), end());
        DeallocateMemory();
        CInstantiatedDefInfoPtr* p = Exchange(_M_start, temp._M_start);
        _M_finish = temp._M_finish;
        _M_end_of_storage = temp._M_end_of_storage;
        if (p)
            free(p);
    }
}
