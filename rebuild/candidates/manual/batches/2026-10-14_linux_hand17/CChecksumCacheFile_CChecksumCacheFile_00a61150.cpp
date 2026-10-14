// CChecksumCacheFile::CChecksumCacheFile(const CWideString& filename, long bufferSize) -- default Filename,
// an empty STLport map (header node malloc'd: red, no parent, left = right = header, count 0), BufferSize
// rounded as (bufferSize + 8) & ~8, NeedsSaving = false, then Open(filename).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A61150, length 85 bytes.
// Callees: CWideString::CWideString @ 0x0099AED0, malloc @ 0x00BFEA0E (0x34-byte node), CChecksumCacheFile::Open
// @ 0x00A60D40. Retail layout: Filename +0, Entries {header +4, count +8, compare +0xc} (12-byte STLport
// _Rb_tree; the PDB's map is 16 bytes), BufferSize +0x10, NeedsSaving +0x14 -- 4 bytes below the PDB offsets.
#include <stdlib.h>
class CWideString {
public:
    void* PStringData;
    CWideString();                              // 0x0099AED0
};
struct SEntryNode {                             // STLport _Rb_tree_node<pair<CWideString, CEntry>>, 0x34 bytes
    bool        Colour;                         // +0x00 (red = false)
    SEntryNode* Parent;                         // +0x04
    SEntryNode* Left;                           // +0x08
    SEntryNode* Right;                          // +0x0c
    unsigned char Value[0x24];                  // +0x10 pair<CWideString, CChecksumCacheFile::CEntry>
};
struct CRetailEntryMap {                        // map<CWideString, CEntry> (STLport _Rb_tree, 12 bytes)
    SEntryNode*   Header;                       // +0
    long          NodeCount;                    // +4
    unsigned char Compare[4];                   // +8 (empty less<>)
    CRetailEntryMap() : Header(0)
    {
        Header = static_cast<SEntryNode*>(malloc(sizeof(SEntryNode)));
        NodeCount = 0;
        Header->Colour = false;
        Header->Parent = 0;
        Header->Left = Header;
        Header->Right = Header;
    }
};
struct CChecksumCacheFile {
    CWideString     Filename;                   // +0x00
    CRetailEntryMap Entries;                    // +0x04
    long            BufferSize;                 // +0x10
    bool            NeedsSaving;                // +0x14
    CChecksumCacheFile(const CWideString& filename, long bufferSize);
    void Open(const CWideString& filename);     // 0x00A60D40
};

CChecksumCacheFile::CChecksumCacheFile(const CWideString& filename, long bufferSize)
    : Filename(), Entries(), BufferSize((bufferSize + 8) & ~8), NeedsSaving(false)
{
    Open(filename);
}
