// CBankFile::UpdateEntry(unsigned long index, CDataInputStream& data, unsigned long dataSize, CDataInputStream& info, unsigned long infoSize, bool computeChecksum) -- CRC the entry's disk filenames into its checksum slot, store the info bytes on the update record, resize the entry's data and write it at its aligned offset, clear RequiresUpdate and notify
// Leaf: ?UpdateEntry@CBankFile@@QAEXKAAVCDataInputStream@@K0K_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D1320, 726 bytes (`ret 0x18`, esp frame 0x20).
// Callees: CPackedUIntArray::operator[] @ 0x00A629F0, virtual IsDiskFile (slot 17, +0x44), CCharString copy ctor
//          @ 0x0099EC30 / dtor @ 0x0099EAE0 / ToWideString @ 0x0099E480, CWideString dtor @ 0x0099B510,
//          CChecksumCacheFile::GetChecksum @ 0x00A60FB0, g_crc32Table @ 0x0129A168, malloc @ 0x00BFEA0E / free
//          @ 0x00BFEA14 (CArray<char> storage), CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0,
//          operator delete[] @ 0x00BFEB1C / new[] @ 0x00BFEB22 (inlined SetInfo, landed 0x009CC6F0),
//          InternalResizeEntryData @ 0x009D1260, WriteEntryData @ 0x009CD330, virtual NotifyEntryUpdated (slot 13, +0x34).
// Layout: CBankFile Checksums +0x14, RuntimeData +0x20, UpdateData +0x2c, PackedDataOffset +0x38, PackedDataType +0x58,
//          Alignment +0x98, ChecksumCache +0xf8; CBankFileEntryUpdateData InfoSize +0x04, InfoBuffer +0x08,
//          Filenames +0x0c/+0x10 (CSmallVector<CCharString,8>), RequiresUpdate +0x14 (three pad bytes after the count).
// Shape: `Align(dataSize)` is a named local computed before the WriteEntryData statement (so the padding is simple and
//          the GetEntryDataOffset call-argument is evaluated first: dataSize/index land in edi/ebx, the div/imul run
//          before the IsPacked test); the CArray<char>(n, value) constructor fills through a local `first` pointer
//          and derives _M_finish from it (STLport uninitialized_fill_n), which keeps the start in edx across the
//          inline memset instead of reloading it from the frame.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
#pragma intrinsic(memcpy, memset)
void __cdecl operator delete[](void* block);          // 0x00BFEB1C
void* __cdecl operator new[](unsigned int size);      // 0x00BFEB22
extern const unsigned long g_crc32Table[256];         // 0x0129A168
struct CWideString {
    void* PStringData;
    ~CWideString();                                    // 0x0099B510
};
struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CWideString ToWideString() const;                  // 0x0099E480
};
struct CDataInputStream {
    void* __vftable;
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
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
struct CBankFileEntryUpdateData {
    unsigned long StateBlockCRC;                       // +0x00
    unsigned long InfoSize;                            // +0x04
    char*         InfoBuffer;                          // +0x08
    CCharString*  Filenames_Start;                     // +0x0c CSmallVector<CCharString,8>
    signed char   Filenames_Size;                      // +0x10
    unsigned char _pad11[3];
    bool          RequiresUpdate;                      // +0x14
    bool          Exists;                              // +0x15
    void SetInfo(const CByteVector& info)              // 0x009CC6F0 (landed), inlined
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
    unsigned long operator[](unsigned long index) const;   // 0x00A629F0
    bool IsPacked() const { return m_pData != 0; }
};
template <class T> struct CArray {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
};
struct CChecksumCacheFile {
    unsigned long GetChecksum(const CWideString& path);   // 0x00A60FB0
};
struct CBankFile_Retail {
                                                    // +0x00 vptr
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
    virtual void s00(); virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05(); virtual void s06();
    virtual void s07(); virtual void s08(); virtual void s09(); virtual void s10(); virtual void s11(); virtual void s12();
    virtual void NotifyEntryUpdated(unsigned long type, unsigned long index, const CByteVector& info);   // slot 13 (+0x34)
    virtual void s14(); virtual void s15(); virtual void s16();
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
    void InternalResizeEntryData(unsigned long index, unsigned long size);   // 0x009D1260
    void WriteEntryData(unsigned long position, CDataInputStream& source, unsigned long size, unsigned long padding);   // 0x009CD330
    void UpdateEntry(unsigned long index, CDataInputStream& data, unsigned long dataSize, CDataInputStream& info, unsigned long infoSize, bool computeChecksum);
};

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
