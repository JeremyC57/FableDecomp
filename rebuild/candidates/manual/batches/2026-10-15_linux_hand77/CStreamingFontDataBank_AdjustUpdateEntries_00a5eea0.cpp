// CStreamingFontDataBank::AdjustUpdateEntries() -- flags every existing bank entry for update and returns how many entries refused the update flag
// Leaf: AdjustUpdateEntries
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5EEA0, 57 bytes; slot 9 of the CStreamingFontDataBank vtable 0x0129D5CC
// (protected virtual, ?AdjustUpdateEntries@CStreamingFontDataBank@@MAEKXZ). Callees (inherited CBankFile members, both
// landed): CBankFile::EntryExists(unsigned long) @ 0x009CC190, CBankFile::EntrySetUpdate(unsigned long, bool) @ 0x009CC300.
// Layout: the loop bound is CBankFile::Size (+0x4), modelled through engine/CBankFile.h (the class's own header,
// engine/CStreamingFontDataBank.h, flattens the CBankFile base into padding).
// Shape: the count lives in ebx, the index in esi, `this` in edi; Size is re-read after each iteration (the calls may
// change the bank), the `&&` gives the two short-circuit tests with the increment on the fall-through.
#include "engine/CBankFile.h"

struct CStreamingFontDataBank_Methods : CBankFile {
    bool EntryExists(unsigned long index);                    // 0x009CC190 (CBankFile)
    bool EntrySetUpdate(unsigned long index, bool update);    // 0x009CC300 (CBankFile)
    unsigned long AdjustUpdateEntries();
};

unsigned long CStreamingFontDataBank_Methods::AdjustUpdateEntries()
{
    unsigned long failed = 0;
    for (unsigned long index = 0; index < Size; ++index) {
        if (EntryExists(index) && !EntrySetUpdate(index, true))
            ++failed;
    }
    return failed;
}
