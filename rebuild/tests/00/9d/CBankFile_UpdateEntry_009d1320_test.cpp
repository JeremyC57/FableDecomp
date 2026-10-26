// Behaviour test for CBankFile::UpdateEntry (0x009D1320): model and body verbatim, every callee stubbed; exercises the
// packed and runtime-table type/offset paths, the checksum loop (disk and non-disk filenames, computeChecksum off),
// the info copy, the resize/write arguments (aligned padding) and the update-record state.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#define _snprintf snprintf
typedef long long __int64;
#define long int   // g++ host: keep Fable's 4-byte unsigned long (sizeof(crc) loop)
#endif
#define malloc test_malloc
#define free test_free
extern "C" void* __cdecl malloc(unsigned int size);
extern "C" void __cdecl free(void* p);
static int g_mallocs = 0, g_frees = 0;
extern "C" void* __cdecl test_malloc(unsigned int size) { g_mallocs++; return ::operator new(size); }
extern "C" void __cdecl test_free(void* p) { if (p) g_frees++; ::operator delete(p); }
extern unsigned long g_crc32Table[256];           // test only: filled at run time (retail: const table @ 0x0129A168)
class CWideString { public:
    void* PStringData;
    ~CWideString();
};
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);
    ~CCharString();
    CWideString ToWideString() const;
};
class CDataInputStream { public:
    void* __vftable;
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dst, long size) {
        if (size <= 0) return;
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(dst, CurrentSourcePtr, size); CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
};
struct CByteVector {                                   // CArray<char> (STLport vector, 12 bytes)
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    __forceinline CByteVector(unsigned long n, char value) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = n != 0 ? (char*)malloc(n) : 0;
        _M_end_of_storage = _M_start + n;
        char* first = _M_start;
        memset(first, value, n);
        _M_finish = first + n;
    }
    ~CByteVector() { if (_M_start) free(_M_start); }
    long GetCount() const { return _M_finish - _M_start; }
    char* GetData() { return _M_finish - _M_start != 0 ? _M_start : (char*)this; }
    const char* GetData() const { return _M_finish - _M_start != 0 ? _M_start : (const char*)this; }
};
class CBankFileEntryUpdateData { public:
    unsigned long StateBlockCRC;                       // +0x00
    unsigned long InfoSize;                            // +0x04
    char*         InfoBuffer;                          // +0x08
    CCharString*  Filenames_Start;                     // +0x0c CSmallVector<CCharString,8>
    signed char   Filenames_Size;                      // +0x10
    unsigned char _pad11[3];
    bool          RequiresUpdate;                      // +0x14
    bool          Exists;                              // +0x15
    void SetInfo(const CByteVector& info)
    {
        if (InfoBuffer != 0) {
            delete[] InfoBuffer;
            InfoBuffer = 0;
        }
        InfoSize = 0;
        InfoSize = info.GetCount();
        if (InfoSize != 0) {
            InfoBuffer = new char[InfoSize];
            memcpy(InfoBuffer, info.GetData(), InfoSize);
        }
    }
};
struct CRuntimeData {
    unsigned long DataOffset;
    unsigned long DataSize;
    unsigned char DataType;
    bool          Valid;
    unsigned char _pad[2];
};
struct CPackedUIntArray {
    unsigned long  m_bitsPerValue;
    unsigned long* m_pData;
    unsigned long  m_count;
    unsigned long  m_capacity;
    unsigned long operator[](unsigned long index) const;
    bool IsPacked() const { return m_pData != 0; }
};
template <class T> class CArray { public:
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
};
struct CChecksumCacheFile {
    unsigned long GetChecksum(const CWideString& path);
};
struct CBankFile_Retail {
    unsigned long             Size;                 // +0x04
    CArray<void*>             Symbols;              // +0x08
    CArray<unsigned long>     Checksums;            // +0x14
    CArray<CRuntimeData>      RuntimeData;          // +0x20
    CArray<CBankFileEntryUpdateData*> UpdateData;   // +0x2c
    CPackedUIntArray          PackedDataOffset;     // +0x38
    CPackedUIntArray          PackedDataSize;       // +0x48
    CPackedUIntArray          PackedDataType;       // +0x58
    CPackedUIntArray          PackedValid;          // +0x68
    unsigned char             _pad78[0x98 - 0x78];
    unsigned long             Alignment;            // +0x98
    unsigned char             _pad9c[0xf8 - 0x9c];
    CChecksumCacheFile        ChecksumCache;        // +0xf8
    virtual void s00() {} virtual void s01() {} virtual void s02() {} virtual void s03() {} virtual void s04() {} virtual void s05() {} virtual void s06() {}
    virtual void s07() {} virtual void s08() {} virtual void s09() {} virtual void s10() {} virtual void s11() {} virtual void s12() {}
    virtual void NotifyEntryUpdated(unsigned long type, unsigned long index, const CByteVector& info);   // slot 13 (+0x34)
    virtual void s14() {} virtual void s15() {} virtual void s16() {}
    virtual bool IsDiskFile(unsigned long type, unsigned long fileIndex);   // slot 17 (+0x44)
    unsigned long GetEntryDataOffset(unsigned long index) const
    {
        if (PackedDataOffset.IsPacked())
            return PackedDataOffset[index - 1];
        return RuntimeData[index].DataOffset;
    }
    unsigned long GetEntryDataType(unsigned long index) const
    {
        if (PackedDataType.IsPacked())
            return PackedDataType[index - 1];
        return RuntimeData[index].DataType;
    }
    unsigned long Align(unsigned long size) const { return (size + Alignment - 1) / Alignment * Alignment; }
    void InternalResizeEntryData(unsigned long index, unsigned long size);
    void WriteEntryData(unsigned long position, CDataInputStream& source, unsigned long size, unsigned long padding);
    void UpdateEntry(unsigned long index, CDataInputStream& data, unsigned long dataSize, CDataInputStream& info, unsigned long infoSize, bool computeChecksum);
};

// ---- body verbatim (final.cpp) ----
void CBankFile_Retail::UpdateEntry(unsigned long index, CDataInputStream& data, unsigned long dataSize, CDataInputStream& info, unsigned long infoSize, bool computeChecksum)
{
    CBankFileEntryUpdateData* update = UpdateData[index];
    unsigned long type = GetEntryDataType(index);
    unsigned long checksum = 0;
    for (unsigned long i = 0; i < (unsigned long)update->Filenames_Size; ++i) {
        if (computeChecksum && IsDiskFile(type, i)) {
            CCharString filename = update->Filenames_Start[i];
            unsigned long crc = ChecksumCache.GetChecksum(filename.ToWideString());
            const unsigned char* bytes = (const unsigned char*)&crc;
            for (unsigned long b = 0; b < sizeof(crc); ++b)
                checksum = g_crc32Table[bytes[b] ^ (checksum & 0xff)] ^ (checksum >> 8);
        }
    }
    Checksums[index] = checksum;
    CByteVector infoBytes(infoSize, 0);
    info.Read(infoBytes.GetData(), infoSize);
    update->SetInfo(infoBytes);
    InternalResizeEntryData(index, dataSize);
    unsigned long aligned = Align(dataSize);
    WriteEntryData(GetEntryDataOffset(index), data, dataSize, aligned - dataSize);
    update->RequiresUpdate = false;
    NotifyEntryUpdated(type, index, infoBytes);
}

// ---- stubs ----
static unsigned long g_table[256];
unsigned long g_crc32Table[256];
static int g_strCopies = 0, g_strDtors = 0, g_wideDtors = 0, g_overflowReads = 0;
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { g_strCopies++; }
CCharString::~CCharString() { g_strDtors++; PStringData = 0; }
CWideString CCharString::ToWideString() const { CWideString w; w.PStringData = PStringData; return w; }
CWideString::~CWideString() { g_wideDtors++; }
unsigned long CChecksumCacheFile::GetChecksum(const CWideString& path) { return (unsigned long)strlen((const char*)path.PStringData) * 0x01010101UL; }
void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char* buffer, long size) { g_overflowReads++; memset(buffer, 0xAB, size); StreamPos += size; }
static unsigned long g_packedValues[8];
unsigned long CPackedUIntArray::operator[](unsigned long index) const { return g_packedValues[index] + m_bitsPerValue; }
static int g_diskMask = 0, g_diskCalls = 0; static unsigned long g_diskType = 0;
bool CBankFile_Retail::IsDiskFile(unsigned long type, unsigned long fileIndex) { g_diskCalls++; g_diskType = type; return (g_diskMask >> fileIndex) & 1; }
static unsigned long g_resizeIndex, g_resizeSize, g_writePos, g_writeSize, g_writePad, g_notifyType, g_notifyIndex, g_notifyInfoSize; static int g_notifyRequires = -1;
static CDataInputStream* g_writeSource;
void CBankFile_Retail::InternalResizeEntryData(unsigned long index, unsigned long size) { g_resizeIndex = index; g_resizeSize = size; }
void CBankFile_Retail::WriteEntryData(unsigned long position, CDataInputStream& source, unsigned long size, unsigned long padding) { g_writePos = position; g_writeSource = &source; g_writeSize = size; g_writePad = padding; }
void CBankFile_Retail::NotifyEntryUpdated(unsigned long type, unsigned long index, const CByteVector& info)
{
    g_notifyType = type; g_notifyIndex = index; g_notifyInfoSize = info.GetCount();
    g_notifyRequires = UpdateData[index]->RequiresUpdate ? 1 : 0;
}

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)

static unsigned long RefCrc(const unsigned long* table, unsigned long seed, unsigned long value)
{
    unsigned long c = seed;
    for (int b = 0; b < 4; ++b) c = table[((value >> (8 * b)) & 0xff) ^ (c & 0xff)] ^ (c >> 8);
    return c;
}

int main()
{
    unsigned long* table = g_crc32Table;
    for (unsigned long n = 0; n < 256; ++n) { unsigned long c = n; for (int k = 0; k < 8; ++k) c = (c & 1) ? 0xEDB88320UL ^ (c >> 1) : c >> 1; table[n] = c; g_table[n] = c; }

    CBankFile_Retail bank; memset((char*)&bank + sizeof(void*), 0, sizeof(bank) - sizeof(void*));   // keep the vptr
    unsigned long checksums[4] = { 0x11, 0x22, 0x33, 0x44 };
    CRuntimeData runtime[4]; memset(runtime, 0, sizeof(runtime));
    runtime[2].DataOffset = 0x1000; runtime[2].DataType = 7;
    CBankFileEntryUpdateData* records[4]; CBankFileEntryUpdateData rec; memset(&rec, 0, sizeof(rec));
    char nameA[] = "alpha.tex", nameB[] = "beta.mesh", nameC[] = "gamma";
    CCharString names[3] = { *(CCharString*)&nameA, *(CCharString*)&nameB, *(CCharString*)&nameC };   // copies (counted, reset below)
    names[0].PStringData = nameA; names[1].PStringData = nameB; names[2].PStringData = nameC;
    rec.Filenames_Start = names; rec.Filenames_Size = 3; rec.RequiresUpdate = true; rec.Exists = true;
    records[2] = &rec;
    bank.Checksums.m_pFirst = checksums; bank.Checksums.m_pLast = checksums + 4;
    bank.RuntimeData.m_pFirst = runtime; bank.RuntimeData.m_pLast = runtime + 4;
    bank.UpdateData.m_pFirst = records; bank.UpdateData.m_pLast = records + 4;
    bank.Alignment = 16;
    unsigned char infoSrc[16]; for (int i = 0; i < 16; ++i) infoSrc[i] = (unsigned char)(0x30 + i);
    CDataInputStream info; memset(&info, 0, sizeof(info)); info.CurrentSourcePtr = infoSrc; info.SourceChunkBytesRemaining = 16;
    CDataInputStream data; memset(&data, 0, sizeof(data));

    // 1. runtime tables, checksum over disk files 0 and 2, 6-byte info, 100-byte data (padding 12)
    g_strCopies = g_strDtors = g_wideDtors = 0; g_diskMask = 5; g_diskCalls = 0; g_mallocs = g_frees = 0;
    bank.UpdateEntry(2, data, 100, info, 6, true);
    CHECK(g_diskCalls == 3 && g_diskType == 7);
    CHECK(checksums[2] == RefCrc(g_table, RefCrc(g_table, 0, 9 * 0x01010101UL), 5 * 0x01010101UL));
    CHECK(g_strCopies == 2 && g_strDtors == 2 && g_wideDtors >= 2);   // the stub returns CWideString by value (no NRVO: extra temp dtors)
    CHECK(rec.InfoSize == 6 && rec.InfoBuffer != 0 && memcmp(rec.InfoBuffer, infoSrc, 6) == 0);
    CHECK(info.StreamPos == 6 && info.SourceChunkBytesRemaining == 10 && g_overflowReads == 0);
    CHECK(g_resizeIndex == 2 && g_resizeSize == 100);
    CHECK(g_writePos == 0x1000 && g_writeSource == &data && g_writeSize == 100 && g_writePad == 12);
    CHECK(rec.RequiresUpdate == false && g_notifyRequires == 0 && g_notifyType == 7 && g_notifyIndex == 2 && g_notifyInfoSize == 6);
    CHECK(g_mallocs == 1 && g_frees == 1);

    // 2. packed tables (index-1 lookups), checksum disabled, aligned size (no padding), 0-byte info frees the old buffer
    unsigned long packedType[8] = { 0 }, packedOffset[8] = { 0 };
    bank.PackedDataType.m_pData = packedType; bank.PackedDataType.m_bitsPerValue = 3; g_packedValues[1] = 2;    // type = 5
    bank.PackedDataOffset.m_pData = packedOffset; bank.PackedDataOffset.m_bitsPerValue = 0x2000;              // offset = 0x2002
    g_strCopies = g_strDtors = 0; g_diskMask = 7; g_diskCalls = 0; rec.RequiresUpdate = true; g_mallocs = g_frees = 0;
    char* oldInfo = rec.InfoBuffer;
    bank.UpdateEntry(2, data, 64, info, 0, false);
    CHECK(g_diskCalls == 0 && g_strCopies == 0 && checksums[2] == 0);
    CHECK(rec.InfoSize == 0 && rec.InfoBuffer == 0 && oldInfo != 0);
    CHECK(g_resizeSize == 64 && g_writePos == 0x2002 && g_writeSize == 64 && g_writePad == 0);
    CHECK(g_notifyType == 5 && g_notifyIndex == 2 && g_notifyInfoSize == 0 && rec.RequiresUpdate == false);
    CHECK(g_mallocs == 0 && g_frees == 0);

    // 3. info larger than the chunk: the overflow reader
    g_overflowReads = 0; bank.UpdateEntry(2, data, 1, info, 12, false);
    CHECK(g_overflowReads == 1 && rec.InfoSize == 12 && rec.InfoBuffer[0] == (char)0xAB && g_writePad == 15);
    delete[] rec.InfoBuffer;

    char line[64];
    _snprintf(line, sizeof(line), "009d1320_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}