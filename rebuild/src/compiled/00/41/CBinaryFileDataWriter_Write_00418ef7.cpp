// CBinaryFileDataWriter::Write(const void* data, unsigned long size) -- forward to the file's virtual Write(data, size, 0) and report success
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00418EF7, length 23 bytes.
// Callees: CAFile virtual slot 4 (+0x10) on the File member (+0x04, PDB CBinaryFileDataWriter).
// Shape: the two arguments are passed straight through (`push dword ptr [esp+0Ch]` twice) with a
// zero third argument, then `mov al,1`: a size-optimised TU (0x0041xxxx) returning a constant
// true. The manifest calls it vfunc_0; it is the writer's Write override.
#pragma optimize("s", on)
class CAFile { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, unsigned long size, long flags);   // slot 4 (+0x10)
};

struct CBinaryFileDataWriter {
    virtual bool Write(const void* data, unsigned long size);   // slot 0
    CAFile* File;                               // +0x04
};

bool CBinaryFileDataWriter::Write(const void* data, unsigned long size)
{
    File->Write(data, size, 0);
    return true;
}