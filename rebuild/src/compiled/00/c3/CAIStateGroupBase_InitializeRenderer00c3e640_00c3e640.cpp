// CAIStateGroupBase_00c3e640::InitializeRenderer(IRenderer**) -- EH-framed: a renderer comes from the factory at +0x1d4 (virtual slot 6) when present, else a 0x824-byte one is allocated through the class allocator (@ 0x00C3C360, state 0, class `operator delete` @ 0x00C33100 is the unwind) and built (@ 0x00C54090); it is stored at +0x17c; -5 when none; a non-zero result of its virtual slot 1 (with `this`) releases it (slot 2), deletes it through its virtual destructor, clears +0x17c and is returned; else the out pointer receives it and 0 is returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3E640, oracle length 229 bytes. Built under /EHsc.
// Retail bytes:  ... mov ecx,[esi+1D4h] ; test ecx,ecx ; je alloc ; mov eax,[ecx] ; call [eax+18h] ; jmp store ; alloc: push 824h ; call CRenderer::operator new ; add esp,4 ; mov [esp+4],eax ; test eax,eax ; mov dword ptr [esp+10h],0 ; je null ; mov ecx,eax ; call CRenderer::CRenderer ; jmp built ; null: xor eax,eax ; built: mov dword ptr [esp+10h],-1 ; store: mov ecx,eax ; test ecx,ecx ; mov [esi+17Ch],eax ; jne init ; mov eax,-5 ; ... ret 4 ; init: mov edx,[ecx] ; push edi ; push esi ; call [edx+4] ; mov edi,eax ; test edi,edi ; je ok ; mov ecx,[esi+17Ch] ; mov eax,[ecx] ; call [eax+8] ; mov ecx,[esi+17Ch] ; test ecx,ecx ; je clear ; mov edx,[ecx] ; push 1 ; call [edx] ; clear: mov eax,edi ; pop edi ; mov dword ptr [esi+17Ch],0 ; ... ret 4 ; ok: mov eax,[esi+17Ch] ; mov ecx,[esp+1Ch] ; mov [ecx],eax ; ... xor eax,eax ... ret 4
// The renderer pointer is re-read from the member after each virtual call. Named from the
// manifest; the renderer is named by role.
struct CAIStateGroupBase_00c3e640;
struct IRenderer_00c3e640 {                // 0x824 bytes
    virtual ~IRenderer_00c3e640();             // slot 0 (scalar deleting destructor)
    virtual long Initialise(CAIStateGroupBase_00c3e640* owner);   // slot 1
    virtual void Release();                // slot 2
    unsigned char _body[0x820];
    IRenderer_00c3e640();                      // 0x00C54090
    static void* __cdecl operator new(unsigned int size);   // 0x00C3C360
    static void __cdecl operator delete(void* block);       // 0x00C33100
};

struct IRendererFactory_00c3e640 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual IRenderer_00c3e640* Create();      // slot 6
};

struct CAIStateGroupBase_00c3e640 {
    unsigned char _pad[0x17c];
    IRenderer_00c3e640* m_pRenderer;           // +0x17c
    unsigned char _pad2[0x54];
    IRendererFactory_00c3e640* m_pFactory;     // +0x1d4
    long InitializeRenderer(IRenderer_00c3e640** out);
};

long CAIStateGroupBase_00c3e640::InitializeRenderer(IRenderer_00c3e640** out)
{
    if (m_pFactory)
        m_pRenderer = m_pFactory->Create();
    else
        m_pRenderer = new IRenderer_00c3e640;
    if (!m_pRenderer)
        return -5;
    long result = m_pRenderer->Initialise(this);
    if (result) {
        m_pRenderer->Release();
        delete m_pRenderer;
        m_pRenderer = 0;
        return result;
    }
    *out = m_pRenderer;
    return 0;
}