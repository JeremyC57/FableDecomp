#include <cstdio>
// behaviour test: CSpeechFrame::ReadFromFile reads tag, count, resized real parts and the four floats (fast and overflow paths)


#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
typedef long long __int64;
#endif

struct CMemoryDataInputStream {
    void* m_pVtable;                       // +0
    unsigned int m_position;              // +4
    long StreamSize;                       // +8
    const unsigned char* m_pData;          // +0xC  current chunk read pointer
    long SourceChunkStreamPos;             // +0x10
    long m_remaining;                      // +0x14 bytes left in the current chunk
    int overflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size) { ++overflowCalls; std::memcpy(buffer, m_pData, size); m_pData += size; m_position += size; }
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
    __forceinline unsigned int ReadULong() { unsigned int v; Read(&v, 4); return v; }
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
    unsigned int size() const { return m_last - m_first; }
    T* begin() { return m_first; }
    T* end() { return m_last; }
    T& operator[](unsigned int i) { return m_first[i]; }
    T* erase(T* first, T* last) { T* d = first; for (T* s = last; s != m_last; ++s, ++d) *d = *s; m_last = d; return first; }
    void _M_fill_insert(T* position, unsigned int n, const T& x) { (void)position; for (unsigned int k = 0; k < n; ++k) *m_last++ = x; }
    void insert(T* position, unsigned int n, const T& x) { _M_fill_insert(position, n, x); }
    void resize(unsigned int newSize, const T& x)
    {
        if (newSize < size())
            erase(begin() + newSize, end());
        else
            insert(end(), newSize - size(), x);
    }
    void resize(unsigned int newSize) { resize(newSize, T()); }
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
    unsigned int count = stream->ReadULong();
    Frequencies.resize(count);
    for (unsigned int i = 0; i < count; ++i)
        Frequencies[i] = CComplexNumber(stream->ReadFloat(), 0.0f);
    Amplitude = stream->ReadFloat();
    MedianFrequency = stream->ReadFloat();
    BaseFrequency = stream->ReadFloat();
    FrequencyVariance = stream->ReadFloat();
}

int main()
{
    static CComplexNumber storage[16];
    CSpeechFrame frame;
    frame.Frequencies.m_first = frame.Frequencies.m_last = storage; frame.Frequencies.m_end = storage + 16;
    float data[16]; unsigned int words[16];
    words[0] = 0x0ff7da7a; words[1] = 3;
    float vals[7] = { 1.5f, 2.5f, 3.5f, 0.25f, 100.0f, 50.0f, 7.0f };
    std::memcpy(data, words, 8); std::memcpy(data + 2, vals, sizeof(vals));
    CMemoryDataInputStream s; s.m_pVtable = 0; s.m_position = 0; s.StreamSize = 36; s.m_pData = (const unsigned char*)data;
    s.SourceChunkStreamPos = 0; s.m_remaining = 20; s.overflowCalls = 0;
    storage[1].Imaginary = 9.0f;
    frame.ReadFromFile(&s);
    bool ok = frame.Frequencies.size() == 3 && storage[0].Real == 1.5f && storage[1].Real == 2.5f && storage[2].Real == 3.5f
        && storage[1].Imaginary == 0.0f && frame.Amplitude == 0.25f && frame.MedianFrequency == 100.0f
        && frame.BaseFrequency == 50.0f && frame.FrequencyVariance == 7.0f && s.m_position == 36 && s.overflowCalls == 4;
    /* shrink: a second frame with count 1 erases the tail */
    words[1] = 1; std::memcpy(data, words, 8);
    s.m_position = 0; s.m_pData = (const unsigned char*)data; s.m_remaining = 100;
    frame.ReadFromFile(&s);
    ok = ok && frame.Frequencies.size() == 1 && storage[0].Real == 1.5f && frame.Amplitude == 2.5f && frame.FrequencyVariance == 100.0f;
    if (!ok) { std::printf("00a49240_TEST FAIL\n"); return 1; }
    std::printf("00a49240_TEST PASS\n");
    return 0;
}