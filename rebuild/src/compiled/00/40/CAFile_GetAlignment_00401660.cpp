// CAFile::GetAlignment() const -- default buffer alignment for a file: 1 (no alignment requirement)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00401660, 6 bytes. Callees: none.
//
// Part of the CAFile interface. Readers that use unbuffered Win32 I/O override this to return the
// sector size they need their buffers aligned to; the base class has no such requirement, so it answers
// 1. The bank-file streaming code multiplies its read sizes up to this value.
//
// Matching: `this` is unused, body is `mov eax,1; ret`.
struct CAFile;
long __fastcall CAFile_GetAlignment(const CAFile* file)
{
    (void)file;
    return 1;
}
