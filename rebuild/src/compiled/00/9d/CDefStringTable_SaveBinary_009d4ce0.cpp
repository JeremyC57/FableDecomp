// CDefStringTable::SaveBinary(bool lock) -- unless the table is locked, write it to its file through a file stream; when asked to lock, also clear the string map and mark the table locked
// Leaf: SaveBinary
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D4CE0, length 200 bytes (+ int3 padding), `ret 4`.
// Callees: CBaseClassNonCopyable ctor @ 0x0099A2F0 / dtor @ 0x0099A300, CWideString::CWideString() @ 0x0099AED0 / ~CWideString
// @ 0x0099B510, CDiskFileWin32::Open(const CWideString&, FileAccessType, ulong) @ 0x0099A6A0 (write access 4, flags 4),
// CDiskFileWin32::Close @ 0x0099A920, CFileDataOutputStream::CFileDataOutputStream(CAFile*) @ 0x00994400 and its dtor
// @ 0x00994430 (catalog "~CFileDataInputStream"), CDefStringTable::WriteTable(CDataOutputStream*) @ 0x009D4A50, the
// map's STLport `_Rb_tree::_M_erase(node)` @ 0x0043020A (catalog "LTextTreeWalkThrough::BuildTreeArray", a shared
// COMDAT body).
// Layout: retail CDefStringTable = Ego_r.pdb with a 12-byte STLport map {header node +0, node count +4, comparator},
// TableFilename +0x30, Locked +0x34 (PDB +0x38/+0x3c). STLport header node: color +0, parent (root) +4, left +8,
// right +0xc. The local CDiskFileWin32 is the engine/CDiskFileWin32.h layout with its virtual Open/Close (as in
// LoadTable 0x009D4C30).
// Shape: the lock branch is the inlined STLport `map::clear()` (`if (count) { _M_erase(root); leftmost = header;
// root = 0; rightmost = header; count = 0; }`, the header re-read after each store) and then the Locked store, all
// before the stream's and the file's inline destructors.
struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                       // 0x0099A2F0
    ~CBaseClassNonCopyable();                      // 0x0099A300
};
class CWideString { public:
    void* PStringData;
    CWideString();                                 // 0x0099AED0
    ~CWideString();                                // 0x0099B510
};
bool __fastcall PathExists(const CWideString& path);   // 0x00999230
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CDiskFileWin32 : public CBaseClassNonCopyable { public:                    // vtable 0x0122D06C
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
struct CFileDataOutputStream {                     // 0xc bytes
    void* __vftable;
    void* m_pFile;
    long m_position;
    CFileDataOutputStream(CDiskFileWin32* file);   // 0x00994400
    ~CFileDataOutputStream();                      // 0x00994430
};

struct SRbTreeNode {
    long Color;                                    // +0x00
    SRbTreeNode* Parent;                           // +0x04
    SRbTreeNode* Left;                             // +0x08
    SRbTreeNode* Right;                            // +0x0c
};
struct CStringMap {                                // STLport map<CCharString, long>
    SRbTreeNode* Header;                           // +0x00 (_STLP_alloc_proxy data)
    unsigned long NodeCount;                       // +0x04
    unsigned long Compare;                         // +0x08 empty less<> padded
    void Erase(SRbTreeNode* node);                 // 0x0043020A _M_erase
    void clear()
    {
        if (NodeCount != 0) {
            Erase(Header->Parent);
            Header->Left = Header;
            Header->Parent = 0;
            Header->Right = Header;
            NodeCount = 0;
        }
    }
};

class CDefStringTable {
public:
    CStringMap StringMap;                          // +0x00
    unsigned char _pad[0x24];                      // StringStream, StringCount, SharedCount, RandomID
    CWideString TableFilename;                     // +0x30
    bool Locked;                                   // +0x34
    bool ReadOnly;                                 // +0x35
    void WriteTable(CFileDataOutputStream* stream);   // 0x009D4A50
    void SaveBinary(bool lock);
};

void CDefStringTable::SaveBinary(bool lock)
{
    if (!Locked) {
        CDiskFileWin32 file;
        file.Open(TableFilename, FILE_ACCESS_WRITE, 4);
        CFileDataOutputStream stream(&file);
        WriteTable(&stream);
        if (lock) {
            StringMap.clear();
            Locked = true;
        }
    }
}