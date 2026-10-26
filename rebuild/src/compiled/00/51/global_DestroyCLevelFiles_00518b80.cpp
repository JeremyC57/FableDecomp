// _global::_Destroy_CLevelFiles(CLevelFiles* first, CLevelFiles* last) -- run ~CLevelFiles over [first, last)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00518B80, length 33 bytes (padded to 48).
// Leaf: _Destroy_CLevelFiles
// Label correction: the catalog "_Destroy_range<std::allocator<CSpeechFrame>>" is propagated. The element destructor is
// ~CLevelFiles @ 0x0050F980 (landed) and the stride is 0x1c = sizeof(CLevelLoader::CObject::CLevelFiles) (Ego_r.pdb, 28
// bytes), so this is STLport `_Destroy(first, last)` over CLevelFiles emitted out of line (LTCG register convention:
// first in ecx, last in edx, no tag argument -> plain `ret`).
// Callees: CLevelFiles::~CLevelFiles @ 0x0050F980.
// Shape: the for loop becomes the guarded do-while (`cmp ; je` once, `cmp ; jne` at the bottom); `last` stays in edi.
class CAFile;
struct CCountedPointerAFile { CAFile* Object; void* Block; };
struct CLevelFiles {                        // CLevelLoader::CObject::CLevelFiles (28 bytes)
    long LevelID;                           // +0x00
    CCountedPointerAFile PTopologyFile;     // +0x04
    CCountedPointerAFile PNavigationFile;   // +0x0c
    CCountedPointerAFile PThingFile;        // +0x14
    ~CLevelFiles();                         // 0x0050F980
};

void __fastcall _Destroy_CLevelFiles(CLevelFiles* first, CLevelFiles* last)
{
    for (; first != last; ++first)
        first->~CLevelFiles();
}