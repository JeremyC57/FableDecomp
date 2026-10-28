// CFontManager::GetFreeVertexBuffer() -- advance the round-robin vertex-buffer cursor modulo the buffer count and return that font vertex buffer
// Leaf: ?GetFreeVertexBuffer@CFontManager@@QAEAAVCVertexBufferWin32@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E2CE0, 31 bytes, thiscall, returns a reference.
// Identity: Ego_r.pdb ?GetFreeVertexBuffer@CFontManager@@QAEAAVCVertexBufferWin32@@XZ (0xB2C250, in the CFontManager
//   block beside ReleaseFont / GetStaticFont / GetStreamingFont / GetFont, all landed or graded here at 0x009E2B40..0x009E2C80);
//   the layout (VertexBuffers +0x24, CurrentVB +0x30) is the PDB CFontManager at its retail offsets and the only member
//   that fits the body. Callers 0x00AB7C8D, 0x00AB82D5 (font renderer). The catalog label already says GetFreeVertexBuffer.
// Shape: `CurrentVB = (CurrentVB + 1) % VertexBuffers.Size(); return *VertexBuffers[CurrentVB];` -- the unsigned
//   CArray size makes the modulo a `div`; the element load re-reads the array base.
class CVertexBufferWin32;
template <class T> struct CRetailArray {           // retail CArray: {first, last, end}
    T* first;
    T* last;
    T* end_of_storage;
    unsigned long Size() const { return last - first; }
    T& operator[](unsigned long i) { return first[i]; }
};
struct CCountedPointerBase { void* Data; void* Info; };
class CFontManager { public:
    void*                Vtbl;                     // +0x00
    CCountedPointerBase  PFontDataBank;            // +0x04
    CCountedPointerBase  PStreamingFontDataBank;   // +0x0c
    CCountedPointerBase  PSymbols;                 // +0x14
    CCountedPointerBase  PStreamingSymbols;        // +0x1c
    CRetailArray<CVertexBufferWin32*> VertexBuffers;   // +0x24
    long                 CurrentVB;                // +0x30
    CVertexBufferWin32& GetFreeVertexBuffer();
};

CVertexBufferWin32& CFontManager::GetFreeVertexBuffer()
{
    CurrentVB = (CurrentVB + 1) % VertexBuffers.Size();
    return *VertexBuffers[CurrentVB];
}