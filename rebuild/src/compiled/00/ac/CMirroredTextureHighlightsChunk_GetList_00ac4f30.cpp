// CMirroredTextureHighlightsChunk::GetList() -- the chunk's vertex-index list as a plain array (the array object itself when it is empty)
// Leaf: ?GetList@CMirroredTextureHighlightsChunk@@QAEPAKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC4F30, 20 bytes. Ego_r.pdb `?GetList@CMirroredTextureHighlightsChunk@@QAEPAKXZ`
//   (non-const; a const twin `QBEPBKXZ` exists in the donor). Retail keeps the class's inline members together after
//   GetChunkInfo (0x00AC4EC0): GetCount 0x00AC4F20, this row, WriteToFile 0x00AC4F50. Its one caller (0x00A92464, the
//   C3DPrimitive2 loader) reaches the chunk through a CCountedPointer<CMirroredTextureHighlightsChunk> (non-const pointee
//   in the PDB name sets) and hands the list to C3DPrimitive2::AddMirroredTextureVertexIndex (0x00A90510).
// Layout: Ego_r.pdb CMirroredTextureHighlightsChunk: CChunkedFileChunk base (vptr +0, three words), TriangleVertexIndexList
//   +0x10 (CArray<unsigned long>, 12 bytes in retail). engine/CMirroredTextureHighlightsChunk.h types the array as opaque
//   bytes, so the class is a local model with the same offsets.
// Shape: CArray::GetAsCArray() inline -- `size() > 0 ? &(*this)[0] : GetZeroSizeArray()`, the private zero-size array
//   being the array object itself (`lea eax,[ecx+10h]` survives as the empty result).
namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef _Tp& reference;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    reference operator[](size_type __n) { return *(begin() + __n); }
};
}

template <class T> class CArray : public std::vector<T> {
public:
    T* GetAsCArray() { return this->size() > 0 ? &(*this)[0] : GetZeroSizeArray(); }
private:
    T* GetZeroSizeArray() { return reinterpret_cast<T*>(this); }
};

class CChunkedFileChunk {
public:
    virtual ~CChunkedFileChunk();
    unsigned char ChunkData[0xC];                       // +0x04 .. +0x0F
};

class CMirroredTextureHighlightsChunk : public CChunkedFileChunk {
public:
    CArray<unsigned long> TriangleVertexIndexList;      // +0x10
    unsigned long* GetList();
};

unsigned long* CMirroredTextureHighlightsChunk::GetList()
{
    return TriangleVertexIndexList.GetAsCArray();
}