// CDataOutputStream::WriteULONG(unsigned long value) -- the unsigned 32-bit writer (CPersistContext CRC/size words); hands the value's 4 bytes to the stream's virtual Write
// Leaf: ?WriteULONG@CDataOutputStream@@QAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00993EA0, 15 bytes: `mov eax,[ecx]; push 4; lea edx,[esp+8]; push edx; call [eax+0x10]; ret 4`.
// Identity (sweep2, FINDINGS chunk 474): the name sets hold `?WriteULONG@CDataOutputStream@@QAEXK@Z`; the retail block
//   0x993E30..0x993EF0 is the CDataOutputStream fixed-size writers in declaration order (WriteEBOOL 0x993E30, three 1-byte writers 0x993E50/60/70 (CBYTE/UBYTE/SBYTE), WriteUWORD 0x993E80, WriteSWORD 0x993E90, WriteULONG 0x993EA0, WriteSLONG 0x993EB0, WriteUHUGE 0x993EC0, WriteSHUGE 0x993ED0, WriteFloat 0x993EE0, WriteDouble 0x993EF0);
//   the argument width (4 bytes) plus the callers fix this one. The earlier landed source was a generic stand-in.
// Vtable (CFileDataOutputStream 0x0129A6F8 / CMemoryDataOutputStream 0x0129A788, name sets): slot 0 dtor, 1 SetPosition,
//   2 GetPosition, 3 Close, 4 Write(const void*, long) at +0x10.
class CDataOutputStream {
public:
    virtual ~CDataOutputStream();
    virtual void SetPosition(unsigned long position) = 0;
    virtual unsigned long GetPosition() const = 0;
    virtual void Close() = 0;
    virtual void Write(const void* data, long size) = 0;   // slot 4 (+0x10)

    void WriteULONG(unsigned long value);
};

void CDataOutputStream::WriteULONG(unsigned long value)
{
    Write(&value, sizeof(value));
}
