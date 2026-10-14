// ReadChunkExtent_00c347b0(void*, void*, long*, long*) -- EH-framed `__fastcall`: a 0x48-byte CChunkedFileChunk (vtable 0x012B4FAC over the CWADFile base 0x012B4F9C, words +4/+0xc zeroed inline) is opened on the stack with the two register arguments (@ 0x00C42850, state 0); -1 when the open fails, else the two out words receive (+0x40 - first + +4) and (+0x30 - +8 + +0x40) and 0 is returned; the inlined destructor stores the base vtable and closes the file (@ 0x00C426E0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C347B0, oracle length 177 bytes. Built under /EHsc.
// Retail bytes:  ... sub esp,48h ; xor eax,eax ; push esi ; mov esi,ecx ; mov [esp+8],eax ; mov [esp+10h],eax ; mov dword ptr [esp+4],012B4FACh ; push edx ; push esi ; lea ecx,[esp+0Ch] ; mov [esp+5Ch],eax ; call Open ; test al,al ; mov dword ptr [esp+4],012B4F9Ch ; mov dword ptr [esp+54h],-1 ; jne ok ; lea ecx,[esp+4] ; call Close ; or eax,-1 ; ... ret 8 ; ok: mov eax,[esp+44h] ; mov edx,[esp+8] ; mov ecx,eax ; sub ecx,esi ; mov esi,[esp+0Ch] ; add ecx,edx ; mov edx,[esp+5Ch] ; mov [edx],ecx ; mov ecx,[esp+34h] ; mov edx,[esp+60h] ; sub ecx,esi ; add ecx,eax ; mov [edx],ecx ; lea ecx,[esp+4] ; call Close ; ... xor eax,eax ; ... ret 8
// The derived destructor (@ 0x00C34780, the unwind funclet's target) stores the base vtable and
// tail-jumps to Close; the base vtable store is hoisted above the branch. The manifest label is
// a description; the open callee's label (CCompressorZlib::ParseWaveHeader) is propagated.
struct CWADFile_00c347b0 {                 // vtable 0x012B4F9C
    virtual void v00();
    long m_offset;                         // +4 (zeroed)
    long m_8;
    long m_c;                              // +0xc (zeroed)
    unsigned char _pad[0x20];
    long m_30;                             // +0x30
    unsigned char _pad2[0xc];
    long m_40;                             // +0x40
    unsigned char _pad3[4];
    CWADFile_00c347b0() : m_offset(0), m_c(0) { }
    ~CWADFile_00c347b0() { Close(); }
    bool Open(void* first, void* second);  // 0x00C42850
    void Close();                          // 0x00C426E0
};

struct CChunkedFileChunk_00c347b0 : CWADFile_00c347b0 {   // vtable 0x012B4FAC
    ~CChunkedFileChunk_00c347b0() { }
};

long __fastcall ReadChunkExtent_00c347b0(void* first, void* second, long* outStart, long* outEnd)
{
    CChunkedFileChunk_00c347b0 chunk;
    if (!chunk.Open(first, second))
        return -1;
    *outStart = chunk.m_offset + (chunk.m_40 - (long)first);
    *outEnd = chunk.m_30 - chunk.m_8 + chunk.m_40;
    return 0;
}