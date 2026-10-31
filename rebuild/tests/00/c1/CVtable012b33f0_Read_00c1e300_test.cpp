// Behaviour test for 0x00C1E300 CVtable_012b33f0::Read: the body over a fake sample, chunk reader and decoder.
// Checks the copy failure, plain PCM (offset = header + data size, size = format size - header), the compressed
// chunk unwrap (offset += start, size = end), the encoded path (data at the offset handed to the decoder, offset
// reset to 0) and the decoder failure.
#include <cstdio>
#include <cstring>
static int g_lockDepth, g_lockCount;
struct PKCProtectionInstance { PKCProtectionInstance(void*) { g_lockDepth++; g_lockCount++; } ~PKCProtectionInstance() { g_lockDepth--; } };
static int g_lockObj;
struct CSoundFormat {
    unsigned long Size, m_reserved1, m_reserved2; unsigned short Channels, Encoding; unsigned long SampleRate;
    unsigned long GetPCMBufferSize() { return Size * 4; }
};
struct CSoundSample {
    long copyResult, compressed, header, dataSize; CSoundFormat m_format;
    long CopyData(void* buffer) { std::memcpy(buffer, "HDRxABCDEFGHIJKL", 16); return copyResult; }
    long IsCompressed() { return compressed; }
    long GetHeaderSize() { return header; }
    long GetDataSize() { return dataSize; }
};
static long g_chunkResult, g_chunkStart, g_chunkEnd; static void* g_chunkSrc; static unsigned long g_chunkSize;
static long ReadChunkExtent_00c347b0(void* first, void* second, long* outStart, long* outEnd)
{ g_chunkSrc = first; g_chunkSize = (unsigned long)second; *outStart = g_chunkStart; *outEnd = g_chunkEnd; return g_chunkResult; }
static long g_decodeResult; static char g_decodeSeen[16]; static unsigned long g_decodeDstSize, g_decodeFmtSize;
static long DecodeSoundData_00c34630(CSoundFormat* format, void* src, void* dst, unsigned long dstSize)
{ std::memcpy(g_decodeSeen, src, format->Size); g_decodeFmtSize = format->Size; g_decodeDstSize = dstSize; (void)dst; return g_decodeResult; }
struct CVtable_012b33f0 {
    CSoundSample* m_pSample;
    long Read(void* buffer, long* pOffset, long* pSize)
    {
        PKCProtectionInstance lock(&g_lockObj);
        if (m_pSample->CopyData(buffer) < 0)
            return -1;
        CSoundFormat format = m_pSample->m_format;
        *pOffset = m_pSample->GetHeaderSize() + m_pSample->GetDataSize();
        format.Size -= m_pSample->GetHeaderSize();
        if (m_pSample->IsCompressed()) {
            long start;
            long end;
            void* src = (char*)buffer + *pOffset;
            if (ReadChunkExtent_00c347b0(src, (void*)format.Size, &start, &end) < 0)
                return -1;
            *pOffset += start;
            format.Size = end;
        }
        if (format.Encoding != 0) {
            unsigned long pcmSize = format.GetPCMBufferSize();
            unsigned char* pcm = new unsigned char[pcmSize];
            std::memcpy(pcm, (char*)buffer + *pOffset, format.Size);
            long result = DecodeSoundData_00c34630(&format, pcm, buffer, pcmSize);
            delete [] pcm;
            *pOffset = 0;
            if (result < 0)
                return -1;
        }
        *pSize = format.Size;
        return 0;
    }
};
int main()
{
    int fails = 0;
    char buf[64]; long off = -7, size = -7;
    CSoundSample s; std::memset(&s, 0, sizeof s);
    CVtable_012b33f0 r; r.m_pSample = &s;
    s.copyResult = -1;
    if (r.Read(buf, &off, &size) != -1 || off != -7 || size != -7 || g_lockDepth != 0) fails |= 1;
    s.copyResult = 0; s.header = 4; s.dataSize = 0; s.m_format.Size = 12;
    if (r.Read(buf, &off, &size) != 0 || off != 4 || size != 8) fails |= 2;
    s.compressed = 1; g_chunkStart = 2; g_chunkEnd = 5; g_chunkResult = 0;
    if (r.Read(buf, &off, &size) != 0 || off != 6 || size != 5 || g_chunkSrc != buf + 4 || g_chunkSize != 8) fails |= 4;
    s.m_format.Encoding = 1; g_decodeResult = 1;
    if (r.Read(buf, &off, &size) != 0 || off != 0 || size != 5 || std::memcmp(g_decodeSeen, "CDEFG", 5) != 0
        || g_decodeDstSize != 20 || g_decodeFmtSize != 5) fails |= 8;
    g_chunkResult = -1; off = 99;
    if (r.Read(buf, &off, &size) != -1 || off != 4) fails |= 16;
    g_chunkResult = 0; g_decodeResult = -3;
    if (r.Read(buf, &off, &size) != -1 || off != 0 || g_lockDepth != 0 || g_lockCount != 6) fails |= 32;
    if (fails) { std::printf("00c1e300_TEST FAIL %d\n", fails); return 1; }
    std::printf("00c1e300_TEST PASS\n");
    return 0;
}