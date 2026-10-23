// CBankFile::ReadFileInformation(bool throwOnError) -- read the bank header: "BBBB" magic, bank format, data version, update-in-progress flag and alignment, then the entry-list position/sizes; a mismatch either asks for a rebuild (writable bank, logged, returns true) or fails (read-only bank: throws or returns false); finally read the entry list
// Grade: DIFFER(1570v1635) -- 42 blocks: `__forceinline` exception ctor plus `#pragma inline_depth(0)` around the third (version) throw gives retail's inline/inline/out-of-line 0x009D1E70 split; residue is one extra frame word (0x50 v 0x4c) shifting every slot, the first two inlined message temps not reusing the dead parameter slot, and the invalid-file throw laid out after the compare in retail
// Leaf: ReadFileInformation
// Flags: /GS
// Retail 0x009D0880, 1648 bytes, `ret 4`, /GS cookie 0x0139C8A8 / check 0x00BFE9F9, throws via _CxxThrowException 0x00BFEB84
//   (throw info 0x01369470 for a 4-byte exception holding an error code).
// Callees: CWideString::ConvertToCharString 0x0099B2C0; CFileDataInputStream(CAFile*, long) 0x00994700 / ~ 0x00994780;
//   CDataInputStream::SetPosition 0x00993BC0 / ReadWithSrcChunkOverflow 0x00993CA0; CCharString(const char*, long)
//   0x0099EBF0 / ~ 0x0099EAE0 / c_str 0x0099E4C0 (out of line); operator+(const CCharString&, const char*) 0x0099F600 and
//   (const CCharString&, const CCharString&) 0x0099F570 (`__fastcall`); long-to-string 0x0099F830 (`__fastcall`);
//   the exception's (CCharString, long) ctor 0x009D1E70; CBankFile vslot 8 (+0x20, data version), the file's vslot 9
//   (+0x24, length), the log's vslot 2 (+0x08); CBankFile 0x009D0380 (recover an interrupted update); ReadEntryList
//   0x009CFBC0.
// Layout: the landed retail CBankFile model (0x009D1160 OpenFileReadWrite): File +0x7c, Filename +0x84, ReadOnly +0x8d,
//   Alignment +0x98, BankFormat +0x9c, FileInformationStart +0xa0, FileSize +0xa8, Log +0xec.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int bytes);
#pragma intrinsic(memcpy, memcmp)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    const char* c_str() const;                         // 0x0099E4C0
};
CCharString __fastcall operator+(const CCharString& a, const char* b);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& a, const CCharString& b);   // 0x0099F570
CCharString __fastcall ToCharString(long value);                                // 0x0099F830
struct CWideString {
    void* PStringData;
    CCharString ConvertToCharString() const;           // 0x0099B2C0
};
struct CAFile {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual long GetLength();                          // slot 9 (+0x24)
};
struct CFilePointer {                                  // CCountedPointer<CAFile>
    CAFile* Data;
    void*   Info;
};
struct CDataInputStream {
    void* __vftable;
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    void SetPosition(unsigned long position);           // 0x00993BC0
    bool IsEOF() const { return StreamPos == (unsigned long)StreamSize; }
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dst, long size) {
        if (size <= 0) return;
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(dst, CurrentSourcePtr, size); CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
    __forceinline unsigned long ReadULong() { unsigned long v; Read(&v, 4); return v; }
};
struct CFileDataInputStream : CDataInputStream {
    void* File;
    void* Buffer;
    long  BufferSize;
    CFileDataInputStream(CAFile* file, long bufferSize);   // 0x00994700
    ~CFileDataInputStream();                               // 0x00994780
};
struct CBankFileException {                            // throw info 0x01369470
    long Code;
    CBankFileException(long code) : Code(code) {}
    __forceinline CBankFileException(CCharString message, long code) : Code(code) { message.c_str(); }   // out of line at 0x009D1E70
};
struct CLog {
    virtual void v00(); virtual void v01();
    virtual void Print(const CCharString& message);    // slot 2 (+0x08)
};
struct CBankFile {
    unsigned char _pad04[0x7c - 4];
    CFilePointer  File;                                // +0x7c
    CWideString   Filename;                            // +0x84
    bool          Opened;                              // +0x88
    bool          _b89;
    bool          _b8a;
    bool          _b8b;
    bool          _b8c;
    bool          ReadOnly;                            // +0x8d
    unsigned char _pad8e[0x98 - 0x8e];
    unsigned long Alignment;                           // +0x98
    unsigned long BankFormat;                          // +0x9c
    unsigned long FileInformationStart;                // +0xa0
    unsigned long _a4;
    unsigned long FileSize;                            // +0xa8
    unsigned char _padac[0xec - 0xac];
    CLog*         Log;                                 // +0xec
    virtual void s00(); virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04();
    virtual void s05(); virtual void s06(); virtual void s07();
    virtual unsigned long GetDataVersion();            // slot 8 (+0x20)
    void LogMessage(const CCharString& message) { if (Log) Log->Print(message); }
    bool RecoverInterruptedUpdate(unsigned long state);   // 0x009D0380
    bool ReadEntryList(CFileDataInputStream& stream, unsigned long maxIndex, unsigned long entryCount);   // 0x009CFBC0
    bool ReadFileInformation(bool throwOnError);
};

bool CBankFile::ReadFileInformation(bool throwOnError)
{
    CCharString name = Filename.ConvertToCharString();
    CFileDataInputStream stream(File.Data, 0x4000);
    stream.SetPosition(0);
    if (!stream.IsEOF()) {
        char magic[] = "BBBB";
        unsigned long id;
        stream.Read(&id, 4);
        if (memcmp(&id, magic, 4) == 0)
            goto valid;
    }
    if (!throwOnError)
        return false;
    throw CBankFileException(name + " is not a valid resource file!", 0);
valid:
    unsigned long format = stream.ReadULong();
    if (format != BankFormat) {
        if (ReadOnly) {
            if (!throwOnError)
                return false;
            throw CBankFileException(name + " has the wrong bank format! (should be " + ToCharString(BankFormat) +
                                     " not " + ToCharString(format) + ")", 1);
        }
        LogMessage("Bank file version has changed, requiring rebuild");
        return true;
    }
    unsigned long version = stream.ReadULong();
    if (version != GetDataVersion()) {
        if (ReadOnly) {
            if (!throwOnError)
                return false;
#pragma inline_depth(0)
            throw CBankFileException(name + " has the wrong version number! (should be " + ToCharString(GetDataVersion()) +
                                     " not " + ToCharString(version) + ")", 1);
#pragma inline_depth()
        }
        LogMessage("Bank data version has changed, requiring rebuild");
        return true;
    }
    unsigned long updateState = stream.ReadULong();
    unsigned long alignment = stream.ReadULong();
    if (alignment != Alignment) {
        if (!ReadOnly) {
            LogMessage("Data alignment has changed, requiring rebuild");
            return true;
        }
        Alignment = alignment;
    }
    unsigned long maxIndex = stream.ReadULong();
    unsigned long entryCount = stream.ReadULong();
    unsigned long entryListStart = stream.ReadULong();
    FileInformationStart = stream.StreamPos;
    FileSize = File.Data->GetLength();
    if (updateState != 1) {
        if (ReadOnly) {
            if (!throwOnError)
                return false;
            throw CBankFileException(0);
        }
        if (!RecoverInterruptedUpdate(updateState))
            LogMessage("Previous bank update was interrupted, requiring rebuild");
        return true;
    }
    stream.SetPosition(entryListStart);
    return ReadEntryList(stream, maxIndex, entryCount);
}
