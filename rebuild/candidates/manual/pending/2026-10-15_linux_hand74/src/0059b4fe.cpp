// _unattributed::PointerDeque::PointerDeque(const PointerDeque& other) -- STLport deque<T*> copy constructor: base built with the other's allocator and size, then the elements uninitialized-copied into the new storage
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059B4FE, 91 bytes. Manifest label "CShaderRenderManager::SetD3DMatrix" is
// propagated and rejected: the body returns `this` (a constructor), sizes a deque from two 16-byte iterators
// and copies the range. The element type is a 4-byte pointer (the iterator difference 0x0059A8F3 divides the
// pointer gaps by 4 and uses 32 elements per 128-byte block, STLport's _MAX_BYTES), but which deque<T*> this
// instantiates is not recoverable from the bytes.
// Callees: _Deque_iterator::operator-(const iterator&) const @ 0x0059A8F3 (size() = finish - start; manifest
// "CThingMapManager::GetMemoryFootprint"), _Deque_base(const allocator&, size_t) @ 0x0059AEF9 (ten zeroed
// words + _M_initialize_map; manifest "CEngineInterpolationInfo"), __uninitialized_copy(first, last, result,
// const __false_type&) @ 0x0059B49B (`__fastcall`: hidden iterator result in ecx, the tag in edx, the three
// iterators by value; manifest "CEnginePrimitiveRenderer::AddStaticMesh").
//
// Shape: a frame-pointer size TU (`#pragma optimize("y", off)` + `("s", on)`, ends in `leave`): `this` spilled
// to [ebp-4] because esi/edi carry the three `movsd` iterator copies; the empty allocator and __false_type
// temporaries both live in the dead high byte of the argument slot (`lea eax,[ebp+0Bh]`, `lea edx,[ebp+0Bh]`);
// the discarded returned iterator is the 16-byte frame temporary (`lea ecx,[ebp-14h]`). The iterator has a
// user default constructor (so it returns through the hidden pointer in ecx) and an implicit copy (so it is
// passed by `movsd` block copies).
// Leaf: ??0PointerDeque
#pragma optimize("y", off)
#pragma optimize("s", on)
struct SAllocator {                              // std::allocator<T*> (empty)
    SAllocator() {}
};
struct SFalseType {                              // __false_type (empty tag)
    SFalseType() {}
};
struct SDequeIterator {                          // _Deque_iterator<T*> (16 bytes)
    void** _M_cur;
    void** _M_first;
    void** _M_last;
    void*** _M_node;
    SDequeIterator() {}
    long operator-(const SDequeIterator& other) const;   // 0x0059A8F3
};
SDequeIterator __fastcall UninitializedCopy(SDequeIterator first, SDequeIterator last, SDequeIterator result, const SFalseType& isPOD);   // 0x0059B49B
struct CDequeBase {                              // _Deque_base<T*> (40 bytes)
    SDequeIterator _M_start;                     // +0x00
    SDequeIterator _M_finish;                    // +0x10
    void***        _M_map;                       // +0x20
    unsigned long  _M_map_size;                  // +0x24
    CDequeBase(const SAllocator& allocator, unsigned long count);   // 0x0059AEF9
};
struct PointerDeque : CDequeBase {
    SAllocator get_allocator() const { return SAllocator(); }
    unsigned long size() const { return _M_finish - _M_start; }
    SDequeIterator begin() const { return _M_start; }
    SDequeIterator end() const { return _M_finish; }
    PointerDeque(const PointerDeque& other);
};
PointerDeque::PointerDeque(const PointerDeque& other)
    : CDequeBase(other.get_allocator(), other.size())
{
    UninitializedCopy(other.begin(), other.end(), _M_start, SFalseType());
}
