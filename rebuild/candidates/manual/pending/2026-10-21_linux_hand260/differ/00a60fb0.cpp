// Grade: DIFFER(412v411) -- register permutation only
// CChecksumCacheFile::GetChecksum(const CWideString& path) -- CRC of a file, served from the cache while the file's modified time is unchanged; otherwise the file is read in BufferSize blocks, the CRC stored with the new time and the cache marked for saving
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A60FB0, 412 bytes (416 with padding).
// Callees: `__fastcall` PathExists(const CWideString&) 0x00999230, map find 0x00A608F0 (catalog
//          BinaryTree_FindByKey_LeftBias, returns the node), map operator[] 0x00A617E0 (catalog TreeNode_GetByKey),
//          CDateAndTime() 0x009E9AB0, CDateAndTime::operator== 0x009E9AD0, `__fastcall` GetFileModifiedTime
//          0x009992A0 (hidden result in ecx), operator new[] 0x00BFEB22 / delete[] 0x00BFEB1C, CBaseClassNonCopyable
//          ctor 0x0099A2F0 / dtor 0x0099A300, CWideString() 0x0099AED0 / ~ 0x0099B510, CDiskFileWin32::Open
//          0x0099A6A0 / Read 0x0099A960 / Close 0x0099A920 (called directly on the local), CRC Calc 0x004014A0.
// Layout: retail CChecksumCacheFile of the landed ctor 0x00A61150 / Save 0x00A60AA0: Filename +0, Entries
//         {header +4, count +8}, BufferSize +0x10, NeedsSaving +0x14; node value {name +0x10, CDateAndTime +0x14,
//         checksum +0x30}. CDiskFileWin32 0x1C bytes as in the landed Save (Length +0x0C read inline).
struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                       // 0x0099A2F0
    ~CBaseClassNonCopyable();                      // 0x0099A300
};
struct CWideString {
    void* PStringData;
    CWideString();                                 // 0x0099AED0
    ~CWideString();                                // 0x0099B510
};
bool __fastcall PathExists(const CWideString& path);   // 0x00999230
unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC
class CASuspendableProcess;
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
struct CDiskFileWin32 : CBaseClassNonCopyable {    // vtable 0x0122D06C
    long        AccessType;                        // +0x04
    long        Position;                          // +0x08
    long        Length;                            // +0x0c
    void*       WinFileHandle;                     // +0x10
    bool        Opened;                            // +0x14
    CWideString Pathname;                          // +0x18
    CDiskFileWin32() { Opened = false; }
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099A6A0
    virtual void Close();                          // 0x0099A920
    virtual void Read(void* data, long size, CASuspendableProcess* process);   // 0x0099A960
};
struct CDateAndTime {
    long Fields[7];
    CDateAndTime();                                // 0x009E9AB0
    bool operator==(const CDateAndTime& other) const;   // 0x009E9AD0
};
CDateAndTime __fastcall GetFileModifiedTime(const CWideString& path);   // 0x009992A0
struct CEntry {                                    // CChecksumCacheFile::CEntry
    CDateAndTime  Date;                            // +0x00 (node +0x14)
    unsigned long Checksum;                        // +0x1c (node +0x30)
};
struct SEntryNode {                                // STLport _Rb_tree_node<pair<const CWideString, CEntry>>
    unsigned char _M_color;
    SEntryNode*   _M_parent;
    SEntryNode*   _M_left;
    SEntryNode*   _M_right;
    CWideString   first;                           // +0x10
    CEntry        second;                          // +0x14
};
struct CEntryMap {                                 // map<CWideString, CEntry> (STLport, 12 bytes)
    SEntryNode*   _M_header;
    unsigned long _M_node_count;
    unsigned char _M_compare;
    SEntryNode* find(const CWideString& key);      // 0x00A608F0
    CEntry& operator[](const CWideString& key);    // 0x00A617E0
};
struct CChecksumCacheFile {
    CWideString Filename;                          // +0x00
    CEntryMap   Entries;                           // +0x04
    long        BufferSize;                        // +0x10
    bool        NeedsSaving;                       // +0x14
    unsigned long GetChecksum(const CWideString& path);
};

unsigned long CChecksumCacheFile::GetChecksum(const CWideString& path)
{
    if (!PathExists(path))
        return 0;
    SEntryNode* found = Entries.find(path);
    CDateAndTime modified;
    if (found != Entries._M_header) {
        modified = GetFileModifiedTime(path);
        if (modified == found->second.Date)
            return found->second.Checksum;
    } else {
        modified = GetFileModifiedTime(path);
    }
    char* buffer = new char[BufferSize];
    unsigned long checksum = 0xFFFFFFFF;
    CDiskFileWin32 file;
    file.Open(path, FILE_ACCESS_READ, 2);
    long length = file.Length;
    for (long position = 0; position < length; position += BufferSize) {
        long size = length - position;
        if (BufferSize < size)
            size = BufferSize;
        file.Read(buffer, size, 0);
        checksum = Calc_004014a0(checksum, buffer, size);
    }
    checksum = ~checksum;
    delete[] buffer;
    CEntry entry;
    entry.Checksum = checksum;
    entry.Date = modified;
    Entries[path] = entry;
    NeedsSaving = true;
    return checksum;
}
