// Leaf: ?GetAlignment@CAFile@@UBEJXZ
// CAFile::GetAlignment() const -- default buffer alignment for a file: 1 (no alignment requirement)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00401660, 6 bytes. Callees: none.
//
// Part of the CAFile interface. Readers that use unbuffered Win32 I/O override this to return the
// sector size they need their buffers aligned to; the base class has no such requirement, so it answers
// 1. The bank-file streaming code multiplies its read sizes up to this value.
//
// Matching: `this` is unused, body is `mov eax,1; ret`.
#include "engine/CAFile.h"
long CAFile::GetAlignment() const
{
    return 1;
}
// Flatten route (flatten_pilot v4, NF324): CAFile_GetAlignment -> CAFile::GetAlignment, the RTTI-placed PDB member; class model: engine/CAFile.h under its default branch.
