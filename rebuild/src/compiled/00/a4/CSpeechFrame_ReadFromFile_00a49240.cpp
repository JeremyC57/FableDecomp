// CSpeechFrame::ReadFromFile(CMemoryDataInputStream* stream) -- load one lip-sync analysis frame: skip the tag, read the frequency count, resize the array, read each frequency's real part (imaginary 0), then amplitude, median, base frequency and variance
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A49240, 699 bytes, `ret 4`. Mirror of the landed WriteToFile 0x00A48AC0.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (slow path of every inlined Read),
//   CArray<CComplexNumber>::erase(first, last) 0x00A49F20, _M_fill_insert(pos, n, const T&) 0x00A4A3D0
//   (both from the inline STLport resize).
// Layout: Ego_r.pdb CSpeechFrame (0x20) with the retail 12-byte array at +0, floats at +0x0c..+0x18.
// Shape: every value is read through a __forceinline ReadX() helper returning a local (all reads share the dead
//   stream-argument slot; the tag read keeps only the pointer/position updates); CComplexNumber has an empty
//   default ctor (resize's `T()` stores nothing) and a (real, imag) ctor whose 0.0f store is hoisted out of the loop.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CMemoryDataInputStream {
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long StreamSize;                       // +8
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long SourceChunkStreamPos;             // +0x10
    long m_remaining;                      // +0x14 bytes left in the current chunk
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
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
    __forceinline unsigned long ReadULong() { unsigned long v; Read(&v, 4); return v; }
    __forceinline float ReadFloat() { float v; Read(&v, 4); return v; }
};

struct CComplexNumber {
    float Real;
    float Imaginary;
    CComplexNumber() { }
    CComplexNumber(float real, float imaginary) : Real(real), Imaginary(imaginary) { }
};

template <class T> struct CArray {                  // 12-byte retail STLport vector {first, last, end}
    T* m_first; T* m_last; T* m_end;
    unsigned long size() const { return m_last - m_first; }
    T* begin() { return m_first; }
    T* end() { return m_last; }
    T& operator[](unsigned long i) { return m_first[i]; }
    T* erase(T* first, T* last);                                    // 0x00A49F20
    void _M_fill_insert(T* position, unsigned long n, const T& x);  // 0x00A4A3D0
    void insert(T* position, unsigned long n, const T& x) { _M_fill_insert(position, n, x); }
    void resize(unsigned long newSize, const T& x)
    {
        if (newSize < size())
            erase(begin() + newSize, end());
        else
            insert(end(), newSize - size(), x);
    }
    void resize(unsigned long newSize) { resize(newSize, T()); }
};

struct CSpeechFrame {
    CArray<CComplexNumber> Frequencies;             // +0x00
    float Amplitude;                                // +0x0c
    float MedianFrequency;                          // +0x10
    float BaseFrequency;                            // +0x14
    float FrequencyVariance;                        // +0x18
    void ReadFromFile(CMemoryDataInputStream* stream);
};

void CSpeechFrame::ReadFromFile(CMemoryDataInputStream* stream)
{
    stream->ReadULong();
    unsigned long count = stream->ReadULong();
    Frequencies.resize(count);
    for (unsigned long i = 0; i < count; ++i)
        Frequencies[i] = CComplexNumber(stream->ReadFloat(), 0.0f);
    Amplitude = stream->ReadFloat();
    MedianFrequency = stream->ReadFloat();
    BaseFrequency = stream->ReadFloat();
    FrequencyVariance = stream->ReadFloat();
}