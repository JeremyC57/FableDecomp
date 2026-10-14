// CTCHeroStats_00c3df10::CreateIndexBuffer() -- EH-framed: a 0xf0-byte buffer allocated through the class allocator (@ 0x00C3C360, state 0 until built; the matching class `operator delete` @ 0x00C33100 is the unwind) and built with whether the mesh at +4 has physics (a `bool`, @ 0x00C3EBA0; ctor @ 0x00C4E2A0 -- the `bool` argument puts the `this` move before the push); 0 when the allocation fails; stored at +0x18 and resized (@ 0x00C52840): a non-zero error is recorded in the global @ 0x0143BB48 once (reported through the `__stdcall` hook @ 0x0143BBD4 when set), the buffer released (@ 0x00C4E2D0) and deleted through its virtual destructor, and 0 returned; else the buffer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3DF10, oracle length 188 bytes. Built under /EHsc.
// Retail bytes:  ... push 0F0h ; mov edi,ecx ; call CIndexBuffer::operator new ; mov esi,eax ; add esp,4 ; mov [esp+8],esi ; test esi,esi ; mov dword ptr [esp+14h],0 ; je null ; mov ecx,[edi+4] ; call HasPhysicsMesh ; mov ecx,esi ; push eax ; call CIndexBuffer::CIndexBuffer ; mov esi,eax ; jmp built ; null: xor esi,esi ; built: test esi,esi ; mov dword ptr [esp+14h],-1 ; jne resize ; ... xor eax,eax ... ret ; resize: mov ecx,esi ; mov [edi+18h],esi ; call Resize ; test eax,eax ; je ok ; mov ecx,[g_error] ; test ecx,ecx ; jne drop ; mov ecx,[g_pfnReport] ; test ecx,ecx ; mov [g_error],eax ; je drop ; push eax ; call ecx ; drop: mov ecx,esi ; call Release ; mov eax,[esi] ; push 1 ; mov ecx,esi ; call [eax] ; xor esi,esi ; ok: ... mov eax,esi ... ret
// The manifest label (CreateSoundBankResizeAction) is propagated; the callee labels are too
// (the allocator is a global-hook wrapper, the release is a plain cleanup). Named by the shape.
struct CMesh_00c3df10 {
    bool HasPhysicsMesh();                 // 0x00C3EBA0
};

struct CIndexBuffer_00c3df10 {             // 0xf0 bytes
    virtual ~CIndexBuffer_00c3df10();      // slot 0 (scalar deleting destructor)
    unsigned char _body[0xec];
    CIndexBuffer_00c3df10(bool hasPhysics);   // 0x00C4E2A0
    long Resize();                         // 0x00C52840
    void Release();                        // 0x00C4E2D0
    static void* __cdecl operator new(unsigned int size);   // 0x00C3C360
    static void __cdecl operator delete(void* block);       // 0x00C33100
};

typedef void (__stdcall* PFNREPORT_00c3df10)(long error);
extern long g_lastError_0143bb48;          // 0x0143BB48
extern PFNREPORT_00c3df10 g_pfnReport_0143bbd4;   // 0x0143BBD4

struct CTCHeroStats_00c3df10 {
    long m_0;
    CMesh_00c3df10* m_pMesh;               // +4
    unsigned char _pad[0x10];
    CIndexBuffer_00c3df10* m_pBuffer;      // +0x18
    CIndexBuffer_00c3df10* CreateIndexBuffer();
};

CIndexBuffer_00c3df10* CTCHeroStats_00c3df10::CreateIndexBuffer()
{
    CIndexBuffer_00c3df10* buffer = new CIndexBuffer_00c3df10(m_pMesh->HasPhysicsMesh());
    if (!buffer)
        return 0;
    m_pBuffer = buffer;
    long error = buffer->Resize();
    if (error) {
        if (!g_lastError_0143bb48) {
            g_lastError_0143bb48 = error;
            if (g_pfnReport_0143bbd4)
                g_pfnReport_0143bbd4(error);
        }
        buffer->Release();
        delete buffer;
        buffer = 0;
    }
    return buffer;
}
