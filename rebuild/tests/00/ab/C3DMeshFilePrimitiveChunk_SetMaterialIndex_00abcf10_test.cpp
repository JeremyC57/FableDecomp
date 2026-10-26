// Behaviour test for 00abcf10 C3DMeshFilePrimitiveChunk::SetMaterialIndex (copy of the model; the array Resize is a
// working stand-in over a fixed buffer).
#include <cstdio>
static int g_resizes = 0;
template<class T> class CArray { public:
    T* Begin; T* End; T* EndOfStorage;
    void Resize(unsigned long n, const T& x)
    {
        ++g_resizes;
        unsigned long size = End - Begin;
        for (unsigned long i = size; i < n; ++i) Begin[i] = x;
        End = Begin + n;
    }
    T& operator[](long i) { return Begin[i]; }
};
class CChunkedFileChunk { public: void* vt; long Type; };
struct CCountedChunk { void* Object; void* Count; };
class CChunkedFileSuperChunk : public CChunkedFileChunk { public:
    CArray<CCountedChunk> SubChunks;
    CArray<CCountedChunk> RegisteredSubChunkTypes;
    unsigned long CurrentChunkIndex;
};
class C3DMeshFilePrimitiveChunk : public CChunkedFileSuperChunk { public:
    CArray<int> MaterialIndices;
    void SetMaterialIndex(long materialIndex);
};
void C3DMeshFilePrimitiveChunk::SetMaterialIndex(long materialIndex)
{
    MaterialIndices.Resize(1, 0);
    MaterialIndices[0] = materialIndex;
}
int main()
{
    int buffer[8] = { 5, 6, 7, 8, 9, 9, 9, 9 };
    C3DMeshFilePrimitiveChunk c;
    c.MaterialIndices.Begin = buffer; c.MaterialIndices.End = buffer + 4; c.MaterialIndices.EndOfStorage = buffer + 8;
    c.SetMaterialIndex(42);                                           // shrink to one
    int ok = 1;
    if (c.MaterialIndices.End - c.MaterialIndices.Begin != 1 || buffer[0] != 42 || g_resizes != 1) ok = 0;
    c.MaterialIndices.End = buffer;                                   // empty: grow to one
    c.SetMaterialIndex(-3);
    if (c.MaterialIndices.End - c.MaterialIndices.Begin != 1 || buffer[0] != -3 || g_resizes != 2) ok = 0;
    if (!ok) { std::printf("00abcf10_TEST FAIL\n"); return 1; }
    std::printf("00abcf10_TEST PASS\n");
    return 0;
}