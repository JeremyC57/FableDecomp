// CMirroredTextureHighlightsChunk::GetList() -- the chunk's vertex-index list as a plain array (the array object itself when it is empty)
// Leaf: ?GetList@CMirroredTextureHighlightsChunk@@QAEPAKXZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
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
#include "engine/CArray.h"


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