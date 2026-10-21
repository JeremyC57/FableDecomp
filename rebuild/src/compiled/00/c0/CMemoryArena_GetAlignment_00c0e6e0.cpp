// CMemoryArena::GetAlignment() const -- the backing allocator's alignment, or 1 when there is no allocator
// Leaf: GetAlignment@CMemoryArena
// Retail: Fable.exe @ 0x00C0E6E0, 21 bytes. Catalog label CAIBrain::GetCurrentStateGroupDebugText (returning CCharString*) is
// wrong: the only caller (0x00C08EC7, inside 0x00C08D40) uses the result as a divisor to round a size up
// (`lea eax,[size+r-1]; div r; imul r`), and the null case returns 1. Owning class not recoverable from the
// manifest in this region; the name here is behavioural. Calls slot 2 (+0x08) of the object at +0x04.
// Shape: plain pointer member (`if (!p)` tests in eax, then reloads for the call); a CCountedPointer-style
// IsValid()/operator-> pair tests in ecx instead.
class IAllocator {
public:
    virtual ~IAllocator();                               // slot 0
    virtual void* Allocate(unsigned long size);          // slot 1
    virtual unsigned long GetAlignment() const;          // slot 2 (+0x08)
};
class CMemoryArena {
public:
    void*            _vptr_pad;                          // +0x00
    IAllocator*      Allocator;                          // +0x04
    unsigned long GetAlignment() const;
};
unsigned long CMemoryArena::GetAlignment() const
{
    if (!Allocator)
        return 1;
    return Allocator->GetAlignment();
}