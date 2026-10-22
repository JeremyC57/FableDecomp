// Grade: DIFFER(621v623) -- register allocation: retail caches the CDiskFileWin32 vptr 0x0122D06C in esi for the inline file ctor/dtor (re-loading it after the loop) and the read size 4 in edi; ours stores the vptr as an immediate and keeps 4 in esi; the loop-body CCharString temporary takes the word below the file (retail: above it)
// CChecksumCacheFile::Open(const CWideString& filename) -- remember the cache file name, drop all cached entries, then (when the
//   file exists) read a version-4 cache: an entry count, then per entry the NUL-terminated name (widened), its CDateAndTime and
//   CRC, stored into Entries[name]
// Leaf: ?Open@CChecksumCacheFile@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A60D40, 623 bytes, ret 4; PDB ?Open@CChecksumCacheFile@@QAEXABVCWideString@@@Z.
// Callees: CWideString operator= 0x0099B7D0 / dtor 0x0099B510 / default ctor 0x0099AED0, Entries _M_erase 0x00A60A00 (inline clear) /
//   operator[] 0x00A617E0, CAFile::PathExists 0x00999230, CBaseClassNonCopyable 0x0099A2F0 / 0x0099A300, CDiskFileWin32 Open 0x0099A6A0
//   / Close 0x0099A920, CFileDataInputStream 0x00994700 / 0x00994780 / ReadNullTerminatedString 0x00994C80 / ReadDateAndTime
//   0x00995770, inline Read (overflow 0x00993CA0), CCharString::ToWideString 0x0099E480 / dtor 0x0099EAE0, CDateAndTime ctor 0x009E9AB0.
// Layout: Ego_r.pdb CChecksumCacheFile (28 bytes): Filename +0, Entries +4 (retail 12-byte map), BufferSize +0x10, NeedsSaving +0x14
//   (retail +0x14: the map is 4 bytes smaller than the PDB's 16); CEntry {CDateAndTime Time (0x1c), long Checksum}.
// Tried: entry before name, entry in its own block, early-return PathExists, named version, entry hoisted out of the loop.
extern "C" int __cdecl memcmp(const void*, const void*, unsigned int);
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcmp, memcpy)
struct CWideString {
    void* PStringData;
    CWideString();                                     // 0x0099AED0
    ~CWideString();                                    // 0x0099B510
    CWideString& operator=(const CWideString& other);  // 0x0099B7D0
};
struct CAFile {
    static bool __fastcall PathExists(const CWideString& path);   // 0x00999230
};
struct CASuspendableProcess;
struct CBaseClassNonCopyable {
    CBaseClassNonCopyable();                           // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                  // 0x0099A300
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
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Open(const CWideString&, FileAccessType, unsigned long);   // slot 1, 0x0099A6A0
    virtual void Close();                                                   // slot 2, 0x0099A920
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
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(dst, CurrentSourcePtr, size); CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
};
struct CFileDataInputStream : CDataInputStream {
    CAFile* File;                                       // +0x18
    void* Buffer;                                       // +0x1c
    long BufferSize;                                    // +0x20
    CFileDataInputStream(CAFile* file, long bufferSize);   // 0x00994700
    ~CFileDataInputStream();                              // 0x00994780
};
struct CCharString {
    void* PStringData;
    ~CCharString();                                    // 0x0099EAE0
    CWideString ToWideString() const;                  // 0x0099E480
};
struct CDateAndTime {
    unsigned long Words[7];                            // 0x1c bytes
    CDateAndTime();                                    // 0x009E9AB0
};
struct CFileDataInputStreamX : CFileDataInputStream {
    CFileDataInputStreamX(CAFile* file, long bufferSize) : CFileDataInputStream(file, bufferSize) {}
    CCharString ReadNullTerminatedString();            // 0x00994C80
    CDateAndTime ReadDateAndTime();                    // 0x00995770
    __forceinline long ReadLong() { long value; Read(&value, 4); return value; }
};
struct CChecksumCacheFile {
    struct CEntry {
        CDateAndTime Time;                             // +0x00
        long         Checksum;                         // +0x1c
    };
    struct CEntryMap {                                 // map<CWideString, CEntry> (retail 12 bytes)
        struct node { long colour; node* parent; node* left; node* right; };
        node*         head;
        unsigned long size;
        unsigned long compare;
        void _M_erase(node* root);                     // 0x00A60A00
        CEntry& operator[](const CWideString& key);    // 0x00A617E0
        void clear()
        {
            if (size != 0) {
                _M_erase(head->parent);
                head->left = head;
                head->parent = 0;
                head->right = head;
                size = 0;
            }
        }
    };
    CWideString Filename;                              // +0x00
    CEntryMap   Entries;                               // +0x04
    long        BufferSize;                            // +0x10
    bool        NeedsSaving;                           // +0x14
    void Open(const CWideString& filename);
};

void CChecksumCacheFile::Open(const CWideString& filename)
{
    Filename = filename;
    Entries.clear();
    NeedsSaving = false;
    if (CAFile::PathExists(filename)) {
        CDiskFileWin32 file;
        file.Open(filename, FILE_ACCESS_READ, 2);
        CFileDataInputStreamX stream((CAFile*)&file, 0x4000);
        if (stream.ReadLong() == 4) {
            long count = stream.ReadLong();
            for (long i = 0; i < count; ++i) {
                CWideString name = stream.ReadNullTerminatedString().ToWideString();
                CEntry entry;
                entry.Time = stream.ReadDateAndTime();
                entry.Checksum = stream.ReadLong();
                Entries[name] = entry;
            }
        }
    }
}
