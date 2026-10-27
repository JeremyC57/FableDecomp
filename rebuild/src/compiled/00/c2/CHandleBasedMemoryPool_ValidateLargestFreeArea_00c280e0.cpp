// CHandleBasedMemoryPool::ValidateLargestFreeArea() -- debug check of the libpk pool: find the highest non-empty free-list bucket and the largest area on it (the comparison with the cached largest size is compiled out of the shipped build; only the walks remain)
// Leaf: ?ValidateLargestFreeArea@CHandleBasedMemoryPool@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C280E0, 57 bytes, thiscall, `ret`.
// Identity: the libpk build of the engine's CHandleBasedMemoryPool (the landed libpk rows FindLargestFreeArea
//   0x00C28050, UpdateLargestFreeArea 0x00C28080, TestIfAreaIsGrowable 0x00C28120, ClearFreeLists 0x00C28200 ...).
//   Ego_r.pdb orders the class's source as FindLargestFreeArea, UpdateLargestFreeArea, ValidateLargestFreeArea,
//   TestIfAreaIsGrowable (donor 0x00B6CE20 / 0x00B6CE50 / 0x00B6CEB0 / 0x00B6CEF0); this row sits between
//   UpdateLargestFreeArea and TestIfAreaIsGrowable, and its only caller 0x00C28323 is the libpk Validate (the engine's
//   Validate 0x00A024C0 inlines the same walk). PDB name ?ValidateLargestFreeArea@CHandleBasedMemoryPool@@AAEXXZ
//   (private, void).
// Layout: the libpk pool keeps its free lists at +0x3C (the engine build at +0x38; the landed 0x00C28080 uses +0x3C),
//   so `this` is modelled locally (engine/CHandleBasedMemoryPool.h carries the engine layout).
// Shape: the bucket scan counts down from 255 with the slot pointer stepped alongside (`sub edx,4`); the hit block
//   re-reads FreeLists[index] and keeps the largest size in a dead register (its ASSERT is compiled out).
struct CHeader {                                // CHandleBasedMemoryPool::CHeader (32 bytes in front of every area)
    void*     Resource;                         // +0x00
    long      Handle;                           // +0x04
    long      Size;                             // +0x08
    short     Padding;                          // +0x0c
    unsigned char Status;                       // +0x0e
    unsigned char Locked;                       // +0x0f
    CHeader** RefAvailable;                     // +0x10
    CHeader*  NextAvailable;                    // +0x14
    CHeader*  PrevContiguous;                   // +0x18
    CHeader*  NextContiguous;                   // +0x1c
};

class CHandleBasedMemoryPool {                  // the libpk build (free lists at +0x3C)
public:
    virtual void v00();
    unsigned char _pad04[0x3c - 0x04];
    CHeader*      FreeLists[256];               // +0x3c
private:
    void ValidateLargestFreeArea();
};

void CHandleBasedMemoryPool::ValidateLargestFreeArea()
{
    for (long index = 255; index >= 0; --index) {
        if (FreeLists[index] != 0) {
            long largest = 0;
            for (CHeader* area = FreeLists[index]; area; area = area->NextAvailable)
                if (largest < area->Size)
                    largest = area->Size;
            break;
        }
    }
}