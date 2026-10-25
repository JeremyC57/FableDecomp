// CMemoryStream::~CMemoryStream() -- EH-framed destructor of the memory-backed read stream: own vtable (0x012B4E08), the buffer at +4 freed through scalar operator delete and the cursor (+8) and buffer pointers cleared when present, then the inline IDataStream destructor restores the base vtable (0x012B4E00)
// Leaf: ??1CMemoryStream@@UAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C31F00, 95 bytes, __thiscall `ret`. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset vtable ; mov eax,[esi+4] ; test eax,eax ; mov dword ptr [esp+10h],0 ; je skip ; push eax ; call operator delete ; add esp,4 ; mov dword ptr [esi+8],0 ; mov dword ptr [esi+4],0 ; skip: mov ecx,[esp+8] ; mov dword ptr [esi],offset base vtable ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Naming (sweep1, 2026-10-25): was landed as CLandscapeBackgroundPatch::~CLandscapeBackgroundPatch (the catalog name;
//   the manifest's "ClearTextures" is propagated); FINDINGS chunk 437 ("0xC31F00 = CMemoryStream dtor, +8 is a cursor").
//   Its vtable 0x012B4E08 is the one the landed CMemoryStream::CMemoryStream 0x00C31F60 stores (slot 0 Read 0x00C31EB0,
//   slot 1 the deleting destructor 0x00C32040, which calls this at 0x00C32043); the layout below is that row's
//   (buffer +4, cursor +8, remaining +0xc); the base IDataStream (vtable 0x012B4E00: _purecall, 0x00C31E90) has the
//   inline virtual destructor. Other callers 0x00C17A65 / 0x00C17AB1. Descriptive names (no PDB covers libpk).
// Callee: scalar operator delete @ 0x00BFE9BC.
struct IDataStream {                        // vtable 0x012B4E00
    virtual bool Read(void* dest, unsigned long size) = 0;   // slot 0
    virtual ~IDataStream() { }              // slot 1
};

struct CMemoryStream : IDataStream {        // vtable 0x012B4E08
    char* m_pBuffer;                        // +0x04
    char* m_pCursor;                        // +0x08
    unsigned long m_remaining;              // +0x0C
    virtual bool Read(void* dest, unsigned long size);   // 0x00C31EB0
    virtual ~CMemoryStream();
};

CMemoryStream::~CMemoryStream()
{
    if (m_pBuffer) {
        ::operator delete(m_pBuffer);
        m_pCursor = 0;
        m_pBuffer = 0;
    }
}
