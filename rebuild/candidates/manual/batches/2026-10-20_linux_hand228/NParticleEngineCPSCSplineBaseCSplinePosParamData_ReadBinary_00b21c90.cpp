// NParticleEngine::CPSCSplineBase::CSplinePosParamData::ReadBinary(CDataInputStream& in) -- read MaintainSplineShape, the parameter count, size SplinePosParam and read one unsigned long per entry
// Leaf: ReadBinary@CSplinePosParamData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B21C90, length 313 bytes (`ret 4`).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0; CArray<unsigned long>::resize(n, const unsigned long&)
// @ 0x00464931 (manifest label "push_back" is wrong; landed family shape, see the ctor 0x00B21A00).
// Layout (PDB CSplinePosParamData, retail-sized, as in the landed ctor 0x00B21A00): Spline +0x4, MaintainSplineShape +0x8,
// SplinePosParam +0xC (retail three-pointer CArray).
// Shape: the twin of the landed CMirroredTextureHighlightsChunk::ReadFromFile 0x00AC5080 preceded by an inlined by-value
// ReadBool() (`test bl,bl ; setne cl`): the count in ebx, `resize(count, 0)` with the zero temporary in the dead
// stream-argument slot, then a loop of inlined ReadULong() reads stored through `*(begin() + i)`; the array address is
// kept in the `push ecx` frame word.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

// CDataInputStream (retail CMemoryDataInputStream layout, as in the landed 0x00701990 / 0x00AC5080):
// +4 read position, +0xC current chunk read pointer, +0x14 bytes left in the chunk.
struct CDataInputStream {
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long m_08;
    const unsigned char* m_pData;          // +0xC
    long m_10;
    long m_remaining;                      // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, sizeof(value)); return value; }
    __forceinline bool ReadBool() { unsigned char value; Read(&value, sizeof(value)); return value != 0; }
};

template<class T> struct CArray {                       // retail CArray<T>: begin/end/capacity
    T* m_begin; T* m_end; T* m_capacity;
    T* begin() { return m_begin; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    void resize(unsigned long n, const T& value);       // 0x00464931 for T = unsigned long
};

struct CParticleSystem;
struct CCardinalSpline;

struct CSplinePosParamData {
    CParticleSystem*      PParentParticleSystem;         // +0x00
    CCardinalSpline*      Spline;                        // +0x04
    bool                  MaintainSplineShape;           // +0x08
    CArray<unsigned long> SplinePosParam;                // +0x0C
    void ReadBinary(CDataInputStream& in);
};

void CSplinePosParamData::ReadBinary(CDataInputStream& in)
{
    MaintainSplineShape = in.ReadBool();
    unsigned long count = in.ReadULong();
    SplinePosParam.resize(count, 0);
    for (unsigned long i = 0; i < count; ++i)
        SplinePosParam[i] = in.ReadULong();
}
