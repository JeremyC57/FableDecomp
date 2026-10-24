// CChecksumCacheFile::CChecksumCacheFile(long bufferSize) -- default Filename, an empty STLport map (header node malloc'd: red, no parent, left = right = header, count 0), BufferSize rounded as (bufferSize + 8) & ~8, NeedsSaving = false; no file is opened
// Leaf: ??0CChecksumCacheFile@@QAE@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A60C90, 73 bytes (+7 int3), `ret 4`.
// Callees: CWideString::CWideString @ 0x0099AED0, malloc @ 0x00BFEA0E (0x34-byte header node).
// Sibling of the landed CChecksumCacheFile(const CWideString&, long) @ 0x00A61150 (same member-init list, then Open).
// Label: the manifest's `CBankFile_ParseHeader` is wrong; PDB FableWin names CChecksumCacheFile::CChecksumCacheFile(long).
// Retail layout: Filename +0, Entries {header +4, count +8, compare +0xc} (12-byte STLport _Rb_tree),
//   BufferSize +0x10, NeedsSaving +0x14.
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
class CChecksumCacheFile {
public:
    CWideString     Filename;                   // +0x00
    CRetailEntryMap Entries;                    // +0x04
    long            BufferSize;                 // +0x10
    bool            NeedsSaving;                // +0x14
    CChecksumCacheFile(long bufferSize);
};

CChecksumCacheFile::CChecksumCacheFile(long bufferSize)
    : Filename(), Entries(), BufferSize((bufferSize + 8) & ~8), NeedsSaving(false)
{
}