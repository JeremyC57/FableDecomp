// CCachedFile::CCachedFile(long bufferSize) -- CBase(), vtable, buffer start 0, a zero-filled byte buffer, empty path, no disk file, flags clear
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5B590, oracle length 76 bytes.
// Callees: CBase::CBase @ 0x0099A2F0 (landed), CArray<unsigned char>::CArray(count, const uchar& fill, const allocator&)
// @ 0x009D2A40 (manifest "Vector_InitFill": allocates count bytes and rep-stos fills them; its third
// argument is never read), CWideString::CWideString @ 0x0099AED0. Vtable 0x012724D4 = CCachedFile
// (rtti-vtables.tsv; neighbours 0x00A5B210.. are CCachedFile::OpenFromFile/Open/Read). The manifest label
// "C3DMeshObject::C3DMeshObject" is propagated and rejected.
//
// Retail bytes:  push ecx ; push ebx ; push esi ; mov esi,ecx ; call CBase::CBase ; mov edx,[esp+10h] ; lea eax,[esp+10h] ; push eax ;
//                lea ecx,[esp+0Fh] ; push ecx ; xor ebx,ebx ; push edx ; lea ecx,[esi+0Ch] ; mov [esi],offset vftable ; mov [esi+8],ebx ;
//                mov [esp+17h],bl ; call CArray<uchar>::CArray ; lea ecx,[esi+18h] ; call CWideString::CWideString ; mov [esi+1Ch],ebx ;
//                mov [esi+20h],ebx ; mov [esi+24h],bl ; mov [esi+25h],bl ; mov eax,esi ; pop esi ; pop ebx ; pop ecx ; ret 4
// Retail layout (PDB struct_layouts_egor.tsv has the same members with a 16-byte debug CArray, so
// everything after DataBuffer sits 4 bytes lower): +4 Position (not initialised), +8 BufferStartPosition,
// +0xC DataBuffer (12-byte CArray<uchar>), +0x18 Pathname (CWideString), +0x1C PDiskFile
// (CCountedPointer<CAFile>, inline zeroing constructor), +0x24 Opened, +0x25 BufferValid; sizeof 0x28.
// The fill byte is a one-byte local (esp+0x17) and the allocator temporary is built in the dead
// argument's home slot (esp+0x10), which is why `&bufferSize` is pushed for the third parameter.
struct CBase {
    virtual void v00();
    CBase();                               // 0x0099A2F0
};

struct SAllocator { SAllocator() { } };    // empty allocator passed by const reference (user constructor: no zero-init of the temporary)

struct CByteArray {                        // retail CArray<unsigned char>: {first, last, end}
    unsigned char* m_pFirst;
    unsigned char* m_pLast;
    unsigned char* m_pEnd;
    CByteArray(unsigned long count, const unsigned char& fill, const SAllocator& al = SAllocator());   // 0x009D2A40
};

typedef unsigned short wchar_t;          // VC7.1 without /Zc:wchar_t: wchar_t is the CRT typedef

struct CWideString {
    wchar_t* m_pString;
    CWideString();                         // 0x0099AED0
};

struct CCountedPointer {
    void* m_pObject;
    void* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
};

struct CCachedFile : CBase {
    virtual void v00();                    // own vtable (0x012724D4)
    long            Position;              // +0x04 (not initialised)
    long            BufferStartPosition;   // +0x08
    CByteArray      DataBuffer;            // +0x0C
    CWideString     Pathname;              // +0x18
    CCountedPointer PDiskFile;             // +0x1C  CCountedPointer<CAFile>
    bool            Opened;                // +0x24
    bool            BufferValid;           // +0x25
    CCachedFile(long bufferSize);
};

CCachedFile::CCachedFile(long bufferSize)
    : CBase(), BufferStartPosition(0), DataBuffer(bufferSize, 0), Pathname(), PDiskFile(), Opened(false), BufferValid(false)
{
}
