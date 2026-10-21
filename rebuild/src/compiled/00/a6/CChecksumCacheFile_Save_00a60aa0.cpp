// CChecksumCacheFile::Save() -- when the cache needs saving, rewrite its file: a placeholder header, then for every entry whose path still exists its name (as a char string), date and checksum, then the header patched with version 4 and the number of entries written; the file is closed and the cache marked saved
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A60AA0, 388 bytes (400 with padding).
// Callees: CBaseClassNonCopyable ctor 0x0099A2F0 / dtor 0x0099A300, CWideString() 0x0099AED0, CWideString(const
//          CWideString&) 0x0099B720, ~CWideString 0x0099B510, CWideString::ConvertToCharString 0x0099B2C0 (by value),
//          ~CCharString 0x0099EAE0; CDiskFileWin32::Open 0x0099A6A0 / Close 0x0099A920 (called directly on the
//          local); CFileDataOutputStream(CAFile*) 0x00994400, its dtor 0x00994430 (catalog "~CFileDataInputStream"),
//          GetPosition 0x00994460, SetPosition 0x00994450, WriteULONG 0x00993EB0, WriteSLONG 0x00993EA0 (both
//          catalogued WriteCBYTE: four-byte WriteData bodies), WriteNullTerminatedString 0x00995AE0,
//          WriteDateAndTime 0x00994180; `__fastcall` PathExists(const CWideString&) 0x00999230.
//
// Shape: the local CDiskFileWin32 / CFileDataOutputStream pair of the landed CDefStringTable::SaveBinary
// (0x009D4CE0); the STLport map walk inlines `++it` (right child then leftmost, else climb) and re-reads the
// header for every end test. Each saved name is a named CWideString copy whose char-string conversion is an
// unnamed temporary passed straight to the stream; the explicit Close precedes the inline destructors.
// Layout: retail CChecksumCacheFile of the landed ctor 0x00A61150: Filename +0, Entries {header +4, count +8},
// BufferSize +0x10, NeedsSaving +0x14; node value {CWideString name +0x10, CDateAndTime +0x14, checksum +0x30}.
struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                       // 0x0099A2F0
    ~CBaseClassNonCopyable();                      // 0x0099A300
};
struct CCharString {
    void* PStringData;
    ~CCharString();                                // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    CWideString();                                 // 0x0099AED0
    CWideString(const CWideString& other);         // 0x0099B720
    ~CWideString();                                // 0x0099B510
    CCharString ConvertToCharString();             // 0x0099B2C0
};
bool __fastcall PathExists(const CWideString& path);   // 0x00999230
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
};
struct CDateAndTime {
    unsigned char _body[0x1c];
};
struct CFileDataOutputStream {                     // 0xc bytes
    void* __vftable;
    void* m_pFile;
    long m_position;
    CFileDataOutputStream(CDiskFileWin32* file);   // 0x00994400
    ~CFileDataOutputStream();                      // 0x00994430
    unsigned long GetPosition() const;             // 0x00994460
    void SetPosition(unsigned long position);      // 0x00994450
    void WriteULONG(unsigned long value);          // 0x00993EB0
    void WriteSLONG(long value);                   // 0x00993EA0
    void WriteNullTerminatedString(const CCharString& text);   // 0x00995AE0
    void WriteDateAndTime(const CDateAndTime& date);           // 0x00994180
};
struct CEntry {                                    // CChecksumCacheFile::CEntry
    CDateAndTime Date;                             // +0x00 (node +0x14)
    long         Checksum;                         // +0x1c (node +0x30)
};
struct SEntryNode {                                // STLport _Rb_tree_node<pair<const CWideString, CEntry>>, 0x34 bytes
    unsigned char _M_color;                        // +0x00
    SEntryNode*   _M_parent;                       // +0x04
    SEntryNode*   _M_left;                         // +0x08
    SEntryNode*   _M_right;                        // +0x0c
    CWideString   first;                           // +0x10
    CEntry        second;                          // +0x14
};
struct CEntryIterator {
    SEntryNode* _M_node;
    CEntryIterator(SEntryNode* node) : _M_node(node) {}
    SEntryNode* operator->() const { return _M_node; }
    bool operator!=(const CEntryIterator& other) const { return _M_node != other._M_node; }
    static SEntryNode* Increment(SEntryNode* node)   // STLport _Rb_global::_M_increment
    {
        if (node->_M_right != 0) {
            node = node->_M_right;
            while (node->_M_left != 0)
                node = node->_M_left;
        } else {
            SEntryNode* y = node->_M_parent;
            while (node == y->_M_right) {
                node = y;
                y = y->_M_parent;
            }
            if (node->_M_right != y)
                node = y;
        }
        return node;
    }
    void operator++() { _M_node = Increment(_M_node); }
};
struct CEntryMap {                                 // map<CWideString, CEntry> (STLport, 12 bytes)
    SEntryNode*   _M_header;
    unsigned long _M_node_count;
    unsigned char _M_compare;
    CEntryIterator begin() { return CEntryIterator(_M_header->_M_left); }
    CEntryIterator end() { return CEntryIterator(_M_header); }
};
struct CChecksumCacheFile {
    CWideString Filename;                          // +0x00
    CEntryMap   Entries;                           // +0x04
    long        BufferSize;                        // +0x10
    bool        NeedsSaving;                       // +0x14
    void Save();
};

void CChecksumCacheFile::Save()
{
    if (NeedsSaving) {
        CDiskFileWin32 file;
        file.Open(Filename, FILE_ACCESS_WRITE, 2);
        CFileDataOutputStream stream(&file);
        unsigned long start = stream.GetPosition();
        stream.WriteULONG(0);
        stream.WriteULONG(0);
        unsigned long count = 0;
        for (CEntryIterator it = Entries.begin(); it != Entries.end(); ++it) {
            if (PathExists(it->first)) {
                CWideString name(it->first);
                stream.WriteNullTerminatedString(name.ConvertToCharString());
                stream.WriteDateAndTime(it->second.Date);
                stream.WriteSLONG(it->second.Checksum);
                ++count;
            }
        }
        stream.SetPosition(start);
        stream.WriteULONG(4);
        stream.WriteULONG(count);
        file.Close();
        NeedsSaving = false;
    }
}