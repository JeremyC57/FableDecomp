// C3DMeshFilePrimitiveChunk::SetMaterialIndex(long materialIndex) -- make the chunk's material-index array exactly one entry long and store the index in it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABCF10, length 40 bytes (48 with int3 padding).
// Leaf: ?SetMaterialIndex@C3DMeshFilePrimitiveChunk
// Callees: the material-index array's Resize(unsigned long, const int&) @ 0x004428F0 (STLport vector<unsigned int>::
// resize in the manifest; the same callee and model as the landed ReadFromFile 0x00ABCF40).
// Label correction: the manifest's "CPersistContext_TransferableComponent<EHeroAbility>::TransferIn" is propagated.
// FableWin.pdb names ?SetMaterialIndex@C3DMeshFilePrimitiveChunk@@QAEXJ@Z; the only caller is
// COptimisedPrimitive::CopyTo (0x00ACC4B3), which passes its MaterialIndex (+0) to the output primitive chunk.
// Layout: retail C3DMeshFilePrimitiveChunk with 12-byte arrays, MaterialIndices at +0x2c (the donor headers have
// 16-byte vectors and +0x34), as the landed ReadFromFile / constructor 0x00ABD080 / WriteToFile 0x00ABC390.
// Shape: the `Resize(1, 0)` zero temporary in the `push ecx` frame slot (stored after the pushes), then the first
// element written through the reloaded array begin -- the else arm of the landed ReadFromFile.
#include "engine/CChunkedFileChunk.h"

template<class T> struct CArray {
    T* Begin; T* End; T* EndOfStorage;
    void Resize(unsigned long, const T&);     // material-index specialization 0x004428F0
    T& operator[](long i) { return Begin[i]; }
};
struct CCountedChunk { void* Object; void* Count; };
struct CChunkedFileSuperChunk : CChunkedFileChunk {
    CArray<CCountedChunk> SubChunks;
    CArray<CCountedChunk> RegisteredSubChunkTypes;
    unsigned long CurrentChunkIndex;
};
struct C3DMeshFilePrimitiveChunk : CChunkedFileSuperChunk {
    CArray<int> MaterialIndices;              // +0x2c
    void SetMaterialIndex(long materialIndex);
};

void C3DMeshFilePrimitiveChunk::SetMaterialIndex(long materialIndex)
{
    MaterialIndices.Resize(1, 0);
    MaterialIndices[0] = materialIndex;
}