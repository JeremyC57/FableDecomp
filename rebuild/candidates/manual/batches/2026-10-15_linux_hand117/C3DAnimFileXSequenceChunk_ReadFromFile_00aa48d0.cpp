// C3DAnimFileXSequenceChunk::ReadFromFile(CDataInputStream& in, unsigned long size, CChunkedFileLoader& loader) -- the chunk payload is the raw compressed track: size the array, then read the bytes straight into it
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA48D0, 160 bytes. Callees: vector<unsigned char>::resize(n, const value&)
// 0x00486189 (out of line), CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0.
// The PDB header (engine/C3DAnimFileXSequenceChunk.h) keeps CompressedData (+0x10) opaque, so the array is
// modelled as in the landed WriteToFile twin (0x00AA4650): retail CArray = begin/end/capacity, and the data
// accessor hands back the array's own address when it is empty. The fill value is a zero byte homed in the
// `push ecx` frame slot. The stream read is the inlined Read(buffer, size) (64-bit position range check,
// inline `rep movsd/movsb` memcpy and re-read pointer/count/position updates, else the overflow reader),
// skipped for a non-positive size. Branch polarity matters: the accessor tests `size != 0` first (begin is
// the fall-through value) and the read tests `size <= remaining` (`cmp size,[rem] ; jg overflow`).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CDataInputStream {
public:
    void* m_pVtable;                       // +0x00
    unsigned long m_position;              // +0x04
    long m_08;
    const unsigned char* m_pData;          // +0x0C current chunk read pointer
    long m_10;
    long m_remaining;                      // +0x14 bytes left in the current chunk
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    __forceinline void Read(void* buffer, long size)
    {
        if (size <= 0)
            return;
        __int64 newPosition = (__int64)(m_position + size);
        if (newPosition < 0 || newPosition > 0x7fffffff)
            return;
        if (size <= m_remaining) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
class CChunkedFileLoader;

template <class T>
struct CArray {                             // retail CArray is 12 bytes (the PDB's is 16)
    T* m_pBegin;                            // +0
    T* m_pEnd;                              // +4
    T* m_pEndOfStorage;                     // +8
    void resize(unsigned long n, const T& value);   // 0x00486189
};

inline unsigned char* GetData(CArray<unsigned char>& a)
{
    if (a.m_pEnd - a.m_pBegin != 0)
        return a.m_pBegin;
    return reinterpret_cast<unsigned char*>(&a);
}

struct C3DAnimFileXSequenceChunk {
    virtual ~C3DAnimFileXSequenceChunk();
    unsigned char _base_body[0x0C];         // +0x04 .. +0x0F (CChunkedFileChunk: type, parent)
    CArray<unsigned char> CompressedData;   // +0x10
    virtual void ReadFromFile(CDataInputStream& in, unsigned long size, CChunkedFileLoader& loader);
};

void C3DAnimFileXSequenceChunk::ReadFromFile(CDataInputStream& in, unsigned long size, CChunkedFileLoader& loader)
{
    CompressedData.resize(size, 0);
    in.Read(GetData(CompressedData), size);
}
