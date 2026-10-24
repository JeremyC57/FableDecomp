// Grade: DIFFER(153v153) -- scheduling only (as 0x009B79B0 / 0x009B6EB0): retail loads the old start and runs the free's null compare BEFORE loading the temporary's start word; ours loads the temporary's start first.
// CArray<SElement12>::DeallocateUnusedMemory() -- shrink the array to fit: when it has spare capacity, copy the elements into an exactly reserved temporary, release the old storage and adopt the temporary's
// Leaf: ?DeallocateUnusedMemory@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A725D0, length 153 bytes (+ int3 padding); called from CPhysicsMesh::GetMeshDataFromFileData (0x00A6CFEB); DeallocateMemory 0x00A71F40 is the
// landed CArray<SElement12> body (`imul 2AAAAAABh ; sar 1` = 12-byte stride).
// Callees: vector::reserve 0x00920FF0 (on the temporary), vector::_M_range_insert(pos, first, last, forward_iterator_tag)
// 0x00A70CB0, CArray::DeallocateMemory 0x00A71F40, free 0x00BFEA14.
// Shape: the CArray<CMapInfo>::DeallocateUnusedMemory 0x0051B560 form for a trivially destructible element (no range destroy):
// `capacity() > size()` compares unsigned (`jbe`); the temporary's inline constructor zeroes its three words; the hand-over
// detaches the old start through `Exchange(slot, new)`, copies the finish / end words and frees the old block. The insert's
// empty tag is a one-byte temporary at frame byte 3.
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

struct SElement12 {                             // 12-byte trivially destructible record (as landed 0x00A71F40)
    unsigned char _body[0xc];
};
struct forward_iterator_tag { forward_iterator_tag() { } };

template<class T> struct CArray {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    unsigned long capacity() const { return (unsigned long)(_M_end_of_storage - _M_start); }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    void reserve(unsigned long n);                                                         // 0x00920FF0
    void _M_range_insert(T* pos, T* first, T* last, const forward_iterator_tag& tag);      // 0x00A70CB0
    void insert(T* pos, T* first, T* last) { _M_range_insert(pos, first, last, forward_iterator_tag()); }
    void DeallocateMemory();                                                               // 0x00A71F40
    void DeallocateUnusedMemory();
};

template<class P> P Exchange(P& slot, P value) { P old = slot; slot = value; return old; }

template<> void CArray<SElement12>::DeallocateUnusedMemory()
{
    if (capacity() > size()) {
        CArray<SElement12> temp;
        temp.reserve(size());
        temp.insert(temp.begin(), begin(), end());
        DeallocateMemory();
        SElement12* p = Exchange(_M_start, temp._M_start);
        _M_finish = temp._M_finish;
        _M_end_of_storage = temp._M_end_of_storage;
        if (p)
            free(p);
    }
}
