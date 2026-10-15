// CGPUScratchBufferTextureManager::FreeMemoryPage(long page) -- mark the memory page's resource as none (-1), free every texture slot still listed on the page (each FreeTextureSlot call removes the page's first slot), then push the page index onto FreeTexturePageSlots
// Grade: DIFFER(146v140) -- lane gap (wave 24l): retail overlays push_back's empty SScalarTag temporary on the LIVE page parameter slot (both grow-call references are `lea [esp+18h]/[esp+1Ch]` = &page, no frame); the lane gives the tag its own `push ecx` frame word and re-stores ebx into the page slot (tried: const& push_back with page reloaded, a local index copy (137v140), by-value push_back (146v140, body otherwise identical))
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F37030, length 140 bytes (+ int3 padding). `ret 4`.
// Callees: CGPUScratchBufferTextureManager::FreeTextureSlot(long) @ 0x00F36F40 (on `this`; catalog name "TexturePool_FreeSlot",
// it pushes onto FreeTextureSlots +0x34), the retail vector insert-and-grow @ 0x00440990 (InsertGrow(where, value, tag, 1, true)).
// Layout: Ego_r.pdb CGPUScratchBufferTextureManager with retail 12-byte CArrays: Resources +0x04, MemoryPages +0x10, ScratchPools +0x1c,
// FreeTexturePageSlots +0x28, FreeTextureSlots +0x34, ShutdownFlag +0x40 (the landed constructor 0x00F37520 clears exactly +4..+0x40).
// A memory page record is 0x1c bytes: its resource index at +0, its slot vector at +4.
// Shape: STLport `operator[]` as `*(begin() + n)` (the page offset `imul 1Ch` hoisted, MemoryPages' first re-read after every call);
// `size() > 0` is the unsigned `sar 2 ; test ; jbe/ja`; the retail push_back inlined (placement store when there is room, else the
// grow call with the page parameter's own slot as the value and the empty tag in a dead slot), as the landed PushBackConstant family.
#include <new>

struct SScalarTag { SScalarTag() { } };

struct CLongVector {                       // retail vector<long>
    long* first;
    long* last;
    long* end;
    long* begin() { return first; }
    unsigned long size() { return last - first; }
    long& operator[](unsigned long n) { return *(begin() + n); }
    void InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00440990
    void push_back(long value)                                   // by value: keeps page in ebx like retail
    {
        if (last != end) {
            new (last) long(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};

struct CResourcePage {                     // 0x1c bytes
    long          ResourceIndex;           // +0x00
    CLongVector   Slots;                   // +0x04
    unsigned char _pad10[0x0c];
};

struct CResourcePageVector {
    CResourcePage* first;
    CResourcePage* last;
    CResourcePage* end;
    CResourcePage* begin() { return first; }
    CResourcePage& operator[](long n) { return *(begin() + n); }
};

struct CGPUScratchBufferTextureManager {
    virtual void v00();
    unsigned char       Resources[0x0c];         // +0x04
    CResourcePageVector MemoryPages;             // +0x10
    unsigned char       ScratchPools[0x0c];      // +0x1c
    CLongVector         FreeTexturePageSlots;    // +0x28
    CLongVector         FreeTextureSlots;        // +0x34
    bool                ShutdownFlag;            // +0x40
    void FreeTextureSlot(long slot);             // 0x00F36F40
    void FreeMemoryPage(long page);
};

void CGPUScratchBufferTextureManager::FreeMemoryPage(long page)
{
    MemoryPages[page].ResourceIndex = -1;
    while (MemoryPages[page].Slots.size() > 0)
        FreeTextureSlot(MemoryPages[page].Slots[0]);
    FreeTexturePageSlots.push_back(page);
}
