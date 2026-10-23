// CBankFileManager::OpenIniFile(const CWideString& pathname) -- clear the bank-info map, read the bank ini file line by line: `name = "dev path" "header"` lines register a CBankInfo (dev path, current retail file, dev/retail header paths), BeginRetailFile / EndRetailFile / DevHeaderDirectory / RetailHeaderDirectory keyword lines set the running values; finally store the ini file's directory as the base path
// Grade: DIFFER(1708v1732) -- 85 blocks: frame-slot layout of the loop temporaries (ToWideString/operator+ results, the CBankInfo temporary at 0x68 in retail), and retail reloads each keyword CCharString temporary from its stack slot instead of using the ctor's eax (4x2 bytes); keyword compare shapes (preload "" for the literal, jne/jmp for the keyword) and all control flow line up
// Leaf: OpenIniFile
// Retail 0x009A8170, 1744 bytes, `ret 4` (the landed source is an asm oracle).
// Callees: map<CCharString, CBankInfo> _M_erase 0x009AA7C0 / operator[] 0x009AC5D0; CBaseClassNonCopyable() 0x0099A2F0 /
//   ~ 0x0099A300; CMemoryFile (vtable 0x0122E0FC) Open 0x009E94A0 / Close 0x009E9390; CFileDataInputStream(CAFile*, long)
//   0x00994700 / ~ 0x00994780; CDataInputStream::ReadLine 0x00994B50 / ReadUntilCharacter 0x00994DC0 / ReadUntilWhiteSpace
//   0x00994E90 / SetPosition 0x00993BC0 / ReadWithSrcChunkOverflow 0x00993CA0; CMemoryDataInputStream(const CCharString&)
//   0x00995F50 / ~ 0x00993B10; CBankFileManager::ReadQuotedString 0x009A77D0; CCharString() 0x0099E4B0 / (const char*,
//   long) 0x0099EBF0 / ~ 0x0099EAE0 / Trim 0x0099F560 / ToWideString 0x0099E480; CWideString() 0x0099AED0 / copy
//   0x0099B720 / ~ 0x0099B510 / operator= 0x0099B7D0 / operator=(const unsigned short*) 0x0099B800 / operator+ 0x0099BE70
//   (`__fastcall`); CAFile::SplitPath 0x00997620 (`__fastcall`); _stricmp 0x00BFEBA8; free 0x00BFEA14.
// Layout: Ego_r.pdb CBankInfo (four CWideStrings + RegisteredBank, 0x14); fable_retail_banks.h CBankFileManager (bank-info
//   map +0x04, base path +0x24); CMemoryFile 0x24 bytes from the inline ctor/dtor here (buffer +0x10, flag +0x1c, path +0x20).
extern "C" void __cdecl free(void* p);                // 0x00BFEA14
extern "C" int __cdecl _stricmp(const char* a, const char* b);   // 0x00BFEBA8
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
struct CWideString {
    void* PStringData;
    CWideString();                                     // 0x0099AED0
    CWideString(const CWideString& other);             // 0x0099B720
    ~CWideString();                                    // 0x0099B510
    CWideString& operator=(const CWideString& other);  // 0x0099B7D0
    CWideString& operator=(const unsigned short* text);       // 0x0099B800
};
CWideString __fastcall operator+(const CWideString& a, const CWideString& b);   // 0x0099BE70
struct SCharStringData { const char* Text; long Length; };
struct CCharString {
    SCharStringData* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    void Trim();                                       // 0x0099F560
    CWideString ToWideString() const;                  // 0x0099E480
    const char* c_str() const { const char* text = ""; if (PStringData) text = PStringData->Text; return text; }
    const char* GetString() const { if (PStringData == 0) return ""; return PStringData->Text; }
    long GetLength() const { return PStringData ? PStringData->Length : 0; }
};
inline bool operator==(const CCharString& a, const CCharString& b) { return _stricmp(a.GetString(), b.c_str()) == 0; }
struct CBaseClassNonCopyable {
    CBaseClassNonCopyable();                           // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                  // 0x0099A300
};
enum FileAccessType { FILE_ACCESS_READ = 1 };
struct CByteVector {
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;
    CByteVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CByteVector() { if (_M_start) free(_M_start); }
};
struct CMemoryFile : CBaseClassNonCopyable {           // vtable 0x0122E0FC
    long        AccessType;                            // +0x04
    long        Position;                              // +0x08
    long        Length;                                // +0x0c
    CByteVector Buffer;                                // +0x10
    bool        Opened;                                // +0x1c
    CWideString Pathname;                              // +0x20
    CMemoryFile() { Opened = false; }
    ~CMemoryFile() { CMemoryFile::Close(); }
    virtual void Open(const CWideString& path, FileAccessType access, unsigned long flags);   // 0x009E94A0
    virtual void Close();                              // 0x009E9390
};
struct CAFile {
    static void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* dir, CWideString* name,
                                     CWideString* ext);   // 0x00997620
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
    CCharString ReadLine();                             // 0x00994B50
    CCharString ReadUntilCharacter(char c, bool* found);   // 0x00994DC0
    CCharString ReadUntilWhiteSpace();                  // 0x00994E90
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
};
struct CFileDataInputStream : CDataInputStream {
    void* File;
    void* Buffer;
    long  BufferSize;
    CFileDataInputStream(CMemoryFile* file, long bufferSize);   // 0x00994700
    ~CFileDataInputStream();                                    // 0x00994780
};
struct CMemoryDataInputStream : CDataInputStream {
    void* Data;
    CMemoryDataInputStream(const CCharString& text);   // 0x00995F50
    ~CMemoryDataInputStream();                         // 0x00993B10
};
class CBankFile;
struct CBankInfo {
    CWideString DevBankFilePath;
    CWideString RetailBankFilePath;
    CWideString DevHeaderFilePath;
    CWideString RetailHeaderFilePath;
    CBankFile*  RegisteredBank;
    CBankInfo(const CWideString& dev, const CWideString& retail, const CWideString& devHeader,
              const CWideString& retailHeader)
        : DevBankFilePath(dev), RetailBankFilePath(retail), DevHeaderFilePath(devHeader),
          RetailHeaderFilePath(retailHeader), RegisteredBank(0) {}
};
struct SBankInfoNode {
    unsigned char  color;
    SBankInfoNode* parent;
    SBankInfoNode* left;
    SBankInfoNode* right;
};
struct CBankInfoMap {                                  // STLport map<CCharString, CBankInfo>
    SBankInfoNode* _M_header;
    unsigned long  _M_node_count;
    bool           _M_key_compare;
    void _M_erase(SBankInfoNode* x);                   // 0x009AA7C0
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->parent);
            _M_header->left = _M_header;
            _M_header->parent = 0;
            _M_header->right = _M_header;
            _M_node_count = 0;
        }
    }
    CBankInfo& operator[](const CCharString& key);     // 0x009AC5D0
};
struct CBankFileManager {
    void*        vtable_;
    CBankInfoMap BankInfos;                            // +0x04
    unsigned char _pad10[0x24 - 0x10];
    CWideString  BasePath;                             // +0x24
    bool ReadQuotedString(CCharString& out, CDataInputStream& stream);   // 0x009A77D0
    void OpenIniFile(const CWideString& pathname);
};

void CBankFileManager::OpenIniFile(const CWideString& pathname)
{
    BankInfos.clear();
    CMemoryFile file;
    file.Open(pathname, FILE_ACCESS_READ, 2);
    CWideString retailFile;
    CWideString devHeaderDirectory;
    CWideString retailHeaderDirectory;
    CFileDataInputStream stream(&file, 0x4000);
    while (!stream.IsEOF()) {
        CCharString line = stream.ReadLine();
        line.Trim();
        CMemoryDataInputStream lineStream(line);
        bool found;
        CCharString key = lineStream.ReadUntilCharacter('=', &found);
        if (found) {
            char equals;
            lineStream.Read(&equals, 1);
            CCharString devPath;
            if (ReadQuotedString(devPath, lineStream)) {
                CCharString header;
                CWideString devHeader;
                CWideString retailHeader;
                if (ReadQuotedString(header, lineStream) && header.GetLength() > 0) {
                    devHeader = devHeaderDirectory + header.ToWideString();
                    retailHeader = retailHeaderDirectory + header.ToWideString();
                }
                BankInfos[key] = CBankInfo(devPath.ToWideString(), retailFile, devHeader, retailHeader);
            }
        } else {
            lineStream.SetPosition(0);
            CCharString keyword = lineStream.ReadUntilWhiteSpace();
            if (keyword == CCharString("BeginRetailFile")) {
                CCharString value;
                ReadQuotedString(value, lineStream);
                retailFile = value.ToWideString();
            } else if (keyword == CCharString("EndRetailFile")) {
                retailFile = L"";
            } else if (keyword == CCharString("DevHeaderDirectory")) {
                CCharString value;
                ReadQuotedString(value, lineStream);
                devHeaderDirectory = value.ToWideString();
            } else if (keyword == CCharString("RetailHeaderDirectory")) {
                CCharString value;
                ReadQuotedString(value, lineStream);
                retailHeaderDirectory = value.ToWideString();
            }
        }
    }
    CWideString iniPath(file.Pathname);
    CWideString directory;
    CAFile::SplitPath(iniPath, 0, &directory, 0, 0);
    BasePath = directory;
}
