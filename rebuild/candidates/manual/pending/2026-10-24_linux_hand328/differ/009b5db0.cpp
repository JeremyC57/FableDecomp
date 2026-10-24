// Grade: DIFFER(129v129) -- scheduling only (as 0x009B79B0 / 0x009B6EB0): retail loads the old start and runs the free's null compare BEFORE loading the temporary's start word; ours loads the temporary's start first.
// CArray<unsigned char>::DeallocateUnusedMemory() -- shrink the array to fit: when it has spare capacity, copy the elements into an exactly reserved temporary, release the old storage and adopt the temporary's
// Leaf: ?DeallocateUnusedMemory@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B5DB0, length 129 bytes (+ int3 padding); called from CDefinitionManager::SaveBinaryDefinitions (0x009AFA86) and
// CCompressorZlib::InternalCompress (0x00A3AE5B, landed); byte element, so size/capacity are plain differences.
// Callees: vector::reserve 0x009B4E20 (on the temporary), vector::_M_range_insert(pos, first, last, forward_iterator_tag)
// 0x009B3630, CArray::DeallocateMemory 0x009B4AE0, free 0x00BFEA14.
// Shape: the CArray<CMapInfo>::DeallocateUnusedMemory 0x0051B560 form for a trivially destructible element (no range destroy):
// `capacity() > size()` compares unsigned (`jbe`); the temporary's inline constructor zeroes its three words; the hand-over
// detaches the old start through `Exchange(slot, new)`, copies the finish / end words and frees the old block. The insert's
// empty tag is a one-byte temporary at frame byte 3.
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14


struct forward_iterator_tag { forward_iterator_tag() { } };

template<class T> struct CArray {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    unsigned long capacity() const { return (unsigned long)(_M_end_of_storage - _M_start); }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    void reserve(unsigned long n);                                                         // 0x009B4E20
    void _M_range_insert(T* pos, T* first, T* last, const forward_iterator_tag& tag);      // 0x009B3630
    void insert(T* pos, T* first, T* last) { _M_range_insert(pos, first, last, forward_iterator_tag()); }
    void DeallocateMemory();                                                               // 0x009B4AE0
    void DeallocateUnusedMemory();
};

template<class P> P Exchange(P& slot, P value) { P old = slot; slot = value; return old; }

template<> void CArray<unsigned char>::DeallocateUnusedMemory()
{
    if (capacity() > size()) {
        CArray<unsigned char> temp;
        temp.reserve(size());
        temp.insert(temp.begin(), begin(), end());
        DeallocateMemory();
        unsigned char* p = Exchange(_M_start, temp._M_start);
        _M_finish = temp._M_finish;
        _M_end_of_storage = temp._M_end_of_storage;
        if (p)
            free(p);
    }
}
