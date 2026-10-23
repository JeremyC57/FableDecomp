// CBankFileManager::OpenRetailBank(const CWideString& pathname, bool startupPriority) -- open a retail bank file: check its magic and version, register a new CRetailBank with the disk file and path, read the contained-bank table (name + five words each) from the header block, then attach a threaded file opened on the path
// Grade: DIFFER(1581v1565) -- 17 blocks: `#pragma inline_depth(0)` around the version-mismatch return now reproduces retail's out-of-line ~CCountedPointer 0x009A9BB0 exit; residue is register choice (esi/edi swap for the bank pointer, map and down-counter) and the inline dtor loading Info via eax then esi at the two tail dtors (+16 bytes)
// Leaf: OpenRetailBank
// Retail 0x009A8840, 1568 bytes, `ret 8` (the landed source is an asm oracle).
// Callees: CAFile::PathExists 0x00999230 (`__fastcall`), operator new 0x00BFEA1A, operator delete 0x00BFE9BC, free
//   0x00BFEA14; CBaseClassNonCopyable 0x0099A2F0, CWideString() 0x0099AED0 (inline CDiskFileWin32 ctor, vtable 0x0122D06C);
//   CCountedPointer<CDiskFileWin32> (CDiskFileWin32*) 0x009A9B60 / ~ 0x009A9BB0; CFileDataInputStream(CAFile*, long)
//   0x00994700 / ~ 0x00994780; CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 / SetPosition 0x00993BC0 /
//   ReadString 0x00995D30; CMemoryDataInputStream(const void*, long) 0x00994930 / ~ 0x00993B10; CRetailBank()
//   0x009AC4D0; CCountedPointer<CRetailBank>(CRetailBank*) 0x009A9D10; list push_back 0x009AAAA0; CWideString::
//   ConvertToCharString 0x0099B2C0; CCharString::operator= 0x0099EFB0 / ~ 0x0099EAE0; CCountedPointer<CDiskFileWin32>::
//   operator= 0x009A9BF0; CArray<char>(n) 0x00411910; CContainedBankMap::operator[] 0x009AC530; CThreadedFile()
//   0x0098DFD0, CCountedPointer<CThreadedFile>::operator=(CThreadedFile*) 0x009A9C80, CThreadedFile::Open 0x0098E1E0.
// Layout: fable_retail_banks.h (CRetailBank: diskFile_ +4, threadedFile_ +0xc, useDma_ +0x14, contained banks +0x18,
//   file path +0x24; CBankFileManager: retail banks list +0x10, retailBankVersion_ +0x34); the magic word 0x0129B07C.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
extern "C" int __cdecl memcmp(const void* a, const void* b, unsigned int bytes);
extern "C" void __cdecl free(void* p);                // 0x00BFEA14
#pragma intrinsic(memcpy, memcmp)
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);
struct CCharString {
    void* PStringData;
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};
struct CWideString {
    void* PStringData;
    CWideString();                                     // 0x0099AED0
    CCharString ConvertToCharString() const;           // 0x0099B2C0
};
struct CAFile {
    static bool __fastcall PathExists(const CWideString& path);   // 0x00999230
};
struct CBaseClassNonCopyable {
    CBaseClassNonCopyable();                           // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};
enum FileAccessType { FILE_ACCESS_READ = 1 };
struct CDiskFileWin32 : CBaseClassNonCopyable {       // vtable 0x0122D06C
    FileAccessType AccessType;                         // +0x04
    long Position;                                     // +0x08
    long Length;                                       // +0x0c
    void* WinFileHandle;                               // +0x10
    bool Opened;                                       // +0x14
    CWideString Pathname;                              // +0x18
    CDiskFileWin32() : CBaseClassNonCopyable(), Pathname() { Opened = false; }
    virtual void Open(const CWideString&, FileAccessType, unsigned long);   // slot 1
};
struct SCountInfo {
    long  Count;
    void (__fastcall* Destroy)(void*);
    void* Object;
};
template <class T> struct CCountedPointer {
    T*          Data;
    SCountInfo* Info;
    CCountedPointer(T* p);                             // 0x009A9B60 / 0x009A9D10
    ~CCountedPointer()
    {
        SCountInfo* info = Info;
        if (info && --info->Count == 0) {
            info->Destroy(info->Object);
            delete info;
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x009A9BF0
    T* operator->() const { return Data; }
};
typedef CCountedPointer<CDiskFileWin32> CDiskFilePointer;
struct CDataInputStream {
    void* __vftable;
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    void SetPosition(unsigned long position);           // 0x00993BC0
    CCharString ReadString();                           // 0x00995D30
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
    CFileDataInputStream(CDiskFileWin32* file, long bufferSize);   // 0x00994700
    ~CFileDataInputStream();                                       // 0x00994780
};
struct CMemoryDataInputStream : CDataInputStream {
    void* Data;
    CMemoryDataInputStream(const void* data, long size);           // 0x00994930
    ~CMemoryDataInputStream();                                     // 0x00993B10
};
struct CByteArray {                                    // CArray<char>
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    CByteArray(unsigned long n);                       // 0x00411910
    ~CByteArray() { if (_M_start) free(_M_start); }
    char* GetData() { return _M_finish - _M_start != 0 ? _M_start : (char*)this; }
};
struct CContainedBank {
    unsigned long DataVersion;
    unsigned long EntryCount;
    unsigned long InfoOffset;
    unsigned long InfoSize;
    unsigned long Alignment;
};
struct CContainedBankMap {
    void* head_;
    unsigned long size_;
    unsigned long allocator_;
    CContainedBank& operator[](const CCharString& key);   // 0x009AC530
};
struct CThreadedFile {
    unsigned char _data[0x1c];
    CThreadedFile();                                   // 0x0098DFD0
    void Open(const CWideString& path, long flags);    // 0x0098E1E0
};
struct CThreadedFilePointer {
    CThreadedFile* Data;
    SCountInfo*    Info;
    CThreadedFilePointer& operator=(CThreadedFile* p);   // 0x009A9C80
};
struct CRetailBank {
    void*                vtable_;
    CCountedPointer<CDiskFileWin32> diskFile_;                    // +0x04
    CThreadedFilePointer threadedFile_;                // +0x0c
    bool                 useDma_;                      // +0x14
    CContainedBankMap    containedBanks_;              // +0x18
    CCharString          filePath_;                    // +0x24
    CRetailBank();                                     // 0x009AC4D0
};
typedef CCountedPointer<CRetailBank> CRetailBankPointer;
struct CRetailBankList {
    void push_back(const CRetailBankPointer& bank);    // 0x009AAAA0
};
extern const unsigned long g_RetailBankMagic;          // 0x0129B07C
struct CBankFileManager {
    unsigned char   _pad00[0x10];
    CRetailBankList RetailBanks;                       // +0x10
    unsigned char   _pad14[0x34 - 0x14];
    unsigned long   RetailBankVersion;                 // +0x34
    void OpenRetailBank(const CWideString& pathname, bool startupPriority);
};

void CBankFileManager::OpenRetailBank(const CWideString& pathname, bool startupPriority)
{
    if (CAFile::PathExists(pathname)) {
        CDiskFilePointer disk(new CDiskFileWin32);
        CDiskFileWin32* file = disk.Data;
        file->Open(pathname, FILE_ACCESS_READ, 2);
        CFileDataInputStream stream(file, 0x4000);
        unsigned long magic;
        stream.Read(&magic, 4);
        if (memcmp(&magic, &g_RetailBankMagic, 4) == 0) {
            if (stream.ReadULong() != RetailBankVersion) {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
            CRetailBankPointer bank(new CRetailBank);
            RetailBanks.push_back(bank);
            bank->filePath_ = pathname.ConvertToCharString();
            bank->diskFile_ = disk;
            unsigned long headerOffset = stream.ReadULong();
            unsigned long headerSize = stream.ReadULong();
            if (headerSize > 0) {
                CByteArray header(headerSize);
                stream.SetPosition(headerOffset);
                stream.Read(header.GetData(), headerSize);
                CMemoryDataInputStream headerStream(header.GetData(), headerSize);
                unsigned long count = headerStream.ReadULong();
                CContainedBankMap& banks = bank->containedBanks_;
                for (unsigned long i = count; i > 0; --i) {
                    CCharString name = headerStream.ReadString();
                    CContainedBank contained;
                    contained.DataVersion = headerStream.ReadULong();
                    contained.EntryCount = headerStream.ReadULong();
                    contained.InfoOffset = headerStream.ReadULong();
                    contained.InfoSize = headerStream.ReadULong();
                    contained.Alignment = headerStream.ReadULong();
                    banks[name] = contained;
                }
            }
            CRetailBank* retailBank = bank.Data;
            CThreadedFilePointer& threaded = retailBank->threadedFile_;
            threaded = new CThreadedFile;
            retailBank->useDma_ = false;
            threaded.Data->Open(pathname, 0);
        }
    }
}
