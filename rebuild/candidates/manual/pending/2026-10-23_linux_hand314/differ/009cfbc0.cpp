// CBankFile::ReadEntryList(CFileDataInputStream& stream, unsigned long maxIndex, unsigned long entryCount) -- read the entry list written by InternalWriteEntryList: size the entry tables, read and report the per-type counts, then for each entry read its record (index, type, size, offset, state CRC, symbol, checksum, filenames, info), fill the runtime data and (by open flags) the symbol, checksum and update record, register it and report it; finally sort the CRC index, trim it, pack a read-only bank and mark the file valid
// Grade: DIFFER(1973v1969) -- 57 blocks, all register/slot assignment: `this` spill and the conditional-temp flag swap L0/L4, the copied entry index and infoSize swap dead-argument slots (0x6c/0x70), infoSize in ebx where retail has esi, and the filename pointer is held in esi/ebx; the structure, inline Read/ReadULong, SetInfo, CSmallVector Clear/Copy and PostInitEntries all line up
// Leaf: ReadEntryList
// Retail 0x009CFBC0, 1984 bytes, `ret 0xc`.
// Callees: CBankFile 0x009CEAE0 (size the entry tables for maxIndex + 1), CSTLportVector<SPair>(n) 0x009D2AF0,
//   CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (inline Read) / ReadPresizedString 0x00996390, the stream's
//   virtual slots 1 (+0x04, SetPosition) and 2 (+0x08, GetPosition); CSmallVector<CCharString,8>::Allocate 0x009D2160 /
//   Copy 0x009D21D0; CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / operator= 0x0099EFB0 / ~ 0x0099EAE0;
//   operator new 0x00BFEA1A, new[] 0x00BFEB22, delete[] 0x00BFEB1C, malloc 0x00BFEA0E, free 0x00BFEA14;
//   CBankFile 0x009CE050 (register the entry's symbol and first filename); the CRC-pair sort 0x009B85A0 (`__fastcall`),
//   CVectorMap::DeallocateUnusedMemory 0x009B7B10, CBankFile::PackRuntimeData 0x009CD740; CBankFile virtual slots
//   10 (+0x28, NotifyBankSize), 12 (+0x30, NotifyEntryAdded), 15 (+0x3c).
// Layout: the landed retail CBankFile model (0x009D1320 / 0x009CFB70): Size +0x04, Symbols +0x08, Checksums +0x14,
//   RuntimeData +0x20, UpdateData +0x2c, OpenFlags +0x78, ReadOnly +0x8d, FileValid +0x94, SymbolCRCIndices +0xd0.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
#pragma intrinsic(memcpy, memset)
void __cdecl operator delete[](void* block);          // 0x00BFEB1C
void* __cdecl operator new[](unsigned int size);      // 0x00BFEB22
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};
struct CDataInputStream {
    virtual void v00();
    virtual void SetPosition(unsigned long position);   // slot 1 (+0x04)
    virtual unsigned long GetPosition();                // slot 2 (+0x08)
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    CCharString ReadPresizedString();                   // 0x00996390
    __forceinline unsigned long ReadULong() { unsigned long v; Read(&v, 4); return v; }
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
struct CFileDataInputStream : CDataInputStream {
    void* File;
    void* Buffer;
    long  BufferSize;
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
struct CFilenameVector {                               // CSmallVector<CCharString, 8>
    CCharString* Start;
    signed char  Size;
    CFilenameVector() : Start(0), Size(0) {}
    ~CFilenameVector() { Clear(); }
    void Clear()
    {
        if (Start)
            delete[] Start;
        Start = 0;
        Size = 0;
    }
    void Allocate(unsigned long count);                // 0x009D2160
    void Copy(const CFilenameVector& other);           // 0x009D21D0
    CFilenameVector& operator=(const CFilenameVector& other)
    {
        if (this != &other)
            Copy(other);
        return *this;
    }
    CCharString& operator[](unsigned long i) { return Start[i]; }
};
struct CBankFileEntryUpdateData {
    unsigned long   StateBlockCRC;                     // +0x00
    unsigned long   InfoSize;                          // +0x04
    char*           InfoBuffer;                        // +0x08
    CFilenameVector Filenames;                         // +0x0c
    bool            RequiresUpdate;                    // +0x14
    bool            Exists;                            // +0x15
    CBankFileEntryUpdateData() : StateBlockCRC(0), InfoSize(0), InfoBuffer(0), RequiresUpdate(false), Exists(true) {}
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
template <class T> struct CArray {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& operator[](unsigned long index) const { return m_pFirst[index]; }
};
struct SPair { unsigned long first; unsigned long second; };
struct CPairVector {                                   // CSTLportVector<SPair>
    SPair* _M_start;
    SPair* _M_finish;
    SPair* _M_end_of_storage;
    CPairVector(unsigned long count);                  // 0x009D2AF0
    ~CPairVector() { if (_M_start) free(_M_start); }
    SPair& operator[](unsigned long i) { return _M_start[i]; }
};
struct CKeyPairCompareLess { unsigned char _empty; };
void __fastcall SortCRCPairs(SPair* first, SPair* last, CKeyPairCompareLess compare);   // 0x009B85A0
struct CCRCVectorMap {
    SPair*              _M_start;
    SPair*              _M_finish;
    SPair*              _M_end_of_storage;
    CKeyPairCompareLess Compare;
    bool                NeedsSort;
    void Sort()
    {
        if (NeedsSort) {
            SortCRCPairs(_M_start, _M_finish, Compare);
            NeedsSort = false;
        }
    }
    void DeallocateUnusedMemory();                     // 0x009B7B10
};
struct CBankFile_Retail {
    unsigned long             Size;                 // +0x04
    CArray<CCharString>       Symbols;              // +0x08
    CArray<unsigned long>     Checksums;            // +0x14
    CArray<CRuntimeData>      RuntimeData;          // +0x20
    CArray<CBankFileEntryUpdateData*> UpdateData;   // +0x2c
    unsigned char             _pad38[0x78 - 0x38];
    unsigned long             OpenFlags;            // +0x78
    unsigned char             _pad7c[0x8d - 0x7c];
    bool                      ReadOnly;             // +0x8d
    unsigned char             _pad8e[0x94 - 0x8e];
    bool                      FileValid;            // +0x94
    unsigned char             _pad95[0xd0 - 0x95];
    CCRCVectorMap             SymbolCRCIndices;     // +0xd0
    virtual void s00(); virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04();
    virtual void s05(); virtual void s06(); virtual void s07(); virtual void s08(); virtual void s09();
    virtual void NotifyBankSize(unsigned long size, const CPairVector& typeCounts);                 // slot 10 (+0x28)
    virtual void NotifyBankOpened();                                                                // slot 11
    virtual void NotifyEntryAdded(unsigned long type, unsigned long index, const CByteVector& info); // slot 12 (+0x30)
    virtual void NotifyEntryUpdated(unsigned long type, unsigned long index, const CByteVector& info);   // slot 13
    virtual void s14();
    virtual void OnEntryListRead();                                                                 // slot 15 (+0x3c)
    void InitEntries(unsigned long count);                                                          // 0x009CEAE0
    void RegisterEntry(unsigned long index, const CCharString& symbol, const CCharString& filename);   // 0x009CE050
    void PackRuntimeData();                                                                         // 0x009CD740
    void PostInitEntries()
    {
        SymbolCRCIndices.Sort();
        SymbolCRCIndices.DeallocateUnusedMemory();
        if (ReadOnly)
            PackRuntimeData();
    }
    bool ReadEntryList(CFileDataInputStream& stream, unsigned long maxIndex, unsigned long entryCount);
};

bool CBankFile_Retail::ReadEntryList(CFileDataInputStream& stream, unsigned long maxIndex, unsigned long entryCount)
{
    InitEntries(maxIndex + 1);
    unsigned long typeCount = stream.ReadULong();
    CPairVector typeCounts(typeCount);
    for (unsigned long t = 0; t < typeCount; ++t) {
        unsigned long type = stream.ReadULong();
        typeCounts[t].first = type;
        unsigned long count = stream.ReadULong();
        typeCounts[t].second = count;
    }
    NotifyBankSize(Size, typeCounts);
    for (unsigned long e = entryCount; e > 0; --e) {
        unsigned long marker = stream.ReadULong();
        unsigned long readIndex = stream.ReadULong();
        unsigned long index = readIndex;
        unsigned long type = stream.ReadULong();
        unsigned long size = stream.ReadULong();
        unsigned long offset = stream.ReadULong();
        unsigned long crc = stream.ReadULong();
        CCharString symbol = stream.ReadPresizedString();
        unsigned long checksum = stream.ReadULong();
        unsigned long filenameCount = stream.ReadULong();
        CFilenameVector filenames;
        filenames.Allocate(filenameCount);
        CCharString* name = filenames.Start;
        for (unsigned long f = filenameCount; f > 0; --f, ++name)
            *name = stream.ReadPresizedString();
        unsigned long infoSize = stream.ReadULong();
        CByteVector info(infoSize, 0);
        stream.Read(info.GetData(), infoSize);
        CRuntimeData& data = RuntimeData[readIndex];
        data.DataType = (unsigned char)type;
        data.DataSize = size;
        data.DataOffset = offset;
        data.Valid = true;
        if (OpenFlags & 8)
            Symbols[index] = symbol;
        if (OpenFlags & 0x10)
            Checksums[index] = checksum;
        if (OpenFlags & 0x20) {
            CBankFileEntryUpdateData* update = new CBankFileEntryUpdateData;
            update->StateBlockCRC = crc;
            update->SetInfo(info);
            update->Filenames = filenames;
            UpdateData[index] = update;
        }
        CCharString filename = filenames.Size ? filenames[0] : CCharString("", -1);
        RegisterEntry(index, symbol, filename);
        unsigned long position = stream.GetPosition();
        NotifyEntryAdded(type, index, info);
        stream.SetPosition(position);
    }
    PostInitEntries();
    OnEntryListRead();
    FileValid = true;
    return true;
}
