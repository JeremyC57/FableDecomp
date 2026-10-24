// CChecksumCacheFile::Close() -- if a cache file is open (non-empty Filename): Save() it, clear the entry map (erase every node, reset the header to the empty state) and reset Filename to L""
// Leaf: ?Close@CChecksumCacheFile@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A60C30, 84 bytes (+12 int3).
// Callees: CWideString::GetLength @ 0x0099B220 (IsOpen), CChecksumCacheFile::Save @ 0x00A60AA0,
//   _Rb_tree<CWideString, pair<const CWideString, CEntry>>::_M_erase @ 0x00A60A00, CWideString::operator=(const wchar_t*)
//   @ 0x0099B800 with L"" @ 0x0122D70C.
// Label: the manifest's `CChecksumCacheFile_Save_WithTreeRebuild` is a guess; PDB FableWin names CChecksumCacheFile::Close.
// Shape: IsOpen() is the inline `Filename.GetLength() != 0`; map::clear() is STLport's inline
//   `if (_M_node_count) { _M_erase(_M_root()); _M_leftmost() = header; _M_root() = 0; _M_rightmost() = header; _M_node_count = 0; }`.
#include <stddef.h>
class CWideString {
public:
    void* PStringData;
    long GetLength() const;                                  // 0x0099B220
    const CWideString& operator=(const wchar_t* text);       // 0x0099B800
};
struct SEntryNode {                             // STLport _Rb_tree_node<pair<CWideString, CEntry>>, 0x34 bytes
    bool        Colour;                         // +0x00
    SEntryNode* Parent;                         // +0x04
    SEntryNode* Left;                           // +0x08
    SEntryNode* Right;                          // +0x0c
    unsigned char Value[0x24];                  // +0x10 pair<CWideString, CChecksumCacheFile::CEntry>
};
class CRetailEntryMap {                         // map<CWideString, CEntry> (STLport _Rb_tree, 12 bytes)
public:
    SEntryNode*   Header;                       // +0
    long          NodeCount;                    // +4
    unsigned char Compare[4];                   // +8 (empty less<>)
    void _M_erase(SEntryNode* node);            // 0x00A60A00
    void clear()
    {
        if (NodeCount != 0) {
            _M_erase(Header->Parent);
            Header->Left = Header;
            Header->Parent = 0;
            Header->Right = Header;
            NodeCount = 0;
        }
    }
};
class CChecksumCacheFile {
public:
    CWideString     Filename;                   // +0x00
    CRetailEntryMap Entries;                    // +0x04
    long            BufferSize;                 // +0x10
    bool            NeedsSaving;                // +0x14
    bool IsOpen() const { return Filename.GetLength() != 0; }
    void Save();                                // 0x00A60AA0
    void Close();
};

void CChecksumCacheFile::Close()
{
    if (IsOpen()) {
        Save();
        Entries.clear();
        Filename = L"";
    }
}